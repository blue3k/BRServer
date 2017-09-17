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
#include "ConspiracyGameInstanceServerClass.h"
#include "Memory/MemoryPool.h"
#include "Types/SFEngineTypedefs.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameNetPolicy.h"

#include "ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServiceEntity/MatchingServiceEntity.h"
#include "ServiceEntity/GamePartyManagerServiceEntity.h"
#include "GameInstance/GameInstanceManager.h"


#include "GameInstanceServerTrans.h"


SF_MEMORYPOOL_IMPLEMENT(BR::ConspiracyGameInstanceServer::GameInstanceServerStartProcess);


namespace SF {
namespace ConspiracyGameInstanceServer {


	// Server start process
	GameInstanceServerStartProcess::GameInstanceServerStartProcess(IHeap &heap)
		: TransactionT(heap, TransactionID() )
	{
		SetExclusive(true);
		BR_TRANS_MESSAGE( Svr::TimerResult, { return OnTimer(pRes); });
	}

	GameInstanceServerStartProcess::~GameInstanceServerStartProcess()
	{
	}

	// Override delete function
	void GameInstanceServerStartProcess::Release()
	{
	}

	Result GameInstanceServerStartProcess::OnTimer(Svr::TransactionResult* pRes)
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

	Result GameInstanceServerStartProcess::InitializeServices()
	{
		Result hr = ResultCode::SUCCESS;
		
		svrChk( Service::ClusterManager->InitializeNotInitializedClusterEntities() );
		

	Proc_End:

		return hr;
	}

	// Start Transaction
	Result GameInstanceServerStartProcess::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		m_Step = StartingStep::WaitEntityServer;
		SetTimer(DurationMS(500) );

	Proc_End:

		return hr;
	}

	Result GameInstanceServerStartProcess::OnCloseTransaction( Result hrRes )
	{
		if( (hrRes) )
		{
			// New connection will not be allowed.
			// The only way to make new connection is registering from a login server by loggin
			//GetMyServer()->GetNetPublic()->SetIsEnableAccept(true);
			GetMyServer()->SetServerState(Svr::ServerState::RUNNING);
		}

		return super::OnCloseTransaction(hrRes);
	}


};// namespace ConspiracyGameInstanceServer 
};// namespace SF 

