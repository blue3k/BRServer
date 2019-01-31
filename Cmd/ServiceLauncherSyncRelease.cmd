
%~d0
cd %~dp0

set SERVICE_BIN_PATH=%~dp0..\Server\bin

xcopy %SERVICE_BIN_PATH%\x64Release\BRServerInstanceLauncher.* %SERVICE_BIN_PATH% /Y /D
xcopy %SERVICE_BIN_PATH%\x64Release\libcrypto-1_1-x64.* %SERVICE_BIN_PATH% /Y /D
xcopy %SERVICE_BIN_PATH%\x64Release\libmysql.* %SERVICE_BIN_PATH% /Y /D
xcopy %SERVICE_BIN_PATH%\x64Release\libssl-1_1-x64.* %SERVICE_BIN_PATH% /Y /D
xcopy %SERVICE_BIN_PATH%\x64Release\zookeeper.* %SERVICE_BIN_PATH% /Y /D



pause