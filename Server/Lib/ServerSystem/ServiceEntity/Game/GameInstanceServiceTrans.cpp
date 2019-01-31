////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "Memory/SFMemoryPool.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"

#include "Protocol/Message/GameInstanceManagerMsgClass.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameNetPolicy.h"


#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "Server/BrServer.h"
#include "ServerEntity/ServerEntityManager.h"
#include "Entity/EntityManager.h"
#include "ServiceEntity/Game/GameInstanceServiceTrans.h"
#include "ServiceEntity/Game/GameInstanceServiceEntity.h"
#include "GameInstanceServiceTrans.h"

SF_MEMORYPOOL_IMPLEMENT(SF::Svr::GameInstanceTrans);



namespace SF {
namespace Svr {



	GameInstanceTrans::GameInstanceTrans(IHeap& memoryManager, uint startMemberCount, uint targetMemberCount)
		: TransactionT(memoryManager, TransactionID() )
	{
		//SetPrintTrace(false);

		//BR_TRANS_MESSAGE( TimerResult, { return OnTimer(pRes); });
		//BR_TRANS_MESSAGE( Message::PartyGameQueue::ReserveItemRes,			{ return OnReserveItem(pRes); });
		//BR_TRANS_MESSAGE( Message::PartyGameQueue::CancelReservationRes,	{ return OnCancelReservation(pRes); });
		//BR_TRANS_MESSAGE( Message::PartyGameQueue::DequeueItemRes,			{ return OnDequeueItem(pRes); });
		//BR_TRANS_MESSAGE( Message::GameInstanceManager::CreateGameRes,			{ return OnCreateGame(pRes); });
	}

	// Start Transaction
	Result GameInstanceTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );


	Proc_End:

		if( !(hr) )
		{
			// We need to retry until it's successed
			CloseTransaction(hr);
		}

		return hr;
	}
	

};// namespace Svr 
};// namespace SF 

