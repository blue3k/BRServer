////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
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
#include "Entity/Entity.h"
#include "ServiceEntity/Game/GameSystem.h"
#include "ServiceEntity/Game/GameInstancePlayer.h"
#include "GameConst.h"
#include "Types/BrGameTypes.h"

#include "ConspiracyGameInstanceSvrConst.h"
#include "Entity/PlayerEntityInformation.h"


namespace SF {
namespace Net {
	class Connection;
};

namespace Policy {
	class ISvrPolicyGame;
	class IPolicyGameServer;
	class ISvrPolicyGameInstance;
	class ISvrPolicyGameServer;
};
};



namespace SF {
namespace ConspiracyGameInstanceServer {


	class GameServer;
	class GameServerEntity;
	class InvenSystem;
	class GameInstanceEntity;


	//////////////////////////////////////////////////////////////////////////
	//
	//	Game Instance entity class
	//

	
	// Player state
	enum class PlayerState
	{
		None,		// Nothing just created
		Playing,	// Playing
		Ghost,		// dead
	};

	class GamePlayer : public Svr::GameInstancePlayer
	{
	public:
		typedef Svr::GameInstancePlayer super;

	private:


		// Player index in game
		uint m_Index;
		uint m_Character;

		// Player state
		PlayerState m_PlayerState;

		// Is revealed by seer?
		bool m_RevealedBySeer;

		// Is voted advance?
		bool m_VotedGameAdvance;

		// Revealed other
		uint m_ReviveCount;

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Player Info
		//

		// 
		PlayerRole m_RequestedRole;
		PlayerRole m_Role;
		uint m_Gender;

		uint m_Voted;
		PlayerID m_Vote;


	public:

		GamePlayer(GameInstanceEntity* pGameOwner, EntityUID playerEntityUID, const PlayerInformation& player);
		virtual ~GamePlayer();

		uint GetIndex() { return m_Index; }
		void SetIndex(uint value) { m_Index = value; }

		uint GetCharacter() { return m_Character; }
		void SetCharacter(uint value) { m_Character = value; }


		PlayerState GetPlayerState() { return m_PlayerState; }
		void SetPlayerState(PlayerState value) { m_PlayerState = value; }

		bool GetRevealedBySeer() { return m_RevealedBySeer; }
		void SetRevealedBySeer(bool value) { m_RevealedBySeer = value; }


		bool GetVotedGameAdvance() { return m_VotedGameAdvance; }
		void SetVotedGameAdvance(bool value) { m_VotedGameAdvance = value; }


		uint GetReviveCount() { return m_ReviveCount; }
		void SetReviveCount(uint value) { m_ReviveCount = value; }


		PlayerRole GetRequestedRole() { return m_RequestedRole; }
		void SetRequestedRole(PlayerRole value) { m_RequestedRole = value; }

		PlayerRole GetRole() { return m_Role; }
		void SetRole(PlayerRole value) { m_Role = value; }

		uint GetGender() { return m_Gender; }
		void SetGender(uint value) { m_Gender = value; }


		uint GetVoted() { return m_Voted; }
		void SetVoted(uint value) { m_Voted = value; }


		PlayerID GetVote() { return m_Vote; }
		void SetVote(PlayerID value) { m_Vote = value; }



		GameInstanceEntity* GetGameOwner() { return (GameInstanceEntity*)super::GetGameOwner(); }

		// check weather the player is in game or not
		bool IsInGame();

		// Initialize player
		Result InitializePlayer(GameInstanceEntity* pGameOwner, bool isBot = false);

		
		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Update
		//

		// Update Game Player 
		Result UpdateGamePlayer(TimeStampMS CurTime );
		
		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	
		//

		bool IsWinnerSide(GameWinner winner);
	};

#include "GamePlayer.inl"

}; // ConspiracyGameInstanceServer
}; // namespace SF
