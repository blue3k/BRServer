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
#include "SharedModuleServerClass.h"
#include "Common/MemoryPool.h"
#include "Common/BrBaseTypes.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameIPolicy.h"

#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"

#include "SharedModuleServerTrans.h"


BR_MEMORYPOOL_IMPLEMENT(SharedModuleServer::SharedModuleServerStartProcess);


namespace BR {
namespace SharedModuleServer {


	// Server start process
	SharedModuleServerStartProcess::SharedModuleServerStartProcess()
		: TransactionT(TransactionID() )
	{
		SetExclusive(true);
		BR_TRANS_MESSAGE( Svr::TimerResult, { return OnTimer(pRes); });
	}

	SharedModuleServerStartProcess::~SharedModuleServerStartProcess()
	{
	}

	// Override delete function
	void SharedModuleServerStartProcess::Release()
	{
	}

	HRESULT SharedModuleServerStartProcess::OnTimer(Svr::TransactionResult* pRes)
	{
		HRESULT hr = S_OK;
		//Svr::ServerEntity *pServer = nullptr;

		switch( m_Step )
		{
		case StartingStep::WaitEntityServer:
			if( GetMyServer()->GetComponent<Svr::ClusterManagerServiceEntity>()->GetInitialized() )
//			if( SUCCEEDED(GetMyServer()->GetComponent<Svr::ServerEntityManager>()->GetEntityManagerServerEntity(pServer)) )
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

		if( FAILED(hr) )
			CloseTransaction(hr);

		return hr;
	}

	HRESULT SharedModuleServerStartProcess::InitializeServices()
	{
		HRESULT hr = S_OK;
		//UINT componentID = 0;
		
		svrChk( Svr::GetServerComponent<Svr::ClusterManagerServiceEntity>()->InitializeNotInitializedClusterEntities() );

		
		// Initialize all cluster entities which is not initialized by ClusterManagerServiceEntity



	Proc_End:

		return hr;
	}

	// Start Transaction
	HRESULT SharedModuleServerStartProcess::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk(super::StartTransaction() );

		m_Step = StartingStep::WaitEntityServer;
		SetTimer(DurationMS(500) );

	Proc_End:

		return hr;
	}

	HRESULT SharedModuleServerStartProcess::OnCloseTransaction( HRESULT hrRes )
	{
		if( SUCCEEDED(hrRes) )
		{
			GetMyServer()->SetServerState(Svr::ServerState::RUNNING);
			// only accept allowed connections
			//GetMyServer()->GetNetPrivate()->SetIsEnableAccept(true);
		}

		return super::OnCloseTransaction(hrRes);
	}


};// namespace SharedModuleServer 
};// namespace BR 

