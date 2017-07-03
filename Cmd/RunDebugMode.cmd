
set COMMAND_ROOT=%~dp0

set PATH_BIN=%COMMAND_ROOT%..\Server\bin\x64Debug
set PATH_CONFIG=%COMMAND_ROOT%..\Server\Config

if not exist %PATH_CONFIG%\ServerConfig.xml (
    copy %PATH_CONFIG%\ServerConfig_winsample.xml %PATH_CONFIG%\ServerConfig.xml
	call notepad %PATH_CONFIG%\ServerConfig.xml
) else (
    rem file doesn't exist
)

call _RunServers.cmd
