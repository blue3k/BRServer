////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) StormForge
// 
// Author : KyungKun Ko
//
// Description : EngineObject manager
//
////////////////////////////////////////////////////////////////////////////////


#pragma once 

#include "SFTypedefs.h"
#include "Service/SFService.h"
#include "Service/SFEngineService.h"
#include "ServerConfig/SFServerConfig.h"
#include "Service/EntityTableService.h"
#include "Service/EntityManagerService.h"
#include "Service/ServiceDirectoryService.h"
#include "Service/ZookeeperSessionService.h"
#include "Service/ClusterManagerService.h"
#include "Service/MessageEndpointManagerService.h"
#include "Service/PlayerManagerService.h"
#include "Service/DataTableManagerService.h"
#include "Service/DatabaseService.h"





namespace SF {


	//////////////////////////////////////////////////////////////////////
	//
	//	Service base class
	//

	namespace Service
	{
		extern ServiceInstance<ServerConfig::ServerService> ServerConfig;
		extern ServiceInstance<EntityTableService> EntityTable;
		extern ServiceInstance<EntityManagerService> EntityManager;
		extern ServiceInstance<ServiceDirectoryService> ServiceDirectory;
		extern ServiceInstance<MessageEndpointManagerService> MessageEndpointManager;
		extern ServiceInstance<ZookeeperSessionService> ZKSession; // main zookeeper session
		//extern ServiceInstance<ClusterManagerService> ClusterManager;
		extern ServiceInstance<PlayerManagerService> PlayerManager;
		extern ServiceInstance<DataTableManagerService> DataTableManager;
		extern ServiceInstance<DatabaseService> Database;
	};



}; // namespace SF

