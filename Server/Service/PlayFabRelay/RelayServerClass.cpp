////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2019 The Braves Corporation.
// 
// Author : KyungKun Ko
//
// Description : RelayServerClass
//
////////////////////////////////////////////////////////////////////////////////

#include "ModuleServerPCH.h"


#include "ModuleServerPCH.h"
#include "Util/SFTimeUtil.h"
#include "SvrConst.h"
#include "Server/BrServer.h"
#include "SvrTrace.h"
#include "Service/ServerService.h"
#include "ServerEntity/ServerEntityManager.h"


#include "ServiceEntity/Game/PlayerManagerServiceEntity.h"

#include "ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServiceEntity/MatchingServiceEntity.h"
#include "ServiceEntity/GamePartyManagerServiceEntity.h"

#include "Table/TableSystem.h"

#include "ServerEntity/GenericServerEntity.h"

#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"


#include "Net/SFNetServerPeerTCP.h"
#include "Net/SFNetSvrDef.h"
#include "Net/SFNetServerUDP.h"
#include "Entity/EntityManager.h"


#include "SharedModuleSvrConst.h"
#include "RelayServerClass.h"

#include "Transaction/RelayServerTrans.h"

#include "DB/GameConspiracyDB.h"
#include "DB/AccountDB.h"


#define PATH_DATABASE L"../../Data"



namespace SF {

	
	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	RelayServer::RelayServer()
		:BrServer(NetClass::Game)
	{
	}


	RelayServer::~RelayServer()
	{
	}



	// Apply configuration
	Result RelayServer::ApplyConfiguration()
	{
		Result hr = ResultCode::SUCCESS;


		svrChk(Svr::BrServer::ApplyConfiguration() );


	Proc_End:

		return hr;
	}


	// Initialize server resource
	Result RelayServer::InitializeServerResource()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::InitializeServerResource() );

		//svrChk( conspiracy::InitializeTable() );

		svrChk( InitializeEntity( EntityID(EntityFaculty::Server,0) ) );

	Proc_End:

		return hr;
	}

	// Close server and release resource
	Result RelayServer::CloseServerResource()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::CloseServerResource() );

		svrChk( TerminateEntity() );

		svrChk(GameTable::TerminateTable() );

	Proc_End:

		return hr;
	}


	// Initialize private Network
	Result RelayServer::InitializeNetPrivate()
	{
		Result hr = ResultCode::SUCCESS;
		SockFamily privateNetSockFamily;


		svrChk(Svr::BrServer::InitializeNetPrivate() );

		GetMyServer()->GetNetPrivate()->SetIsEnableAccept(true);

		privateNetSockFamily = GetMyServer()->GetNetPrivate()->GetLocalAddress().SocketFamily;

		// push Startup transaction
		{
			TransactionPtr pProcess;
			svrMem( pProcess = new(GetHeap()) RelayServerStartProcess(GetHeap()) );
			svrChk( pProcess->InitializeTransaction(this) );
			svrChk( PendingTransaction(ThisThread::GetThreadID(), pProcess) );
		}


	Proc_End:

		return hr;
	}


	// Close Private Network
	Result RelayServer::CloseNetPrivate()
	{
		Result hr = ResultCode::SUCCESS;

		hr = Svr::BrServer::CloseNetPrivate();


		// Server Entity Manager will clear this
		SetLoopbackServerEntity( nullptr );

		return hr;
	}



	// create remote entity by class
	Result RelayServer::CreateServerEntity( NetClass netClass, Svr::ServerEntity* &pServerEntity )
	{
		pServerEntity = new(GetHeap()) Svr::GenericServerEntity();

		if( pServerEntity == nullptr )
			return ResultCode::OUT_OF_MEMORY;

		return ResultCode::SUCCESS;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	//	virtual Entity interface
	//


}; // namespace SF







