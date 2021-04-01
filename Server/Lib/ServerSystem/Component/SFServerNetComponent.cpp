////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StormForge
// 
// Author : KyungKun Ko
//
// Description : Server Config
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"
#include "Util/SFStringCrc32.h"
#include "SFEngine.h"
#include "Service/ServerService.h"
#include "Component/SFServerNetComponent.h"
#include "Component/SFZookeeperSessionComponent.h"
#include "SvrTrace.h"


#include "Protocol/ClusterServerMsgDebug.h"
#include "Protocol/ClusterServerMsgParsing.h"


#include "Protocol/GameInstanceManagerMsgDebug.h"
#include "Protocol/GameInstanceManagerMsgParsing.h"

#include "Protocol/GameInstanceMsgDebug.h"
#include "Protocol/GameInstanceMsgParsing.h"

#include "Protocol/GamePartyManagerMsgDebug.h"
#include "Protocol/GamePartyManagerMsgParsing.h"

#include "Protocol/GamePartyMsgDebug.h"
#include "Protocol/GamePartyMsgParsing.h"

#include "Protocol/GameServerMsgDebug.h"
#include "Protocol/GameServerMsgParsing.h"

#include "Protocol/LoginServerMsgDebug.h"
#include "Protocol/LoginServerMsgParsing.h"

#include "Protocol/MonitoringMsgDebug.h"
#include "Protocol/MonitoringMsgParsing.h"

#include "Protocol/PartyMatchingMsgDebug.h"
#include "Protocol/PartyMatchingMsgParsing.h"

#include "Protocol/PartyMatchingQueueMsgDebug.h"
#include "Protocol/PartyMatchingQueueMsgParsing.h"

#include "Protocol/RankingServerMsgDebug.h"
#include "Protocol/RankingServerMsgParsing.h"

#include "Protocol/ServerMsgDebug.h"
#include "Protocol/ServerMsgParsing.h"

#include "Protocol/RelayServerMsgDebug.h"
#include "Protocol/RelayServerMsgParsing.h"


namespace SF
{

	ServerNetComponent::ServerNetComponent()
		: LibraryComponent(TypeName)
		, m_Heap(GetSystemHeap())
	{
	}

	ServerNetComponent::~ServerNetComponent()
	{
	}


	// Initialize component
	Result ServerNetComponent::InitializeComponent()
	{
		auto result = LibraryComponent::InitializeComponent();
		if (!result)
			return result;


		Protocol::RegisterDebugTraceClusterServer();
		Protocol::RegisterParserClusterServer();

		Protocol::RegisterDebugTraceGameInstanceManager();
		Protocol::RegisterParserGameInstanceManager();

		Protocol::RegisterDebugTraceGameInstance();
		Protocol::RegisterParserGameInstance();

		Protocol::RegisterDebugTraceGameParty();
		Protocol::RegisterParserGameParty();

		Protocol::RegisterDebugTraceGamePartyManager();
		Protocol::RegisterParserGamePartyManager();

		Protocol::RegisterDebugTraceGameServer();
		Protocol::RegisterParserGameServer();

		Protocol::RegisterDebugTraceLoginServer();
		Protocol::RegisterParserLoginServer();

		Protocol::RegisterDebugTraceGameInstance();
		Protocol::RegisterParserGameInstance();

		Protocol::RegisterDebugTraceGameInstanceManager();
		Protocol::RegisterParserGameInstanceManager();

		Protocol::RegisterDebugTraceLoginServer();
		Protocol::RegisterParserLoginServer();

		Protocol::RegisterDebugTraceMonitoring();
		Protocol::RegisterParserMonitoring();

		Protocol::RegisterDebugTracePartyMatching();
		Protocol::RegisterParserPartyMatching();

		Protocol::RegisterDebugTracePartyMatchingQueue();
		Protocol::RegisterParserPartyMatchingQueue();

		Protocol::RegisterDebugTraceRankingServer();
		Protocol::RegisterParserRankingServer();

		Protocol::RegisterDebugTraceServer();
		Protocol::RegisterParserServer();

		Protocol::RegisterDebugTraceRelayServer();
		Protocol::RegisterParserRelayServer();


		return result;
	}

	// Terminate component
	void ServerNetComponent::DeinitializeComponent()
	{

		LibraryComponent::DeinitializeComponent();
	}

}


