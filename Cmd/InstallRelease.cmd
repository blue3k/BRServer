
set COMMAND_ROOT=%~dp0
set SERVICE_COMMAND=install
set PATH_BIN=%COMMAND_ROOT%..\Server\bin\x64Release



call %COMMAND_ROOT%\_InstallServerServices.cmd

pause
