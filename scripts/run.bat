@echo off

set APP_PRESET_NAME=windows-ninja-clangcl
set APP_BUILD_TYPE=Debug
set APP_ARCH=x64
set APP_NAME=Application.exe
set MSVC_VERSION=18

set NO_CONFIGURE=0
if "%1"=="-no_configure" set NO_CONFIGURE=1

set CURRENT_DIR=%~dp0
for %%a in ("%CURRENT_DIR%\..") do set PROJECT_DIR=%%~fa

:: vcvarsall call to initialize environment for x64
call "%ProgramFiles%\Microsoft Visual Studio\%MSVC_VERSION%\Community\VC\Auxiliary\Build\vcvarsall.bat" %APP_ARCH%

:: cmake configure
if %NO_CONFIGURE%==0 (
    cmake --preset="%APP_PRESET_NAME%"
    if errorlevel 1 goto :end
) else (
    echo [run.bat] Skipping CMake configure due to -no_configure
)

:: cmake build
cmake --build --preset="%APP_PRESET_NAME%" --config="%APP_BUILD_TYPE%"
if errorlevel 1 goto :end

:: run the app
start "" "%PROJECT_DIR%\build\%APP_PRESET_NAME%\app\%APP_NAME%"

:end
