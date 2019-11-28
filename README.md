# BRServer
Braves Server 

Prerequisits
	- Visual Studio 2017 is required
	- JDK jdk7 or higher: http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html
	- StormForge


Use Setup.cmd to setup StormForge
Cmd folder has command line scripts for running server


Linux build scripts
1. CMakeLinuxGen.sh to generate build project
2. CMakeLinuxBuild.sh to build

Windows build scripts
1. CMakeWinGen.cmd to generate build project
2. CMakeWinBuild.cmd to build
3. BrTools.sln: Server tools build
3. buildWindows\x64\BrServers.sln: Generated server project



1. Generic Build Order
 a. Build StormForge
 b. Sync StormForge libries(Run SyncSF3rdParites.cmd)
 c. Build Tools(BrTools.sln)
 d. Build Main game server(CMakeWinBuild.cmd or CMakeLinuxBuild.sh)
 e. Select build binary
	 - Use ServerUseDebug.cmd to copy debug version to bin for use
	 - Use ServerUseRelease.cmd to copy release version to bin for use


2. DB setup
 - Guide for mysql: https://dev.mysql.com/doc/refman/8.0/en/installing.html
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

	[mysqld]
	lower_case_table_name     = 1
	character-set-server      = utf8
	collation-server          = utf8_general_ci
	default_authentication_plugin=mysql_native_password

 
3. Install ZooKeeper service
	- You need JDK 1.7 or heigher, and JAVA_HOME environment variable assigned
	- Use ZookeeperServiceInstall.cmd

4. Editing Server Config and upload
	- You can upload config xml to zookeeper with SFSetverManager client or other server tool
	- Easy way
		- Open /Server/Config/ServerConfig.xml
		- Search all IP sections and replace them to yours 
		- Upload with manager tool
	

5. Install Server Instance manager service
	- You can use either Debug or release build for both server instances and server instance launcher
	   Use ServerUseDebug.cmd or ServerUseRelease.cmd to switch
	- Use ServiceLauncher.cmd to run console mode
	- Or you can use ServiceInstall.cmd or ServiceUnInstall.cmd to install windows system service


6. Upload Server config to ZooKeeper
	- Server instance and config management are handled with SFServerManager
	- Run SFServerManager
	- Select your config xml
	- Select ZooKeeper connection string something like => 127.0.0.1:2081
	- Click Upload XML to ZooKeeper


7. Running
	- Run Zookeeper
	- Run ServiceLauncher
	- Run SFServerManager
	- Upload config if not uploaded or updated
	- Click Start Server Instances
	- Click Stop Server Instances when you want to stop them


8. Miscellaneous
	- //BRServer/Server/Config/ Config foler
	- //BRServer/Server/Config/traceConfig.cfg Configuration file for trace channel setting. every 1 min, server will reload this file
	

	
	
