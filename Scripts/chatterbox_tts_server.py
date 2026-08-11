import argparse
import hashlib
import json
import multiprocessing
import os
import threading
import time
import wave
from collections import OrderedDict
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from io import BytesIO


MODEL_KIND = "multilingual"
DEVICE = "cuda"
T3_MODEL = "v3"
CACHE_DIR = ""
IDLE_UNLOAD_SECONDS = 60.0
WORKER_REQUEST_TIMEOUT_SECONDS = 180.0
MAX_MEMORY_VOICE_CONDITIONS = 4
VOICE_CONDITION_CACHE_VERSION = 1

# HTTP requests may arrive on different threads, but Chatterbox/PyTorch must
# never enter generate concurrently. The model lives in a disposable worker so
# terminating that worker returns PyTorch allocator and DLL memory to Windows.
WORKER_LOCK = threading.Lock()
WORKER_PROCESS = None
WORKER_CONNECTION = None


def language_code_to_id(language_code):
    if not language_code:
        return "zh"
    language_code = language_code.lower()
    if language_code.startswith("zh"):
        return "zh"
    return language_code.split("-")[0]


def tensor_to_wav_bytes(wav, sample_rate):
    import numpy as np
    import torch

    if isinstance(wav, torch.Tensor):
        wav = wav.detach().cpu()
        if wav.ndim > 1:
            wav = wav.squeeze(0)
        audio = wav.numpy()
    else:
        audio = np.asarray(wav)

    audio = np.clip(audio, -1.0, 1.0)
    pcm = (audio * 32767.0).astype(np.int16)

    output = BytesIO()
    with wave.open(output, "wb") as wav_file:
        wav_file.setnchannels(1)
        wav_file.setsampwidth(2)
        wav_file.setframerate(int(sample_rate))
        wav_file.writeframes(pcm.tobytes())
    return output.getvalue()


def load_model(model_kind, device):
    if model_kind == "turbo":
        from chatterbox.tts_turbo import ChatterboxTurboTTS

        return ChatterboxTurboTTS.from_pretrained(device=device)
    if model_kind == "english":
        from chatterbox.tts import ChatterboxTTS

        return ChatterboxTTS.from_pretrained(device=device)

    import torch
    from chatterbox.mtl_tts import ChatterboxMultilingualTTS

    return ChatterboxMultilingualTTS.from_pretrained(
        device=torch.device(device),
    )


def get_conditionals_class(model_kind):
    if model_kind == "turbo":
        from chatterbox.tts_turbo import Conditionals

        return Conditionals
    if model_kind == "english":
        from chatterbox.tts import Conditionals

        return Conditionals

    from chatterbox.mtl_tts import Conditionals

    return Conditionals


def get_voice_condition_key(model_kind, audio_prompt_path, generate_kwargs):
    resolved_path = os.path.normcase(os.path.realpath(audio_prompt_path))
    stat = os.stat(resolved_path)
    default_exaggeration = 0.0 if model_kind == "turbo" else 0.5
    key_payload = {
        "version": VOICE_CONDITION_CACHE_VERSION,
        "model_kind": model_kind,
        "audio_prompt_path": resolved_path,
        "audio_prompt_size": stat.st_size,
        "audio_prompt_mtime_ns": stat.st_mtime_ns,
        "exaggeration": float(
            generate_kwargs.get("exaggeration", default_exaggeration)
        ),
    }
    if model_kind == "turbo":
        key_payload["norm_loudness"] = bool(
            generate_kwargs.get("norm_loudness", True)
        )
    return hashlib.sha256(
        json.dumps(key_payload, sort_keys=True, ensure_ascii=False).encode("utf-8")
    ).hexdigest()


def prepare_voice_condition(model, model_kind, audio_prompt_path, generate_kwargs):
    default_exaggeration = 0.0 if model_kind == "turbo" else 0.5
    exaggeration = float(
        generate_kwargs.get("exaggeration", default_exaggeration)
    )
    if model_kind == "turbo":
        model.prepare_conditionals(
            audio_prompt_path,
            exaggeration=exaggeration,
            norm_loudness=bool(generate_kwargs.get("norm_loudness", True)),
        )
    else:
        model.prepare_conditionals(
            audio_prompt_path,
            exaggeration=exaggeration,
        )
    return model.conds


def save_voice_condition(condition, cache_path):
    os.makedirs(os.path.dirname(cache_path), exist_ok=True)
    temporary_path = f"{cache_path}.{os.getpid()}.tmp"
    try:
        condition.save(temporary_path)
        os.replace(temporary_path, cache_path)
    finally:
        if os.path.exists(temporary_path):
            try:
                os.remove(temporary_path)
            except OSError:
                pass


def get_voice_condition(
    model,
    model_kind,
    device,
    audio_prompt_path,
    generate_kwargs,
    memory_cache,
    cache_dir,
    max_memory_entries,
):
    key = get_voice_condition_key(
        model_kind,
        audio_prompt_path,
        generate_kwargs,
    )
    if key in memory_cache:
        condition = memory_cache.pop(key)
        memory_cache[key] = condition
        return condition, "memory"

    cache_path = os.path.join(cache_dir, f"{key}.pt") if cache_dir else ""
    condition = None
    cache_source = "analyzed"
    if cache_path and os.path.exists(cache_path):
        try:
            conditionals_class = get_conditionals_class(model_kind)
            condition = conditionals_class.load(
                cache_path,
                map_location="cpu",
            ).to(device)
            cache_source = "disk"
        except Exception as exc:
            print(
                f"TTS voice condition disk cache ignored path={cache_path!r} "
                f"error={exc}",
                flush=True,
            )

    if condition is None:
        condition = prepare_voice_condition(
            model,
            model_kind,
            audio_prompt_path,
            generate_kwargs,
        )
        if cache_path:
            try:
                save_voice_condition(condition, cache_path)
            except Exception as exc:
                print(
                    f"TTS voice condition disk cache write failed "
                    f"path={cache_path!r} error={exc}",
                    flush=True,
                )

    if max_memory_entries > 0:
        memory_cache[key] = condition
        while len(memory_cache) > max_memory_entries:
            memory_cache.popitem(last=False)
    return condition, cache_source


def tts_worker_main(
    connection,
    model_kind,
    device,
    idle_unload_seconds,
    voice_condition_cache_dir,
    max_memory_voice_conditions,
):
    """Own all heavy ML imports and exit completely after an idle period."""
    model = None
    voice_condition_cache = OrderedDict()
    try:
        while True:
            if idle_unload_seconds > 0.0:
                if not connection.poll(idle_unload_seconds):
                    print(
                        f"TTS worker exiting after {idle_unload_seconds:.0f}s idle",
                        flush=True,
                    )
                    return
            request = connection.recv()
            if request is None:
                return

            try:
                if model is None:
                    model = load_model(model_kind, device)
                    print("TTS worker model loaded", flush=True)

                import torch

                generate_kwargs = dict(request["kwargs"])
                audio_prompt_path = generate_kwargs.pop("audio_prompt_path", "")
                condition_cache_source = "builtin"
                if audio_prompt_path:
                    model.conds, condition_cache_source = get_voice_condition(
                        model,
                        model_kind,
                        device,
                        audio_prompt_path,
                        generate_kwargs,
                        voice_condition_cache,
                        voice_condition_cache_dir,
                        max_memory_voice_conditions,
                    )

                with torch.inference_mode():
                    wav = model.generate(request["text"], **generate_kwargs)
                wav_bytes = tensor_to_wav_bytes(wav, model.sr)
                del wav
                connection.send(
                    {
                        "ok": True,
                        "wav_bytes": wav_bytes,
                        "voice_condition_cache": condition_cache_source,
                    }
                )
            except Exception as exc:
                connection.send({"ok": False, "error": str(exc)})
    except (EOFError, BrokenPipeError, ConnectionResetError):
        return
    finally:
        try:
            connection.close()
        except Exception:
            pass


def stop_worker_locked(force):
    global WORKER_PROCESS, WORKER_CONNECTION

    connection = WORKER_CONNECTION
    process = WORKER_PROCESS
    WORKER_CONNECTION = None
    WORKER_PROCESS = None

    if connection is not None:
        if not force:
            try:
                connection.send(None)
            except Exception:
                pass
        try:
            connection.close()
        except Exception:
            pass

    if process is not None:
        if force and process.is_alive():
            process.terminate()
        process.join(timeout=5.0)
        if process.is_alive():
            process.kill()
            process.join(timeout=2.0)


def ensure_worker_locked():
    global WORKER_PROCESS, WORKER_CONNECTION

    if WORKER_PROCESS is not None and WORKER_PROCESS.is_alive():
        return
    if WORKER_PROCESS is not None or WORKER_CONNECTION is not None:
        stop_worker_locked(force=False)

    context = multiprocessing.get_context("spawn")
    parent_connection, child_connection = context.Pipe(duplex=True)
    process = context.Process(
        target=tts_worker_main,
        args=(
            child_connection,
            MODEL_KIND,
            DEVICE,
            IDLE_UNLOAD_SECONDS,
            os.path.join(CACHE_DIR, "VoiceConditionals") if CACHE_DIR else "",
            MAX_MEMORY_VOICE_CONDITIONS,
        ),
        name="ChatterboxTTSWorker",
        daemon=True,
    )
    process.start()
    child_connection.close()
    WORKER_CONNECTION = parent_connection
    WORKER_PROCESS = process
    print(f"TTS worker started pid={process.pid}", flush=True)


def is_worker_loaded():
    with WORKER_LOCK:
        if WORKER_PROCESS is None:
            return False
        if WORKER_PROCESS.is_alive():
            return True
        stop_worker_locked(force=False)
        return False


def generate_with_worker(text, kwargs):
    with WORKER_LOCK:
        ensure_worker_locked()
        try:
            WORKER_CONNECTION.send({"text": text, "kwargs": kwargs})
            if not WORKER_CONNECTION.poll(WORKER_REQUEST_TIMEOUT_SECONDS):
                raise TimeoutError(
                    "Chatterbox worker exceeded the inference timeout"
                )
            response = WORKER_CONNECTION.recv()
        except Exception:
            stop_worker_locked(force=True)
            raise

        if not response.get("ok"):
            error = response.get("error", "Unknown Chatterbox worker error")
            # CUDA failures can leave the runtime poisoned. Recreate the worker
            # for the next request instead of retaining a bad model instance.
            stop_worker_locked(force=True)
            raise RuntimeError(error)
        return (
            response["wav_bytes"],
            response.get("voice_condition_cache", "unknown"),
        )


def resolve_audio_prompt_path(payload):
    audio_prompt_path = str(payload.get("audio_prompt_path", "")).strip()
    voice = str(payload.get("voice", "")).strip()
    if not audio_prompt_path and voice and os.path.exists(voice):
        audio_prompt_path = voice
    if not audio_prompt_path:
        audio_prompt_path = os.environ.get("CHATTERBOX_AUDIO_PROMPT", "")
    return audio_prompt_path


def get_cache_path(payload, audio_prompt_path):
    if not CACHE_DIR:
        return ""
    os.makedirs(CACHE_DIR, exist_ok=True)
    resolved_audio_prompt_path = ""
    audio_prompt_size = None
    audio_prompt_mtime_ns = None
    if audio_prompt_path:
        resolved_audio_prompt_path = os.path.normcase(
            os.path.realpath(audio_prompt_path)
        )
        if os.path.exists(resolved_audio_prompt_path):
            stat = os.stat(resolved_audio_prompt_path)
            audio_prompt_size = stat.st_size
            audio_prompt_mtime_ns = stat.st_mtime_ns
    key_payload = {
        "model_kind": MODEL_KIND,
        "text": str(payload.get("text", "")).strip(),
        "voice": str(payload.get("voice", "")).strip(),
        "language_code": str(payload.get("language_code", "")).strip(),
        "audio_prompt_path": resolved_audio_prompt_path,
        "audio_prompt_size": audio_prompt_size,
        "audio_prompt_mtime_ns": audio_prompt_mtime_ns,
    }
    key = hashlib.sha256(
        json.dumps(key_payload, sort_keys=True, ensure_ascii=False).encode("utf-8")
    ).hexdigest()
    return os.path.join(CACHE_DIR, f"{key}.wav")


class ChatterboxHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path != "/health":
            self.send_error(404, "Use GET /health or POST /tts")
            return

        body = json.dumps(
            {
                "ok": True,
                "model_kind": MODEL_KIND,
                "device": DEVICE,
                "model_loaded": is_worker_loaded(),
            }
        ).encode("utf-8")
        self.send_response(200)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def do_POST(self):
        if self.path != "/tts":
            self.send_error(404, "Use POST /tts")
            return

        try:
            length = int(self.headers.get("Content-Length", "0"))
            payload = json.loads(self.rfile.read(length).decode("utf-8"))
            text = str(payload.get("text", "")).strip()
            if not text:
                raise ValueError("Missing text")

            start = time.perf_counter()
            audio_prompt_path = resolve_audio_prompt_path(payload)
            cache_path = get_cache_path(payload, audio_prompt_path)
            if cache_path and os.path.exists(cache_path):
                with open(cache_path, "rb") as cached_file:
                    wav_bytes = cached_file.read()
                self._send_wav(wav_bytes)
                print(
                    f"TTS cache_hit model={MODEL_KIND} bytes={len(wav_bytes)} "
                    f"ms={(time.perf_counter() - start) * 1000.0:.1f} "
                    f"text={text[:40]!r}",
                    flush=True,
                )
                return

            kwargs = {}
            if audio_prompt_path:
                kwargs["audio_prompt_path"] = audio_prompt_path
            if MODEL_KIND == "multilingual":
                kwargs["language_id"] = language_code_to_id(
                    payload.get("language_code", "en-US")
                )

            wav_bytes, condition_cache_source = generate_with_worker(text, kwargs)
            if cache_path:
                with open(cache_path, "wb") as cached_file:
                    cached_file.write(wav_bytes)

            print(
                f"TTS generated model={MODEL_KIND} bytes={len(wav_bytes)} "
                f"ms={(time.perf_counter() - start) * 1000.0:.1f} "
                f"voice_condition_cache={condition_cache_source} "
                f"text={text[:40]!r}",
                flush=True,
            )
            self._send_wav(wav_bytes)
        except (BrokenPipeError, ConnectionAbortedError, ConnectionResetError):
            print("TTS client disconnected before the response completed", flush=True)
        except Exception as exc:
            self._send_error_json(exc)

    def _send_wav(self, wav_bytes):
        self.send_response(200)
        self.send_header("Content-Type", "audio/wav")
        self.send_header("Content-Length", str(len(wav_bytes)))
        self.end_headers()
        self.wfile.write(wav_bytes)

    def _send_error_json(self, exc):
        body = json.dumps({"error": str(exc)}, ensure_ascii=False).encode("utf-8")
        try:
            self.send_response(500)
            self.send_header("Content-Type", "application/json; charset=utf-8")
            self.send_header("Content-Length", str(len(body)))
            self.end_headers()
            self.wfile.write(body)
        except (BrokenPipeError, ConnectionAbortedError, ConnectionResetError):
            print("TTS client disconnected while reporting an error", flush=True)

    def log_message(self, fmt, *args):
        print("%s - %s" % (self.address_string(), fmt % args), flush=True)


def main():
    global DEVICE, MODEL_KIND, T3_MODEL, CACHE_DIR
    global IDLE_UNLOAD_SECONDS, WORKER_REQUEST_TIMEOUT_SECONDS
    global MAX_MEMORY_VOICE_CONDITIONS

    parser = argparse.ArgumentParser()
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--port", type=int, default=8020)
    parser.add_argument("--device", default="cuda")
    parser.add_argument(
        "--model-kind",
        choices=["multilingual", "turbo", "english"],
        default="multilingual",
    )
    parser.add_argument("--t3-model", default="v3")
    parser.add_argument(
        "--cache-dir",
        default=os.path.join("Saved", "TTSCache", "Chatterbox"),
    )
    parser.add_argument("--prewarm-text", default="")
    parser.add_argument("--idle-unload-seconds", type=float, default=60.0)
    parser.add_argument("--worker-timeout-seconds", type=float, default=180.0)
    parser.add_argument("--max-memory-voice-conditions", type=int, default=4)
    args = parser.parse_args()

    DEVICE = args.device
    MODEL_KIND = args.model_kind
    T3_MODEL = args.t3_model
    CACHE_DIR = args.cache_dir
    IDLE_UNLOAD_SECONDS = max(0.0, args.idle_unload_seconds)
    WORKER_REQUEST_TIMEOUT_SECONDS = max(10.0, args.worker_timeout_seconds)
    MAX_MEMORY_VOICE_CONDITIONS = max(0, args.max_memory_voice_conditions)

    server = ThreadingHTTPServer((args.host, args.port), ChatterboxHandler)
    server.daemon_threads = True
    print(
        f"Chatterbox TTS server ready on http://{args.host}:{args.port}/tts "
        f"model={MODEL_KIND} device={DEVICE}",
        flush=True,
    )
    if args.prewarm_text:
        print(f"Prewarming Chatterbox with {args.prewarm_text!r}...", flush=True)
        kwargs = {"language_id": "zh"} if MODEL_KIND == "multilingual" else {}
        start = time.perf_counter()
        wav_bytes, _condition_cache_source = generate_with_worker(
            args.prewarm_text,
            kwargs,
        )
        print(
            f"Prewarm complete bytes={len(wav_bytes)} "
            f"ms={(time.perf_counter() - start) * 1000.0:.1f}",
            flush=True,
        )
    try:
        server.serve_forever()
    finally:
        with WORKER_LOCK:
            stop_worker_locked(force=True)


if __name__ == "__main__":
    multiprocessing.freeze_support()
    main()
