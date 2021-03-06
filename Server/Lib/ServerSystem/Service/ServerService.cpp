////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : KyungKun Ko
//
// Description : EngineObject manager
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "Util/SFLog.h"
#include "Service/ServerService.h"




namespace SF {

	namespace Service
	{
		ServiceInstance<ServerConfig::ServerService> ServerConfig;
		ServiceInstance<EntityTableService> EntityTable;
		ServiceInstance<EntityManagerService> EntityManager;
		ServiceInstance<ServiceDirectoryService> ServiceDirectory;
		ServiceInstance<MessageEndpointManagerService> MessageEndpointManager;
		ServiceInstance<ZookeeperSessionService> ZKSession;
		//ServiceInstance<ClusterManagerService> ClusterManager;
		ServiceInstance<PlayerManagerService> PlayerManager;
		ServiceInstance<DataTableManagerService> DataTableManager;
		ServiceInstance<DatabaseService> Database;
	}


}; // namespace SF

