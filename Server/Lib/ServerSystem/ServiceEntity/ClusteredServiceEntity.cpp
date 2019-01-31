////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Server service Component entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "String/SFStrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/SFThread.h"
#include "Net/SFNetDef.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServerEntity/ServerEntity.h"
#include "ServerEntity/ServerEntityManager.h"
#include "ServiceEntity/ClusteredServiceEntity.h"
#include "ServiceEntity/ClusterServiceTrans.h"
#include "Protocol/ServerService/ClusterServerService.h"
#include "SvrTrace.h"
#include "Task/ServerTaskEvent.h"

#include "Protocol/Policy/ClusterServerNetPolicy.h"


SF_MEMORYPOOL_IMPLEMENT(SF::Svr::ClusteredServiceEntity::ServiceTableItem);

namespace SF {
namespace Svr {


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ClusteredServiceEntity class
	//


	ClusteredServiceEntity::ClusteredServiceEntity(ClusterType clusterType, GameID gameID, ClusterID clusterID, ClusterMembership initialMembership, ServerEntity* pServerEntity )
		: m_MasterUID(0)
		, m_GameID(gameID)
		, m_ClusterID(clusterID)
		, m_ClusterType(clusterType )
		, m_ClusterMembership(initialMembership)
		, m_ServiceStatus(ServiceStatus::Offline)
		, m_Workload(0)
		, m_ServerEntity(pServerEntity)
	{
		// Setup connection type by cluster type
		switch (clusterType)
		{
		case ClusterType::Replication:
		case ClusterType::FreeReplication:
		case ClusterType::Ring:
			SetActivelyConnectRemote(true);
			break;
		default:
			SetActivelyConnectRemote(false);
			break;
		}


		//if (BrServer::GetInstance()->GetNetClass() == NetClass::Entity)
		//{
		//	BR_ENTITY_MESSAGE(Message::ClusterServer::GetLowestWorkloadClusterMemberCmd)	{ svrMemReturn(pNewTrans = new(GetHeap()) GetLowestWorkloadClusterMemberTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//	BR_ENTITY_MESSAGE(Message::ClusterServer::NewServerServiceJoinedC2SEvt)			{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterNewServerServiceJoinedC2SEvtEntityTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//}
		//else
		//{
		//	BR_ENTITY_MESSAGE(Message::ClusterServer::NewServerServiceJoinedC2SEvt)			{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterNewServerServiceJoinedC2SEvtTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//}
		//BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterMasterAssignedS2CEvt)				{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterMasterAssignedTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterMasterVoteC2SEvt)					{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterMasterVoteTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::ClusterServer::RequestDataSyncCmd)						{ svrMemReturn(pNewTrans = new(GetHeap()) RequestDataSyncTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterUpdateStatusC2SEvt)				{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterUpdateStatusTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterUpdateWorkloadC2SEvt)				{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterUpdateWorkloadTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );

	}

	ClusteredServiceEntity::~ClusteredServiceEntity()
	{
	}

	//// Check connectivity of two service
	//bool ClusteredServiceEntity::CheckConnectivity( NetClass netClass1, ClusterMembership membership1, NetClass netClass2, ClusterMembership membership2 )
	//{
	//	if( netClass1 == NetClass::Entity || netClass2 == NetClass::Entity
	//		|| membership1 != ClusterMembership::StatusWatcher || membership2 != ClusterMembership::StatusWatcher )
	//	{
	//		return true;
	//	}
	//	return false;
	//}
	//
	//// Check connectivity of two service
	//bool ClusteredServiceEntity::CheckConnectivity( NetClass netClass2, ClusterMembership membership2 )
	//{
	//	if( BrServer::GetInstance()->GetNetClass() == NetClass::Entity || netClass2 == NetClass::Entity
	//		|| GetClusterMembership() != ClusterMembership::StatusWatcher || membership2 != ClusterMembership::StatusWatcher )
	//	{
	//		return true;
	//	}
	//	return false;
	//}
	//

	////////////////////////////////////////////////////////////////////////////
	////
	////	Voting information
	////

	//void ClusteredServiceEntity::StartVoting()
	//{
	//	if( m_IsInVoting )
	//		return;

	//	m_IsInVoting = true;

	//	ForEach( [&](ServerServiceInformation *pService) 
	//	{
	//		pService->SetVotedCount(0);
	//	});
	//}

	//void ClusteredServiceEntity::EndVoting()
	//{
	//	if( !m_IsInVoting ) 
	//		return;

	//	m_IsInVoting = false;

	//	ForEach( [&](ServerServiceInformation *pService) 
	//	{
	//		pService->SetVotedCount(0);
	//	});
	//}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity 
	//

	// Initialize entity to proceed new connection
	Result ClusteredServiceEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;
		ServerServiceInformation* pMyService = nullptr;

		m_Initialized = false;

		svrChk(ServiceEntity::InitializeEntity( newEntityID ) );

		m_ServiceStatus = ServiceStatus::Online;

		// If not server entity is assigned use local loop back entity
		if( m_ServerEntity == nullptr )
			m_ServerEntity = GetLoopbackServerEntity();

	Proc_End:

		return hr;
	}

	Result ClusteredServiceEntity::StartInitializeTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		ClusterInitializationTrans *pInitTransaction = nullptr;
		TransactionPtr pTrans;

		// Push initialization transaction
		svrMem( pInitTransaction = new(GetHeap()) ClusterInitializationTrans(GetHeap()));
		svrChk( pInitTransaction->InitializeTransaction( this ) );
		pTrans = pInitTransaction;
		pInitTransaction = nullptr;

		svrChk(PendingTransaction(ThisThread::GetThreadID(), pTrans));

		pTrans = nullptr;

	Proc_End:

		Util::SafeDelete(pInitTransaction);

		return hr;
	}

	// clear transaction
	Result ClusteredServiceEntity::ClearEntity()
	{
		Result hr = ResultCode::SUCCESS;

		//auto itIDMap = m_ServiceEntityUIDMap.begin();
		//for(; itIDMap.IsValid(); ++itIDMap )
		//{
		//	ServiceTableItem *pService = *itIDMap;
		//	Util::SafeDelete( pService );
		//}
		//m_ServiceEntityUIDMap.clear();
		m_MyServiceInfo = nullptr;


		svrChk(ServiceEntity::ClearEntity() );

	Proc_End:

		return hr;
	}


	Result ClusteredServiceEntity::TickUpdate(TimerAction *pAction)
	{
		return ServiceEntity::TickUpdate(pAction);
	}

	Result ClusteredServiceEntity::ProcessTransaction(TransactionPtr &pTrans)
	{
		return ServiceEntity::ProcessTransaction(pTrans);
	}



	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	// Change workload
	Result ClusteredServiceEntity::SetWorkload( uint workload )
	{
		Result hr = ResultCode::SUCCESS;
		ServerServiceInformation *pServiceInfo = nullptr;
		ClusterServerService *pService = nullptr;

		if( m_Workload == workload )
			return ResultCode::SUCCESS;

		m_Workload = workload;

		Service::ClusterManager->UpdateWorkLoad(this);
		//svrChk( Service::ClusterManager->FindRandomService( pServiceInfo ) );
		//svrChkPtr( pService = pServiceInfo->GetService<ClusterServerService>() );
		//svrChk( pService->ClusterUpdateWorkloadC2SEvt( GetEntityUID(), 0, GetEntityUID(), GetClusterID(), m_Workload ) );

	Proc_End:


		return hr;
	}


	Result ClusteredServiceEntity::SetServiceStatus( ServiceStatus newStatus )
	{
		Result hr = ResultCode::SUCCESS;
		ServerServiceInformation *pServiceInfo = nullptr;
		ClusterServerService *pService = nullptr;

		m_ServiceStatus = newStatus;

		if( m_MasterUID.UID == 0 )
		{
			return ResultCode::SUCCESS_FALSE;
		}

		Service::ClusterManager->UpdateServiceStatus(this);
		//// Ask to the cluster manager master to update status
		//svrChk( Service::ClusterManager->FindRandomService( pServiceInfo ) );
		////svrChk( Service::ClusterManager->GetService<ClusterServerService>(pService) );
		//svrChkPtr( pService = pServiceInfo->GetService<ClusterServerService>() );
		//svrChk( pService->ClusterUpdateStatusC2SEvt( GetEntityUID(), 0, GetEntityUID(), GetClusterID(), GetServiceStatus() ) );

	Proc_End:


		return hr;
	}


	// Assign master to given UID
	Result ClusteredServiceEntity::SetMaster( EntityUID entityUID )
	{
		Result hr = ResultCode::SUCCESS;
		//ServerServiceInformation* pNewMaster = nullptr;

		//svrChk( FindService( entityUID, pNewMaster ) );

		//pNewMaster->SetClusterMembership( ClusterMembership::Master );

		if( GetEntityUID() == entityUID )
		{
			SetClusterMembership( ClusterMembership::Master );
		}
		else
		{
			if( GetClusterMembership() == ClusterMembership::Master )
				SetClusterMembership( ClusterMembership::Slave );
		}

		m_MasterUID = entityUID;

	Proc_End:

		return hr;
	}

	//Result ClusteredServiceEntity::AssignMaster( EntityUID entityUID )
	//{
	//	Result hr = ResultCode::SUCCESS;

	//	svrChk( SetMaster( entityUID ) );

	//	// If i'm the master than broadcast the result
	//	// broadcast new master assignment
	//	ForEach( [&](ServerServiceInformation *pService) 
	//	{
	//		Policy::NetSvrPolicyClusterServer(pService->GetConnection()).ClusterMasterAssignedS2CEvt( RouteContext( GetEntityUID(), pService->GetEntityUID() ), 0, GetEntityUID(), GetClusterID(), entityUID );
	//	});


	//Proc_End:

	//	return hr;
	//}

	
	// Sync data to target 
	Result ClusteredServiceEntity::SyncDataToTarget( EntityUID entityUID )
	{
		Result hr = ResultCode::SUCCESS;
		//ServerServiceInformation* pNewMaster = nullptr;

		//svrChk( FindService( entityUID, pNewMaster ) );

		//// broadcast new master assignment
		//ForEach( [&](ServerServiceInformation *pService) 
		//{
		//	pService->GetConnection()->GetInterface<Policy::NetSvrPolicyClusterServer>()->ClusterMasterAssignedS2CEvt( RouteContext( GetEntityUID(), pService->GetEntityUID() ),1, GetClusterID(), entityUID );
		//});

	//Proc_End:

		return hr;
	}



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ReplicaClusterServiceEntity class
	//

	ReplicaClusterServiceEntity::ReplicaClusterServiceEntity(GameID gameID, ClusterID clusterID, ClusterMembership initialMembership, ServerEntity* pServerEntity )
		: ClusteredServiceEntity(ClusterType::Replication, gameID, clusterID, initialMembership, pServerEntity)
	{
	}


	// Register message handler for this component
	Result ReplicaClusterServiceEntity::RegisterServiceMessageHandler( ServerEntity *pServerEntity )
	{
		ClusteredServiceEntity::RegisterServiceMessageHandler( pServerEntity );

		return ResultCode::SUCCESS;
	}

	Result ReplicaClusterServiceEntity::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;
		auto pClusterManager = Service::ClusterManager;
		svrChk(ClusteredServiceEntity::TickUpdate(pAction) );

	Proc_End:

		return hr;
	}

	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	FreeReplicaClusterServiceEntity class
	//

	FreeReplicaClusterServiceEntity::FreeReplicaClusterServiceEntity(GameID gameID, ClusterID clusterID, ClusterMembership initialMembership, ServerEntity* pServerEntity )
		:ClusteredServiceEntity(ClusterType::FreeReplication, gameID, clusterID, initialMembership, pServerEntity)
	{
	}



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	RingClusterServiceEntity class
	//
	
	RingClusterServiceEntity::RingClusterServiceEntity(GameID gameID, ClusterID clusterID, ClusterMembership initialMembership, ServerEntity* pServerEntity)
		: ClusteredServiceEntity(ClusterType::Ring, gameID, clusterID, initialMembership, pServerEntity)
	{
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ShardedClusterServiceEntity class
	//
	
	ShardedClusterServiceEntity::ShardedClusterServiceEntity(GameID gameID, ClusterID clusterID, ClusterMembership initialMembership, ServerEntity* pServerEntity)
		:ClusteredServiceEntity(ClusterType::Shard, gameID, clusterID, initialMembership, pServerEntity)
	{
	}



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	LoadbalanceClusterServiceEntity class
	//

	// Constructor
	LoadbalanceClusterServiceEntity::LoadbalanceClusterServiceEntity(GameID gameID, ClusterID clusterID, ClusterMembership initialMembership, ServerEntity* pServerEntity )
		:ClusteredServiceEntity(ClusterType::Shard, gameID, clusterID, initialMembership, pServerEntity) // Basic model is same to shard
		,m_pCurrentQueryService(nullptr)
	{
	}

	
	// Initialize entity to proceed new connection
	Result LoadbalanceClusterServiceEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(ClusteredServiceEntity::InitializeEntity(newEntityID) );

		m_WorkloadCheckTimer.SetTimer( DurationMS(Const::WORKLOAD_UPDATE_TIME) );

	Proc_End:

		return hr;
	}


	Result LoadbalanceClusterServiceEntity::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(ClusteredServiceEntity::TickUpdate(pAction) );

		if( m_WorkloadCheckTimer.CheckTimer() )
		{
			m_WorkloadCheckTimer.SetTimer( DurationMS(Const::WORKLOAD_UPDATE_TIME) );

			SetWorkload( (uint)m_LocalWorkload.load(std::memory_order_relaxed) );
		}

	Proc_End:

		return hr;
	}


}; // namespace Svr
}; // namespace SF



