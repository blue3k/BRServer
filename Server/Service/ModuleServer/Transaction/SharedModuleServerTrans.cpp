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
#include "SvrTrace.h"
#include "Entity/EntityManager.h"
#include "ServerEntity/ServerEntityManager.h"
#include "Transaction/MessageRoute.h"
#include "SharedModuleServerClass.h"
#include "Memory/MemoryPool.h"
#include "Types/SFEngineTypedefs.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameNetPolicy.h"

#include "ServiceEntity/ClusterManagerServiceEntity.h"

#include "SharedModuleServerTrans.h"


SF_MEMORYPOOL_IMPLEMENT(BR::SharedModuleServer::SharedModuleServerStartProcess);


namespace SF {
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

	Result SharedModuleServerStartProcess::OnTimer(Svr::TransactionResult* pRes)
	{
		Result hr = ResultCode::SUCCESS;
		//Svr::ServerEntity *pServer = nullptr;

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

	Result SharedModuleServerStartProcess::InitializeServices()
	{
		Result hr = ResultCode::SUCCESS;
		//uint componentID = 0;
		
		svrChk( Svr::GetServerComponent<Svr::ClusterManagerServiceEntity>()->InitializeNotInitializedClusterEntities() );

		
		// Initialize all cluster entities which is not initialized by ClusterManagerServiceEntity



	Proc_End:

		return hr;
	}

	// Start Transaction
	Result SharedModuleServerStartProcess::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::StartTransaction() );

		m_Step = StartingStep::WaitEntityServer;
		SetTimer(DurationMS(500) );

	Proc_End:

		return hr;
	}

	Result SharedModuleServerStartProcess::OnCloseTransaction( Result hrRes )
	{
		if( (hrRes) )
		{
			GetMyServer()->SetServerState(Svr::ServerState::RUNNING);
			// only accept allowed connections
			//GetMyServer()->GetNetPrivate()->SetIsEnableAccept(true);
		}

		return super::OnCloseTransaction(hrRes);
	}


};// namespace SharedModuleServer 
};// namespace SF 

