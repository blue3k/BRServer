




set(BR_LIB_FILES
    SharedModuleSvrConst.h
    SharedModuleServer.cpp
	SharedModuleServerClass.h
	SharedModuleServerClass.inl
    SharedModuleServerClass.cpp
    Transaction/SharedModuleServerTrans.h
    Transaction/SharedModuleServerTrans.cpp
  )




include_directories(./)



add_executable(BRModuleServer ${BR_LIB_FILES})

add_dependencies(BRModuleServer BRDB BRServerSystem BRServerProtocol)

target_link_libraries(BRModuleServer ${ENGINE_LINK_LIBS} ${PLATFORM_LIBS})


install (TARGETS BRModuleServer
         ARCHIVE DESTINATION lib COMPONENT BRServer
         LIBRARY DESTINATION lib COMPONENT BRServer
         RUNTIME DESTINATION bin COMPONENT BRServer)

if(WIN32 AND NOT ENABLE_CLANG)
	install (FILES $<TARGET_PDB_FILE:BRModuleServer> DESTINATION bin COMPONENT BRServer)
endif()
