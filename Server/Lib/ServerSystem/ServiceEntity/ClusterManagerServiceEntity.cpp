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
#include "Types/BrSvrTypes.h"
#include "Net/NetDef.h"
#include "Server/BrServer.h"
#include "Entity/Entity.h"
#include "Entity/EntityManager.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServerEntity/ServerEntity.h"
#include "ServiceEntity/ClusterManagerServiceEntity.h"
#include "SvrTrace.h"

#include "Protocol/Message/ClusterServerMsgClass.h"
#include "ServerSystem/ServiceEntity/ClusterServiceTrans.h"
#include "ServerSystem/ServiceEntity/ClusterManagerServiceTrans.h"
#include "Protocol/ServerService/ClusterServerService.h"


namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	ClusterManagerServiceEntity::ClusterManagerServiceEntity(ClusterMembership initialMembership)
		: ReplicaClusterServiceEntity(ClusterID::ClusterManager, initialMembership )
		, IServerComponent(ComponentID)
	{
		// only entity has some special operations
		if (BrServer::GetInstance()->GetNetClass() == NetClass::Entity)
		{
			BR_ENTITY_MESSAGE(Message::ClusterServer::GetClusterMemberListCmd) { svrMemReturn(pNewTrans = new(GetMemoryManager()) ClusterGetMemberListTrans(pMsgData)); return ResultCode::SUCCESS; } );
			BR_ENTITY_MESSAGE(Message::ClusterServer::JoinClusterCmd) { svrMemReturn(pNewTrans = new(GetMemoryManager()) JoinClusterTransForEntityServer(pMsgData)); return ResultCode::SUCCESS; } );
			BR_ENTITY_MESSAGE(Message::ClusterServer::GetLowestWorkloadClusterMemberCmd) { svrMemReturn(pNewTrans = new(GetMemoryManager()) GetLowestWorkloadClusterMemberTrans(pMsgData)); return ResultCode::SUCCESS; } );
			BR_ENTITY_MESSAGE(Message::ClusterServer::NewServerServiceJoinedC2SEvt) { svrMemReturn(pNewTrans = new(GetMemoryManager()) ClusterNewServerServiceJoinedC2SEvtEntityTrans(pMsgData)); return ResultCode::SUCCESS; } );
		}
		else
		{
			BR_ENTITY_MESSAGE(Message::ClusterServer::JoinClusterCmd) { svrMemReturn(pNewTrans = new(GetMemoryManager()) JoinClusterTrans(pMsgData)); return ResultCode::SUCCESS; } );
			BR_ENTITY_MESSAGE(Message::ClusterServer::NewServerServiceJoinedC2SEvt) { svrMemReturn(pNewTrans = new(GetMemoryManager()) ClusterNewServerServiceJoinedC2SEvtTrans(pMsgData)); return ResultCode::SUCCESS; } );
		}
		BR_ENTITY_MESSAGE(Message::ClusterServer::SyncClusterServiceC2SEvt) { svrMemReturn(pNewTrans = new(GetMemoryManager()) SyncClusterServiceTrans(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterMasterAssignedS2CEvt) { svrMemReturn(pNewTrans = new(GetMemoryManager()) ClusterMasterAssignedTrans(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterMasterVoteC2SEvt) { svrMemReturn(pNewTrans = new(GetMemoryManager()) ClusterMasterVoteTrans(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::ClusterServer::RequestDataSyncCmd) { svrMemReturn(pNewTrans = new(GetMemoryManager()) RequestDataSyncTrans(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterUpdateStatusC2SEvt) { svrMemReturn(pNewTrans = new(GetMemoryManager()) ClusterUpdateStatusTrans(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterUpdateWorkloadC2SEvt) { svrMemReturn(pNewTrans = new(GetMemoryManager()) ClusterUpdateWorkloadTrans(pMsgData)); return ResultCode::SUCCESS; } );
	}
	
	ClusterManagerServiceEntity::~ClusterManagerServiceEntity()
	{
	}

	Result ClusterManagerServiceEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(ReplicaClusterServiceEntity::InitializeEntity(newEntityID) );

		svrChk( m_ClusterIDMap.insert( (uint)GetClusterID(), this ) );

		svrChk(StartInitializeTransaction() );

	Proc_End:

		return hr;
	}

	// Initialize not initialized cluster entities
	Result ClusterManagerServiceEntity::InitializeNotInitializedClusterEntities()
	{
		Result hr = ResultCode::SUCCESS;
		ClusterIDMap::iterator itCur;

		for( itCur = m_ClusterIDMap.begin(); itCur.IsValid(); ++itCur )
		{
			ClusteredServiceEntity* pClusterServiceEntity = *itCur;
			ServiceInformation serviceInfo;
			if( !pClusterServiceEntity->GetInitialized() )
			{
				svrChk( pClusterServiceEntity->StartInitializeTransaction() );
			}
		}

	Proc_End:

		return hr;
	}

	// Get cluster service entity
	Result ClusterManagerServiceEntity::GetClusterServiceEntity( ClusterID clusterID, ClusteredServiceEntity* &pServiceEntity )
	{
		Result hr = ResultCode::SUCCESS;
		ClusterIDMap::iterator iterMap;

		hr = m_ClusterIDMap.find((uint)clusterID, iterMap );
		if( (hr) )
			pServiceEntity = *iterMap;

	//Proc_End:

		return hr;
	}

	// Add cluster service entity
	Result ClusterManagerServiceEntity::AddClusterServiceEntity( ClusteredServiceEntity* pServiceEntity )
	{
		Result hr = ResultCode::SUCCESS;

		svrChkPtr( pServiceEntity );

		svrChk( m_ClusterIDMap.insert((uint)pServiceEntity->GetClusterID(), pServiceEntity ) );

		pServiceEntity = nullptr;

	Proc_End:

		return hr;
	}
	
	
	// Create a watcher for cluster
	Result ClusterManagerServiceEntity::CreateWatcherForCluster( ClusterID clusterID, ClusterType clusterType, ClusteredServiceEntity* &pServiceEntity )
	{
		Result hr = ResultCode::SUCCESS;
		EntityID entityID(EntityFaculty::Service,(uint32_t)clusterID);
		SharedPointerT<Entity> pEntity;

		if( ( GetServerComponent<EntityManager>()->FindEntity( entityID, pEntity ) ) )
		{
			// same entity already registered, just check integrity
			ClusteredServiceEntity* pClusterEntity = BR_DYNAMIC_CAST(ClusteredServiceEntity*,(Entity*)pEntity);
			unused(pClusterEntity);
			Assert(pClusterEntity != nullptr);
			Assert( pClusterEntity->GetClusterID() == clusterID && pClusterEntity->GetClusterType() == clusterType );
			goto Proc_End;
		}

		// Create one for status watch
		switch (clusterType)
		{
		case ClusterType::Replication:
		case ClusterType::FreeReplication:
		case ClusterType::Ring:
		case ClusterType::Shard:
			svrMem( pServiceEntity = new(GetMemoryManager()) WatcherClusteredServiceEntity( clusterType, clusterID, ClusterMembership::StatusWatcher ) );
			break;
		default:
			svrErr(ResultCode::SVR_INVALID_CLUSTERTYPE);
			break;
		}

		svrTrace(SVR_CLUSTER, "Creating a watcher:{0} for ClusterID:{1} in {2}", entityID, clusterID, GetClusterID());

		// We are going to use reserved entity ID
		svrChk( GetServerComponent<EntityManager>()->AddEntity( entityID, pServiceEntity ) );

		svrChk( pServiceEntity->SetServiceStatus( ServiceStatus::Ready ) );

		// We need new one for this
		svrChk( GetServerComponent<ClusterManagerServiceEntity>()->AddClusterServiceEntity( pServiceEntity ) );

	Proc_End:

		return hr;
	}

	// Sync data to target 
	Result ClusterManagerServiceEntity::SyncDataToTarget( EntityUID entityUID )
	{
		Result hr = ResultCode::SUCCESS;
		ClusterIDMap::iterator itCur;
		StaticArray<ServiceInformation,100> memberList;
		ServerServiceInformation *pTargetServiceInfo = nullptr;
		ClusterServerService *pTargetService = nullptr;

		svrChk( FindService( entityUID, pTargetServiceInfo ) );

		svrChkPtr( pTargetService = pTargetServiceInfo->GetService<ClusterServerService>() );

		for( itCur = m_ClusterIDMap.begin(); itCur.IsValid(); ++itCur )
		{
			ClusteredServiceEntity* pClusterServiceEntity = *itCur;
			ServiceInformation serviceInfo;
			memberList.Clear();
			
			pClusterServiceEntity->ForEach( [&]( ServerServiceInformation* pServiceInfo )
			{
				if( pServiceInfo->IsServiceAvailable() )
				{
					pServiceInfo->GetServiceInformation( serviceInfo );
					memberList.push_back(serviceInfo);
				}
			});

			svrChk( pTargetService->SyncClusterServiceC2SEvt( GetEntityID(), 0, pClusterServiceEntity->GetClusterID(), pClusterServiceEntity->GetClusterType(), memberList ) );
		}

	Proc_End:

		return hr;
	}

	Result ClusterManagerServiceEntity::RegisterServiceMessageHandler( ServerEntity *pServerEntity )
	{
		// Do not use parent service mapping
		//ReplicaClusterServiceEntity::RegisterServiceMessageHandler( pServerEntity );

		// only entity has some special operations
		if( BrServer::GetInstance()->GetNetClass() == NetClass::Entity )
		{
			pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::GetClusterMemberListCmd)				{ svrMemReturn(pNewTrans = new(GetMemoryManager()) ClusterGetMemberListTrans(pMsgData)); return ResultCode::SUCCESS; } );
			pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::JoinClusterCmd)						{ svrMemReturn(pNewTrans = new(GetMemoryManager()) JoinClusterTransForEntityServer(pMsgData)); return ResultCode::SUCCESS; } );
			pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::GetLowestWorkloadClusterMemberCmd)		{ svrMemReturn(pNewTrans = new(GetMemoryManager()) GetLowestWorkloadClusterMemberTrans(pMsgData)); return ResultCode::SUCCESS; } );
			pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::NewServerServiceJoinedC2SEvt)			{ svrMemReturn(pNewTrans = new(GetMemoryManager()) ClusterNewServerServiceJoinedC2SEvtEntityTrans(pMsgData)); return ResultCode::SUCCESS; } );
		}
		else
		{
			pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::JoinClusterCmd)						{ svrMemReturn(pNewTrans = new(GetMemoryManager()) JoinClusterTrans(pMsgData)); return ResultCode::SUCCESS; } );
			pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::NewServerServiceJoinedC2SEvt)			{ svrMemReturn(pNewTrans = new(GetMemoryManager()) ClusterNewServerServiceJoinedC2SEvtTrans(pMsgData)); return ResultCode::SUCCESS; } );
		}
		pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::SyncClusterServiceC2SEvt)				{ svrMemReturn(pNewTrans = new(GetMemoryManager()) SyncClusterServiceTrans(pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterMasterAssignedS2CEvt)			{ svrMemReturn(pNewTrans = new(GetMemoryManager()) ClusterMasterAssignedTrans(pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterMasterVoteC2SEvt)				{ svrMemReturn(pNewTrans = new(GetMemoryManager()) ClusterMasterVoteTrans(pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::RequestDataSyncCmd)					{ svrMemReturn(pNewTrans = new(GetMemoryManager()) RequestDataSyncTrans(pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterUpdateStatusC2SEvt)				{ svrMemReturn(pNewTrans = new(GetMemoryManager()) ClusterUpdateStatusTrans(pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterUpdateWorkloadC2SEvt)			{ svrMemReturn(pNewTrans = new(GetMemoryManager()) ClusterUpdateWorkloadTrans(pMsgData)); return ResultCode::SUCCESS; } );

		return ResultCode::SUCCESS;
	}


	Result ClusterManagerServiceEntity::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;
		ServiceEntityUIDMap::iterator itService;

		svrChk(ReplicaClusterServiceEntity::TickUpdate(pAction) );

		if( GetClusterMembership() <= ClusterMembership::Slave )
			UpdateOnMasterManager();


	Proc_End:

		return hr;

	}
	
}; // namespace Svr {
}; // namespace SF {



