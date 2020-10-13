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


namespace SF {
	namespace ConspiracyGameInstanceServer {
		namespace Const {


			////////////////////////////////////////////////////////////////////////////////
			//
			//	Category : Server system
			//

			// GameMaster remote entity queue page size
			static constexpr uint ENTITY_GAMEMASTER_TRANS_QUEUE = 2048;
			static constexpr uint ENTITY_GAMEMASTER_TRANSRES_QUEUE = 2048;

			// Game remote entity queue page size
			static constexpr uint ENTITY_GAME_TRANS_QUEUE = 2048;
			static constexpr uint ENTITY_GAME_TRANSRES_QUEUE = 2048;

			// Zone remote entity queue page size
			static constexpr uint ENTITY_ENTITY_TRANS_QUEUE = 2048;
			static constexpr uint ENTITY_ENTITY_TRANSRES_QUEUE = 2048;

			// Entity remote manager thread count
			static constexpr uint ENTITY_REMOTEMANAGER_THREAD = 4;

			static constexpr uint ENTITY_GAMEMANAGER_THREAD = 8;

			static constexpr uint TIME_WAIT_PLAYER_JOIN = 2 * 60 * 1000;
			static constexpr uint TIME_DELETE_DISCONNECTED_PLAYER = 60 * 1000;

			static constexpr uint ENTITY_LOBYGAME_MSGPROC_MAX = 10;

			// Time to delete zone instance that player count is 0(no one is playing)
			static constexpr uint TIME_DELETE_GAMEINSTANCE_NOPLAYER = 30 * 1000;

			// Time to delete zone instance delay
			static constexpr uint TIME_DELETE_GAMEINSTANCE_DELAY = 5 * 1000;

			static constexpr uint GAMEINSTANCE_TICK_TIME = 3 * 1000;


		} // namespace Const
	} // namespace ConspiracyGameInstanceServer
} // namespace SF

