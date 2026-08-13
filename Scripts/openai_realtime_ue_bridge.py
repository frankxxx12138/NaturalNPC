"""Localhost bridge between Unreal push-to-talk and OpenAI Realtime.

The OpenAI API key stays in this process environment. Unreal only talks to the
loopback HTTP endpoints exposed below, so the key is never stored in an asset.
"""

import argparse
import asyncio
import base64
import inspect
import json
import os
import threading
import time
import urllib.error
import urllib.request
import wave
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from io import BytesIO
from typing import Optional

import numpy as np
import sounddevice as sd
import websockets


SAMPLE_RATE = 24000
ACE_SAMPLE_RATE = 16000
CHANNELS = 1
SAMPLE_WIDTH = 2
SAMPLE_DTYPE = "int16"


def resample_pcm16_mono(pcm: bytes, source_rate: int, target_rate: int) -> bytes:
    if not pcm or source_rate == target_rate:
        return pcm
    samples = np.frombuffer(pcm, dtype=np.int16)
    if samples.size == 0:
        return pcm
    target_count = max(1, int(round(samples.size * target_rate / source_rate)))
    source_positions = np.arange(samples.size, dtype=np.float32)
    target_positions = np.linspace(
        0, samples.size - 1, target_count, dtype=np.float32
    )
    resampled = np.interp(
        target_positions, source_positions, samples.astype(np.float32)
    )
    return np.clip(resampled, -32768, 32767).astype(np.int16).tobytes()


def pcm16_to_wav(pcm: bytes, sample_rate: int) -> bytes:
    output = BytesIO()
    with wave.open(output, "wb") as wav:
        wav.setnchannels(CHANNELS)
        wav.setsampwidth(SAMPLE_WIDTH)
        wav.setframerate(sample_rate)
        wav.writeframes(pcm)
    return output.getvalue()


def audio_levels(pcm: bytes) -> tuple[float, int, float]:
    samples = np.frombuffer(pcm, dtype=np.int16)
    if samples.size == 0:
        return 0.0, 0, 0.0
    rms = float(np.sqrt(np.mean(samples.astype(np.float32) ** 2)))
    peak = int(np.max(np.abs(samples)))
    duration_ms = samples.size / SAMPLE_RATE * 1000.0
    return rms, peak, duration_ms


def transcription_config(language: str) -> dict:
    result = {"model": "gpt-4o-mini-transcribe"}
    if language.lower() not in {"", "auto", "detect", "none"}:
        result["language"] = language
    return result


class Recorder:
    def __init__(self, input_device: Optional[int], min_record_ms: int):
        self.input_device = input_device
        self.min_record_ms = min_record_ms
        self.chunks: list[bytes] = []
        self.stream = None
        self.started_at = 0.0
        self.lock = threading.Lock()

    def start(self) -> None:
        with self.lock:
            if self.stream is not None:
                return
            self.chunks = []
            self.started_at = time.perf_counter()

            def callback(indata, frames, time_info, status):
                del frames, time_info
                if status:
                    print(f"Realtime bridge microphone status: {status}", flush=True)
                with self.lock:
                    self.chunks.append(bytes(indata))

            self.stream = sd.RawInputStream(
                samplerate=SAMPLE_RATE,
                channels=CHANNELS,
                dtype=SAMPLE_DTYPE,
                device=self.input_device,
                blocksize=2048,
                callback=callback,
            )
            self.stream.start()

    def stop(self) -> bytes:
        min_seconds = max(0.0, self.min_record_ms / 1000.0)
        deadline = time.perf_counter() + 1.0
        while time.perf_counter() - self.started_at < min_seconds:
            if time.perf_counter() >= deadline:
                break
            time.sleep(0.01)

        with self.lock:
            stream = self.stream
            self.stream = None
        if stream is not None:
            stream.stop()
            stream.close()
        with self.lock:
            audio = b"".join(self.chunks)
            self.chunks = []
        return audio


class RealtimeClient:
    def __init__(self, args):
        self.args = args
        self.ws = None
        self.lock = asyncio.Lock()
        self.active_instructions = ""
        self.active_npc_id = ""
        self.active_voice = ""
        self.turn_counter = 0

    async def connect(self, voice: str = "") -> None:
        if self.ws is not None:
            return
        api_key = os.environ.get(self.args.api_key_env)
        if not api_key:
            raise RuntimeError(f"Missing {self.args.api_key_env}.")

        headers = {"Authorization": f"Bearer {api_key}"}
        if self.args.safety_identifier:
            headers["OpenAI-Safety-Identifier"] = self.args.safety_identifier
        kwargs = {
            "ping_interval": 20,
            "ping_timeout": 20,
            "max_size": 32 * 1024 * 1024,
        }
        signature = inspect.signature(websockets.connect)
        header_parameter = (
            "additional_headers"
            if "additional_headers" in signature.parameters
            else "extra_headers"
        )
        kwargs[header_parameter] = headers
        url = f"{self.args.url}?model={self.args.model}"
        self.ws = await websockets.connect(url, **kwargs)
        resolved_voice = (voice or self.args.voice).strip()
        await self.ws.send(
            json.dumps(
                {
                    "type": "session.update",
                    "session": {
                        "type": "realtime",
                        "model": self.args.model,
                        "instructions": self.args.instructions,
                        "output_modalities": ["audio"],
                        "audio": {
                            "input": {
                                "format": {
                                    "type": "audio/pcm",
                                    "rate": SAMPLE_RATE,
                                },
                                "turn_detection": None,
                                "transcription": transcription_config(
                                    self.args.language
                                ),
                            },
                            "output": {
                                "format": {
                                    "type": "audio/pcm",
                                    "rate": SAMPLE_RATE,
                                },
                                "voice": resolved_voice,
                                "speed": self.args.speed,
                            },
                        },
                        "max_output_tokens": self.args.max_output_tokens,
                    },
                }
            )
        )
        self.active_voice = resolved_voice

    async def reset(self) -> None:
        self.active_instructions = ""
        self.active_npc_id = ""
        self.active_voice = ""
        if self.ws is not None:
            try:
                await self.ws.close()
            finally:
                self.ws = None

    async def ask(
        self, pcm: bytes, instructions: str, npc_id: str, voice: str
    ) -> dict:
        async with self.lock:
            self.turn_counter += 1
            turn_id = self.turn_counter
            if self.args.reset_session_per_turn or (
                self.active_npc_id and self.active_npc_id != npc_id
            ) or (
                self.active_voice and self.active_voice != voice
            ):
                await self.reset()
            try:
                await self.connect(voice)
                result = await self._ask_connected(pcm, instructions, npc_id)
            except websockets.ConnectionClosed:
                await self.reset()
                await self.connect(voice)
                result = await self._ask_connected(pcm, instructions, npc_id)
            finally:
                if self.args.reset_session_per_turn:
                    await self.reset()
            result["turn_id"] = turn_id
            return result

    async def say(
        self, text: str, instructions: str, npc_id: str, voice: str
    ) -> dict:
        async with self.lock:
            self.turn_counter += 1
            turn_id = self.turn_counter
            await self.reset()
            await self.connect(voice)
            try:
                await self._update_identity(instructions, npc_id)
                await self.ws.send(
                    json.dumps(
                        {
                            "type": "response.create",
                            "response": {
                                "instructions": (
                                    "Say exactly this line, with no extra words: "
                                    + text
                                )
                            },
                        }
                    )
                )
                result = await self._collect_response(include_user_text=False)
                result["reply_text"] = result.get("reply_text") or text
            finally:
                await self.reset()
            result["turn_id"] = turn_id
            return result

    async def respond(
        self, text: str, instructions: str, npc_id: str, voice: str
    ) -> dict:
        async with self.lock:
            self.turn_counter += 1
            turn_id = self.turn_counter
            await self.reset()
            await self.connect(voice)
            try:
                await self._update_identity(instructions, npc_id)
                await self.ws.send(
                    json.dumps(
                        {
                            "type": "conversation.item.create",
                            "item": {
                                "type": "message",
                                "role": "user",
                                "content": [
                                    {"type": "input_text", "text": text}
                                ],
                            },
                        }
                    )
                )
                await self.ws.send(json.dumps({"type": "response.create"}))
                result = await self._collect_response(include_user_text=False)
            finally:
                await self.reset()
            result["turn_id"] = turn_id
            return result

    async def _update_identity(self, instructions: str, npc_id: str) -> None:
        if instructions and instructions != self.active_instructions:
            await self.ws.send(
                json.dumps(
                    {
                        "type": "session.update",
                        "session": {"type": "realtime", "instructions": instructions},
                    }
                )
            )
            self.active_instructions = instructions
        self.active_npc_id = npc_id

    async def _ask_connected(
        self, pcm: bytes, instructions: str, npc_id: str
    ) -> dict:
        await self._update_identity(instructions, npc_id)
        await self.ws.send(json.dumps({"type": "input_audio_buffer.clear"}))
        await self.ws.send(
            json.dumps(
                {
                    "type": "input_audio_buffer.append",
                    "audio": base64.b64encode(pcm).decode("ascii"),
                }
            )
        )
        await self.ws.send(json.dumps({"type": "input_audio_buffer.commit"}))
        await self.ws.send(json.dumps({"type": "response.create"}))
        return await self._collect_response(include_user_text=True)

    async def _collect_response(self, include_user_text: bool) -> dict:
        started = time.perf_counter()
        reply_pcm = bytearray()
        reply_transcript: list[str] = []
        user_transcript = ""
        active_response_id = ""
        while True:
            event = json.loads(await self.ws.recv())
            event_type = event.get("type", "")
            response_id = event.get("response_id", "")
            response = event.get("response", {})
            if isinstance(response, dict) and not response_id:
                response_id = response.get("id", "")
            if event_type == "response.created" and response_id:
                active_response_id = response_id
                continue
            if response_id and active_response_id and response_id != active_response_id:
                continue
            if event_type == "response.output_audio.delta":
                reply_pcm.extend(base64.b64decode(event.get("delta", "")))
            elif event_type == "response.output_audio_transcript.delta":
                reply_transcript.append(event.get("delta", ""))
            elif event_type == "response.output_audio_transcript.done":
                transcript = event.get("transcript", "")
                if transcript:
                    reply_transcript = [transcript]
            elif (
                include_user_text
                and event_type
                == "conversation.item.input_audio_transcription.completed"
            ):
                user_transcript = event.get("transcript", "")
            elif event_type == "error":
                raise RuntimeError(json.dumps(event, ensure_ascii=False))
            elif event_type == "response.done":
                break

        ace_pcm = resample_pcm16_mono(
            bytes(reply_pcm), SAMPLE_RATE, ACE_SAMPLE_RATE
        )
        wav = pcm16_to_wav(ace_pcm, ACE_SAMPLE_RATE)
        return {
            "ok": True,
            "user_text": user_transcript.strip(),
            "reply_text": "".join(reply_transcript).strip(),
            "audio_wav_base64": base64.b64encode(wav).decode("ascii"),
            "realtime_ms": round((time.perf_counter() - started) * 1000, 1),
            "audio_bytes": len(wav),
            "audio_sample_rate": ACE_SAMPLE_RATE,
        }


class BridgeState:
    def __init__(self, args):
        self.args = args
        self.recorder = Recorder(args.input_device, args.min_record_ms)
        self.loop = asyncio.new_event_loop()
        self.client = RealtimeClient(args)
        self.say_client = RealtimeClient(args)
        self.thread = threading.Thread(target=self._run_loop, daemon=True)
        self.instructions = args.instructions
        self.npc_id = ""
        self.memory_file = ""
        self.voice = args.voice
        self.metadata_lock = threading.Lock()
        self.thread.start()

    def _run_loop(self) -> None:
        asyncio.set_event_loop(self.loop)
        self.loop.run_forever()

    def set_metadata(self, body: dict) -> None:
        with self.metadata_lock:
            self.instructions = body.get("instructions") or self.instructions
            self.npc_id = str(body.get("npc_id") or "")
            self.memory_file = str(body.get("memory_file") or "")
            self.voice = str(body.get("voice") or self.args.voice)

    def ask(self, pcm: bytes) -> dict:
        with self.metadata_lock:
            instructions = self.instructions
            npc_id = self.npc_id
            memory_file = self.memory_file
            voice = self.voice
        future = asyncio.run_coroutine_threadsafe(
            self.client.ask(pcm, instructions, npc_id, voice), self.loop
        )
        try:
            result = future.result(timeout=self.args.timeout_seconds)
        except Exception:
            future.cancel()
            raise
        result.update(
            {"npc_id": npc_id, "memory_file": memory_file, "voice": voice}
        )
        return result

    def say(self, body: dict) -> dict:
        text = str(body.get("text") or "").strip()
        if not text:
            raise ValueError("Missing text.")
        instructions = body.get("instructions") or self.instructions
        npc_id = str(body.get("npc_id") or "")
        voice = str(body.get("voice") or self.args.voice)
        future = asyncio.run_coroutine_threadsafe(
            self.say_client.say(text, instructions, npc_id, voice), self.loop
        )
        try:
            return future.result(timeout=self.args.timeout_seconds)
        except Exception:
            future.cancel()
            raise

    def respond(self, body: dict) -> dict:
        text = str(body.get("text") or "").strip()
        if not text:
            raise ValueError("Missing text.")
        instructions = body.get("instructions") or self.instructions
        npc_id = str(body.get("npc_id") or "")
        voice = str(body.get("voice") or self.args.voice)
        future = asyncio.run_coroutine_threadsafe(
            self.say_client.respond(text, instructions, npc_id, voice), self.loop
        )
        try:
            result = future.result(timeout=self.args.timeout_seconds)
        except Exception:
            future.cancel()
            raise
        result.update({"npc_id": npc_id, "voice": voice})
        return result

    def chat_completions(self, body: dict) -> dict:
        api_key = os.environ.get(self.args.api_key_env)
        if not api_key:
            raise RuntimeError(f"Missing {self.args.api_key_env}.")
        request = urllib.request.Request(
            self.args.chat_url,
            data=json.dumps(body).encode("utf-8"),
            headers={
                "Authorization": f"Bearer {api_key}",
                "Content-Type": "application/json",
            },
            method="POST",
        )
        try:
            with urllib.request.urlopen(
                request, timeout=self.args.timeout_seconds
            ) as response:
                return json.loads(response.read().decode("utf-8"))
        except urllib.error.HTTPError as exc:
            detail = exc.read().decode("utf-8", errors="replace")
            raise RuntimeError(
                f"OpenAI chat failed with HTTP {exc.code}: {detail}"
            ) from exc

    def reset(self) -> None:
        for client in (self.client, self.say_client):
            future = asyncio.run_coroutine_threadsafe(client.reset(), self.loop)
            future.result(timeout=10)


class Handler(BaseHTTPRequestHandler):
    server_version = "NaturalNPCOpenAIRealtimeBridge/4.0"

    def do_GET(self):
        if self.path.startswith("/health"):
            self.send_json(
                {
                    "ok": True,
                    "bridge_version": 4,
                    "features": [
                        "dynamic_voice",
                        "rerouted_response",
                        "chat_proxy",
                    ],
                    "model": STATE.args.model,
                    "voice": STATE.args.voice,
                    "language": STATE.args.language,
                    "api_key_configured": bool(
                        os.environ.get(STATE.args.api_key_env)
                    ),
                }
            )
        elif self.path.startswith("/devices"):
            self.send_json({"ok": True, "devices": str(sd.query_devices())})
        else:
            self.send_error(404)

    def do_POST(self):
        try:
            if self.path.startswith("/realtime/start"):
                STATE.set_metadata(self.read_json())
                STATE.recorder.start()
                self.send_json({"ok": True})
            elif self.path.startswith("/realtime/stop"):
                pcm = STATE.recorder.stop()
                rms, peak, duration_ms = audio_levels(pcm)
                if duration_ms < STATE.args.min_record_ms:
                    self.send_json(
                        {
                            "ok": False,
                            "skipped": True,
                            "reason": "too_short",
                            "duration_ms": duration_ms,
                        }
                    )
                    return
                if rms < STATE.args.min_rms and peak < STATE.args.min_peak:
                    self.send_json(
                        {
                            "ok": False,
                            "skipped": True,
                            "reason": "silence",
                            "duration_ms": duration_ms,
                            "rms": rms,
                            "peak": peak,
                        }
                    )
                    return
                result = STATE.ask(pcm)
                result.update(
                    {"input_ms": duration_ms, "input_rms": rms, "input_peak": peak}
                )
                print(
                    f"Realtime turn={result.get('turn_id')} "
                    f"npc={result.get('npc_id')} voice={result.get('voice')} "
                    f"input_ms={duration_ms:.1f} "
                    f"reply_chars={len(result.get('reply_text', ''))}",
                    flush=True,
                )
                self.send_json(result)
            elif self.path.startswith("/realtime/say"):
                self.send_json(STATE.say(self.read_json()))
            elif self.path.startswith("/realtime/respond"):
                self.send_json(STATE.respond(self.read_json()))
            elif self.path.startswith("/chat/completions"):
                self.send_json(STATE.chat_completions(self.read_json()))
            elif self.path.startswith("/realtime/reset"):
                STATE.reset()
                self.send_json({"ok": True})
            else:
                self.send_error(404)
        except Exception as exc:
            self.send_json({"ok": False, "error": str(exc)}, status=500)

    def read_json(self) -> dict:
        length = int(self.headers.get("Content-Length", "0") or "0")
        if length <= 0:
            return {}
        return json.loads(self.rfile.read(length).decode("utf-8"))

    def send_json(self, payload: dict, status: int = 200) -> None:
        raw = json.dumps(payload, ensure_ascii=False).encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(raw)))
        self.end_headers()
        self.wfile.write(raw)

    def log_message(self, fmt, *args):
        print(f"{self.address_string()} - {fmt % args}", flush=True)


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser()
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--port", type=int, default=8040)
    parser.add_argument("--model", default="gpt-realtime-2.1")
    parser.add_argument("--voice", default="marin")
    parser.add_argument("--speed", type=float, default=1.0)
    parser.add_argument("--language", default="en")
    parser.add_argument("--api-key-env", default="OPENAI_API_KEY")
    parser.add_argument("--url", default="wss://api.openai.com/v1/realtime")
    parser.add_argument(
        "--chat-url", default="https://api.openai.com/v1/chat/completions"
    )
    parser.add_argument("--safety-identifier", default="")
    parser.add_argument("--input-device", type=int, default=None)
    parser.add_argument("--min-record-ms", type=int, default=350)
    parser.add_argument("--min-rms", type=float, default=350.0)
    parser.add_argument("--min-peak", type=int, default=1200)
    parser.add_argument("--timeout-seconds", type=float, default=60.0)
    parser.add_argument("--max-output-tokens", type=int, default=220)
    parser.add_argument(
        "--reset-session-per-turn",
        action=argparse.BooleanOptionalAction,
        default=False,
    )
    parser.add_argument(
        "--instructions",
        default=(
            "You are a realtime VR NPC. Reply naturally in one or two short "
            "spoken sentences. Never describe system behavior."
        ),
    )
    return parser


STATE = None


def main() -> None:
    global STATE
    args = build_parser().parse_args()
    if not os.environ.get(args.api_key_env):
        raise RuntimeError(f"Missing {args.api_key_env}.")
    STATE = BridgeState(args)
    print(
        f"OpenAI Realtime UE bridge ready on http://{args.host}:{args.port} "
        f"model={args.model} voice={args.voice} language={args.language}",
        flush=True,
    )
    ThreadingHTTPServer((args.host, args.port), Handler).serve_forever()


if __name__ == "__main__":
    main()
