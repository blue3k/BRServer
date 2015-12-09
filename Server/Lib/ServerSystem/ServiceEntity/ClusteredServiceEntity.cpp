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
#include "Net/NetDef.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/ServiceEntity/ClusteredServiceEntity.h"
#include "ServerSystem/ServiceEntity/ClusterServiceTrans.h"
#include "ServerSystem/ServerService/ClusterServerService.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/EventTask.h"

#include "Protocol/Policy/ClusterServerNetPolicy.h"


BR_MEMORYPOOL_IMPLEMENT(Svr::ClusteredServiceEntity::ServiceTableItem);

namespace BR {
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
		, m_MyServiceInfo(nullptr)
		, m_ServerEntity(pServerEntity)
		, m_IsInVoting(false)
		, m_Initialized(false)
	{
	}

	ClusteredServiceEntity::~ClusteredServiceEntity()
	{
	}

	// Check connectivity of two service
	bool ClusteredServiceEntity::CheckConnectivity( NetClass netClass1, ClusterMembership membership1, NetClass netClass2, ClusterMembership membership2 )
	{
		if( netClass1 == NetClass::Entity || netClass2 == NetClass::Entity
			|| membership1 != ClusterMembership::StatusWatcher || membership2 != ClusterMembership::StatusWatcher )
		{
			return true;
		}
		return false;
	}
	
	// Check connectivity of two service
	bool ClusteredServiceEntity::CheckConnectivity( NetClass netClass2, ClusterMembership membership2 )
	{
		if( BrServer::GetInstance()->GetNetClass() == NetClass::Entity || netClass2 == NetClass::Entity
			|| GetClusterMembership() != ClusterMembership::StatusWatcher || membership2 != ClusterMembership::StatusWatcher )
		{
			return true;
		}
		return false;
	}
	

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
	HRESULT ClusteredServiceEntity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_OK;
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

	HRESULT ClusteredServiceEntity::StartInitializeTransaction()
	{
		HRESULT hr = S_OK;
		ClusterInitializationTrans *pInitTransaction = nullptr;
		Transaction* pTrans = nullptr;

		// Push initialization transaction
		svrMem( pInitTransaction = new ClusterInitializationTrans );
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
	HRESULT ClusteredServiceEntity::ClearEntity()
	{
		HRESULT hr = S_OK;

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


	HRESULT ClusteredServiceEntity::TickUpdate(TimerAction *pAction)
	{
		return ServiceEntity::TickUpdate(pAction);
	}

	HRESULT ClusteredServiceEntity::ProcessTransaction(Transaction* &pTrans)
	{
		return ServiceEntity::ProcessTransaction(pTrans);
	}



	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	// Change workload
	HRESULT ClusteredServiceEntity::SetWorkload( UINT workload )
	{
		HRESULT hr = S_OK;
		ServerServiceInformation *pServiceInfo = nullptr;
		ClusterServerService *pService = nullptr;

		if( m_Workload == workload )
			return S_OK;

		m_Workload = workload;

		svrChk( GetServerComponent<ClusterManagerServiceEntity>()->FindRandomService( pServiceInfo ) );
		//svrChk( FindRandomService( pServiceInfo ) );
		svrChkPtr( pService = pServiceInfo->GetService<ClusterServerService>() );
		svrChk( pService->ClusterUpdateWorkloadC2SEvt( GetEntityUID(), 0, GetEntityUID(), GetClusterID(), m_Workload ) );

	Proc_End:


		return hr;
	}


	HRESULT ClusteredServiceEntity::SetServiceStatus( ServiceStatus newStatus )
	{
		HRESULT hr = S_OK;
		ServerServiceInformation *pServiceInfo = nullptr;
		ClusterServerService *pService = nullptr;

		m_ServiceStatus = newStatus;

		if( m_MasterUID.UID == 0 )
		{
			return S_FALSE;
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
	HRESULT ClusteredServiceEntity::NewServerService( EntityUID entityUID, ServerEntity *pServerEntity, ClusterMembership membership, ServiceStatus status, ServerServiceInformation* &pService )
	{
		HRESULT hr = S_OK;
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
		Assert(pServerEntity->GetRemoteClass() != NetClass::Unknown);

		// Create new one
		svrMem( pNewService = new ServiceTableItem( GetClusterID(), pServerEntity, membership ) );

		pNewService->SetEntityUID( entityUID );
		pNewService->SetServiceStatus( status );

		if( membership == ClusterMembership::StatusWatcher )
		{
			if( FAILED(m_WatcherUIDMap.insert((ULONGLONG)pNewService->GetEntityUID(), pNewService)) )
			{
				svrChk(m_WatcherUIDMap.find((UINT64)entityUID, itFound));

				ActionOnAlreadyExist(*itFound);

				hr = S_FALSE;
				goto Proc_End;
			}
		}
		else
		{
			if( FAILED(m_ServiceEntityUIDMap.insert((ULONGLONG)pNewService->GetEntityUID(), pNewService)) )
			{
				svrChk(m_ServiceEntityUIDMap.find((UINT64)entityUID, itFound));

				ActionOnAlreadyExist(*itFound);

				hr = S_FALSE;
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
	HRESULT ClusteredServiceEntity::FindService( EntityUID entityUID, ServerServiceInformation* &pService )
	{
		HRESULT hr = S_OK;
		ServiceEntityUIDMap::iterator itFound;

		if (FAILED(m_ServiceEntityUIDMap.find((UINT64)entityUID, itFound)))
		{
			if (FAILED(m_WatcherUIDMap.find((UINT64)entityUID, itFound)))
				return E_FAIL;
		}
		pService = *itFound;

	//Proc_End:

		return hr;
	}
	
	// Find random service, maybe lowest workload service
	HRESULT ClusteredServiceEntity::FindRandomService( ServerServiceInformation* &pService )
	{
		//HRESULT hr = S_OK;
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

		return pBestService != nullptr ? S_OK : E_FAIL;
	}

	// Foreach service
	HRESULT ClusteredServiceEntity::ForEach( std::function<void(ServerServiceInformation*)> func )
	{
		HRESULT hr = S_OK;
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

	HRESULT ClusteredServiceEntity::ForEachNonWatcher( std::function<void(ServerServiceInformation*)> func )
	{
		HRESULT hr = S_OK;
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
	HRESULT ClusteredServiceEntity::SetMaster( EntityUID entityUID )
	{
		HRESULT hr = S_OK;
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

	HRESULT ClusteredServiceEntity::AssignMaster( EntityUID entityUID )
	{
		HRESULT hr = S_OK;

		svrChk( SetMaster( entityUID ) );

		// If i'm the master than broadcast the result
		// broadcast new master assignment
		ForEach( [&](ServerServiceInformation *pService) 
		{
			pService->GetConnection()->GetPolicy<Policy::ISvrPolicyClusterServer>()->ClusterMasterAssignedS2CEvt( RouteContext( GetEntityUID(), pService->GetEntityUID() ), 0, GetEntityUID(), GetClusterID(), entityUID );
		});


	Proc_End:

		return hr;
	}

	
	// Sync data to target 
	HRESULT ClusteredServiceEntity::SyncDataToTarget( EntityUID entityUID )
	{
		HRESULT hr = S_OK;
		//ServerServiceInformation* pNewMaster = nullptr;

		//svrChk( FindService( entityUID, pNewMaster ) );

		//// broadcast new master assignment
		//ForEach( [&](ServerServiceInformation *pService) 
		//{
		//	pService->GetConnection()->GetPolicy<Policy::ISvrPolicyClusterServer>()->ClusterMasterAssignedS2CEvt( RouteContext( GetEntityUID(), pService->GetEntityUID() ),1, GetClusterID(), entityUID );
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

	HRESULT ReplicaClusterServiceEntity::UpdateOnMasterManager()
	{
		HRESULT hr = S_OK;
		ServiceEntityUIDMap::iterator itService;

		
		// Vote for new master if the master isn't available
		if (FAILED(m_ServiceEntityUIDMap.find((UINT64)GetMasterUID(), itService))
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

			auto itIDMap = m_ServiceEntityUIDMap.begin();
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
				svrTrace( Svr::TRC_CLUSTER, "New Master is selected Cluster:%0% Entity:%1%", GetClusterID(), pCandidate->GetEntityUID() );
				svrChk( AssignMaster( pCandidate->GetEntityUID() ) );
			}
		}

	Proc_End:

		return hr;
	}

	// Register message handler for this component
	HRESULT ReplicaClusterServiceEntity::RegisterServiceMessageHandler( ServerEntity *pServerEntity )
	{
		ClusteredServiceEntity::RegisterServiceMessageHandler( pServerEntity );

		return S_OK;
	}

	HRESULT ReplicaClusterServiceEntity::TickUpdate(Svr::TimerAction *pAction)
	{
		HRESULT hr = S_OK;

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
	HRESULT RingClusterServiceEntity::NewServerService( EntityUID entityUID, ServerEntity *pServerEntity, ClusterMembership membership, ServiceStatus status, ServerServiceInformation* &pService )
	{
		HRESULT hr = S_OK;
		ServiceTableItem *pTblItem = nullptr;
		OrderedServiceList::Node *pPrevNode = nullptr;

		// This need to be thread safe
		MutexScopeLock scopelock(m_ListLock);

		HRESULT hrRes = ClusteredServiceEntity::NewServerService(entityUID, pServerEntity, membership, status, pService);

		svrChkPtr(pService);
		pTblItem = (ServiceTableItem*)pService;

		// When the return value is S_FALSE, it means this service is already in the list
		// Remove it if the key value isn't same
		if( hrRes == S_FALSE && pTblItem->m_ListNode.Key != pServerEntity->GetServerUpTime().time_since_epoch().count() )
		{
			// I hope this not happened here
			Assert(pTblItem->GetServerEntity() == pServerEntity);
			svrChk( m_ServiceList.FindPrevNode( pTblItem->m_ListNode.Key, pPrevNode ) );
			svrChk( m_ServiceList.FindAndRemove( pPrevNode, &pTblItem->m_ListNode ) );
			hrRes = S_OK;
		}

		if( SUCCEEDED(hrRes) )
		{
			svrChk( m_ServiceList.FindPrevNode( pServerEntity->GetServerUpTime().time_since_epoch().count(), pPrevNode ) );

			// we don't have to update it again
			if( hrRes != S_FALSE )
			{
				Assert(pPrevNode != &pTblItem->m_ListNode );
				Assert(pTblItem->m_ListNode.pNext == nullptr);
				// If the table already exist in the list, and it just updated hrRes would have S_FALSE
				Assert(pPrevNode->pNext != &pTblItem->m_ListNode);

				svrChk( m_ServiceList.Insert( pPrevNode, pServerEntity->GetServerUpTime().time_since_epoch().count(), &pTblItem->m_ListNode ) );
			}
		}
		svrChk(hr);

	Proc_End:

		return hr;
	}

	// Get next ring token
	HRESULT RingClusterServiceEntity::GetNextRing( ServerServiceInformation* pService, ServerServiceInformation* &pNextService )
	{
		HRESULT hr = S_OK;
		ServiceTableItem *pTblItem = nullptr;
		Indexing::MapItemConverter<ServiceTableItem,TableItemType,&ServiceTableItem::m_ListNode> converter;

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

		return pNextService ? hr : E_FAIL;
	}

	// Get a service form the ring
	HRESULT RingClusterServiceEntity::GetService( ServerServiceInformation* &pService )
	{
		HRESULT hr = S_OK;
		ServiceTableItem *pTblItem = nullptr;
		Indexing::MapItemConverter<ServiceTableItem,TableItemType,&ServiceTableItem::m_ListNode> converter;

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
		svrTrace( Svr::TRC_DBGINFO, "Service Item: ClusterID:%0%, EntityUID:%1%, Membership:%2%", GetClusterID(), GetEntityUID(), GetClusterMembership() );
		for( auto itService = m_ServiceList.begin(); itService.IsValid(); ++itService )
		{
			ServiceTableItem *pServiceItem = converter(&*itService);
			svrTrace( Svr::TRC_DBGINFO, "Service Item: ClusterID:%0%, EntityUID:%1%, Membership:%2%", pServiceItem->GetClusterID(), pServiceItem->GetEntityUID(), pServiceItem->GetClusterMembership() );
		}
	}
#endif

		return pService ? hr : E_FAIL;
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
	UINT ShardedClusterServiceEntity::KeyHash( UINT64 key )
	{
		return (UINT)std::hash<ULONGLONG>()(key);
	}

	// Get Service shard by key
	HRESULT ShardedClusterServiceEntity::GetShard( UINT64 key, ServerServiceInformation* &pService )
	{
		HRESULT hr = S_OK;
		UINT hashedKey = KeyHash(key);
		UINT serviceIndex = -1;
		StaticArray<ServerServiceInformation*,50> services;

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
			serviceIndex = hashedKey % services.size();
		}

		if( serviceIndex < 0 || serviceIndex >= services.size() )
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
	HRESULT LoadbalanceClusterServiceEntity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_OK;

		svrChk(ClusteredServiceEntity::InitializeEntity(newEntityID) );

		m_WorkloadCheckTimer.SetTimer( DurationMS(Const::WORKLOAD_UPDATE_TIME) );

	Proc_End:

		return hr;
	}


	// Override so that we can make ring linked list
	HRESULT LoadbalanceClusterServiceEntity::NewServerService( EntityUID entityUID, ServerEntity *pServerEntity, ClusterMembership membership, ServiceStatus status, ServerServiceInformation* &pService )
	{
		HRESULT hr = S_OK;
		ServiceTableItem *pTblItem = nullptr;
		OrderedServiceList::Node *pPrevNode = nullptr;

		hr = ClusteredServiceEntity::NewServerService(entityUID, pServerEntity, membership, status, pService);

		pTblItem = (ServiceTableItem*)pService;

		// When the return value is S_FALSE, it means this service is already in the list
		if( hr == S_OK )
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
	HRESULT LoadbalanceClusterServiceEntity::GetService( ServerServiceInformation* &pService )
	{
		HRESULT hr = S_OK;
		ServiceTableItem *pTblItem = nullptr;
		Indexing::MapItemConverter<ServiceTableItem,TableItemType,&ServiceTableItem::m_ListNode> converter;

		if( m_pCurrentQueryService == nullptr )
		{
			m_pCurrentQueryService = converter(&*m_ServiceList.begin());
		}

		pTblItem = (ServiceTableItem*)m_pCurrentQueryService;

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

		return pService ? hr : E_FAIL;
	}


	HRESULT LoadbalanceClusterServiceEntity::TickUpdate(Svr::TimerAction *pAction)
	{
		HRESULT hr = S_OK;

		svrChk(ClusteredServiceEntity::TickUpdate(pAction) );

		if( m_WorkloadCheckTimer.CheckTimer() )
		{
			m_WorkloadCheckTimer.SetTimer( DurationMS(Const::WORKLOAD_UPDATE_TIME) );

			SetWorkload( (UINT)m_LocalWorkload.load(std::memory_order_relaxed) );
		}

	Proc_End:

		return hr;
	}


}; // namespace Svr
}; // namespace BR



