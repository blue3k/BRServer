////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game Instance entity
//	
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrTrace.h"
#include "Util/TimeUtil.h"
#include "Types/BrBaseTypes.h"

#include "ConspiracyGameInstanceSvrConst.h"
#include "ConspiracyGameInstanceServerClass.h"


#include "Protocol/Policy/EntityServerIPolicy.h"
#include "Protocol/Policy/GameServerIPolicy.h"

#include "Protocol/Message/GameMsgClass.h"
#include "ConspiracyGameInstanceServerClass.h"
#include "GameInstanceEntity.h"
#include "GamePlayer.h"

#include "Transaction/GameInstanceTransPlayer.h"


namespace BR {
namespace ConspiracyGameInstanceServer {

	
	////////////////////////////////////////////////////////////
	//
	//	network process
	//

	//// Process connection event
	//Result GameInstanceEntity::ProcessConnectionEvent( GamePlayer *pGamePlayer, Net::Connection *pCon, const Net::IConnection::Event& conEvent )
	//{
	//	Result hr = ResultCode::SUCCESS;

	//	svrChkPtr( pGamePlayer );
	//	svrChkPtr( pCon );

	//	switch( conEvent.EventType )
	//	{
	//	case Net::IConnection::Event::EVT_CONNECTION_RESULT:
	//		break;
	//	case Net::IConnection::Event::EVT_DISCONNECTED:
	//		//pGamePlayer->SetupKickTimer( Util::MainTimer.GetTimeStampMs(), Const::TIME_DELETE_DISCONNECTED_PLAYER );
	//		break;
	//	case Net::IConnection::Event::EVT_STATE_CHANGE:

	//		break;
	//	};

	//Proc_End:

	//	return ResultCode::SUCCESS;
	//}


} // ConspiracyGameInstanceServer
} // namespace BR


