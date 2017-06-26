# BRServer
Braves Server 

Visual Studio 2017 is required

BrExternalBuild.sln: Build 3rdparites libraries, you can unload linux projectes if you don't need them
BrServers.sln: Main server build
BrTools.sln: Server tools

1. Build
 - Build External libraries
 - Build Tools
 - Build Main game server
 
2. DB setup
 - Install MySql 5.6 or higher
 - Unzip /Server/DBBackup/dbSchemaSQL.zip
 - Creates databases in each sql file. dbaccount, dbconspiracy00, dbgametransaction00, dbsession00, dbconspiracyranking, and dbconspiracytable
 - Run all sql files
 
Download: https://dev.mysql.com/downloads/mysql/ 
 
General Gude: https://corlewsolutions.com/articles/article-21-how-to-install-mysql-server-5-6-on-windows-7-development-machine 
 
Mandatory configs for server. These values should be set to the server config
 
lower_case_table_name     = 1
character-set-server      = utf8
collation-server          = utf8_general_ci
 
3. Editing Config
	- Open /Server/Config/ServerConfig.xml
	- Search all IP sections and replace them to yours 
 
4. Running on windows
/Cmd: All commands for windows
	- RunDebugMode.cmd: debug mode server run
	- StopServers.cmd: stop all debug mode server instances
/LinuxBuild: all commands for linux
/Server/Config: all configs
	- ServerConfig.xml is for windows
	
	