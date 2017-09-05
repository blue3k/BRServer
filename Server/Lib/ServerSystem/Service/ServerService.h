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
#include "Service/Service.h"
#include "Service/EngineService.h"
#include "ServerConfig/SFServerConfig.h"





namespace SF {


	//////////////////////////////////////////////////////////////////////
	//
	//	Service base class
	//

	namespace Service
	{
		extern ServiceInstance<SF::ServerConfig> ServerConfig;
		//extern ServiceInstance<EngineTaskService> EngineTaskManager;
		//extern ServiceInstance<EngineObjectManagerService> EngineObjectManager;
		//extern ServiceInstance<GraphicDeviceService> GraphicDevice;
		//extern ServiceInstance<NetSystemService> NetSystem;

	};



}; // namespace SF

