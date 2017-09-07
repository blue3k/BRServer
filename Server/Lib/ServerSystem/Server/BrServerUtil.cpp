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

#include "SFEngine.h"
#include "Component/SFServerConfigComponent.h"
#include "Component/SFZooKeeperSessionComponent.h"
#include "Component/SFConnectionManagerComponent.h"
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



	// Initialize and deinitialization
	void InitializeEngineForServer()
	{
		//char strCfgPath[1024];
		auto bIsDebugRun = StrUtil::StringCmpLwr(ParameterSetting::GetSetting("debug"), -1, "true", -1) == 0;
		auto zkaddress = ParameterSetting::GetSetting("zkaddress", "127.0.0.1:2181");
		auto zkconfig = ParameterSetting::GetSetting("zkconfig", "/ServerConfig");
		SF::EngineInitParam initParam;

		//StrUtil::Format(strCfgPath, "{0}{1}", Util::GetModulePath(), ParameterSetting::GetSetting("config", "..\\..\\Config\\ServerConfig.xml"));


		initParam.LogFilePrefix = Util::GetServiceName();
		initParam.LogOutputFile = LogChannelMask();
		initParam.AsyncTaskThreadCount = 6;
		initParam.NetworkThreadCount = 4;

		if (!bIsDebugRun)
			initParam.LogOutputConsole = { 0, };


		auto pEngine = SF::Engine::Start(initParam);
		if (pEngine == nullptr)
			return;

		pEngine->AddComponent<ZooKeeperSessionComponent>(zkaddress);
		pEngine->AddComponent<ServerConfigComponent>(zkconfig);
		pEngine->AddComponent<ConnectionManagerComponent>(2048);
		pEngine->AddComponent<EntityTable>();
	}

	void DeinitializeEngine()
	{
		SF::Engine::Stop();
	}


}
}

