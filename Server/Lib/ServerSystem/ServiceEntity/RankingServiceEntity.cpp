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
#include "Common/StrUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "Common/BrMemory.h"
#include "Common/GameConst.h"
#include "Common/BrRandom.h"
#include "Net/NetDef.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/ServiceEntity/RankingServiceEntity.h"
#include "ServerSystem/ServiceEntity/RankingServiceTrans.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/SvrConst.h"

#include "DB/RankingDB.h"


namespace BR {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	RankingServiceEntity::RankingServiceEntity(ClusterID clusterID, ClusterMembership initialMembership)
		:FreeReplicaClusterServiceEntity(clusterID, initialMembership)
	{
		BR_ENTITY_MESSAGE(Message::RankingServer::AddPlayerCmd) { svrMemReturn(pNewTrans = new RankingServerAddPlayerTrans(pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::RankingServer::RemovePlayerCmd) { svrMemReturn(pNewTrans = new RankingServerRemovePlayerTrans(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::RankingServer::UpdatePlayerScoreCmd) { svrMemReturn(pNewTrans = new RankingServerUpdatePlayerScoreTrans(pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::RankingServer::GetPlayerRankingCmd) { svrMemReturn(pNewTrans = new RankingServerGetPlayerRankingTrans(pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::RankingServer::GetRankingCmd) { svrMemReturn(pNewTrans = new RankingServerGetRankingTrans(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::RankingServer::DebugPrintALLRankingCmd) { svrMemReturn(pNewTrans = new RankingServerDebugPrintALLRankingTrans(pMsgData)); return ResultCode::SUCCESS; } );
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

		svrChk(FreeReplicaClusterServiceEntity::InitializeEntity(newEntityID) );

		svrChk(pServerInst->AddDBCluster<DB::RankingDB>(Config::GetConfig().RankingDB));

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

			svrTrace(TRC_INFO, "Ranking request: from {0}, to {1}, count{2}", baseIndex, baseIndex + requestSize, pResult->m_RowsetResult.size());
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
			pPlayerRankInformation = new TotalRankingPlayerInformation(0, 0, player.PlayerID, player.FBUID, player.NickName, player.Level, (int32_t)score, (int32_t)(score >> 32));
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

			rankingKey.Score = (uint32_t)score;
			svrChk(m_RankingMap.Insert(rankingKey.RankingKeyValue, pPlayerRankInformation, &playerRanking));
		}

	Proc_End:

		return hr;
	}

	typedef TotalRankingPlayerInformation* TotalRankingPlayerInformationPtr;
	Result RankingServiceEntity::GetRankingList(int64_t from, int64_t count, Array<TotalRankingPlayerInformation> &rankingList)
	{
		int32_t expectedRanking = (int32_t)from;
		m_RankingMap.ForeachOrder((int)from, (int)count, [&expectedRanking, &rankingList](const uint64_t& key, const TotalRankingPlayerInformationPtr &value)->bool
		{
			TotalRankingPlayerInformation newValue = *value;
			newValue.Ranking = expectedRanking++;
			rankingList.Add(newValue);
			return true;
		});

		return ResultCode::SUCCESS;
	}

	Result RankingServiceEntity::GetRankingListAll(Array<TotalRankingPlayerInformation> &rankingList)
	{
		int32_t expectedRanking = 0;
		CounterType itemCount = m_RankingMap.GetItemCount();

		return GetRankingList(0, itemCount, rankingList);
	}

	
	Result RankingServiceEntity::ForeachAll(const ForeEachFuntor& functor)
	{		
		int32_t expectedRanking = 0;
		int32_t itemCount = m_RankingMap.GetItemCount();

		m_RankingMap.ForeachOrder(0, (int)itemCount, [&expectedRanking, &functor ] (const uint64_t& key, const TotalRankingPlayerInformationPtr &value)->bool
		{
			const TotalRankingPlayerInformation newValue = *value;
			functor(expectedRanking++, &newValue);
			
			return true;
		});

		return ResultCode::SUCCESS;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	RankingWatcherServiceEntity class
	//


	RankingWatcherServiceEntity::RankingWatcherServiceEntity( ClusterID clusterID, UINT componentID )
		: ReplicaClusterServiceEntity(clusterID, ClusterMembership::StatusWatcher)
		, IServerComponent(componentID)
	{
	}

	RankingWatcherServiceEntity::~RankingWatcherServiceEntity()
	{
	}




}; // namespace Svr {
}; // namespace BR {



