param([string]$EngineRoot = "C:\Program Files\Epic Games\UE_5.8")
$ErrorActionPreference = "Stop"
$Project = (Resolve-Path (Join-Path $PSScriptRoot "..\HamsterExpress.uproject")).Path
$UAT = Join-Path $EngineRoot "Engine\Build\BatchFiles\RunUAT.bat"
if(!(Test-Path $UAT)) { throw "UE 5.8 RunUAT.bat not found: $UAT" }
$Archive = Join-Path (Split-Path $Project) "Build\Win64_Development"
& $UAT BuildCookRun -project="$Project" -noP4 -platform=Win64 -clientconfig=Development -build -cook -stage -pak -archive -archivedirectory="$Archive" -utf8output
if($LASTEXITCODE -ne 0){ throw "Unreal packaging failed with exit code $LASTEXITCODE" }
Write-Host "Build complete: $Archive" -ForegroundColor Green
