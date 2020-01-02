////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2019 The Braves Corporation.
// 
// Author : KyungKun Ko
//
// Description : RelayServerClass
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Server/BrServer.h"
#include "Entity/Entity.h"
#include "Transaction/Transaction.h"





namespace SF {
namespace Net {
	class RawUDP;
}};


namespace SF {

	class RelayServerEntity;



	//////////////////////////////////////////////////////////////////////////
	//
	//	Relay server class
	//

	class RelayServer : public Svr::BrServer
	{
	public:

	private:

		ServerConfig::GenericServer m_MyConfig;

	protected:


	public:
		RelayServer();
		~RelayServer();


		//////////////////////////////////////////////////////////////////////////
		//
		//	Public Network event handling
		//

		virtual Result SetupConfiguration() override;

		virtual Result CreateEntityManager() override;

		// Apply configuration
		virtual Result ApplyConfiguration() override;

		virtual Result InitializeMonitoring() override;


		// Initialize server resource
		virtual Result InitializeServerResource() override;

		// Close server and release resource
		virtual Result CloseServerResource() override;


		// Initialize private Network
		virtual Result InitializeNetPrivate() override;

		// Close Private Network
		virtual Result CloseNetPrivate() override;


		// create remote entity by class
		virtual Result CreateServerEntity( NetClass netClass, Svr::ServerEntity* &pServerEntity ) override;

		virtual Result CreateServerInstanceZK(const char* nodeName) override;

		//////////////////////////////////////////////////////////////////////////
		// GSDK callbacks
		void OnShutdownRequested();
		bool IsAlive();
		void OnMaintenanceScheduled(tm t);

	};


	// Query Game server instance
	inline RelayServer* GetMyServer() { return static_cast<RelayServer*>(Svr::BrServer::GetInstance()); }




} // namespace SF







