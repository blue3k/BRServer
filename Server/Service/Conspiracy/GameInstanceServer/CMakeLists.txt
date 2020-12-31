



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




include_directories(./ ../GameDB ../GameTable)



add_executable(BRConspiracyGameInstance ${BRGAMEINSTANCESERVER_LIB_FILES})

add_dependencies(BRConspiracyGameInstance BRDB BRServerSystem BRServerProtocol BRConspiracyGameTable BRConspiracyGameDB)

target_link_libraries(BRConspiracyGameInstance ${ENGINE_LINK_LIBS} ${PLATFORM_LIBS} BRConspiracyGameTable BRConspiracyGameDB)


install (TARGETS BRConspiracyGameInstance
         ARCHIVE DESTINATION lib COMPONENT BRServer
         LIBRARY DESTINATION lib COMPONENT BRServer
         RUNTIME DESTINATION bin COMPONENT BRServer)

if(WIN32 AND NOT ENABLE_CLANG)
	install (FILES $<TARGET_PDB_FILE:BRConspiracyGameInstance> DESTINATION bin COMPONENT BRServer)
endif()
