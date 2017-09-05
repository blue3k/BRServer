////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server Utility definition
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Transaction/Transaction.h"
#include "Types/BrSvrTypes.h"
#include "Task/ServerTaskManager.h"
#include "Entity/Entity.h"
#include "Entity/EntityTable.h"
#include "ServerConfig/SFServerConfig.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"

#include "SFEngine.h"
#include "Component/SFServerConfigComponent.h"
#include "Component/SFZooKeeperSessionComponent.h"
#include "Component/SFConnectionManagerComponent.h"


namespace SF {
namespace Svr {


	// Entity Table
	EntityTable					g_EntityTable;


	// Get entity table
	EntityTable& GetEntityTable()
	{
		return g_EntityTable;
	}

	Result FindEntity(EntityID entityID, SharedPointerT<Entity>& entity)
	{
		return g_EntityTable.Find(entityID, entity);
	}

	uint GetServerUID()
	{
		return Svr::BrServer::GetInstance()->GetServerUID();
	}


	// Get server ID
	ServerID GetMyServerID()
	{
		Assert(BrServer::GetInstance());
		return BrServer::GetInstance()->GetServerUID();
	}



	// Initialize and deinitialization
	void InitializeEngineForServer()
	{
		SF::EngineInitParam initParam;

		initParam.LogFilePrefix = Util::GetServiceName();
		initParam.LogOutputFile = LogChannelMask();
		initParam.AsyncTaskThreadCount = 6;
		initParam.NetworkThreadCount = 4;

		auto pEngine = SF::Engine::Start(initParam);
		if (pEngine == nullptr)
			return;

		pEngine->AddComponent<ZooKeeperSessionComponent>("127.0.0.1:2181");
		pEngine->AddComponent<ServerConfigComponent>("/ServerConfig");
		pEngine->AddComponent<ConnectionManagerComponent>(2048);
	}

	void DeinitializeEngine()
	{
		SF::Engine::Stop();
	}


}
}

