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
#include "Util/TimeUtil.h"
#include "SvrConst.h"
#include "Server/BrServer.h"
#include "SvrTrace.h"
#include "Service/ServerService.h"
#include "ServerEntity/ServerEntityManager.h"
//#include "ServiceEntity/EntityManagerServiceEntity.h"

#include "ServiceEntity/Game/PlayerManagerServiceEntity.h"

#include "ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServiceEntity/MatchingServiceEntity.h"
#include "ServiceEntity/GamePartyManagerServiceEntity.h"

#include "Table/TableSystem.h"

#include "ServerEntity/GenericServerEntity.h"


#include "Protocol/Policy/EntityServerNetPolicy.h"

#include "Protocol/Policy/EntityServerNetPolicy.h"
#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"


#include "Net/NetServerPeerTCP.h"
#include "Net/NetSvrDef.h"
#include "Net/NetServerUDP.h"
#include "Entity/EntityManager.h"


#include "SharedModuleSvrConst.h"
#include "SharedModuleServerClass.h"

#include "Transaction/SharedModuleServerTrans.h"

#include "DB/GameConspiracyDB.h"
#include "DB/AccountDB.h"


#define PATH_DATABASE L"../../Data"



namespace SF {
namespace SharedModuleServer {

	


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	SharedModuleServer::SharedModuleServer()
		:BrServer(NetClass::Game)
	{
	}


	SharedModuleServer::~SharedModuleServer()
	{
	}



	// Apply configuration
	Result SharedModuleServer::ApplyConfiguration()
	{
		Result hr = ResultCode::SUCCESS;


		svrChk(Svr::BrServer::ApplyConfiguration() );


	Proc_End:

		return hr;
	}


	// Initialize server resource
	Result SharedModuleServer::InitializeServerResource()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::InitializeServerResource() );

		//svrChk( conspiracy::InitializeTable() );

		svrChk( InitializeEntity( EntityID(EntityFaculty::Server,0) ) );

	Proc_End:

		return hr;
	}

	// Close server and release resource
	Result SharedModuleServer::CloseServerResource()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::CloseServerResource() );

		svrChk( TerminateEntity() );

		svrChk(GameTable::TerminateTable() );

	Proc_End:

		return hr;
	}


	// Initialize private Network
	Result SharedModuleServer::InitializeNetPrivate()
	{
		Result hr = ResultCode::SUCCESS;
		SockFamily privateNetSockFamily;


		svrChk(Svr::BrServer::InitializeNetPrivate() );

		GetMyServer()->GetNetPrivate()->SetIsEnableAccept(true);


		privateNetSockFamily = GetMyServer()->GetNetPrivate()->GetLocalAddress().SocketFamily;




		// push Startup transaction
		{
			TransactionPtr pProcess;
			svrMem( pProcess = new(GetHeap()) SharedModuleServerStartProcess(GetHeap()) );
			svrChk( pProcess->InitializeTransaction(this) );
			svrChk( PendingTransaction(ThisThread::GetThreadID(), pProcess) );
		}


	Proc_End:

		return hr;
	}


	// Close Private Network
	Result SharedModuleServer::CloseNetPrivate()
	{
		Result hr = ResultCode::SUCCESS;

		hr = Svr::BrServer::CloseNetPrivate();


		// Server Entity Manager will clear this
		SetLoopbackServerEntity( nullptr );

		return hr;
	}



	// create remote entity by class
	Result SharedModuleServer::CreateServerEntity( NetClass netClass, Svr::ServerEntity* &pServerEntity )
	{
		pServerEntity = new(GetMemoryManager()) Svr::GenericServerEntity();

		if( pServerEntity == nullptr )
			return ResultCode::OUT_OF_MEMORY;

		return ResultCode::SUCCESS;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	//	virtual Entity interface
	//


}; // namespace SharedModuleServer
}; // namespace SF







