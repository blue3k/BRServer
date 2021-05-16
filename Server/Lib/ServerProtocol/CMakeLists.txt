



set(BRSERVERPROTOCOL_LIB_FILES
    ServerProtocolPCH.h
    GenericMsgClass.h
    Protocol/ChatChannelManagerMsgClass.h
    Protocol/ChatChannelManagerMsgDebug.h
    Protocol/ChatChannelManagerMsgParsing.h
    Protocol/ChatChannelMsgClass.h
    Protocol/ChatChannelMsgDebug.h
    Protocol/ChatChannelMsgParsing.h
    Protocol/ProtocolChatChannel.h
    Protocol/ProtocolChatChannelManager.h
    Protocol/ClusterServerMsgClass.h
    Protocol/ClusterServerMsgDebug.h
    Protocol/ClusterServerMsgParsing.h
    Protocol/GameInstanceManagerMsgClass.h
    Protocol/GameInstanceManagerMsgDebug.h
    Protocol/GameInstanceManagerMsgParsing.h
    Protocol/GameInstanceMsgClass.h
    Protocol/GameInstanceMsgDebug.h
    Protocol/GameInstanceMsgParsing.h
    Protocol/GamePartyManagerMsgClass.h
    Protocol/GamePartyManagerMsgDebug.h
    Protocol/GamePartyManagerMsgParsing.h
    Protocol/GamePartyMsgClass.h
    Protocol/GamePartyMsgDebug.h
    Protocol/GamePartyMsgParsing.h
    Protocol/GameServerMsgClass.h
    Protocol/GameServerMsgDebug.h
    Protocol/GameServerMsgParsing.h
    Protocol/LoginServerMsgClass.h
    Protocol/LoginServerMsgDebug.h
    Protocol/LoginServerMsgParsing.h
    Protocol/MonitoringMsgClass.h
    Protocol/MonitoringMsgDebug.h
    Protocol/MonitoringMsgParsing.h
    Protocol/PartyMatchingMsgClass.h
    Protocol/PartyMatchingMsgDebug.h
    Protocol/PartyMatchingMsgParsing.h
    Protocol/PartyMatchingQueueMsgClass.h
    Protocol/PartyMatchingQueueMsgDebug.h
    Protocol/PartyMatchingQueueMsgParsing.h
    Protocol/ProtocolClusterServer.h
    Protocol/ProtocolGameInstance.h
    Protocol/ProtocolGameInstanceManager.h
    Protocol/ProtocolGameParty.h
    Protocol/ProtocolGamePartyManager.h
    Protocol/ProtocolGameServer.h
    Protocol/ProtocolLoginServer.h
    Protocol/ProtocolMonitoring.h
    Protocol/ProtocolPartyMatching.h
    Protocol/ProtocolPartyMatchingQueue.h
    Protocol/ProtocolRankingServer.h
    Protocol/ProtocolServer.h
    Protocol/RankingServerMsgClass.h
    Protocol/RankingServerMsgDebug.h
    Protocol/RankingServerMsgParsing.h
    Protocol/ServerMsgClass.h
    Protocol/ServerMsgDebug.h
    Protocol/ServerMsgParsing.h
    Protocol/ChatChannelManagerNetPolicy.h
    Protocol/ChatChannelNetPolicy.h
    Protocol/ClusterServerNetPolicy.h
    Protocol/GameInstanceManagerNetPolicy.h
    Protocol/GameInstanceNetPolicy.h
    Protocol/GamePartyManagerNetPolicy.h
    Protocol/GamePartyNetPolicy.h
    Protocol/GameServerNetPolicy.h
    Protocol/LoginServerNetPolicy.h
    Protocol/MonitoringNetPolicy.h
    Protocol/PartyMatchingNetPolicy.h
    Protocol/PartyMatchingQueueNetPolicy.h
    Protocol/RankingServerNetPolicy.h
    Protocol/ServerNetPolicy.h
    Protocol/ChatChannelManagerService.h
    Protocol/ChatChannelService.h
    Protocol/ClusterServerService.h
    Protocol/GameInstanceManagerService.h
    Protocol/GameInstanceService.h
    Protocol/GamePartyManagerService.h
    Protocol/GamePartyService.h
    Protocol/GameServerService.h
    Protocol/LoginServerService.h
    Protocol/MonitoringService.h
    Protocol/PartyMatchingQueueService.h
    Protocol/PartyMatchingService.h
    Protocol/RankingServerService.h
    Protocol/ServerService.h
    Protocol/SvrProtocol.h
    GenericMsgClass.cpp
    Protocol/ChatChannelManagerMsgClass.cpp
    Protocol/ChatChannelManagerMsgDebug.cpp
    Protocol/ChatChannelManagerMsgParsing.cpp
    Protocol/ChatChannelMsgClass.cpp
    Protocol/ChatChannelMsgDebug.cpp
    Protocol/ChatChannelMsgParsing.cpp
    Protocol/ClusterServerMsgClass.cpp
    Protocol/ClusterServerMsgDebug.cpp
    Protocol/ClusterServerMsgParsing.cpp
    Protocol/GameInstanceManagerMsgClass.cpp
    Protocol/GameInstanceManagerMsgDebug.cpp
    Protocol/GameInstanceManagerMsgParsing.cpp
    Protocol/GameInstanceMsgClass.cpp
    Protocol/GameInstanceMsgDebug.cpp
    Protocol/GameInstanceMsgParsing.cpp
    Protocol/GamePartyManagerMsgClass.cpp
    Protocol/GamePartyManagerMsgDebug.cpp
    Protocol/GamePartyManagerMsgParsing.cpp
    Protocol/GamePartyMsgClass.cpp
    Protocol/GamePartyMsgDebug.cpp
    Protocol/GamePartyMsgParsing.cpp
    Protocol/GameServerMsgClass.cpp
    Protocol/GameServerMsgDebug.cpp
    Protocol/GameServerMsgParsing.cpp
    Protocol/LoginServerMsgClass.cpp
    Protocol/LoginServerMsgDebug.cpp
    Protocol/LoginServerMsgParsing.cpp
    Protocol/MonitoringMsgClass.cpp
    Protocol/MonitoringMsgDebug.cpp
    Protocol/MonitoringMsgParsing.cpp
    Protocol/PartyMatchingMsgClass.cpp
    Protocol/PartyMatchingMsgDebug.cpp
    Protocol/PartyMatchingMsgParsing.cpp
    Protocol/PartyMatchingQueueMsgClass.cpp
    Protocol/PartyMatchingQueueMsgDebug.cpp
    Protocol/PartyMatchingQueueMsgParsing.cpp
    Protocol/RankingServerMsgClass.cpp
    Protocol/RankingServerMsgDebug.cpp
    Protocol/RankingServerMsgParsing.cpp
    Protocol/ServerMsgClass.cpp
    Protocol/ServerMsgDebug.cpp
    Protocol/ServerMsgParsing.cpp
    Protocol/ChatChannelManagerNetPolicy.cpp
    Protocol/ChatChannelNetPolicy.cpp
    Protocol/ClusterServerNetPolicy.cpp
    Protocol/GameInstanceManagerNetPolicy.cpp
    Protocol/GameInstanceNetPolicy.cpp
    Protocol/GamePartyManagerNetPolicy.cpp
    Protocol/GamePartyNetPolicy.cpp
    Protocol/GameServerNetPolicy.cpp
    Protocol/LoginServerNetPolicy.cpp
    Protocol/MonitoringNetPolicy.cpp
    Protocol/PartyMatchingNetPolicy.cpp
    Protocol/PartyMatchingQueueNetPolicy.cpp
    Protocol/RankingServerNetPolicy.cpp
    Protocol/ServerNetPolicy.cpp
	Protocol/RelayServerMsgClass.cpp
	Protocol/RelayServerMsgClass.h
	Protocol/RelayServerMsgDebug.cpp
	Protocol/RelayServerMsgDebug.h
	Protocol/RelayServerMsgParsing.cpp
	Protocol/RelayServerMsgParsing.h
	Protocol/RelayServerNetPolicy.cpp
	Protocol/RelayServerNetPolicy.h
    Protocol/ChatChannelManagerService.cpp
    Protocol/ChatChannelService.cpp
    Protocol/ClusterServerService.cpp
    Protocol/GameInstanceManagerService.cpp
    Protocol/GameInstanceService.cpp
    Protocol/GamePartyManagerService.cpp
    Protocol/GamePartyService.cpp
    Protocol/GameServerService.cpp
    Protocol/LoginServerService.cpp
    Protocol/MonitoringService.cpp
    Protocol/PartyMatchingQueueService.cpp
    Protocol/PartyMatchingService.cpp
    Protocol/RankingServerService.cpp
    Protocol/ServerService.cpp
  )




include_directories(./)



add_library(BRServerProtocol STATIC ${BRSERVERPROTOCOL_LIB_FILES})


install (TARGETS BRServerProtocol
         ARCHIVE DESTINATION lib COMPONENT BRServer
         LIBRARY DESTINATION lib COMPONENT BRServer
         RUNTIME DESTINATION bin COMPONENT BRServer)


