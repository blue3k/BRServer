



set(BR_LIB_FILES
    GameInstance/GamePlayerEntity.h
    GameServer.h
    GameServerClass.h
    GameSvrConst.h
    GameSystem/UserFriendSystem.h
    GameSystem/UserGamePlayerInfoSystem.h
    GameSystem/UserNotifySystem.h
    GameSystem/UserSystemComponentIDs.h
    Transaction/GamePlayerEntityTrans.h
    Transaction/GamePlayerEntityTransChatChannel.h
    Transaction/GamePlayerEntityTransClose.h
    Transaction/GamePlayerEntityTransFriend.h
    Transaction/GamePlayerEntityTransGame.h
    Transaction/GamePlayerEntityTransParty.h
    Transaction/GameServerTrans.h
	GameServerClass.inl
    GameInstance/GamePlayerEntity.cpp
    GameServer.cpp
    GameServerClass.cpp
    GameSystem/UserFriendSystem.cpp
    GameSystem/UserGamePlayerInfoSystem.cpp
    GameSystem/UserNotifySystem.cpp
    Transaction/GamePlayerEntityTrans.cpp
    Transaction/GamePlayerEntityTransChatChannel.cpp
    Transaction/GamePlayerEntityTransClose.cpp
    Transaction/GamePlayerEntityTransFriend.cpp
    Transaction/GamePlayerEntityTransGame.cpp
    Transaction/GamePlayerEntityTransParty.cpp
    Transaction/GameServerTrans.cpp
  )




include_directories(./)



add_executable(BRGameServerConspriacy ${BR_LIB_FILES})

add_dependencies(BRGameServerConspriacy BRDB BRServerSystem BRServerProtocol BRTable)

target_link_libraries(BRGameServerConspriacy ${ENGINE_LINK_LIBS} ${PLATFORM_LIBS})


install (TARGETS BRGameServerConspriacy
         ARCHIVE DESTINATION lib COMPONENT BRServer
         LIBRARY DESTINATION lib COMPONENT BRServer
         RUNTIME DESTINATION bin COMPONENT BRServer)


