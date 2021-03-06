////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : Remote Entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"
#include "Util/SFStrUtil.h"
#include "Util/SFTimeUtil.h"
#include "ServerLog/SvrLog.h"
#include "Multithread/SFThread.h"
#include "SvrConst.h"
#include "ServerEntity/ServerEntity.h"
#include "Transaction/Transaction.h"
#include "SvrTrace.h"
#include "ServerEntity/GenericServerEntity.h"
#include "Transaction/ServerTransactionGeneric.h"

#include "Protocol/GameServerNetPolicy.h"



namespace SF {
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

		//BR_ENTITY_MESSAGE(Message::Server::ServerConnectedC2SEvt) { svrMemReturn(pNewTrans = new(GetHeap()) Svr::GenericServerStartedTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );

	Proc_End:

		return hr;
	}



}; // namespace Svr
}; // namespace SF



