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
#include "Util/SFPath.h"

#include "SFEngine.h"
#include "Component/SFServerNetComponent.h"
#include "Component/SFServerConfigComponent.h"
#include "Component/SFZookeeperSessionComponent.h"
#include "Component/SFConnectionManagerComponent.h"
#include "Object/SFLibraryComponentAdapter.h"
#include "ServerLog/SvrLogComponent.h"
#include "ServerLog/SFLogOutputPlayFabGSDK.h"
#include "Service/ServerService.h"


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

	void InitializeEngine()
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

		StrUtil::Format(strLogPath, "{0}{1}{2}", logPath, Util::Path::DirectorySeparatorCharString, Util::GetServiceName());

		initParam.LogFilePrefix = strLogPath;
		initParam.LogOutputFile = LogOutputMask();

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
		pEngine->AddComponent<ServerLogComponent>("..\\Config\\traceConfig.cfg");
		pEngine->AddComponent<ZookeeperSessionComponent>(zkaddress);
		pEngine->AddComponent<ServerConfigComponent>(zkconfig);

	}

	// Initialize and deinitialization
	void InitializeEngineForServer()
	{
		InitializeEngine();

		// Config can be accessed after ServerConfigComponent is initialized
		auto pEngine = SF::Engine::GetInstance();
		if (pEngine == nullptr)
			return;

		SF::EngineInitParam initParam;
		pEngine->AddComponent<SF::Net::NetSystem>(initParam.NetRecvBufferSize, initParam.NetSendBufferSize, Service::ServerConfig->NetIOThreadCount, 1024);
		pEngine->AddComponent<ConnectionManagerComponent>(2048);
		pEngine->AddComponent<EntityTable>();
		pEngine->AddComponent<LibraryComponentAdapter<EntityManager, uint>, IHeap&, uint>(GetSystemHeap(), Service::ServerConfig->WorkerThreadCount);
		pEngine->AddComponent<LibraryComponentAdapter<Svr::ServerEntityManager,uint>, IHeap&, uint>(GetSystemHeap(), Service::ServerConfig->WorkerThreadCount);
		pEngine->AddComponent<LibraryComponentAdapter<Svr::ClusterManagerServiceEntity>, IHeap&>(GetSystemHeap());
	}


	void InitializeEngineForPlayFabServer(uint32_t workerThreadCount, uint32_t netIOThreadCount)
	{
		SF::EngineInitParam initParam;

		auto strServiceName = ParameterSetting::GetSetting("servicename");
		if (!StrUtil::IsNullOrEmpty(strServiceName))
			Util::SetServiceName(strServiceName);


		initParam.LogOutputFile = LogOutputMask();

		initParam.AsyncTaskThreadCount = 6;
		initParam.NetworkThreadCount = 0; // Net module will be initialized manually

		// disable memory leak detection for now
		initParam.EnableMemoryLeakDetection = false;

		initParam.NetRecvBufferSize = Net::Const::SVR_RECV_BUFFER_SIZE;
		initParam.NetSendBufferSize = Net::Const::SVR_SEND_BUFFER_SIZE;

		// turn off output console
		initParam.LogOutputConsole = { 0xFFFFFFFFL };
		initParam.LogOutputCommon = { 0xFFFFFFFFL };
		initParam.LogOutputDebugger = { 0, };
		initParam.LogOutputFile = { 0, };

		initParam.EnableCrashDump = false;

		auto pEngine = SF::Engine::Start(initParam);
		if (pEngine == nullptr)
			return;

		pEngine->AddComponent<ServerNetComponent>();
		pEngine->AddComponent<ServerLogComponent>(nullptr);

		pEngine->AddComponent<SF::Net::NetSystem>(initParam.NetRecvBufferSize, initParam.NetSendBufferSize, netIOThreadCount, 1024);
		pEngine->AddComponent<EntityTable>();
		pEngine->AddComponent<LibraryComponentAdapter<EntityManager, uint>, IHeap&, uint>(GetSystemHeap(), workerThreadCount);
		pEngine->AddComponent<LibraryComponentAdapter<Svr::ServerEntityManager, uint>, IHeap&, uint>(GetSystemHeap(), workerThreadCount);
	}

	void DeinitializeEngine()
	{
		SF::Engine::Stop();
	}


}
}

