////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Component class definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"
#include "Memory/SFMemory.h"
#include "Component/BrComponent.h"
#include "Component/ServerComponent.h"


namespace SF {
namespace Svr {

	class Entity;
	class ServerEntity;
	class ServerServiceBase;


	enum ServerComponentID
	{
		ServerComponentID_AccountDB,
		ServerComponentID_LoginSessionDB,
		ServerComponentID_GameDB,
		ServerComponentID_GameTransactionDB,
		ServerComponentID_RankingDB,
		ServerComponentID_ServerEntityManager,		               // Server
		ServerComponentID_EntityManager,
		ServerComponentID_ParallelTransactionManager,

		ServerComponentID_ClusterManagerService,
		ServerComponentID_GameClusterService,
		ServerComponentID_GameInstanceManagerService,
		ServerComponentID_GamePartyManagerService,
		ServerComponentID_PartyManagerService,

		ServerComponentID_MatchingWatcherService_4,
		ServerComponentID_MatchingWatcherService_8,


		ServerComponentID_MatchingQueueWatcherService_4x1,
		ServerComponentID_MatchingQueueWatcherService_4x2,
		ServerComponentID_MatchingQueueWatcherService_4x3,
		ServerComponentID_MatchingQueueWatcherService_4x1S,
		ServerComponentID_MatchingQueueWatcherService_4x1W,


		ServerComponentID_MatchingQueueWatcherService_8x1,
		ServerComponentID_MatchingQueueWatcherService_8x2,
		ServerComponentID_MatchingQueueWatcherService_8x3,
		ServerComponentID_MatchingQueueWatcherService_8x4,
		ServerComponentID_MatchingQueueWatcherService_8x5,
		ServerComponentID_MatchingQueueWatcherService_8x6,
		ServerComponentID_MatchingQueueWatcherService_8x7,
		ServerComponentID_MatchingQueueWatcherService_8x1S,
		ServerComponentID_MatchingQueueWatcherService_8x1W,

		ServerComponentID_ChatChannelManagerService,

		ServerComponentID_Max
	};

	


}; // namespace Svr
}; // namespace SF




