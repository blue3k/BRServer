////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
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
#include "Service/ZookeeperSessionService.h"
#include "Service/ClusterManagerService.h"
#include "Service/ServerEntityManagerService.h"
#include "Service/PlayerManagerService.h"





namespace SF {


	//////////////////////////////////////////////////////////////////////
	//
	//	Service base class
	//

	namespace Service
	{
		extern ServiceInstance<SF::ServerConfig> ServerConfig;
		extern ServiceInstance<EntityTableService> EntityTable;
		extern ServiceInstance<EntityManagerService> EntityManager;
		extern ServiceInstance<ServerEntityManagerService> ServerEntityManager;
		extern ServiceInstance<ZooKeeperSessionService> ZKSession; // main zookeeper session
		extern ServiceInstance<ClusterManagerService> ClusterManager;
		extern ServiceInstance<PlayerManagerService> PlayerManager;
	};



}; // namespace SF

