

del /s %PATH_BIN%\..\log\*.txt


start %PATH_BIN%\BREntityServer.exe -debug -n:BREntityServer1

timeout /t 2 /nobreak

start %PATH_BIN%\BREntityServer.exe -debug -n:BREntityServer2


timeout /t 4 /nobreak

start %PATH_BIN%\BRModuleServer.exe -debug -n:BRLoginServer01

start %PATH_BIN%\BRModuleServer.exe -debug -n:BRMatchingQueue01


timeout /t 2 /nobreak


start %PATH_BIN%\BRConspiracyGameServer.exe -debug -n:BRConspiracyGameServer01
start %PATH_BIN%\BRConspiracyGameServer.exe -debug -n:BRConspiracyGameServer02

timeout /t 1 /nobreak

start %PATH_BIN%\BRConspiracyGameInstanceServer.exe -debug -n:BRConspiracyGameInstanceServer01
start %PATH_BIN%\BRConspiracyGameInstanceServer.exe -debug -n:BRConspiracyGameInstanceServer02


