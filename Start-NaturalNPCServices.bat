@echo off
setlocal
cd /d "%~dp0"
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0Scripts\Start-NaturalNPCServices.ps1"
echo.
echo NaturalNPC services launcher finished. You can close this window after checking the messages above.
pause
