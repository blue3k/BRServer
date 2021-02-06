////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Remote Player class
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "SvrConst.h"
#include "SvrTrace.h"
#include "Util/SFTimeUtil.h"
#include "Types/SFEngineTypedefs.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "Net/SFMessage.h"

#include "ServerEntity/ServerEntity.h"
#include "Entity/PlayerEntityInformation.h"


namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Remote Player class
	//

	PlayerEntityInformation::PlayerEntityInformation( const PlayerInformation& player )
		:m_PlayerInfo(player)
		,m_LatestServerUpTime(UTCTimeStampSec::min())
		,m_PlayerEntityUID(0)
		,m_IsActivePlayer(true)
	{
	}

	PlayerEntityInformation::~PlayerEntityInformation()
	{
	}

	
	// Set game server entity
	Result PlayerEntityInformation::SetRemoteEndpoint(const SharedPointerT<MessageEndpoint>& remoteEndpoint, EntityUID playerUID )
	{
		m_RemoteEndpoint = remoteEndpoint;

		m_PlayerEntityUID = playerUID;

		// Make the play inactive
		if( m_PlayerEntityUID.UID == 0 )
			m_IsActivePlayer = false;

		return ResultCode::SUCCESS;
	}
	


}; // namespace Svr
}; // namespace SF


