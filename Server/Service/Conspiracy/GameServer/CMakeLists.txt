



set(BR_LIB_FILES
    GameServerPCH.h
    GameInstance/GamePlayerEntity.h
    GameServer.h
    GameServerClass.h
    GameSvrConst.h
    GameSystem/UserFriendSystem.h
    GameSystem/UserGamePlayerInfoSystem.h
    GameSystem/UserNotifySystem.h
    GameSystem/UserSystemComponentIDs.h
    Transaction/GamePlayerEntityTrans.h
    Transaction/GamePlayerEntityTransClose.h
    Transaction/GamePlayerEntityTransGame.h
    Transaction/GameServerTrans.h
	GameServerClass.inl
    GameInstance/GamePlayerEntity.cpp
    GameServer.cpp
    GameServerClass.cpp
    GameSystem/UserFriendSystem.cpp
    GameSystem/UserGamePlayerInfoSystem.cpp
    GameSystem/UserNotifySystem.cpp
    Transaction/GamePlayerEntityTrans.cpp
    Transaction/GamePlayerEntityTransClose.cpp
    Transaction/GamePlayerEntityTransGame.cpp
    Transaction/GameServerTrans.cpp
  )




include_directories(./ ../GameDB ../GameTable)



add_executable(BRConspriacyGameServer ${BR_LIB_FILES})

add_dependencies(BRConspriacyGameServer BRDB BRServerSystem BRServerProtocol BRConspiracyGameTable BRConspiracyGameDB)

target_link_libraries(BRConspriacyGameServer ${ENGINE_LINK_LIBS} ${PLATFORM_LIBS} BRConspiracyGameTable BRConspiracyGameDB)


install (TARGETS BRConspriacyGameServer
         ARCHIVE DESTINATION lib COMPONENT BRServer
         LIBRARY DESTINATION lib COMPONENT BRServer
         RUNTIME DESTINATION bin COMPONENT BRServer)

if(WIN32 AND NOT ENABLE_CLANG)
	install (FILES $<TARGET_PDB_FILE:BRConspriacyGameServer> DESTINATION bin COMPONENT BRServer)
endif()
