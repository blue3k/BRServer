
%~d0
cd %~dp0

set SERVICE_BIN_PATH=%~dp0..\Server\bin




%SERVICE_BIN_PATH%\BRServerLauncher.exe -install -servicemode -logpath:log -modulepath:x64Debug -zkaddress:127.0.0.1:2181 -zkconfig:/ServerConfig


pause