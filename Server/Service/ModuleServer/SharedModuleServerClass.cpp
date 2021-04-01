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
#include "Util/SFTimeUtil.h"
#include "SvrConst.h"
#include "Server/BrServer.h"
#include "SvrTrace.h"
#include "Service/ServerService.h"
#include "ServerEntity/ServerEntityManager.h"
#include "ServiceEntity/Game/PlayerDirectoryManager.h"
#include "ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServiceEntity/MatchingServiceEntity.h"
#include "ServiceEntity/Party/GamePartyManagerServiceEntity.h"
#include "ServerEntity/GenericServerEntity.h"

#include "Protocol/GameServerNetPolicy.h"
#include "Protocol/GameInstanceNetPolicy.h"

#include "Net/SFNetServerPeerTCP.h"
#include "Net/SFNetSvrDef.h"
#include "Net/SFNetServerUDP.h"
#include "Entity/EntityManager.h"

#include "SharedModuleSvrConst.h"
#include "SharedModuleServerClass.h"

#include "Transaction/SharedModuleServerTrans.h"

#include "DB/AccountDB.h"



namespace SF {
namespace SharedModuleServer {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	SharedModuleServer::SharedModuleServer()
		:BrServer()
	{
	}

	SharedModuleServer::~SharedModuleServer()
	{
	}

	// Initialize private Network
	Result SharedModuleServer::InitializeEntities()
	{
		ScopeContext hr;
		SockFamily privateNetSockFamily;

		svrCheck(Svr::BrServer::InitializeEntities() );

		// push Startup transaction
		{
			TransactionPtr pProcess;
			svrCheckMem( pProcess = new(GetHeap()) SharedModuleServerStartProcess(GetHeap()) );
			svrCheck( pProcess->InitializeTransaction(this) );
			svrCheck( PendingTransaction(ThisThread::GetThreadID(), pProcess) );
		}

		return hr;
	}

	// Close Private Network
	Result SharedModuleServer::CloseEntities()
	{
		Result hr = ResultCode::SUCCESS;

		hr = Svr::BrServer::CloseEntities();

		return hr;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	//	virtual Entity interface
	//


} // namespace SharedModuleServer
} // namespace SF

