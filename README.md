# BRServer
Braves Server 

Visual Studio 2017 is required for linux 
Visual Studio 2015 is required for windows 

StormForge should be linked and prebuilt


BrServers.sln: Main server build, you can unload linux projectes if you don't need them
BrTools.sln: Server tools


BrServers.sln: 메인 빌드 프로젝트. 리눅스 프로젝트들은 필요없으면 언로드하여 사용하십시요.
BrTools.sln: 서버 툴 빌



Linking StormForge.
 - submodule haven't setup so use MakeJunctionSF.cmd or clone Stormforge to the root path


1. Build order(빌드 순서)
 - Build StormForge 3rdParties
 - Build StormForge
 - Sync StormForge libries(Run SyncSF3rdParites.cmd)
 - Build Tools
 - Build Main game server

 
2. DB setup
 - Install MySql 5.6 or higher
 - Unzip /Server/DBBackup/dbSchemaSQL.zip
 - Creates databases in each sql file. dbaccount, dbconspiracy00, dbgametransaction00, dbsession00, dbconspiracyranking, and dbconspiracytable
 - Run all sql files
 - PlayerID 1~10 is reserved, create fake player by calling spCreateSystemPlayer('Bot', 0, 10, 2)
 
Download: https://dev.mysql.com/downloads/mysql/ 
 
General Gude: https://corlewsolutions.com/articles/article-21-how-to-install-mysql-server-5-6-on-windows-7-development-machine 
 
 
Mandatory configs for server. These values should be set to the server config
아래 항목들은 필수 항목으로 mysql서버 설치후 설정파일에 넣어주십시요.

lower_case_table_name     = 1
character-set-server      = utf8
collation-server          = utf8_general_ci


 
3. Editing Config(서버 컨피그)
	- Open /Server/Config/ServerConfig.xml
	- Search all IP sections and replace them to yours 

	- /Server/Config/ServerConfig.xml을 에디터로 열기
	- 모든 IP관련된 항목을 자신의 IP로 교체

	
4. Running on windows
/Cmd: All commands for windows
	- RunDebugMode.cmd: debug mode server run
	- StopServers.cmd: stop all debug mode server instances
/LinuxBuild: all commands for linux
/Server/Config: all configs
	- ServerConfig.xml is for windows
	
	