

del /s %PATH_BIN%\..\log\*.log

start %PATH_BIN%\BRModuleServer.exe -n:BRLoginServer01

start %PATH_BIN%\BRModuleServer.exe -n:BRMatchingQueue01

start %PATH_BIN%\BRModuleServer.exe -n:BRMyTownHeroRanking


timeout /t 1 /nobreak


start %PATH_BIN%\BRConspiracyGameServer.exe -n:BRConspiracyGameServer01

timeout /t 1 /nobreak

start %PATH_BIN%\BRConspiracyGameInstanceServer.exe -n:BRConspiracyGameInstanceServer01


