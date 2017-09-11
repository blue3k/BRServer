////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Server service Component entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "String/StrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/Thread.h"
#include "Memory/SFMemory.h"
#include "GameConst.h"
#include "Util/SFRandom.h"
#include "Net/NetDef.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServerEntity/ServerEntity.h"
#include "ServiceEntity/RankingServiceEntity.h"
#include "ServiceEntity/RankingServiceTrans.h"
#include "SvrTrace.h"
#include "SvrConst.h"
#include "Service/ServerService.h"
#include "IO/SFFile.h"

#include "DB/RankingDB.h"


namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	RankingServiceEntity::RankingServiceEntity(ClusterID clusterID, ClusterMembership initialMembership)
		: FreeReplicaClusterServiceEntity(clusterID, initialMembership)
		, m_RankingMap(GetMemoryManager())
		, m_PlayerMap(GetMemoryManager())
	{
		BR_ENTITY_MESSAGE(Message::RankingServer::AddPlayerCmd)					{ svrMemReturn(pNewTrans = new(GetMemoryManager()) RankingServerAddPlayerTrans(GetMemoryManager(),pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::RankingServer::UpdatePlayerScoreCmd)			{ svrMemReturn(pNewTrans = new(GetMemoryManager()) RankingServerUpdatePlayerScoreTrans(GetMemoryManager(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::RankingServer::DebugPrintALLRankingCmd)		{ svrMemReturn(pNewTrans = new(GetMemoryManager()) RankingServerDebugPrintALLRankingTrans(GetMemoryManager(), pMsgData)); return ResultCode::SUCCESS; } );
	}

	RankingServiceEntity::~RankingServiceEntity()
	{
	}

	Result RankingServiceEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;
		auto pServerInst = BrServer::GetInstance();
		DB::RankingDB* pRankDB = nullptr;
		int64_t baseIndex = 0;
		auto requestSize = Const::RANKING_DB_MAX_REQUEST;
		auto maxRequestCount = (Const::RANKING_MAX_PLAYER * 2 - 1) / requestSize;

		auto rankingDBConfig = Service::ServerConfig->FindDBCluster("RankingDB");

		svrChk(FreeReplicaClusterServiceEntity::InitializeEntity(newEntityID) );

		svrChk(pServerInst->AddDBCluster<DB::RankingDB>(rankingDBConfig));

		m_CurrentProcessingNumberofMember = 0;


		pRankDB = GetServerComponent<DB::RankingDB>();
		svrChkPtr(pRankDB);
		
		baseIndex = 0;
		for (int64_t iRequest = 0; iRequest < maxRequestCount; iRequest++, baseIndex += requestSize)
		{
			auto pResult = pRankDB->GetRankers((int32_t)baseIndex, (int32_t)requestSize);
			svrChkPtr(pResult);

			// Nothing to query anymore
			if (pResult->m_RowsetResult.size() == 0)
				break;

			svrTrace(Info, "Ranking request: from {0}, to {1}, count{2}", baseIndex, baseIndex + requestSize, pResult->m_RowsetResult.size());
			for (auto& itRowSet : pResult->m_RowsetResult)
			{
				int64_t playerRanking;
				PlayerInformation playerInfo(itRowSet.PlayerID, itRowSet.FBUID, itRowSet.NickName, itRowSet.Level, false, 0);
				svrChk(UpdatePlayerScore(playerInfo, itRowSet.Score, playerRanking));
			}
		}

		// Apply loaded changes
		m_RankingMap.CommitChanges();

		m_RankingCheckTimer.SetTimer(DurationMS(Svr::Const::TIME_INTERVAL_RANKING_UPDATE));

	Proc_End:

		return hr;
	}

	// clear transaction
	Result RankingServiceEntity::ClearEntity()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(FreeReplicaClusterServiceEntity::ClearEntity() );

	Proc_End:

		return hr;
	}

	Result RankingServiceEntity::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(FreeReplicaClusterServiceEntity::TickUpdate(pAction) );

		// check below only if we are working
		if( GetEntityState() != EntityState::WORKING )
			goto Proc_End;

		if( BrServer::GetInstance()->GetServerState() != ServerState::RUNNING )
			goto Proc_End;


		if (m_RankingCheckTimer.CheckTimer())
		{
			m_RankingMap.CommitChanges();
			m_RankingCheckTimer.SetTimer(DurationMS(Svr::Const::TIME_INTERVAL_RANKING_UPDATE));
		}

	Proc_End:

		return hr;
	}



	//////////////////////////////////////////////////////////////////////////
	//
	//	Ranking operations
	//

	Result RankingServiceEntity::UpdatePlayerScore(const PlayerInformation& player, int64_t score, int64_t& playerRanking)
	{
		Result hr = ResultCode::SUCCESS;
		TotalRankingPlayerInformation *pPlayerRankInformation = nullptr;
		TotalRankingPlayerInformation *pRemoved = nullptr;
		RankingKey rankingKey;

		playerRanking = -1;
		
		rankingKey.PlayerID = (int32_t)player.PlayerID;

		hr = m_PlayerMap.Find(player.PlayerID, pPlayerRankInformation);
		if (!hr)
		{
			hr = ResultCode::SUCCESS;

			// not yet ranked
			// TODO: we need to use more generalied player information description
			pPlayerRankInformation = new(GetMemoryManager()) TotalRankingPlayerInformation(0, 0, player.PlayerID, player.FBUID, player.NickName, player.Level, (int32_t)score, (int32_t)(score >> 32));
			svrChk(m_PlayerMap.Insert(player.PlayerID, pPlayerRankInformation));
			m_PlayerMap.CommitChanges();

			// TODO: only use lower 32bit value
			rankingKey.Score = (uint32_t)score;
			svrChk(m_RankingMap.Insert(rankingKey.RankingKeyValue, pPlayerRankInformation, &playerRanking));
		}
		else
		{
			// Use original score for search key
			rankingKey.Score = pPlayerRankInformation->ScoreLow; // TODO
																		  
			// we already has ranking information
			pPlayerRankInformation->SetLongScore(score);

			hr = m_RankingMap.Remove(rankingKey.RankingKeyValue, pRemoved);
			Assert(pRemoved == pPlayerRankInformation);

			pPlayerRankInformation->FBUID = player.FBUID;
			StrUtil::StringCpy(pPlayerRankInformation->NickName, player.NickName);

			rankingKey.Score = (uint32_t)score;
			svrChk(m_RankingMap.Insert(rankingKey.RankingKeyValue, pPlayerRankInformation, &playerRanking));
		}

	Proc_End:

		return hr;
	}

	Result RankingServiceEntity::GetBaseRanking(int64_t score, int64_t& playerRanking)
	{
		RankingKey rankingKey;
		rankingKey.Score = static_cast<decltype(rankingKey.Score)>(score);
		rankingKey.PlayerID = std::numeric_limits<decltype(rankingKey.PlayerID)>::max();
		TotalRankingPlayerInformation *pInfo = nullptr;

		Result result =  m_RankingMap.FindBiggest(rankingKey.RankingKeyValue, pInfo, &playerRanking);

		// expected ranking will be 1 bigger
		if(pInfo != nullptr && pInfo->GetScore() > (uint64_t)score) playerRanking += 1;

		return result;
	}

	typedef TotalRankingPlayerInformation* TotalRankingPlayerInformationPtr;
	Result RankingServiceEntity::GetRankingList(int64_t from, int64_t count, Array<TotalRankingPlayerInformation> &rankingList)
	{
		int32_t expectedRanking = (int32_t)from;
		m_RankingMap.ForeachOrder((int)from, (int)count, [&expectedRanking, &rankingList](const uint64_t& key, const TotalRankingPlayerInformationPtr &value)->bool
		{
			TotalRankingPlayerInformation newValue = *value;
			newValue.Ranking = expectedRanking++;
			rankingList.Append(newValue);
			return true;
		});

		return ResultCode::SUCCESS;
	}

	Result RankingServiceEntity::GetRankingListAll(Array<TotalRankingPlayerInformation> &rankingList)
	{
		int32_t expectedRanking = 0;
		int32_t itemCount = static_cast<int32_t>(m_RankingMap.GetItemCount());

		return GetRankingList(0, itemCount, rankingList);
	}

	
	Result RankingServiceEntity::ForeachAll(const ForeEachFuntor& functor)
	{		
		int32_t expectedRanking = 0;
		int32_t itemCount = static_cast<int32_t>(m_RankingMap.GetItemCount());

		m_RankingMap.ForeachOrder(0, (int)itemCount, [&expectedRanking, &functor ] (const uint64_t& key, const TotalRankingPlayerInformationPtr &value)->bool
		{
			const TotalRankingPlayerInformation newValue = *value;
			functor(expectedRanking++, &newValue);
			
			return true;
		});

		return ResultCode::SUCCESS;
	}

	void RankingServiceEntity::PrintAllRanking(const char* fileName)
	{
		Result hr = ResultCode::SUCCESS;

		// parameter from client
		File fileStream;
		char szBuff[1024];
		int64_t currentRanking = 0, expectedRanking = 0;
		int64_t currentScore = 0;


		CommitChanges();

		TimeStampSec nowTime = Util::Time.GetRawUTCSec();

		time_t time = nowTime.time_since_epoch().count() + Util::Time.GetUTCSecOffset().count();
		struct tm nowTimeTM = *gmtime(&time);

		char strFileName[MAX_PATH];
		snprintf(strFileName, MAX_PATH, "%s..\\log\\%s[%d_%04d_%02d_%02d_%02d_%02d]_%s_log.csv",
			Util::GetModulePath(), Util::GetServiceName(), nowTimeTM.tm_year + 1900, nowTimeTM.tm_mon + 1, nowTimeTM.tm_mday, nowTimeTM.tm_hour,
			nowTimeTM.tm_min, nowTimeTM.tm_sec, fileName);

		fileStream.Open(strFileName, File::OpenMode::Append, File::SharingMode::Exclusive);

		//uint32_t		RankingID;
		//uint32_t		Ranking;
		//AccountID		PlayerID;
		//FacebookUID   FBUID;
		//char			NickName[MAX_NAME];
		//uint32_t		Level;
		//uint32_t		ScoreLow;	// Win
		//uint32_t		ScoreHigh;	// Lose

		StrUtil::Format(szBuff, "Order, Ranking, Score, PlayerID, FacebookUID, NickName, Level\n");
		uint32_t dwStrLen = (uint32_t)strlen(szBuff);

		size_t szWritlen;
		// write header..
		fileStream.Write((byte*)szBuff, dwStrLen, szWritlen);

		ForeachAll([&](int32_t Ranking, const TotalRankingPlayerInformation* rankInfo)
		{
			if (currentScore != rankInfo->GetScore())
			{
				expectedRanking = Ranking;
				currentScore = rankInfo->GetScore();
			}

			int64_t calculatedRanking;
			GetBaseRanking(rankInfo->GetScore(), calculatedRanking);
			assert(expectedRanking == calculatedRanking);

			//	TotalRankingPlayerInformation& rankInfo = m_RankingList[i];				
			StrUtil::Format(szBuff, "{0}, {1}, {2}, {3}, {4}, {5}, {6}\n",
				Ranking, expectedRanking, rankInfo->GetScore(),
				rankInfo->PlayerID, rankInfo->FBUID, rankInfo->NickName, rankInfo->Level);

			dwStrLen = (uint32_t)strlen(szBuff);

			fileStream.Write((byte*)szBuff, dwStrLen, szWritlen);
		});

	Proc_End:

		fileStream.Close();

		return;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	RankingWatcherServiceEntity class
	//


	RankingWatcherServiceEntity::RankingWatcherServiceEntity( ClusterID clusterID, uint componentID )
		: ReplicaClusterServiceEntity(clusterID, ClusterMembership::StatusWatcher)
		, IServerComponent(componentID)
	{
	}

	RankingWatcherServiceEntity::~RankingWatcherServiceEntity()
	{
	}




}; // namespace Svr {
}; // namespace SF {



