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
#include "ResultCode/SFResultCodeCommon.h"
#include "ResultCode/SFResultCodeGame.h"
#include "ResultCode/SFResultCodeLogin.h"
#include "Memory/MemoryPool.h"
#include "Types/BrSvrTypes.h"

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


//SF_MEMORYPOOL_IMPLEMENT(BR::Svr::ClusterManagerInitializationTrans);
SF_MEMORYPOOL_IMPLEMENT(BR::Svr::ClusterGetMemberListTrans);
SF_MEMORYPOOL_IMPLEMENT(BR::Svr::JoinClusterTrans);
SF_MEMORYPOOL_IMPLEMENT(BR::Svr::JoinClusterTransForEntityServer);
SF_MEMORYPOOL_IMPLEMENT(BR::Svr::SyncClusterServiceTrans);




namespace SF {
namespace Svr {


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Cluster service transactions
	//


	// Start Transaction
	Result ClusterGetMemberListTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		ClusteredServiceEntity *pClusterService = nullptr;

		svrChk(super::StartTransaction() );

		// Get cluster watcher for this server
		if( (GetMyOwner()->GetClusterServiceEntity( GetClusterID(), pClusterService )) )
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
	Result JoinClusterTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		ClusteredServiceEntity *pServiceEntity = nullptr;
		ServerServiceInformation *pRequestedService = nullptr;
		ClusterMembership memberShip = GetClusterMembership();
		ServerEntity *pSenderEntity = nullptr;

		svrChk(super::StartTransaction() );

		Trace::Flush();

		if( !(GetServerComponent<ClusterManagerServiceEntity>()->GetClusterServiceEntity( GetClusterID(), pServiceEntity )) )
		{
			if( GetSender().GetServerID() == GetMyServerID() )
			{
				SharedPointerT<Entity> pEntity;
				svrChk( GetServerComponent<EntityManager>()->FindEntity( GetSender().GetEntityID(), pEntity ) );
				svrChkPtr( pServiceEntity = BR_DYNAMIC_CAST(ClusteredServiceEntity*,(Entity*)pEntity) );
			}
			else
			{
				svrErrClose(ResultCode::E_SVR_INVALID_SERVERID);
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
	Result JoinClusterTransForEntityServer::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		ClusteredServiceEntity *pServiceEntity = nullptr;
		ServerServiceInformation *pRequestedService = nullptr;
		ClusterMembership memberShip = GetClusterMembership();
		ServerEntity *pSenderEntity = nullptr;

		svrChk( super::StartTransaction() );

		svrAssert( GetMyOwner() == GetServerComponent<ClusterManagerServiceEntity>() );
		if( !(GetMyOwner()->GetClusterServiceEntity( GetClusterID(), pServiceEntity )) )
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
	Result SyncClusterServiceTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		ClusteredServiceEntity *pServiceEntity = nullptr;
		ServerEntity *pServerEntity = nullptr;
		ServerServiceInformation* pNewService = nullptr;
		bool bAddStatusWatcher;

		// This should not be failed
		if( !(GetMyOwner()->GetClusterServiceEntity( GetClusterID(), pServiceEntity )) )
		{
			svrTrace(Svr::TRC_CLUSTER, "Ignoring unregistered cluster service sync: ClusterID:{0}", GetClusterID() );
			//svrErr(ResultCode::E_SVR_INVALID_CLUSTERID);
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
};// namespace SF 

