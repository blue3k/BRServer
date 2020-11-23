
cmake_minimum_required(VERSION 3.16)

include (../StormForge/build/StormForge.cmake)


list(PREPEND ENGINE_LINK_LIBS BRDB BRServerSystem BRServerProtocol BRDB)


include_directories(AFTER 
	#../gsdk/cpp/cppsdk
	../BRServer/Server/Lib
	../BRServer/Server/Lib/DB
	../BRServer/Server/Lib/ServerProtocol
	../BRServer/Server/Lib/ServerSystem
	../BRServer/Server/Lib/Table
	)




if(WIN32)

elseif(UNIX)


endif()









