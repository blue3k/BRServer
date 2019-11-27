
%~d0
cd %~dp0

set SERVICE_BIN_PATH=%~dp0..\Server\bin
set SERVICE_BUILD_PTAH=%~dp0..\buildWindows\x64Debug\bin

xcopy %SERVICE_BUILD_PTAH%\*.dll %SERVICE_BIN_PATH% /Y /D
xcopy %SERVICE_BUILD_PTAH%\*.exe %SERVICE_BIN_PATH% /Y /D
xcopy %SERVICE_BUILD_PTAH%\*.pdb %SERVICE_BIN_PATH% /Y /D



pause