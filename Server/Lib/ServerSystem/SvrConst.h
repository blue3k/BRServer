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

	extern INT		ENTITY_REMOTE_TRANS_QUEUE;
	extern INT		ENTITY_REMOTE_TRANSRES_QUEUE;


	extern INT		LOGIN_SESSION_EXPIRE_TIME;

	enum {
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

		DEFAULT_TICKTASK_INTERVAL = 1000,

#if defined(DEBUG)
		SIMPLEUSER_TICKTASK_INTERVAL = 60 * 1000,
		PARALLEL_TRANSACTION_MANAGER_TICKTASK_INTERVAL = 60 * 1000,
#else
		SIMPLEUSER_TICKTASK_INTERVAL = 15 * 60 * 1000,
		PARALLEL_TRANSACTION_MANAGER_TICKTASK_INTERVAL = 2 * 60 * 1000,
#endif

		PARTY_TICKTASK_INTERVAL = 2 * 60 * 1000,
	};



}; // namespace Const
}; // namespace Svr
}; // namespace BR

