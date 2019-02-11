rem DO NOT USE, Only for example

del /s %PATH_BIN%\..\log\*.log

start %PATH_BIN%\BRModuleServer.exe -servicename:BRLoginServer01

start %PATH_BIN%\BRModuleServer.exe -servicename:BRMatchingQueue01

start %PATH_BIN%\BRModuleServer.exe -servicename:BRMyTownHeroRanking


timeout /t 1 /nobreak


start %PATH_BIN%\BRConspiracyGameServer.exe -servicename:BRConspiracyGameServer01

timeout /t 1 /nobreak

start %PATH_BIN%\BRConspiracyGameInstanceServer.exe -servicename:BRConspiracyGameInstanceServer01


