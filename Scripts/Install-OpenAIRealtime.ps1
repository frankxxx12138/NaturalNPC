param(
    [string]$Python = ""
)

$ErrorActionPreference = "Stop"
$ProjectRoot = Split-Path -Parent $PSScriptRoot
if (-not $Python) {
    $Python = Join-Path $ProjectRoot ".venv-stt\Scripts\python.exe"
}
if (-not (Test-Path -LiteralPath $Python)) {
    throw "Missing Python environment: $Python. Run Scripts\Install-WhisperSTT.ps1 first."
}

Write-Host "Installing the OpenAI Realtime bridge dependency into:"
Write-Host "  $Python"
& $Python -m pip install --upgrade "websockets>=15,<17"
if ($LASTEXITCODE -ne 0) {
    throw "Failed to install websockets. pip exited with code $LASTEXITCODE."
}

& $Python -c "import numpy, sounddevice, websockets; print('OpenAI Realtime Python dependencies are ready.')"
if ($LASTEXITCODE -ne 0) {
    throw "The STT environment is missing numpy or sounddevice. Re-run Scripts\Install-WhisperSTT.ps1."
}
