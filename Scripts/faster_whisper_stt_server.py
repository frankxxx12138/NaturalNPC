import argparse
import json
import multiprocessing
import sys
import threading
import time
import traceback
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from urllib.parse import urlparse

try:
    import numpy as np
    import sounddevice as sd
except Exception as exc:
    print(
        "Missing STT dependencies. Install them with:\n"
        "  python -m pip install faster-whisper sounddevice numpy\n"
        f"\nImport error: {exc}",
        file=sys.stderr,
        flush=True,
    )
    raise


def normalize_language(language):
    if not language:
        return None
    value = str(language).strip()
    if not value or value.lower() == "auto":
        return None
    if value.lower() in ("en-us", "en-gb"):
        return "en"
    if value.lower() in ("zh-cn", "zh-hans", "chinese"):
        return "zh"
    return value


def stt_worker_main(connection, config):
    """Load CTranslate2 only in a worker that can return all memory on exit."""
    from faster_whisper import WhisperModel

    model = None
    idle_unload_seconds = config["idle_unload_seconds"]
    try:
        while True:
            if idle_unload_seconds > 0.0:
                if not connection.poll(idle_unload_seconds):
                    print(
                        f"Whisper worker exiting after "
                        f"{idle_unload_seconds:.0f}s idle",
                        flush=True,
                    )
                    return
            request = connection.recv()
            if request is None:
                return

            try:
                if model is None:
                    print(
                        "Loading faster-whisper model on demand "
                        f"model={config['model']} device={config['device']} "
                        f"compute_type={config['compute_type']}...",
                        flush=True,
                    )
                    model = WhisperModel(
                        config["model"],
                        device=config["device"],
                        compute_type=config["compute_type"],
                    )
                    print("Whisper STT worker model loaded.", flush=True)

                start = time.perf_counter()
                segments, info = model.transcribe(
                    request["audio"],
                    language=normalize_language(request["language"]),
                    beam_size=config["beam_size"],
                    vad_filter=not config["no_vad_filter"],
                    condition_on_previous_text=False,
                    temperature=0.0,
                )

                segment_payloads = []
                texts = []
                for segment in segments:
                    text = segment.text.strip()
                    if text:
                        texts.append(text)
                    segment_payloads.append(
                        {
                            "start": segment.start,
                            "end": segment.end,
                            "text": text,
                        }
                    )

                connection.send(
                    {
                        "ok": True,
                        "result": {
                            "text": " ".join(texts).strip(),
                            "language": getattr(
                                info,
                                "language",
                                request["language"] or "auto",
                            ),
                            "language_probability": getattr(
                                info,
                                "language_probability",
                                0.0,
                            ),
                            "segments": segment_payloads,
                            "stt_ms": (
                                time.perf_counter() - start
                            ) * 1000.0,
                        },
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


class Recorder:
    def __init__(self, sample_rate):
        self.sample_rate = sample_rate
        self.lock = threading.Lock()
        self.stream = None
        self.frames = []
        self.recording = False
        self.started_at = 0.0
        self.language = "auto"

    def _callback(self, indata, frames, time_info, status):
        if status:
            print(f"STT audio status: {status}", flush=True)
        with self.lock:
            if self.recording:
                self.frames.append(indata[:, 0].copy())

    def start(self, language):
        with self.lock:
            if self.recording:
                return {"ok": True, "already_recording": True}

            self.frames = []
            self.language = language or "auto"
            self.started_at = time.perf_counter()
            self.stream = sd.InputStream(
                samplerate=self.sample_rate,
                channels=1,
                dtype="float32",
                callback=self._callback,
            )
            self.stream.start()
            self.recording = True
            return {"ok": True, "already_recording": False}

    def stop(self):
        with self.lock:
            if not self.recording:
                return np.zeros(0, dtype=np.float32), 0.0, self.language

            stream = self.stream
            self.stream = None
            self.recording = False
            duration = max(0.0, time.perf_counter() - self.started_at)
            frames = self.frames
            self.frames = []
            language = self.language

        if stream is not None:
            stream.stop()
            stream.close()

        if not frames:
            return np.zeros(0, dtype=np.float32), duration, language

        audio = np.concatenate(frames).astype(np.float32, copy=False)
        return audio, duration, language


class STTState:
    def __init__(self, args):
        self.args = args
        self.recorder = Recorder(args.sample_rate)
        self.worker_lock = threading.Lock()
        self.worker_process = None
        self.worker_connection = None

    def _stop_worker_locked(self, force):
        connection = self.worker_connection
        process = self.worker_process
        self.worker_connection = None
        self.worker_process = None

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

    def _ensure_worker_locked(self):
        if self.worker_process is not None and self.worker_process.is_alive():
            return
        if self.worker_process is not None or self.worker_connection is not None:
            self._stop_worker_locked(force=False)

        config = {
            "model": self.args.model,
            "device": self.args.device,
            "compute_type": self.args.compute_type,
            "beam_size": self.args.beam_size,
            "no_vad_filter": self.args.no_vad_filter,
            "idle_unload_seconds": max(0.0, self.args.idle_unload_seconds),
        }
        context = multiprocessing.get_context("spawn")
        parent_connection, child_connection = context.Pipe(duplex=True)
        process = context.Process(
            target=stt_worker_main,
            args=(child_connection, config),
            name="WhisperSTTWorker",
            daemon=True,
        )
        process.start()
        child_connection.close()
        self.worker_connection = parent_connection
        self.worker_process = process
        print(f"Whisper worker started pid={process.pid}", flush=True)

    def is_model_loaded(self):
        with self.worker_lock:
            if self.worker_process is None:
                return False
            if self.worker_process.is_alive():
                return True
            self._stop_worker_locked(force=False)
            return False

    def transcribe(self, audio, language):
        if audio.size == 0:
            return {
                "text": "",
                "language": language or "auto",
                "segments": [],
                "duration": 0.0,
            }

        with self.worker_lock:
            self._ensure_worker_locked()
            try:
                self.worker_connection.send(
                    {"audio": audio, "language": language}
                )
                if not self.worker_connection.poll(self.args.worker_timeout_seconds):
                    raise TimeoutError("Whisper worker exceeded the timeout")
                response = self.worker_connection.recv()
            except Exception:
                self._stop_worker_locked(force=True)
                raise

            if not response.get("ok"):
                error = response.get("error", "Unknown Whisper worker error")
                self._stop_worker_locked(force=True)
                raise RuntimeError(error)
            return response["result"]

    def shutdown(self):
        with self.worker_lock:
            self._stop_worker_locked(force=True)


def make_handler(state):
    class Handler(BaseHTTPRequestHandler):
        server_version = "NaturalNPCWhisperSTT/1.0"

        def _read_json(self):
            length = int(self.headers.get("Content-Length", "0") or "0")
            if length <= 0:
                return {}
            raw = self.rfile.read(length).decode("utf-8")
            return json.loads(raw) if raw else {}

        def _send_json(self, code, payload):
            encoded = json.dumps(payload, ensure_ascii=False).encode("utf-8")
            self.send_response(code)
            self.send_header("Content-Type", "application/json; charset=utf-8")
            self.send_header("Content-Length", str(len(encoded)))
            self.end_headers()
            self.wfile.write(encoded)

        def do_GET(self):
            path = urlparse(self.path).path
            if path == "/health":
                self._send_json(
                    200,
                    {
                        "ok": True,
                        "model": state.args.model,
                        "device": state.args.device,
                        "sample_rate": state.args.sample_rate,
                        "model_loaded": state.is_model_loaded(),
                    },
                )
                return
            self._send_json(404, {"error": "not found"})

        def do_POST(self):
            path = urlparse(self.path).path
            try:
                body = self._read_json()
                language = body.get("language", state.args.language)

                if path == "/stt/start":
                    result = state.recorder.start(language)
                    print(
                        f"STT start language={language} "
                        f"already={result['already_recording']}",
                        flush=True,
                    )
                    self._send_json(200, result)
                    return

                if path == "/stt/stop":
                    audio, record_duration, active_language = state.recorder.stop()
                    result = state.transcribe(audio, active_language)
                    result["ok"] = True
                    result["record_seconds"] = record_duration
                    result["samples"] = int(audio.size)
                    print(
                        "STT stop "
                        f"record_s={record_duration:.2f} "
                        f"stt_ms={result.get('stt_ms', 0.0):.1f} "
                        f"language={result.get('language')} "
                        f"text={result.get('text')}",
                        flush=True,
                    )
                    self._send_json(200, result)
                    return

                self._send_json(404, {"error": "not found"})
            except Exception as exc:
                traceback.print_exc()
                self._send_json(500, {"error": str(exc)})

        def log_message(self, fmt, *args):
            print(f"{self.address_string()} - {fmt % args}", flush=True)

    return Handler


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--port", type=int, default=8030)
    parser.add_argument("--model", default="small")
    parser.add_argument("--device", default="cpu")
    parser.add_argument("--compute-type", default="int8")
    parser.add_argument("--language", default="auto")
    parser.add_argument("--sample-rate", type=int, default=16000)
    parser.add_argument("--beam-size", type=int, default=1)
    parser.add_argument("--no-vad-filter", action="store_true")
    parser.add_argument("--idle-unload-seconds", type=float, default=60.0)
    parser.add_argument("--worker-timeout-seconds", type=float, default=120.0)
    args = parser.parse_args()
    args.worker_timeout_seconds = max(10.0, args.worker_timeout_seconds)

    state = STTState(args)
    server = ThreadingHTTPServer((args.host, args.port), make_handler(state))
    server.daemon_threads = True
    print(
        f"Whisper STT listening on http://{args.host}:{args.port}",
        flush=True,
    )
    try:
        server.serve_forever()
    finally:
        state.shutdown()


if __name__ == "__main__":
    multiprocessing.freeze_support()
    main()
