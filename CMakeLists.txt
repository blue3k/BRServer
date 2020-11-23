
cmake_minimum_required(VERSION 3.16)

include (../BRServer/build/BRServer.cmake)


project(BRServer C CXX)



# include_directories(AFTER 
	# ../gsdk/cpp/cppsdk
	# )




if(WIN32)

	# add_definitions(-DSTATIC_CONCPP) # mysql cpp conn static

	# include_directories(AFTER 
		# ../StormForge/3rdParties/src/mysql/buildWindows/${ARTECTURE}/include
	# )
	
	# set(PLATFORM_LIBS ${PLATFORM_LIBS} mysqlcppconn8-static libcrypto libssl Dnsapi)

	# link_directories(
		# ../StormForge/3rdParties/src/mysql/buildWindows/${ARTECTURE}/lib64/vs14/$(Configuration)
		# ../StormForge/3rdParties/src/mysql/buildWindows/${ARTECTURE}/lib64/vs14
		# ../StormForge/3rdParties/src/openssl/buildWindows/openssl/lib
	# )

elseif(UNIX)


endif()



# include_directories(AFTER 
	# ../VulkanSDK/include
	# ../StormForge/3rdParties/${CMAKE_SYSTEM_NAME}/${ARTECTURE}/include
# )

# link_directories(BEFORE ../StormForge/3rdParties/${CMAKE_SYSTEM_NAME}/${ARTECTURE}/lib)
# include_directories(AFTER 
	# ../StormForge/3rdParties/${CMAKE_SYSTEM_NAME}/${ARTECTURE}/${CMAKE_BUILD_TYPE}/lib
# )
	
# link_directories(BEFORE 
	# ../StormForge/3rdParties/${CMAKE_SYSTEM_NAME}/${ARTECTURE}/${CMAKE_BUILD_TYPE}/lib
# )


set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/build${CMAKE_SYSTEM_NAME}/${ARTECTURE}${CMAKE_BUILD_TYPE})


add_subdirectory(../StormForge/Engine/Src/SFCore SFCore)
add_subdirectory(../StormForge/Engine/Src/SFEngine SFEngine)
add_subdirectory(../StormForge/Engine/Src/Protocol SFProtocol)
add_subdirectory(../StormForge/Engine/Src/SFEngineDLL SFEngineDLL)
add_subdirectory(../StormForge/Test SFTest)

add_subdirectory(3rdParties)
add_subdirectory(Server)
add_subdirectory(TestApp)



