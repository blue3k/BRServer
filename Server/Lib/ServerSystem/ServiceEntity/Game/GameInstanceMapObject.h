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
			StringCrc32 m_MapObjectType;
			uint32_t m_MapObjectId{};
			uint32_t m_MapObjectTableId{};

			// Occupied player Id
			PlayerID m_OccupiedPlayerId{};

			// object Position
			Vector4 m_Position{};

			// Attribute
			VariableTable m_Attributes;

		public:

			GameInstanceMapObject(GameInstanceEntity* pGameOwner, StringCrc32 mapObjectType, uint32_t mapObjectId, uint32_t mapObjectTableId);
			virtual ~GameInstanceMapObject();

			GameInstanceEntity* GetGameOwner() { return m_GameOwner; }
			IHeap& GetHeap();

			// map object position
			SF_FORCEINLINE void SetPosition(const Vector4& pos) { m_Position = pos; }
			SF_FORCEINLINE const Vector4& GetPosition() const { return m_Position; }
			
			SF_FORCEINLINE uint32_t GetMapObjectID() const { return m_MapObjectId; }
			SF_FORCEINLINE uint32_t GetMapObjectTableID() const { return m_MapObjectTableId; }
			SF_FORCEINLINE StringCrc32 GetMapObjectType() const { return m_MapObjectType; }
			SF_FORCEINLINE PlayerID GetOccupiedPlayerID() const { return m_OccupiedPlayerId; }

			SF_FORCEINLINE bool IsOccupied() const { return GetOccupiedPlayerID() != 0; }
			Result SetOccupiedPlayer(PlayerID playerId);
			Result ObjectReleased(PlayerID playerId);
			Result ObjectReleased();

			// use map object
			Result UseObject(GameInstancePlayer* pPlayer);
		};

	} // Svr
} // namespace SF
