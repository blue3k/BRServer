
set COMMAND_ROOT=%~dp0

%~d0
cd %COMMAND_ROOT%..\External\zookeeper\bin\

nssm remove zookeeper
