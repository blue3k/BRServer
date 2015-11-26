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
#include "ServerSystem/ServiceEntity/ClusterServiceTrans.h"
#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"


BR_MEMORYPOOL_IMPLEMENT(Svr::ClusterInitializationTrans);
BR_MEMORYPOOL_IMPLEMENT(Svr::RequestDataSyncTrans);
BR_MEMORYPOOL_IMPLEMENT(Svr::ClusterMasterAssignedTrans);
BR_MEMORYPOOL_IMPLEMENT(Svr::ClusterMasterVoteTrans);



namespace BR {
namespace Svr {



	ClusterInitializationTrans::ClusterInitializationTrans()
		: TransactionT( TransactionID() )
	{
		BR_TRANS_MESSAGE( TimerResult, { return OnTimer(pRes); });
		//BR_TRANS_MESSAGE( Message::ClusterServer::GetClusterMemberListRes,	{ return OnGetClusterMemberList(pRes); });
		BR_TRANS_MESSAGE( Message::ClusterServer::JoinClusterRes,			{ return OnClusterJoined(pRes); });
		BR_TRANS_MESSAGE( Message::ClusterServer::RequestDataSyncRes,		{ return OnClusterDataSync(pRes); });
	}

	// Set timer when it fails
	void ClusterInitializationTrans::SetFailRetryTimer(HRESULT hrRes)
	{
		if( FAILED(hrRes) )
		{
			svrTrace( Svr::TRC_CLUSTER, "Cluster initialization Failed Entity:%0%, ClusterID:%1%,Type:%2%,Membership:%3%, Step:%4%, hr:{5:X8}. Retrying ...", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership(), (int)m_Step, hrRes );
			SetTimer(DurationMS(10*1000));
		}
	}

	// Timer handling
	HRESULT ClusterInitializationTrans::OnTimer(TransactionResult* pRes)
	{
		HRESULT hr = S_OK;

		svrTrace(Svr::TRC_CLUSTER, "Cluster OnTImer Entity:%0%, ClusterID:%1%,Type:%2%,Membership:%3%, Step:%4%", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership(), (UINT)m_Step);

		switch( m_Step )
		{
		//case Step_GetMemberList:
		//	svrChk( GetClusterMemberList() );
		//	break;
		case Step_JoinCluster:
			svrChk(JoinCluster());
			break;
		case Step_RequestDataSync:
			svrChk(RequestDataSync()); // try one more
			break;
		default:
			Assert(false);
			break;
		};

	Proc_End:

		SetFailRetryTimer(hr);

		return S_OK;
	}


	//HRESULT ClusterInitializationTrans::GetClusterMemberList()
	//{
	//	HRESULT hr = S_OK;
	//	ServerEntity *pServerEntity = nullptr;
	//	EntityUID clusterManagerMasterUID;

	//	// 1. Find entity manager server
	//	m_Step = Step_GetMemberList;

	//	clusterManagerMasterUID = GetServerComponent<ClusterManagerServiceEntity>()->GetMasterUID();
	//	if( SUCCEEDED(GetServerComponent<ServerEntityManager>()->GetServerEntity( clusterManagerMasterUID.SvrID, pServerEntity )) )
	//	{
	//		svrTrace( Svr::TRC_CLUSTER, "Cluster memberlist query Entity:%0%, ClusterID:%1%,Type:%2%,Membership:%3%", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership() );

	//		// 2. Get service entity list in the cluster
	//		svrChk( pServerEntity->GetPolicy<Policy::IPolicyClusterServer>()->GetClusterMemberListCmd( GetTransID(), RouteContext(GetOwnerEntityUID(),clusterManagerMasterUID), 0, GetMyOwner()->GetClusterID() ) );
	//	}
	//	else
	//	{
	//		svrTrace( Svr::TRC_CLUSTER, "Waiting Entity Server ready:%0%, ClusterID:%1%,Type:%2%,Membership:%3%", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership() );
	//		SetTimer(1000);
	//	}

	//Proc_End:

	//	SetFailRetryTimer(hr);

	//	return hr;
	//}

	//HRESULT ClusterInitializationTrans::OnGetClusterMemberList(TransactionResult* pRes)
	//{
	//	HRESULT hr = S_OK;
	//	Message::ClusterServer::GetClusterMemberListRes msgRes;
	//	const ServiceInformation *currentMaster = nullptr, *currentSlave = nullptr, *myInformation = nullptr;

	//	svrChk(pRes->GetHRESULT());

	//	svrChk( msgRes.ParseIMsg( ((MessageResult*)pRes)->GetMessage() ) );

	//	svrTrace( Svr::TRC_CLUSTER, "Cluster memberlist(%4%) Entity:%0%, ClusterID:%1%,Type:%2%,Membership:%3%", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership(), msgRes.GetMemberList().GetSize() );

	//	// 3. Open a cluster if this is the master, and there is no service, otherwise join as a non-master
	//	for( UINT iMember = 0; iMember < msgRes.GetMemberList().GetSize(); iMember++ )
	//	{
	//		const ServiceInformation *pCurrentService = &msgRes.GetMemberList()[iMember];
	//		if( pCurrentService->Membership == ClusterMembership::Master )
	//		{
	//			if( pCurrentService->Status == ServiceStatus::Ready || pCurrentService->Status == ServiceStatus::Online || pCurrentService->UID == GetMyOwner()->GetEntityUID() )
	//			{
	//				currentMaster = pCurrentService;
	//			}
	//			//else if( pCurrentService->Status == ServiceStatus::Online )
	//			//{
	//			//	currentSlave = pCurrentService;
	//			//}
	//		}
	//		else if( pCurrentService->Membership == ClusterMembership::Slave 
	//			&& pCurrentService->Status == ServiceStatus::Online )
	//		{
	//			currentSlave = pCurrentService;
	//		}

	//		if( pCurrentService->UID == GetMyOwner()->GetEntityUID() )
	//		{
	//			myInformation = pCurrentService;
	//		}

	//		// if we found all
	//		if( currentMaster != nullptr && myInformation != nullptr )
	//			break;
	//	}

	//	if( currentMaster != nullptr )
	//	{
	//		// I'm the master and already in the list
	//		if( currentMaster->UID == GetMyOwner()->GetEntityUID() )
	//		{
	//			svrTrace( Svr::TRC_CLUSTER, "Cluster GetMemberList Entity:%0%, ClusterID:%1%,Type:%2%,Membership:%3% : %4%", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership(),
	//				"I'm the master and already in the list" );

	//			CloseTransaction( hr );
	//		}
	//		else
	//		{
	//			// We have the master. join as a slave or watcher
	//			svrTrace( Svr::TRC_CLUSTER, "Cluster GetMemberList Entity:%0%, ClusterID:%1%,Type:%2%,Membership:%3% : %4%", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership(),
	//				"We have the master. join as a slave or watcher" );
	//			if( GetMyOwner()->GetClusterMembership() == ClusterMembership::Master )
	//			{
	//				GetMyOwner()->SetClusterMembership( ClusterMembership::Slave );
	//			}

	//			m_currentMaster = *currentMaster;

	//			if( myInformation == nullptr )
	//			{
	//				svrChk( JoinCluster() );
	//			}
	//			else
	//			{
	//				// Fill my cluster status
	//				svrChk( AddOtherServicesToMe((UINT)msgRes.GetMemberList().GetSize(), msgRes.GetMemberList().data() ) );

	//				// 4. Request full data if replica
	//				if( FAILED(RequestDataSync()) )
	//				{
	//					// if failed sync we can retry laster
	//					m_Step = Step_RequestDataSync;
	//					SetFailRetryTimer(hr);
	//				}

	//			}
	//		}
	//	}
	//	else if( currentSlave != nullptr )
	//	{
	//		// We don't have any master yet, but this slave will be going to be the master. we need to wait
	//		// We are going to check this again after 1sec
	//		svrTrace( Svr::TRC_CLUSTER, "Cluster GetMemberList Entity:%0%, ClusterID:%1%,Type:%2%,Membership:%3% : %4%", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership(),
	//			"We don't have any master yet, but this slave will be going to be the master. we need to wait" );
	//		SetTimer( 2000 );
	//	}
	//	else// if( GetMyOwner()->GetClusterMembership() == ClusterMembership::Master )
	//	{
	//		// I'm going to be the first master
	//		svrTrace( Svr::TRC_CLUSTER, "Cluster GetMemberList Entity:%0%, ClusterID:%1%,Type:%2%,Membership:%3% : %4%", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership(),
	//			"I'm the first one in this cluster" );
	//		//m_currentMaster.UID = GetMyOwner()->GetEntityUID();

	//		// Add other members to me
	//		svrChk( AddOtherServicesToMe( (UINT)msgRes.GetMemberList().GetSize(), msgRes.GetMemberList().data() ) );

	//		if( myInformation == nullptr )
	//		{
	//			svrChk( JoinCluster() );
	//		}
	//		else
	//		{
	//			// Request full data if replica
	//			if( FAILED(RequestDataSync()) )
	//			{
	//				// if failed sync we can retry laster
	//				m_Step = Step_RequestDataSync;
	//				SetFailRetryTimer(hr);
	//			}

	//		}
	//	}
	//	//else
	//	//{
	//	//	// No one in the list, and I'm not the master so wait until other master comes
	//	//	svrTrace( Svr::TRC_CLUSTER, "Cluster GetMemberList Entity:%0%, ClusterID:%1%,Type:%2%,Membership:%3% : %4%", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership(),
	//	//		"No one in the list, and I'm not the master so wait until other master comes" );
	//	//	SetTimer( 2000 );
	//	//}

	//Proc_End:

	//	SetFailRetryTimer(hr);

	//	return S_OK;
	//}

	HRESULT ClusterInitializationTrans::JoinCluster(  )
	{
		HRESULT hr = S_OK;
		ClusterServerService *pService = nullptr;
		ServerEntity *pMasterServerEntity = nullptr;
		EntityUID clusterManagerMasterUID;
		ClusterManagerServiceEntity *pClusterManager = nullptr;
		ClusterMembership membership = GetMyOwner()->GetClusterMembership();

		// send join cluster
		m_Step = Step_JoinCluster;

		svrTrace(Svr::TRC_CLUSTER, "Cluster Join Entity:%0%, ClusterID:%1%,Type:%2%,Membership:%3%", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership());

		auto netPrivate = BrServer::GetInstance()->GetNetPrivate();
		Assert(netPrivate->GetNetClass() != NetClass::Unknown);

		// 1. Find entity manager servery
		pClusterManager = GetServerComponent<ClusterManagerServiceEntity>();
		clusterManagerMasterUID = pClusterManager->GetMasterUID();
		GetServerComponent<ServerEntityManager>()->GetServerEntity(clusterManagerMasterUID.GetServerID(), pMasterServerEntity);

		if (pClusterManager == GetOwnerEntity()) // If I'm the clustermanager entity of this server
		{
			// wait at least 2 secs for connections if 
			if (Util::TimeSince(GetTransactionStartTime()) > DurationMS(4000))
			{
				if (clusterManagerMasterUID == 0)
				{
					svrTrace(Trace::TRC_WARN, "Waiting Master entity too long ready:%0%, ClusterID:%1%,Type:%2%,Membership:%3%", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership());
				}
				//GetServerComponent<ServerEntityManager>()->GetEntityManagerServerEntity(pMasterServerEntity);
			}

			if (pMasterServerEntity == GetOwnerEntity() && membership >= ClusterMembership::Slave)
			{
				svrTrace( Svr::TRC_CLUSTER, "Cluster GetMemberList Entity:%0%, ClusterID:%1%,Type:%2%,Membership:%3% : %4%", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership(),
					"I'm the first one in this cluster" );
				membership = ClusterMembership::Master;
			}
		}

		if(pMasterServerEntity == nullptr || clusterManagerMasterUID == 0)
		{
			svrTrace(Svr::TRC_CLUSTER, "Waiting Entity Server ready:%0%, ClusterID:%1%,Type:%2%,Membership:%3%", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership());
			SetTimer(DurationMS(1000));
			goto Proc_End;
		}

		svrTrace( Svr::TRC_CLUSTER, "Cluster memberlist query Entity:%0%, ClusterID:%1%,Type:%2%,Membership:%3%", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership() );

		// 2. Get service entity list in the cluster
		svrChk(pMasterServerEntity->GetPolicy<Policy::IPolicyClusterServer>()->JoinClusterCmd( GetTransID(), RouteContext(GetOwnerEntityUID(),clusterManagerMasterUID), 0, 
			GetOwnerEntityUID(), netPrivate->GetNetClass(), netPrivate->GetLocalAddress(),
			GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), membership));


		//svrChk( GetServerComponent<ClusterManagerServiceEntity>()->GetService<ClusterServerService>( pService ) );
		//svrChk( pService->JoinClusterCmd( GetTransID(), 0, 
		//	GetOwnerEntityUID(), netPrivate->GetNetClass(), netPrivate->GetLocalAddress(),
		//	GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership() ) );

	Proc_End:

		if( FAILED(hr) )
		{
			// if failed something retry after 1 sec from scratch
			m_Step = Step_JoinCluster;
			SetFailRetryTimer(hr);
		}

		return hr;
	}

	HRESULT ClusterInitializationTrans::OnClusterJoined(TransactionResult* pRes)
	{
		HRESULT hr = S_OK;
		Message::ClusterServer::JoinClusterRes msgRes;

		svrChk(pRes->GetHRESULT());

		svrChk( msgRes.ParseIMsg( ((MessageResult*)pRes)->GetMessage() ) );

		svrTrace( Svr::TRC_CLUSTER, "Cluster Joined Entity:%0%, ClusterID:%1%,Type:%2%,Membership:%3%", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership() );

		// Fill my cluster status
		svrChk( AddOtherServicesToMe((UINT)msgRes.GetMemberList().GetSize(), msgRes.GetMemberList().data()) );

		// 4. Request full data if replica
		if( FAILED(RequestDataSync()) )
		{
			// if failed sync we can retry laster
			m_Step = Step_RequestDataSync;
			SetFailRetryTimer(hr);
		}

	Proc_End:

		if( FAILED(hr) )
		{
			// if failed something retry after 1 sec from scratch
			m_Step = Step_JoinCluster;
			SetFailRetryTimer(hr);
		}

		return S_OK;
	}

	HRESULT ClusterInitializationTrans::RequestDataSync()
	{
		HRESULT hr = S_OK;
		ServerEntity *pServerEntity = nullptr;

		if( m_currentMaster.UID == 0 )
		{
			CloseTransaction(hr);
		}
		else
		{
			svrTrace( Svr::TRC_CLUSTER, "Cluster RequestData Entity:%0%, ClusterID:%1%,Type:%2%,Membership:%3%", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership() );

			// 4. Request full data if replica
			m_Step = Step_RequestDataSync;
			svrChk( GetServerComponent<ServerEntityManager>()->GetServerEntity( m_currentMaster.UID.GetServerID(), pServerEntity ) );

			svrChk( pServerEntity->GetPolicy<Policy::IPolicyClusterServer>()->RequestDataSyncCmd( GetTransID(), RouteContext(GetMyOwner()->GetEntityUID(), m_currentMaster.UID), 0, 
				GetMyOwner()->GetClusterID() ) );
		}

	Proc_End:

		SetFailRetryTimer(hr);

		return hr;
	}

	HRESULT ClusterInitializationTrans::OnClusterDataSync(TransactionResult* pRes)
	{
		HRESULT hr = S_OK;
		//Message::ClusterServer::GetClusterMemberListRes msgRes;

		svrChk(pRes->GetHRESULT());

		//svrChk( msgRes.ParseIMsg( ((MessageResult*)pRes)->GetMessage() ) );
		svrTrace( Svr::TRC_CLUSTER, "Cluster RequestDataSync Done Entity:%0%, ClusterID:%1%,Type:%2%,Membership:%3%", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership() );

		CloseTransaction( hr );

	Proc_End:

		SetFailRetryTimer(hr);

		return S_OK;
	}

	// Add other services to me
	HRESULT ClusterInitializationTrans::AddOtherServicesToMe( UINT numServices, const ServiceInformation *pServiceInformations )
	{
		HRESULT hr = S_OK;
		bool bAddStatusWatcher = GetMyOwner()->GetClusterMembership() != ClusterMembership::StatusWatcher;

		ServerEntityManager *pServerEntityManager = GetServerComponent<ServerEntityManager>();
		for( UINT iMember = 0; iMember < numServices; iMember++ )
		{
			ServerEntity *pServerEntity = nullptr;
			ServerServiceInformation *pService = nullptr;
			const ServiceInformation *pServiceInfo = &pServiceInformations[iMember];

			// Skip watcher
			if( !bAddStatusWatcher && pServiceInfo->Membership == ClusterMembership::StatusWatcher )
				continue;

			svrChk(pServerEntityManager->GetOrRegisterServer(pServiceInfo->UID.GetServerID(), pServiceInfo->ServerClass,
				pServiceInfo->ServerAddress.strAddr, pServiceInfo->ServerAddress.usPort,
				pServerEntity ) );

			if( pServerEntity->GetServerUpTime() == TimeStampSec::min() || pServerEntity->GetServerUpTime() < pServiceInfo->ServerUpTime )
				pServerEntity->SetServerUpTime( pServiceInfo->ServerUpTime );

			svrChk( GetMyOwner()->NewServerService( pServiceInfo->UID, pServerEntity, pServiceInfo->Membership, pServiceInfo->Status, pService ) );
		}

		m_currentMaster.UID = GetMyOwner()->GetMasterUID();


	Proc_End:

		return hr;
	}

	// Start Transaction
	HRESULT ClusterInitializationTrans::StartTransaction()
	{
		HRESULT hr = S_OK;

		m_hr = S_OK;

		svrChk( __super::StartTransaction() );

		BrServer::GetInstance()->GetNumberServicesToWait().fetch_add(1, std::memory_order_relaxed);

		svrTrace( Svr::TRC_CLUSTER, "Cluster Initialization Entity:%0%:%1%, ClusterID:%2%,Type:%3%,Membership:%4%", GetOwnerEntityUID(), typeid(*GetMyOwner()).name(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership() );

		svrChk(JoinCluster());
		//svrChk( GetClusterMemberList() );

	Proc_End:

		SetFailRetryTimer(hr);

		return S_OK;
	}

	HRESULT ClusterInitializationTrans::OnCloseTransaction( HRESULT hrRes )
	{
		m_hr = hrRes;

		if (SUCCEEDED(hrRes))
			m_Step = Step_Done;

		GetMyOwner()->SetInitialized(SUCCEEDED(hrRes));

		BrServer::GetInstance()->GetNumberServicesToWait().fetch_sub(1, std::memory_order_relaxed);

		__super::OnCloseTransaction( hrRes );

		if( SUCCEEDED(hrRes) )
			GetMyOwner()->SetServiceStatus( ServiceStatus::Ready );

		return hrRes;
	}

	
	/////////////////////////////////////////////////////////////////////////////
	//
	//	Cluster service transactions
	//

	// Start Transaction
	HRESULT RequestDataSyncTrans::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( __super::StartTransaction() );

		svrChk( GetMyOwner()->SyncDataToTarget( GetRouteContext().GetFrom()) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}

	// Start Transaction
	HRESULT ClusterMasterAssignedTrans::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( __super::StartTransaction() );

		svrChk( GetMyOwner()->SetMaster( GetMasterUID() ) );

		GetMyOwner()->EndVoting();

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}

	// Start Transaction
	HRESULT ClusterMasterVoteTrans::StartTransaction()
	{
		HRESULT hr = S_OK;
		ServerServiceInformation *pVotedService = nullptr;
		UINT uiExpectedVoterCount = 0, uiTotalVoted = 0;
		UINT uiMaxVotedCount = 0;
		EntityUID maxEntity(0);

		svrChk( __super::StartTransaction() );

		// Only master or slave can process vote result
		if( GetMyOwner()->GetClusterMembership() < ClusterMembership::Slave )
			goto Proc_End;

		svrChk( GetMyOwner()->FindService( GetVoteToUID(), pVotedService ) );

		// If not voting state yet, enter it
		GetMyOwner()->StartVoting();

		pVotedService->SetVotedCount( pVotedService->GetVotedCount() +1 );

		// check voted status
		GetMyOwner()->ForEach( [&](ServerServiceInformation *pService) 
		{
			// Only master or slave will process vote so send only to them
			if( pService->IsServiceAvailable() && pService->GetClusterMembership() >= ClusterMembership::Slave )
			{
				uiExpectedVoterCount++;
				uiTotalVoted += pService->GetVotedCount();
				if( uiMaxVotedCount < pService->GetVotedCount() )
				{
					uiMaxVotedCount = pService->GetVotedCount();
					maxEntity = pService->GetEntityUID();
				}
			}
		});

		// If vote is done
		if( uiTotalVoted >= uiExpectedVoterCount )
		{
			if( maxEntity == GetMyOwner()->GetEntityUID() )
			{
				GetMyOwner()->AssignMaster( maxEntity );
			}
			GetMyOwner()->EndVoting();
		}

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}



};// namespace GameServer 
};// namespace BR 

