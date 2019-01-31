
set COMMAND_ROOT=%~dp0

%~d0
cd %COMMAND_ROOT%..\External\zookeeper\bin\



cd %COMMAND_ROOT%..\External\kafka\bin\windows\
nssm install Kafka %COMMAND_ROOT%..\External\kafka\bin\windows\kafka-server-start.bat %COMMAND_ROOT%..\External\kafka\config\tools-log4j.properties
nssm set Kafka AppEnvironmentExtra  JAVA_HOME="%JAVA_HOME%"

