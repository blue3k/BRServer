
set COMMAND_ROOT=%~dp0

PATH=%PATH%;"C:\Program Files (x86)\WinSCP"

set RELEASE_MODE=x64Release
set INCLUDE_FILES="*.pdb"
set FTP_CMD_TMP=ftpcmd_ReleaseSym.dat
set PATH_SERVER=119.205.221.125
set PATH_SERVER_PORT=10122

call _UploadToDevServer.cmd

pause
