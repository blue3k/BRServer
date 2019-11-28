
%~d0
cd %~dp0

set SERVICE_BIN_PATH=%~dp0..\Server\bin
set SERVICE_BUILD_PTAH=%~dp0..\buildWindows\x64Debug\bin

robocopy %SERVICE_BUILD_PTAH%\ %SERVICE_BIN_PATH% *.dll *.exe *.pdb



pause