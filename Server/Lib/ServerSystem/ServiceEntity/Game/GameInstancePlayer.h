////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game player
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


		class GameServer;
		class GameServerEntity;
		class GameInstanceEntity;
		class GameInstancePlayerComponentMovement;


		//////////////////////////////////////////////////////////////////////////
		//
		//	Game Instance entity class
		//


		class GameInstancePlayerComponentManager : public ComponentManager
		{
		public:
			GameInstancePlayerComponentManager(IHeap& heap)
				: ComponentManager(heap)
			{
			}

		};


		//////////////////////////////////////////////////////////////////////////
		//
		//	Game Instance entity class
		//



		class GameInstancePlayer : public Svr::PlayerEntityInformation
		{
		public:
			using super = Svr::PlayerEntityInformation;

		private:

			// Game instance that this player
			GameInstanceEntity* m_GameOwner;

			VariableTable m_CharacterVisual;
			VariableTable m_CharacterAttribute;

			GameInstancePlayerComponentManager m_ComponentManager;

			// player state
			StringCrc32 m_PlayerState;

			uint32_t m_OccupiedObjectId{};
			uint32_t m_OccupiedObjectTableId{};
			StringCrc32 m_OccupiedObjectType{};

			bool m_IsJoined{};

			Util::TimeStampTimer m_TimeToRemove;

			TimeStampMS m_CurTime{};
			uint32_t m_CurMovementFrame{};
			uint32_t m_BroadcastedMovementFrame{};

			//CriticalSection m_PlayerLock;
			ActorMovement m_LatestMovement;
			GameInstancePlayerComponentMovement* m_pMovementManager{};

		public:

			GameInstancePlayer(GameInstanceEntity* pGameOwner, EntityUID playerEntityUID, const PlayerInformation& player);
			virtual ~GameInstancePlayer();

			GameInstanceEntity* GetGameOwner() { return m_GameOwner; }

			GameInstancePlayerComponentManager& GetComponentManager() { return m_ComponentManager; }

			SF_FORCEINLINE uint32_t GetOccupiedObjectId() const { return m_OccupiedObjectId; }
			SF_FORCEINLINE uint32_t GetOccupiedObjectTableId() const { return m_OccupiedObjectTableId; }
			SF_FORCEINLINE StringCrc32 GetOccupiedObjectType() const { return m_OccupiedObjectType; }
			Result OccupyObject(StringCrc32 mapObjectType, uint32_t mapObjectId, uint32_t mapObjectTableId);
			void ClearOccupyObject();

			// Player state accessor
			SF_FORCEINLINE StringCrc32 GetPlayerState() const { return m_PlayerState; }
			SF_FORCEINLINE void SetPlayerState(StringCrc32 newState) {
				m_PlayerState = newState;
			}

			// Movement frame the play is on
			uint32_t GetMovementFrame() const { return m_CurMovementFrame; }
			void SetMovementFrame(uint32_t newFrame) { m_CurMovementFrame = newFrame; }

			// latest broad casted movement frame
			uint32_t GetBroadCastedMovementFrame() const { return m_BroadcastedMovementFrame; }
			void SetBroadCastedMovementFrame(uint32_t newMovementFrame) { m_BroadcastedMovementFrame = newMovementFrame; }

			// latest movement
			const ActorMovement& GetLatestMovement() const { return m_LatestMovement; }
			void SetLatestMovement(const ActorMovement& src) { m_LatestMovement = src; }

			// Quick access to component
			GameInstancePlayerComponentMovement* GetMovementManager() { return m_pMovementManager; }

			// Character information
			const VariableTable& GetCharacterVisual() const { return m_CharacterVisual; }
			const VariableTable& GetCharacterAttribute() const { return m_CharacterAttribute; }

			bool IsJoined() const { return m_IsJoined; }
			void SetJoined(bool value) { m_IsJoined = value; }

			// Initialize player
			virtual Result InitializePlayer(GameInstanceEntity* pGameOwner);

			Result SetCharacterData(const VariableTable& characterVisual, const VariableTable& characterAttribute);

			Util::TimeStampTimer& GetRemoveTimer() { return m_TimeToRemove; }

			//////////////////////////////////////////////////////////////////////////////////////////////////
			//
			//	Update
			//

			// Update Game Player 
			virtual Result UpdateGamePlayer(TimeStampMS CurTime, uint32_t movementFrame);

		};

	} // Svr
} // namespace SF
