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
#include "String/SFStrUtil.h"
#include "Util/SFTimeUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/SFThread.h"
#include "SvrConst.h"
#include "SvrConst.h"
#include "ServerEntity/ServerEntity.h"
#include "Transaction/Transaction.h"
#include "SvrTrace.h"
#include "ServerEntity/SvrGameServerEntity.h"
#include "Transaction/ServerTransactionGeneric.h"

#include "Protocol/Policy/GameServerNetPolicy.h"



namespace SF {
namespace Svr {
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Game server entity
	//


	GameServerEntity::GameServerEntity()
		:ServerEntity(Const::ENTITY_GAME_TRANS_QUEUE, Const::ENTITY_GAME_TRANSRES_QUEUE)
	{
	}

	GameServerEntity::~GameServerEntity()
	{
	}


	// Initialize entity to proceed new connection
	Result GameServerEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(ServerEntity::InitializeEntity( newEntityID ) );

		BR_ENTITY_MESSAGE(Message::Server::ServerConnectedC2SEvt) { svrMemReturn(pNewTrans = new(GetHeap()) Svr::GenericServerStartedTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );

	Proc_End:

		return hr;
	}



}; // namespace Svr
}; // namespace SF



