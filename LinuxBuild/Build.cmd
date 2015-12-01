


rem call %~dp0\UploadSource.cmd . 

call %~dp0\EnvSetup.cmd

rem relative path from root
set PROJECT_BASE_PATH=%1
rem project name which will be target name
set PROJECT_NAME=%2
rem DEBUG, RELEASE
set BUILD_MODE=%3
rem build, clean
set MAKE_BUILD_TARGET=%4

if "%1"=="" goto error
if "%2"=="" goto error
if "%3"=="" goto error
if "%4"=="" goto error

set REMOTE_PATH=%REMOTE_BASE_PATH%/%PROJECT_BASE_PATH%/%PROJECT_NAME%
if /i %PROJECT_BASE_PATH% == .       set REMOTE_PATH=%REMOTE_BASE_PATH%/%PROJECT_NAME%

@echo REMOTE_PATH: %REMOTE_PATH%
@echo build remotely



plink.exe blue3k@LinuxBuild "cd %REMOTE_PATH%;export ROOT_PATH=%REMOTE_BASE_PATH%;export PROJECT_NAME=%PROJECT_NAME%;export BUILD_MODE=%BUILD_MODE%; make -f makefile.mak %MAKE_BUILD_TARGET%" 


@echo Done %PROJECT_NAME%
