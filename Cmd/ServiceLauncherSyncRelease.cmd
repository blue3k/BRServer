
%~d0
cd %~dp0

set SERVICE_BIN_PATH=%~dp0..\Server\bin

xcopy %SERVICE_BIN_PATH%\x64Release\*.dll %SERVICE_BIN_PATH% /Y /D
xcopy %SERVICE_BIN_PATH%\x64Release\*.exe %SERVICE_BIN_PATH% /Y /D
xcopy %SERVICE_BIN_PATH%\x64Release\*.pdb %SERVICE_BIN_PATH% /Y /D


pause