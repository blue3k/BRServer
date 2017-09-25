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

#include "ZooKeeper/SFZooKeeper.h"
#include "ZooKeeperCommand.h"

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

		CommandWatcher m_CommandWatcher;

	public:
		ServerInstanceLauncher();
		~ServerInstanceLauncher();


		///////////////////////////////////////////////////////////////////////////////////
		//
		//	
		//
		
	protected:


		virtual Svr::ServerEntity* CreateLoopbackEntity() override;

		virtual bool OnStart() override;

		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;

		virtual Result CreateServerEntity(NetClass netClass, Svr::ServerEntity* &pServerEntity) override { return ResultCode::NOT_IMPLEMENTED; }

		//////////////////////////////////////////////////////////////////////////
		//
		//	Public Network event handling
		//

		// Apply configuration
		virtual Result ApplyConfiguration();


		// Initialize server resource
		virtual Result InitializeServerResource();

		// Close server and release resource
		virtual Result CloseServerResource();


	public:


		//////////////////////////////////////////////////////////////////////////
		//
		//	Service launcher
		//

		Result ReloadConfig();
		Result RestartServerInstance();
		Result StartServerInstance();
		Result StopServerInstance();


	};


	// Query login server instance
	inline ServerInstanceLauncher* GetMyServer()
	{
		return dynamic_cast<ServerInstanceLauncher*>(Svr::BrServer::GetInstance());
	}




} // namespace ServerInstanceLauncher
} // namespace SF







