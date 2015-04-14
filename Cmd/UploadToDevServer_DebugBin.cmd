
set COMMAND_ROOT=%~dp0

set RELEASE_MODE=x64Debug
set RELEASE_EXTS=exe dll
set FTP_CMD_TMP=ftpcmd_DebugBin.dat

call _IndexNSymStore.cmd
call _UploadToDevServer.cmd

pause
