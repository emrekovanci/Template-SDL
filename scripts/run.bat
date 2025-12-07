@echo off

:: Configs
set APP_PRESET_NAME=windows-ninja-clangcl
set APP_BUILD_TYPE=Debug
set APP_ARCH=x64
set APP_NAME=Application.exe
set MSVC_VERSION=18

set NO_CONFIGURE=0
if "%1"=="-no-configure" set NO_CONFIGURE=1

:: Paths
set CURRENT_DIR=%~dp0
for %%a in ("%CURRENT_DIR%\..") do set PROJECT_DIR=%%~fa

set BUILD_DIR=%PROJECT_DIR%\build\%APP_PRESET_NAME%
set BUILD_APP_DIR=%BUILD_DIR%\app
set BUILD_COMPILE_COMMANDS=%BUILD_DIR%\compile_commands.json
set TARGET_COMPILE_COMMANDS=%PROJECT_DIR%\compile_commands.json

echo ------------------------------------------------------
echo [run.bat] Project Dir:             %PROJECT_DIR%
echo [run.bat] Build Dir:               %BUILD_DIR%
echo [run.bat] Compile Commands Src:    %BUILD_COMPILE_COMMANDS%
echo [run.bat] Compile Commands Dst:    %TARGET_COMPILE_COMMANDS%
echo ------------------------------------------------------

:: Initialize MSVC environment
call "%ProgramFiles%\Microsoft Visual Studio\%MSVC_VERSION%\Community\VC\Auxiliary\Build\vcvarsall.bat" %APP_ARCH%

:: CMake Configure
if %NO_CONFIGURE%==0 (
    cmake --preset="%APP_PRESET_NAME%"
    if errorlevel 1 exit /b
)

:: CMake Build
cmake --build --preset="%APP_PRESET_NAME%" --config="%APP_BUILD_TYPE%"
if errorlevel 1 exit /b

:: Copy compile_commands.json
if exist "%TARGET_COMPILE_COMMANDS%" del "%TARGET_COMPILE_COMMANDS%"
copy /y "%BUILD_COMPILE_COMMANDS%" "%TARGET_COMPILE_COMMANDS%" >nul

:: Run Application
set APP_PATH=%BUILD_APP_DIR%\%APP_NAME%
start "" "%APP_PATH%"
