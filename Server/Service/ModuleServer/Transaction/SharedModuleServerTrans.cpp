////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : KyungKun Ko
//
// Description : Entity server implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "ModuleServerPCH.h"
#include "GameServer.h"
#include "Net/SFNetServerUDP.h"
#include "Server/BrService.h"
#include "SvrTrace.h"
#include "Entity/EntityManager.h"
#include "ServerEntity/ServerEntityManager.h"
#include "Transaction/MessageRoute.h"
#include "SharedModuleServerClass.h"
#include "MemoryManager/SFMemoryPool.h"
#include "Types/SFEngineTypedefs.h"
#include "Protocol/GameMsgClass.h"
#include "Protocol/GameNetPolicy.h"



#include "SharedModuleServerTrans.h"




namespace SF {
namespace SharedModuleServer {


	// Server start process
	SharedModuleServerStartProcess::SharedModuleServerStartProcess(IHeap& heap)
		: TransactionT(heap, TransactionID() )
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
		SF::ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});

		switch( m_Step )
		{
		case StartingStep::StartInitialization:
			//if( Service::ServiceDirectory->GetIsInitialized() )
			{
				svrChkReturn( InitializeServices() );
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

		return hr;
	}

	Result SharedModuleServerStartProcess::InitializeServices()
	{
		Result hr = ResultCode::SUCCESS;

		return hr;
	}

	// Start Transaction
	Result SharedModuleServerStartProcess::StartTransaction()
	{
		ScopeContext hr = ResultCode::SUCCESS;

		svrCheck(super::StartTransaction() );

		m_Step = StartingStep::StartInitialization;
		SetTimer(DurationMS(500));

		return hr;
	}

	Result SharedModuleServerStartProcess::OnCloseTransaction( Result hrRes )
	{
		if( (hrRes) )
		{
			GetMyServer()->SetServerState(ServerState::RUNNING);
			// only accept allowed connections
			//GetMyServer()->GetNetPrivate()->SetIsEnableAccept(true);
		}

		return super::OnCloseTransaction(hrRes);
	}


}// namespace SharedModuleServer 
}// namespace SF 

