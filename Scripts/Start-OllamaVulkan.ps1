$ErrorActionPreference = "Stop"

$ollama = Join-Path $env:LOCALAPPDATA "Programs\Ollama\ollama.exe"
if (-not (Test-Path -LiteralPath $ollama)) {
    throw "Ollama was not found at: $ollama"
}

$ProjectRoot = Split-Path -Parent $PSScriptRoot
$PidDir = Join-Path $ProjectRoot "Saved\ServicePids"
$PidFile = Join-Path $PidDir "OllamaVulkan.pid"

try {
    $version = Invoke-RestMethod `
        -Uri "http://127.0.0.1:11435/api/version" `
        -TimeoutSec 2
    Write-Host "Ollama Vulkan is already running on port 11435 ($($version.version))."
    exit 0
}
catch {
    # Start a dedicated Vulkan service for Unreal Engine.
}

$env:OLLAMA_HOST = "127.0.0.1:11435"
$env:OLLAMA_LLM_LIBRARY = "vulkan"
$env:OLLAMA_KEEP_ALIVE = "10m"
$env:OLLAMA_CONTEXT_LENGTH = "4096"

New-Item -ItemType Directory -Force -Path $PidDir | Out-Null

$process = Start-Process `
    -FilePath $ollama `
    -ArgumentList "serve" `
    -WindowStyle Hidden `
    -PassThru

Set-Content -LiteralPath $PidFile -Value $process.Id

Start-Sleep -Seconds 3
$version = Invoke-RestMethod `
    -Uri "http://127.0.0.1:11435/api/version" `
    -TimeoutSec 10

Write-Host "Ollama Vulkan started on port 11435 ($($version.version))."
