
set COMMAND_ROOT=%~dp0
PATH=%PATH%;"C:\Program Files (x86)\WinSCP"

set RELEASE_MODE=x64Debug
set INCLUDE_FILES="*.pdb"
set FTP_CMD_TMP=ftpcmd_DebugSym.dat
set PATH_SERVER=106.247.237.180
set PATH_SERVER_PORT=21


call _UploadToDevServer.cmd

pause
