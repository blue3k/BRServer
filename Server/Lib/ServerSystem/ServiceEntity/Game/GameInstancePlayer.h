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

			bool m_IsBot;

			Util::TimeStampTimer m_TimeToRemove;

			TimeStampMS m_CurTime{};
			uint32_t m_CurMovementFrame{};

			//CriticalSection m_PlayerLock;
			ActorMovement m_LatestMovement;
			GameInstancePlayerComponentMovement* m_pMovementManager{};


		public:

			GameInstancePlayer(GameInstanceEntity* pGameOwner, EntityUID playerEntityUID, const PlayerInformation& player);
			virtual ~GameInstancePlayer();

			GameInstanceEntity* GetGameOwner() { return m_GameOwner; }

			uint32_t GetMovementFrame() const { return m_CurMovementFrame; }
			ActorMovement& GetLatestMovement() { return m_LatestMovement; }
			const ActorMovement& GetLatestMovement() const { return m_LatestMovement; }
			GameInstancePlayerComponentMovement* GetMovementManager() { return m_pMovementManager; }

			const VariableTable& GetCharacterVisual() const { return m_CharacterVisual; }
			const VariableTable& GetCharacterAttribute() const { return m_CharacterAttribute; }

			bool GetIsBot() { return m_IsBot; }
			void SetIsBot(bool value) { m_IsBot = value; }

			// Initialize player
			Result InitializePlayer(GameInstanceEntity* pGameOwner, bool isBot = false);

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
