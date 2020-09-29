


set CMAKE_SYSTEM_NAME=Windows
set PROCESS_ARCHITECTUR=x64

call ..\StormForge\3rdParties\FindVC.cmd

if not exist build%CMAKE_SYSTEM_NAME% mkdir build%CMAKE_SYSTEM_NAME%
set BUILD_DIR=build%CMAKE_SYSTEM_NAME%\%PROCESS_ARCHITECTUR%


if not exist %BUILD_DIR% mkdir %BUILD_DIR%
REM if not exist %BUILD_DIR%\%PROCESS_ARCHITECTUR%Debug mkdir %BUILD_DIR%\%PROCESS_ARCHITECTUR%Debug
REM if not exist %BUILD_DIR%\%PROCESS_ARCHITECTUR%Release mkdir %BUILD_DIR%\%PROCESS_ARCHITECTUR%Release


cd %BUILD_DIR%

cmake ../../ -G %VCCMAKEGENERATOR% -A %PROCESS_ARCHITECTUR% ^
	-DCMAKE_INSTALL_PREFIX=../../build%CMAKE_SYSTEM_NAME%/%PROCESS_ARCHITECTUR% 


cd %~dp0

if not "%1" == "nopause" pause
