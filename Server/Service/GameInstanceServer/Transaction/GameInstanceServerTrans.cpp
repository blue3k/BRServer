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
#include "Shellapi.h"
#include "Net/NetServerUDP.h"
#include "ServerSystem/BrService.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/MessageRoute.h"
#include "ConspiracyGameInstanceServerClass.h"
#include "Common/MemoryPool.h"
#include "Common/BRBaseTypes.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameIPolicy.h"

#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServerSystem/ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServerSystem/ServiceEntity/MatchingServiceEntity.h"
#include "ServerSystem/ServiceEntity/GamePartyManagerServiceEntity.h"
#include "GameInstance/GameInstanceManager.h"


#include "GameInstanceServerTrans.h"


BR_MEMORYPOOL_IMPLEMENT(ConspiracyGameInstanceServer::GameInstanceServerStartProcess);


namespace BR {
namespace ConspiracyGameInstanceServer {


	// Server start process
	GameInstanceServerStartProcess::GameInstanceServerStartProcess()
		: TransactionT(TransactionID() )
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

	HRESULT GameInstanceServerStartProcess::OnTimer(Svr::TransactionResult* pRes)
	{
		HRESULT hr = S_OK;
		Svr::ServerEntity *pServer = nullptr;

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

	HRESULT GameInstanceServerStartProcess::InitializeServices()
	{
		HRESULT hr = S_OK;
		UINT componentID = 0;
		
		svrChk( Svr::GetServerComponent<Svr::ClusterManagerServiceEntity>()->InitializeNotInitializedClusterEntities() );

		
		// Initialize all cluster entities which is not initialized by ClusterManagerServiceEntity



	Proc_End:

		return hr;
	}

	// Start Transaction
	HRESULT GameInstanceServerStartProcess::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( __super::StartTransaction() );

		m_Step = StartingStep::WaitEntityServer;
		SetTimer(DurationMS(500) );

	Proc_End:

		return hr;
	}

	HRESULT GameInstanceServerStartProcess::OnCloseTransaction( HRESULT hrRes )
	{
		if( SUCCEEDED(hrRes) )
		{
			// New connection will not be allowed.
			// The only way to make new connection is registering from a login server by loggin
			//GetMyServer()->GetNetPublic()->SetIsEnableAccept(true);
			GetMyServer()->SetServerState(Svr::ServerState::RUNNING);
		}

		return __super::OnCloseTransaction(hrRes);
	}


};// namespace ConspiracyGameInstanceServer 
};// namespace BR 

