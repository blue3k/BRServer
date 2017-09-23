////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Entity server main
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Server/BrServer.h"
#include "Entity/Entity.h"
#include "Transaction/Transaction.h"



namespace SF {
namespace ServerInstanceLauncher {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//

	class ServerInstanceLauncher : public Svr::BrServer
	{
	public:

	private:

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



	};


	// Query login server instance
	inline ServerInstanceLauncher* GetMyServer()
	{
		return dynamic_cast<ServerInstanceLauncher*>(Svr::BrServer::GetInstance());
	}




} // namespace ServerInstanceLauncher
} // namespace SF







