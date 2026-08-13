param(
    [int]$Port = 8040,
    [string]$Model = "gpt-realtime-2.1",
    [string]$Voice = "marin",
    [double]$Speed = 1.0,
    [string]$Language = "en",
    [string]$ApiKeyEnv = "OPENAI_API_KEY",
    [string]$SafetyIdentifier = "",
    [Nullable[int]]$InputDevice = $null,
    [int]$MinRecordMs = 350,
    [double]$MinRms = 350.0,
    [int]$MinPeak = 1200,
    [switch]$ResetSessionPerTurn,
    [switch]$NoHiddenWindow
)

$ErrorActionPreference = "Stop"
$ProjectRoot = Split-Path -Parent $PSScriptRoot
$Python = Join-Path $ProjectRoot ".venv-stt\Scripts\python.exe"
$BridgeScript = Join-Path $PSScriptRoot "openai_realtime_ue_bridge.py"
$LogsDir = Join-Path $ProjectRoot "Saved\Logs"
$PidDir = Join-Path $ProjectRoot "Saved\ServicePids"
$StdoutLog = Join-Path $LogsDir "OpenAIRealtimeBridge.out.log"
$StderrLog = Join-Path $LogsDir "OpenAIRealtimeBridge.err.log"
$PidFile = Join-Path $PidDir "OpenAIRealtimeBridge.pid"
$RequiredBridgeVersion = 4

function Get-BridgeHealth {
    try {
        return Invoke-RestMethod -Uri "http://127.0.0.1:$Port/health" -TimeoutSec 2
    }
    catch {
        return $null
    }
}

$existingHealth = Get-BridgeHealth
if ($existingHealth -and $existingHealth.ok -and
    [int]$existingHealth.bridge_version -ge $RequiredBridgeVersion -and
    [string]$existingHealth.language -eq $Language) {
    Write-Host "OpenAI Realtime bridge is already ready on port $Port."
    exit 0
}
if (-not (Test-Path -LiteralPath $Python)) {
    throw "Missing Python environment: $Python. Run Scripts\Install-WhisperSTT.ps1 first."
}
if (-not (Test-Path -LiteralPath $BridgeScript)) {
    throw "Missing bridge script: $BridgeScript"
}

$apiKey = [Environment]::GetEnvironmentVariable($ApiKeyEnv, "Process")
if (-not $apiKey) {
    $apiKey = [Environment]::GetEnvironmentVariable($ApiKeyEnv, "User")
}
if (-not $apiKey) {
    $apiKey = [Environment]::GetEnvironmentVariable($ApiKeyEnv, "Machine")
}
if (-not $apiKey) {
    throw "Missing $ApiKeyEnv. Set it as a user environment variable, then start again."
}
[Environment]::SetEnvironmentVariable($ApiKeyEnv, $apiKey, "Process")

if ($existingHealth -and $existingHealth.ok) {
    Write-Host "An older OpenAI Realtime bridge is running; restarting it with the current project code."
    & (Join-Path $PSScriptRoot "Stop-NaturalNPCOpenAI.ps1") -Port $Port
    Start-Sleep -Milliseconds 500
}

New-Item -ItemType Directory -Force -Path $LogsDir, $PidDir | Out-Null
$bridgeArgs = @(
    "-u", $BridgeScript,
    "--port", "$Port",
    "--model", $Model,
    "--voice", $Voice,
    "--speed", "$Speed",
    "--language", $Language,
    "--api-key-env", $ApiKeyEnv,
    "--min-record-ms", "$MinRecordMs",
    "--min-rms", "$MinRms",
    "--min-peak", "$MinPeak"
)
if ($SafetyIdentifier) {
    $bridgeArgs += @("--safety-identifier", $SafetyIdentifier)
}
if ($null -ne $InputDevice) {
    $bridgeArgs += @("--input-device", "$InputDevice")
}
if ($ResetSessionPerTurn) {
    $bridgeArgs += "--reset-session-per-turn"
}

$windowStyle = if ($NoHiddenWindow) { "Normal" } else { "Hidden" }
$process = Start-Process `
    -FilePath $Python `
    -ArgumentList $bridgeArgs `
    -WorkingDirectory $ProjectRoot `
    -WindowStyle $windowStyle `
    -RedirectStandardOutput $StdoutLog `
    -RedirectStandardError $StderrLog `
    -PassThru
Set-Content -LiteralPath $PidFile -Value $process.Id

$deadline = (Get-Date).AddSeconds(20)
while ((Get-Date) -lt $deadline) {
    $health = Get-BridgeHealth
    if ($health -and $health.ok -and
        [int]$health.bridge_version -ge $RequiredBridgeVersion -and
        [string]$health.language -eq $Language) {
        Write-Host "OpenAI Realtime backend is ready."
        Write-Host "  URL:   http://127.0.0.1:$Port"
        Write-Host "  Model: $Model"
        Write-Host "  STT:   $Language"
        Write-Host "  PID:   $($process.Id)"
        Write-Host "Use LLM Provider = OpenAI API in the NPC component."
        exit 0
    }
    if ($process.HasExited) {
        $errorTail = Get-Content -LiteralPath $StderrLog -Tail 20 -ErrorAction SilentlyContinue
        throw "OpenAI Realtime bridge exited early.`n$errorTail"
    }
    Start-Sleep -Milliseconds 250
}
throw "OpenAI Realtime bridge did not become ready. Check $StderrLog"
