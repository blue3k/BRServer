////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server constants definitions. 
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"


namespace BR {
namespace ConspiracyGameInstanceServer {
namespace Const {


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Category : Server system
	//

	enum {

		// GameMaster remote entity queue page size
		ENTITY_GAMEMASTER_TRANS_QUEUE			= 2048,
		ENTITY_GAMEMASTER_TRANSRES_QUEUE		= 2048,

		// Game remote entity queue page size
		ENTITY_GAME_TRANS_QUEUE				= 2048,
		ENTITY_GAME_TRANSRES_QUEUE				= 2048,

		// Zone remote entity queue page size
		ENTITY_ENTITY_TRANS_QUEUE				= 2048,
		ENTITY_ENTITY_TRANSRES_QUEUE			= 2048,

		// Entity remote manager thread count
		ENTITY_REMOTEMANAGER_THREAD				= 4,

		ENTITY_GAMEMANAGER_THREAD				= 8,

		TIME_WAIT_PLAYER_JOIN					= 2*60*1000,
		TIME_DELETE_DISCONNECTED_PLAYER			= 60*1000,

		ENTITY_LOBYGAME_MSGPROC_MAX				= 10,
		
		// Time to delete zone instance that player count is 0(noone playing)
		TIME_DELETE_GAMEINSTANCE_NOPLAYER		= 30*1000,

		// Time to delete zone instance delay
		TIME_DELETE_GAMEINSTANCE_DELAY			= 5*1000,


		GAMEINSTANCE_TICK_TIME = 3 * 1000,
	};


}; // namespace Const
}; // namespace ConspiracyGameInstanceServer
}; // namespace BR

