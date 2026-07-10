@echo off
setlocal
cd /d "%~dp0"
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0Scripts\Stop-NaturalNPCServices.ps1"
echo.
echo NaturalNPC services stopper finished. You can close this window after checking the messages above.
pause
