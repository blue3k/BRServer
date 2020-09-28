



set(BRGAMEINSTANCESERVER_LIB_FILES
    ConspiracyGameInstanceServerClass.h
    ConspiracyGameInstanceSvrConst.h
    GameInstance/GameInstanceEntity.h
    GameInstance/GameInstanceManager.h
    GameInstance/GamePlayer.h
    GameServer.h
    GameSystem/ChattingLogSystem.h
    GameSystem/GameLogSystem.h
    GameSystem/GamePlaySystem.h
    GameSystem/GameStateSystem.h
    GameSystem/GameSystemComponentIDs.h
    GameSystem/GameVote.h
    Transaction/GameInstanceServerTrans.h
    Transaction/GameInstanceTransaction.h
    Transaction/GameInstanceTransPlayer.h
    ConspiracyGameInstanceServerClass.inl
    GameInstance/GameInstanceEntity.inl
    GameInstance/GamePlayer.inl
    GameSystem/GamePlaySystem.inl
    ConspiracyGameInstanceServer.cpp
    ConspiracyGameInstanceServerClass.cpp
    GameInstance/GameInstanceEntity.cpp
    GameInstance/GameInstanceEntity_MsgProc.cpp
    GameInstance/GameInstanceManager.cpp
    GameInstance/GamePlayer.cpp
    GameSystem/ChattingLogSystem.cpp
    GameSystem/GameLogSystem.cpp
    GameSystem/GamePlaySystem.cpp
    GameSystem/GameStateSystem.cpp
    GameSystem/GameVote.cpp
    Transaction/GameInstanceServerTrans.cpp
    Transaction/GameInstanceTransPlayer.cpp
  )




include_directories(./)



add_executable(BRGameInstanceServerConspiracy ${BRGAMEINSTANCESERVER_LIB_FILES})

add_dependencies(BRGameInstanceServerConspiracy BRDB BRServerSystem BRServerProtocol BRTable)

target_link_libraries(BRGameInstanceServerConspiracy ${ENGINE_LINK_LIBS} ${PLATFORM_LIBS})


install (TARGETS BRGameInstanceServerConspiracy
         ARCHIVE DESTINATION lib COMPONENT BRServer
         LIBRARY DESTINATION lib COMPONENT BRServer
         RUNTIME DESTINATION bin COMPONENT BRServer)


