param(
    [Parameter(Mandatory = $true)]
    [int]$DaemonPid,
    [Parameter(Mandatory = $true)]
    [string]$OllamaExecutable,
    [int]$PollSeconds = 3
)

$ErrorActionPreference = "Continue"

$Utilities = Join-Path $PSScriptRoot "OllamaProcessUtilities.ps1"
. $Utilities

$safePollSeconds = [Math]::Max(1, $PollSeconds)
Write-Host "Ollama runner watchdog started for daemon PID $DaemonPid."

while ($true) {
    $daemon = Get-Process -Id $DaemonPid -ErrorAction SilentlyContinue
    if ($null -eq $daemon -or -not (Test-NaturalNPCProcessPath `
        -Process $daemon `
        -ExpectedPath $OllamaExecutable)) {
        break
    }

    Stop-NaturalNPCOrphanedOllamaRunners `
        -OllamaExecutable $OllamaExecutable | Out-Null
    Start-Sleep -Seconds $safePollSeconds
}

Start-Sleep -Seconds 2
Stop-NaturalNPCOrphanedOllamaRunners `
    -OllamaExecutable $OllamaExecutable | Out-Null
Write-Host "Ollama runner watchdog finished for daemon PID $DaemonPid."
