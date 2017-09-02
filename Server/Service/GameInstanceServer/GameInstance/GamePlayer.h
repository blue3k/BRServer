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
#include "Common/ClassUtil.h"
#include "Util/TimeUtil.h"
#include "Common/HashTable.h"
#include "Memory/MemoryPool.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/GameSystem.h"
#include "ServerSystem/ServiceEntity/Game/GameInstancePlayer.h"
#include "Common/GameConst.h"
#include "Types/BrGameTypes.h"

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

	class GamePlayer : public Svr::GameInstancePlayer, public MemoryPoolObject<GamePlayer>
	{
	public:
		typedef Svr::GameInstancePlayer super;

	private:


		// Player index in game
		BRCLASS_ATTRIBUTE(UINT,Index);
		BRCLASS_ATTRIBUTE(UINT,Character);

		// Player state
		BRCLASS_ATTRIBUTE(PlayerState,PlayerState);

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

		// 
		BRCLASS_ATTRIBUTE(PlayerRole, RequestedRole);
		BRCLASS_ATTRIBUTE(PlayerRole, Role);
		BRCLASS_ATTRIBUTE(UINT,Gender);

		BRCLASS_ATTRIBUTE(UINT,Voted);
		BRCLASS_ATTRIBUTE(PlayerID,Vote);


	public:

		GamePlayer(GameInstanceEntity* pGameOwner, const PlayerInformation& player);
		virtual ~GamePlayer();


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
}; // namespace BR
