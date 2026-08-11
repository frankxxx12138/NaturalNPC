function Initialize-NaturalNPCParentProcessQuery {
    if ("NaturalNPCParentProcessQuery" -as [type]) {
        return
    }

    Add-Type -TypeDefinition @'
using System;
using System.ComponentModel;
using System.Runtime.InteropServices;

public static class NaturalNPCParentProcessQuery
{
    [StructLayout(LayoutKind.Sequential)]
    private struct ProcessBasicInformation
    {
        public IntPtr Reserved1;
        public IntPtr PebBaseAddress;
        public IntPtr Reserved2_0;
        public IntPtr Reserved2_1;
        public IntPtr UniqueProcessId;
        public IntPtr InheritedFromUniqueProcessId;
    }

    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern IntPtr OpenProcess(
        uint desiredAccess,
        bool inheritHandle,
        int processId);

    [DllImport("kernel32.dll")]
    private static extern bool CloseHandle(IntPtr handle);

    [DllImport("ntdll.dll")]
    private static extern int NtQueryInformationProcess(
        IntPtr processHandle,
        int processInformationClass,
        ref ProcessBasicInformation processInformation,
        int processInformationLength,
        out int returnLength);

    public static int GetParentProcessId(int processId)
    {
        const uint ProcessQueryLimitedInformation = 0x1000;
        IntPtr handle = OpenProcess(
            ProcessQueryLimitedInformation,
            false,
            processId);
        if (handle == IntPtr.Zero)
        {
            throw new Win32Exception(Marshal.GetLastWin32Error());
        }

        try
        {
            ProcessBasicInformation information =
                new ProcessBasicInformation();
            int returnLength;
            int status = NtQueryInformationProcess(
                handle,
                0,
                ref information,
                Marshal.SizeOf(information),
                out returnLength);
            if (status != 0)
            {
                throw new InvalidOperationException(
                    "NtQueryInformationProcess status=" + status);
            }
            return information.InheritedFromUniqueProcessId.ToInt32();
        }
        finally
        {
            CloseHandle(handle);
        }
    }
}
'@
}

function Get-NaturalNPCProcessPath([System.Diagnostics.Process]$Process) {
    try {
        return [string]$Process.Path
    }
    catch {
        return ""
    }
}

function Test-NaturalNPCProcessPath(
    [System.Diagnostics.Process]$Process,
    [string]$ExpectedPath
) {
    $actualPath = Get-NaturalNPCProcessPath -Process $Process
    if (-not $actualPath -or -not $ExpectedPath) {
        return $false
    }
    return [string]::Equals(
        [System.IO.Path]::GetFullPath($actualPath),
        [System.IO.Path]::GetFullPath($ExpectedPath),
        [System.StringComparison]::OrdinalIgnoreCase
    )
}

function Get-NaturalNPCParentProcessId([int]$ProcessId) {
    try {
        Initialize-NaturalNPCParentProcessQuery
        return [NaturalNPCParentProcessQuery]::GetParentProcessId($ProcessId)
    }
    catch {
        Write-Warning (
            "Unable to inspect parent process for PID {0}: {1}" -f `
                $ProcessId, $_.Exception.Message
        )
        return 0
    }
}

function Get-NaturalNPCOllamaRunnerPath([string]$OllamaExecutable) {
    $installDir = Split-Path -Parent $OllamaExecutable
    return Join-Path $installDir "lib\ollama\llama-server.exe"
}

function Get-NaturalNPCOllamaRunners([string]$OllamaExecutable) {
    $expectedRunnerPath = Get-NaturalNPCOllamaRunnerPath `
        -OllamaExecutable $OllamaExecutable
    return @(
        Get-Process -Name "llama-server" -ErrorAction SilentlyContinue |
            Where-Object {
                Test-NaturalNPCProcessPath `
                    -Process $_ `
                    -ExpectedPath $expectedRunnerPath
            }
    )
}

function Get-NaturalNPCOrphanedOllamaRunners(
    [string]$OllamaExecutable
) {
    $orphans = @()
    foreach ($runner in @(Get-NaturalNPCOllamaRunners `
        -OllamaExecutable $OllamaExecutable)) {
        $parentId = Get-NaturalNPCParentProcessId -ProcessId $runner.Id
        if ($parentId -le 0) {
            continue
        }

        $parent = Get-Process -Id $parentId -ErrorAction SilentlyContinue
        $parentIsOllama = $null -ne $parent -and `
            (Test-NaturalNPCProcessPath `
                -Process $parent `
                -ExpectedPath $OllamaExecutable)
        $parentPredatesRunner = $false
        if ($parentIsOllama) {
            try {
                $parentPredatesRunner = $parent.StartTime -le $runner.StartTime
            }
            catch {
                $parentPredatesRunner = $true
            }
        }

        if (-not $parentIsOllama -or -not $parentPredatesRunner) {
            $orphans += $runner
        }
    }
    return @($orphans)
}

function Stop-NaturalNPCOrphanedOllamaRunners(
    [string]$OllamaExecutable,
    [switch]$DryRun
) {
    $stopped = @()
    foreach ($runner in @(Get-NaturalNPCOrphanedOllamaRunners `
        -OllamaExecutable $OllamaExecutable)) {
        $parentId = Get-NaturalNPCParentProcessId -ProcessId $runner.Id
        if ($DryRun) {
            $message = (
                "[DryRun] Would stop orphaned Ollama runner PID {0} " +
                "(former parent PID {1})."
            ) -f $runner.Id, $parentId
            Write-Host $message
            $stopped += $runner.Id
            continue
        }

        $message = (
            "Stopping orphaned Ollama runner PID {0} " +
            "(former parent PID {1})..."
        ) -f $runner.Id, $parentId
        Write-Host $message
        Stop-Process -Id $runner.Id -Force -ErrorAction SilentlyContinue
        $stopped += $runner.Id
    }
    return @($stopped)
}

function Get-NaturalNPCOllamaChildRunnerPids(
    [string]$OllamaExecutable,
    [int[]]$DaemonPids
) {
    if ($null -eq $DaemonPids -or $DaemonPids.Count -eq 0) {
        return @()
    }

    $result = @()
    foreach ($runner in @(Get-NaturalNPCOllamaRunners `
        -OllamaExecutable $OllamaExecutable)) {
        $parentId = Get-NaturalNPCParentProcessId -ProcessId $runner.Id
        if ($DaemonPids -contains $parentId) {
            $result += $runner.Id
        }
    }
    return @($result)
}

function Stop-NaturalNPCValidatedOllamaRunnerPids(
    [string]$OllamaExecutable,
    [int[]]$RunnerPids,
    [switch]$DryRun
) {
    $expectedRunnerPath = Get-NaturalNPCOllamaRunnerPath `
        -OllamaExecutable $OllamaExecutable
    foreach ($runnerPid in @($RunnerPids | Select-Object -Unique)) {
        $runner = Get-Process -Id $runnerPid -ErrorAction SilentlyContinue
        if ($null -eq $runner -or -not (Test-NaturalNPCProcessPath `
            -Process $runner `
            -ExpectedPath $expectedRunnerPath)) {
            continue
        }

        if ($DryRun) {
            Write-Host "[DryRun] Would stop Ollama runner PID $runnerPid."
            continue
        }

        Write-Host "Stopping retained Ollama runner PID $runnerPid..."
        Stop-Process -Id $runnerPid -Force -ErrorAction SilentlyContinue
    }
}
