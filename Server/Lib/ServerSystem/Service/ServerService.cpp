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
		ServiceInstance<ZooKeeperSessionService> ZKSession;
		//ServiceInstance<SF::ServerConfig> ServerConfig;
	}


}; // namespace SF

