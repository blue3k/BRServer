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
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/MessageRoute.h"
#include "EntityServerClass.h"
#include "Common/MemoryPool.h"
#include "Common/BrBaseTypes.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameIPolicy.h"

#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"

#include "EntityServerTrans.h"


BR_MEMORYPOOL_IMPLEMENT(EntityServer::EntityServerStartProcess);

	
namespace BR {
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

	HRESULT EntityServerStartProcess::OnTimer(Svr::TransactionResult* pRes)
	{
		HRESULT hr = S_SYSTEM_OK;
		//Svr::ServerEntity *pServer = nullptr;
		
		switch( m_Step )
		{
		case StartingStep::WaitEntityServer:
			GetMyServer()->GetNetPrivate()->SetIsEnableAccept(true);

			if( GetMyServer()->GetComponent<Svr::ClusterManagerServiceEntity>()->GetInitialized() )
			//if( (SUCCEEDED(GetMyServer()->GetComponent<Svr::ServerEntityManager>()->GetEntityManagerServerEntity(pServer)) && pServer != Svr::BrServer::GetInstance()->GetLoopbackServerEntity())
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

		if( FAILED(hr) )
			CloseTransaction(hr);

		return hr;
	}

	HRESULT EntityServerStartProcess::InitializeServices()
	{
		HRESULT hr = S_SYSTEM_OK;


		// Grab Game server list from entity
		// try connecte to it
		// Only entity server can be slave or master
		//svrChk( Svr::GetServerComponent<Svr::EntityManager>()->AddEntity( EntityFaculty::Service, Svr::GetServerComponent<Svr::ClusterManagerServiceEntity>() ) );


	//Proc_End:

		return hr;
	}

	// Start Transaction
	HRESULT EntityServerStartProcess::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk( super::StartTransaction() );

		m_Step = StartingStep::WaitEntityServer;
		SetTimer(DurationMS(500) );

	Proc_End:

		return hr;
	}



};// namespace EntityServer 
};// namespace BR 

