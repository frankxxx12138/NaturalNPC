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
$OllamaWatchdogPidFile = Join-Path $PidDir "OllamaVulkanWatchdog.pid"
$TtsPidFile = Join-Path $PidDir "ChatterboxTTS.pid"
$SttPidFile = Join-Path $PidDir "WhisperSTT.pid"
$OllamaExecutable = Join-Path $env:LOCALAPPDATA `
    "Programs\Ollama\ollama.exe"
$OllamaUtilities = Join-Path $PSScriptRoot `
    "OllamaProcessUtilities.ps1"

. $OllamaUtilities

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
            $netstatPids = @()
            foreach ($line in @(& netstat.exe -ano -p TCP 2>$null)) {
                if ($line -match (
                    "^\s*TCP\s+\S+:" + $Port +
                    "\s+\S+\s+LISTENING\s+(\d+)\s*$"
                )) {
                    $netstatPids += [int]$Matches[1]
                }
            }
            return @($netstatPids | Select-Object -Unique)
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

function Get-ValidatedOllamaDaemonPids([int]$Port) {
    $candidates = @()
    if (Test-Path -LiteralPath $OllamaPidFile) {
        $rawPid = Get-Content -LiteralPath $OllamaPidFile `
            -ErrorAction SilentlyContinue | Select-Object -First 1
        $pidValue = 0
        if ([int]::TryParse([string]$rawPid, [ref]$pidValue)) {
            $candidates += $pidValue
        }
    }
    $candidates += @(Get-ListeningPids -Port $Port)

    $validated = @()
    foreach ($candidate in @($candidates | Select-Object -Unique)) {
        $process = Get-Process -Id $candidate -ErrorAction SilentlyContinue
        if ($null -ne $process -and (Test-NaturalNPCProcessPath `
            -Process $process `
            -ExpectedPath $OllamaExecutable)) {
            $validated += [int]$candidate
        }
    }
    return @($validated)
}

function Request-OllamaModelUnload([int]$Port) {
    if ($DryRun) {
        Write-Host "[DryRun] Would request graceful Ollama model unload."
        return
    }

    try {
        $runningModels = Invoke-RestMethod `
            -Uri "http://127.0.0.1:$Port/api/ps" `
            -TimeoutSec 3
        foreach ($model in @($runningModels.models)) {
            if (-not $model.name) {
                continue
            }
            $body = @{
                model = [string]$model.name
                keep_alive = 0
            } | ConvertTo-Json -Compress
            Invoke-RestMethod `
                -Method Post `
                -Uri "http://127.0.0.1:$Port/api/generate" `
                -ContentType "application/json" `
                -Body $body `
                -TimeoutSec 30 | Out-Null
            Write-Host "Requested Ollama model unload: $($model.name)"
        }
    }
    catch {
        Write-Warning "Ollama graceful unload was unavailable: $($_.Exception.Message)"
    }
}

if ($DryRun) {
    Write-Host "NaturalNPC service stopper dry run"
    Write-Host "ProjectRoot: $ProjectRoot"
    Write-Host "OllamaPidFile: $OllamaPidFile"
    Write-Host "OllamaWatchdogPidFile: $OllamaWatchdogPidFile"
    Write-Host "TtsPidFile: $TtsPidFile"
    Write-Host "SttPidFile: $SttPidFile"
}

if (-not $KeepSTT) {
    Write-Step "Stopping Whisper STT"
    Stop-ByPidFile -Path $SttPidFile -Name "Whisper STT" | Out-Null
    # A venv python launcher can leave the real base-python child alive.
    Stop-ByPort -Port $SttPort -Name "Whisper STT"
}
else {
    Write-Host "Keeping Whisper STT running."
}

if (-not $KeepTts) {
    Write-Step "Stopping Chatterbox TTS"
    Stop-ByPidFile -Path $TtsPidFile -Name "Chatterbox TTS" | Out-Null
    # Always verify the listener because the PID file may name only the
    # lightweight venv launcher rather than its memory-heavy child process.
    Stop-ByPort -Port $TtsPort -Name "Chatterbox TTS"
}
else {
    Write-Host "Keeping Chatterbox TTS running."
}

if (-not $KeepOllama) {
    Write-Step "Stopping Ollama Vulkan"
    $ollamaDaemonPids = @(Get-ValidatedOllamaDaemonPids `
        -Port $OllamaPort)
    $ollamaRunnerPids = @(
        Get-NaturalNPCOllamaChildRunnerPids `
            -OllamaExecutable $OllamaExecutable `
            -DaemonPids $ollamaDaemonPids
    )
    Request-OllamaModelUnload -Port $OllamaPort
    Stop-ByPidFile -Path $OllamaPidFile -Name "Ollama Vulkan" | Out-Null
    Stop-ByPort -Port $OllamaPort -Name "Ollama Vulkan"
    if (-not $DryRun) {
        $deadline = (Get-Date).AddSeconds(5)
        do {
            $remainingRunners = @(
                $ollamaRunnerPids | Where-Object {
                    $null -ne (Get-Process -Id $_ `
                        -ErrorAction SilentlyContinue)
                }
            )
            if ($remainingRunners.Count -eq 0) {
                break
            }
            Start-Sleep -Milliseconds 250
        } while ((Get-Date) -lt $deadline)
    }
    Stop-NaturalNPCValidatedOllamaRunnerPids `
        -OllamaExecutable $OllamaExecutable `
        -RunnerPids $ollamaRunnerPids `
        -DryRun:$DryRun
    Stop-NaturalNPCOrphanedOllamaRunners `
        -OllamaExecutable $OllamaExecutable `
        -DryRun:$DryRun | Out-Null
    if (-not $DryRun) {
        $watchdogPid = 0
        if (Test-Path -LiteralPath $OllamaWatchdogPidFile) {
            $rawWatchdogPid = Get-Content `
                -LiteralPath $OllamaWatchdogPidFile `
                -ErrorAction SilentlyContinue | Select-Object -First 1
            [int]::TryParse(
                [string]$rawWatchdogPid,
                [ref]$watchdogPid
            ) | Out-Null
        }
        $watchdogDeadline = (Get-Date).AddSeconds(8)
        do {
            $watchdog = Get-Process -Id $watchdogPid `
                -ErrorAction SilentlyContinue
            if ($null -eq $watchdog) {
                break
            }
            Start-Sleep -Milliseconds 250
        } while ((Get-Date) -lt $watchdogDeadline)
        if ($null -ne $watchdog) {
            Write-Warning (
                "Ollama watchdog PID $watchdogPid did not exit yet; " +
                "it was left running rather than stopping an unverified " +
                "PowerShell process."
            )
        }
        else {
            Remove-Item -LiteralPath $OllamaWatchdogPidFile `
                -Force -ErrorAction SilentlyContinue
        }
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
