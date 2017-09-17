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



#include "stdafx.h"
#include "Util/SFLog.h"
#include "Service/ServerService.h"




namespace SF {

	namespace Service
	{
		ServiceInstance<SF::ServerConfig> ServerConfig;
		ServiceInstance<EntityTableService> EntityTable;
		ServiceInstance<EntityManagerService> EntityManager;
		ServiceInstance<ServerEntityManagerService> ServerEntityManager;
		ServiceInstance<ZooKeeperSessionService> ZKSession;
		ServiceInstance<ClusterManagerService> ClusterManager;
	}


}; // namespace SF

