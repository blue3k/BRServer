////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server constants definitions. 
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Common/Typedefs.h"


namespace BR{
namespace Svr{
namespace Const{


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Category : Server system
	//
	extern INT		TRANSACTION_TIMEOUT;
	extern INT		SVRMAINENTITY_RESPROC_MAX;
	extern INT		PLUGIN_ENTITYID_BASE;



	enum {

		ENTITY_GAME_TRANS_QUEUE = 2048,
		ENTITY_GAME_TRANSRES_QUEUE = 2048,

		ENTITY_REMOTE_TRANS_QUEUE = 1024,
		ENTITY_REMOTE_TRANSRES_QUEUE = 1024,

		LOGIN_SESSION_EXPIRE_TIME = 60 * 60 * 2, // 2 min

		MAX_NAME = 64,

		ENTITY_GAMEMANAGER_THREAD = 8,

		SERVER_TRANS_QUEUE = 1024,
		SERVER_TRANSRES_QUEUE = 1024,

		WORKLOAD_UPDATE_TIME = 1500, // 1.5 sec

		MATCHING_QUEUE_RESERVATION_TIMEOUT = 120 * 1000, // 2min

		// EntityServerEntity queue size
		ENTITY_ENTITY_TRANS_QUEUE = 2048,
		ENTITY_ENTITY_TRANSRES_QUEUE = 2048,

		REMOTE_CONNECTION_RETRY = 5 * 1000,
		REMOTE_CONNECTION_RETRY_MAX = 5 * 60 * 1000,


		SIMPLEUSER_TICKTASK_INTERVAL = 60 * 1000,
		PARALLEL_TRANSACTION_MANAGER_TICKTASK_INTERVAL = 1000,

		PARTY_TICKTASK_INTERVAL = 2 * 60 * 1000,

		// Wait time for login sequences
#ifdef DEBUG
		LOGIN_TIME_WAIT_PLAYER_JOIN = 1 * 60 * 1000,
#else
		LOGIN_TIME_WAIT_PLAYER_JOIN = 2 * 60 * 1000,  // This is DB maximum, see spRegisterAuthTicket
#endif

		GAMEINSTANCE_TICK_TIME = 3 * 1000,
		GAMEINSTANCE_EMPTYINSTANCE_KILL_TIMEOUT = 60 * 1000,


		TIME_INTERVAL_RANKING_UPDATE = 10 * 1000,
	};



}; // namespace Const
}; // namespace Svr
}; // namespace BR

