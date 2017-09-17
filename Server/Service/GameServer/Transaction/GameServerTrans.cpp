////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : KyungKun Ko
//
// Description : Entity server implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "GameServer.h"
#include "Net/NetServerUDP.h"
#include "Server/BrService.h"
#include "SvrTrace.h"
#include "Entity/EntityManager.h"
#include "ServerEntity/ServerEntityManager.h"
#include "Transaction/MessageRoute.h"
#include "GameServerClass.h"
#include "Memory/MemoryPool.h"
#include "Types/SFEngineTypedefs.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameNetPolicy.h"

#include "ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServiceEntity/MatchingServiceEntity.h"
#include "ServiceEntity/GamePartyManagerServiceEntity.h"



#include "GameServerTrans.h"


SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::GameServerStartProcess);


namespace SF {
namespace GameServer {


	// Server start process
	GameServerStartProcess::GameServerStartProcess(IHeap& heap)
		: TransactionT(heap, TransactionID() )
	{
		SetExclusive(true);
		BR_TRANS_MESSAGE( Svr::TimerResult, { return OnTimer(pRes); });
	}

	GameServerStartProcess::~GameServerStartProcess()
	{
	}

	// Override delete function
	void GameServerStartProcess::Release()
	{
	}

	Result GameServerStartProcess::OnTimer(Svr::TransactionResult* pRes)
	{
		Result hr = ResultCode::SUCCESS;

		switch( m_Step )
		{
		case StartingStep::WaitEntityServer:
			if( Service::ClusterManager->GetInitialized() )
//			if( (Service::ServerEntityManager->GetEntityManagerServerEntity(pServer)) )
			{
				svrChk( InitializeServices() );
				m_Step = StartingStep::WaitInitializeComponents;
			}
			SetTimer(DurationMS(500));
			break;
		case StartingStep::WaitInitializeComponents:
			if( GetMyServer()->GetNumberServicesToWait() == 0 )
			{
				m_Step = StartingStep::Done;
			}
			SetTimer(DurationMS(500));
			break;
		case StartingStep::Done:
			CloseTransaction(hr);
			break;
		}

	Proc_End:

		if( !(hr) )
			CloseTransaction(hr);

		return hr;
	}

	Result GameServerStartProcess::InitializeServices()
	{
		Result hr = ResultCode::SUCCESS;
		
		svrChk( Service::ClusterManager->InitializeNotInitializedClusterEntities() );

	Proc_End:

		return hr;
	}

	// Start Transaction
	Result GameServerStartProcess::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		m_Step = StartingStep::WaitEntityServer;
		SetTimer(DurationMS(500) );

	Proc_End:

		return hr;
	}

	Result GameServerStartProcess::OnCloseTransaction( Result hrRes )
	{
		if( (hrRes) )
		{
			svrTrace(SVR_INFO, "Server Initialization is completed, enabling incomming connections");
			// We don't need this because all connection will be registered when it requested join by Login server
			//GetMyServer()->GetNetPublic()->SetIsEnableAccept(true);
			GetMyServer()->SetServerState(Svr::ServerState::RUNNING);
		}
		else
		{
			svrTrace(Error, "Server Initialization is failed, restart may required");
		}

		return super::OnCloseTransaction(hrRes);
	}


};// namespace GameServer 
};// namespace SF 

