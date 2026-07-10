param(
    [int]$Port = 8030,
    [string]$Model = "small",
    [string]$Device = "cpu",
    [string]$ComputeType = "int8",
    [string]$Language = "auto",
    [int]$SampleRate = 16000,
    [switch]$NoHiddenWindow
)

$ErrorActionPreference = "Stop"

$ProjectRoot = Split-Path -Parent $PSScriptRoot
$SavedDir = Join-Path $ProjectRoot "Saved"
$LogsDir = Join-Path $SavedDir "Logs"
$PidDir = Join-Path $SavedDir "ServicePids"
$Python = Join-Path $ProjectRoot ".venv-stt\Scripts\python.exe"
$ServerScript = Join-Path $PSScriptRoot "faster_whisper_stt_server.py"
$StdoutLog = Join-Path $LogsDir "WhisperSTT.out.log"
$StderrLog = Join-Path $LogsDir "WhisperSTT.err.log"
$PidFile = Join-Path $PidDir "WhisperSTT.pid"

function Test-TcpPort([int]$PortToCheck) {
    $client = [System.Net.Sockets.TcpClient]::new()
    try {
        $async = $client.BeginConnect("127.0.0.1", $PortToCheck, $null, $null)
        if (-not $async.AsyncWaitHandle.WaitOne(500)) {
            return $false
        }
        $client.EndConnect($async)
        return $true
    }
    catch {
        return $false
    }
    finally {
        $client.Close()
    }
}

if (Test-TcpPort $Port) {
    Write-Host "Whisper STT is already running on port $Port."
    exit 0
}

if (-not (Test-Path -LiteralPath $Python)) {
    throw "Missing STT Python environment: $Python. Run Scripts\Install-WhisperSTT.ps1 first."
}
if (-not (Test-Path -LiteralPath $ServerScript)) {
    throw "Missing STT server script: $ServerScript"
}

New-Item -ItemType Directory -Force -Path $LogsDir, $PidDir | Out-Null

$args = @(
    "-u",
    $ServerScript,
    "--port", "$Port",
    "--model", $Model,
    "--device", $Device,
    "--compute-type", $ComputeType,
    "--language", $Language,
    "--sample-rate", "$SampleRate"
)

Write-Host "Starting Whisper STT on http://127.0.0.1:$Port..."
Write-Host "Model: $Model, device: $Device, compute_type: $ComputeType, language: $Language"

$env:HF_HUB_DISABLE_XET = "1"
$env:HF_HUB_DISABLE_SYMLINKS_WARNING = "1"

$windowStyle = if ($NoHiddenWindow) { "Normal" } else { "Hidden" }
$process = Start-Process `
    -FilePath $Python `
    -ArgumentList $args `
    -WorkingDirectory $ProjectRoot `
    -WindowStyle $windowStyle `
    -RedirectStandardOutput $StdoutLog `
    -RedirectStandardError $StderrLog `
    -PassThru

Set-Content -LiteralPath $PidFile -Value $process.Id
Write-Host "Whisper STT process started: PID $($process.Id)"
Write-Host "Logs:"
Write-Host "  $StdoutLog"
Write-Host "  $StderrLog"
