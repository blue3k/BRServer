
cmake_minimum_required(VERSION 3.16)

include (../BRServer/build/BRServer.cmake)


project(BRServer C CXX)


if(WIN32)

elseif(UNIX)


endif()



set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/build${CMAKE_SYSTEM_NAME}/${ARTECTURE}${CMAKE_BUILD_TYPE})


add_subdirectory(../${SF_FOLDER}/Engine/Src/SFCore SFCore)
add_subdirectory(../${SF_FOLDER}/Engine/Src/SFEngine SFEngine)
add_subdirectory(../${SF_FOLDER}/Engine/Src/Protocol SFProtocol)
add_subdirectory(../${SF_FOLDER}/Engine/Src/SFEngineDLL SFEngineDLL)
add_subdirectory(../${SF_FOLDER}/Test SFTest)

add_subdirectory(3rdParties)
add_subdirectory(Server)
add_subdirectory(TestApp)

#I am rewritting many parts of the system, I will revisit the example later
#add_subdirectory(example)



