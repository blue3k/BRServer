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
#include "Server/ParameterSetting.h"
#include "ServerEntity/ServerEntityManager.h"
#include "ServiceEntity/ClusterManagerServiceEntity.h"
#include "Entity/EntityManager.h"

#include "SFEngine.h"
#include "Component/SFServerNetComponent.h"
#include "Component/SFServerConfigComponent.h"
#include "Component/SFZooKeeperSessionComponent.h"
#include "Component/SFConnectionManagerComponent.h"
#include "Object/LibraryComponentAdapter.h"
#include "ServerLog/SvrLogComponent.h"
#include "Service/ServerService.h"
#include "zookeeper.h"

namespace SF {
namespace Svr {




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
		char strLogPath[1024];
		auto bIsDebugRun = StrUtil::StringCmpLwr(ParameterSetting::GetSetting("debug"), -1, "true", -1);
		auto zkaddress = ParameterSetting::GetSetting("zkaddress", "127.0.0.1:2181");
		auto zkconfig = ParameterSetting::GetSetting("zkconfig", "/ServerConfig");
		SF::EngineInitParam initParam;


		auto strServiceName = ParameterSetting::GetSetting("servicename");
		if (!StrUtil::IsNullOrEmpty(strServiceName))
			Util::SetServiceName(strServiceName);

		auto modulePath = Util::GetModulePath();

		StrUtil::Format(strLogPath, "{0}..\\log\\{1}", Util::GetModulePath(), Util::GetServiceName());

		initParam.LogFilePrefix = strLogPath;
		initParam.LogOutputFile = LogChannelMask();
		initParam.AsyncTaskThreadCount = 6;
		initParam.NetworkThreadCount = 4;
		initParam.EnableMemoryLeakDetection = true;

		initParam.NetRecvBufferSize = Net::Const::SVR_RECV_BUFFER_SIZE;
		initParam.NetSendBufferSize = Net::Const::SVR_SEND_BUFFER_SIZE;

		if (!bIsDebugRun)
			initParam.LogOutputConsole = { 0, };


		auto pEngine = SF::Engine::Start(initParam);
		if (pEngine == nullptr)
			return;

		pEngine->AddComponent<ServerNetComponent>();
		pEngine->AddComponent<ServerLogComponent>("..\\..\\Config\\traceConfig.cfg");
		pEngine->AddComponent<ZooKeeperSessionComponent>(zkaddress, ZOO_LOG_LEVEL_DEBUG);
		pEngine->AddComponent<ServerConfigComponent>(zkconfig);


		auto pMyConfig = Service::ServerConfig->FindGenericServer(Util::GetServiceName());
		if (pMyConfig == nullptr)
			return;

		pEngine->AddComponent<ConnectionManagerComponent>(2048);
		pEngine->AddComponent<EntityTable>();
		pEngine->AddComponent<LibraryComponentAdapter<EntityManager, uint>, IHeap&, uint>(GetSystemMemoryManager(), pMyConfig->EntityControlCount);
		pEngine->AddComponent<LibraryComponentAdapter<Svr::ServerEntityManager,uint>, IHeap&, uint>(GetSystemMemoryManager(), pMyConfig->EntityControlCount);
		pEngine->AddComponent<LibraryComponentAdapter<Svr::ClusterManagerServiceEntity>, IHeap&>(GetSystemMemoryManager());
	}

	void DeinitializeEngine()
	{
		SF::Engine::Stop();
	}


}
}

