param(
    [int]$Port = 8040,
    [switch]$DryRun
)

$ErrorActionPreference = "Stop"
$ProjectRoot = Split-Path -Parent $PSScriptRoot
$PidFile = Join-Path $ProjectRoot "Saved\ServicePids\OpenAIRealtimeBridge.pid"
$BridgeScript = Join-Path $PSScriptRoot "openai_realtime_ue_bridge.py"

if (-not (Test-Path -LiteralPath $PidFile)) {
    Write-Host "No OpenAI Realtime bridge PID file was found."
    Write-Host "If port $Port is occupied, it was not stopped because its ownership could not be verified."
    exit 0
}

$rawPid = Get-Content -LiteralPath $PidFile -ErrorAction Stop | Select-Object -First 1
$processId = 0
if (-not [int]::TryParse([string]$rawPid, [ref]$processId)) {
    throw "Invalid PID file: $PidFile"
}
$process = Get-Process -Id $processId -ErrorAction SilentlyContinue
if ($null -eq $process) {
    Remove-Item -LiteralPath $PidFile -Force
    Write-Host "The recorded bridge process is no longer running."
    exit 0
}

$commandLine = ""
try {
    $commandLine = (Get-CimInstance Win32_Process -Filter "ProcessId=$processId").CommandLine
}
catch {
    Write-Warning "Could not inspect the recorded process command line."
}
if (-not $commandLine -or -not $commandLine.Contains($BridgeScript)) {
    throw "PID $processId is not the recorded NaturalNPC OpenAI bridge; it was not stopped."
}

if ($DryRun) {
    Write-Host "[DryRun] Would stop OpenAI Realtime bridge PID $processId."
    exit 0
}
Stop-Process -Id $processId -Force
Remove-Item -LiteralPath $PidFile -Force
Write-Host "Stopped OpenAI Realtime bridge PID $processId."
