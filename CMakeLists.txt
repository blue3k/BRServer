
cmake_minimum_required(VERSION 3.16)

include (../StormForge/build/StormForge.cmake)



project(BRServer C CXX)


set(ENGINE_LINK_LIBS BRDB BRServerSystem BRServerProtocol BRDB ${ENGINE_LINK_LIBS})


include_directories(AFTER 
	../StormForge/3rdParties/src/mbedtls/mbedtls-2.16.0/include
	../StormForge/3rdParties/src/libiconv/libiconv-1.15/include
	../StormForge/3rdParties/src/libpng/lpng1617
	../StormForge/3rdParties/src/jpeg/jpeg-9a
	../StormForge/3rdParties/src/libxml/libxml2/include
	../StormForge/3rdParties/src/curl/curl/include
	../StormForge/3rdParties/src/zlib/zlib-1.2.8
	../StormForge/3rdParties/src/jsoncpp/jsoncpp/include
	../StormForge/3rdParties/src/zookeeper/zookeeperConnector/include
	../StormForge/Engine/Src/SFCore
	../StormForge/Engine/Src/SFEngine
	../StormForge/Engine/Src/Protocol
	../gsdk/cpp/cppsdk
	Server/Lib
	Server/Lib/DB
	Server/Lib/ServerProtocol
	Server/Lib/ServerSystem
	Server/Lib/Table
	)




if(WIN32)

	add_definitions(-DSTATIC_CONCPP) # mysql cpp conn static

	include_directories(AFTER 
		../StormForge/3rdParties/src/mysql/buildWindows/${ARTECTURE}/include
	)
	
	set(PLATFORM_LIBS ${PLATFORM_LIBS} mysqlcppconn8-static libcrypto libssl Dnsapi)

	link_directories(
		../StormForge/3rdParties/src/mysql/buildWindows/${ARTECTURE}/lib64/vs14/$(Configuration)
		../StormForge/3rdParties/src/mysql/buildWindows/${ARTECTURE}/lib64/vs14
		../StormForge/3rdParties/src/openssl/buildWindows/openssl/lib
	)

elseif(UNIX)


endif()



include_directories(AFTER 
	../VulkanSDK/include
	../StormForge/3rdParties/${CMAKE_SYSTEM_NAME}/${ARTECTURE}/include
)

link_directories(BEFORE ../StormForge/3rdParties/${CMAKE_SYSTEM_NAME}/${ARTECTURE}/lib)


set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/build${CMAKE_SYSTEM_NAME}/${ARTECTURE}${CMAKE_BUILD_TYPE})


add_subdirectory(../StormForge/Engine/Src/SFCore SFCore)
add_subdirectory(../StormForge/Engine/Src/SFEngine SFEngine)
add_subdirectory(../StormForge/Engine/Src/Protocol SFProtocol)
add_subdirectory(../StormForge/Engine/Src/SFEngineDLL SFEngineDLL)
add_subdirectory(../StormForge/Test SFTest)

add_subdirectory(3rdParties)
add_subdirectory(Server)
add_subdirectory(TestApp)



