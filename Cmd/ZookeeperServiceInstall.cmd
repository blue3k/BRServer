
set COMMAND_ROOT=%~dp0

%~d0
cd %COMMAND_ROOT%..\External\zookeeper\bin\


nssm install zookeeper %COMMAND_ROOT%..\External\zookeeper\bin\zkServer.cmd
nssm set zookeeper AppEnvironmentExtra  JAVA_HOME="%JAVA_HOME%"

