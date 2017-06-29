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
#include "Protocol/Message/RankingServerMsgClass.h"
#include "Protocol/Policy/RankingServerIPolicy.h"


#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServiceEntity/RankingServiceTrans.h"
#include "ServerSystem/ServiceEntity/RankingServiceEntity.h"
#include "ServerSystem/ServiceEntity/Game/GameInstanceManagerServiceEntity.h"

#include "ServerSystem/ServerService/GameInstanceManagerService.h"


BR_MEMORYPOOL_IMPLEMENT(Svr::RankingServerAddPlayerTrans);
BR_MEMORYPOOL_IMPLEMENT(Svr::RankingServerUpdatePlayerScoreTrans);



namespace BR {
namespace Svr {



	RankingServerAddPlayerTrans::RankingServerAddPlayerTrans(Message::MessageData* &pIMsg)
		: ServerEntityMessageTransaction(pIMsg)
	{
	}

	// Start Transaction
	Result RankingServerAddPlayerTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );



	Proc_End:

		CloseTransaction(hr);

		return hr;
	}
	



	RankingServerUpdatePlayerScoreTrans::RankingServerUpdatePlayerScoreTrans(Message::MessageData* &pIMsg)
		: super(pIMsg)
	{
	}

	// Start Transaction
	Result RankingServerUpdatePlayerScoreTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		int64_t rankingBase = 0;

		svrChk(super::StartTransaction());


		svrChk(GetMyOwner()->UpdatePlayerScore(GetPlayerInfo(), GetRankingScore(), m_PlayerRanking));

		rankingBase = m_PlayerRanking - (m_PlayerRanking % GetCount());
		m_RankingList.Clear();
		svrChk(GetMyOwner()->GetRankingList(rankingBase, GetCount(), m_RankingList));

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


};// namespace Svr 
};// namespace BR 

