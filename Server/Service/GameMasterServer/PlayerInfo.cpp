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


#include "stdafx.h"
#include "Common/TypeDefs.h"
#include "Common/HRESSvrSys.h"
#include "PlayerInfo.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerEntity/GameServerEntity.h"
#include "Protocol/Policy/GameMasterServerIPolicy.h"


BR_MEMORYPOOL_IMPLEMENT(GameMasterServer::PlayerInfo);


namespace BR {
namespace GameMasterServer {


	//////////////////////////////////////////////////////////////////////////////
	//
	// PlayerInfo Class
	//

	PlayerInfo::PlayerInfo()
		:m_GameSvrPolicy(nullptr),
		m_ServerID(0),
		m_AccountID(0)
	{
	}

	PlayerInfo::~PlayerInfo()
	{
	}

	HRESULT PlayerInfo::Initialize( 
			ServerID SvrID,
			Policy::IPolicyGameServer* pGameSvrPolicy, 
			AccountID accID,
			const char* strName )
	{
		m_ServerID = SvrID;

		m_AccountID = accID;

		m_GameSvrPolicy = pGameSvrPolicy;

		Assert(m_GameSvrPolicy);

		StrUtil::StringCpy( m_strName, strName );

		return m_GameSvrPolicy ? S_OK : E_UNEXPECTED;
	}


}; // namespace GameMasterServer
}; // namespace BR






