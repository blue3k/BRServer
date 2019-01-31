
set COMMAND_ROOT=%~dp0

%~d0
cd %COMMAND_ROOT%..\External\kafka\bin\windows\

nssm remove Kafka
