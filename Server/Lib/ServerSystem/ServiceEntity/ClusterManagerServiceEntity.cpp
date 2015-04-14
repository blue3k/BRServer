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



#include "StdAfx.h"
#include "Common/StrUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "Net/NetDef.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServicebase.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServerSystem/SvrTrace.h"

#include "Protocol/Message/ClusterServerMsgClass.h"
#include "ServerSystem/ServiceEntity/ClusterServiceTrans.h"
#include "ServerSystem/ServiceEntity/ClusterManagerServiceTrans.h"
#include "ServerSystem/ServerService/ClusterServerService.h"


namespace BR {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	ClusterManagerServiceEntity::ClusterManagerServiceEntity(ClusterMembership initialMembership)
		:IServerComponent(ComponentID)
		,ReplicaClusterServiceEntity(ClusterID::ClusterManager, initialMembership )
	{
	}
	
	ClusterManagerServiceEntity::~ClusterManagerServiceEntity()
	{
	}

	HRESULT ClusterManagerServiceEntity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_OK;

		svrChk( __super::InitializeEntity(newEntityID) );

		svrChk( m_ClusterIDMap.insert( this ) );

		svrChk( __super::StartInitializeTransaction() );

	Proc_End:

		return hr;
	}

	// Initialize not initialized cluster entities
	HRESULT ClusterManagerServiceEntity::InitializeNotInitializedClusterEntities()
	{
		HRESULT hr = S_OK;
		ClusterIDMap::iterator itCur;
		ServerServiceInformation *pTargetServiceInfo = nullptr;
		ClusterServerService *pTargetService = nullptr;

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
	HRESULT ClusterManagerServiceEntity::GetClusterServiceEntity( ClusterID clusterID, ClusteredServiceEntity* &pServiceEntity )
	{
		HRESULT hr = S_OK;
		ClusterIDMap::iterator iterMap;

		hr = m_ClusterIDMap.find( clusterID, iterMap );
		if( SUCCEEDED(hr) )
			pServiceEntity = *iterMap;

	Proc_End:

		return hr;
	}

	// Add cluster service entity
	HRESULT ClusterManagerServiceEntity::AddClusterServiceEntity( ClusteredServiceEntity* pServiceEntity )
	{
		HRESULT hr = S_OK;

		svrChkPtr( pServiceEntity );

		svrChk( m_ClusterIDMap.insert( pServiceEntity ) );

		pServiceEntity = nullptr;

	Proc_End:

		return hr;
	}
	
	
	// Create a watcher for cluster
	HRESULT ClusterManagerServiceEntity::CreateWatcherForCluster( ClusterID clusterID, ClusterType clusterType, ClusteredServiceEntity* &pServiceEntity )
	{
		HRESULT hr = S_OK;
		EntityID entityID(EntityFaculty::Service,(UINT32)clusterID);
		SharedPointerT<Entity> pEntity;

		if( SUCCEEDED( GetServerComponent<EntityManager>()->FindEntity( entityID, pEntity ) ) )
		{
			// same entity already registered, just check integrity
			ClusteredServiceEntity* pClusterEntity = BR_DYNAMIC_CAST(ClusteredServiceEntity*,(Entity*)pEntity);
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
			svrMem( pServiceEntity = new WatcherClusteredServiceEntity( clusterType, clusterID, ClusterMembership::StatusWatcher ) );
			break;
		default:
			svrErr(E_SVR_INVALID_CLUSTERTYPE);
			break;
		}

		svrTrace(Svr::TRC_CLUSTER, "Creating a watcher:%0% for ClusterID:%1% in %2%", entityID, clusterID, GetClusterID());

		// We are going to use reserved entity ID
		svrChk( GetServerComponent<EntityManager>()->AddEntity( entityID, pServiceEntity ) );

		svrChk( pServiceEntity->SetServiceStatus( ServiceStatus::Ready ) );

		// We need new one for this
		svrChk( GetServerComponent<ClusterManagerServiceEntity>()->AddClusterServiceEntity( pServiceEntity ) );

	Proc_End:

		return hr;
	}

	// Sync data to target 
	HRESULT ClusterManagerServiceEntity::SyncDataToTarget( EntityUID entityUID )
	{
		HRESULT hr = S_OK;
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

	HRESULT ClusterManagerServiceEntity::RegisterServiceMessageHandler( ServerEntity *pServerEntity )
	{
		// Do not use parent service mapping
		//__super::RegisterServiceMessageHandler( pServerEntity );

		// only entity has some special operations
		if( BrServer::GetInstance()->GetNetClass() == NetClass::Entity )
		{
			pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::GetClusterMemberListCmd)				{ svrMemReturn(pNewTrans = new ClusterGetMemberListTrans(pMsgData)); return S_OK; } );
			pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::JoinClusterCmd)						{ svrMemReturn(pNewTrans = new JoinClusterTransForEntityServer(pMsgData)); return S_OK; } );
			pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::GetLowestWorkloadClusterMemberCmd)		{ svrMemReturn(pNewTrans = new GetLowestWorkloadClusterMemberTrans(pMsgData)); return S_OK; } );
			pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::NewServerServiceJoinedC2SEvt)			{ svrMemReturn(pNewTrans = new ClusterNewServerServiceJoinedC2SEvtEntityTrans(pMsgData)); return S_OK; } );
		}
		else
		{
			pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::JoinClusterCmd)						{ svrMemReturn(pNewTrans = new JoinClusterTrans(pMsgData)); return S_OK; } );
			pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::NewServerServiceJoinedC2SEvt)			{ svrMemReturn(pNewTrans = new ClusterNewServerServiceJoinedC2SEvtTrans(pMsgData)); return S_OK; } );
		}
		pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::SyncClusterServiceC2SEvt)				{ svrMemReturn(pNewTrans = new SyncClusterServiceTrans(pMsgData)); return S_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterMasterAssignedS2CEvt)			{ svrMemReturn(pNewTrans = new ClusterMasterAssignedTrans(pMsgData)); return S_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterMasterVoteC2SEvt)				{ svrMemReturn(pNewTrans = new ClusterMasterVoteTrans(pMsgData)); return S_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::RequestDataSyncCmd)					{ svrMemReturn(pNewTrans = new RequestDataSyncTrans(pMsgData)); return S_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterUpdateStatusC2SEvt)				{ svrMemReturn(pNewTrans = new ClusterUpdateStatusTrans(pMsgData)); return S_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterUpdateWorkloadC2SEvt)			{ svrMemReturn(pNewTrans = new ClusterUpdateWorkloadTrans(pMsgData)); return S_OK; } );

		return S_OK;
	}


	HRESULT ClusterManagerServiceEntity::TickUpdate(Svr::TimerAction *pAction)
	{
		HRESULT hr = S_OK;
		ServiceEntityUIDMap::iterator itService;

		svrChk( __super::TickUpdate(pAction) );

		if( GetClusterMembership() <= ClusterMembership::Slave )
			UpdateOnMasterManager();


	Proc_End:

		return hr;

	}
	
}; // namespace Svr {
}; // namespace BR {



