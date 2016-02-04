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
#include "Protocol/Message/LoginMsgClass.h"
#include "Protocol/Policy/LoginIPolicy.h"


#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServiceEntity/Login/LoginServiceTrans.h"
#include "ServerSystem/ServiceEntity/Login/LoginServiceEntity.h"

BR_MEMORYPOOL_IMPLEMENT(Svr::LoginPartyTrans);



namespace BR {
namespace Svr {



	LoginPartyTrans::LoginPartyTrans(UINT startMemberCount, UINT targetMemberCount)
		: TransactionT( TransactionID() )
	{
		//SetPrintTrace(false);

		//BR_TRANS_MESSAGE( TimerResult, { return OnTimer(pRes); });
		//BR_TRANS_MESSAGE( Message::PartyLoginQueue::ReserveItemRes,			{ return OnReserveItem(pRes); });
		//BR_TRANS_MESSAGE( Message::PartyLoginQueue::CancelReservationRes,	{ return OnCancelReservation(pRes); });
		//BR_TRANS_MESSAGE( Message::PartyLoginQueue::DequeueItemRes,			{ return OnDequeueItem(pRes); });
		//BR_TRANS_MESSAGE( Message::GameInstanceManager::CreateGameRes,			{ return OnCreateGame(pRes); });
	}

	// Start Transaction
	HRESULT LoginPartyTrans::StartTransaction()
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

