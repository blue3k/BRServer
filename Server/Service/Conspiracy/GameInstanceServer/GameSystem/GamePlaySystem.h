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
#include "MemoryManager/SFMemoryPool.h"

#include "ServiceEntity/Game/GameSystem.h"
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
		
		static constexpr StringCrc32 ComponentID = "GamePlaySystem"_crc32c;
		static constexpr int MAX_WEREWOLF = 2;


		// vote filter functor
		typedef std::function<bool(GamePlayer*)> VoteFilterFunc;

		static const PlayerRole stm_PlayerRoleByCount[GameLogItem::LEGACY_MAX_GAMEPLAYER];

	private:
		typedef Svr::GameSystem<GameInstanceEntity, GamePlaySystem> super;

		// Count cache
		uint m_NumWereWolf;
		uint m_NumVillager;

		// suspects list
		PlayerID m_Suspects[2];

		// Dead peoples role
		PlayerRole m_LynchedRole;
		PlayerID m_LynchedPlayer;

		PlayerID m_HuntedPlayer;

		PlayerID m_Seer;

		PlayerID m_BodyGuard;

		PlayerID m_Owlman;
		PlayerID m_OwlmansChoice;

		// List of werewolves
		StaticArray<GamePlayer*,MAX_WEREWOLF> m_Werewolves;

	public:
		
		// Constructor 
		GamePlaySystem( GameInstanceEntity* pEntity );
		~GamePlaySystem();

		uint GetNumWereWolf() { return m_NumWereWolf; }
		void SetNumWereWolf(uint value) { m_NumWereWolf = value; }

		uint GetNumVillager() { return m_NumVillager; }
		void SetNumVillager(uint value) { m_NumVillager = value; }

		PlayerRole GetLynchedRole() { return m_LynchedRole; }
		void SetLynchedRole(PlayerRole value) { m_LynchedRole = value; }

		PlayerID GetLynchedPlayer() { return m_LynchedPlayer; }
		void SetLynchedPlayer(PlayerID value) { m_LynchedPlayer = value; }

		PlayerID GetHuntedPlayer() { return m_HuntedPlayer; }
		void SetHuntedPlayer(PlayerID value) { m_HuntedPlayer = value; }

		PlayerID GetSeer() { return m_Seer; }
		void SetSeer(PlayerID value) { m_Seer = value; }

		PlayerID GetBodyGuard() { return m_BodyGuard; }
		void SetBodyGuard(PlayerID value) { m_BodyGuard = value; }

		PlayerID GetOwlman() { return m_Owlman; }
		void SetOwlman(PlayerID value) { m_Owlman = value; }

		PlayerID GetOwlmansChoice() { return m_OwlmansChoice; }
		void SetOwlmansChoice(PlayerID value) { m_OwlmansChoice = value; }



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







