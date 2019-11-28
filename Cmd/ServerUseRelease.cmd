
%~d0
cd %~dp0

set SERVICE_BIN_PATH=%~dp0..\Server\bin
set SERVICE_BUILD_PTAH=%~dp0..\buildWindows\x64Release\bin
set SF_BIN_PTAH=%~dp0..\Server\bin\x64Release

robocopy %SERVICE_BUILD_PTAH%\ %SERVICE_BIN_PATH% *.dll *.exe *.pdb
robocopy %SF_BIN_PTAH%\ %SERVICE_BIN_PATH% *.dll *.exe *.pdb


pause