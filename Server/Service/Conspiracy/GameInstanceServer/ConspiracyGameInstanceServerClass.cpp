////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : KyungKun Ko
//
// Description : Entity server implementation
//
////////////////////////////////////////////////////////////////////////////////



#include "GameInstancePCH.h"
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
#include "GameInstance/GameInstanceManager.h"

#include "TableSystem.h"

#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"

#include "Net/SFNetSvrDef.h"
#include "Entity/EntityManager.h"
#include "GameInstance/GameInstanceEntity.h"


#include "ConspiracyGameInstanceSvrConst.h"
#include "ConspiracyGameInstanceServerClass.h"

#include "Transaction/GameInstanceServerTrans.h"


#include "GameConspiracyDB.h"
#include "DB/AccountDB.h"


namespace SF {
namespace ConspiracyGameInstanceServer {

	

	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	GameInstanceServer::GameInstanceServer()
		:BrServer()
	{
	}


	GameInstanceServer::~GameInstanceServer()
	{
	}


	// Initialize server resource
	Result GameInstanceServer::InitializeServerResource()
	{
		Result hr;

		svrCheck(Svr::BrServer::InitializeServerResource() );

		svrCheck(GameTable::InitializeTable() );

		return hr;
	}

	// Close server and release resource
	Result GameInstanceServer::CloseServerResource()
	{
		Result hr;

		svrCheck(Svr::BrServer::CloseServerResource() );

		svrCheck( TerminateEntity() );

		svrCheck(GameTable::TerminateTable() );

		return hr;
	}

	Svr::Entity* GameInstanceServer::CreateEntity(ClusterID clusterID, EntityFaculty faculty)
	{
		if (clusterID == ClusterID::GameInstanceManager && faculty == EntityFaculty::GameInstance)
		{
			return new(GetHeap()) GameInstanceEntity;
		}

		return nullptr;
	}

	// Initialize private Network
	Result GameInstanceServer::InitializeEntities()
	{
		Result hr;
		SockFamily privateNetSockFamily;

		Service::EntityManager->RegisterEntityCreator([this](ClusterID clusterID, EntityFaculty faculty) { return CreateEntity(clusterID, faculty); });

		svrCheck(Svr::BrServer::InitializeEntities() );


		//////////////////////////////////////////////////////////////////////////////////////////
		//
		// Register service entities
		//

		{
			assert(false);
			// FIXME: endpoint address has been refactored
			GameInstanceManagerServiceEntity *pGameInstanceManager = nullptr;
			svrCheckMem(pGameInstanceManager = new(GetHeap()) GameInstanceManagerServiceEntity(Service::ServerConfig->GameClusterID, nullptr, ClusterID::GameInstanceManager, {}));
			svrCheck(Service::EntityManager->AddEntity( EntityFaculty::Service, pGameInstanceManager ) );
		}


		// push Startup transaction
		{
			TransactionPtr pProcess;
			svrCheckMem( pProcess = new(GetHeap()) GameInstanceServerStartProcess(GetHeap()) );
			svrCheck( pProcess->InitializeTransaction(this) );
			svrCheck(PendingTransaction(ThisThread::GetThreadID(), pProcess));
		}

		return hr;
	}


	// Close Private Network
	Result GameInstanceServer::CloseEntities()
	{
		Result hr = ResultCode::SUCCESS;

		hr = Svr::BrServer::CloseEntities();

		return hr;
	}


} // namespace ConspiracyGameInstanceServer
} // namespace SF







