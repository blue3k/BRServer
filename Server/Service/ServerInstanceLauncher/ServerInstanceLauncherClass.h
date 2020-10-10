////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StromForge
// 
// Author : KyungKun Ko
//
// Description : Launcher service main class
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Server/BrServer.h"

#include "Zookeeper/SFZookeeper.h"
#include "ZookeeperCommand.h"
#include "SFProcessManager.h"

namespace SF {
namespace ServerInstanceLauncher {




	//////////////////////////////////////////////////////////////////////////
	//
	//	Server Instance Launcher server class
	//

	class ServerInstanceLauncher : public Svr::BrServer
	{
	public:

	private:

		NetAddress m_MyIPV4Address;
		NetAddress m_MyIPV6Address;

		CircularPageQueue<StringCrc64> m_NewCommands;

		CommandWatcher m_CommandWatcher;
		ProcessManager m_ProcessManager;

	public:
		ServerInstanceLauncher();
		~ServerInstanceLauncher();


		///////////////////////////////////////////////////////////////////////////////////
		//
		//	
		//
		
	protected:


		virtual Svr::ServerEntity* CreateLoopbackEntity() override;

		virtual Result OnStart() override;

		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;

		virtual Result CreateServerEntity(NetClass netClass, Svr::ServerEntity* &pServerEntity) override { return ResultCode::NOT_IMPLEMENTED; }


		Result UpdateCommand();


		//////////////////////////////////////////////////////////////////////////
		//
		//	Public Network event handling
		//

		// Apply configuration
		virtual Result ApplyConfiguration() override;


		// Initialize server resource
		virtual Result InitializeServerResource() override;

		// Close server and release resource
		virtual Result CloseServerResource() override;


	public:

		Result RegisterRunningServices();

		//////////////////////////////////////////////////////////////////////////
		//
		//	Service launcher
		//

		Result ReloadConfig(Zookeeper* pZkInstance, const Json::Value& commandValue);
		Result RestartServerInstance(Zookeeper* pZkInstance, const Json::Value& commandValue);
		Result StartServerInstance(Zookeeper* pZkInstance, const Json::Value& commandValue);
		Result StopServerInstance(Zookeeper* pZkInstance, const Json::Value& commandValue);


	};


	// Query login server instance
	inline ServerInstanceLauncher* GetMyServer()
	{
		return dynamic_cast<ServerInstanceLauncher*>(Svr::BrServer::GetInstance());
	}




} // namespace ServerInstanceLauncher
} // namespace SF







