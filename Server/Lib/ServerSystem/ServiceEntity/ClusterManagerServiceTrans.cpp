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
#include "Common/HRESCommon.h"
#include "Common/HRESGame.h"
#include "Common/HRESLogin.h"
#include "Common/MemoryPool.h"
#include "Common/BrSvrTypes.h"

#include "Protocol/Message/ClusterServerMsgClass.h"
#include "Protocol/Policy/ClusterServerIPolicy.h"

#include "ServerSystem/ServerService/ClusterServerService.h"

#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServiceEntity/ClusterManagerServiceTrans.h"
#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"


//BR_MEMORYPOOL_IMPLEMENT(Svr::ClusterManagerInitializationTrans);
BR_MEMORYPOOL_IMPLEMENT(Svr::ClusterGetMemberListTrans);
BR_MEMORYPOOL_IMPLEMENT(Svr::JoinClusterTrans);
BR_MEMORYPOOL_IMPLEMENT(Svr::JoinClusterTransForEntityServer);
BR_MEMORYPOOL_IMPLEMENT(Svr::ClusterUpdateStatusTrans);
BR_MEMORYPOOL_IMPLEMENT(Svr::ClusterUpdateWorkloadTrans);
BR_MEMORYPOOL_IMPLEMENT(Svr::GetLowestWorkloadClusterMemberTrans);
BR_MEMORYPOOL_IMPLEMENT(Svr::ClusterNewServerServiceJoinedC2SEvtTrans);
BR_MEMORYPOOL_IMPLEMENT(Svr::ClusterNewServerServiceJoinedC2SEvtEntityTrans);
BR_MEMORYPOOL_IMPLEMENT(Svr::SyncClusterServiceTrans);




namespace BR {
namespace Svr {


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Cluster service transactions
	//


	// Start Transaction
	HRESULT ClusterGetMemberListTrans::StartTransaction()
	{
		HRESULT hr = S_OK;
		ClusteredServiceEntity *pClusterService = nullptr;

		svrChk(super::StartTransaction() );

		// Get cluster watcher for this server
		if( SUCCEEDED(GetMyOwner()->GetClusterServiceEntity( GetClusterID(), pClusterService )) )
		{
			svrChk( m_MemberList.Reserve( pClusterService->GetNumberOfServices() ) );

			pClusterService->ForEach( [&]( ServerServiceInformation* pService ) 
			{
				auto &connectionInfo = pService->GetServerEntity()->GetConnection()->GetConnectionInfo();
				Assert(connectionInfo.RemoteClass != NetClass::Unknown);

				ServiceInformation serviceInfo( 
					pService->GetEntityUID(), pService->GetClusterMembership(), pService->GetServiceStatus(), 
					connectionInfo.RemoteClass, connectionInfo.Remote, pService->GetServerEntity()->GetServerUpTime(), pService->GetWorkload() );
				m_MemberList.push_back( serviceInfo );
			});
		}


	Proc_End:

		CloseTransaction( hr );

		return hr;
	}

	// Start Transaction
	HRESULT JoinClusterTrans::StartTransaction()
	{
		HRESULT hr = S_OK;
		ClusteredServiceEntity *pServiceEntity = nullptr;
		ServerServiceInformation *pRequestedService = nullptr;
		ClusterMembership memberShip = GetClusterMembership();
		ServerEntity *pSenderEntity = nullptr;

		svrChk(super::StartTransaction() );

		Trace::Flush();

		if( FAILED(GetServerComponent<ClusterManagerServiceEntity>()->GetClusterServiceEntity( GetClusterID(), pServiceEntity )) )
		{
			if( GetSender().GetServerID() == GetMyServerID() )
			{
				SharedPointerT<Entity> pEntity;
				svrChk( GetServerComponent<EntityManager>()->FindEntity( GetSender().GetEntityID(), pEntity ) );
				svrChkPtr( pServiceEntity = BR_DYNAMIC_CAST(ClusteredServiceEntity*,(Entity*)pEntity) );
			}
			else
			{
				svrErrClose(E_SVR_INVALID_SERVERID);
			}
		}

		// If no master yet
		if( pServiceEntity->GetMasterUID() == 0 )
		{
			// And it can be a master
			if( memberShip == ClusterMembership::Slave )
				memberShip = ClusterMembership::Master; // make it a master
		}
		else // If there is a master
		{
			// and it's a master 
			if( memberShip == ClusterMembership::Master )
				memberShip = ClusterMembership::Slave; // make it a slave
		}

		svrChk( GetServerComponent<ServerEntityManager>()->GetOrRegisterServer( GetSender().GetServerID(), GetSenderNetClass(), GetSenderAddress(), pSenderEntity ) );

		svrChk( pServiceEntity->NewServerService( GetSender(), pSenderEntity, memberShip, ServiceStatus::Online, pRequestedService ) );

		svrChk( m_MemberList.Reserve( GetMyOwner()->GetNumberOfServices() - 1 ) );

		// return back the service list
		pServiceEntity->ForEach( [&]( ServerServiceInformation* pService ) 
		{
			ServiceInformation serviceInfo;
			pService->GetServiceInformation( serviceInfo );

			Assert(serviceInfo.ServerClass != NetClass::Unknown);

			m_MemberList.push_back( serviceInfo );

			if( pService->GetEntityUID() != GetSender() )
			{
				Assert(pService->GetClusterID() == GetClusterID());
				pService->GetService<ClusterServerService>()->NewServerServiceJoinedC2SEvt( GetOwnerEntity()->GetEntityID(), 0, GetSender(), GetSenderNetClass(), GetSenderAddress(), GetClusterID(), GetClusterType(), memberShip );
			}
		});


	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	



	// Start Transaction
	HRESULT JoinClusterTransForEntityServer::StartTransaction()
	{
		HRESULT hr = S_OK;
		ClusteredServiceEntity *pServiceEntity = nullptr;
		ServerServiceInformation *pRequestedService = nullptr;
		ClusterMembership memberShip = GetClusterMembership();
		ServerEntity *pSenderEntity = nullptr;

		svrChk( super::StartTransaction() );

		svrAssert( GetMyOwner() == GetServerComponent<ClusterManagerServiceEntity>() );
		if( FAILED(GetMyOwner()->GetClusterServiceEntity( GetClusterID(), pServiceEntity )) )
		{
			if( GetSender().GetServerID() == GetMyServerID() )
			{
				SharedPointerT<Entity> pEntity;
				svrChk( GetServerComponent<EntityManager>()->FindEntity( GetSender().GetEntityID(), pEntity ) );
				svrChkPtr(pServiceEntity = BR_DYNAMIC_CAST(ClusteredServiceEntity*, (Entity*)pEntity));
			}
			else
			{
				svrChk( GetServerComponent<ClusterManagerServiceEntity>()->CreateWatcherForCluster( GetClusterID(), GetClusterType(), pServiceEntity ) );
			}
		}

		// If no master yet
		if( pServiceEntity->GetMasterUID() == 0 )
		{
			// And it can be a master
			if( memberShip == ClusterMembership::Slave )
				memberShip = ClusterMembership::Master; // make it as a master
		}
		else if(pServiceEntity->GetMasterUID() != GetSender())// If there is a master
		{
			// and it's a master 
			if( memberShip == ClusterMembership::Master )
				memberShip = ClusterMembership::Slave; // make it as a slave
		}

		svrChk( GetServerComponent<ServerEntityManager>()->GetOrRegisterServer( GetSender().GetServerID(), GetSenderNetClass(), GetSenderAddress(), pSenderEntity ) );

		svrChk( pServiceEntity->NewServerService( GetSender(), pSenderEntity, memberShip, ServiceStatus::Online, pRequestedService ) );

		svrChk( m_MemberList.Reserve( GetMyOwner()->GetNumberOfServices() - 1 ) );

		// return back the service list
		pServiceEntity->ForEach( [&]( ServerServiceInformation* pService ) 
		{
			ServiceInformation serviceInfo;

			pService->GetServiceInformation( serviceInfo );
			Assert(serviceInfo.ServerClass != NetClass::Unknown);

			m_MemberList.push_back( serviceInfo );

			if( pService->GetEntityUID() != GetSender() )
			{
				Assert(pService->GetClusterID() == GetClusterID());
				pService->GetService<ClusterServerService>()->NewServerServiceJoinedC2SEvt( GetOwnerEntity()->GetEntityID(), 0, GetSender(), GetSenderNetClass(), GetSenderAddress(), GetClusterID(), GetClusterType(), memberShip );
			}

		});

		// This message comes form different cluster so replication between cluster managers must be done my manually
		GetMyOwner()->ForEachNonWatcher( [&]( Svr::ServerServiceInformation *pServiceInfo )
		{
			pServiceInfo->GetService<ClusterServerService>()->NewServerServiceJoinedC2SEvt( GetOwnerEntityUID().GetEntityID(), 0, GetSender(), GetSenderNetClass(), GetSenderAddress(), GetClusterID(), GetClusterType(), memberShip );
		});


	Proc_End:

		CloseTransaction( hr );

		return hr;
	}


	// Start Transaction
	HRESULT ClusterUpdateStatusTrans::StartTransaction()
	{
		HRESULT hr = S_OK;
		ServerServiceInformation *pService = nullptr;
		Svr::ClusteredServiceEntity *pServiceEntity = nullptr;

		svrChk( super::StartTransaction() );

		if( SUCCEEDED(GetServerComponent<ClusterManagerServiceEntity>()->GetClusterServiceEntity( GetClusterID(), pServiceEntity )) )
		{
			if( SUCCEEDED(pServiceEntity->FindService( GetSender(), pService )) )
			{
				pService->SetServiceStatus( GetMemberStatus() );
			}
			else
			{
				svrTrace( Svr::TRC_CLUSTER, "Ignoring update status for ClusterID:%0%, Entity:%1% to %2%, The entity seems not ready.", GetClusterID(), GetSender(), GetMemberStatus() );
			}
		}
		else
		{
			svrTrace( Svr::TRC_CLUSTER, "Ignoring update status for ClusterID:%0%, Entity:%1% to %2%", GetClusterID(), GetSender(), GetMemberStatus() );
		}

	Proc_End:

		if( FAILED(hr) )
		{
			svrTrace( Svr::TRC_CLUSTER, "Update state is failed ClusterID:%0%, Entity:%1% to %2%", GetClusterID(), GetSender(), GetMemberStatus() );
			CloseTransaction( hr );
		}
		else
		{
			svrTrace( Svr::TRC_CLUSTER, "State is updated ClusterID:%0%, Entity:%1% to %2%", GetClusterID(), GetSender(), GetMemberStatus() );
			CloseTransaction( hr );
		}

		return hr;
	}


	// Start Transaction
	HRESULT ClusterUpdateWorkloadTrans::StartTransaction()
	{
		HRESULT hr = S_OK;
		ClusteredServiceEntity* pServiceEntity = nullptr;
		ServerServiceInformation *pUpdatedService = nullptr;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetClusterID() == GetClusterID() )
		{
			pServiceEntity = GetMyOwner();
		}
		else
		{
			if( FAILED(GetServerComponent<ClusterManagerServiceEntity>()->GetClusterServiceEntity( GetClusterID(), pServiceEntity )) )
			{
				svrTrace( Svr::TRC_CLUSTER, "Ignoring workload update of an unregistered cluster ClusterID:%0% Sender:%1%", GetClusterID(), GetSender() );
				CloseTransaction(hr);
				goto Proc_End;
			}
		}

		hr = pServiceEntity->FindService(GetSender(), pUpdatedService);
		if (FAILED(hr))
		{
			goto Proc_End;
		}

		pUpdatedService->SetWorkload( GetWorkload() );

		// Manual replication is needed while this is not a usual transaction
		// Hop count 1 is the maximum number with replica clustring model
		if( GetRouteHopCount() >= 1 )
			goto Proc_End;

		pServiceEntity->ForEach( [&]( ServerServiceInformation* pService ) 
		{
			if( pService->GetEntityUID() == pUpdatedService->GetEntityUID() )
				return;

			TossMessageToTarget( pService );
		});


	Proc_End:

		if( FAILED(hr) )
		{
			svrTrace( Trace::TRC_ERROR, "Failed to update workload of unregistered cluster ClusterID:%0%, Sender:%1%", GetClusterID(), GetSender() );
			CloseTransaction( hr );
		}
		else
			CloseTransaction( hr );

		return hr;
	}

	
	
	// Start Transaction
	HRESULT GetLowestWorkloadClusterMemberTrans::StartTransaction()
	{
		HRESULT hr = S_OK;
		ServerServiceInformation *pLowestService = nullptr;

		svrChk( super::StartTransaction() );

		GetMyOwner()->ForEachNonWatcher( [&](ServerServiceInformation* pService)
		{
			if( !pService->IsServiceAvailable() ) return;

			if( pLowestService == nullptr || pLowestService->GetWorkload() > pService->GetWorkload() )
				pLowestService = pService;

			return;
		});

		if( pLowestService == nullptr )
			svrErrClose(E_SVR_CLUSTER_NOTREADY);

		pLowestService->GetServiceInformation(m_LowestMemberInfo);

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}

	
	
	// Start Transaction
	HRESULT ClusterNewServerServiceJoinedC2SEvtEntityTrans::StartTransaction()
	{
		HRESULT hr = S_OK;
		ServerServiceInformation *pRequestedService = nullptr;
		ServerEntity *pSenderEntity = nullptr;
		ClusteredServiceEntity *pServiceEntity = nullptr;

		Assert(GetJoinedServiceNetClass() != NetClass::Unknown);
		Assert(GetJoinedServiceUID().GetServerID() < 1000);

		svrChk( super::StartTransaction() );

		// This transaction is used for two cases. so it's better to query cluster service again
		if( GetMyOwner()->GetClusterID() == GetClusterID() )
		{
			pServiceEntity = GetMyOwner();
		}
		else
		{
			if( FAILED( GetServerComponent<ClusterManagerServiceEntity>()->GetClusterServiceEntity( GetClusterID(), pServiceEntity ) ) )
			{
				svrTrace(Svr::TRC_CLUSTER, "Ignoring to add new server service cluster member ClusterID:%0%, Joined:%1%", GetClusterID(), GetJoinedServiceUID() );
				goto Proc_End;
			}
		}
		
		svrChk(GetServerComponent<ServerEntityManager>()->GetOrRegisterServer(GetJoinedServiceUID().GetServerID(), GetJoinedServiceNetClass(), GetJoinedServiceAddress(), pSenderEntity));

		svrChk( pServiceEntity->NewServerService( GetJoinedServiceUID(), pSenderEntity, GetJoinedServiceMembership(), ServiceStatus::Online, pRequestedService ) );


	Proc_End:

		if(FAILED(hr))
		{
			svrTrace(Trace::TRC_ERROR, "Failed to add new server service cluster member ClusterID:%0%, Joined:%1%", GetClusterID(), GetJoinedServiceUID() );
		}

		CloseTransaction( hr );

		return hr;
	}



	// Start Transaction
	HRESULT ClusterNewServerServiceJoinedC2SEvtTrans::StartTransaction()
	{
		HRESULT hr = S_OK;
		ServerServiceInformation *pRequestedService = nullptr;
		ServerEntity *pSenderEntity = nullptr;
		ClusteredServiceEntity *pServiceEntity = nullptr;

		Assert(GetJoinedServiceNetClass() != NetClass::Unknown);
		Assert(GetJoinedServiceUID().GetServerID() < 1000);

		svrChk(super::StartTransaction());

		if (GetClusterID() != ClusterID::ClusterManager || GetJoinedServiceNetClass() == NetClass::Entity)
		{
			// This transaction is used for two cases. so it's better to query cluster service again
			if (GetMyOwner()->GetClusterID() == GetClusterID())
			{
				pServiceEntity = GetMyOwner();
			}
			else
			{
				if (FAILED(GetServerComponent<ClusterManagerServiceEntity>()->GetClusterServiceEntity(GetClusterID(), pServiceEntity)))
				{
					svrTrace(Svr::TRC_CLUSTER, "Ignoring to add new server service cluster member ClusterID:%0%, Joined:%1%", GetClusterID(), GetJoinedServiceUID());
					goto Proc_End;
				}
			}

			svrChk(GetServerComponent<ServerEntityManager>()->GetOrRegisterServer(GetJoinedServiceUID().GetServerID(), GetJoinedServiceNetClass(), GetJoinedServiceAddress(), pSenderEntity));

			svrChk(pServiceEntity->NewServerService(GetJoinedServiceUID(), pSenderEntity, GetJoinedServiceMembership(), ServiceStatus::Online, pRequestedService));
		}


	Proc_End:

		if (FAILED(hr))
		{
			svrTrace(Trace::TRC_ERROR, "Failed to add new server service cluster member ClusterID:%0%, Joined:%1%", GetClusterID(), GetJoinedServiceUID());
		}

		CloseTransaction(hr);

		return hr;
	}



	// Start Transaction
	HRESULT SyncClusterServiceTrans::StartTransaction()
	{
		HRESULT hr = S_OK;
		ClusteredServiceEntity *pServiceEntity = nullptr;
		ServerEntity *pServerEntity = nullptr;
		ServerServiceInformation* pNewService = nullptr;
		bool bAddStatusWatcher;

		// This should not be failed
		if( FAILED(GetMyOwner()->GetClusterServiceEntity( GetClusterID(), pServiceEntity )) )
		{
			svrTrace(Svr::TRC_CLUSTER, "Ignoring unregistered cluster service sync: ClusterID:%0%", GetClusterID() );
			//svrErr(E_SVR_INVALID_CLUSTERID);
			goto Proc_End;
		}

		bAddStatusWatcher = pServiceEntity->GetClusterMembership() != ClusterMembership::StatusWatcher;

		for( UINT iMember = 0; iMember < GetMemberList().GetSize(); iMember++ )
		{
			const ServiceInformation& serviceInfo = GetMemberList()[iMember];
			
			if( !bAddStatusWatcher && serviceInfo.Membership == ClusterMembership::StatusWatcher )
				continue;

			svrChk( GetServerComponent<ServerEntityManager>()->GetOrRegisterServer( serviceInfo.UID.GetServerID(), serviceInfo.ServerClass, serviceInfo.ServerAddress, pServerEntity ) );

			pNewService = nullptr;
			svrChk( pServiceEntity->NewServerService( serviceInfo.UID, pServerEntity, serviceInfo.Membership, serviceInfo.Status, pNewService ) );
		}


	Proc_End:

		CloseTransaction( hr );

		return hr;
	}


};// namespace GameServer 
};// namespace BR 

