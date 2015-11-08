
call %~dp0\EnvSetup.cmd

set FTP_CMD_TMP=buildcmd.dat
set LOCAL_PATH=%PATH_ROOT%

set RELATIVE_PATH=%1

set LOCAL_PATH=%LOCAL_BASE_PATH%/%1
if /i %1 == .       set LOCAL_PATH=%LOCAL_BASE_PATH%

set REMOTE_PATH=%REMOTE_BASE_PATH%/%1
if /i %1 == .       set REMOTE_PATH=%REMOTE_BASE_PATH%




set EXCLUDES_FILES=*.cmd;*.user;*.suo;*.sdf;*.opensdf;*.recipe;*.vs;*.obj;*.o;*.pdf;*.pch;*.tlb;*.lib;*.exe;*.dll;*.zip;*.log;*.idb;*.manifest;*.rc;.git/;ipch/;Debug/;Release/;x64Debug/;x64Release/;bin/;temp/;*.tlog/



echo synchronize -transfer=binary -filemask="|%EXCLUDES_FILES%" remote %LOCAL_PATH% %REMOTE_PATH% > %FTP_CMD_TMP%

echo close>> %FTP_CMD_TMP%
echo exit>> %FTP_CMD_TMP%


winscp.com Linux /script=%FTP_CMD_TMP%

del %FTP_CMD_TMP%



