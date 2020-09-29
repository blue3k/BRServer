
cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER clang)
#set(CMAKE_CXX_COMPILER clang++)


project(BRServer C CXX)


message ( "Input system = ${CMAKE_SYSTEM_NAME}" )


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)



add_definitions(-DHAVE_CONFIG_H)
add_definitions(-DCURL_STATICLIB)
add_definitions(-DHAVE_STDDEF_H=1)
add_definitions(-DHAVE_SYS_TYPES_H=1)
add_definitions(-DSF_USE_MBEDTLS)


SET (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
SET (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")


set(ENGINE_LINK_LIBS BRDB BRServerSystem BRServerProtocol BRDB SFProtocol SFEngine  SFCore PlayFabGSDK curl iconv png mng jpeg tiff zookeeper jsoncpp mbedtls xml2 zlib)


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
#	../StormForge/3rdParties/src/mysql/mysql-8.0.15/include
	3rdParties/PlayFabGSDK/gsdk/cpp/cppsdk
	Server/Lib
	Server/Lib/DB
	Server/Lib/ServerProtocol
	Server/Lib/ServerSystem
	Server/Lib/Table
	)




if(WIN32)

	message ( "Setup Windows configs" + ${CMAKE_BUILD_TYPE} )

	add_definitions(-DNOMINMAX)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_definitions(-DWIN32_LEAN_AND_MEAN)
	add_definitions(-DWIN64)
	add_definitions(-DWIN32)
	add_definitions(-D_WINDOWS)
	add_definitions(-DSTATIC_CONCPP) # mysql cpp conn static



	SET (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Od")
	SET (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Od")

	SET (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -Oy-")
	SET (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -Oy-")

	SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")

	set(ARTECTURE x64)

	include_directories(AFTER 
		../VulkanSDK/1.1.82.1/include
		../StormForge/3rdParties/src/mysql/buildWindows/${ARTECTURE}/include
	)
	
	set(PLATFORM_LIBS Ws2_32 Mswsock mysqlcppconn8 libcrypto libssl Dnsapi)

	link_directories(
		../StormForge/build${CMAKE_SYSTEM_NAME}/${ARTECTURE}$(Configuration)/lib
		../StormForge/3rdParties/src/mysql/buildWindows/${ARTECTURE}/lib64/vs14
		../StormForge/3rdParties/src/openssl/buildWindows/openssl/lib
		Server/bin/lib/${ARTECTURE}$(Configuration)
	)


elseif(UNIX)

	message ( "Setup UNIX configs" + ${CMAKE_BUILD_TYPE} )

	SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -fexceptions -frtti -pthread -fPIC")
	SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fexceptions -frtti -pthread -fPIC")

	SET (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
	SET (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

	SET (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_DEBUG} -O3")
	SET (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} -O3")


	add_definitions(-D_LINUX_=1)
	add_definitions(-DEPOLL)


	set(ARTECTURE x64)

	include_directories(AFTER 
		#../StormForge/3rdParties/src/mysql/linuxLib/include
		/usr/include/mysql-cppconn-8/
		#/usr/local/mysql/connector-c++-8.0/include/
		#3rdParties/VulkanSDK/1.1.82.1/include
	)
	
	link_directories(
		/usr/lib
		/usr/lib/x86_64-linux-gnu/
		#/usr/local/mysql/connector-c++-8.0/lib64/debug
		#../StormForge/3rdParties/src/mysql/linuxLib/lib64
		../StormForge/build${CMAKE_SYSTEM_NAME}/${ARTECTURE}${CMAKE_BUILD_TYPE}/lib
		)

	set(PLATFORM_LIBS mysqlcppconn8 rt m)

endif()



set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/build${CMAKE_SYSTEM_NAME}/${ARTECTURE}${CMAKE_BUILD_TYPE})

add_subdirectory(3rdParties)
add_subdirectory(Server)
add_subdirectory(TestApp)



