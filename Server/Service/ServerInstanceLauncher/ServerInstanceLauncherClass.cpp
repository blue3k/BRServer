////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : KyungKun Ko
//
// Description : Entity server implementation
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "SvrConst.h"
#include "Server/BrServer.h"
#include "Server/ParameterSetting.h"
#include "SvrTrace.h"
#include "ServerConfig/SFServerConfig.h"
#include "ServerConfig/SFServerConfigZooKeeper.h"
#include "Entity/EntityManager.h"
#include "ServerEntity/ServerEntityManager.h"
#include "ServiceEntity/ClusterManagerServiceEntity.h"
#include "Util/TimeUtil.h"

#include "ServerInstanceLauncherClass.h"






namespace SF {
namespace ServerInstanceLauncher {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	ServerInstanceLauncher::ServerInstanceLauncher()
		: BrServer(NetClass::Server)
		, m_NewCommands(GetHeap())
		, m_CommandWatcher(GetHeap())
		, m_ProcessManager(GetHeap())
	{
	}


	ServerInstanceLauncher::~ServerInstanceLauncher()
	{
	}
	
	
	Svr::ServerEntity* ServerInstanceLauncher::CreateLoopbackEntity()
	{
		return new(GetMemoryManager()) Svr::ServerEntity;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Public Network event handling
	//

	bool ServerInstanceLauncher::OnStart()
	{
		Result hr = ResultCode::SUCCESS;

		SetServerState(Svr::ServerState::STARTING);

		svrTrace(Info, "Starting Server");

		svrChk(InitializeServerResource());

		Net::GetLocalAddress(SockFamily::IPV4, m_MyIPV4Address);
		Net::GetLocalAddress(SockFamily::IPV6, m_MyIPV6Address);

		m_CommandWatcher.SetNewCommandHandler( [this](const String& command)
		{
			m_NewCommands.Enqueue(command);
		});

		m_CommandWatcher.WatchCommands();

	Proc_End:

		if (!(hr))
		{
			// TODO
			Service::EntityManager->Clear();

			GetComponentCarrier().TerminateComponents();
			GetComponentCarrier().ClearComponents();

			Service::EntityManager->FlushDeletedEntity();

			SetServerState(Svr::ServerState::STOPED);
			svrTrace(Info, "Start failed hr:{0:X8}", hr);
		}
		else
		{
			svrTrace(Info, "Start process done");
		}

		return (hr);
	}

	// Apply configuration
	Result ServerInstanceLauncher::ApplyConfiguration()
	{
		Result hr = ResultCode::SUCCESS;

		//svrChk(Svr::BrServer::ApplyConfiguration() );

	Proc_End:

		return hr;
	}


	// Initialize server resource
	Result ServerInstanceLauncher::InitializeServerResource()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::InitializeServerResource() );

		svrChk( InitializeEntity( EntityID(EntityFaculty::Server,0)) );

	Proc_End:

		return hr;
	}

	// Close server and release resource
	Result ServerInstanceLauncher::CloseServerResource()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::CloseServerResource() );

		svrChk( TerminateEntity() );

	Proc_End:

		return hr;
	}


	Result ServerInstanceLauncher::TickUpdate(TimerAction *pAction)
	{
		String commandNodeName(GetHeap());

		while (m_NewCommands.Dequeue(commandNodeName))
		{
			Json::Value commandValue;
			String nodePath;
			auto pZKInstance = m_CommandWatcher.GetZKInstance();

			nodePath.Format("{0}/{1}", m_CommandWatcher.GetRootPath(), commandNodeName);
			if (!pZKInstance->Get(nodePath, commandValue))
				continue;

			auto ipAddress = commandValue.get("IPAddress", "").asCString();

			if (!StrUtil::StringCmp(m_MyIPV4Address.Address, -1, ipAddress, -1)
				&& !StrUtil::StringCmp(m_MyIPV6Address.Address, -1, ipAddress, -1))
				continue;

			if (!m_CommandWatcher.ConsumeCommand(commandNodeName))
				continue;


			auto commandName = commandValue.get("Command", "").asCString();

			FixedString commandNameF = commandName;
			switch (commandNameF)
			{
			case "ReloadConfig"_hash64:
				ReloadConfig(pZKInstance, commandValue);
				break;
			case "RestartServerInstance"_hash64:
				RestartServerInstance(pZKInstance, commandValue);
				break;
			case "StartServerInstance"_hash64:
				StartServerInstance(pZKInstance, commandValue);
				break;
			case "StopServerInstance"_hash64:
				StopServerInstance(pZKInstance, commandValue);
				break;
			default:
				break;
			}
		}

		return BrServer::TickUpdate(pAction);
	}


	//////////////////////////////////////////////////////////////////////////
	//
	//	Service launcher
	//

	Result ServerInstanceLauncher::ReloadConfig(ZooKeeper* pZkInstance, const Json::Value& commandValue)
	{
		auto zkconfigPath = ParameterSetting::GetSetting("zkconfig", "/ServerConfig");
		auto pServerConfig = *Service::ServerConfig;

		ServerConfigZooKeeper zkConfig(*pServerConfig, *pZkInstance);

		return zkConfig.LoadConfig(zkconfigPath);
	}

	Result ServerInstanceLauncher::RestartServerInstance(ZooKeeper* pZkInstance, const Json::Value& commandValue)
	{
		StopServerInstance(pZkInstance, commandValue);
		return StartServerInstance(pZkInstance, commandValue);
	}

	Result ServerInstanceLauncher::StartServerInstance(ZooKeeper* pZkInstance, const Json::Value& commandValue)
	{
		auto serverInstanceName = commandValue.get("ServerInstanceName", "").asCString();
		auto serverModuleName = commandValue.get("ServerExecutionModule", "").asCString();

		if (StrUtil::IsNullOrEmpty(serverInstanceName) || StrUtil::IsNullOrEmpty(serverModuleName))
		{
			svrTrace(Error, "Instance name or Module name is invalid {0}, {1}", serverInstanceName, serverModuleName);
			return ResultCode::INVALID_ARG;
		}

		char instanceNameParam[256];
		StrUtil::Format(instanceNameParam, "-n:{0}", serverInstanceName);

		char moduleNameParam[256];
#if WINDOWS
		StrUtil::Format(moduleNameParam, "{0}.exe", serverModuleName);
#else
		StrUtil::Format(moduleNameParam, "{0}", serverModuleName);
#endif

		StaticArray<const char*, 10> args(GetHeap());
		args.push_back(instanceNameParam);
		args.push_back(nullptr);

		return m_ProcessManager.StartProcess(serverInstanceName, moduleNameParam, args);
	}

	Result ServerInstanceLauncher::StopServerInstance(ZooKeeper* pZkInstance, const Json::Value& commandValue)
	{
		auto serverInstanceName = commandValue.get("ServerInstanceName", "").asCString();

		return m_ProcessManager.StopProcess(serverInstanceName);
	}






} // namespace ServerInstanceLauncher
} // namespace SF







