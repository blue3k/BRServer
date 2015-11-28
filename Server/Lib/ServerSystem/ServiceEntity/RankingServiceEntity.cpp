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
#include "Common/Memory.h"
#include "Common/GameConst.h"
#include "Net/NetDef.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServicebase.h"
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
		:ReplicaClusterServiceEntity(clusterID, initialMembership)
	{
	}

	RankingServiceEntity::~RankingServiceEntity()
	{
	}

	HRESULT RankingServiceEntity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_OK;

		svrChk(ReplicaClusterServiceEntity::InitializeEntity(newEntityID) );

		m_CurrentProcessingNumberofMember = 0;

		m_LastRankingFailed = false;

		//m_RankingCheckTimer.SetTimer(TIME_START_MATCHING);

	Proc_End:

		return hr;
	}

	// clear transaction
	HRESULT RankingServiceEntity::ClearEntity()
	{
		HRESULT hr = S_OK;

		svrChk(ReplicaClusterServiceEntity::ClearEntity() );

	Proc_End:

		return hr;
	}

	HRESULT RankingServiceEntity::TickUpdate(Svr::TimerAction *pAction)
	{
		HRESULT hr = S_OK;

		svrChk(ReplicaClusterServiceEntity::TickUpdate(pAction) );

		// check below only if we are working
		if( GetEntityState() != EntityState::WORKING )
			goto Proc_End;

		if( BrServer::GetInstance()->GetServerState() != ServerState::RUNNING )
			goto Proc_End;

		//// Make this task keep working
		//UINT numTransactions = (UINT)GetTransactionCount() + (UINT)GetActiveTransactionCount();
		//if( numTransactions == 0 )
		//{
		//	if( !m_RankingCheckTimer.IsTimerWorking() && m_LastRankingFailed )
		//	{
		//		// set delay on failure
		//		m_RankingCheckTimer.SetTimer(TIME_REMATCHING_ONFAIL);
		//	}
		//	else if( !m_LastRankingFailed || m_RankingCheckTimer.CheckTimer() )
		//	{
		//		m_RankingCheckTimer.ClearTimer();

		//		m_CurrentProcessingNumberofMember++;
		//		if( m_CurrentProcessingNumberofMember >= m_RankingMemberCount ) m_CurrentProcessingNumberofMember = 1;

		//		Transaction *pTrans = new RankingPartyTrans( m_CurrentProcessingNumberofMember, m_RankingMemberCount );
		//		svrMem(pTrans);
		//		svrChk( PendingTransaction( pTrans ) );
		//	}
		//}

	Proc_End:

		return hr;
	}



	//////////////////////////////////////////////////////////////////////////
	//
	//	Ranking operations
	//





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



