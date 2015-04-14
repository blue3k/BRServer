
set COMMAND_ROOT=%~dp0

set PATH_BIN=%COMMAND_ROOT%..\Server\bin\%RELEASE_MODE%
set PATH_SYMBOL=D:\SymbolServer


set SRCSRV_INI_FILE=srcsrv.ini
set SRC_PATH=.\
set PATH=%PATH%;"C:\Program Files (x86)\Windows Kits\8.1\Debuggers\x64\srcsrv";"C:\Program Files (x86)\Windows Kits\8.1\Debuggers\x64"

cd cmd
cd ..
call p4index -ini=%SRCSRV_INI_FILE% -source=%SRC_PATH% -symbols=%PATH_BIN% -debug 

echo Server symstore...
symstore add /r /f %PATH_BIN%\*.exe /s %PATH_SYMBOL% /t "BRServer"
symstore add /r /f %PATH_BIN%\*.dll /s %PATH_SYMBOL% /t "BRServer"
symstore add /r /f %PATH_BIN%\*.pdb /s %PATH_SYMBOL% /t "BRServer"

cd cmd



