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
#include "ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServiceEntity/ServiceDirectoryManager.h"
#include "ServiceEntity/Game/PlayerDirectoryManager.h"
#include "Entity/EntityManager.h"
#include "Util/SFPath.h"
#include "ServerConfig/SFServerConfigJson.h"

#include "SFEngine.h"
#include "Component/SFServerNetComponent.h"
#include "Component/SFZookeeperSessionComponent.h"
#include "Component/SFConnectionManagerComponent.h"
#include "Component/SFServerMessageEndpointComponent.h"
#include "Component/SFDBTableManagerComponent.h"
#include "Component/SFLibraryComponentAdapter.h"
#include "ServerLog/SvrLogComponent.h"
#include "Service/ServerService.h"
#include "Net/SFMessageEndpointManager.h"
#include "DB/DatabaseServiceComponent.h"
#include "Mongo/SFMongoDBComponent.h"

namespace SF {

	uint GetServerUID()
	{
		return Service::ServerConfig->UID;
	}

	// Get server ID
	ServerID GetMyServerID()
	{
		return Service::ServerConfig->UID;
	}

	void InitializeEngine(SF::EngineInitParam& initParam)
	{
		constexpr uint32_t LogMaskALL = uint32_t(-1);
		auto configPath = ParameterSetting::GetSetting("config", "../config/SampleLogin.cfg");
		auto bAsService = StrUtil::StringCompairIgnoreCase(ParameterSetting::GetSetting("servicemode"), -1, "true", -1);

		ServerConfigJson loader(GetSystemHeap());
		auto result = loader.LoadConfig(configPath, *Service::ServerConfig);
		if (!result)
		{
			printf("Failed to load server config modulepath:%s hr:%08X", Util::GetModulePath(), (uint32_t)result);
			return;
		}

		if (StrUtil::IsNullOrEmpty(Service::ServerConfig->Name))
		{
			printf("Service doesn't have name hr:%08X", (uint32_t)result);
			return;
		}

		svrTrace(Info, "Initializing server: {0}", Service::ServerConfig->Name);

		Util::SetServiceName(Service::ServerConfig->Name);

		auto traceConfigPath = SF::Util::Path::Combine(SF::Util::Path::GetFileDirectory(configPath), "traceConfig.cfg");


		if (!Service::ServerConfig->LogFilePath.IsNullOrEmpty())
		{
			char strLogPath[1024];
			StrUtil::Format(strLogPath, "{0}{1}{2}", Service::ServerConfig->LogFilePath, Util::Path::DirectorySeparatorCharString, Util::GetServiceName());

			initParam.LogFilePrefix = strLogPath;
		}
		initParam.LogOutputFile = LogOutputMask(LogMaskALL);
		initParam.GlobalLogOutputMask = LogOutputMask(LogMaskALL);
		initParam.LogServerAddress = Service::ServerConfig->LogServer;

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
		pEngine->AddComponent<MongoDBComponent>();
		pEngine->AddComponent<ServerLogComponent>(traceConfigPath);
		pEngine->AddComponent<ZookeeperSessionComponent>(Service::ServerConfig->DataCenter.Server);
		pEngine->AddComponent<DatabaseServiceComponent>();

		svrTrace(Info, "Engine Initialized");
	}

	// Initialize server
	void InitializeEngineForServer()
	{
		SF::EngineInitParam initParam;

		InitializeEngine(initParam);

		// Config can be accessed after ServerConfigComponent is initialized
		auto pEngine = SF::Engine::GetInstance();
		if (pEngine == nullptr)
			return;

		pEngine->AddComponent<SF::Net::NetSystem>(initParam.NetRecvBufferSize, initParam.NetSendBufferSize, Service::ServerConfig->NetIOThreadCount, 1024);
		pEngine->AddComponent<ConnectionManagerComponent>(2048);
		pEngine->AddComponent<Svr::EntityTable>();
		pEngine->AddComponent<LibraryComponentAdapter<Svr::EntityManager, uint>, IHeap&, uint>(GetSystemHeap(), Service::ServerConfig->WorkerThreadCount);
		pEngine->AddComponent<LibraryComponentAdapter<ServiceDirectoryManager>, IHeap&>(GetSystemHeap());
		pEngine->AddComponent<LibraryComponentAdapter<MessageEndpointManager>, IHeap&>(GetSystemHeap());
		pEngine->AddComponent<ServerMessageEndpointComponent>();
		pEngine->AddComponent<LibraryComponentAdapter<PlayerDirectoryManager>, IHeap&>(GetSystemHeap());
		pEngine->AddComponent<DBTableManagerComponent>();
	}

	void DeinitializeEngine()
	{
		SF::Engine::Stop();
	}


}

