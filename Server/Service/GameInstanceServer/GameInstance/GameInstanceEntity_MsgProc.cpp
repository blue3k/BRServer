////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game Instance entity
//	
////////////////////////////////////////////////////////////////////////////////



#include "GameInstancePCH.h"
#include "SvrConst.h"
#include "SvrTrace.h"
#include "Util/SFTimeUtil.h"
#include "Types/SFEngineTypedefs.h"

#include "ConspiracyGameInstanceSvrConst.h"
#include "ConspiracyGameInstanceServerClass.h"

#include "Protocol/Policy/GameServerNetPolicy.h"

#include "Protocol/Message/GameMsgClass.h"
#include "ConspiracyGameInstanceServerClass.h"
#include "GameInstanceEntity.h"
#include "GamePlayer.h"

#include "Transaction/GameInstanceTransPlayer.h"


namespace SF {
namespace ConspiracyGameInstanceServer {

	
	////////////////////////////////////////////////////////////
	//
	//	network process
	//

	//// Process connection event
	//Result GameInstanceEntity::ProcessConnectionEvent( GamePlayer *pGamePlayer, Net::Connection *pCon, const Net::ConnectionEvent& conEvent )
	//{
	//	Result hr = ResultCode::SUCCESS;

	//	svrChkPtr( pGamePlayer );
	//	svrChkPtr( pCon );

	//	switch( conEvent.EventType )
	//	{
	//	case Net::ConnectionEvent::EVT_CONNECTION_RESULT:
	//		break;
	//	case Net::ConnectionEvent::EVT_DISCONNECTED:
	//		//pGamePlayer->SetupKickTimer( Util::MainTimer.GetTimeStampMs(), Const::TIME_DELETE_DISCONNECTED_PLAYER );
	//		break;
	//	case Net::ConnectionEvent::EVT_STATE_CHANGE:

	//		break;
	//	};

	//Proc_End:

	//	return ResultCode::SUCCESS;
	//}


} // ConspiracyGameInstanceServer
} // namespace SF


