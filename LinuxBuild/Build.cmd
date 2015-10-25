

set PATH=%PATH%;"C:\Program Files (x86)\WinSCP";""C:\Program Files (x86)\Mono\bin""
set COMMAND_ROOT=%~dp0
set PROJECT_BASE_PATH=%1
set PROJECT_NAME=%2
set MAKE_BUILD_TARGET=%3
set PATH_ROOT=%COMMAND_ROOT%..
set REMOTE_BASE_PATH=/home/blue3k/work/test
set BUILD_MODE=DEBUG
set FTP_CMD_TMP=buildcmd.dat

set LOCAL_PATH=%PATH_ROOT%\%PROJECT_BASE_PATH%\%PROJECT_NAME%
if /i %PROJECT_BASE_PATH% == .       set LOCAL_PATH=%PATH_ROOT%\%PROJECT_NAME%

set REMOTE_PATH=%REMOTE_BASE_PATH%/%PROJECT_BASE_PATH%/%PROJECT_NAME%
if /i %PROJECT_BASE_PATH% == .       set REMOTE_PATH=%REMOTE_BASE_PATH%/%PROJECT_NAME%


call %COMMAND_ROOT%/UploadSource.cmd


echo build remotely


plink.exe blue3k@LocalLinux "cd %REMOTE_PATH%;export PROJECT_NAME=%PROJECT_NAME%;export BUILD_MODE=%BUILD_MODE%; make -f makefile.mak %MAKE_BUILD_TARGET%" 
