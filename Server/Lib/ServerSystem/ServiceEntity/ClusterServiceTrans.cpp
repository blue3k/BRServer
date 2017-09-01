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
#include "ServerSystem/ServiceEntity/ClusterServiceTrans.h"
#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"


SF_MEMORYPOOL_IMPLEMENT(BR::Svr::ClusterInitializationTrans);
SF_MEMORYPOOL_IMPLEMENT(BR::Svr::RequestDataSyncTrans);
SF_MEMORYPOOL_IMPLEMENT(BR::Svr::ClusterMasterAssignedTrans);
SF_MEMORYPOOL_IMPLEMENT(BR::Svr::ClusterMasterVoteTrans);
SF_MEMORYPOOL_IMPLEMENT(BR::Svr::ClusterUpdateStatusTrans);
SF_MEMORYPOOL_IMPLEMENT(BR::Svr::ClusterUpdateWorkloadTrans);
SF_MEMORYPOOL_IMPLEMENT(BR::Svr::GetLowestWorkloadClusterMemberTrans);
SF_MEMORYPOOL_IMPLEMENT(BR::Svr::ClusterNewServerServiceJoinedC2SEvtTrans);
SF_MEMORYPOOL_IMPLEMENT(BR::Svr::ClusterNewServerServiceJoinedC2SEvtEntityTrans);



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

	ClusterInitializationTrans::~ClusterInitializationTrans()
	{
	}

	// Set timer when it fails
	void ClusterInitializationTrans::SetFailRetryTimer(Result hrRes)
	{
		if( !(hrRes) )
		{
			svrTrace( Svr::TRC_CLUSTER, "Cluster initialization Failed Entity:{0}, ClusterID:{1},Type:{2},Membership:{3}, Step:{4}, hr:{5:X8}. Retrying ...", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership(), (int)m_Step, hrRes );
			SetTimer(DurationMS(10*1000));
		}
	}

	// Timer handling
	Result ClusterInitializationTrans::OnTimer(TransactionResult* pRes)
	{
		Result hr = ResultCode::SUCCESS;

		svrTrace(Svr::TRC_CLUSTER, "Cluster OnTimer Entity:{0}, ClusterID:{1},Type:{2},Membership:{3}, Step:{4}", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership(), (UINT)m_Step);

		switch( m_Step )
		{
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

		return ResultCode::SUCCESS;
	}


	Result ClusterInitializationTrans::JoinCluster(  )
	{
		Result hr = ResultCode::SUCCESS;
		//ClusterServerService *pService = nullptr;
		ServerEntity *pMasterServerEntity = nullptr;
		EntityUID clusterManagerMasterUID;
		ClusterManagerServiceEntity *pClusterManager = nullptr;
		ClusterMembership membership = GetMyOwner()->GetClusterMembership();

		// send join cluster
		m_Step = Step_JoinCluster;

		svrTrace(Svr::TRC_CLUSTER, "Cluster Join Entity:{0}, ClusterID:{1}, Type:{2}, Membership:{3}", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership());

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
					svrTrace(Trace::TRC_WARN, "Waiting Master entity too long ready:{0}, ClusterID:{1},Type:{2},Membership:{3}", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership());
				}
				//GetServerComponent<ServerEntityManager>()->GetEntityManagerServerEntity(pMasterServerEntity);
			}

			if (pMasterServerEntity == GetOwnerEntity() && membership >= ClusterMembership::Slave)
			{
				svrTrace( Svr::TRC_CLUSTER, "Cluster GetMemberList Entity:{0}, ClusterID:{1},Type:{2},Membership:{3} : {4}", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership(),
					"I'm the first one in this cluster" );
				membership = ClusterMembership::Master;
			}
		}

		if(pMasterServerEntity == nullptr || clusterManagerMasterUID == 0)
		{
			svrTrace(Svr::TRC_CLUSTER, "Waiting Entity Server ready:{0}, ClusterID:{1},Type:{2},Membership:{3}", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership());
			SetTimer(DurationMS(1000));
			goto Proc_End;
		}

		svrTrace( Svr::TRC_CLUSTER, "Cluster memberlist query Entity:{0}, ClusterID:{1},Type:{2},Membership:{3}", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership() );

		// 2. Get service entity list in the cluster
		svrChk(pMasterServerEntity->GetPolicy<Policy::IPolicyClusterServer>()->JoinClusterCmd( RouteContext(GetOwnerEntityUID(),clusterManagerMasterUID), GetTransID(), 0,
			GetOwnerEntityUID(), netPrivate->GetNetClass(), netPrivate->GetLocalAddress(),
			GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), membership));


		//svrChk( GetServerComponent<ClusterManagerServiceEntity>()->GetService<ClusterServerService>( pService ) );
		//svrChk( pService->JoinClusterCmd( GetTransID(), 0, 
		//	GetOwnerEntityUID(), netPrivate->GetNetClass(), netPrivate->GetLocalAddress(),
		//	GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership() ) );

	Proc_End:

		if( !(hr) )
		{
			// if failed something retry after 1 sec from scratch
			m_Step = Step_JoinCluster;
			SetFailRetryTimer(hr);
		}

		return hr;
	}

	Result ClusterInitializationTrans::OnClusterJoined(TransactionResult* pRes)
	{
		Result hr = ResultCode::SUCCESS;
		Message::ClusterServer::JoinClusterRes msgRes;

		svrChk(pRes->GetResult());

		svrChk( msgRes.ParseMessage( ((MessageResult*)pRes)->GetMessage() ) );

		svrTrace( Svr::TRC_CLUSTER, "Cluster Joined Entity:{0}, ClusterID:{1},Type:{2},Membership:{3}", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership() );

		// Fill my cluster status
		svrChk( AddOtherServicesToMe((UINT)msgRes.GetMemberList().GetSize(), msgRes.GetMemberList().data()) );

		// 4. Request full data if replica
		if( !(RequestDataSync()) )
		{
			// if failed sync we can retry laster
			m_Step = Step_RequestDataSync;
			SetFailRetryTimer(hr);
		}

	Proc_End:

		if( !(hr) )
		{
			// if failed something retry after 1 sec from scratch
			m_Step = Step_JoinCluster;
			SetFailRetryTimer(hr);
		}

		return ResultCode::SUCCESS;
	}

	Result ClusterInitializationTrans::RequestDataSync()
	{
		Result hr = ResultCode::SUCCESS;
		ServerEntity *pServerEntity = nullptr;

		if( m_currentMaster.UID == 0 )
		{
			CloseTransaction(hr);
		}
		else
		{
			svrTrace( Svr::TRC_CLUSTER, "Cluster RequestData Entity:{0}, ClusterID:{1},Type:{2},Membership:{3}", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership() );

			// 4. Request full data if replica
			m_Step = Step_RequestDataSync;
			svrChk( GetServerComponent<ServerEntityManager>()->GetServerEntity( m_currentMaster.UID.GetServerID(), pServerEntity ) );

			svrChk( pServerEntity->GetPolicy<Policy::IPolicyClusterServer>()->RequestDataSyncCmd( RouteContext(GetMyOwner()->GetEntityUID(), m_currentMaster.UID), GetTransID(), 0,
				GetMyOwner()->GetClusterID() ) );
		}

	Proc_End:

		SetFailRetryTimer(hr);

		return hr;
	}

	Result ClusterInitializationTrans::OnClusterDataSync(TransactionResult* pRes)
	{
		Result hr = ResultCode::SUCCESS;
		//Message::ClusterServer::GetClusterMemberListRes msgRes;

		svrChk(pRes->GetResult());

		//svrChk( msgRes.ParseMessage( ((MessageResult*)pRes)->GetMessage() ) );
		svrTrace( Svr::TRC_CLUSTER, "Cluster RequestDataSync Done Entity:{0}, ClusterID:{1},Type:{2},Membership:{3}", GetOwnerEntityUID(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership() );

		CloseTransaction( hr );

	Proc_End:

		SetFailRetryTimer(hr);

		return ResultCode::SUCCESS;
	}

	// Add other services to me
	Result ClusterInitializationTrans::AddOtherServicesToMe( UINT numServices, const ServiceInformation *pServiceInformations )
	{
		Result hr = ResultCode::SUCCESS;
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
				pServiceInfo->ServerAddress,
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
	Result ClusterInitializationTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		m_hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		BrServer::GetInstance()->GetNumberServicesToWait().fetch_add(1, std::memory_order_relaxed);

		svrTrace( Svr::TRC_CLUSTER, "Cluster Initialization Entity:{0}:{1}, ClusterID:{2},Type:{3},Membership:{4}", GetOwnerEntityUID(), typeid(*GetMyOwner()).name(), GetMyOwner()->GetClusterID(), GetMyOwner()->GetClusterType(), GetMyOwner()->GetClusterMembership() );

		svrChk(JoinCluster());
		//svrChk( GetClusterMemberList() );

	Proc_End:

		SetFailRetryTimer(hr);

		return ResultCode::SUCCESS;
	}

	Result ClusterInitializationTrans::OnCloseTransaction( Result hrRes )
	{
		m_hr = hrRes;

		if ((hrRes))
			m_Step = Step_Done;

		GetMyOwner()->SetInitialized((hrRes));

		BrServer::GetInstance()->GetNumberServicesToWait().fetch_sub(1, std::memory_order_relaxed);

		super::OnCloseTransaction( hrRes );

		if( (hrRes) )
			GetMyOwner()->SetServiceStatus( ServiceStatus::Ready );

		return hrRes;
	}

	
	/////////////////////////////////////////////////////////////////////////////
	//
	//	Cluster service transactions
	//

	// Start Transaction
	Result RequestDataSyncTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		svrChk( GetMyOwner()->SyncDataToTarget( GetRouteContext().GetFrom()) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}

	// Start Transaction
	Result ClusterMasterAssignedTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		svrChk( GetMyOwner()->SetMaster( GetMasterUID() ) );

		GetMyOwner()->EndVoting();

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}

	// Start Transaction
	Result ClusterMasterVoteTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		ServerServiceInformation *pVotedService = nullptr;
		UINT uiExpectedVoterCount = 0, uiTotalVoted = 0;
		UINT uiMaxVotedCount = 0;
		EntityUID maxEntity(0);

		svrChk( super::StartTransaction() );

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


	// Start Transaction
	Result ClusterUpdateStatusTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		ServerServiceInformation *pService = nullptr;
		Svr::ClusteredServiceEntity *pServiceEntity = nullptr;

		svrChk(super::StartTransaction());

		if ((GetServerComponent<ClusterManagerServiceEntity>()->GetClusterServiceEntity(GetClusterID(), pServiceEntity)))
		{
			if ((pServiceEntity->FindService(GetSender(), pService)))
			{
				pService->SetServiceStatus(GetMemberStatus());
			}
			else
			{
				svrTrace(Svr::TRC_CLUSTER, "Ignoring update status for ClusterID:{0}, Entity:{1} to {2}, The entity seems not ready.", GetClusterID(), GetSender(), GetMemberStatus());
			}
		}
		else
		{
			svrTrace(Svr::TRC_CLUSTER, "Ignoring update status for ClusterID:{0}, Entity:{1} to {2}", GetClusterID(), GetSender(), GetMemberStatus());
		}

	Proc_End:

		if (!(hr))
		{
			svrTrace(Svr::TRC_CLUSTER, "Update state is failed ClusterID:{0}, Entity:{1} to {2}", GetClusterID(), GetSender(), GetMemberStatus());
			CloseTransaction(hr);
		}
		else
		{
			svrTrace(Svr::TRC_CLUSTER, "State is updated ClusterID:{0}, Entity:{1} to {2}", GetClusterID(), GetSender(), GetMemberStatus());
			CloseTransaction(hr);
		}

		return hr;
	}


	// Start Transaction
	Result ClusterUpdateWorkloadTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		ClusteredServiceEntity* pServiceEntity = nullptr;
		ServerServiceInformation *pUpdatedService = nullptr;

		svrChk(super::StartTransaction());

		if (GetMyOwner()->GetClusterID() == GetClusterID())
		{
			pServiceEntity = GetMyOwner();
		}
		else
		{
			if (!(GetServerComponent<ClusterManagerServiceEntity>()->GetClusterServiceEntity(GetClusterID(), pServiceEntity)))
			{
				svrTrace(Svr::TRC_CLUSTER, "Ignoring workload update of an unregistered cluster ClusterID:{0} Sender:{1}", GetClusterID(), GetSender());
				CloseTransaction(hr);
				goto Proc_End;
			}
		}

		hr = pServiceEntity->FindService(GetSender(), pUpdatedService);
		if (!(hr))
		{
			goto Proc_End;
		}

		pUpdatedService->SetWorkload(GetWorkload());

		// Manual replication is needed while this is not a usual transaction
		// Hop count 1 is the maximum number with replica clustring model
		if (GetRouteHopCount() >= 1)
			goto Proc_End;

		pServiceEntity->ForEach([&](ServerServiceInformation* pService)
		{
			if (pService->GetEntityUID() == pUpdatedService->GetEntityUID())
				return;

			TossMessageToTarget(pService);
		});


	Proc_End:

		if (!(hr))
		{
			svrTrace(Trace::TRC_ERROR, "Failed to update workload of unregistered cluster ClusterID:{0}, Sender:{1}", GetClusterID(), GetSender());
			CloseTransaction(hr);
		}
		else
			CloseTransaction(hr);

		return hr;
	}



	// Start Transaction
	Result GetLowestWorkloadClusterMemberTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		ServerServiceInformation *pLowestService = nullptr;

		svrChk(super::StartTransaction());

		GetMyOwner()->ForEachNonWatcher([&](ServerServiceInformation* pService)
		{
			if (!pService->IsServiceAvailable()) return;

			if (pLowestService == nullptr || pLowestService->GetWorkload() > pService->GetWorkload())
				pLowestService = pService;

			return;
		});

		if (pLowestService == nullptr)
			svrErrClose(ResultCode::E_SVR_CLUSTER_NOTREADY);

		pLowestService->GetServiceInformation(m_LowestMemberInfo);

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}



	// Start Transaction
	Result ClusterNewServerServiceJoinedC2SEvtEntityTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		ServerServiceInformation *pRequestedService = nullptr;
		ServerEntity *pSenderEntity = nullptr;
		ClusteredServiceEntity *pServiceEntity = nullptr;

		Assert(GetJoinedServiceNetClass() != NetClass::Unknown);
		Assert(GetJoinedServiceUID().GetServerID() < 1000);

		svrChk(super::StartTransaction());

		// This transaction is used for two cases. so it's better to query cluster service again
		if (GetMyOwner()->GetClusterID() == GetClusterID())
		{
			pServiceEntity = GetMyOwner();
		}
		else
		{
			if (!(GetServerComponent<ClusterManagerServiceEntity>()->GetClusterServiceEntity(GetClusterID(), pServiceEntity)))
			{
				svrTrace(Svr::TRC_CLUSTER, "Ignoring to add new server service cluster member ClusterID:{0}, Joined:{1}", GetClusterID(), GetJoinedServiceUID());
				goto Proc_End;
			}
		}

		svrChk(GetServerComponent<ServerEntityManager>()->GetOrRegisterServer(GetJoinedServiceUID().GetServerID(), GetJoinedServiceNetClass(), GetJoinedServiceAddress(), pSenderEntity));

		svrChk(pServiceEntity->NewServerService(GetJoinedServiceUID(), pSenderEntity, GetJoinedServiceMembership(), ServiceStatus::Online, pRequestedService));


	Proc_End:

		if (!(hr))
		{
			svrTrace(Trace::TRC_ERROR, "Failed to add new server service cluster member ClusterID:{0}, Joined:{1}", GetClusterID(), GetJoinedServiceUID());
		}

		CloseTransaction(hr);

		return hr;
	}



	// Start Transaction
	Result ClusterNewServerServiceJoinedC2SEvtTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
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
				if (!(GetServerComponent<ClusterManagerServiceEntity>()->GetClusterServiceEntity(GetClusterID(), pServiceEntity)))
				{
					svrTrace(Svr::TRC_CLUSTER, "Ignoring to add new server service cluster member ClusterID:{0}, Joined:{1}", GetClusterID(), GetJoinedServiceUID());
					goto Proc_End;
				}
			}

			svrChk(GetServerComponent<ServerEntityManager>()->GetOrRegisterServer(GetJoinedServiceUID().GetServerID(), GetJoinedServiceNetClass(), GetJoinedServiceAddress(), pSenderEntity));

			svrChk(pServiceEntity->NewServerService(GetJoinedServiceUID(), pSenderEntity, GetJoinedServiceMembership(), ServiceStatus::Online, pRequestedService));
		}


	Proc_End:

		if (!(hr))
		{
			svrTrace(Trace::TRC_ERROR, "Failed to add new server service cluster member ClusterID:{0}, Joined:{1}", GetClusterID(), GetJoinedServiceUID());
		}

		CloseTransaction(hr);

		return hr;
	}



};// namespace GameServer 
};// namespace BR 

