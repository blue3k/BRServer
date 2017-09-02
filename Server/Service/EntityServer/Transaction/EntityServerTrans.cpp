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
#include "EntityServer.h"
#include "Net/NetServerUDP.h"
#include "ServerSystem/BrService.h"
#include "SvrTrace.h"
#include "Entity/EntityManager.h"
#include "ServerEntity/ServerEntityManager.h"
#include "Transaction/MessageRoute.h"
#include "EntityServerClass.h"
#include "Memory/MemoryPool.h"
#include "Types/SFEngineTypedefs.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameNetPolicy.h"

#include "ServiceEntity/ClusterManagerServiceEntity.h"

#include "EntityServerTrans.h"


SF_MEMORYPOOL_IMPLEMENT(BR::EntityServer::EntityServerStartProcess);

	
namespace SF {
namespace EntityServer {


	// Server start process
	EntityServerStartProcess::EntityServerStartProcess()
		: TransactionT(TransactionID() )
	{
		m_WaitCount = 0;
		SetExclusive(true);
		BR_TRANS_MESSAGE( Svr::TimerResult, { return OnTimer(pRes); });
	}

	EntityServerStartProcess::~EntityServerStartProcess()
	{
	}

	// Override delete function
	void EntityServerStartProcess::Release()
	{
	}

	Result EntityServerStartProcess::OnTimer(Svr::TransactionResult* pRes)
	{
		Result hr = ResultCode::SUCCESS;
		//Svr::ServerEntity *pServer = nullptr;
		
		switch( m_Step )
		{
		case StartingStep::WaitEntityServer:
			GetMyServer()->GetNetPrivate()->SetIsEnableAccept(true);

			if( GetMyServer()->GetComponent<Svr::ClusterManagerServiceEntity>()->GetInitialized() )
			//if( ((GetMyServer()->GetComponent<Svr::ServerEntityManager>()->GetEntityManagerServerEntity(pServer)) && pServer != Svr::BrServer::GetInstance()->GetLoopbackServerEntity())
			//	|| m_WaitCount > 6 ) // wait 6*500 ms
			{
				svrChk( InitializeServices() );
				m_Step = StartingStep::WaitInitializeComponents;
			}
			else
			{
				m_WaitCount++;
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

	Result EntityServerStartProcess::InitializeServices()
	{
		Result hr = ResultCode::SUCCESS;


		// Grab Game server list from entity
		// try connecte to it
		// Only entity server can be slave or master
		//svrChk( Svr::GetServerComponent<Svr::EntityManager>()->AddEntity( EntityFaculty::Service, Svr::GetServerComponent<Svr::ClusterManagerServiceEntity>() ) );


	//Proc_End:

		return hr;
	}

	// Start Transaction
	Result EntityServerStartProcess::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		m_Step = StartingStep::WaitEntityServer;
		SetTimer(DurationMS(500) );

	Proc_End:

		return hr;
	}



};// namespace EntityServer 
};// namespace SF 

