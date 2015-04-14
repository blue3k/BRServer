1.Build External lib
  - Read External/Readme.txt
2.Build Tools
  - OpenBrTools.sln
  - Select Release
  - Build
3.Build server
  - Open&Build BrServer.sln
4.Make config for your system
  - Server\Config\ServerConfig.xml
  - See ServerConfig_Sample.xml
5.Make Log folder, .\Server\bin\log
6.Copy tables. Server\Conspiracy\table
7.Use commands to run/stop, etc
  - RunDebugMode.cmd : run servers with debug build and console
  - StopServers.cmd : Stop servers
  
BrServer.sln     => Main server build project
BrServerTest.sln => Unit test project

