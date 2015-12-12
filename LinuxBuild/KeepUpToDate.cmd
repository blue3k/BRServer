
call %~dp0\EnvSetup.cmd

set FTP_CMD_TMP=buildcmd.dat
set LOCAL_PATH=%PATH_ROOT%


set LOCAL_PATH=%LOCAL_BASE_PATH%\%1
if "%1"=="" or "%1" == "." set LOCAL_PATH=%LOCAL_BASE_PATH%\

set REMOTE_PATH=%REMOTE_BASE_PATH%/%1
if "%1"=="" or "%1" == "." REMOTE_PATH=%REMOTE_BASE_PATH%/

@echo %LOCAL_PATH% => %REMOTE_PATH%



set EXCLUDES_FILES=*.cmd;*.user;*.suo;*.sdf;*.opensdf;*.recipe;*.vs;*.obj;*.o;*.pdf;*.pch;*.pdb;*.tlb;*.lib;*.exe;*.dll;*.zip;*.log;*.idb;*.manifest;*.rc;*.opendb/.git/;ipch/;Debug/;Release/;DEBUG/;RELEASE/;x64Debug/;x64Release/;bin/;obj/;temp/;*.tlog/


echo synchronize remote %LOCAL_PATH% %REMOTE_PATH% -delete -transfer=binary -filemask="|%EXCLUDES_FILES%" > %FTP_CMD_TMP%

echo keepuptodate %LOCAL_PATH% %REMOTE_PATH% -delete -transfer=binary -filemask="|%EXCLUDES_FILES%" >> %FTP_CMD_TMP%


winscp.com Linux /script=%FTP_CMD_TMP%




