////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game play system
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/TypeDefs.h"
#include "Common/BrGameTypes.h"
#include "Common/MemoryPool.h"
#include "Common/ClassUtil.h"
#include "ServerSystem/GameSystem.h"
#include "Common/ArrayUtil.h"
#include "Common/GameConst.h"
#include "GameSystemComponentIDs.h"

namespace BR {
namespace ConspiracyGameInstanceServer {

	class GameInstanceEntity;
	class GamePlayer;
	class GamePlaySystem;
	class GameStateSystem;


	//////////////////////////////////////////////////////////////////////////
	//
	//	GamePlaySystem class
	//

	class GamePlaySystem : public Svr::GameSystem<GameInstanceEntity,GamePlaySystem>
	{
	public:
		enum {
			ComponentID = GameSystemComponentID_GamePlay,

			MAX_WEREWOLF = 2,
		};


		// vote filter functor
		typedef std::function<bool(GamePlayer*)> VoteFilterFunc;

		static const PlayerRole stm_PlayerRoleByCount[GameConst::MAX_GAMEPLAYER];

	private:

		// Count cache
		BRCLASS_ATTRIBUTE_READONLY(UINT,NumWereWolf);
		BRCLASS_ATTRIBUTE_READONLY(UINT,NumVillager);

		// suspects list
		PlayerID m_Suspects[2];

		// Dead peoples role
		BRCLASS_ATTRIBUTE_READONLY(PlayerRole,LynchedRole);
		BRCLASS_ATTRIBUTE_READONLY(PlayerID,LynchedPlayer);

		BRCLASS_ATTRIBUTE(PlayerID,HuntedPlayer);

		BRCLASS_ATTRIBUTE(PlayerID,Seer);

		BRCLASS_ATTRIBUTE(PlayerID,BodyGuard);

		BRCLASS_ATTRIBUTE(PlayerID,Owlman);
		BRCLASS_ATTRIBUTE(PlayerID,OwlmansChoice);

		// List of werewolfes
		StaticArray<GamePlayer*,MAX_WEREWOLF> m_Werewolves;

	public:
		
		// Constructor 
		GamePlaySystem( GameInstanceEntity* pEntity );
		~GamePlaySystem();

		// Set suspect list
		void SetSuspect( PlayerID suspect1, PlayerID suspect2 );

		// Get suspect list
		FORCEINLINE PlayerID GetSuspect( int index );
		FORCEINLINE UINT GetNumberOfSuspects();
		FORCEINLINE bool IsSuspect( PlayerID playerID );

		FORCEINLINE const Array<GamePlayer*>& GetWerewolves();

		// Initialize server component
		virtual HRESULT InitializeComponent();

		// Assign roles
		HRESULT AssignRole();

		// Update Villager and werewolf count
		HRESULT UpdateVillagerNWerewolfCount();

		// Check game end condition
		GameWinner GetGameWinner();
		HRESULT UpdateGameEnd();

		// 
		PlayerID PickAnyoneCloseToHuntedPlayer( PlayerID exculdePlayer );

		// Kill Player
		HRESULT KillPlayer( PlayerID playerToKill, PlayerKilledReason reason );
		HRESULT KillPlayer( GamePlayer* pPlayerToKill, PlayerKilledReason reason );

		HRESULT RevivePlayer(GamePlayer* pPlayerToRevive);

		// Get revealed role
		PlayerRole GetRevealedRole( GamePlayer* pPlayer, GamePlayer* pOtherPlayer );

		// Called when a player get out of game
		HRESULT OnPlayerGetOutOfGame( GamePlayer* pPlayer );
	};


#include "GamePlaySystem.inl"

}; // namespace ConspiracyGameInstanceServer
}; // namespace BR







