
set COMMAND_ROOT=%~dp0

start %COMMAND_ROOT%..\External\kafka\bin\windows\kafka-server-start.bat %COMMAND_ROOT%..\External\kafka\config\server.properties

