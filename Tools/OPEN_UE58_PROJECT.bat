@echo off
setlocal
set "PROJ=%~dp0..\HamsterExpress.uproject"
set "UE=C:\Program Files\Epic Games\UE_5.8\Engine\Binaries\Win64\UnrealEditor.exe"
if exist "%UE%" (
  start "" "%UE%" "%PROJ%"
  exit /b 0
)
echo Unreal Engine 5.8 was not found at the default Epic Games path.
echo Install UE 5.8 in Epic Games Launcher, or edit this BAT with your UnrealEditor.exe path.
pause
