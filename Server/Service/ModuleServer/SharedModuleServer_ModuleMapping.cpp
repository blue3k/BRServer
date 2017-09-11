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

#include "ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServiceEntity/Game/GameClusterServiceEntity.h"
#include "ServiceEntity/Game/GameInstanceManagerServiceEntity.h"
#include "ServiceEntity/Login/LoginServiceEntity.h"
#include "ServiceEntity/Game/GameServiceEntity.h"
#include "ServiceEntity/Game/GameInstanceServiceEntity.h"

#include "ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServiceEntity/MatchingServiceEntity.h"
#include "ServiceEntity/GamePartyManagerServiceEntity.h"
#include "ServiceEntity/RankingServiceEntity.h"
#include "ServiceEntity/MonitoringServiceEntity.h"

#include "Table/TableSystem.h"

#include "ServerEntity/SvrEntityServerEntity.h"
#include "Transaction/ExternalTransactionManager.h"


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






namespace SF {
namespace SharedModuleServer {


	template< class ServiceEntityType, typename... ConstructorArgs >
	Result SharedModuleServer::AddServiceEntityComponent(ConstructorArgs... constructorArgs)
	{
		Result hr = ResultCode::SUCCESS;
		Svr::IServerComponent *pComponent = nullptr;
		Svr::ClusteredServiceEntity *pServiceEntityTest = nullptr;
		ServiceEntityType* pServiceEntity = nullptr;

		svrMem(pServiceEntity = new(GetHeap()) ServiceEntityType(constructorArgs...));

		if( (GetComponentCarrier().GetComponent<Svr::ClusterManagerServiceEntity>()->GetClusterServiceEntity( pServiceEntity->GetClusterID(), pServiceEntityTest )) )
		{
			if (pServiceEntityTest == pServiceEntity)
				goto Proc_End;

			if (pServiceEntity->GetClusterMembership() <= ClusterMembership::StatusWatcher)
			{
				svrTrace(Warning, "Duplicated entity watcher entity {0} EntityUID:{1}, while adding {2} EntityUID:{3}", typeid(*pServiceEntityTest).name(), pServiceEntityTest->GetEntityUID(), typeid(*pServiceEntity).name(), pServiceEntity->GetEntityUID());
			}
			else
			{
				svrTrace(Error, "Duplicated cluster entity {0} EntityUID:{1}, while adding {2} EntityUID:{3}", typeid(*pServiceEntityTest).name(), pServiceEntityTest->GetEntityUID(), typeid(*pServiceEntity).name(), pServiceEntity->GetEntityUID());
			}
			IHeap::Delete(pServiceEntity);
			goto Proc_End;
		}

		svrChk(Service::EntityManager->AddEntity( EntityFaculty::Service, pServiceEntity ) );
		svrChk( GetComponentCarrier().GetComponent<Svr::ClusterManagerServiceEntity>()->AddClusterServiceEntity( pServiceEntity ) );

		pComponent = dynamic_cast<Svr::IServerComponent*>(pServiceEntity);
		if( pComponent != nullptr )
		{
			svrChk(GetComponentCarrier().AddComponent(pComponent) );
		}

	Proc_End:

		return hr;
	}

	// Create clustered service
	Result SharedModuleServer::RegisterClusteredService(ServerConfig::ServerModule* module)
	{
		Result hr = ResultCode::SUCCESS;
		char strRelativePath[1024];
		FixedString32 moduleName = module->ModuleName;
		svrChkPtr(module);

		switch(moduleName)
		{
		case "ModMonitoring"_hash32:
			svrChk(Service::EntityManager->AddEntity(EntityFaculty::Service, new(GetHeap()) Svr::MonitoringServiceEntity));
			break;

		case "ModLogin"_hash32:
		{
			auto pLogin = (ServerConfig::ServerModulePublicService*)module;
			svrChk(Service::EntityManager->AddEntity(EntityFaculty::Service, new(GetHeap()) Svr::LoginServiceEntity(&pLogin->PublicNet)));
			break;
		}

		case "ModGame"_hash32:
		{
			auto pGame = (ServerConfig::ServerModulePublicService*)module;
			svrChk(Service::EntityManager->AddEntity(EntityFaculty::Service, new(GetHeap()) Svr::GameServiceEntity(GetGameID(), &pGame->PublicNet)));
			svrChk(AddServiceEntityComponent<Svr::GameInstanceManagerWatcherServiceEntity>(GetGameID()));
			break;
		}

		case "ModRanking"_hash32:
		{
			Svr::ClusteredServiceEntity* pServiceEntity = new(GetHeap()) Svr::RankingServiceEntity(ClusterID::Ranking, ClusterMembership::Master);
			svrChk(Service::EntityManager->AddEntity(EntityFaculty::Service, pServiceEntity));
			svrChk(GetComponentCarrier().GetComponent<Svr::ClusterManagerServiceEntity>()->AddClusterServiceEntity(pServiceEntity));
			break;
		}

		case "ModGameInstanceManager"_hash32:
		{
			Svr::ClusteredServiceEntity* pServiceEntity = new(GetHeap()) Svr::GameInstanceManagerServiceEntity(ClusterID::GameInstanceManager, ClusterMembership::Master);
			svrChk(Service::EntityManager->AddEntity(EntityFaculty::Service, pServiceEntity));
			svrChk(GetComponentCarrier().GetComponent<Svr::ClusterManagerServiceEntity>()->AddClusterServiceEntity(pServiceEntity));
			break;
		}

		case "ModGamePartyManager"_hash32:
			svrChk(AddServiceEntityComponent<Svr::GamePartyManagerServiceEntity>(ClusterMembership::Slave));
			svrChk(AddServiceEntityComponent<Svr::GameInstanceManagerWatcherServiceEntity>(GetGameID()));
			svrChk(RegisterClustereWatcherComponents(ClusterID::MatchingQueue_Game_4x1, Svr::ServerComponentID_MatchingQueueWatcherService_4x1, Svr::ServerComponentID_MatchingQueueWatcherService_4x1W));
			svrChk(RegisterClustereWatcherComponents(ClusterID::MatchingQueue_Game_8x1, Svr::ServerComponentID_MatchingQueueWatcherService_8x1, Svr::ServerComponentID_MatchingQueueWatcherService_8x1W));
			break;

		case "ModMatching_Game_4"_hash32:
		{
			auto pMatching = (ServerConfig::ServerModuleMatching_4*)module;
			svrChk(AddServiceEntityComponent<Svr::MatchingServiceEntity>(ClusterID::Matching_Game_4, ClusterMembership::Slave, pMatching->UseBot));
			svrChk(AddServiceEntityComponent<Svr::GameInstanceManagerWatcherServiceEntity>(GetGameID()));
			svrChk(RegisterClustereWatcherComponents(ClusterID::MatchingQueue_Game_4x1, Svr::ServerComponentID_MatchingQueueWatcherService_4x1, Svr::ServerComponentID_MatchingQueueWatcherService_4x1W));
			break;
		}
		case "ModMatching_Game_8"_hash32:
		{
			auto pMatching = (ServerConfig::ServerModuleMatching_8*)module;
			svrChk(AddServiceEntityComponent<Svr::MatchingServiceEntity>(ClusterID::Matching_Game_8, ClusterMembership::Slave, pMatching->UseBot));
			svrChk(AddServiceEntityComponent<Svr::GameInstanceManagerWatcherServiceEntity>(GetGameID()));
			svrChk(RegisterClustereWatcherComponents(ClusterID::MatchingQueue_Game_8x1, Svr::ServerComponentID_MatchingQueueWatcherService_8x1, Svr::ServerComponentID_MatchingQueueWatcherService_8x1W));
			break;
		}
		case "ModPurchaseValidateGoogle"_hash32:
		{
			auto pModule = (ServerConfig::ServerModuleGooglePurchaseValidate*)module;
			StrUtil::Format(strRelativePath, "../../Config/{0}", pModule->P12KeyFile);
			svrChk(GetComponentCarrier().AddComponent<Svr::ExternalTransactionManager>());
			svrChk(GetComponentCarrier().GetComponent<Svr::ExternalTransactionManager>()->InitializeManagerGoogle(
				strRelativePath,
				pModule->Account,
				pModule->AuthScopes));
			break;
		}
		case "ModPurchaseValidateIOS"_hash32:
		{
			auto pModule = (ServerConfig::ServerModuleIOSPurchaseValidate*)module;
			svrChk(GetComponentCarrier().AddComponent<Svr::ExternalTransactionManager>());
			svrChk(GetComponentCarrier().GetComponent<Svr::ExternalTransactionManager>()->InitializeManagerIOS(pModule->URL, pModule->AltURL));
			break;
		}

		case "ModMatchingQueue_Game_8x1"_hash32:
		case "ModMatchingQueue_Game_8x2"_hash32:
		case "ModMatchingQueue_Game_8x3"_hash32:
		case "ModMatchingQueue_Game_8x4"_hash32:
		case "ModMatchingQueue_Game_8x5"_hash32:
		case "ModMatchingQueue_Game_8x6"_hash32:
		case "ModMatchingQueue_Game_8x7"_hash32:
		case "ModMatchingQueue_Game_8x1S"_hash32:
		case "ModMatchingQueue_Game_8x1W"_hash32:

		case "ModMatchingQueue_Game_4x1"_hash32:
		case "ModMatchingQueue_Game_4x2"_hash32:
		case "ModMatchingQueue_Game_4x3"_hash32:
		case "ModMatchingQueue_Game_4x1S"_hash32:
		case "ModMatchingQueue_Game_4x1W"_hash32:
		{
			ClusterID clusterID;
			switch (moduleName)
			{
			case "ModMatchingQueue_Game_8x1"_hash32:		clusterID = ClusterID::MatchingQueue_Game_8x1;	break;
			case "ModMatchingQueue_Game_8x2"_hash32:		clusterID = ClusterID::MatchingQueue_Game_8x2;	break;
			case "ModMatchingQueue_Game_8x3"_hash32:		clusterID = ClusterID::MatchingQueue_Game_8x3;	break;
			case "ModMatchingQueue_Game_8x4"_hash32:		clusterID = ClusterID::MatchingQueue_Game_8x4;	break;
			case "ModMatchingQueue_Game_8x5"_hash32:		clusterID = ClusterID::MatchingQueue_Game_8x5;	break;
			case "ModMatchingQueue_Game_8x6"_hash32:		clusterID = ClusterID::MatchingQueue_Game_8x6;	break;
			case "ModMatchingQueue_Game_8x7"_hash32:		clusterID = ClusterID::MatchingQueue_Game_8x7;	break;
			case "ModMatchingQueue_Game_8x1S"_hash32:		clusterID = ClusterID::MatchingQueue_Game_8x1S;	break;
			case "ModMatchingQueue_Game_8x1W"_hash32:		clusterID = ClusterID::MatchingQueue_Game_8x1W;	break;

			case "ModMatchingQueue_Game_4x1"_hash32:		clusterID = ClusterID::MatchingQueue_Game_4x1;	break;
			case "ModMatchingQueue_Game_4x2"_hash32:		clusterID = ClusterID::MatchingQueue_Game_4x2;	break;
			case "ModMatchingQueue_Game_4x3"_hash32:		clusterID = ClusterID::MatchingQueue_Game_4x3;	break;
			case "ModMatchingQueue_Game_4x1S"_hash32:		clusterID = ClusterID::MatchingQueue_Game_4x1S;	break;
			case "ModMatchingQueue_Game_4x1W"_hash32:		clusterID = ClusterID::MatchingQueue_Game_4x1W;	break;
			default:
				assert(false);
				svrTrace(Error, "Invalid cluster ID for module{0}", module->ModuleName);
				break;
			}

			svrChk(AddServiceEntityComponent<Svr::MatchingQueueServiceEntity>(clusterID, ClusterMembership::Slave));
			break;
		}
		default:
			assert(false);
			svrTrace(Error, "Invalid cluster ID for module: {0}", module->ModuleName);
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
				if (GetComponentCarrier().GetComponent(componentID) != nullptr)
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
}; // namespace SF







