

del /s %PATH_BIN%\..\log\*.log


rem start %PATH_BIN%\BREntityServer.Win.exe -debug -n:BREntityServer1

rem timeout /t 2 /nobreak

rem start %PATH_BIN%\BREntityServer.Win.exe -debug -n:BREntityServer2


rem timeout /t 4 /nobreak

start %PATH_BIN%\BRModuleServer.Win.exe -debug -n:BRLoginServer01

start %PATH_BIN%\BRModuleServer.Win.exe -debug -n:BRMatchingQueue01


timeout /t 2 /nobreak


start %PATH_BIN%\BRConspiracyGameServer.Win.exe -debug -n:BRConspiracyGameServer01

timeout /t 1 /nobreak

start %PATH_BIN%\BRConspiracyGameInstanceServer.Win.exe -debug -n:BRConspiracyGameInstanceServer01


