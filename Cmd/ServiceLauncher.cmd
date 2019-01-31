
%~d0
cd %~dp0

set SERVICE_BIN_PATH=%~dp0..\Server\bin

cd %SERVICE_BIN_PATH%

start BRServerInstanceLauncher.exe -logpath:log -modulepath:x64Debug -zkaddress:127.0.0.1:2181 -zkconfig:/ServerConfig


