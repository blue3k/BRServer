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
#include "Common/ClassUtil.h"
#include "Util/TimeUtil.h"
#include "Common/HashTable.h"
#include "Memory/MemoryPool.h"
#include "GameConst.h"
#include "Types/BrGameTypes.h"

#include "Entity/Entity.h"
#include "ServiceEntity/Game/GameSystem.h"
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
		//BRCLASS_ATTRIBUTE(uint,Index);
		//BRCLASS_ATTRIBUTE(uint,Character);


		//// Player state
		//BRCLASS_ATTRIBUTE(PlayerState, PlayerState);

		//// Is revealed by seer?
		//BRCLASS_ATTRIBUTE(bool,RevealedBySeer);

		//// Is voted advance?
		//BRCLASS_ATTRIBUTE(bool,VotedGameAdvance);

		//// Revealed other
		//BRCLASS_ATTRIBUTE(uint, ReviveCount);

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Player Info
		//

		// 
		//BRCLASS_ATTRIBUTE(PlayerRole, RequestedRole);
		//BRCLASS_ATTRIBUTE(PlayerRole, Role);
		//BRCLASS_ATTRIBUTE(uint,Gender);

		//BRCLASS_ATTRIBUTE(uint,Voted);
		//BRCLASS_ATTRIBUTE(PlayerID,Vote);


	public:

		GameInstancePlayer(GameInstanceEntity* pGameOwner, const PlayerInformation& player);
		virtual ~GameInstancePlayer();



		// Initialize player
		Result InitializePlayer(GameInstanceEntity* pGameOwner, bool isBot = false);

		
		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Update
		//

		// Update Game Player 
		virtual Result UpdateGamePlayer(TimeStampMS CurTime );
		
	};

#include "GameInstancePlayer.inl"

}; // Svr
}; // namespace SF
