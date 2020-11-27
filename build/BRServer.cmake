
cmake_minimum_required(VERSION 3.16)

include (../StormForge/build/StormForge.cmake)


IF(NOT BRSERVER_FOLDER)
   SET(BRSERVER_FOLDER BRServer)
ENDIF()


list(PREPEND ENGINE_LINK_LIBS BRDB BRServerSystem BRServerProtocol BRDB)


include_directories(AFTER 
	#../gsdk/cpp/cppsdk
	../${BRSERVER_FOLDER}/Server/Lib
	../${BRSERVER_FOLDER}/Server/Lib/DB
	../${BRSERVER_FOLDER}/Server/Lib/ServerProtocol
	../${BRSERVER_FOLDER}/Server/Lib/ServerSystem
	../${BRSERVER_FOLDER}/Server/Lib/Table
	)


if(WIN32)

elseif(UNIX)


endif()


