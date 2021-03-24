////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : Game instance map object
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"

#include "Util/SFTimeUtil.h"
#include "Container/SFHashTable.h"
#include "MemoryManager/SFMemoryPool.h"
#include "GameConst.h"
#include "Types/BrGameTypes.h"

#include "Entity/Entity.h"
#include "ServiceEntity/Game/GameSystem.h"
#include "Entity/PlayerEntityInformation.h"
#include "Actor/Movement/SFActorMovement.h"


namespace SF {
	namespace Net {
		class Connection;
	}
}



namespace SF {
	namespace Svr {

		class GameInstancePlayer;
		class GameInstanceEntity;


		//////////////////////////////////////////////////////////////////////////
		//
		//	Game Instance map object class
		//


		class GameInstanceMapObject
		{
		private:

			// Game instance that this player
			GameInstanceEntity* m_GameOwner{};

			// Map object Id
			StringCrc32 m_MapObjectId{};

			// Occupied player Id
			PlayerID m_OccupiedPlayerId{};

			// Attribute
			VariableTable m_Attributes;

		public:

			GameInstanceMapObject(GameInstanceEntity* pGameOwner, StringCrc32 mapObjectId);
			virtual ~GameInstanceMapObject();

			GameInstanceEntity* GetGameOwner() { return m_GameOwner; }
			IHeap& GetHeap();
			
			StringCrc32 GetMapObjectID() const { return m_MapObjectId; }
			PlayerID GetOccupiedPlayerID() const { return m_OccupiedPlayerId; }

			bool IsOccupied() const { return GetOccupiedPlayerID() != 0; }
			Result SetOccupiedPlayer(PlayerID playerId);

			// use map object
			Result UseObject(GameInstancePlayer* pPlayer);
		};

	} // Svr
} // namespace SF
