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
#include "String/SFFixedString32.h"
#include "SFEngine.h"
#include "Service/ServerService.h"
#include "Component/SFServerNetComponent.h"
#include "Component/SFZooKeeperSessionComponent.h"
#include "SvrTrace.h"


#include "Protocol/Message/ClusterServerMsgDebug.h"
#include "Protocol/Message/ClusterServerMsgParsing.h"


#include "Protocol/Message/GameInstanceManagerMsgDebug.h"
#include "Protocol/Message/GameInstanceManagerMsgParsing.h"

#include "Protocol/Message/GameInstanceMsgDebug.h"
#include "Protocol/Message/GameInstanceMsgParsing.h"

#include "Protocol/Message/GamePartyManagerMsgDebug.h"
#include "Protocol/Message/GamePartyManagerMsgParsing.h"

#include "Protocol/Message/GamePartyMsgDebug.h"
#include "Protocol/Message/GamePartyMsgParsing.h"

#include "Protocol/Message/GameServerMsgDebug.h"
#include "Protocol/Message/GameServerMsgParsing.h"

#include "Protocol/Message/LoginServerMsgDebug.h"
#include "Protocol/Message/LoginServerMsgParsing.h"

#include "Protocol/Message/MonitoringMsgDebug.h"
#include "Protocol/Message/MonitoringMsgParsing.h"

#include "Protocol/Message/PartyMatchingMsgDebug.h"
#include "Protocol/Message/PartyMatchingMsgParsing.h"

#include "Protocol/Message/PartyMatchingQueueMsgDebug.h"
#include "Protocol/Message/PartyMatchingQueueMsgParsing.h"

#include "Protocol/Message/RankingServerMsgDebug.h"
#include "Protocol/Message/RankingServerMsgParsing.h"

#include "Protocol/Message/ServerMsgDebug.h"
#include "Protocol/Message/ServerMsgParsing.h"



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



		return result;
	}

	// Terminate component
	void ServerNetComponent::DeinitializeComponent()
	{

		LibraryComponent::DeinitializeComponent();
	}

}


