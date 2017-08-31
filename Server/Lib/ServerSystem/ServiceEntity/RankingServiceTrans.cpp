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
#include "Common/File/BRFile.h"

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
BR_MEMORYPOOL_IMPLEMENT(Svr::RankingServerDebugPrintALLRankingTrans);



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




	RankingServerDebugPrintALLRankingTrans::RankingServerDebugPrintALLRankingTrans(Message::MessageData* &pIMsg)
		: super(pIMsg)
	{
	}

	// Start Transaction
	Result RankingServerDebugPrintALLRankingTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		// parameter from client
		auto fileName = GetFileName();
		IO::File fileStream;
		char szBuff[1024];

		svrChk(super::StartTransaction());

		// TODO: fill it
		m_RankingList.Clear();

		svrChk(GetMyOwner()->GetRankingList(0, 100, m_RankingList));

		TimeStampSec nowTime = Util::Time.GetRawUTCSec();

		time_t time = nowTime.time_since_epoch().count() + Util::Time.GetUTCSecOffset().count();
		struct tm nowTimeTM = *gmtime(&time);

		char strFileName[MAX_PATH];		
		snprintf(strFileName, MAX_PATH, "%s..\\log\\%s[%d_%04d_%02d_%02d]_%s_log.txt",
			Util::GetModulePathA(), Util::GetServiceNameA(), nowTimeTM.tm_year + 1900, nowTimeTM.tm_mon + 1, nowTimeTM.tm_mday, nowTimeTM.tm_hour, fileName);

		fileStream.Open(strFileName, IO::File::OpenMode::Append, IO::File::SharingMode::Exclusive);

		//uint32_t		RankingID;
		//uint32_t		Ranking;
		//AccountID		PlayerID;
		//FacebookUID   FBUID;
		//char			NickName[MAX_NAME];
		//uint32_t		Level;
		//uint32_t		ScoreLow;	// Win
		//uint32_t		ScoreHigh;	// Lose

		snprintf(szBuff, MAX_PATH, "Ranking, Score, PlayerID, FaceboolUID, NickName, Level\n");
		DWORD dwStrLen = (DWORD)strlen(szBuff);
		size_t szWritlen;

		// write header..
		fileStream.Write((byte*) szBuff, dwStrLen, szWritlen);

		for (unsigned i = 0; i < m_RankingList.GetSize(); i++)
		{
			TotalRankingPlayerInformation& rankInfo = m_RankingList[i];
			snprintf(szBuff, MAX_PATH, "%d, %d, %d, %d, %s, %d\n",
				rankInfo.Ranking, rankInfo.GetLongScore(), rankInfo.PlayerID, rankInfo.FBUID, rankInfo.NickName, rankInfo.Level);
			dwStrLen = (DWORD)strlen(szBuff);
			fileStream.Write((byte*)szBuff, dwStrLen, szWritlen);
		}

	Proc_End:

		fileStream.Close();
		CloseTransaction(hr);

		return hr;
	}

}// namespace Svr 
}// namespace BR 

