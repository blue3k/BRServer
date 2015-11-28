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
#include "Common/HRESCommon.h"
#include "Common/HRESGame.h"
#include "Common/HRESLogin.h"
#include "Common/MemoryPool.h"
#include "Common/BrBaseTypes.h"
#include "Common/GameConst.h"

#include "Protocol/Message/GameInstanceManagerMsgClass.h"
#include "Protocol/Message/RankingMsgClass.h"
#include "Protocol/Policy/RankingIPolicy.h"


#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServiceEntity/RankingServiceTrans.h"
#include "ServerSystem/ServiceEntity/RankingServiceEntity.h"
#include "ServerSystem/ServiceEntity/GameInstanceManagerServiceEntity.h"

#include "ServerSystem/ServerService/GameInstanceManagerService.h"


BR_MEMORYPOOL_IMPLEMENT(Svr::RankingPartyTrans);



namespace BR {
namespace Svr {



	RankingPartyTrans::RankingPartyTrans(UINT startMemberCount, UINT targetMemberCount)
		: TransactionT( TransactionID() )
		,m_QueryMemberCount(startMemberCount)
		,m_TargetMemberCount(targetMemberCount)
	{
		Assert(m_QueryMemberCount);
		Assert(m_TargetMemberCount);

		SetPrintTrace(false);

		//BR_TRANS_MESSAGE( TimerResult, { return OnTimer(pRes); });
		//BR_TRANS_MESSAGE( Message::PartyRankingQueue::ReserveItemRes,			{ return OnReserveItem(pRes); });
		//BR_TRANS_MESSAGE( Message::PartyRankingQueue::CancelReservationRes,	{ return OnCancelReservation(pRes); });
		//BR_TRANS_MESSAGE( Message::PartyRankingQueue::DequeueItemRes,			{ return OnDequeueItem(pRes); });
		//BR_TRANS_MESSAGE( Message::GameInstanceManager::CreateGameRes,			{ return OnCreateGame(pRes); });
	}

	// Start Transaction
	HRESULT RankingPartyTrans::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( super::StartTransaction() );

		// Only false when match is succeeded
		//GetMyOwner()->SetLastRankingFailed(true);

		//if( m_TargetMemberCount <= 0 || m_TargetMemberCount > GameConst::MAX_GAMEPLAYER )
		//	svrErr(E_GAME_INAVLID_PLAYER_COUNT);

		//if( m_QueryMemberCount <= 0 || m_QueryMemberCount > GameConst::MAX_GAMEPLAYER || m_QueryMemberCount >= m_TargetMemberCount )
		//	svrErr(E_GAME_INAVLID_PLAYER_COUNT);

		//m_Step = Step::Grabbing;
		//SetCurrentGrabbing(0);
		//SetGrabTryCount(0);
		//SetProcessingIndex(0);

		//SetCurrentMemberCount(0);
		//m_ReservedMember.Clear();

		////hr = ReserveItem(GetQueryMemberCount());
		//if( hr == E_SVR_NOITEM_INQUEUE )
		//{
		//	hr = S_OK;
		//	SetTimer(1000);
		//}

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

