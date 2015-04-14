
set COMMAND_ROOT=%~dp0

rem set RELEASE_MODE=x64Debug
rem set RELEASE_EXTS=*.exe *.dll
rem set FTP_CMD_TMP=ftpcmd.dat

set PATH_BIN=%COMMAND_ROOT%..\Server\bin\%RELEASE_MODE%
set PATH_DEVSERVER=106.247.237.180

cd cmd

@echo off
echo user braves> %FTP_CMD_TMP%
echo braves1234>> %FTP_CMD_TMP%
echo bin>> %FTP_CMD_TMP%
echo hash>> %FTP_CMD_TMP%
echo prompt>> %FTP_CMD_TMP%
echo cd /gameserver/bin/%RELEASE_MODE%>> %FTP_CMD_TMP%

for %%n in (%RELEASE_EXTS%) do echo mput %PATH_BIN%\*.%%n>> %FTP_CMD_TMP%

rem echo mput %PATH_BIN%\*.exe>> %FTP_CMD_TMP%
rem echo mput %PATH_BIN%\*.dll>> %FTP_CMD_TMP%
echo quit>> %FTP_CMD_TMP%
ftp -n -s:%FTP_CMD_TMP% %PATH_DEVSERVER%
del %FTP_CMD_TMP%

echo %PATH_FILEZILLA%\filezilla ftp://braves:braves1234@106.247.237.180:21 --local=%PATH_BIN%

echo set COPY_OPTIONS=/e /xd obj /r:5 /w:1
echo Robocopy  %PATH_BIN%\ %PATH_DEVSERVER%\ %COPY_OPTIONS%


