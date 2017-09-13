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
#include "Net/NetDef.h"
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


	ClusteredServiceEntity::ClusteredServiceEntity( ClusterType clusterType, ClusterID clusterID, ClusterMembership initialMembership, ServerEntity* pServerEntity )
		: m_MasterUID(0)
		, m_ClusterID(clusterID)
		, m_ClusterType(clusterType )
		, m_ClusterMembership(initialMembership)
		, m_ServiceStatus(ServiceStatus::Offline)
		, m_Workload(0)
		, m_ServiceEntityUIDMap(GetHeap())
		, m_WatcherUIDMap(GetHeap())
		, m_ServerEntity(pServerEntity)
	{
		if (BrServer::GetInstance()->GetNetClass() == NetClass::Entity)
		{
			BR_ENTITY_MESSAGE(Message::ClusterServer::GetLowestWorkloadClusterMemberCmd)	{ svrMemReturn(pNewTrans = new(GetHeap()) GetLowestWorkloadClusterMemberTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
			BR_ENTITY_MESSAGE(Message::ClusterServer::NewServerServiceJoinedC2SEvt)			{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterNewServerServiceJoinedC2SEvtEntityTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		}
		else
		{
			BR_ENTITY_MESSAGE(Message::ClusterServer::NewServerServiceJoinedC2SEvt)			{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterNewServerServiceJoinedC2SEvtTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		}
		BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterMasterAssignedS2CEvt)				{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterMasterAssignedTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterMasterVoteC2SEvt)					{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterMasterVoteTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::ClusterServer::RequestDataSyncCmd)						{ svrMemReturn(pNewTrans = new(GetHeap()) RequestDataSyncTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterUpdateStatusC2SEvt)				{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterUpdateStatusTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterUpdateWorkloadC2SEvt)				{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterUpdateWorkloadTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );

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

	//////////////////////////////////////////////////////////////////////////
	//
	//	Voting information
	//

	void ClusteredServiceEntity::StartVoting()
	{
		if( m_IsInVoting )
			return;

		m_IsInVoting = true;

		ForEach( [&](ServerServiceInformation *pService) 
		{
			pService->SetVotedCount(0);
		});
	}

	void ClusteredServiceEntity::EndVoting()
	{
		if( !m_IsInVoting ) 
			return;

		m_IsInVoting = false;

		ForEach( [&](ServerServiceInformation *pService) 
		{
			pService->SetVotedCount(0);
		});
	}

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

		svrChk( NewServerService( GetEntityUID(), GetServerEntity(), GetClusterMembership(), GetServiceStatus(), pMyService ) );

		m_MyServiceInfo = (ServiceTableItem*)pMyService;

		//OnInitializeEntity();

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
		//svrChk( GetTransactionQueue().Enqueue( pInitTransaction ) );

		pTrans = nullptr;

	Proc_End:

		Util::SafeDelete(pInitTransaction);

		return hr;
	}

	// clear transaction
	Result ClusteredServiceEntity::ClearEntity()
	{
		Result hr = ResultCode::SUCCESS;

		auto itIDMap = m_ServiceEntityUIDMap.begin();
		for(; itIDMap.IsValid(); ++itIDMap )
		{
			ServiceTableItem *pService = *itIDMap;
			Util::SafeDelete( pService );
		}
		m_ServiceEntityUIDMap.clear();
		m_MyServiceInfo = nullptr;

		itIDMap = m_WatcherUIDMap.begin();
		for(; itIDMap.IsValid(); ++itIDMap )
		{
			ServiceTableItem *pService = *itIDMap;
			Util::SafeDelete( pService );
		}
		m_WatcherUIDMap.clear();


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

		svrChk( GetServerComponent<ClusterManagerServiceEntity>()->FindRandomService( pServiceInfo ) );
		//svrChk( FindRandomService( pServiceInfo ) );
		svrChkPtr( pService = pServiceInfo->GetService<ClusterServerService>() );
		svrChk( pService->ClusterUpdateWorkloadC2SEvt( GetEntityUID(), 0, GetEntityUID(), GetClusterID(), m_Workload ) );

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

		// Ask to the cluster manager master to update status
		svrChk( GetServerComponent<ClusterManagerServiceEntity>()->FindRandomService( pServiceInfo ) );
		//svrChk( GetServerComponent<ClusterManagerServiceEntity>()->GetService<ClusterServerService>(pService) );
		svrChkPtr( pService = pServiceInfo->GetService<ClusterServerService>() );
		svrChk( pService->ClusterUpdateStatusC2SEvt( GetEntityUID(), 0, GetEntityUID(), GetClusterID(), GetServiceStatus() ) );

	Proc_End:


		return hr;
	}

	size_t ClusteredServiceEntity::GetNumberOfAvailableServices()
	{
		size_t szNumServices = 0;
		auto itIDMap = m_ServiceEntityUIDMap.begin();
		for(; itIDMap.IsValid(); ++itIDMap )
		{
			ServiceTableItem *pService = *itIDMap;
			if( pService->IsServiceAvailable() )
				szNumServices++;
		}
		itIDMap = nullptr;

		return szNumServices;
	}

	// Register Entity by Given serverID
	Result ClusteredServiceEntity::NewServerService( EntityUID entityUID, ServerEntity *pServerEntity, ClusterMembership membership, ServiceStatus status, ServerServiceInformation* &pService )
	{
		Result hr = ResultCode::SUCCESS;
		ServiceTableItem *pNewService = nullptr;
		ServiceEntityUIDMap::iterator itFound;

		auto ActionOnAlreadyExist = [&]( ServiceTableItem *pServiceEntity )
		{
			// Just update status 
			if( pServiceEntity->GetServiceStatus() != status )
				pServiceEntity->SetServiceStatus( status );
			pServiceEntity->SetClusterMembership( membership );

			Assert(pServiceEntity->GetServerEntity() == pServerEntity);
			pService = pServiceEntity;

			if( pServiceEntity->GetClusterMembership() == ClusterMembership::Master )
			{
				m_MasterUID = pServiceEntity->GetEntityUID();

				if( entityUID == GetEntityUID() )
					SetClusterMembership( ClusterMembership::Master );
			}
		};

		svrChkPtr( pServerEntity );

		// Create new one
		svrMem( pNewService = new(GetHeap()) ServiceTableItem( GetClusterID(), pServerEntity, membership ) );

		pNewService->SetEntityUID( entityUID );
		pNewService->SetServiceStatus( status );

		if( membership == ClusterMembership::StatusWatcher )
		{
			if( !(m_WatcherUIDMap.insert((Context)pNewService->GetEntityUID(), pNewService)) )
			{
				svrChk(m_WatcherUIDMap.find((uint64_t)entityUID, itFound));

				ActionOnAlreadyExist(*itFound);

				hr = ResultCode::SUCCESS_FALSE;
				goto Proc_End;
			}
		}
		else
		{
			if( !(m_ServiceEntityUIDMap.insert((Context)pNewService->GetEntityUID(), pNewService)) )
			{
				svrChk(m_ServiceEntityUIDMap.find((uint64_t)entityUID, itFound));

				ActionOnAlreadyExist(*itFound);

				hr = ResultCode::SUCCESS_FALSE;
				goto Proc_End; // already exist, just ignore it
			}
		}

		if( pNewService->GetClusterMembership() == ClusterMembership::Master )
		{
			m_MasterUID = pNewService->GetEntityUID();
		}

		pService = pNewService;
		pNewService = nullptr;

	Proc_End:

		Util::SafeDelete(pNewService);
		return hr;
	}

	// Find Service information by ServiceID
	Result ClusteredServiceEntity::FindService( EntityUID entityUID, ServerServiceInformation* &pService )
	{
		Result hr = ResultCode::SUCCESS;
		ServiceEntityUIDMap::iterator itFound;

		if (!(m_ServiceEntityUIDMap.find((uint64_t)entityUID, itFound)))
		{
			if (!(m_WatcherUIDMap.find((uint64_t)entityUID, itFound)))
				return ResultCode::FAIL;
		}
		pService = *itFound;

	//Proc_End:

		return hr;
	}
	
	// Find random service, maybe lowest workload service
	Result ClusteredServiceEntity::FindRandomService( ServerServiceInformation* &pService )
	{
		//Result hr = ResultCode::SUCCESS;
		ServiceTableItem *pBestService = nullptr;

		auto itIDMap = m_ServiceEntityUIDMap.begin();
		for(; itIDMap.IsValid(); ++itIDMap )
		{
			ServiceTableItem *pService = *itIDMap;
			if( pService->IsServiceAvailable() )
			{
				if( pBestService == nullptr || pBestService->GetWorkload() > pService->GetWorkload() )
					pBestService = pService;
			}
		}
		itIDMap = nullptr;

		pService = pBestService;

	//Proc_End:

		return pBestService != nullptr ? ResultCode::SUCCESS : ResultCode::FAIL;
	}

	// Foreach service
	Result ClusteredServiceEntity::ForEach( std::function<void(ServerServiceInformation*)> func )
	{
		Result hr = ResultCode::SUCCESS;
		auto itIDMap = m_ServiceEntityUIDMap.begin();
		for(; itIDMap.IsValid(); ++itIDMap )
		{
			ServiceTableItem *pService = *itIDMap;
			if( pService->IsServiceAvailable() )
				func(pService);
		}
		itIDMap = nullptr;

		auto itWatcher = m_WatcherUIDMap.begin();
		for(; itWatcher.IsValid(); ++itWatcher )
		{
			ServiceTableItem *pService = *itWatcher;
			if( pService->IsServiceAvailable() )
				func(pService);
		}
		itWatcher = nullptr;

	//Proc_End:

		return hr;
	}

	Result ClusteredServiceEntity::ForEachNonWatcher( std::function<void(ServerServiceInformation*)> func )
	{
		Result hr = ResultCode::SUCCESS;
		auto itIDMap = m_ServiceEntityUIDMap.begin();
		for(; itIDMap.IsValid(); ++itIDMap )
		{
			ServiceTableItem *pService = *itIDMap;
			if( pService->IsServiceAvailable() )
				func(pService);
		}
		itIDMap = nullptr;

	//Proc_End:

		return hr;
	}


	// Assign master to given UID
	Result ClusteredServiceEntity::SetMaster( EntityUID entityUID )
	{
		Result hr = ResultCode::SUCCESS;
		ServerServiceInformation* pNewMaster = nullptr;

		svrChk( FindService( entityUID, pNewMaster ) );

		pNewMaster->SetClusterMembership( ClusterMembership::Master );

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

	Result ClusteredServiceEntity::AssignMaster( EntityUID entityUID )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( SetMaster( entityUID ) );

		// If i'm the master than broadcast the result
		// broadcast new master assignment
		ForEach( [&](ServerServiceInformation *pService) 
		{
			Policy::NetSvrPolicyClusterServer(pService->GetConnection()).ClusterMasterAssignedS2CEvt( RouteContext( GetEntityUID(), pService->GetEntityUID() ), 0, GetEntityUID(), GetClusterID(), entityUID );
		});


	Proc_End:

		return hr;
	}

	
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


	WatcherClusteredServiceEntity::WatcherClusteredServiceEntity( ClusterType clusterType, ClusterID clusterID, ClusterMembership initialMembership, ServerEntity* pServerEntity )
		:ClusteredServiceEntity(clusterType, clusterID, initialMembership, pServerEntity)
	{
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ReplicaClusterServiceEntity class
	//

	ReplicaClusterServiceEntity::ReplicaClusterServiceEntity( ClusterID clusterID, ClusterMembership initialMembership, ServerEntity* pServerEntity )
		:ClusteredServiceEntity(ClusterType::Replication, clusterID, initialMembership, pServerEntity)
	{
	}

	Result ReplicaClusterServiceEntity::UpdateOnMasterManager()
	{
		Result hr = ResultCode::SUCCESS;
		ServiceEntityUIDMap::iterator itService;

		
		// Vote for new master if the master isn't available
		if (!(GetServiceEntityUIDMap().find((uint64_t)GetMasterUID(), itService))
			|| !itService->IsServiceAvailable() )
		{
			//ServiceTableItem *pMaster = itService.IsValid() ? *itService : nullptr;

			if( !m_MasterCheckTimer.IsTimerWorking() )
			{
				m_MasterCheckTimer.SetTimer( DurationMS(MASTER_CHECK_TIME) );
				goto Proc_End;
			}

			if( !m_MasterCheckTimer.CheckTimer() )
			{
				goto Proc_End;
			}

			// start voting if not started
			//StartVoting();

			TimeStampSec earliestUpTime = TimeStampSec::max();
			ServiceTableItem *pCandidate = nullptr;

			auto itIDMap = GetServiceEntityUIDMap().begin();
			for(; itIDMap.IsValid(); ++itIDMap )
			{
				ServiceTableItem *pService = *itIDMap;
				if( !pService->IsServiceAvailable() 
					|| pService->GetClusterMembership() >= ClusterMembership::StatusWatcher )
					continue;

				if( earliestUpTime > pService->GetServerEntity()->GetServerUpTime() )
				{
					earliestUpTime = pService->GetServerEntity()->GetServerUpTime();
					pCandidate = pService;
				}
			}
			itIDMap = nullptr;

			if( pCandidate != nullptr )
			{
				svrTrace( SVR_CLUSTER, "New Master is selected Cluster:{0} Entity:{1}", GetClusterID(), pCandidate->GetEntityUID() );
				svrChk( AssignMaster( pCandidate->GetEntityUID() ) );
			}
		}

	Proc_End:

		return hr;
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

		svrChk(ClusteredServiceEntity::TickUpdate(pAction) );

		if( GetServerComponent<ClusterManagerServiceEntity>()->GetClusterMembership() == ClusterMembership::Master )
		{
			UpdateOnMasterManager();
		}

	Proc_End:

		return hr;
	}

	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	FreeReplicaClusterServiceEntity class
	//

	FreeReplicaClusterServiceEntity::FreeReplicaClusterServiceEntity( ClusterID clusterID, ClusterMembership initialMembership, ServerEntity* pServerEntity )
		:ClusteredServiceEntity(ClusterType::FreeReplication, clusterID, initialMembership, pServerEntity)
	{
	}



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	RingClusterServiceEntity class
	//
	
	RingClusterServiceEntity::RingClusterServiceEntity(ClusterID clusterID, ClusterMembership initialMembership, ServerEntity* pServerEntity)
		:ClusteredServiceEntity(ClusterType::Ring, clusterID, initialMembership, pServerEntity)
		,m_pCurrentQueryService(nullptr)
	{
	}

	// Override so that we can make ring linked list
	Result RingClusterServiceEntity::NewServerService( EntityUID entityUID, ServerEntity *pServerEntity, ClusterMembership membership, ServiceStatus status, ServerServiceInformation* &pService )
	{
		Result hr = ResultCode::SUCCESS;
		ServiceTableItem *pTblItem = nullptr;
		OrderedServiceList::Node *pPrevNode = nullptr;

		// This need to be thread safe
		MutexScopeLock scopelock(m_ListLock);

		Result hrRes = ClusteredServiceEntity::NewServerService(entityUID, pServerEntity, membership, status, pService);

		svrChkPtr(pService);
		pTblItem = (ServiceTableItem*)pService;

		// When the return value is ResultCode::SUCCESS_FALSE, it means this service is already in the list
		// Remove it if the key value isn't same
		if( hrRes == Result(ResultCode::SUCCESS_FALSE) && pTblItem->m_ListNode.Key != pServerEntity->GetServerUpTime().time_since_epoch().count() )
		{
			// I hope this not happened here
			Assert(pTblItem->GetServerEntity() == pServerEntity);
			svrChk( m_ServiceList.FindPrevNode( pTblItem->m_ListNode.Key, pPrevNode ) );
			svrChk( m_ServiceList.FindAndRemove( pPrevNode, &pTblItem->m_ListNode ) );
			hrRes = ResultCode::SUCCESS;
		}

		if( (hrRes) )
		{
			svrChk( m_ServiceList.FindPrevNode( pServerEntity->GetServerUpTime().time_since_epoch().count(), pPrevNode ) );

			// we don't have to update it again
			if( hrRes != Result(ResultCode::SUCCESS_FALSE) )
			{
				Assert(pPrevNode != &pTblItem->m_ListNode );
				Assert(pTblItem->m_ListNode.pNext == nullptr);
				// If the table already exist in the list, and it just updated hrRes would have ResultCode::SUCCESS_FALSE
				Assert(pPrevNode->pNext != &pTblItem->m_ListNode);

				svrChk( m_ServiceList.Insert( pPrevNode, pServerEntity->GetServerUpTime().time_since_epoch().count(), &pTblItem->m_ListNode ) );
			}
		}
		svrChk(hr);

	Proc_End:

		return hr;
	}

	// Get next ring token
	Result RingClusterServiceEntity::GetNextRing( ServerServiceInformation* pService, ServerServiceInformation* &pNextService )
	{
		Result hr = ResultCode::SUCCESS;
		ServiceTableItem *pTblItem = nullptr;
		MapItemConverter<ServiceTableItem,TableItemType,&ServiceTableItem::m_ListNode> converter;

		pTblItem = (ServiceTableItem*)pService;

		do {
			pTblItem = converter(pTblItem->m_ListNode.pNext);

			// circulate the list until meet itself
			if( pTblItem == nullptr )
			{
				pTblItem = converter(&*m_ServiceList.begin());
			}

			if( pTblItem == pService )
			{
				// couldn't find any proper service
				pNextService = nullptr;
			}
			else
			{
				pNextService = pTblItem;
			}

		} while( pNextService != nullptr && !pNextService->IsServiceAvailable() );

	//Proc_End:

		return pNextService ? hr : ResultCode::FAIL;
	}

	// Get a service form the ring
	Result RingClusterServiceEntity::GetService( ServerServiceInformation* &pService )
	{
		Result hr = ResultCode::SUCCESS;
		ServiceTableItem *pTblItem = nullptr;
		MapItemConverter<ServiceTableItem,TableItemType,&ServiceTableItem::m_ListNode> converter;

		if( m_pCurrentQueryService == nullptr )
		{
			auto itBegin = m_ServiceList.begin();
			if (itBegin.IsValid())
				m_pCurrentQueryService = converter(&*m_ServiceList.begin());
			else
				m_pCurrentQueryService = nullptr;
		}

		// no service for now
		if (m_pCurrentQueryService == nullptr) goto Proc_End;

		pTblItem = m_pCurrentQueryService ? (ServiceTableItem*)m_pCurrentQueryService : nullptr;

		Assert( pTblItem != nullptr );
		while( pTblItem != nullptr )
		{
			pTblItem = converter(pTblItem->m_ListNode.pNext);

			// circulate the list until meet itself
			if( pTblItem == nullptr )
			{
				pTblItem = converter(&*m_ServiceList.begin());
			}

			// We don't have any service now
			if( pTblItem == nullptr )
				break;

			bool bServiceAvailable = pTblItem->IsServiceAvailable();

			// If we circulate once
			if( pTblItem == m_pCurrentQueryService )
			{
				// Can't find new one
				// If this is not available service set to nullptr
				if (!bServiceAvailable || pTblItem->GetClusterMembership() > ClusterMembership::Slave)
				{
					pTblItem = nullptr;
				}
				break;
			}
			else if (bServiceAvailable && pTblItem->GetClusterMembership() <= ClusterMembership::Slave)
			{
				// we found new one
				break;
			}
		}

		if( pTblItem != nullptr )
			m_pCurrentQueryService = pTblItem;

		pService = pTblItem;

	Proc_End:

#if 0 //defined(DEBUG)
	if( pService == nullptr )
	{
		svrTrace( SVR_DBGINFO, "Service Item: ClusterID:{0}, EntityUID:{1}, Membership:{2}", GetClusterID(), GetEntityUID(), GetClusterMembership() );
		for( auto itService = m_ServiceList.begin(); itService.IsValid(); ++itService )
		{
			ServiceTableItem *pServiceItem = converter(&*itService);
			svrTrace( SVR_DBGINFO, "Service Item: ClusterID:{0}, EntityUID:{1}, Membership:{2}", pServiceItem->GetClusterID(), pServiceItem->GetEntityUID(), pServiceItem->GetClusterMembership() );
		}
	}
#endif

		return pService ? hr : ResultCode::FAIL;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ShardedClusterServiceEntity class
	//
	
	ShardedClusterServiceEntity::ShardedClusterServiceEntity(ClusterID clusterID, ClusterMembership initialMembership, ServerEntity* pServerEntity)
		:ClusteredServiceEntity(ClusterType::Shard, clusterID, initialMembership, pServerEntity)
		,m_HashMod(0)
	{
	}

	// Hash the key value
	uint ShardedClusterServiceEntity::KeyHash( uint64_t key )
	{
		return (uint)std::hash<uint64_t>()(key);
	}

	// Get Service shard by key
	Result ShardedClusterServiceEntity::GetShard( uint64_t key, ServerServiceInformation* &pService )
	{
		Result hr = ResultCode::SUCCESS;
		uint hashedKey = KeyHash(key);
		INT serviceIndex = -1;
		StaticArray<ServerServiceInformation*,50> services(GetHeap());

		svrChkPtr( pService );

		// Basically dynamic sharding
		ForEachNonWatcher( [&]( ServerServiceInformation *pCurService )
		{
			services.push_back( pCurService );
		});

		if( m_HashMod > 0 )
		{
			// Fixed sharding if HashMode is specified
			serviceIndex = hashedKey % m_HashMod;
		}
		else if( services.size() > 0 )
		{
			// Otherwise dynamic sharding
			serviceIndex = (decltype(serviceIndex))(hashedKey % services.size());
		}

		if( serviceIndex < 0 || serviceIndex >= (int)services.size() )
			goto Proc_End;

		pService = services[serviceIndex];

	Proc_End:

		return hr;
	}



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	LoadbalanceClusterServiceEntity class
	//

	// Constructor
	LoadbalanceClusterServiceEntity::LoadbalanceClusterServiceEntity( ClusterID clusterID, ClusterMembership initialMembership, ServerEntity* pServerEntity )
		:ClusteredServiceEntity(ClusterType::Shard, clusterID, initialMembership, pServerEntity) // Basic model is same to shard
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


	// Override so that we can make ring linked list
	Result LoadbalanceClusterServiceEntity::NewServerService( EntityUID entityUID, ServerEntity *pServerEntity, ClusterMembership membership, ServiceStatus status, ServerServiceInformation* &pService )
	{
		Result hr = ResultCode::SUCCESS;
		ServiceTableItem *pTblItem = nullptr;
		OrderedServiceList::Node *pPrevNode = nullptr;

		hr = ClusteredServiceEntity::NewServerService(entityUID, pServerEntity, membership, status, pService);

		pTblItem = (ServiceTableItem*)pService;

		// When the return value is ResultCode::SUCCESS_FALSE, it means this service is already in the list
		if( hr == Result(ResultCode::SUCCESS) )
		{
			if( pService->GetClusterMembership() <= ClusterMembership::Slave )
			{
				Assert(pTblItem->m_ListNode.pNext == nullptr);
				svrChk( m_ServiceList.FindPrevNode( pServerEntity->GetServerUpTime().time_since_epoch().count(), pPrevNode ) );

				Assert(pPrevNode != &pTblItem->m_ListNode );

				Assert(pPrevNode->pNext != &pTblItem->m_ListNode );

				svrChk( m_ServiceList.Insert( pPrevNode, pServerEntity->GetServerUpTime().time_since_epoch().count(), &pTblItem->m_ListNode ) );
			}
		}
		svrChk(hr);

	Proc_End:

		return hr;
	}

	// Get a service form the ring
	Result LoadbalanceClusterServiceEntity::GetService( ServerServiceInformation* &pService )
	{
		Result hr = ResultCode::SUCCESS;
		ServiceTableItem *pTblItem = nullptr;
		MapItemConverter<ServiceTableItem,TableItemType,&ServiceTableItem::m_ListNode> converter;

		pService = nullptr;

		if( m_pCurrentQueryService == nullptr )
		{
			auto itService = m_ServiceList.begin();
			if (!itService.IsValid())
				return ResultCode::SVR_SERVICE_FAILED;
			m_pCurrentQueryService = converter(&*itService);
		}

		pTblItem = (ServiceTableItem*)m_pCurrentQueryService;

		while( pTblItem != nullptr )
		{
			pTblItem = converter(pTblItem->m_ListNode.pNext);

			// circulate the list until meet itself
			if( pTblItem == nullptr )
			{
				auto itService = m_ServiceList.begin();
				if (!itService.IsValid())
					return ResultCode::SVR_SERVICE_FAILED;
				pTblItem = converter(&*itService);
			}

			// We don't have any service now
			if( pTblItem == nullptr )
				break;

			// If we circulate once
			if( pTblItem == m_pCurrentQueryService )
			{
				// Can't find new one
				// If this is not available service set to nullptr
				if( !pTblItem->IsServiceAvailable() || pTblItem->GetClusterMembership() > ClusterMembership::Slave )
				{
					pTblItem = nullptr;
				}
				break;
			}
			else if( pTblItem->IsServiceAvailable() && pTblItem->GetClusterMembership() <= ClusterMembership::Slave )
			{
				// we found new one
				break;
			}
		}

		if( pTblItem != nullptr )
			m_pCurrentQueryService = pTblItem;

		pService = pTblItem;

	//Proc_End:

		return pService ? hr : ResultCode::FAIL;
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



