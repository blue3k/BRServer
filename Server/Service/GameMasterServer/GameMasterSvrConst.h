////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game master server constants definitions. 
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"

namespace BR {
namespace GameMasterServer {
namespace Const {


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Category : Server system
	//

	enum {

		// Game remote entity queue page size
		ENTITY_GAME_TRANS_QUEUE					= 2048,
		ENTITY_GAME_TRANSRES_QUEUE				= 2048,

		// Zone remote entity queue page size
		ENTITY_ENTITY_TRANS_QUEUE				= 2048,
		ENTITY_ENTITY_TRANSRES_QUEUE			= 2048,

		// Entity remote manager thread count
		ENTITY_REMOTEMANAGER_THREAD				= 4,


		// Limit try count for channel search
		LIMIT_WAIT_SEARCH_CHANNEL				= 20,

		// Limit try count for channel search
		MAX_STATIC_ZONE_CHANNEL					= 256,
	};

} // namespace Const
} // namespace GameMasterServer
} // namespace BR

