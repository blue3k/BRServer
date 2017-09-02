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
#include "ResultCode/SFResultCodeCommon.h"
#include "ResultCode/SFResultCodeGame.h"
#include "Memory/MemoryPool.h"
#include "Types/BrBaseTypes.h"
#include "Common/GameConst.h"

#include "Protocol/Message/GameInstanceManagerMsgClass.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameIPolicy.h"


#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServiceEntity/Game/GameServiceTrans.h"
#include "ServerSystem/ServiceEntity/Game/GameServiceEntity.h"

SF_MEMORYPOOL_IMPLEMENT(BR::Svr::GamePartyTrans);



namespace SF {
namespace Svr {



	GamePartyTrans::GamePartyTrans(UINT startMemberCount, UINT targetMemberCount)
		: TransactionT( TransactionID() )
	{
		//BR_TRANS_MESSAGE( TimerResult, { return OnTimer(pRes); });
		//BR_TRANS_MESSAGE( Message::PartyGameQueue::ReserveItemRes,			{ return OnReserveItem(pRes); });
		//BR_TRANS_MESSAGE( Message::PartyGameQueue::CancelReservationRes,	{ return OnCancelReservation(pRes); });
		//BR_TRANS_MESSAGE( Message::PartyGameQueue::DequeueItemRes,			{ return OnDequeueItem(pRes); });
		//BR_TRANS_MESSAGE( Message::GameInstanceManager::CreateGameRes,			{ return OnCreateGame(pRes); });
	}

	// Start Transaction
	Result GamePartyTrans::StartTransaction()
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

