param(
    [int]$OllamaPort = 11435,
    [int]$TtsPort = 8020,
    [int]$SttPort = 8030,
    [switch]$KeepOllama,
    [switch]$KeepTts,
    [switch]$KeepSTT,
    [switch]$DryRun
)

$ErrorActionPreference = "Stop"

$ProjectRoot = Split-Path -Parent $PSScriptRoot
$PidDir = Join-Path $ProjectRoot "Saved\ServicePids"
$OllamaPidFile = Join-Path $PidDir "OllamaVulkan.pid"
$TtsPidFile = Join-Path $PidDir "ChatterboxTTS.pid"
$SttPidFile = Join-Path $PidDir "WhisperSTT.pid"

function Write-Step([string]$Message) {
    Write-Host ""
    Write-Host "== $Message =="
}

function Get-ListeningPids([int]$Port) {
    try {
        $connections = Get-NetTCPConnection `
            -LocalAddress "127.0.0.1" `
            -LocalPort $Port `
            -State Listen `
            -ErrorAction Stop
    }
    catch {
        try {
            $connections = Get-NetTCPConnection `
                -LocalPort $Port `
                -State Listen `
                -ErrorAction Stop
        }
        catch {
            return @()
        }
    }

    return @($connections | Select-Object -ExpandProperty OwningProcess -Unique)
}

function Stop-Pid([int]$ProcessId, [string]$Name) {
    if ($ProcessId -le 0) {
        return $false
    }

    $process = Get-Process -Id $ProcessId -ErrorAction SilentlyContinue
    if ($null -eq $process) {
        Write-Host "$Name PID $ProcessId is not running."
        return $false
    }

    if ($DryRun) {
        Write-Host "[DryRun] Would stop $Name PID $ProcessId ($($process.ProcessName))."
        return $true
    }

    Write-Host "Stopping $Name PID $ProcessId ($($process.ProcessName))..."
    Stop-Process -Id $ProcessId -Force -ErrorAction Stop
    return $true
}

function Stop-ByPidFile([string]$Path, [string]$Name) {
    if (-not (Test-Path -LiteralPath $Path)) {
        return $false
    }

    $raw = (Get-Content -LiteralPath $Path -ErrorAction SilentlyContinue | Select-Object -First 1)
    $pidValue = 0
    if (-not [int]::TryParse([string]$raw, [ref]$pidValue)) {
        Write-Host "$Name PID file exists but does not contain a valid PID: $Path"
        return $false
    }

    $stopped = Stop-Pid -ProcessId $pidValue -Name $Name
    if ($stopped -and -not $DryRun) {
        Remove-Item -LiteralPath $Path -Force -ErrorAction SilentlyContinue
    }
    return $stopped
}

function Stop-ByPort([int]$Port, [string]$Name) {
    $pids = Get-ListeningPids $Port
    if ($pids.Count -eq 0) {
        Write-Host "$Name is not listening on port $Port."
        return
    }

    foreach ($pidValue in $pids) {
        Stop-Pid -ProcessId ([int]$pidValue) -Name "$Name on port $Port" | Out-Null
    }
}

if ($DryRun) {
    Write-Host "NaturalNPC service stopper dry run"
    Write-Host "ProjectRoot: $ProjectRoot"
    Write-Host "OllamaPidFile: $OllamaPidFile"
    Write-Host "TtsPidFile: $TtsPidFile"
    Write-Host "SttPidFile: $SttPidFile"
}

if (-not $KeepSTT) {
    Write-Step "Stopping Whisper STT"
    $stoppedByPid = Stop-ByPidFile -Path $SttPidFile -Name "Whisper STT"
    if (-not $stoppedByPid) {
        Stop-ByPort -Port $SttPort -Name "Whisper STT"
    }
}
else {
    Write-Host "Keeping Whisper STT running."
}

if (-not $KeepTts) {
    Write-Step "Stopping Chatterbox TTS"
    $stoppedByPid = Stop-ByPidFile -Path $TtsPidFile -Name "Chatterbox TTS"
    if (-not $stoppedByPid) {
        Stop-ByPort -Port $TtsPort -Name "Chatterbox TTS"
    }
}
else {
    Write-Host "Keeping Chatterbox TTS running."
}

if (-not $KeepOllama) {
    Write-Step "Stopping Ollama Vulkan"
    $stoppedByPid = Stop-ByPidFile -Path $OllamaPidFile -Name "Ollama Vulkan"
    if (-not $stoppedByPid) {
        Stop-ByPort -Port $OllamaPort -Name "Ollama Vulkan"
    }
}
else {
    Write-Host "Keeping Ollama Vulkan running."
}

Write-Step "NaturalNPC local services stopped"
Write-Host "Stopped only the project service ports:"
Write-Host "  Ollama Vulkan: 127.0.0.1:$OllamaPort"
Write-Host "  Chatterbox:    127.0.0.1:$TtsPort"
Write-Host "  Whisper STT:   127.0.0.1:$SttPort"
