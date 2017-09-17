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
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "ResultCode/SFResultCodeLogin.h"
#include "Memory/MemoryPool.h"
#include "Types/BrSvrTypes.h"

#include "Protocol/Message/ClusterServerMsgClass.h"
#include "Protocol/Policy/ClusterServerNetPolicy.h"

#include "Protocol/ServerService/ClusterServerService.h"

#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "Server/BrServer.h"
#include "ServerEntity/ServerEntityManager.h"
#include "Entity/EntityManager.h"
#include "ServiceEntity/ClusterManagerServiceTrans.h"
#include "ServiceEntity/ClusterManagerServiceEntity.h"


//SF_MEMORYPOOL_IMPLEMENT(SF::Svr::ClusterManagerInitializationTrans);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::ClusterGetMemberListTrans);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::JoinClusterTrans);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::JoinClusterTransForEntityServer);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::SyncClusterServiceTrans);




namespace SF {
namespace Svr {


	///////////////////////////////////////////////////////////////////////////////
	////
	////	Cluster service transactions
	////


	//// Start Transaction
	//Result ClusterGetMemberListTrans::StartTransaction()
	//{
	//	Result hr = ResultCode::SUCCESS;
	//	ClusterManagerServiceEntity::ClusterServiceInfo *pClusterInfo = nullptr;

	//	svrChk(super::StartTransaction() );



	//	// Get cluster watcher for this server
	//	if( (GetMyOwner()->GetClusterInfo( GetClusterID(), pClusterInfo)) )
	//	{
	//		svrChk( m_MemberList.Reserve(pClusterInfo->Services.size()) ) );

	//		for(auto pService : pClusterInfo->Services)
	//		{
	//			auto &remoteInfo = pService->GetServerEntity()->GetConnection()->GetRemoteInfo();
	//			Assert(remoteInfo.PeerClass != NetClass::Unknown);

	//			ServiceInformation serviceInfo( 
	//				pService->GetEntityUID(), pService->GetClusterMembership(), pService->GetServiceStatus(), 
	//				remoteInfo.PeerClass, remoteInfo.PeerAddress, pService->GetServerEntity()->GetServerUpTime(), pService->GetWorkload() );
	//			m_MemberList.push_back( serviceInfo );
	//		};
	//	}


	//Proc_End:

	//	CloseTransaction( hr );

	//	return hr;
	//}

	//// Start Transaction
	//Result JoinClusterTrans::StartTransaction()
	//{
	//	Result hr = ResultCode::SUCCESS;
	//	ClusteredServiceEntity *pServiceEntity = nullptr;
	//	ServerServiceInformation *pRequestedService = nullptr;
	//	ClusterMembership memberShip = GetClusterMembership();
	//	ServerEntity *pSenderEntity = nullptr;

	//	svrChk(super::StartTransaction() );

	//	Service::LogModule->Flush();

	//	if( !(Service::ClusterManager->GetClusterServiceEntity( GetClusterID(), pServiceEntity )) )
	//	{
	//		if( EntityUID(GetSender()).GetServerID() == GetMyServerID() )
	//		{
	//			SharedPointerT<Entity> pEntity;
	//			svrChk(Service::EntityTable->find(EntityUID(GetSender()).GetEntityID(), pEntity ) );
	//			svrChkPtr( pServiceEntity = dynamic_cast<ClusteredServiceEntity*>((Entity*)pEntity) );
	//		}
	//		else
	//		{
	//			svrErrClose(ResultCode::SVR_INVALID_SERVERID);
	//		}
	//	}

	//	// If no master yet
	//	if( pServiceEntity->GetMasterUID().UID == 0 )
	//	{
	//		// And it can be a master
	//		if( memberShip == ClusterMembership::Slave )
	//			memberShip = ClusterMembership::Master; // make it a master
	//	}
	//	else // If there is a master
	//	{
	//		// and it's a master 
	//		if( memberShip == ClusterMembership::Master )
	//			memberShip = ClusterMembership::Slave; // make it a slave
	//	}

	//	svrChk( Service::ServerEntityManager->GetOrRegisterServer(EntityUID(GetSender()).GetServerID(), GetSenderNetClass(), GetSenderAddress(), pSenderEntity ) );

	//	svrChk( pServiceEntity->NewServerService( GetSender(), pSenderEntity, memberShip, ServiceStatus::Online, pRequestedService ) );

	//	svrChk( m_MemberList.Reserve( GetMyOwner()->GetNumberOfServices() - 1 ) );

	//	// return back the service list
	//	pServiceEntity->ForEach( [&]( ServerServiceInformation* pService ) 
	//	{
	//		ServiceInformation serviceInfo;
	//		pService->GetServiceInformation( serviceInfo );

	//		Assert(serviceInfo.ServerClass != NetClass::Unknown);

	//		m_MemberList.push_back( serviceInfo );

	//		if( pService->GetEntityUID().UID != GetSender() )
	//		{
	//			Assert(pService->GetClusterID() == GetClusterID());
	//			pService->GetService<ClusterServerService>()->NewServerServiceJoinedC2SEvt( GetOwnerEntity()->GetEntityID(), 0, GetSender(), GetSenderNetClass(), GetSenderAddress(), GetClusterID(), GetClusterType(), memberShip );
	//		}
	//	});


	//Proc_End:

	//	CloseTransaction( hr );

	//	return hr;
	//}
	//



	//// Start Transaction
	//Result JoinClusterTransForEntityServer::StartTransaction()
	//{
	//	Result hr = ResultCode::SUCCESS;
	//	ClusteredServiceEntity *pServiceEntity = nullptr;
	//	ServerServiceInformation *pRequestedService = nullptr;
	//	ClusterMembership memberShip = GetClusterMembership();
	//	ServerEntity *pSenderEntity = nullptr;

	//	svrChk( super::StartTransaction() );

	//	svrAssert( GetMyOwner() == Service::ClusterManager );
	//	if( !(GetMyOwner()->GetClusterServiceEntity( GetClusterID(), pServiceEntity )) )
	//	{
	//		if(EntityUID(GetSender()).GetServerID() == GetMyServerID() )
	//		{
	//			SharedPointerT<Entity> pEntity;
	//			svrChk(Service::EntityTable->find(EntityUID(GetSender()).GetEntityID(), pEntity ) );
	//			svrChkPtr(pServiceEntity = dynamic_cast<ClusteredServiceEntity*>((Entity*)pEntity));
	//		}
	//		else
	//		{
	//			svrChk( Service::ClusterManager->CreateWatcherForCluster( GetClusterID(), GetClusterType(), pServiceEntity ) );
	//		}
	//	}

	//	// If no master yet
	//	if( pServiceEntity->GetMasterUID().UID == 0 )
	//	{
	//		// And it can be a master
	//		if( memberShip == ClusterMembership::Slave )
	//			memberShip = ClusterMembership::Master; // make it as a master
	//	}
	//	else if(pServiceEntity->GetMasterUID().UID != GetSender())// If there is a master
	//	{
	//		// and it's a master 
	//		if( memberShip == ClusterMembership::Master )
	//			memberShip = ClusterMembership::Slave; // make it as a slave
	//	}

	//	svrChk( Service::ServerEntityManager->GetOrRegisterServer(EntityUID(GetSender()).GetServerID(), GetSenderNetClass(), GetSenderAddress(), pSenderEntity ) );

	//	svrChk( pServiceEntity->NewServerService( GetSender(), pSenderEntity, memberShip, ServiceStatus::Online, pRequestedService ) );

	//	svrChk( m_MemberList.Reserve( GetMyOwner()->GetNumberOfServices() - 1 ) );

	//	// return back the service list
	//	pServiceEntity->ForEach( [&]( ServerServiceInformation* pService ) 
	//	{
	//		ServiceInformation serviceInfo;

	//		pService->GetServiceInformation( serviceInfo );
	//		Assert(serviceInfo.ServerClass != NetClass::Unknown);

	//		m_MemberList.push_back( serviceInfo );

	//		if( pService->GetEntityUID().UID != GetSender() )
	//		{
	//			Assert(pService->GetClusterID() == GetClusterID());
	//			pService->GetService<ClusterServerService>()->NewServerServiceJoinedC2SEvt( GetOwnerEntity()->GetEntityID(), 0, GetSender(), GetSenderNetClass(), GetSenderAddress(), GetClusterID(), GetClusterType(), memberShip );
	//		}

	//	});

	//	// This message comes form different cluster so replication between cluster managers must be done my manually
	//	GetMyOwner()->ForEachNonWatcher( [&]( Svr::ServerServiceInformation *pServiceInfo )
	//	{
	//		pServiceInfo->GetService<ClusterServerService>()->NewServerServiceJoinedC2SEvt( GetOwnerEntityUID().GetEntityID(), 0, GetSender(), GetSenderNetClass(), GetSenderAddress(), GetClusterID(), GetClusterType(), memberShip );
	//	});


	//Proc_End:

	//	CloseTransaction( hr );

	//	return hr;
	//}



	//// Start Transaction
	//Result SyncClusterServiceTrans::StartTransaction()
	//{
	//	Result hr = ResultCode::SUCCESS;
	//	ClusteredServiceEntity *pServiceEntity = nullptr;
	//	ServerEntity *pServerEntity = nullptr;
	//	ServerServiceInformation* pNewService = nullptr;
	//	bool bAddStatusWatcher;

	//	// This should not be failed
	//	if( !(GetMyOwner()->GetClusterServiceEntity( GetClusterID(), pServiceEntity )) )
	//	{
	//		svrTrace(SVR_CLUSTER, "Ignoring unregistered cluster service sync: ClusterID:{0}", GetClusterID() );
	//		//svrErr(ResultCode::SVR_INVALID_CLUSTERID);
	//		goto Proc_End;
	//	}

	//	bAddStatusWatcher = pServiceEntity->GetClusterMembership() != ClusterMembership::StatusWatcher;

	//	for( uint iMember = 0; iMember < GetMemberList().size(); iMember++ )
	//	{
	//		const ServiceInformation& serviceInfo = GetMemberList()[iMember];
	//		
	//		if( !bAddStatusWatcher && serviceInfo.Membership == ClusterMembership::StatusWatcher )
	//			continue;

	//		svrChk( Service::ServerEntityManager->GetOrRegisterServer( serviceInfo.UID.GetServerID(), serviceInfo.ServerClass, serviceInfo.ServerAddress, pServerEntity ) );

	//		pNewService = nullptr;
	//		svrChk( pServiceEntity->NewServerService( serviceInfo.UID, pServerEntity, serviceInfo.Membership, serviceInfo.Status, pNewService ) );
	//	}


	//Proc_End:

	//	CloseTransaction( hr );

	//	return hr;
	//}


};// namespace GameServer 
};// namespace SF 

