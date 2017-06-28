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
#include "ServerSystem/BrService.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/MessageRoute.h"
#include "GameServerClass.h"
#include "Common/MemoryPool.h"
#include "Common/BrBaseTypes.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameIPolicy.h"

#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServerSystem/ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServerSystem/ServiceEntity/MatchingServiceEntity.h"
#include "ServerSystem/ServiceEntity/GamePartyManagerServiceEntity.h"



#include "GameServerTrans.h"


BR_MEMORYPOOL_IMPLEMENT(GameServer::GameServerStartProcess);


namespace BR {
namespace GameServer {


	// Server start process
	GameServerStartProcess::GameServerStartProcess()
		: TransactionT(TransactionID() )
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
			if( GetMyServer()->GetComponent<Svr::ClusterManagerServiceEntity>()->GetInitialized() )
//			if( (GetMyServer()->GetComponent<Svr::ServerEntityManager>()->GetEntityManagerServerEntity(pServer)) )
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
		
		svrChk( Svr::GetServerComponent<Svr::ClusterManagerServiceEntity>()->InitializeNotInitializedClusterEntities() );

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
			svrTrace(Svr::TRC_INFO, "Server Initialization is completed, enabling incomming connections");
			// We don't need this because all connection will be registered when it requested join by Login server
			//GetMyServer()->GetNetPublic()->SetIsEnableAccept(true);
			GetMyServer()->SetServerState(Svr::ServerState::RUNNING);
		}
		else
		{
			svrTrace(Trace::TRC_ERROR, "Server Initialization is failed, restart may required");
		}

		return super::OnCloseTransaction(hrRes);
	}


};// namespace GameServer 
};// namespace BR 

