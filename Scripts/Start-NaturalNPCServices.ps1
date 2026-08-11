param(
    [string]$OllamaModel = "gemma4:e4b",
    [int]$OllamaPort = 11435,
    [int]$TtsPort = 8020,
    [int]$SttPort = 8030,
    [string]$TtsModelKind = "turbo",
    [string]$TtsDevice = "cuda",
    [string]$TtsPrewarmText = "",
    [int]$TtsIdleUnloadSeconds = 60,
    [int]$TtsMaxMemoryVoiceConditions = 4,
    [string]$SttModel = "small",
    [string]$SttDevice = "cpu",
    [string]$SttComputeType = "int8",
    [string]$SttLanguage = "auto",
    [int]$SttIdleUnloadSeconds = 60,
    [switch]$Warmup,
    [switch]$NoWarmup,
    [switch]$NoSTT,
    [switch]$DryRun
)

$ErrorActionPreference = "Stop"

$ProjectRoot = Split-Path -Parent $PSScriptRoot
$SavedDir = Join-Path $ProjectRoot "Saved"
$LogsDir = Join-Path $SavedDir "Logs"
$PidDir = Join-Path $SavedDir "ServicePids"
$OllamaScript = Join-Path $PSScriptRoot "Start-OllamaVulkan.ps1"
$Python = Join-Path $ProjectRoot ".venv-tts\Scripts\python.exe"
$TtsServerScript = Join-Path $PSScriptRoot "chatterbox_tts_server.py"
$TtsCacheDir = Join-Path $SavedDir "TTSCache\Chatterbox"
$TtsStdoutLog = Join-Path $LogsDir "ChatterboxTTS.out.log"
$TtsStderrLog = Join-Path $LogsDir "ChatterboxTTS.err.log"
$TtsPidFile = Join-Path $PidDir "ChatterboxTTS.pid"
$SttPython = Join-Path $ProjectRoot ".venv-stt\Scripts\python.exe"
$SttServerScript = Join-Path $PSScriptRoot "faster_whisper_stt_server.py"
$SttStdoutLog = Join-Path $LogsDir "WhisperSTT.out.log"
$SttStderrLog = Join-Path $LogsDir "WhisperSTT.err.log"
$SttPidFile = Join-Path $PidDir "WhisperSTT.pid"

function Write-Step([string]$Message) {
    Write-Host ""
    Write-Host "== $Message =="
}

function Test-TcpPort([int]$Port) {
    $client = [System.Net.Sockets.TcpClient]::new()
    try {
        $async = $client.BeginConnect("127.0.0.1", $Port, $null, $null)
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

function Wait-TcpPort([int]$Port, [int]$TimeoutSeconds, [string]$Name) {
    $deadline = (Get-Date).AddSeconds($TimeoutSeconds)
    while ((Get-Date) -lt $deadline) {
        if (Test-TcpPort $Port) {
            Write-Host "$Name is listening on port $Port."
            return
        }
        Start-Sleep -Milliseconds 500
    }
    throw "$Name did not open port $Port within $TimeoutSeconds seconds."
}

function Test-Ollama() {
    try {
        $version = Invoke-RestMethod `
            -Uri "http://127.0.0.1:$OllamaPort/api/version" `
            -TimeoutSec 2
        Write-Host "Ollama is running on port $OllamaPort ($($version.version))."
        return $true
    }
    catch {
        return $false
    }
}

function Warm-OllamaModel() {
    if ($NoWarmup -or -not $Warmup) {
        Write-Host "Skipping Ollama model warmup (models load on first use)."
        return
    }

    Write-Host "Warming Ollama model '$OllamaModel'..."
    $body = @{
        model = $OllamaModel
        messages = @(
            @{
                role = "system"
                content = "Reply with OK only."
            },
            @{
                role = "user"
                content = "Warm up."
            }
        )
        stream = $false
        think = $false
        keep_alive = "60s"
        options = @{
            num_ctx = 4096
            num_gpu = 999
            num_predict = 4
        }
    } | ConvertTo-Json -Depth 8

    $result = Invoke-RestMethod `
        -Method Post `
        -Uri "http://127.0.0.1:$OllamaPort/api/chat" `
        -ContentType "application/json" `
        -Body $body `
        -TimeoutSec 120

    $reply = $result.message.content
    Write-Host "Ollama model warmup complete: $reply"
}

function Test-Tts() {
    if (-not (Test-TcpPort $TtsPort)) {
        return $false
    }

    try {
        Invoke-RestMethod `
            -Uri "http://127.0.0.1:$TtsPort/health" `
            -TimeoutSec 3 | Out-Null
        Write-Host "Chatterbox TTS is responding on port $TtsPort."
        return $true
    }
    catch {
        Write-Host "Chatterbox port is open, but /tts is not ready yet."
        return $false
    }
}

function Start-Tts() {
    if (-not (Test-Path -LiteralPath $Python)) {
        throw "Missing Python virtual environment: $Python"
    }
    if (-not (Test-Path -LiteralPath $TtsServerScript)) {
        throw "Missing Chatterbox server script: $TtsServerScript"
    }

    New-Item -ItemType Directory -Force -Path $LogsDir, $PidDir, $TtsCacheDir | Out-Null

    $args = @(
        "-u",
        $TtsServerScript,
        "--port", "$TtsPort",
        "--device", $TtsDevice,
        "--model-kind", $TtsModelKind,
        "--cache-dir", $TtsCacheDir,
        "--idle-unload-seconds", "$TtsIdleUnloadSeconds",
        "--max-memory-voice-conditions", "$TtsMaxMemoryVoiceConditions"
    )
    if ($Warmup -and -not $NoWarmup -and $TtsPrewarmText) {
        $args += @("--prewarm-text", $TtsPrewarmText)
    }

    Write-Host "Starting Chatterbox TTS on http://127.0.0.1:$TtsPort/tts..."
    $process = Start-Process `
        -FilePath $Python `
        -ArgumentList $args `
        -WorkingDirectory $ProjectRoot `
        -WindowStyle Hidden `
        -RedirectStandardOutput $TtsStdoutLog `
        -RedirectStandardError $TtsStderrLog `
        -PassThru

    Set-Content -LiteralPath $TtsPidFile -Value $process.Id
    Write-Host "Chatterbox TTS process started: PID $($process.Id)"
    Write-Host "Logs:"
    Write-Host "  $TtsStdoutLog"
    Write-Host "  $TtsStderrLog"
}

function Test-Stt() {
    if (-not (Test-TcpPort $SttPort)) {
        return $false
    }

    try {
        Invoke-RestMethod `
            -Uri "http://127.0.0.1:$SttPort/health" `
            -TimeoutSec 2 | Out-Null
        Write-Host "Whisper STT is responding on port $SttPort."
        return $true
    }
    catch {
        Write-Host "Whisper STT port is open, but /health is not ready yet."
        return $false
    }
}

function Start-Stt() {
    if (-not (Test-Path -LiteralPath $SttPython)) {
        Write-Warning "Whisper STT is not installed. Run Scripts\Install-WhisperSTT.ps1, then start services again."
        return $false
    }
    if (-not (Test-Path -LiteralPath $SttServerScript)) {
        throw "Missing Whisper STT server script: $SttServerScript"
    }

    New-Item -ItemType Directory -Force -Path $LogsDir, $PidDir | Out-Null

    $args = @(
        "-u",
        $SttServerScript,
        "--port", "$SttPort",
        "--model", $SttModel,
        "--device", $SttDevice,
        "--compute-type", $SttComputeType,
        "--language", $SttLanguage,
        "--idle-unload-seconds", "$SttIdleUnloadSeconds"
    )

    Write-Host "Starting Whisper STT on http://127.0.0.1:$SttPort..."
    Write-Host "Model: $SttModel, device: $SttDevice, compute_type: $SttComputeType, language: $SttLanguage"
    $env:HF_HUB_DISABLE_XET = "1"
    $env:HF_HUB_DISABLE_SYMLINKS_WARNING = "1"
    $process = Start-Process `
        -FilePath $SttPython `
        -ArgumentList $args `
        -WorkingDirectory $ProjectRoot `
        -WindowStyle Hidden `
        -RedirectStandardOutput $SttStdoutLog `
        -RedirectStandardError $SttStderrLog `
        -PassThru

    Set-Content -LiteralPath $SttPidFile -Value $process.Id
    Write-Host "Whisper STT process started: PID $($process.Id)"
    Write-Host "Logs:"
    Write-Host "  $SttStdoutLog"
    Write-Host "  $SttStderrLog"
    return $true
}

if ($DryRun) {
    Write-Host "NaturalNPC service launcher dry run"
    Write-Host "ProjectRoot: $ProjectRoot"
    Write-Host "OllamaScript: $OllamaScript"
    Write-Host "OllamaModel: $OllamaModel"
    Write-Host "TTS Python: $Python"
    Write-Host "TTS Script: $TtsServerScript"
    Write-Host "STT Python: $SttPython"
    Write-Host "STT Script: $SttServerScript"
    Write-Host "Warmup: $Warmup"
    exit 0
}

Write-Step "Starting Ollama Vulkan"
if (-not (Test-Ollama)) {
    & $OllamaScript
}

Wait-TcpPort -Port $OllamaPort -TimeoutSeconds 20 -Name "Ollama"
Warm-OllamaModel

Write-Step "Starting Chatterbox TTS"
if (Test-TcpPort $TtsPort) {
    Write-Host "Chatterbox TTS port $TtsPort is already open."
}
else {
    Start-Tts
    Wait-TcpPort -Port $TtsPort -TimeoutSeconds 120 -Name "Chatterbox TTS"
}

$ttsReady = $false
$deadline = (Get-Date).AddSeconds(180)
while ((Get-Date) -lt $deadline) {
    if (Test-Tts) {
        $ttsReady = $true
        break
    }
    Start-Sleep -Seconds 2
}
if (-not $ttsReady) {
    throw "Chatterbox TTS health check failed within 180 seconds."
}

Write-Step "Starting Whisper STT"
if ($NoSTT) {
    Write-Host "Skipping Whisper STT."
}
elseif (Test-Stt) {
    Write-Host "Whisper STT port $SttPort is already ready."
}
else {
    $sttStarted = Start-Stt
    if ($sttStarted) {
        Wait-TcpPort -Port $SttPort -TimeoutSeconds 300 -Name "Whisper STT"
        Test-Stt | Out-Null
    }
}

Write-Step "NaturalNPC local services are ready"
Write-Host "Ollama:       http://127.0.0.1:$OllamaPort"
Write-Host "Chatterbox:   http://127.0.0.1:$TtsPort/tts"
Write-Host "Whisper STT:  http://127.0.0.1:$SttPort"
Write-Host "Ollama model: $OllamaModel"
