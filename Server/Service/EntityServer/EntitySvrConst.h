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


namespace BR {
namespace EntityServer {
namespace Const {


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Category : Entity Server System
	//
	
	enum {
		// GameDBCache remote entity queue page size
		ENTITY_WORLDDBCACHE_TRANS_QUEUE			= 2048,
		ENTITY_WORLDDBCACHE_TRANSRES_QUEUE			= 2048,

		// Community remote entity queue page size
		ENTITY_COMMUNITY_TRANS_QUEUE			= 2048,
		ENTITY_COMMUNITY_TRANSRES_QUEUE			= 2048,

		// Login remote entity queue page size
		ENTITY_LOGIN_TRANS_QUEUE				= 2048,
		ENTITY_LOGIN_TRANSRES_QUEUE				= 2048,

		// Transaction remote entity queue page size
		ENTITY_TRANSACTION_TRANS_QUEUE			= 2048,
		ENTITY_TRANSACTION_TRANSRES_QUEUE		= 2048,

		// Game remote entity queue page size
		ENTITY_WORLD_TRANS_QUEUE				= 2048,
		ENTITY_WORLD_TRANSRES_QUEUE				= 2048,

		// Zone remote entity queue page size
		ENTITY_ZONE_TRANS_QUEUE					= 2048,
		ENTITY_ZONE_TRANSRES_QUEUE				= 2048,

		// Zone remote entity queue page size
		ENTITY_ENTITY_TRANS_QUEUE				= 2048,
		ENTITY_ENTITY_TRANSRES_QUEUE			= 2048,

		// Entity remote manager thread count
		ENTITY_REMOTEMANAGER_THREAD				= 4,

	};

} // namespace Const
} // namespace Svr
} // namespace BR

