
set COMMAND_ROOT=%~dp0


start %COMMAND_ROOT%..\External\zookeeper\bin\zkServer.cmd

timeout 5

%COMMAND_ROOT%..\External\kafka\bin\windows\kafka-server-start.bat %COMMAND_ROOT%..\External\kafka\config\server.properties

