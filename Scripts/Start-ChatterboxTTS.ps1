param(
    [int]$Port = 8020,
    [string]$Device = "cuda",
    [string]$ModelKind = "turbo",
    [string]$PrewarmText = "",
    [int]$IdleUnloadSeconds = 60,
    [int]$MaxMemoryVoiceConditions = 4
)

$ProjectRoot = Split-Path -Parent $PSScriptRoot
$Python = Join-Path $ProjectRoot ".venv-tts\Scripts\python.exe"
$ServerScript = Join-Path $ProjectRoot "Scripts\chatterbox_tts_server.py"
$CacheDir = Join-Path $ProjectRoot "Saved\TTSCache\Chatterbox"

if (-not (Test-Path -LiteralPath $Python)) {
    throw "Missing Python virtual environment: $Python"
}

if (-not (Test-Path -LiteralPath $ServerScript)) {
    throw "Missing Chatterbox server script: $ServerScript"
}

Set-Location $ProjectRoot
Write-Host "Starting Chatterbox TTS on http://127.0.0.1:$Port/tts"
Write-Host "Press Ctrl+C to stop the service."
$serverArgs = @(
    "-u", $ServerScript,
    "--port", "$Port",
    "--device", $Device,
    "--model-kind", $ModelKind,
    "--cache-dir", $CacheDir,
    "--idle-unload-seconds", "$IdleUnloadSeconds",
    "--max-memory-voice-conditions", "$MaxMemoryVoiceConditions"
)
if ($PrewarmText) {
    $serverArgs += @("--prewarm-text", $PrewarmText)
}
& $Python @serverArgs
