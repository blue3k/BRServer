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
#include "GameServerClass.h"
#include "Common/MemoryPool.h"
#include "Common/BRBaseTypes.h"
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

	HRESULT GameServerStartProcess::OnTimer(Svr::TransactionResult* pRes)
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
			SetTimer(500);
			break;
		case StartingStep::WaitInitializeComponents:
			if( GetMyServer()->GetNumberServicesToWait() == 0 )
			{
				m_Step = StartingStep::Done;
			}
			SetTimer(500);
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

	HRESULT GameServerStartProcess::InitializeServices()
	{
		HRESULT hr = S_OK;
		UINT componentID = 0;
		
		svrChk( Svr::GetServerComponent<Svr::ClusterManagerServiceEntity>()->InitializeNotInitializedClusterEntities() );

		
		// Initialize all cluster entities which is not initialized by ClusterManagerServiceEntity

		//// Queue items
		//componentID = Svr::ServerComponentID_MatchingQueueWatcherService_4x1;
		//for( ClusterID matchingQueueClusterID = ClusterID::MatchingQueue_Game_4x1; matchingQueueClusterID <= ClusterID::MatchingQueue_Game_12x11; matchingQueueClusterID++, componentID++ )
		//{
		//	svrChk( Svr::GetServerComponent<Svr::MatchingQueueWatcherServiceEntity>(componentID)->StartInitializeTransaction() );
		//}


		//// Adding matching entities
		//componentID = Svr::ServerComponentID_MatchingWatcherService_4;
		//for( ClusterID matchingQueueClusterID = ClusterID::Matching_Game_4; matchingQueueClusterID <= ClusterID::Matching_Game_12; matchingQueueClusterID++, componentID++ )
		//{
		//	svrChk( Svr::GetServerComponent<Svr::MatchingWatcherServiceEntity>(componentID)->StartInitializeTransaction() );
		//}

		//svrChk( Svr::GetServerComponent<Svr::GamePartyManagerWatcherServiceEntity>()->StartInitializeTransaction() );



	Proc_End:

		return hr;
	}

	// Start Transaction
	HRESULT GameServerStartProcess::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( __super::StartTransaction() );

		m_Step = StartingStep::WaitEntityServer;
		SetTimer( 500 );

	Proc_End:

		return hr;
	}

	HRESULT GameServerStartProcess::OnCloseTransaction( HRESULT hrRes )
	{
		if( SUCCEEDED(hrRes) )
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

		return __super::OnCloseTransaction(hrRes);
	}


};// namespace GameServer 
};// namespace BR 

