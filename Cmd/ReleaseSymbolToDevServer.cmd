
set COMMAND_ROOT=%~dp0
set RELEASE_MODE=x64Debug
set PATH_BIN=%COMMAND_ROOT%..\Server\bin\%RELEASE_MODE%
set PATH_DEVSERVER=106.247.237.180
set FTP_CMD_TMP=ftpcmd_symbol.dat

cd cmd

@echo off
echo user braves> %FTP_CMD_TMP%
echo braves1234>> %FTP_CMD_TMP%
echo bin>> %FTP_CMD_TMP%
echo hash>> %FTP_CMD_TMP%
echo prompt>> %FTP_CMD_TMP%
echo cd /gameserver/bin/%RELEASE_MODE%>> %FTP_CMD_TMP%
echo mput %PATH_BIN%\*.pdb>> %FTP_CMD_TMP%
echo quit>> %FTP_CMD_TMP%
ftp -n -s:%FTP_CMD_TMP% %PATH_DEVSERVER%
del %FTP_CMD_TMP%

pause
