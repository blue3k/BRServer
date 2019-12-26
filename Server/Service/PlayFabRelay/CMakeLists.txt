



set(BR_LIB_FILES
	RelayServerPCH.h
	RelayServerPCH.cpp
	RelayServer.h
    RelayServer.cpp
    RelayServerClass.cpp
	NetRelayServer.cpp
	NetRelayServer.h
  )




include_directories(./)



add_executable(BRRelayServer ${BR_LIB_FILES})

add_dependencies(BRRelayServer BRDB BRServerSystem BRServerProtocol BRTable PlayFabGSDK)

target_link_libraries(BRRelayServer ${ENGINE_LINK_LIBS} ${PLATFORM_LIBS})


install (TARGETS BRRelayServer
         ARCHIVE DESTINATION lib COMPONENT BRServer
         LIBRARY DESTINATION lib COMPONENT BRServer
         RUNTIME DESTINATION bin COMPONENT BRServer)


