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

#include "SFTypedefs.h"
#include "Types/BrGameTypes.h"
#include "Memory/MemoryPool.h"
#include "Common/ClassUtil.h"
#include "ServerSystem/GameSystem.h"
#include "Container/SFArray.h"
#include "GameConst.h"
#include "GameSystemComponentIDs.h"

namespace SF {
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
		typedef Svr::GameSystem<GameInstanceEntity, GamePlaySystem> super;

		// Count cache
		BRCLASS_ATTRIBUTE_READONLY(uint,NumWereWolf);
		BRCLASS_ATTRIBUTE_READONLY(uint,NumVillager);

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
		PlayerID GetSuspect( int index );
		uint GetNumberOfSuspects();
		bool IsSuspect( PlayerID playerID );

		const Array<GamePlayer*>& GetWerewolves();

		// Initialize server component
		virtual Result InitializeComponent();

		// Assign roles
		Result PlayerRoleAssigned(GamePlayer* pPlayer);
		Result AssignRole();

		// Update Villager and werewolf count
		Result UpdateVillagerNWerewolfCount();

		// Check game end condition
		GameWinner GetGameWinner();
		Result UpdateGameEnd();

		// 
		PlayerID PickAnyoneCloseToHuntedPlayer( PlayerID exculdePlayer );

		// Kill Player
		Result KillPlayer( PlayerID playerToKill, PlayerKilledReason reason );
		Result KillPlayer( GamePlayer* pPlayerToKill, PlayerKilledReason reason );

		Result RevivePlayer(GamePlayer* pPlayerToRevive);

		// Get revealed role
		PlayerRole GetRevealedRole( GamePlayer* pPlayer, GamePlayer* pOtherPlayer );

		// Called when a player get out of game
		Result OnPlayerGetOutOfGame( GamePlayer* pPlayer );

		Result BroadCastChatMessage(GamePlayer *pMyPlayer, PlayerRole role, const char* chatMessage);

		Result BroadCastRandomBotMessage(int minID, int maxID, int& index);
		Result BroadCastRandomBotMessage(PlayerRole roleToTalk, int minID, int maxID, int& index);
		Result BroadCastRandomBotMessageSuspect(int minID, int maxID, int& index);
	};


#include "GamePlaySystem.inl"

}; // namespace ConspiracyGameInstanceServer
}; // namespace SF







