
set COMMAND_ROOT=%~dp0
set SERVICE_COMMAND=install
set PATH_BIN=%COMMAND_ROOT%..\Server\bin\x64Debug



call %COMMAND_ROOT%\_InstallServerServices.cmd

pause
