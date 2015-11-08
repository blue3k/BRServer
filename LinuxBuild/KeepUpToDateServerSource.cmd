
call %~dp0\EnvSetup.cmd

set FTP_CMD_TMP=buildcmd.dat
set LOCAL_PATH=%PATH_ROOT%

set RELATIVE_PATH=%1

set LOCAL_PATH=%LOCAL_BASE_PATH%/Server
set REMOTE_PATH=%REMOTE_BASE_PATH%/Server




set EXCLUDES_FILES=*.cmd;*.user;*.suo;*.sdf;*.opensdf;*.recipe;*.vs;*.obj;*.o;*.pdf;*.pch;*.tlb;*.lib;*.exe;*.dll;*.zip;*.log;*.idb;*.manifest;*.rc;.git/;ipch/;Debug/;Release/;x64Debug/;x64Release/;bin/;temp/;*.tlog/



echo keepuptodate %LOCAL_PATH% %REMOTE_PATH% -transfer=binary -filemask="|%EXCLUDES_FILES%"  > %FTP_CMD_TMP%

echo close>> %FTP_CMD_TMP%
echo exit>> %FTP_CMD_TMP%


winscp.com Linux /script=%FTP_CMD_TMP%

del %FTP_CMD_TMP%



