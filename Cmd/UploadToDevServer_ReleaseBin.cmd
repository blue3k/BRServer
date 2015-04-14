
set COMMAND_ROOT=%~dp0

set RELEASE_MODE=x64Release
set RELEASE_EXTS=exe dll
set FTP_CMD_TMP=ftpcmd_ReleaseBin.dat

call _IndexNSymStore.cmd
call _UploadToDevServer.cmd

pause
