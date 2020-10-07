////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : KyungKun Ko
//
// Description : Entity server implementation
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerInstanceLanucherPCH.h"
#include "SvrConst.h"
#include "Server/BrServer.h"
#include "Server/ParameterSetting.h"
#include "SvrTrace.h"
#include "ServerConfig/SFServerConfig.h"
#include "ServerConfig/SFServerConfigZookeeper.h"
#include "Entity/EntityManager.h"
#include "ServerEntity/ServerEntityManager.h"
#include "ServiceEntity/ClusterManagerServiceEntity.h"
#include "Util/SFTimeUtil.h"
#include "SvrConst.h"

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
			return new(GetHeap()) Svr::ServerEntity;
		}

		//////////////////////////////////////////////////////////////////////////
		//
		//	Public Network event handling
		//

		Result ServerInstanceLauncher::OnStart()
		{
			FunctionContext hr([this](Result hr)
				{
					if (!hr)
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
				});
			char instanceName[512];

			SetServerState(Svr::ServerState::STARTING);

			svrTrace(Info, "Starting Server");

			svrCheck(InitializeServerResource());

			Net::GetLocalAddress(SockFamily::IPV4, m_MyIPV4Address);
			Net::GetLocalAddress(SockFamily::IPV6, m_MyIPV6Address);

			// override watch address
			m_MyIPV4Address = ParameterSetting::GetSetting("watchIPV4", m_MyIPV4Address.Address);
			m_MyIPV6Address = ParameterSetting::GetSetting("watchIPV6", m_MyIPV6Address.Address);

			svrTrace(Info, "Listening commands for IPV4: {0}", m_MyIPV4Address);
			svrTrace(Info, "                       IPV6: {0}", m_MyIPV6Address);


			svrCheck(RegisterRunningServices());


			m_CommandWatcher.SetNewCommandHandler([this](const StringCrc64& command)
				{
					m_NewCommands.Enqueue(command);
				});

			m_CommandWatcher.WatchCommands();


			StrUtil::Format(instanceName, "{0}_{1}", Util::GetServiceName(), m_MyIPV4Address.Address);
			svrCheck(CreateServerInstanceZK(instanceName));

			SetServerState(Svr::ServerState::RUNNING);

			return hr;
		}

		// Apply configuration
		Result ServerInstanceLauncher::ApplyConfiguration()
		{
			FunctionContext hr;

			return hr;
		}


		// Initialize server resource
		Result ServerInstanceLauncher::InitializeServerResource()
		{
			FunctionContext hr;

			svrCheck(Svr::BrServer::InitializeServerResource());

			svrCheck(InitializeEntity(EntityID(EntityFaculty::Server, 0)));

			return hr;
		}

		// Close server and release resource
		Result ServerInstanceLauncher::CloseServerResource()
		{
			FunctionContext hr;

			svrCheck(Svr::BrServer::CloseServerResource());

			svrCheck(TerminateEntity());

			return hr;
		}


		Result ServerInstanceLauncher::TickUpdate(TimerAction* pAction)
		{
			m_ProcessManager.UpdateProcessStatus();

			UpdateCommand();

			return BrServer::TickUpdate(pAction);
		}


		Result ServerInstanceLauncher::UpdateCommand()
		{
			FunctionContext hr;

			StringCrc64 commandNodeNameCrc;

			while (m_NewCommands.Dequeue(commandNodeNameCrc))
			{
				auto commandNodeName = commandNodeNameCrc.ToString();
				Json::Value commandValue;
				String nodePath;
				auto pZKInstance = m_CommandWatcher.GetZKInstance();

				nodePath.Format("{0}/{1}", m_CommandWatcher.GetRootPath(), commandNodeName);
				if (!pZKInstance->Get(nodePath, commandValue))
					continue;

				auto ipAddress = commandValue.get("IPAddress", "");

				bool isMyIP4Address = StrUtil::StringCompair(m_MyIPV4Address.Address, -1, ipAddress.asCString(), -1);
				bool isMyIP6Address = StrUtil::StringCompair(m_MyIPV6Address.Address, -1, ipAddress.asCString(), -1);
				if (!isMyIP4Address && !isMyIP6Address)
					continue;

				if (!m_CommandWatcher.ConsumeCommand(commandNodeName))
					continue;

				auto commandName = commandValue.get("Command", "");

				StringCrc64 commandNameCrc = commandName.asCString();
				switch (commandNameCrc)
				{
				case "ReloadConfig"_hash64:
					svrCheck(ReloadConfig(pZKInstance, commandValue));
					break;
				case "RestartServerInstance"_hash64:
					svrCheck(RestartServerInstance(pZKInstance, commandValue));
					break;
				case "StartServerInstance"_hash64:
					svrCheck(StartServerInstance(pZKInstance, commandValue));
					break;
				case "StopServerInstance"_hash64:
					svrCheck(StopServerInstance(pZKInstance, commandValue));
					break;
				default:
					break;
				}
			}

			return hr;
		}


		Result ServerInstanceLauncher::RegisterRunningServices()
		{
			auto zkSession = Service::ZKSession->GetZookeeperSession();
			if (zkSession == nullptr || !zkSession->IsConnected())
				return ResultCode::NOT_INITIALIZED;

			svrTrace(Debug, "ZK requesting command list");

			DynamicArray<String> children(GetHeap());
			char nodePath[512];
			zkSession->GetChildren(Svr::Const::ZK_SERVER_INSTANCE_NODE, children);
			for (auto itChild : children)
			{
				Json::Value nodeValue(Json::objectValue);
				StrUtil::Format(nodePath, "{0}/{1}", Svr::Const::ZK_SERVER_INSTANCE_NODE, itChild);

				if (!zkSession->Get(nodePath, nodeValue))
					continue;

				auto ipAddress = nodeValue.get("IPAddress", "");
				if (StrUtil::IsNullOrEmpty(ipAddress.asCString()))
					continue;

				if (!StrUtil::StringCompair(m_MyIPV4Address.Address, -1, ipAddress.asCString(), -1)
					&& !StrUtil::StringCompair(m_MyIPV6Address.Address, -1, ipAddress.asCString(), -1))
					continue;

				auto serviceName = nodeValue.get("ServiceName", "");
				auto processID = nodeValue.get("ProcessID", 0);

				if (StrUtil::IsNullOrEmpty(serviceName.asCString()))
					continue;

				if (processID.asUInt64() == 0)
					continue;

				m_ProcessManager.ManageProcesses(serviceName.asCString(), processID.asUInt());
			}

			return ResultCode::SUCCESS;
		}

		//////////////////////////////////////////////////////////////////////////
		//
		//	Service launcher
		//

		Result ServerInstanceLauncher::ReloadConfig(Zookeeper* pZkInstance, const Json::Value& commandValue)
		{
			auto zkconfigPath = ParameterSetting::GetSetting("zkconfig", "/ServerConfig");
			auto pServerConfig = *Service::ServerConfig;

			svrTrace(Error, "Reloading configuration from {0}", zkconfigPath);

			ServerConfigZookeeper zkConfig(*pServerConfig, *pZkInstance);

			return zkConfig.LoadConfig(zkconfigPath);
		}

		Result ServerInstanceLauncher::RestartServerInstance(Zookeeper* pZkInstance, const Json::Value& commandValue)
		{
			StopServerInstance(pZkInstance, commandValue);
			return StartServerInstance(pZkInstance, commandValue);
		}

		Result ServerInstanceLauncher::StartServerInstance(Zookeeper* pZkInstance, const Json::Value& commandValue)
		{
			auto serverInstanceName = commandValue.get("ServerInstanceName", "");
			auto serverModuleName = commandValue.get("ServerExecutionModule", "");
			//auto configuration = commandValue.get("configuration", "Release"); // TODO: disabled for now. 
			std::string modulePath = ParameterSetting::GetSetting("modulepath", "");
			std::string zkaddress = ParameterSetting::GetSetting("zkaddress", "127.0.0.1:2181");
			std::string zkconfig = ParameterSetting::GetSetting("zkconfig", "/ServerConfig");

			// Override options with the one in command
			zkaddress = commandValue.get("ZKAddress", zkaddress).asString();
			zkconfig = commandValue.get("ZKConfig", zkconfig).asString();

			svrTrace(Error, "Starting Server Instance {0}, {1}", serverInstanceName.asCString(), serverModuleName.asCString());

			if (StrUtil::IsNullOrEmpty(serverInstanceName.asCString()) || StrUtil::IsNullOrEmpty(serverModuleName.asCString()))
			{
				svrTrace(Error, "Instance name or Module name is invalid {0}, {1}", serverInstanceName.asCString(), serverModuleName.asCString());
				return ResultCode::INVALID_ARG;
			}

			char instanceNameParam[256];
			StrUtil::Format(instanceNameParam, "-servicename:{0}", serverInstanceName.asCString());

			char zkaddressParam[128];
			StrUtil::Format(zkaddressParam, "-zkaddress:{0}", zkaddress.c_str());

			char zkconfigParam[128];
			StrUtil::Format(zkconfigParam, "-zkconfig:{0}", zkconfig.c_str());

			PathString moduleNameParam;
			if (modulePath.length() > 0)
			{
				moduleNameParam.SetPathString(modulePath.c_str());
				moduleNameParam.Combine(serverModuleName.asCString());
			}
			else
			{
				moduleNameParam.SetPathString(serverModuleName.asCString());
			}

			String modulePathString = moduleNameParam;
#if SF_PLATFORM == SF_PLATFORM_WINDOWS
			modulePathString.Append(".exe");
#endif

			StaticArray<const char*, 10> args(GetHeap());
			args.push_back(instanceNameParam);
			args.push_back(zkaddressParam);
			args.push_back(zkconfigParam);
			args.push_back(nullptr);

			svrTrace(Info, "	Server Instance module path {0}", modulePathString);

			return m_ProcessManager.StartProcess(serverInstanceName.asCString(), modulePathString, args);
		}

		Result ServerInstanceLauncher::StopServerInstance(Zookeeper* pZkInstance, const Json::Value& commandValue)
		{
			auto serverInstanceName = commandValue.get("ServerInstanceName", "");

			svrTrace(Error, "Stopping Server Instance {0}", serverInstanceName.asCString());

			return m_ProcessManager.StopProcess(serverInstanceName.asCString());
		}


	} // namespace ServerInstanceLauncher
} // namespace SF

