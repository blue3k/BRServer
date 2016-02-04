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

#include "Common/Typedefs.h"
#include "Common/ClassUtil.h"
#include "Common/TimeUtil.h"
#include "Common/HashTable.h"
#include "Common/MemoryPool.h"
#include "Common/GameConst.h"
#include "Common/BrGameTypes.h"

#include "ServerSystem/Entity.h"
#include "ServerSystem/GameSystem.h"
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
namespace Svr {


	class GameServer;
	class GameServerEntity;
	class GameInstanceEntity;


	//////////////////////////////////////////////////////////////////////////
	//
	//	Game Instance entity class
	//

	

	class GameInstancePlayer : public Svr::PlayerEntityInformation
	{
	public:

	private:

		// Game instance that this player
		BRCLASS_ATTRIBUTE_READONLY(GameInstanceEntity*,GameOwner);

		BRCLASS_ATTRIBUTE(bool, IsBot);

		//// Player index in game
		//BRCLASS_ATTRIBUTE(UINT,Index);
		//BRCLASS_ATTRIBUTE(UINT,Character);


		//// Player state
		//BRCLASS_ATTRIBUTE(PlayerState, PlayerState);

		//// Is revealed by seer?
		//BRCLASS_ATTRIBUTE(bool,RevealedBySeer);

		//// Is voted advance?
		//BRCLASS_ATTRIBUTE(bool,VotedGameAdvance);

		//// Revealed other
		//BRCLASS_ATTRIBUTE(UINT, ReviveCount);

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Player Info
		//

		// 
		//BRCLASS_ATTRIBUTE(PlayerRole, RequestedRole);
		//BRCLASS_ATTRIBUTE(PlayerRole, Role);
		//BRCLASS_ATTRIBUTE(UINT,Gender);

		//BRCLASS_ATTRIBUTE(UINT,Voted);
		//BRCLASS_ATTRIBUTE(PlayerID,Vote);


	public:

		GameInstancePlayer(GameInstanceEntity* pGameOwner, const PlayerInformation& player);
		virtual ~GameInstancePlayer();



		// Initialize player
		HRESULT InitializePlayer(GameInstanceEntity* pGameOwner, bool isBot = false);

		
		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Update
		//

		// Update Game Player 
		virtual HRESULT UpdateGamePlayer(TimeStampMS CurTime );
		
	};

#include "GameInstancePlayer.inl"

}; // Svr
}; // namespace BR
