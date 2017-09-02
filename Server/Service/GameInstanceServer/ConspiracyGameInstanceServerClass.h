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

#include "SFTypedefs.h"
#include "Server/BrServer.h"
#include "Entity/Entity.h"
#include "Transaction/Transaction.h"



//namespace SF {
//namespace Policy {
//		interface IPolicyEntityServer;
//		interface IPolicyGameMasterServer;
//		interface IPolicyGameServer;
//		interface IPolicyGameInstance;
//}};
//

namespace SF {
namespace Net {
	class ServerMUDP;
}};


namespace SF {
namespace ConspiracyGameInstanceServer
{

	class EntityServerEntity;
	class GameServerEntity;
	class ConspiracyGameInstanceServerEntity;



	//////////////////////////////////////////////////////////////////////////
	//
	//	Game server class
	//

	class GameInstanceServer : public Svr::BrServer
	{
	public:

	private:
		// Game cluster config
		const Svr::Config::GameClusterInfo*	m_pGameClusterCfg;

	protected:

		virtual Svr::EntityManager* CreateEntityManager() override;

		// Create entity manager
		virtual Svr::ServerEntity* CreateLoopbackEntity() override;

	public:
		GameInstanceServer();
		~GameInstanceServer();


		///////////////////////////////////////////////////////////////////////////////////
		//
		//	
		//

		// Query Loopback entity
		inline ConspiracyGameInstanceServerEntity* GetLoopbackGameServerEntity();
		
		//////////////////////////////////////////////////////////////////////////
		//
		//	virtual network process
		//



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


		// Initialize private Network
		virtual Result InitializeNetPrivate() override;

		// Close Private Network
		virtual Result CloseNetPrivate() override;


		// create remote entity by class
		virtual Result CreateServerEntity( NetClass netClass, Svr::ServerEntity* &pServerEntity ) override;


	};


	// Query Game server instance
	inline GameInstanceServer* GetMyServer();

	// Query Loopback entity
	inline ConspiracyGameInstanceServerEntity* GetLoopbackGameServerEntity();

	// Get Entity Server
	//EntityServerEntity* GetEntityServer();


#include "ConspiracyGameInstanceServerClass.inl"


}; // namespace ConspiracyGameInstanceServer
}; // namespace SF







