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
#include "ServerSystem/BrServer.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/Transaction.h"
#include "Common/ClassUtil.h"
#include "Table/conspiracy/GameConfigTbl.h"



namespace BR {
namespace Net {
	class ServerMUDP;
}};


namespace BR {

	namespace Svr {
		class ServerEntity;
	}

namespace GameServer
{


	//////////////////////////////////////////////////////////////////////////
	//
	//	Game server class
	//

	class GameServer : public Svr::BrServer
	{
	public:
		typedef ::conspiracy::GameConfigTbl::GameConfigItem GameConfigType;

	private:
		// Public network interface
		Net::ServerMUDP*			m_pNetPublic;

		// Game cluster config
		const Svr::Config::GameClusterInfo*	m_pGameClusterCfg;

		// game config preset
		int m_TableVersion;
		UINT					m_PresetGameConfigID;
		BRCLASS_ATTRIBUTE_READONLY_PTR(GameConfigType*,PresetGameConfig);

		//BRCLASS_ATTRIBUTE_READONLY(NetAddress, PublicNetAddressIPv4);

	protected:

		// Create entity manager
		virtual Svr::EntityManager* CreateEntityManager() override;
		virtual Svr::ServerEntity* CreateLoopbackEntity() override;

	public:
		GameServer();
		~GameServer();



		
		// Update game config
		Result UpdateGameConfig(UINT configPresetID);

		///////////////////////////////////////////////////////////////////////////////////
		//
		//	
		//

		// Get Login server config
		inline const Svr::Config::PublicServer* GetGameConfig();

		const Svr::Config::GameClusterInfo* GetGameClusterInfo() { return m_pGameClusterCfg; }

		const Svr::Config::PublicNetSocket* GetPublicNetConfig() { return GetGameConfig()->NetPublic; }


		// Get net public
		inline Net::ServerMUDP* GetNetPublic();

		// Query Loopback entity
		inline Svr::ServerEntity* GetLoopbackGameServerEntity();
		
		//////////////////////////////////////////////////////////////////////////
		//
		//	virtual network process
		//

		// Process Public network event
		virtual Result ProcessPublicNetworkEvent() override;


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


		//Result InitializeAccountDB();
		//Result InitializeGameDB(Svr::Config::DBCluster* pDBClusterCfg);


		// Initialize private Network
		virtual Result InitializeNetPublic() override;

		// Close Public Network
		virtual Result CloseNetPublic() override;

		// Run the task
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;

	};


	// Query Game server instance
	inline GameServer* GetMyServer();

	// Query Loopback entity
	inline Svr::ServerEntity* GetLoopbackGameServerEntity();

	// Get Entity Server
	//EntityServerEntity* GetEntityServer();


#include "GameServerClass.inl"


}; // namespace GameServer
}; // namespace BR







