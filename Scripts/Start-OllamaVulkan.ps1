$ErrorActionPreference = "Stop"

$ollama = Join-Path $env:LOCALAPPDATA "Programs\Ollama\ollama.exe"
if (-not (Test-Path -LiteralPath $ollama)) {
    throw "Ollama was not found at: $ollama"
}

$ProjectRoot = Split-Path -Parent $PSScriptRoot
$PidDir = Join-Path $ProjectRoot "Saved\ServicePids"
$LogsDir = Join-Path $ProjectRoot "Saved\Logs"
$PidFile = Join-Path $PidDir "OllamaVulkan.pid"
$WatchdogPidFile = Join-Path $PidDir "OllamaVulkanWatchdog.pid"
$WatchdogScript = Join-Path $PSScriptRoot "Watch-OllamaRunners.ps1"
$Utilities = Join-Path $PSScriptRoot "OllamaProcessUtilities.ps1"

. $Utilities

function Resolve-OllamaDaemonPid([int]$Port) {
    $candidates = @()
    if (Test-Path -LiteralPath $PidFile) {
        $rawPid = Get-Content -LiteralPath $PidFile `
            -ErrorAction SilentlyContinue | Select-Object -First 1
        $pidValue = 0
        if ([int]::TryParse([string]$rawPid, [ref]$pidValue)) {
            $candidates += $pidValue
        }
    }

    try {
        $connections = Get-NetTCPConnection `
            -LocalPort $Port `
            -State Listen `
            -ErrorAction Stop
        $candidates += @(
            $connections | Select-Object -ExpandProperty OwningProcess
        )
    }
    catch {
        foreach ($line in @(& netstat.exe -ano -p TCP 2>$null)) {
            if ($line -match (
                "^\s*TCP\s+\S+:" + $Port +
                "\s+\S+\s+LISTENING\s+(\d+)\s*$"
            )) {
                $candidates += [int]$Matches[1]
            }
        }
    }

    foreach ($candidate in @($candidates | Select-Object -Unique)) {
        $process = Get-Process -Id $candidate -ErrorAction SilentlyContinue
        if ($null -ne $process -and (Test-NaturalNPCProcessPath `
            -Process $process `
            -ExpectedPath $ollama)) {
            return [int]$candidate
        }
    }
    return 0
}

function Ensure-OllamaRunnerWatchdog([int]$DaemonPid) {
    if ($DaemonPid -le 0) {
        Write-Warning "Cannot start Ollama watchdog without a validated daemon PID."
        return
    }
    if (-not (Test-Path -LiteralPath $WatchdogScript)) {
        Write-Warning "Ollama watchdog script is missing: $WatchdogScript"
        return
    }

    if (Test-Path -LiteralPath $WatchdogPidFile) {
        $watchdogRecord = @(Get-Content -LiteralPath $WatchdogPidFile `
            -ErrorAction SilentlyContinue)
        $rawPid = $watchdogRecord | Select-Object -First 1
        $watchdogPid = 0
        $recordedDaemonPid = 0
        if ($watchdogRecord.Count -gt 1) {
            [int]::TryParse(
                [string]$watchdogRecord[1],
                [ref]$recordedDaemonPid
            ) | Out-Null
        }
        if ([int]::TryParse([string]$rawPid, [ref]$watchdogPid)) {
            $existing = Get-Process -Id $watchdogPid `
                -ErrorAction SilentlyContinue
            if ($null -ne $existing -and
                $existing.ProcessName -match "^(powershell|pwsh)$" -and
                $recordedDaemonPid -eq $DaemonPid) {
                Write-Host "Ollama runner watchdog is already running (PID $watchdogPid)."
                return
            }
        }
        Remove-Item -LiteralPath $WatchdogPidFile `
            -Force -ErrorAction SilentlyContinue
    }

    New-Item -ItemType Directory -Force -Path $LogsDir, $PidDir | Out-Null
    $powerShellExecutable = (Get-Process -Id $PID).Path
    $watchdogArguments = (
        "-NoProfile -ExecutionPolicy Bypass -File `"{0}`" " +
        "-DaemonPid {1} -OllamaExecutable `"{2}`""
    ) -f $WatchdogScript, $DaemonPid, $ollama
    $watchdogStdoutLog = Join-Path $LogsDir `
        "OllamaWatchdog-$DaemonPid.out.log"
    $watchdogStderrLog = Join-Path $LogsDir `
        "OllamaWatchdog-$DaemonPid.err.log"
    $watchdog = Start-Process `
        -FilePath $powerShellExecutable `
        -ArgumentList $watchdogArguments `
        -WindowStyle Hidden `
        -RedirectStandardOutput $watchdogStdoutLog `
        -RedirectStandardError $watchdogStderrLog `
        -PassThru
    Set-Content -LiteralPath $WatchdogPidFile `
        -Value @($watchdog.Id, $DaemonPid)
    Write-Host "Ollama runner watchdog started: PID $($watchdog.Id)"
}

Stop-NaturalNPCOrphanedOllamaRunners `
    -OllamaExecutable $ollama | Out-Null

try {
    $version = Invoke-RestMethod `
        -Uri "http://127.0.0.1:11435/api/version" `
        -TimeoutSec 2
    Write-Host "Ollama Vulkan is already running on port 11435 ($($version.version))."
    Ensure-OllamaRunnerWatchdog `
        -DaemonPid (Resolve-OllamaDaemonPid -Port 11435)
    return
}
catch {
    # Start a dedicated Vulkan service for Unreal Engine.
}

$env:OLLAMA_HOST = "127.0.0.1:11435"
$env:OLLAMA_LLM_LIBRARY = "vulkan"
$env:OLLAMA_KEEP_ALIVE = "60s"
$env:OLLAMA_CONTEXT_LENGTH = "4096"

New-Item -ItemType Directory -Force -Path $PidDir, $LogsDir | Out-Null

$process = Start-Process `
    -FilePath $ollama `
    -ArgumentList "serve" `
    -WindowStyle Hidden `
    -PassThru

Set-Content -LiteralPath $PidFile -Value $process.Id
Ensure-OllamaRunnerWatchdog -DaemonPid $process.Id

Start-Sleep -Seconds 3
$version = Invoke-RestMethod `
    -Uri "http://127.0.0.1:11435/api/version" `
    -TimeoutSec 10

Write-Host "Ollama Vulkan started on port 11435 ($($version.version))."
