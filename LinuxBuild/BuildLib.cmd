

call EnvSetup.cmd

set PROJECT_BASE_PATH=%1
set PROJECT_NAME=%2
set MAKE_BUILD_TARGET=%3
set BUILD_MODE=%4

if "%1"=="" goto error
if "%2"=="" goto error
if "%3"=="" goto error
if "%4"=="" goto error

set REMOTE_PATH=%REMOTE_BASE_PATH%/%PROJECT_BASE_PATH%/%PROJECT_NAME%
if /i %PROJECT_BASE_PATH% == .       set REMOTE_PATH=%REMOTE_BASE_PATH%/%PROJECT_NAME%


@echo build remotely


plink.exe blue3k@LocalLinux "cd %REMOTE_PATH%;export PROJECT_NAME=%PROJECT_NAME%;export BUILD_MODE=%BUILD_MODE%; make -f makefile.mak %MAKE_BUILD_TARGET%" 

goto quit


error:

@echo Failed %PROJECT_NAME%

quit:

@echo Done %PROJECT_NAME%