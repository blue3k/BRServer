////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server Utility definition
//
////////////////////////////////////////////////////////////////////////////////

#include "ServerSystemPCH.h"
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
#include "Object/SFLibraryComponentAdapter.h"
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
		auto logPath = ParameterSetting::GetSetting("logpath", "../log");
		auto bAsService = StrUtil::StringCompairIgnoreCase(ParameterSetting::GetSetting("servicemode"), -1, "true", -1);
		auto zkaddress = ParameterSetting::GetSetting("zkaddress", "127.0.0.1:2181");
		auto zkconfig = ParameterSetting::GetSetting("zkconfig", "/ServerConfig");
		SF::EngineInitParam initParam;


		auto strServiceName = ParameterSetting::GetSetting("servicename");
		if (!StrUtil::IsNullOrEmpty(strServiceName))
			Util::SetServiceName(strServiceName);



		auto modulePath = Util::GetModulePath();
		StrUtil::Format(strLogPath, "{0}{1}", Util::GetModulePath(), logPath);

		StrUtil::StringCat(strLogPath, "/");
		StrUtil::StringCat(strLogPath, Util::GetServiceName());

		initParam.LogFilePrefix = strLogPath;
		initParam.LogOutputFile = LogChannelMask();

		initParam.AsyncTaskThreadCount = 6;
		initParam.NetworkThreadCount = 0; // Net module will be initialized manually

		// for test
		//initParam.EnableMemoryLeakDetection = true;

		initParam.NetRecvBufferSize = Net::Const::SVR_RECV_BUFFER_SIZE;
		initParam.NetSendBufferSize = Net::Const::SVR_SEND_BUFFER_SIZE;

		if (bAsService)
			initParam.LogOutputConsole = { 0, };


		auto pEngine = SF::Engine::Start(initParam);
		if (pEngine == nullptr)
			return;

		pEngine->AddComponent<ServerNetComponent>();
		pEngine->AddComponent<ServerLogComponent>("..\\..\\Config\\traceConfig.cfg");
		pEngine->AddComponent<ZooKeeperSessionComponent>(zkaddress, ZOO_LOG_LEVEL_DEBUG);
		pEngine->AddComponent<ServerConfigComponent>(zkconfig);

		// Config can be accessed after ServerConfigComponent is initialized
		auto pMyConfig = Service::ServerConfig->FindGenericServer(Util::GetServiceName());
		if (pMyConfig == nullptr)
			return;


		pEngine->AddComponent<SF::Net::NetSystem>(initParam.NetRecvBufferSize, initParam.NetSendBufferSize, pMyConfig->NetIOThreadCount, 1024);
		pEngine->AddComponent<ConnectionManagerComponent>(2048);
		pEngine->AddComponent<EntityTable>();
		pEngine->AddComponent<LibraryComponentAdapter<EntityManager, uint>, IHeap&, uint>(GetSystemHeap(), pMyConfig->WorkerThreadCount);
		pEngine->AddComponent<LibraryComponentAdapter<Svr::ServerEntityManager,uint>, IHeap&, uint>(GetSystemHeap(), pMyConfig->WorkerThreadCount);
		pEngine->AddComponent<LibraryComponentAdapter<Svr::ClusterManagerServiceEntity>, IHeap&>(GetSystemHeap());
	}

	void DeinitializeEngine()
	{
		SF::Engine::Stop();
	}


}
}

