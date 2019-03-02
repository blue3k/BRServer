
%~d0
cd %~dp0

set SERVICE_BIN_PATH=%~dp0..\Server\bin

xcopy %SERVICE_BIN_PATH%\x64Debug\*.dll %SERVICE_BIN_PATH% /Y /D
xcopy %SERVICE_BIN_PATH%\x64Debug\*.exe %SERVICE_BIN_PATH% /Y /D
xcopy %SERVICE_BIN_PATH%\x64Debug\*.pdb %SERVICE_BIN_PATH% /Y /D



pause