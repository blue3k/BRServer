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
#include "Common/TimeUtil.h"
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/SvrConfig.h"
#include "ServerSystem/ServerEntityManager.h"

#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServerSystem/ServiceEntity/Game/GameClusterServiceEntity.h"
#include "ServerSystem/ServiceEntity/Game/GameInstanceManagerServiceEntity.h"
#include "ServerSystem/ServiceEntity/Login/LoginServiceEntity.h"
#include "ServerSystem/ServiceEntity/Game/GameServiceEntity.h"
#include "ServerSystem/ServiceEntity/Game/GameInstanceServiceEntity.h"

#include "ServerSystem/ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServerSystem/ServiceEntity/MatchingServiceEntity.h"
#include "ServerSystem/ServiceEntity/GamePartyManagerServiceEntity.h"
#include "ServerSystem/ServiceEntity/RankingServiceEntity.h"
#include "ServerSystem/ServiceEntity/MonitoringServiceEntity.h"

#include "Table/TableSystem.h"

#include "ServerSystem/ServerEntity/EntityServerEntity.h"


#include "Protocol/Policy/EntityServerIPolicy.h"

#include "Protocol/Policy/EntityServerIPolicy.h"
#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Policy/GameInstanceIPolicy.h"
#include "Protocol/Policy/GameMasterServerIPolicy.h"

#include "Net/NetServerPeer.h"
#include "Net/NetServer.h"
#include "Net/NetServerUDP.h"
#include "ServerSystem/EntityManager.h"


#include "SharedModuleSvrConst.h"
#include "SharedModuleServerClass.h"

#include "Transaction/SharedModuleServerTrans.h"

#include "DB/GameConspiracyDB.h"
#include "DB/AccountDB.h"






namespace BR {
namespace SharedModuleServer {


	template< class ServiceEntityType, typename... ConstructorArgs >
	Result SharedModuleServer::AddServiceEntityComponent(ConstructorArgs... constructorArgs)
	{
		Result hr = ResultCode::SUCCESS;
		Svr::IServerComponent *pComponent = nullptr;
		Svr::ClusteredServiceEntity *pServiceEntityTest = nullptr;
		ServiceEntityType* pServiceEntity = nullptr;

		svrMem(pServiceEntity = new ServiceEntityType(constructorArgs...));

		if( (GetComponent<Svr::ClusterManagerServiceEntity>()->GetClusterServiceEntity( pServiceEntity->GetClusterID(), pServiceEntityTest )) )
		{
			if (pServiceEntityTest == pServiceEntity)
				goto Proc_End;

			if (pServiceEntity->GetClusterMembership() <= ClusterMembership::StatusWatcher)
			{
				svrTrace(Trace::TRC_WARN, "Duplicated entity watcher entity {0} EntityUID:{1}, while adding {2} EntityUID:{3}", typeid(*pServiceEntityTest).name(), pServiceEntityTest->GetEntityUID(), typeid(*pServiceEntity).name(), pServiceEntity->GetEntityUID());
			}
			else
			{
				svrTrace(Trace::TRC_ERROR, "Duplicated cluster entity {0} EntityUID:{1}, while adding {2} EntityUID:{3}", typeid(*pServiceEntityTest).name(), pServiceEntityTest->GetEntityUID(), typeid(*pServiceEntity).name(), pServiceEntity->GetEntityUID());
			}
			delete pServiceEntity;
			goto Proc_End;
		}

		svrChk( GetComponent<Svr::EntityManager>()->AddEntity( EntityFaculty::Service, pServiceEntity ) );
		svrChk( GetComponent<Svr::ClusterManagerServiceEntity>()->AddClusterServiceEntity( pServiceEntity ) );

		pComponent = dynamic_cast<Svr::IServerComponent*>(pServiceEntity);
		if( pComponent != nullptr )
		{
			svrChk( AddComponent(pComponent) );
		}

	Proc_End:

		return hr;
	}

	// Create clustered service
	Result SharedModuleServer::RegisterClusteredService(Svr::Config::ModuleBase* module)
	{
		Result hr = ResultCode::SUCCESS;
		ClusterID clusterID;

		svrChkPtr(module);

		clusterID = module->GetModuleClusterID();
		switch(clusterID)
		{
		case ClusterID::Monitoring:
			svrChk(GetComponent<Svr::EntityManager>()->AddEntity(EntityFaculty::Service, new Svr::MonitoringServiceEntity));
			break;

		case ClusterID::Login:
		{
			auto pLogin = (Svr::Config::ModuleLogin*)module;
			svrChk(GetComponent<Svr::EntityManager>()->AddEntity(EntityFaculty::Service, new Svr::LoginServiceEntity(pLogin->NetPublic)));
			break;
		}

		case ClusterID::Game:
		{
			auto pGame = (Svr::Config::ModuleGame*)module;
			svrChk(GetComponent<Svr::EntityManager>()->AddEntity(EntityFaculty::Service, new Svr::GameServiceEntity(GetGameID(), pGame->NetPublic)));
			svrChk(AddServiceEntityComponent<Svr::GameInstanceManagerWatcherServiceEntity>(GetGameID()));
			break;
		}

		case ClusterID::Ranking:
		{
			Svr::ClusteredServiceEntity* pServiceEntity = new Svr::RankingServiceEntity(ClusterID::Ranking, ClusterMembership::Master);
			svrChk(GetComponent<Svr::EntityManager>()->AddEntity(EntityFaculty::Service, pServiceEntity));
			svrChk(GetComponent<Svr::ClusterManagerServiceEntity>()->AddClusterServiceEntity(pServiceEntity));
			break;
		}

		case ClusterID::GameInstanceManager:
			// TODO: 
			//svrChk(AddServiceEntityComponent<Svr::GameInstanceManagerServiceEntity>(ClusterID::GameInstanceManager, ClusterMembership::Slave));
			break;

		case ClusterID::GamePartyManager:
			svrChk(AddServiceEntityComponent<Svr::GamePartyManagerServiceEntity>(ClusterMembership::Slave));
			svrChk(AddServiceEntityComponent<Svr::GameInstanceManagerWatcherServiceEntity>(GetGameID()));
			svrChk(RegisterClustereWatcherComponents(ClusterID::MatchingQueue_Game_4x1, Svr::ServerComponentID_MatchingQueueWatcherService_4x1, Svr::ServerComponentID_MatchingQueueWatcherService_4x1W));
			svrChk(RegisterClustereWatcherComponents(ClusterID::MatchingQueue_Game_8x1, Svr::ServerComponentID_MatchingQueueWatcherService_8x1, Svr::ServerComponentID_MatchingQueueWatcherService_8x1W));
			break;

		case ClusterID::Matching_Game_4:
		{
			auto pMatching = (Svr::Config::ModuleMatching*)module;
			svrChk(AddServiceEntityComponent<Svr::MatchingServiceEntity>(clusterID, ClusterMembership::Slave, pMatching->UseBot));
			svrChk(AddServiceEntityComponent<Svr::GameInstanceManagerWatcherServiceEntity>(GetGameID()));
			svrChk(RegisterClustereWatcherComponents(ClusterID::MatchingQueue_Game_4x1, Svr::ServerComponentID_MatchingQueueWatcherService_4x1, Svr::ServerComponentID_MatchingQueueWatcherService_4x1W));
			break;
		}
		case ClusterID::Matching_Game_8:
		{
			auto pMatching = (Svr::Config::ModuleMatching*)module;
			svrChk(AddServiceEntityComponent<Svr::MatchingServiceEntity>(clusterID, ClusterMembership::Slave, pMatching->UseBot));
			svrChk(AddServiceEntityComponent<Svr::GameInstanceManagerWatcherServiceEntity>(GetGameID()));
			svrChk(RegisterClustereWatcherComponents(ClusterID::MatchingQueue_Game_8x1, Svr::ServerComponentID_MatchingQueueWatcherService_8x1, Svr::ServerComponentID_MatchingQueueWatcherService_8x1W));
			break;
		}

		case ClusterID::MatchingQueue_Game_4x1:
		case ClusterID::MatchingQueue_Game_4x2:
		case ClusterID::MatchingQueue_Game_4x3:
		case ClusterID::MatchingQueue_Game_4x1W:
		case ClusterID::MatchingQueue_Game_4x1S:

		case ClusterID::MatchingQueue_Game_8x1:
		case ClusterID::MatchingQueue_Game_8x2:
		case ClusterID::MatchingQueue_Game_8x3:
		case ClusterID::MatchingQueue_Game_8x4:
		case ClusterID::MatchingQueue_Game_8x5:
		case ClusterID::MatchingQueue_Game_8x6:
		case ClusterID::MatchingQueue_Game_8x7:
		case ClusterID::MatchingQueue_Game_8x1W:
		case ClusterID::MatchingQueue_Game_8x1S:

			svrChk( AddServiceEntityComponent<Svr::MatchingQueueServiceEntity>( clusterID, ClusterMembership::Slave ) );
			break;

		default:
			Assert(false);
			svrTrace(Trace::TRC_ERROR, "Invalid cluster ID for module{0}", clusterID);
			break;
		}


	Proc_End:

		return hr;
	}


	Result SharedModuleServer::RegisterClustereWatcherComponents(ClusterID clusterID, Svr::ServerComponentID componentIDStart, Svr::ServerComponentID componentIDEnd)
	{
		Result hr = ResultCode::SUCCESS;

		if (componentIDStart != 0 && componentIDEnd != 0)
		{
			for (INT componentID = componentIDStart; componentID <= componentIDEnd; componentID++, clusterID++)
			{
				if (GetComponent(componentID) != nullptr)
				{
					continue;
				}

				svrChk(AddServiceEntityComponent<Svr::MatchingQueueWatcherServiceEntity>(clusterID, componentID));
			}
		}

	Proc_End:

		Assert((hr));

		return hr;
	}


}; // namespace SharedModuleServer
}; // namespace BR







