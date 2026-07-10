param(
    [string]$Python = "python",
    [switch]$Force
)

$ErrorActionPreference = "Stop"

$ProjectRoot = Split-Path -Parent $PSScriptRoot
$VenvDir = Join-Path $ProjectRoot ".venv-stt"
$VenvPython = Join-Path $VenvDir "Scripts\python.exe"

if ((Test-Path -LiteralPath $VenvPython) -and -not $Force) {
    Write-Host "Whisper STT virtual environment already exists:"
    Write-Host "  $VenvPython"
    Write-Host "Use -Force to recreate/update dependencies."
}
else {
    if (-not (Test-Path -LiteralPath $VenvDir)) {
        Write-Host "Creating Whisper STT virtual environment..."
        & $Python -m venv $VenvDir
    }
}

Write-Host "Upgrading pip..."
& $VenvPython -m pip install --upgrade pip

Write-Host "Installing faster-whisper STT dependencies..."
& $VenvPython -m pip install faster-whisper sounddevice numpy

Write-Host ""
Write-Host "Whisper STT install complete."
Write-Host "Python: $VenvPython"
Write-Host "Start it with:"
Write-Host "  powershell -ExecutionPolicy Bypass -File Scripts\Start-WhisperSTT.ps1"
