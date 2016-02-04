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
#include "LoginServer.h"
#include "ServerSystem/BrService.h"
#include "ServerSystem/SvrTrace.h"
#include "LoginServerClass.h"
#include "Common/MemoryPool.h"
#include "Common/BrBaseTypes.h"
#include "Net/NetServerUDP.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameIPolicy.h"
#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/MessageRoute.h"

#include "LoginServerTrans.h"



BR_MEMORYPOOL_IMPLEMENT(LoginServer::LoginServerStartProcess);
	
namespace BR {
namespace LoginServer {


	// Server start process
	LoginServerStartProcess::LoginServerStartProcess()
		: TransactionT(TransactionID() )
	{
		SetExclusive(true);
		BR_TRANS_MESSAGE( Svr::TimerResult, { return OnTimer(pRes); });
	}

	LoginServerStartProcess::~LoginServerStartProcess()
	{
	}

	// Override delete function
	void LoginServerStartProcess::Release()
	{
	}

	HRESULT LoginServerStartProcess::OnTimer(Svr::TransactionResult* pRes)
	{
		HRESULT hr = S_SYSTEM_OK;
		//Svr::ServerEntity *pServer = nullptr;

		switch( m_Step )
		{
		case StartingStep::WaitEntityServer:
			if( GetMyServer()->GetComponent<Svr::ClusterManagerServiceEntity>()->GetInitialized() )
			{
				svrTrace( Trace::TRC_TRACE, "Initializing services" );
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

		if( FAILED(hr) )
			CloseTransaction(hr);

		return hr;
	}

	HRESULT LoginServerStartProcess::InitializeServices()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk( Svr::GetServerComponent<Svr::ClusterManagerServiceEntity>()->InitializeNotInitializedClusterEntities() );

	Proc_End:

		return hr;
	}

	HRESULT LoginServerStartProcess::OnCloseTransaction( HRESULT hrRes )
	{
		if( SUCCEEDED(hrRes) )
		{
			svrTrace(Svr::TRC_INFO, "Server Initialization is completed, enabling incomming connections");
			GetMyServer()->GetNetPublic()->SetIsEnableAccept(true);
			GetMyServer()->SetServerState(Svr::ServerState::RUNNING);
		}
		else
		{
			svrTrace(Trace::TRC_ERROR, "Server Initialization is failed, restart may required");
		}

		return super::OnCloseTransaction(hrRes);
	}

	// Start Transaction
	HRESULT LoginServerStartProcess::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk( super::StartTransaction() );

		m_Step = StartingStep::WaitEntityServer;
		SetTimer( DurationMS(500) );

	Proc_End:

		return hr;
	}



};// namespace LoginServer 
};// namespace BR 

