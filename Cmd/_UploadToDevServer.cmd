rem DO NOT USE, Only for example

set COMMAND_ROOT=%~dp0
set PATH_BIN=%COMMAND_ROOT%..\Server\bin\%RELEASE_MODE%

cd cmd

@echo off

echo synchronize -transfer=binary -filemask=%INCLUDE_FILES% remote %PATH_BIN% /gameserver/bin/%RELEASE_MODE% > %FTP_CMD_TMP%

rem echo cd /gameserver/bin/%RELEASE_MODE%> %FTP_CMD_TMP%
rem for %%n in (%RELEASE_EXTS%) do echo put -transfer=binary %PATH_BIN%\*.%%n>> %FTP_CMD_TMP%

echo close>> %FTP_CMD_TMP%
echo exit>> %FTP_CMD_TMP%


winscp.com ftp://braves:braves1234@%PATH_SERVER%:%PATH_SERVER_PORT% /passive=on /script=%FTP_CMD_TMP%

del %FTP_CMD_TMP%

cmd



