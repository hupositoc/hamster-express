$ErrorActionPreference = "SilentlyContinue"
Write-Host "=== Hamster Express UE5.8 Preflight ===" -ForegroundColor Cyan
$ue = "C:\Program Files\Epic Games\UE_5.8\Engine\Binaries\Win64\UnrealEditor.exe"
Write-Host ("UE 5.8: " + $(if(Test-Path $ue){"FOUND"}else{"NOT FOUND"}))
$gpu = Get-CimInstance Win32_VideoController | Select-Object Name,AdapterRAM,DriverVersion
$cpu = Get-CimInstance Win32_Processor | Select-Object Name,NumberOfCores,NumberOfLogicalProcessors
$ram = [math]::Round((Get-CimInstance Win32_ComputerSystem).TotalPhysicalMemory/1GB,1)
Write-Host "RAM: $ram GB"
$cpu | Format-Table -AutoSize
$gpu | Format-Table -AutoSize
Write-Host "Recommended: UE 5.8 + VS2022 Game development with C++ + latest NVIDIA driver."
