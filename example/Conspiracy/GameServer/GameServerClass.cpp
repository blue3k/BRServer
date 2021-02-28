////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : KyungKun Ko
//
// Description : Entity server implementation
//
////////////////////////////////////////////////////////////////////////////////



#include "GameServerPCH.h"
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
#include "ServiceEntity/Game/GameInstanceManagerServiceEntity.h"
#include "ServiceEntity/Game/GameServiceEntity.h"

#include "TableSystem.h"

#include "ServerEntity/GenericServerEntity.h"

#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"

#include "GameInstance/GamePlayerEntity.h"
#include "Net/SFNetSvrDef.h"
#include "GameSvrConst.h"
#include "GameServerClass.h"
#include "Transaction/GameServerTrans.h"

#include "GameConspiracyDB.h"
#include "DB/GameTransactionDB.h"
#include "DB/AccountDB.h"
#include "DB/RankingDB.h"
#include "DB/LoginSessionDB.h"






namespace SF {
namespace GameServer {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	GameServer::GameServer()
		: BrServer()
		//, m_pNetPublic(nullptr)
		, m_TableVersion(0)
		, m_PresetGameConfigID(1)
		, m_PresetGameConfig(nullptr)
	{
	}


	GameServer::~GameServer()
	{
	}



	// Update game config
	Result GameServer::UpdateGameConfig(uint configPresetID)
	{
		Result hr = ResultCode::SUCCESS;

		GameConfigType *pGameConfig = nullptr;
		svrChk( conspiracy::GameConfigTbl::FindItem(configPresetID, pGameConfig ) );

		// set value only if it succeeded
		m_TableVersion = GameTable::GetTableVersion();
		m_PresetGameConfigID = configPresetID;
		m_PresetGameConfig = pGameConfig;

	Proc_End:

		return hr;
	}

	// Initialize server resource
	Result GameServer::InitializeServerResource()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::InitializeServerResource() );

		svrChk(GameTable::InitializeTable() );

		svrChk( UpdateGameConfig(m_PresetGameConfigID) );


	Proc_End:

		return hr;
	}

	// Close server and release resource
	Result GameServer::CloseServerResource()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::CloseServerResource() );

		svrChk( TerminateEntity() );

		svrChk(GameTable::TerminateTable() );

	Proc_End:

		return hr;
	}


	Svr::Entity* GameServer::CreateEntity(ClusterID clusterID, EntityFaculty faculty)
	{
		if(clusterID == ClusterID::Game && faculty == EntityFaculty::User)
		{
			return new(GetHeap()) GamePlayerEntity;
		}

		return nullptr;
	}


	// Initialize private Network
	Result GameServer::InitializeEntities()
	{
		Result hr = ResultCode::SUCCESS;

		Service::EntityManager->RegisterEntityCreator([this](ClusterID clusterID, EntityFaculty faculty) { return CreateEntity(clusterID, faculty); });

		//Engine::GetInstance()->AddComponent<SF::GameServer::GameEntityManager>(Service::ServerConfig->WorkerThreadCount);

		svrChk(Svr::BrServer::InitializeEntities() );

		// Account DB
		svrChk(AddDBCluster<DB::AccountDB>(Service::ServerConfig->FindDBCluster("AccountDB")));

		svrChk(AddDBCluster<DB::LoginSessionDB>(Service::ServerConfig->FindDBCluster("LoginSessionDB")));

		// Game DB initialize
		svrChk(AddDBCluster<conspiracy::GameConspiracyDB>(Service::ServerConfig->FindDBCluster("GameDB")));

		svrChk(AddDBCluster<DB::GameTransactionDB>(Service::ServerConfig->FindDBCluster("GameTransactionLogDB")));

		// Ranking DB 
		svrChk(AddDBCluster<DB::RankingDB>(Service::ServerConfig->FindDBCluster("RankingDB")) );

		// push Startup transaction
		{
			TransactionPtr pProcess;
			svrMem( pProcess = new(GetHeap()) GameServerStartProcess(GetHeap()) );
			svrChk( pProcess->InitializeTransaction(this) );
			svrChk( PendingTransaction(ThisThread::GetThreadID(),pProcess) );
		}


		//////////////////////////////////////////////////////////////////////////////////////////
		//
		// Register service entities
		//

		// Queue items
		for (ClusterID matchingQueueClusterID = ClusterID::MatchingQueue_Game_4x1; static_cast<uint32_t>(matchingQueueClusterID) <= ClusterID_MatchingQueue_Max; matchingQueueClusterID++)
		{
			svrChk(Service::ServiceDirectory->WatchForService( Service::ServerConfig->GameClusterID, matchingQueueClusterID) );
		}


		// Adding matching entities
		for( ClusterID matchingClusterID = ClusterID::Matching_Game_4; matchingClusterID <= ClusterID::Matching_Game_8; matchingClusterID++ )
		{
			svrChk(Service::ServiceDirectory->WatchForService(Service::ServerConfig->GameClusterID, matchingClusterID));
		}



		svrChk(Service::ServiceDirectory->WatchForService(Service::ServerConfig->GameClusterID, ClusterID::GameInstanceManager));
		svrChk(Service::ServiceDirectory->WatchForService(Service::ServerConfig->GameClusterID, ClusterID::GamePartyManager));




	Proc_End:

		return hr;
	}


	// Close Private Network
	Result GameServer::CloseEntities()
	{
		Result hr = ResultCode::SUCCESS;

		hr = Svr::BrServer::CloseEntities();

		return hr;
	}



	// Run the task
	Result GameServer::TickUpdate(TimerAction *pAction)
	{
		if (m_TableVersion != GameTable::GetTableVersion())
		{
			UpdateGameConfig(m_PresetGameConfigID);
		}

		return Svr::BrServer::TickUpdate(pAction);
	}


	// create remote entity by class
	Result GameServer::CreateServerEntity( Svr::ServerEntity* &pServerEntity )
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



	


}; // namespace GameServer
}; // namespace SF







