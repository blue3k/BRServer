

del /s %PATH_BIN%\..\log\*.log

start %PATH_BIN%\BRModuleServer.Win.exe -debug -n:BRLoginServer01

start %PATH_BIN%\BRModuleServer.Win.exe -debug -n:BRMatchingQueue01

start %PATH_BIN%\BRModuleServer.Win.exe -debug -n:BRMyTownHeroRanking


timeout /t 2 /nobreak


start %PATH_BIN%\BRConspiracyGameServer.Win.exe -debug -n:BRConspiracyGameServer01

timeout /t 1 /nobreak

start %PATH_BIN%\BRConspiracyGameInstanceServer.Win.exe -debug -n:BRConspiracyGameInstanceServer01


