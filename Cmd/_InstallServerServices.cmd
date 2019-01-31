



%PATH_BIN%\BRLoginServer.exe -%SERVICE_COMMAND% -servicemode -n:BRLoginServer01
%PATH_BIN%\BRLoginServer.exe -%SERVICE_COMMAND% -servicemode -n:BRLoginServer02

%PATH_BIN%\BRSharedModuleServer.exe -%SERVICE_COMMAND% -servicemode -n:BRPartyManager01
%PATH_BIN%\BRSharedModuleServer.exe -%SERVICE_COMMAND% -servicemode -n:BRPartyManager02

%PATH_BIN%\BRSharedModuleServer.exe -%SERVICE_COMMAND% -servicemode -n:BRMatchingQueue01
%PATH_BIN%\BRSharedModuleServer.exe -%SERVICE_COMMAND% -servicemode -n:BRMatchingQueue02

%PATH_BIN%\BRSharedModuleServer.exe -%SERVICE_COMMAND% -servicemode -n:BRMatching01

%PATH_BIN%\BRConspiracyGameServer.exe -%SERVICE_COMMAND% -servicemode -n:BRConspiracyGameServer01
%PATH_BIN%\BRConspiracyGameServer.exe -%SERVICE_COMMAND% -servicemode -n:BRConspiracyGameServer02

%PATH_BIN%\BRConspiracyGameInstanceServer.exe -%SERVICE_COMMAND% -servicemode -n:BRConspiracyGameInstanceServer01
%PATH_BIN%\BRConspiracyGameInstanceServer.exe -%SERVICE_COMMAND% -servicemode -n:BRConspiracyGameInstanceServer02



