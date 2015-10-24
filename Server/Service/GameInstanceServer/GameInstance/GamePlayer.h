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

#include "Common/TypeDefs.h"
#include "Common/ClassUtil.h"
#include "Common/TimeUtil.h"
#include "Common/HashTable.h"
#include "Common/MemoryPool.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/GameSystem.h"
#include "Common/GameConst.h"
#include "Common/BrGameTypes.h"

#include "ConspiracyGameInstanceSvrConst.h"
#include "ServerSystem/PlayerEntityInformation.h"


namespace BR {
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



namespace BR {
namespace ConspiracyGameInstanceServer {


	class GameServer;
	class GameServerEntity;
	class InvenSystem;
	class GameInstanceEntity;
	class GameMasterServerEntity;


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

	class GamePlayer : public Svr::PlayerEntityInformation, public MemoryPoolObject<GamePlayer>
	{
	public:

	private:

		// Game instance that this player
		BRCLASS_ATTRIBUTE_READONLY(GameInstanceEntity*,GameOwner);

		// Player index in game
		BRCLASS_ATTRIBUTE(UINT,Index);
		BRCLASS_ATTRIBUTE(UINT,Character);

		// Player state
		BRCLASS_ATTRIBUTE(PlayerState,PlayerState);

		BRCLASS_ATTRIBUTE(bool, IsBot);

		// Is revealed by seer?
		BRCLASS_ATTRIBUTE(bool,RevealedBySeer);

		// Is voted advance?
		BRCLASS_ATTRIBUTE(bool,VotedGameAdvance);

		// Revealed other
		BRCLASS_ATTRIBUTE(UINT, ReviveCount);

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Player Info
		//

		//BRCLASS_ATTRIBUTE(AuthTicket,AuthTicket);

		// 
		BRCLASS_ATTRIBUTE(PlayerRole, RequestedRole);
		BRCLASS_ATTRIBUTE(PlayerRole, Role);
		BRCLASS_ATTRIBUTE(UINT,Gender);

		BRCLASS_ATTRIBUTE(UINT,Voted);
		BRCLASS_ATTRIBUTE(PlayerID,Vote);


	public:

		GamePlayer(GameInstanceEntity* pGameOwner, const PlayerInformation& player);
		virtual ~GamePlayer();

		// check weather the player is in game or not
		FORCEINLINE bool IsInGame();

		// Initialize player
		HRESULT InitializePlayer(GameInstanceEntity* pGameOwner, bool isBot = false);

		
		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Update
		//

		// Update Game Player 
		HRESULT UpdateGamePlayer( ULONG CurTime );
		
		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	
		//

		bool IsWinnerSide(GameWinner winner);
	};

#include "GamePlayer.inl"

}; // ConspiracyGameInstanceServer
}; // namespace BR
