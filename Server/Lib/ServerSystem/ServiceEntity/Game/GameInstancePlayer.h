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

#include "Util/TimeUtil.h"
#include "Container/HashTable.h"
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
		GameInstanceEntity* m_GameOwner;

		bool m_IsBot;


	public:

		GameInstancePlayer(GameInstanceEntity* pGameOwner, const PlayerInformation& player);
		virtual ~GameInstancePlayer();

		GameInstanceEntity* GetGameOwner() { return m_GameOwner; }

		bool GetIsBot() { return m_IsBot; }
		void SetIsBot(bool value) { m_IsBot = value; }

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
