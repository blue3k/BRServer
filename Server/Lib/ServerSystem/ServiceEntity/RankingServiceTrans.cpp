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
#include "Memory/MemoryPool.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"
#include "Util/SFRandom.h"
#include "IO/SFFile.h"

#include "Protocol/Message/GameInstanceManagerMsgClass.h"
#include "Protocol/Message/RankingServerMsgClass.h"
#include "Protocol/Policy/RankingServerNetPolicy.h"


#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "Server/BrServer.h"
#include "ServerEntity/ServerEntityManager.h"
#include "Entity/EntityManager.h"
#include "ServiceEntity/RankingServiceTrans.h"
#include "ServiceEntity/RankingServiceEntity.h"
#include "ServiceEntity/Game/GameInstanceManagerServiceEntity.h"

#include "Protocol/ServerService/GameInstanceManagerService.h"

#include "DB/RankingDB.h"

SF_MEMORYPOOL_IMPLEMENT(SF::Svr::RankingServerAddPlayerTrans);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::RankingServerUpdatePlayerScoreTrans);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::RankingServerDebugPrintALLRankingTrans);



namespace SF {
namespace Svr {



	RankingServerAddPlayerTrans::RankingServerAddPlayerTrans(IMemoryManager& memMgr, MessageDataPtr &pIMsg)
		: ServerEntityMessageTransaction(memMgr, pIMsg)
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
	



	RankingServerUpdatePlayerScoreTrans::RankingServerUpdatePlayerScoreTrans(IMemoryManager& memMgr, MessageDataPtr &pIMsg)
		: super(memMgr, pIMsg)
		, m_RankingList(memMgr)
	{
	}

	// Start Transaction
	Result RankingServerUpdatePlayerScoreTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		int64_t rankingBase = 0;
		int64_t currentRanking = 0, expectedRanking = 0;
		uint64_t latestScore;
		bool added = false, removedInTheMiddle = false;

		auto rankCount = (int)GetCount();

		svrChk(super::StartTransaction());

		rankCount = std::min((int)Const::MAX_RANKING_LIST, rankCount);


		svrChk(GetMyOwner()->UpdatePlayerScore(GetPlayerInfo(), GetRankingScore(), m_PlayerRanking));

		// we are not going to wait db result
		svrChk(GetServerComponent<DB::RankingDB>()->UpdateRankingScoreCmd(TransactionID(), GetPlayerInfo().PlayerID, GetPlayerInfo().FBUID, GetPlayerInfo().NickName, GetPlayerInfo().Level, GetRankingScore()));

		// TODO: commit changes for project. remove it later
		// -------------------------------
		GetMyOwner()->CommitChanges();

		// -------------------------------
		//GetMyOwner()->GetBaseRanking(GetRankingScore(), expectedRanking);


		rankingBase = std::max((m_PlayerRanking - (rankCount >> 1)), (int64_t)0);
		m_RankingList.Clear();
		svrChk(GetMyOwner()->GetRankingList(rankingBase, rankCount, m_RankingList));

		if (m_RankingList.size() == 0)
			goto Proc_End;



		if (m_RankingList.size() > 0)
		{
			GetMyOwner()->GetBaseRanking(m_RankingList[0].GetScore(), expectedRanking);
			currentRanking = m_RankingList[0].Ranking;
			latestScore = m_RankingList[0].GetScore();
		}
		else
		{
			expectedRanking = 0;
			currentRanking = 0;
			latestScore = 0;
		}

		for (unsigned iRank = 0; iRank < m_RankingList.size(); iRank++, currentRanking++)
		{
			auto& rankInfo = m_RankingList[iRank];

			// Define ranking 
			if (latestScore != rankInfo.GetScore())
			{
				latestScore = rankInfo.GetScore();
				expectedRanking = currentRanking;
			}
			rankInfo.Ranking = static_cast<decltype(rankInfo.Ranking)>(expectedRanking);
		}

		//GetMyOwner()->PrintAllRanking("RankingTest");

		//// Remove myself if in the expected list
		//for (unsigned iRank = 0; iRank < m_RankingList.size(); iRank++)
		//{
		//	auto& rankInfo = m_RankingList[iRank];
		//	if (rankInfo.PlayerID == GetPlayerInfo().PlayerID)
		//	{
		//		// It's old information remove it
		//		removedInTheMiddle = true;
		//		m_RankingList.RemoveAt((int)iRank);
		//		break;
		//	}
		//}

		//// Remove one if the player isn't in the list
		//if ((int)m_RankingList.size() >= rankCount)
		//{
		//	m_RankingList.RemoveAt((int)m_RankingList.size() - 1);
		//}


		//// Insert player where it need to be
		//added = false;

		//if (m_RankingList.size() > 0)
		//{
		//	GetMyOwner()->GetBaseRanking(m_RankingList[0].GetScore(), expectedRanking);
		//	currentRanking = m_RankingList[0].Ranking;
		//	latestScore = m_RankingList[0].GetScore();
		//}
		//else
		//{
		//	expectedRanking = 0;
		//	currentRanking = 0;
		//	latestScore = 0;
		//}


		//// the ranking key using
		//assert(GetPlayerInfo().PlayerID < std::numeric_limits<uint32_t>::max());
		//assert(GetRankingScore() < std::numeric_limits<uint32_t>::max());
		//myRankingKey.PlayerID = static_cast<uint32_t>(GetPlayerInfo().PlayerID);
		//myRankingKey.Score = static_cast<decltype(myRankingKey.Score)>(GetRankingScore());

		//for (unsigned iRank = 0; iRank < m_RankingList.size(); iRank++, currentRanking++)
		//{
		//	RankingServiceEntity::RankingKey rankingKey;
		//	auto& rankInfo = m_RankingList[iRank];

		//	rankingKey.PlayerID = static_cast<uint32_t>(rankInfo.PlayerID);
		//	rankingKey.Score = static_cast<decltype(rankingKey.Score)>(rankInfo.GetScore());

		//	// Define ranking 
		//	if (latestScore != rankInfo.GetScore())
		//	{
		//		latestScore = rankInfo.GetScore();
		//		GetMyOwner()->GetBaseRanking(latestScore, expectedRanking);
		//		//expectedRanking = currentRanking;
		//	}
		//	rankInfo.Ranking = static_cast<decltype(rankInfo.Ranking)>(expectedRanking);

		//	if (added || rankingKey.RankingKeyValue > myRankingKey.RankingKeyValue)
		//		continue;

		//	// If i am going to be the first guy in the list, My ranking need to be calculated separately
		//	latestScore = GetRankingScore();
		//	GetMyOwner()->GetBaseRanking(latestScore, expectedRanking);

		//	added = true;
		//	m_RankingList.Insert(iRank, TotalRankingPlayerInformation(0, expectedRanking, GetPlayerInfo().PlayerID, GetPlayerInfo().FBUID, GetPlayerInfo().NickName, GetPlayerInfo().Level, (int32_t)GetRankingScore(), (int32_t)(GetRankingScore() >> 32)));
		//}

		//if (!added)
		//{
		//	if (latestScore != GetRankingScore())
		//	{
		//		expectedRanking = currentRanking;
		//	}

		//	m_RankingList.Add(TotalRankingPlayerInformation(0, expectedRanking, GetPlayerInfo().PlayerID, GetPlayerInfo().FBUID, GetPlayerInfo().NickName, GetPlayerInfo().Level, (int32_t)GetRankingScore(), (int32_t)(GetRankingScore() >> 32)));
		//}


	Proc_End:

		CloseTransaction(hr);

		return hr;
	}




	RankingServerDebugPrintALLRankingTrans::RankingServerDebugPrintALLRankingTrans(IMemoryManager& memMgr, MessageDataPtr &pIMsg)
		: super(memMgr, pIMsg)
		, m_RankingList(memMgr)
	{
	}

	// Start Transaction
	Result RankingServerDebugPrintALLRankingTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		
		svrChk(super::StartTransaction());

		GetMyOwner()->PrintAllRanking(GetFileName());

		
	Proc_End:

		CloseTransaction(hr);

		return hr;
	}

}// namespace Svr 
}// namespace SF 

