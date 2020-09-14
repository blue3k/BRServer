



set(BRSERVERPROTOCOL_LIB_FILES
    GenericMsgClass.h
    ProtocolTrace.h
    Protocol/Message/ChatChannelManagerMsgClass.h
    Protocol/Message/ChatChannelManagerMsgDebug.h
    Protocol/Message/ChatChannelManagerMsgParsing.h
    Protocol/Message/ChatChannelMsgClass.h
    Protocol/Message/ChatChannelMsgDebug.h
    Protocol/Message/ChatChannelMsgParsing.h
    Protocol/Message/ProtocolChatChannel.h
    Protocol/Message/ProtocolChatChannelManager.h
    Protocol/Message/ClusterServerMsgClass.h
    Protocol/Message/ClusterServerMsgDebug.h
    Protocol/Message/ClusterServerMsgParsing.h
    Protocol/Message/CharacterDataServerMsgClass.cpp
    Protocol/Message/CharacterDataServerMsgClass.h
    Protocol/Message/CharacterDataServerMsgDebug.cpp
    Protocol/Message/CharacterDataServerMsgDebug.h
    Protocol/Message/CharacterDataServerMsgParsing.h
    Protocol/Message/CharacterDataServerMsgParsing.cpp
    Protocol/Message/ProtocolCharacterDataServer.h
    Protocol/Policy/CharacterDataServerNetPolicy.h
    Protocol/Policy/CharacterDataServerNetPolicy.cpp
    Protocol/ServerService/CharacterDataServerService.h
    Protocol/ServerService/CharacterDataServerService.cpp
    Protocol/Message/GameInstanceManagerMsgClass.h
    Protocol/Message/GameInstanceManagerMsgDebug.h
    Protocol/Message/GameInstanceManagerMsgParsing.h
    Protocol/Message/GameInstanceMsgClass.h
    Protocol/Message/GameInstanceMsgDebug.h
    Protocol/Message/GameInstanceMsgParsing.h
    Protocol/Message/GamePartyManagerMsgClass.h
    Protocol/Message/GamePartyManagerMsgDebug.h
    Protocol/Message/GamePartyManagerMsgParsing.h
    Protocol/Message/GamePartyMsgClass.h
    Protocol/Message/GamePartyMsgDebug.h
    Protocol/Message/GamePartyMsgParsing.h
    Protocol/Message/GameServerMsgClass.h
    Protocol/Message/GameServerMsgDebug.h
    Protocol/Message/GameServerMsgParsing.h
    Protocol/Message/LoginServerMsgClass.h
    Protocol/Message/LoginServerMsgDebug.h
    Protocol/Message/LoginServerMsgParsing.h
    Protocol/Message/MonitoringMsgClass.h
    Protocol/Message/MonitoringMsgDebug.h
    Protocol/Message/MonitoringMsgParsing.h
    Protocol/Message/PartyMatchingMsgClass.h
    Protocol/Message/PartyMatchingMsgDebug.h
    Protocol/Message/PartyMatchingMsgParsing.h
    Protocol/Message/PartyMatchingQueueMsgClass.h
    Protocol/Message/PartyMatchingQueueMsgDebug.h
    Protocol/Message/PartyMatchingQueueMsgParsing.h
    Protocol/Message/ProtocolClusterServer.h
    Protocol/Message/ProtocolGameInstance.h
    Protocol/Message/ProtocolGameInstanceManager.h
    Protocol/Message/ProtocolGameParty.h
    Protocol/Message/ProtocolGamePartyManager.h
    Protocol/Message/ProtocolGameServer.h
    Protocol/Message/ProtocolLoginServer.h
    Protocol/Message/ProtocolMonitoring.h
    Protocol/Message/ProtocolPartyMatching.h
    Protocol/Message/ProtocolPartyMatchingQueue.h
    Protocol/Message/ProtocolRankingServer.h
    Protocol/Message/ProtocolServer.h
    Protocol/Message/RankingServerMsgClass.h
    Protocol/Message/RankingServerMsgDebug.h
    Protocol/Message/RankingServerMsgParsing.h
    Protocol/Message/ServerMsgClass.h
    Protocol/Message/ServerMsgDebug.h
    Protocol/Message/ServerMsgParsing.h
    Protocol/Policy/ChatChannelManagerNetPolicy.h
    Protocol/Policy/ChatChannelNetPolicy.h
    Protocol/Policy/ClusterServerNetPolicy.h
    Protocol/Policy/GameInstanceManagerNetPolicy.h
    Protocol/Policy/GameInstanceNetPolicy.h
    Protocol/Policy/GamePartyManagerNetPolicy.h
    Protocol/Policy/GamePartyNetPolicy.h
    Protocol/Policy/GameServerNetPolicy.h
    Protocol/Policy/LoginServerNetPolicy.h
    Protocol/Policy/MonitoringNetPolicy.h
    Protocol/Policy/PartyMatchingNetPolicy.h
    Protocol/Policy/PartyMatchingQueueNetPolicy.h
    Protocol/Policy/RankingServerNetPolicy.h
    Protocol/Policy/ServerNetPolicy.h
    Protocol/ServerService/ChatChannelManagerService.h
    Protocol/ServerService/ChatChannelService.h
    Protocol/ServerService/ClusterServerService.h
    Protocol/ServerService/GameInstanceManagerService.h
    Protocol/ServerService/GameInstanceService.h
    Protocol/ServerService/GamePartyManagerService.h
    Protocol/ServerService/GamePartyService.h
    Protocol/ServerService/GameServerService.h
    Protocol/ServerService/LoginServerService.h
    Protocol/ServerService/MonitoringService.h
    Protocol/ServerService/PartyMatchingQueueService.h
    Protocol/ServerService/PartyMatchingService.h
    Protocol/ServerService/RankingServerService.h
    Protocol/ServerService/ServerService.h
    Protocol/SvrProtocol.h
    GenericMsgClass.cpp
    Protocol/Message/ChatChannelManagerMsgClass.cpp
    Protocol/Message/ChatChannelManagerMsgDebug.cpp
    Protocol/Message/ChatChannelManagerMsgParsing.cpp
    Protocol/Message/ChatChannelMsgClass.cpp
    Protocol/Message/ChatChannelMsgDebug.cpp
    Protocol/Message/ChatChannelMsgParsing.cpp
    Protocol/Message/ClusterServerMsgClass.cpp
    Protocol/Message/ClusterServerMsgDebug.cpp
    Protocol/Message/ClusterServerMsgParsing.cpp
    Protocol/Message/GameInstanceManagerMsgClass.cpp
    Protocol/Message/GameInstanceManagerMsgDebug.cpp
    Protocol/Message/GameInstanceManagerMsgParsing.cpp
    Protocol/Message/GameInstanceMsgClass.cpp
    Protocol/Message/GameInstanceMsgDebug.cpp
    Protocol/Message/GameInstanceMsgParsing.cpp
    Protocol/Message/GamePartyManagerMsgClass.cpp
    Protocol/Message/GamePartyManagerMsgDebug.cpp
    Protocol/Message/GamePartyManagerMsgParsing.cpp
    Protocol/Message/GamePartyMsgClass.cpp
    Protocol/Message/GamePartyMsgDebug.cpp
    Protocol/Message/GamePartyMsgParsing.cpp
    Protocol/Message/GameServerMsgClass.cpp
    Protocol/Message/GameServerMsgDebug.cpp
    Protocol/Message/GameServerMsgParsing.cpp
    Protocol/Message/LoginServerMsgClass.cpp
    Protocol/Message/LoginServerMsgDebug.cpp
    Protocol/Message/LoginServerMsgParsing.cpp
    Protocol/Message/MonitoringMsgClass.cpp
    Protocol/Message/MonitoringMsgDebug.cpp
    Protocol/Message/MonitoringMsgParsing.cpp
    Protocol/Message/PartyMatchingMsgClass.cpp
    Protocol/Message/PartyMatchingMsgDebug.cpp
    Protocol/Message/PartyMatchingMsgParsing.cpp
    Protocol/Message/PartyMatchingQueueMsgClass.cpp
    Protocol/Message/PartyMatchingQueueMsgDebug.cpp
    Protocol/Message/PartyMatchingQueueMsgParsing.cpp
    Protocol/Message/RankingServerMsgClass.cpp
    Protocol/Message/RankingServerMsgDebug.cpp
    Protocol/Message/RankingServerMsgParsing.cpp
    Protocol/Message/ServerMsgClass.cpp
    Protocol/Message/ServerMsgDebug.cpp
    Protocol/Message/ServerMsgParsing.cpp
    Protocol/Policy/ChatChannelManagerNetPolicy.cpp
    Protocol/Policy/ChatChannelNetPolicy.cpp
    Protocol/Policy/ClusterServerNetPolicy.cpp
    Protocol/Policy/GameInstanceManagerNetPolicy.cpp
    Protocol/Policy/GameInstanceNetPolicy.cpp
    Protocol/Policy/GamePartyManagerNetPolicy.cpp
    Protocol/Policy/GamePartyNetPolicy.cpp
    Protocol/Policy/GameServerNetPolicy.cpp
    Protocol/Policy/LoginServerNetPolicy.cpp
    Protocol/Policy/MonitoringNetPolicy.cpp
    Protocol/Policy/PartyMatchingNetPolicy.cpp
    Protocol/Policy/PartyMatchingQueueNetPolicy.cpp
    Protocol/Policy/RankingServerNetPolicy.cpp
    Protocol/Policy/ServerNetPolicy.cpp
	Protocol/Message/RelayServerMsgClass.cpp
	Protocol/Message/RelayServerMsgClass.h
	Protocol/Message/RelayServerMsgDebug.cpp
	Protocol/Message/RelayServerMsgDebug.h
	Protocol/Message/RelayServerMsgParsing.cpp
	Protocol/Message/RelayServerMsgParsing.h
	Protocol/Policy/RelayServerNetPolicy.cpp
	Protocol/Policy/RelayServerNetPolicy.h
    Protocol/ServerService/ChatChannelManagerService.cpp
    Protocol/ServerService/ChatChannelService.cpp
    Protocol/ServerService/ClusterServerService.cpp
    Protocol/ServerService/GameInstanceManagerService.cpp
    Protocol/ServerService/GameInstanceService.cpp
    Protocol/ServerService/GamePartyManagerService.cpp
    Protocol/ServerService/GamePartyService.cpp
    Protocol/ServerService/GameServerService.cpp
    Protocol/ServerService/LoginServerService.cpp
    Protocol/ServerService/MonitoringService.cpp
    Protocol/ServerService/PartyMatchingQueueService.cpp
    Protocol/ServerService/PartyMatchingService.cpp
    Protocol/ServerService/RankingServerService.cpp
    Protocol/ServerService/ServerService.cpp
  )




include_directories(./)



add_library(BRServerProtocol STATIC ${BRSERVERPROTOCOL_LIB_FILES})


install (TARGETS BRServerProtocol
         ARCHIVE DESTINATION lib COMPONENT BRServer
         LIBRARY DESTINATION lib COMPONENT BRServer
         RUNTIME DESTINATION bin COMPONENT BRServer)


