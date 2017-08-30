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
#include "Common/BrRandom.h"

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

#include "DB/RankingDB.h"


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
		int32_t currentRanking;
		RankingServiceEntity::RankingKey myRankingKey;
		bool added = false;

		auto rankCount = (int)GetCount();

		svrChk(super::StartTransaction());

		rankCount = std::min((int)Const::MAX_RANKING_LIST, rankCount);


		svrChk(GetMyOwner()->UpdatePlayerScore(GetPlayerInfo(), GetRankingScore(), m_PlayerRanking));

		// we are not going to wait db result
		svrChk(GetServerComponent<DB::RankingDB>()->UpdateRankingScoreCmd(TransactionID(), GetPlayerInfo().PlayerID, GetPlayerInfo().FBUID, GetPlayerInfo().NickName, GetPlayerInfo().Level, GetRankingScore()));


		rankingBase = m_PlayerRanking - (m_PlayerRanking % rankCount);
		m_RankingList.Clear();
		svrChk(GetMyOwner()->GetRankingList(rankingBase, rankCount, m_RankingList));

		if (m_RankingList.GetSize() == 0)
			goto Proc_End;

		for (unsigned iRank = 0; iRank < m_RankingList.GetSize(); iRank++)
		{
			auto& rankInfo = m_RankingList[iRank];
			if (rankInfo.PlayerID == GetPlayerInfo().PlayerID)
			{
				// It's old information remove it
				m_RankingList.RemoveAt((int)iRank);
				break;
			}
		}
		// Remove one if the player isn't in the list
		if ((int)m_RankingList.GetSize() >= rankCount)
		{
			m_RankingList.RemoveAt((int)m_RankingList.GetSize() - 1);
		}


		// Insert player where it need to be
		added = false;

		if (m_RankingList.GetSize() > 0)
			currentRanking = m_RankingList[0].Ranking;
		else
			currentRanking = 0;

		// the ranking key using
		assert(GetPlayerInfo().PlayerID < std::numeric_limits<uint32_t>::max());
		assert(GetRankingScore() < std::numeric_limits<uint32_t>::max());
		myRankingKey.PlayerID = static_cast<uint32_t>(GetPlayerInfo().PlayerID);
		myRankingKey.Score = static_cast<decltype(myRankingKey.Score)>(GetRankingScore());

		for (unsigned iRank = 0; iRank < m_RankingList.GetSize(); iRank++, currentRanking++)
		{
			RankingServiceEntity::RankingKey rankingKey;
			auto& rankInfo = m_RankingList[iRank];
			rankInfo.Ranking = currentRanking;

			rankingKey.PlayerID = static_cast<uint32_t>(rankInfo.PlayerID);
			rankingKey.Score = static_cast<decltype(rankingKey.Score)>(rankInfo.GetLongScore());

			if (added || rankingKey.RankingKeyValue > myRankingKey.RankingKeyValue)
				continue;

			added = true;
			m_RankingList.Insert(iRank, TotalRankingPlayerInformation(0, currentRanking, GetPlayerInfo().PlayerID, GetPlayerInfo().FBUID, GetPlayerInfo().NickName, GetPlayerInfo().Level, (int32_t)GetRankingScore(), (int32_t)(GetRankingScore() >> 32)));
		}

		if (!added)
		{
			m_RankingList.Add(TotalRankingPlayerInformation(0, currentRanking, GetPlayerInfo().PlayerID, GetPlayerInfo().FBUID, GetPlayerInfo().NickName, GetPlayerInfo().Level, (int32_t)GetRankingScore(), (int32_t)(GetRankingScore() >> 32)));
		}


	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


}// namespace Svr 
}// namespace BR 

