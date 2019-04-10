

source_group(INCLUDES FILES 
    ServerInstanceLauncher.h
    ServerInstanceLauncherClass.h
    ServerEntity/ServerInstanceLauncherEntity.h
    SFProcessManager.h
    stdafx.h
    ZooKeeperCommand.h
)


set(BR_LIB_FILES
    ServerInstanceLauncher.cpp
    ServerInstanceLauncherClass.cpp
    SFProcessManager.cpp
    SFProcessManagerLinux.cpp
    SFProcessManagerWin.cpp
    ZooKeeperCommand.cpp
  )




include_directories(./)



add_executable(BRServerLauncher ${BR_LIB_FILES})

add_dependencies(BRServerLauncher BRDB BRServerSystem BRServerProtocol BRTable)

target_link_libraries(BRServerLauncher ${ENGINE_LINK_LIBS} ${PLATFORM_LIBS})


install (TARGETS BRServerLauncher
         ARCHIVE DESTINATION lib COMPONENT BRServer
         LIBRARY DESTINATION lib COMPONENT BRServer
         RUNTIME DESTINATION bin COMPONENT BRServer)


