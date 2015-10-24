
set COMMAND_ROOT=%~dp0

set PATH_BIN=%COMMAND_ROOT%..\Server\bin\x64Debug

del /s %PATH_BIN%\..\log\*.txt

start %PATH_BIN%\BREntityServer.exe -debug -n:BREntityServer1
timeout /t 1 /nobreak
start %PATH_BIN%\BREntityServer.exe -debug -n:BREntityServer2



