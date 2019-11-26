

%~d0
cd %~dp0

set SERVICE_BIN_PATH=%~dp0..\Server\bin\x64Release

call ZookeeperServiceUninstall.cmd





%SERVICE_BIN_PATH%\BRServerLauncher.exe -uninstall -servicemode


pause