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
	}

	RankingServiceEntity::~RankingServiceEntity()
	{
	}

	Result RankingServiceEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(FreeReplicaClusterServiceEntity::InitializeEntity(newEntityID) );

		m_CurrentProcessingNumberofMember = 0;

		//m_LastRankingFailed = false;

		m_RankingCheckTimer.SetTimer(DurationMS(Svr::Const::TIME_INTERVAL_RANKING_UPDATE));




		for (int iPlayer = 1; iPlayer < 50; iPlayer++)
		{
			char temp[128];
			StrUtil::Format(temp, "TestPlayer{0}", iPlayer);
			PlayerInformation playerInfo(iPlayer, iPlayer, temp, 0, 0, Util::Time.GetRawUTCSec().time_since_epoch().count());
			int64_t playerRanking;
			svrChk(UpdatePlayerScore(playerInfo, Util::Random.Rand() % 50000, playerRanking));
		}


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
			rankingKey.Score = /*(uint64_t)*/pPlayerRankInformation->Win; // | (uint64_t)pPlayerRankInformation->Lose << 32;
																		  
			// we already has ranking information
			pPlayerRankInformation->Win = (int32_t)score;
			pPlayerRankInformation->Lose = (int32_t)(score >> 32);

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



