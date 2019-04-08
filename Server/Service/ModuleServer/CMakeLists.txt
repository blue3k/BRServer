

source_group(INCLUDES
    GameServer.h
    SharedModuleServerClass.h
    SharedModuleSvrConst.h
    stdafx.h
    Transaction/SharedModuleServerTrans.h
	SharedModuleServerClass.inl
)


set(BR_LIB_FILES
    SharedModuleServer.cpp
    SharedModuleServerClass.cpp
    SharedModuleServer_ModuleMapping.cpp
    SharedModuleServer_NetProc.cpp
    Transaction/SharedModuleServerTrans.cpp
  )




include_directories(./)



add_executable(BRModuleServer ${BR_LIB_FILES})

add_dependencies(BRModuleServer BRDB BRServerSystem BRServerProtocol BRTable)

target_link_libraries(BRModuleServer ${ENGINE_LINK_LIBS} ${PLATFORM_LIBS})


install (TARGETS BRModuleServer
         ARCHIVE DESTINATION lib COMPONENT BRServer
         LIBRARY DESTINATION lib COMPONENT BRServer
         RUNTIME DESTINATION bin COMPONENT BRServer)


