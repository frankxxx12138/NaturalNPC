import argparse
import json
import wave
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from io import BytesIO


RIVA_URI = "localhost:50051"


def pcm_to_wav_bytes(pcm_bytes, sample_rate_hz):
    output = BytesIO()
    with wave.open(output, "wb") as wav_file:
        wav_file.setnchannels(1)
        wav_file.setsampwidth(2)
        wav_file.setframerate(int(sample_rate_hz))
        wav_file.writeframes(pcm_bytes)
    return output.getvalue()


class RivaHandler(BaseHTTPRequestHandler):
    def do_POST(self):
        if self.path != "/tts":
            self.send_error(404, "Use POST /tts")
            return

        try:
            import riva.client

            length = int(self.headers.get("Content-Length", "0"))
            payload = json.loads(self.rfile.read(length).decode("utf-8"))
            text = str(payload.get("text", "")).strip()
            if not text:
                raise ValueError("Missing text")

            sample_rate_hz = int(payload.get("sample_rate_hz", 44100))
            language_code = str(payload.get("language_code", "zh-CN"))
            voice = str(payload.get("voice", "Mandarin-CN.Male-1"))

            auth = riva.client.Auth(uri=RIVA_URI)
            service = riva.client.SpeechSynthesisService(auth)
            response = service.synthesize(
                text=text,
                language_code=language_code,
                encoding=riva.client.AudioEncoding.LINEAR_PCM,
                sample_rate_hz=sample_rate_hz,
                voice_name=voice,
            )
            wav_bytes = pcm_to_wav_bytes(response.audio, sample_rate_hz)

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
    global RIVA_URI

    parser = argparse.ArgumentParser()
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--port", type=int, default=8020)
    parser.add_argument("--riva-uri", default="localhost:50051")
    args = parser.parse_args()

    RIVA_URI = args.riva_uri
    server = ThreadingHTTPServer((args.host, args.port), RivaHandler)
    print(
        f"Riva TTS HTTP gateway ready on http://{args.host}:{args.port}/tts "
        f"-> {RIVA_URI}"
    )
    server.serve_forever()


if __name__ == "__main__":
    main()
