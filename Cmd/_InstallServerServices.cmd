



%PATH_BIN%\BRLoginServer.exe -%SERVICE_COMMAND% -n:BRLoginServer01
%PATH_BIN%\BRLoginServer.exe -%SERVICE_COMMAND% -n:BRLoginServer02

%PATH_BIN%\BRSharedModuleServer.exe -%SERVICE_COMMAND% -n:BRPartyManager01
%PATH_BIN%\BRSharedModuleServer.exe -%SERVICE_COMMAND% -n:BRPartyManager02

%PATH_BIN%\BRSharedModuleServer.exe -%SERVICE_COMMAND% -n:BRMatchingQueue01
%PATH_BIN%\BRSharedModuleServer.exe -%SERVICE_COMMAND% -n:BRMatchingQueue02

%PATH_BIN%\BRSharedModuleServer.exe -%SERVICE_COMMAND% -n:BRMatching01

%PATH_BIN%\BRConspiracyGameServer.exe -%SERVICE_COMMAND% -n:BRConspiracyGameServer01
%PATH_BIN%\BRConspiracyGameServer.exe -%SERVICE_COMMAND% -n:BRConspiracyGameServer02

%PATH_BIN%\BRConspiracyGameInstanceServer.exe -%SERVICE_COMMAND% -n:BRConspiracyGameInstanceServer01
%PATH_BIN%\BRConspiracyGameInstanceServer.exe -%SERVICE_COMMAND% -n:BRConspiracyGameInstanceServer02



