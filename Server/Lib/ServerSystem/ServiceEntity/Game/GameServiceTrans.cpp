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
#include "Common/ResultCode/BRResultCodeCommon.h"
#include "Common/ResultCode/BRResultCodeGame.h"
#include "Common/MemoryPool.h"
#include "Common/BrBaseTypes.h"
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

BR_MEMORYPOOL_IMPLEMENT(Svr::GamePartyTrans);



namespace BR {
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
	HRESULT GamePartyTrans::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk( super::StartTransaction() );


	Proc_End:

		if( FAILED(hr) )
		{
			// We need to retry until it's successed
			CloseTransaction(hr);
		}

		return hr;
	}
	

};// namespace Svr 
};// namespace BR 

