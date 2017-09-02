////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Remote Player class
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrTrace.h"
#include "Util/TimeUtil.h"
#include "Types/BrBaseTypes.h"
#include "ResultCode/SFResultCodeCommon.h"
#include "ResultCode/SFResultCodeGame.h"
#include "Net/Message.h"

#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/PlayerEntityInformation.h"


namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Remote Player class
	//

	PlayerEntityInformation::PlayerEntityInformation( const PlayerInformation& player )
		:m_PlayerInfo(player)
		,m_LatestServerUpTime(TimeStampSec::min())
		,m_ServerEntity(nullptr)
		,m_PlayerEntityUID(0)
		,m_IsActivePlayer(true)
	{
	}

	PlayerEntityInformation::~PlayerEntityInformation()
	{
	}

	
	// Set game server entity
	Result PlayerEntityInformation::SetServerEntity( ServerEntity* pServerEntity, EntityUID playerUID )
	{
		if( pServerEntity )
			m_LatestServerUpTime = pServerEntity->GetServerUpTime();

		m_ServerEntity = pServerEntity;

		m_PlayerEntityUID = playerUID;

		// Make the play inactive
		if( m_ServerEntity == nullptr || m_PlayerEntityUID == 0 )
			m_IsActivePlayer = false;

		return ResultCode::SUCCESS;
	}
	


}; // namespace Svr
}; // namespace SF


