@echo off
setlocal EnableExtensions EnableDelayedExpansion

set "PROJECT_DIR=%~dp0"
set "UPROJECT=%PROJECT_DIR%ShootThemUp.uproject"
set "OUTPUT_DIR=%PROJECT_DIR%Builded"
set "PROJECT_TARGET=ShootThemUp"
set "ENGINE_DIR="
set "KEEP_OPEN=1"
set "USE_ALL_MAPS=0"
set "MAPS="
set "MAPS_SOURCE=PackagingSettings"
set "CLEAN_OUTPUT=1"
set "DRY_RUN=0"

:parse_args
if "%~1"=="" goto args_done
if /I "%~1"=="-nopause" set "KEEP_OPEN=0"
if /I "%~1"=="-allmaps" set "USE_ALL_MAPS=1"
if /I "%~1"=="-noclean" set "CLEAN_OUTPUT=0"
if /I "%~1"=="-dryrun" set "DRY_RUN=1"
set "ARG=%~1"
if /I "%ARG:~0,6%"=="-maps=" set "MAPS=%ARG:~6%"
if /I "%ARG:~0,8%"=="-engine=" set "ENGINE_DIR=%ARG:~8%"
shift
goto parse_args

:args_done
if not exist "%UPROJECT%" (
    echo [ERROR] Uproject not found: "%UPROJECT%"
    set "EXIT_CODE=1"
    goto :finish
)

if not defined ENGINE_DIR (
    for /f "delims=" %%I in ('powershell -NoProfile -Command "$project = Get-Content -Raw -Path $env:UPROJECT | ConvertFrom-Json; $assoc = [string]$project.EngineAssociation; if ($assoc) { $reg = Get-ItemProperty -Path ''HKCU:\Software\Epic Games\Unreal Engine\Builds'' -ErrorAction SilentlyContinue; if ($null -ne $reg) { $prop = $reg.PSObject.Properties[$assoc]; if ($null -ne $prop) { $prop.Value } } }" 2^>nul') do set "ENGINE_DIR=%%I"
)
if not defined ENGINE_DIR if defined UE_ENGINE_DIR set "ENGINE_DIR=%UE_ENGINE_DIR%"
if not defined ENGINE_DIR set "ENGINE_DIR=C:\UE_Engines\UE_56Source\UnrealEngine"

set "RUNUAT=%ENGINE_DIR%\Engine\Build\BatchFiles\RunUAT.bat"
if not exist "%RUNUAT%" (
    echo [ERROR] RunUAT not found: "%RUNUAT%"
    echo [ERROR] Use -engine=... or set UE_ENGINE_DIR.
    set "EXIT_CODE=1"
    goto :finish
)

if "%USE_ALL_MAPS%"=="0" if not defined MAPS if exist "%PROJECT_DIR%Config\DefaultGame.ini" call :read_packaged_maps

if "%USE_ALL_MAPS%"=="0" if not defined MAPS (
    set "MAPS=/Game/Menu/Menu"
    set "MAPS_SOURCE=FallbackDefaultMap"
)

if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"

if "%USE_ALL_MAPS%"=="1" (
    set "MAPS_ARG=-allmaps"
    set "MAPS_SOURCE=AllMaps"
) else (
    set "MAPS_ARG=-map=%MAPS%"
)

echo [INFO] Engine : "%ENGINE_DIR%"
echo [INFO] Project: "%UPROJECT%"
echo [INFO] Output : "%OUTPUT_DIR%"
echo [INFO] Maps   : %MAPS_ARG%
echo [INFO] MapsSrc: %MAPS_SOURCE%
echo [INFO] Mode   : Light Shipping (no debug info, no editor content)

if "%DRY_RUN%"=="1" (
    echo [INFO] Dry run enabled. Exiting without build.
    set "EXIT_CODE=0"
    goto :finish
)

if "%CLEAN_OUTPUT%"=="1" if exist "%OUTPUT_DIR%\Windows" (
    echo [INFO] Cleaning old output "%OUTPUT_DIR%\Windows"
    rmdir /s /q "%OUTPUT_DIR%\Windows"
)

call "%RUNUAT%" -nocompileuat -NoCompile BuildCookRun ^
 -project="%UPROJECT%" ^
 -noP4 ^
 -platform=Win64 ^
 -target="%PROJECT_TARGET%" ^
 -clientconfig=Shipping ^
 -build ^
 -cook ^
 -stage ^
 -pak ^
 -iostore ^
 -compressed ^
 -archive ^
 -nodebuginfo ^
 -SkipCookingEditorContent ^
 -nocrashreporter ^
 -archivedirectory="%OUTPUT_DIR%" ^
 %MAPS_ARG% ^
 -utf8output

set "EXIT_CODE=%ERRORLEVEL%"
if not "%EXIT_CODE%"=="0" (
    echo [ERROR] Shipping build failed. Exit code: %EXIT_CODE%
    goto :finish
)

echo [OK] Shipping build completed successfully.
echo [OK] Build output: "%OUTPUT_DIR%\Windows"
echo [OK] Launcher   : "%OUTPUT_DIR%\Windows\ShootThemUp.exe"
goto :finish

:read_packaged_maps
for /f usebackq^ tokens^=2^ delims^=^" %%M in (`findstr /C:"+MapsToCook=" "%PROJECT_DIR%Config\DefaultGame.ini"`) do (
    if defined MAPS (
        set "MAPS=!MAPS!+%%M"
    ) else (
        set "MAPS=%%M"
    )
)
exit /b 0

:finish
if not defined EXIT_CODE set "EXIT_CODE=0"
if "%KEEP_OPEN%"=="1" pause
exit /b %EXIT_CODE%
