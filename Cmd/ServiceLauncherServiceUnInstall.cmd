

%~d0
cd %~dp0

set SERVICE_BIN_PATH=%~dp0..\Server\bin\x64Release





%SERVICE_BIN_PATH%\BRServerInstanceLauncher.exe -uninstall -servicemode


pause