
set COMMAND_ROOT=%~dp0

set RELEASE_MODE=x64Release
set RELEASE_EXTS=pdb
set FTP_CMD_TMP=ftpcmd_ReleaseSym.dat

call _UploadToDevServer.cmd

pause
