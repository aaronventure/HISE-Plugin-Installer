@echo off
set "script_dir=%~dp0"
set "projucer_path=Projucer_Win64.exe"
set "msbuild=C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MsBuild.exe"

REM Echo the paths to verify
echo Projucer Path: %projucer_path%

REM Build
"%projucer_path%" --resave "Installer.jucer"
powershell -ExecutionPolicy Bypass -File "Set_UAC_Execution_Level.ps1"
"Builds\VisualStudio2022\Installer.sln"