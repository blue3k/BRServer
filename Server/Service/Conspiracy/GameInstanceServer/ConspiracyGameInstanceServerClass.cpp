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
		:BrServer(NetClass::Game)
		,m_pGameClusterCfg(nullptr)
	{
	}


	GameInstanceServer::~GameInstanceServer()
	{
	}






	// Apply configuration
	Result GameInstanceServer::ApplyConfiguration()
	{
		Result hr = ResultCode::SUCCESS;

		svrChkPtr(GetMyConfig());

		m_pGameClusterCfg = GetMyConfig()->pGameCluster;
		svrChkPtr( m_pGameClusterCfg );

		svrChk(Svr::BrServer::ApplyConfiguration() );


	Proc_End:

		return hr;
	}


	// Initialize server resource
	Result GameInstanceServer::InitializeServerResource()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::InitializeServerResource() );

		svrChk(GameTable::InitializeTable(Service::ServerConfig) );

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
	Result GameInstanceServer::InitializeNetPrivate()
	{
		Result hr = ResultCode::SUCCESS;
		SockFamily privateNetSockFamily;

		Service::EntityManager->RegisterEntityCreator([this](ClusterID clusterID, EntityFaculty faculty) { return CreateEntity(clusterID, faculty); });

		svrChk(Svr::BrServer::InitializeNetPrivate() );

		GetMyServer()->GetNetPrivate()->SetIsEnableAccept(true);

		privateNetSockFamily = GetMyServer()->GetNetPrivate()->GetLocalAddress().SocketFamily;



		//////////////////////////////////////////////////////////////////////////////////////////
		//
		// Register service entities
		//

		{
		GameInstanceManagerServiceEntity *pGameInstanceManager = nullptr;
		svrMem( pGameInstanceManager = new(GetHeap()) GameInstanceManagerServiceEntity(Svr::GetServerGameID(), ClusterID::GameInstanceManager, ClusterMembership::Slave) );
		svrChk(Service::EntityManager->AddEntity( EntityFaculty::Service, pGameInstanceManager ) );
		svrChk( Service::ClusterManager->AddClusterServiceEntity( pGameInstanceManager ) );
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
	Result GameInstanceServer::CloseNetPrivate()
	{
		Result hr = ResultCode::SUCCESS;

		hr = Svr::BrServer::CloseNetPrivate();


		// Server Entity Manager will clear this
		SetLoopbackServerEntity( nullptr );

		return hr;
	}



	// create remote entity by class
	Result GameInstanceServer::CreateServerEntity( NetClass netClass, Svr::ServerEntity* &pServerEntity )
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




}; // namespace ConspiracyGameInstanceServer
}; // namespace SF







