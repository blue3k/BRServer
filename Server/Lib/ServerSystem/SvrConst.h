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


#include "SFTypedefs.h"


namespace SF {
namespace Svr {
namespace Const {


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Category : Server system
	//
#if defined(DEBUG)
	constexpr DurationMS		TRANSACTION_TIMEOUT = DurationMS(1000 * 60);
#else
	constexpr DurationMS		TRANSACTION_TIMEOUT = DurationMS(1000 * 30);
#endif
	constexpr int		ENTITY_SIMPLE_TRANS_QUEUE = 4;
	constexpr int		ENTITY_SIMPLE_TRANSRES_QUEUE = 8;

	constexpr int		ENTITY_GAME_TRANS_QUEUE = 2048;
	constexpr int		ENTITY_GAME_TRANSRES_QUEUE = 2048;

	constexpr int		ENTITY_REMOTE_TRANS_QUEUE = 1024;
	constexpr int		ENTITY_REMOTE_TRANSRES_QUEUE = 1024;

	constexpr DurationMS		LOGIN_SESSION_EXPIRE_TIME = DurationMS(60 * 60 * 2); // 2 min

	constexpr int		MAX_NAME = 64;
	constexpr int		MAX_RANKING_LIST = 20;


	constexpr int		SERVER_TRANS_QUEUE = 1024;
	constexpr int		SERVER_TRANSRES_QUEUE = 1024;

	constexpr DurationMS		WORKLOAD_UPDATE_TIME = DurationMS(1500); // 1.5 sec

	constexpr DurationMS		MATCHING_QUEUE_RESERVATION_TIMEOUT = DurationMS(120 * 1000); // 2min

	// EntityServerEntity queue size
	constexpr int		ENTITY_ENTITY_TRANS_QUEUE = 2048;
	constexpr int		ENTITY_ENTITY_TRANSRES_QUEUE = 2048;

	constexpr DurationMS		REMOTE_CONNECTION_RETRY = DurationMS(5 * 1000);
	constexpr DurationMS		REMOTE_CONNECTION_RETRY_MAX = DurationMS(5 * 60 * 1000);

	constexpr DurationMS		SIMPLEUSER_TICKTASK_INTERVAL = DurationMS(60 * 1000);
	constexpr DurationMS		PARALLEL_TRANSACTION_MANAGER_TICKTASK_INTERVAL = DurationMS(1000);

	constexpr DurationMS		PARTY_TICKTASK_INTERVAL = DurationMS(2 * 60 * 1000);

		// Wait time for login sequences
#if 0
	constexpr DurationMS		LOGIN_TIME_WAIT_PLAYER_JOIN = DurationMS(60 * 1000);
#else
	constexpr DurationMS		LOGIN_TIME_WAIT_PLAYER_JOIN = DurationMS(2 * 60 * 1000);  // This is DB maximum, see spRegisterAuthTicket
#endif

	constexpr DurationMS		GAMEINSTANCE_TICK_TIME = DurationMS(1000);
	constexpr DurationMS		GAMEINSTANCE_EMPTYINSTANCE_KILL_TIMEOUT = DurationMS(60 * 1000);


	constexpr DurationMS		TIME_INTERVAL_RANKING_UPDATE = DurationMS(10 * 1000);


	constexpr int64_t RANKING_DB_MAX_REQUEST = 1000;
	constexpr int64_t RANKING_MAX_PLAYER = 1000 * RANKING_DB_MAX_REQUEST;

	constexpr const char* ZK_SERVER_INSTANCE_NODE = "BRServerInstances";

	constexpr int32_t RELAY_PLAYER_JOIN_TIMEOUT = 1 * 60 * 1000;
	constexpr int32_t RELAY_PLAYER_TIMEOUT = 30 * 1000;

} // namespace Const
} // namespace Svr
} // namespace SF

