////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game server class
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/Transaction.h"
#include "Common/ClassUtil.h"
#include "Common/GlobalUIDGenerator.h"


namespace BR {
namespace Net {
	class ServerMUDP;
}};


namespace BR {
namespace LoginServer
{


	//////////////////////////////////////////////////////////////////////////
	//
	//	Game server class
	//

	class LoginServer : public Svr::BrServer
	{
	public:

	private:
		// Public network interface
		Net::ServerMUDP*			m_pNetPublic;

		// 
		BRCLASS_ATTRIBUTE_CONST(GlobalUIDGenerator,AuthTicketGenerator);

	protected:

		// Create entity manager
		virtual Svr::EntityManager* CreateEntityManager() override;
		//virtual Svr::ServerEntity* CreateLoopbackEntity() override;



	public:
		LoginServer();
		~LoginServer();


		///////////////////////////////////////////////////////////////////////////////////
		//
		//	
		//

		// Get net public
		inline Net::ServerMUDP* GetNetPublic();

		
		//////////////////////////////////////////////////////////////////////////
		//
		//	virtual network process
		//

		// Process Public network event
		virtual HRESULT ProcessPublicNetworkEvent() override;


		//////////////////////////////////////////////////////////////////////////
		//
		//	Public Network event handling
		//


		// Apply configuration
		virtual HRESULT ApplyConfiguration() override;


		// Initialize server resource
		virtual HRESULT InitializeServerResource() override;

		// Close server and release resource
		virtual HRESULT CloseServerResource() override;


		// Initialize private Network
		virtual HRESULT InitializeNetPrivate() override;

		// Close Private Network
		virtual HRESULT CloseNetPrivate() override;


		// create remote entity by class
		virtual HRESULT CreateServerEntity( NetClass netClass, Svr::ServerEntity* &pServerEntity ) override;


		//HRESULT InitializeAccountDB();
		//HRESULT InitializeSessionDB(Svr::Config::DBCluster *pDBClusterCfg);


		// Initialize private Network
		virtual HRESULT InitializeNetPublic() override;

		// Close Public Network
		virtual HRESULT CloseNetPublic() override;


	};


	// Query Game server instance
	inline LoginServer* GetMyServer();



#include "LoginServerClass.inl"


}; // namespace LoginServer
}; // namespace BR







