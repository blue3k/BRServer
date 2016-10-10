////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Remote Entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "ServerSystem/SvrConstDefault.h"
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/Transaction.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/ServerEntity/GenericServerEntity.h"
#include "ServerSystem/Transaction/ServerTransactionGeneric.h"

#include "Protocol/Policy/GameServerIPolicy.h"



namespace BR {
namespace Svr {
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Game server entity
	//


	GenericServerEntity::GenericServerEntity()
		:ServerEntity(Const::ENTITY_GAME_TRANS_QUEUE, Const::ENTITY_GAME_TRANSRES_QUEUE)
	{
	}

	GenericServerEntity::~GenericServerEntity()
	{
	}


	// Initialize entity to proceed new connection
	Result GenericServerEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(ServerEntity::InitializeEntity( newEntityID ) );

		BR_ENTITY_MESSAGE(Message::Server::ServerConnectedC2SEvt) { svrMemReturn(pNewTrans = new Svr::GenericServerStartedTrans(pMsgData)); return ResultCode::SUCCESS; } );

	Proc_End:

		return hr;
	}



}; // namespace Svr
}; // namespace BR



