# BRServer
Braves Server 

Prerequisits
	- Visual Studio 2017 is required
	- JDK jdk7 or higher: http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html
	- Perl: You need perl to build openssl. https://www.activestate.com/activeperl/downloads
	- StormForge should be cloned at ..\StormForge


Use Setup.cmd to setup StormForge



Linux build
1. CMakeLinuxGen.sh to generate build project
2. CMakeLinuxBuild.sh to build

Windows build
1. CMakeWinGen.cmd to generate build project
2. CMakeWinBuild.cmd to build
3. BrServers.sln: Main server build, you can unload linux projectes if you don't need them
BrTools.sln: Server tools


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
 - PlayerID 1~10 is reserved, create fake player by calling spCreateSystemPlayer('Bot', 0, 10, 4) in sql statement
 
	Download: https://dev.mysql.com/downloads/mysql/ 
	 
	General Guide: https://corlewsolutions.com/articles/article-21-how-to-install-mysql-server-5-6-on-windows-7-development-machine 
	 
	 
	Mandatory configs for server. These values should be set to the server config

	Linux : my.cnf
	Windows: my.ini

	lower_case_table_name     = 1
	character-set-server      = utf8
	collation-server          = utf8_general_ci


 
3. Install ZooKeeper service
	- You need JDK 1.7 or heigher, and JAVA_HOME environment variable assigned
	- Use ZookeeperServiceInstall.cmd

4. Editing Server Config and upload
	- Open /Server/Config/ServerConfig.xml
	- Search all IP sections and replace them to yours 

	- /Server/Config/ServerConfig.xml을 에디터로 열기
	- 모든 IP관련된 항목을 자신의 IP로 교체
	- Run StormForge/bin/SFServerManager.exe
	- Select your config file and enter your server IP
	- Hit upload xml to zookeeper
	

5. Build Server instances
	- Open BrServersSF.sln
	- Run Debug&Release build


6. Install Server Instance manager service
	- You can use either Debug or release build for both server instances and server instance launcher
	- You need to sync lanucher before install service. 
		- Use ServiceLauncherSyncDebug.cmd for debug build service launcher
		- Use ServiceLauncherSyncRelease.cmd for release build service launcher.
	- Now you can run either ServiceInstallDebug.cmd or ServiceInstallRelease.cmd
		- ServiceInstallDebug.cmd will let the service launcher ues debug build for server instances
		- ServiceInstallRelease.cmd for release build server instances.


6. Upload Server config to ZooKeeper
	- Server instance and config management are handled with StormForge/Bin/SFServerManager.exe
	- Run SFServerManager
	- Select your config xml
	- Select ZooKeeper connection string something like => 127.0.0.1:2081
	- Click Upload XML to ZooKeeper


7. Running
	- Run SFServerManager
	- Click Start Server Instances
	- Click Stop Server Instances when you want to stop them


8. Miscellaneous
	- //BRServer/Server/Config/ Config foler
	- //BRServer/Server/Config/traceConfig.cfg Configuration file for trace channel setting. every 1 min, server will reload this file
	

	
	
