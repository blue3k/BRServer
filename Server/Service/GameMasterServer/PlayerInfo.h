////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Player Information
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/TypeDefs.h"
#include "ServerSystem/BrServer.h"
#include "Common/MemoryPool.h"
#include "Common/ClassUtil.h"
#include "Common\GameConst.h"

namespace BR {
namespace Policy {
	interface IPolicyGameServer;
}
}


namespace BR {
namespace GameMasterServer {

	class GameServerEntity;


	//////////////////////////////////////////////////////////////////////////
	//
	//	PlayerInfo class
	//

	class PlayerInfo : public BR::MemoryPoolObject<PlayerInfo>
	{
	private:
		// network process policy
		BRCLASS_ATTRIBUTE_READONLY(Policy::IPolicyGameServer*,GameSvrPolicy);

		// Server ID that currently in
		BRCLASS_ATTRIBUTE(ServerID,ServerID);

		BRCLASS_ATTRIBUTE_READONLY(AccountID,AccountID);

		// Player Name
		char				m_strName[GameConst::MAX_NAME];


	public:
		PlayerInfo();
		virtual ~PlayerInfo();


		// Initialize
		HRESULT Initialize( 
			ServerID SvrID,
			Policy::IPolicyGameServer* pGameSvrPolicy, 
			AccountID accID,
			const char* strName );



		////////////////////////////////////////////////////////////////////////
		//
		//	Attributes 
		//

		// name
		inline const char* GetPlayerName() const;

		// User Entity
		inline Svr::SimpleUserEntity* GetUserEntity() const;

	};


#include "PlayerInfo.inl"

}; // namespace GameMasterServer
}; // namespace BR







