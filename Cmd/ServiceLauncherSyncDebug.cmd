
%~d0
cd %~dp0

set SERVICE_BIN_PATH=%~dp0..\Server\bin

xcopy %SERVICE_BIN_PATH%\x64Debug\BRServerInstanceLauncher.* %SERVICE_BIN_PATH% /Y /D
xcopy %SERVICE_BIN_PATH%\x64Debug\libcrypto-1_1-x64.* %SERVICE_BIN_PATH% /Y /D
xcopy %SERVICE_BIN_PATH%\x64Debug\libmysql.* %SERVICE_BIN_PATH% /Y /D
xcopy %SERVICE_BIN_PATH%\x64Debug\libssl-1_1-x64.* %SERVICE_BIN_PATH% /Y /D
xcopy %SERVICE_BIN_PATH%\x64Debug\zookeeper.* %SERVICE_BIN_PATH% /Y /D



pause