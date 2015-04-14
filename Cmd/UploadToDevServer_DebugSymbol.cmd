
set COMMAND_ROOT=%~dp0

set RELEASE_MODE=x64Debug
set RELEASE_EXTS=pdb
set FTP_CMD_TMP=ftpcmd_DebugSym.dat

call _UploadToDevServer.cmd

pause
