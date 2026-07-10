import argparse
import hashlib
import json
import os
import time
import wave
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from io import BytesIO


MODEL = None
MODEL_KIND = "multilingual"
DEVICE = "cuda"
T3_MODEL = "v3"
CACHE_DIR = ""


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


def get_cache_path(payload):
    if not CACHE_DIR:
        return ""
    os.makedirs(CACHE_DIR, exist_ok=True)
    key_payload = {
        "model_kind": MODEL_KIND,
        "text": str(payload.get("text", "")).strip(),
        "voice": str(payload.get("voice", "")).strip(),
        "language_code": str(payload.get("language_code", "")).strip(),
        "audio_prompt_path": str(payload.get("audio_prompt_path", "")).strip(),
    }
    key = hashlib.sha256(
        json.dumps(key_payload, sort_keys=True, ensure_ascii=False).encode("utf-8")
    ).hexdigest()
    return os.path.join(CACHE_DIR, f"{key}.wav")


def get_model():
    global MODEL
    if MODEL is not None:
        return MODEL

    if MODEL_KIND == "turbo":
        from chatterbox.tts_turbo import ChatterboxTurboTTS

        MODEL = ChatterboxTurboTTS.from_pretrained(device=DEVICE)
    elif MODEL_KIND == "english":
        from chatterbox.tts import ChatterboxTTS

        MODEL = ChatterboxTTS.from_pretrained(device=DEVICE)
    else:
        import torch
        from chatterbox.mtl_tts import ChatterboxMultilingualTTS

        MODEL = ChatterboxMultilingualTTS.from_pretrained(
            device=torch.device(DEVICE),
        )
    return MODEL


class ChatterboxHandler(BaseHTTPRequestHandler):
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
            cache_path = get_cache_path(payload)
            if cache_path and os.path.exists(cache_path):
                with open(cache_path, "rb") as cached_file:
                    wav_bytes = cached_file.read()
                self.send_response(200)
                self.send_header("Content-Type", "audio/wav")
                self.send_header("Content-Length", str(len(wav_bytes)))
                self.end_headers()
                self.wfile.write(wav_bytes)
                print(
                    f"TTS cache_hit model={MODEL_KIND} bytes={len(wav_bytes)} "
                    f"ms={(time.perf_counter() - start) * 1000.0:.1f} text={text[:40]!r}",
                    flush=True,
                )
                return

            model = get_model()
            voice = str(payload.get("voice", "")).strip()
            audio_prompt_path = str(payload.get("audio_prompt_path", "")).strip()
            if not audio_prompt_path and voice and os.path.exists(voice):
                audio_prompt_path = voice
            if not audio_prompt_path:
                audio_prompt_path = os.environ.get("CHATTERBOX_AUDIO_PROMPT", "")

            kwargs = {}
            if audio_prompt_path:
                kwargs["audio_prompt_path"] = audio_prompt_path

            if MODEL_KIND == "multilingual":
                kwargs["language_id"] = language_code_to_id(
                    payload.get("language_code", "en-US")
                )

            wav = model.generate(text, **kwargs)
            wav_bytes = tensor_to_wav_bytes(wav, model.sr)
            if cache_path:
                with open(cache_path, "wb") as cached_file:
                    cached_file.write(wav_bytes)

            print(
                f"TTS generated model={MODEL_KIND} bytes={len(wav_bytes)} "
                f"ms={(time.perf_counter() - start) * 1000.0:.1f} text={text[:40]!r}",
                flush=True,
            )

            self.send_response(200)
            self.send_header("Content-Type", "audio/wav")
            self.send_header("Content-Length", str(len(wav_bytes)))
            self.end_headers()
            self.wfile.write(wav_bytes)
        except Exception as exc:
            body = json.dumps({"error": str(exc)}, ensure_ascii=False).encode("utf-8")
            self.send_response(500)
            self.send_header("Content-Type", "application/json; charset=utf-8")
            self.send_header("Content-Length", str(len(body)))
            self.end_headers()
            self.wfile.write(body)

    def log_message(self, fmt, *args):
        print("%s - %s" % (self.address_string(), fmt % args))


def main():
    global DEVICE, MODEL_KIND, T3_MODEL, CACHE_DIR

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
    parser.add_argument("--prewarm-text", default="Sure.")
    args = parser.parse_args()

    DEVICE = args.device
    MODEL_KIND = args.model_kind
    T3_MODEL = args.t3_model
    CACHE_DIR = args.cache_dir

    server = ThreadingHTTPServer((args.host, args.port), ChatterboxHandler)
    print(
        f"Chatterbox TTS server ready on http://{args.host}:{args.port}/tts "
        f"model={MODEL_KIND} device={DEVICE}",
        flush=True,
    )
    if args.prewarm_text:
        print(f"Prewarming Chatterbox with {args.prewarm_text!r}...", flush=True)
        model = get_model()
        kwargs = {}
        if MODEL_KIND == "multilingual":
            kwargs["language_id"] = "zh"
        start = time.perf_counter()
        wav = model.generate(args.prewarm_text, **kwargs)
        wav_bytes = tensor_to_wav_bytes(wav, model.sr)
        print(
            f"Prewarm complete bytes={len(wav_bytes)} "
            f"ms={(time.perf_counter() - start) * 1000.0:.1f}",
            flush=True,
        )
    server.serve_forever()


if __name__ == "__main__":
    main()
