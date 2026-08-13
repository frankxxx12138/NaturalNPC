@echo off
setlocal
cd /d "%~dp0"
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0Scripts\Stop-NaturalNPCOpenAI.ps1" %*
echo.
echo OpenAI backend stopper finished.
pause
