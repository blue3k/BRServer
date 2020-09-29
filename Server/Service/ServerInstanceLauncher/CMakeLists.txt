



set(BR_LIB_FILES
    ServerInstanceLauncher.h
    ServerInstanceLauncherClass.h
    SFProcessManager.h
    ZookeeperCommand.h
    ServerInstanceLauncher.cpp
    ServerInstanceLauncherClass.cpp
    SFProcessManager.cpp
    SFProcessManagerLinux.cpp
    SFProcessManagerWin.cpp
    ZookeeperCommand.cpp
  )




include_directories(./)



add_executable(BRServerLauncher ${BR_LIB_FILES})

add_dependencies(BRServerLauncher BRDB BRServerSystem BRServerProtocol)

target_link_libraries(BRServerLauncher ${ENGINE_LINK_LIBS} ${PLATFORM_LIBS})


install (TARGETS BRServerLauncher
         ARCHIVE DESTINATION lib COMPONENT BRServer
         LIBRARY DESTINATION lib COMPONENT BRServer
         RUNTIME DESTINATION bin COMPONENT BRServer)


