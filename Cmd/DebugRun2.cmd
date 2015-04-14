
set COMMAND_ROOT=%~dp0

set PATH_BIN=%COMMAND_ROOT%..\bin\x64Debug

start %PATH_BIN%\BREntityServer.exe -debug -n:BREntityServer2

timeout /t 6 /nobreak

echo start %PATH_BIN%\BREntityServer.exe -debug -n:BREntityServer2
echo start %PATH_BIN%\BREntityServer.exe -debug -n:BREntityServer3

timeout /t 1 /nobreak


echo %PATH_BIN%\BRGameServer.exe -debug

