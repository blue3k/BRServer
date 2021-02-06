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

#include "ServiceEntity/Game/PlayerManagerServiceEntity.h"

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


#define PATH_DATABASE L"../../Data"



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






	// Apply configuration
	Result GameInstanceServer::ApplyConfiguration()
	{
		Result hr = ResultCode::SUCCESS;

		svrCheck(Svr::BrServer::ApplyConfiguration() );

		return hr;
	}


	// Initialize server resource
	Result GameInstanceServer::InitializeServerResource()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::InitializeServerResource() );

		svrChk(GameTable::InitializeTable() );

		svrChk( InitializeEntity( EntityID(EntityFaculty::Server,0) ) );

	Proc_End:

		return hr;
	}

	// Close server and release resource
	Result GameInstanceServer::CloseServerResource()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::CloseServerResource() );

		svrChk( TerminateEntity() );

		svrChk(GameTable::TerminateTable() );

	Proc_End:

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
		Result hr = ResultCode::SUCCESS;
		SockFamily privateNetSockFamily;

		Service::EntityManager->RegisterEntityCreator([this](ClusterID clusterID, EntityFaculty faculty) { return CreateEntity(clusterID, faculty); });

		svrChk(Svr::BrServer::InitializeEntities() );



		//////////////////////////////////////////////////////////////////////////////////////////
		//
		// Register service entities
		//

		{
		GameInstanceManagerServiceEntity *pGameInstanceManager = nullptr;
		svrMem( pGameInstanceManager = new(GetHeap()) GameInstanceManagerServiceEntity(Svr::GetServerGameID(), nullptr, ClusterID::GameInstanceManager, ClusterMembership::Slave) );
		svrChk(Service::EntityManager->AddEntity( EntityFaculty::Service, pGameInstanceManager ) );
		svrChk(GetComponentCarrier().AddComponentWithAdapter(pGameInstanceManager) );
		}


		// push Startup transaction
		{
			TransactionPtr pProcess;
			svrMem( pProcess = new(GetHeap()) GameInstanceServerStartProcess(GetHeap()) );
			svrChk( pProcess->InitializeTransaction(this) );
			svrChk(PendingTransaction(ThisThread::GetThreadID(), pProcess));
		}



	Proc_End:

		return hr;
	}


	// Close Private Network
	Result GameInstanceServer::CloseEntities()
	{
		Result hr = ResultCode::SUCCESS;

		hr = Svr::BrServer::CloseEntities();


		return hr;
	}




	//////////////////////////////////////////////////////////////////////////
	//
	//	virtual Entity interface
	//




}; // namespace ConspiracyGameInstanceServer
}; // namespace SF







