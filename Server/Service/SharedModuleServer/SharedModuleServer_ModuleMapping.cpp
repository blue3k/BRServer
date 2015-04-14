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
#include "ServerSystem/ServiceEntity/LoginClusterServiceEntity.h"
#include "ServerSystem/ServiceEntity/GameClusterServiceEntity.h"
#include "ServerSystem/ServiceEntity/GameInstanceManagerServiceEntity.h"

#include "ServerSystem/ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServerSystem/ServiceEntity/MatchingServiceEntity.h"
#include "ServerSystem/ServiceEntity/GamePartyManagerServiceEntity.h"
#include "ServerSystem/ServiceEntity/RankingServiceEntity.h"
#include "ServerSystem/ServiceEntity/MonitoringServiceEntity.h"

#include "Table/TableSystem.h"

#include "ServerEntity/EntityServerEntity.h"
#include "ServerEntity/GameServerEntity.h"

#include "Protocol/Policy/EntityServerIPolicy.h"

#include "Protocol/Policy/EntityServerIPolicy.h"
#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Policy/GameInstanceIPolicy.h"
#include "Protocol/Policy/GameMasterServerIPolicy.h"

#include "Net/NetServerPeer.h"
#include "Net/NetServer.h"
#include "Net/NetServerUDP.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ExternalTask.h"

#include "SharedModuleSvrConst.h"
#include "SharedModuleServerClass.h"

#include "Transaction/SharedModuleServerTrans.h"

#include "DB/GameConspiracyDB.h"
#include "DB/AccountDB.h"



#define PATH_DATABASE L"../../Data"



namespace BR {
namespace SharedModuleServer {

	static Svr::ServerComponentID ComponentIDFromClusterID( ClusterID clusterID )
	{
		switch(clusterID)
		{
		case ClusterID::Matching_Game_4:				return Svr::ServerComponentID_MatchingWatcherService_4;
		case ClusterID::Matching_Game_8:				return Svr::ServerComponentID_MatchingWatcherService_8;
		case ClusterID::Matching_Game_10:				return Svr::ServerComponentID_MatchingWatcherService_10;
		case ClusterID::Matching_Game_12:				return Svr::ServerComponentID_MatchingWatcherService_12;

		case ClusterID::MatchingQueue_Game_4x1:			return Svr::ServerComponentID_MatchingQueueWatcherService_4x1;
		case ClusterID::MatchingQueue_Game_4x2:			return Svr::ServerComponentID_MatchingQueueWatcherService_4x2;
		case ClusterID::MatchingQueue_Game_4x3:			return Svr::ServerComponentID_MatchingQueueWatcherService_4x3;

		case ClusterID::MatchingQueue_Game_8x1:			return Svr::ServerComponentID_MatchingQueueWatcherService_8x1;
		case ClusterID::MatchingQueue_Game_8x2:			return Svr::ServerComponentID_MatchingQueueWatcherService_8x2;
		case ClusterID::MatchingQueue_Game_8x3:			return Svr::ServerComponentID_MatchingQueueWatcherService_8x3;
		case ClusterID::MatchingQueue_Game_8x4:			return Svr::ServerComponentID_MatchingQueueWatcherService_8x4;
		case ClusterID::MatchingQueue_Game_8x5:			return Svr::ServerComponentID_MatchingQueueWatcherService_8x5;
		case ClusterID::MatchingQueue_Game_8x6:			return Svr::ServerComponentID_MatchingQueueWatcherService_8x6;
		case ClusterID::MatchingQueue_Game_8x7:			return Svr::ServerComponentID_MatchingQueueWatcherService_8x7;

		case ClusterID::MatchingQueue_Game_10x1:		return Svr::ServerComponentID_MatchingQueueWatcherService_10x1;
		case ClusterID::MatchingQueue_Game_10x2:		return Svr::ServerComponentID_MatchingQueueWatcherService_10x2;
		case ClusterID::MatchingQueue_Game_10x3:		return Svr::ServerComponentID_MatchingQueueWatcherService_10x3;
		case ClusterID::MatchingQueue_Game_10x4:		return Svr::ServerComponentID_MatchingQueueWatcherService_10x4;
		case ClusterID::MatchingQueue_Game_10x5:		return Svr::ServerComponentID_MatchingQueueWatcherService_10x5;
		case ClusterID::MatchingQueue_Game_10x6:		return Svr::ServerComponentID_MatchingQueueWatcherService_10x6;
		case ClusterID::MatchingQueue_Game_10x7:		return Svr::ServerComponentID_MatchingQueueWatcherService_10x7;
		case ClusterID::MatchingQueue_Game_10x8:		return Svr::ServerComponentID_MatchingQueueWatcherService_10x8;
		case ClusterID::MatchingQueue_Game_10x9:		return Svr::ServerComponentID_MatchingQueueWatcherService_10x9;

		case ClusterID::MatchingQueue_Game_12x1:		return Svr::ServerComponentID_MatchingQueueWatcherService_12x1;
		case ClusterID::MatchingQueue_Game_12x2:		return Svr::ServerComponentID_MatchingQueueWatcherService_12x2;
		case ClusterID::MatchingQueue_Game_12x3:		return Svr::ServerComponentID_MatchingQueueWatcherService_12x3;
		case ClusterID::MatchingQueue_Game_12x4:		return Svr::ServerComponentID_MatchingQueueWatcherService_12x4;
		case ClusterID::MatchingQueue_Game_12x5:		return Svr::ServerComponentID_MatchingQueueWatcherService_12x5;
		case ClusterID::MatchingQueue_Game_12x6:		return Svr::ServerComponentID_MatchingQueueWatcherService_12x6;
		case ClusterID::MatchingQueue_Game_12x7:		return Svr::ServerComponentID_MatchingQueueWatcherService_12x7;
		case ClusterID::MatchingQueue_Game_12x8:		return Svr::ServerComponentID_MatchingQueueWatcherService_12x8;
		case ClusterID::MatchingQueue_Game_12x9:		return Svr::ServerComponentID_MatchingQueueWatcherService_12x9;
		case ClusterID::MatchingQueue_Game_12x10:		return Svr::ServerComponentID_MatchingQueueWatcherService_12x10;
		case ClusterID::MatchingQueue_Game_12x11:		return Svr::ServerComponentID_MatchingQueueWatcherService_12x11;
			break;
		}
	}

	template< class ServiceEntityType >
	HRESULT SharedModuleServer::AddServiceEntityComponent( ServiceEntityType* pServiceEntity )
	{
		HRESULT hr = S_OK;
		Svr::IServerComponent *pComponent = nullptr;
		Svr::ClusteredServiceEntity *pServiceEntityTest = nullptr;

		if( SUCCEEDED(GetComponent<Svr::ClusterManagerServiceEntity>()->GetClusterServiceEntity( pServiceEntity->GetClusterID(), pServiceEntityTest )) )
		{
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
	HRESULT SharedModuleServer::RegisterClusteredService(ClusterID clusterID, const char* params)
	{
		HRESULT hr = S_OK;
		Svr::ServerComponentID componentIDStart = (Svr::ServerComponentID)0;
		Svr::ServerComponentID componentIDEnd = (Svr::ServerComponentID)0;
		ClusterID queueClusterID = ClusterID::Max;

		switch( clusterID )
		{
		case ClusterID::Monitoring:
			svrChk(GetComponent<Svr::EntityManager>()->AddEntity(EntityFaculty::Service, new Svr::MonitoringServiceEntity));
			break;

		case ClusterID::GamePartyManager:
			svrChk( AddServiceEntityComponent( new Svr::GamePartyManagerServiceEntity( ClusterMembership::Slave ) ) );
			svrChk(AddServiceEntityComponent(new Svr::GameInstanceManagerWatcherServiceEntity(GameID::Conspiracy)));
			svrChk(RegisterClustereWatcherComponents(ClusterID::MatchingQueue_Game_4x1, Svr::ServerComponentID_MatchingQueueWatcherService_4x1, Svr::ServerComponentID_MatchingQueueWatcherService_4x3));
			svrChk(RegisterClustereWatcherComponents(ClusterID::MatchingQueue_Game_8x1, Svr::ServerComponentID_MatchingQueueWatcherService_8x1, Svr::ServerComponentID_MatchingQueueWatcherService_8x7));
			break;

		case ClusterID::Matching_Game_4:
			svrChk(AddServiceEntityComponent(new Svr::MatchingServiceEntity(clusterID, ClusterMembership::Slave, params)));
			svrChk( AddServiceEntityComponent( new Svr::GameInstanceManagerWatcherServiceEntity( GameID::Conspiracy ) ) );
			svrChk(RegisterClustereWatcherComponents(ClusterID::MatchingQueue_Game_4x1, Svr::ServerComponentID_MatchingQueueWatcherService_4x1, Svr::ServerComponentID_MatchingQueueWatcherService_4x3));
			break;
		case ClusterID::Matching_Game_8:
			svrChk(AddServiceEntityComponent(new Svr::MatchingServiceEntity(clusterID, ClusterMembership::Slave, params)));
			svrChk( AddServiceEntityComponent( new Svr::GameInstanceManagerWatcherServiceEntity( GameID::Conspiracy ) ) );
			svrChk(RegisterClustereWatcherComponents(ClusterID::MatchingQueue_Game_8x1, Svr::ServerComponentID_MatchingQueueWatcherService_8x1, Svr::ServerComponentID_MatchingQueueWatcherService_8x7));
			break;
		case ClusterID::Matching_Game_10:
			svrChk(AddServiceEntityComponent(new Svr::MatchingServiceEntity(clusterID, ClusterMembership::Slave, params)));
			svrChk( AddServiceEntityComponent( new Svr::GameInstanceManagerWatcherServiceEntity( GameID::Conspiracy ) ) );
			svrChk(RegisterClustereWatcherComponents(ClusterID::MatchingQueue_Game_10x1, Svr::ServerComponentID_MatchingQueueWatcherService_10x1, Svr::ServerComponentID_MatchingQueueWatcherService_10x9));
			break;
		case ClusterID::Matching_Game_12:
			svrChk(AddServiceEntityComponent(new Svr::MatchingServiceEntity(clusterID, ClusterMembership::Slave, params)));
			svrChk( AddServiceEntityComponent( new Svr::GameInstanceManagerWatcherServiceEntity( GameID::Conspiracy ) ) );
			svrChk(RegisterClustereWatcherComponents(ClusterID::MatchingQueue_Game_12x1, Svr::ServerComponentID_MatchingQueueWatcherService_12x1, Svr::ServerComponentID_MatchingQueueWatcherService_12x11));
			break;

		case ClusterID::MatchingQueue_Game_4x1:
		case ClusterID::MatchingQueue_Game_4x2:
		case ClusterID::MatchingQueue_Game_4x3:

		case ClusterID::MatchingQueue_Game_8x1:
		case ClusterID::MatchingQueue_Game_8x2:
		case ClusterID::MatchingQueue_Game_8x3:
		case ClusterID::MatchingQueue_Game_8x4:
		case ClusterID::MatchingQueue_Game_8x5:
		case ClusterID::MatchingQueue_Game_8x6:
		case ClusterID::MatchingQueue_Game_8x7:

		case ClusterID::MatchingQueue_Game_10x1:
		case ClusterID::MatchingQueue_Game_10x2:
		case ClusterID::MatchingQueue_Game_10x3:
		case ClusterID::MatchingQueue_Game_10x4:
		case ClusterID::MatchingQueue_Game_10x5:
		case ClusterID::MatchingQueue_Game_10x6:
		case ClusterID::MatchingQueue_Game_10x7:
		case ClusterID::MatchingQueue_Game_10x8:
		case ClusterID::MatchingQueue_Game_10x9:

		case ClusterID::MatchingQueue_Game_12x1:
		case ClusterID::MatchingQueue_Game_12x2:
		case ClusterID::MatchingQueue_Game_12x3:
		case ClusterID::MatchingQueue_Game_12x4:
		case ClusterID::MatchingQueue_Game_12x5:
		case ClusterID::MatchingQueue_Game_12x6:
		case ClusterID::MatchingQueue_Game_12x7:
		case ClusterID::MatchingQueue_Game_12x8:
		case ClusterID::MatchingQueue_Game_12x9:
		case ClusterID::MatchingQueue_Game_12x10:
		case ClusterID::MatchingQueue_Game_12x11:
			svrChk( AddServiceEntityComponent( new Svr::MatchingQueueServiceEntity( clusterID, ClusterMembership::Slave ) ) );
			break;
		}


	Proc_End:

		return hr;
	}


	HRESULT SharedModuleServer::RegisterClustereWatcherComponents(ClusterID clusterID, Svr::ServerComponentID componentIDStart, Svr::ServerComponentID componentIDEnd)
	{
		HRESULT hr = S_OK;

		if (componentIDStart != 0 && componentIDEnd != 0)
		{
			Svr::MatchingQueueWatcherServiceEntity *pQueueWatcherEntity = nullptr;

			for (INT componentID = componentIDStart; componentID <= componentIDEnd; componentID++, clusterID++)
			{
				if (GetComponent(componentID) != nullptr)
				{
					continue;
				}

				svrMem(pQueueWatcherEntity = new Svr::MatchingQueueWatcherServiceEntity(clusterID, componentID));
				svrChk(GetComponent<Svr::EntityManager>()->AddEntity(EntityFaculty::Service, pQueueWatcherEntity));
				svrChk(GetComponent<Svr::ClusterManagerServiceEntity>()->AddClusterServiceEntity(pQueueWatcherEntity));
				svrChk(AddComponent(pQueueWatcherEntity));
			}
		}

	Proc_End:

		return hr;
	}


}; // namespace SharedModuleServer
}; // namespace BR







