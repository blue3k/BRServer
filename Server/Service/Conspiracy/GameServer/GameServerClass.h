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

#include "conspiracy/GameConfigTbl.h"



namespace SF {
namespace Net {
	class ServerMUDP;
}};


namespace SF {

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
		using GameConfigType = conspiracy::GameConfigTbl::GameConfigItem ;

	private:
		// Public network interface
		//SharedPointerT<Net::ServerMUDP>			m_pNetPublic;

		// Game cluster config
		const ServerConfig::GameCluster*	m_pGameClusterCfg;

		// game config preset
		int m_TableVersion;
		uint					m_PresetGameConfigID;
		GameConfigType* m_PresetGameConfig;


	protected:

	public:
		GameServer();
		~GameServer();


		GameConfigType* GetPresetGameConfig() { return m_PresetGameConfig; }
		
		// Update game config
		Result UpdateGameConfig(uint configPresetID);

		///////////////////////////////////////////////////////////////////////////////////
		//
		//	
		//

		// Get Login server config
		inline const ServerConfig::GameServer* GetGameConfig();

		const ServerConfig::GameCluster* GetGameClusterInfo() { return m_pGameClusterCfg; }

		//const ServerConfig::NetPublic* GetPublicNetConfig() { return &GetGameConfig()->PublicNet; }


		// Get net public
		//inline SharedPointerT<Net::ServerMUDP>& GetNetPublic();

		// Query Loopback entity
		inline Svr::ServerEntity* GetLoopbackGameServerEntity();


		Svr::Entity* CreateEntity(ClusterID clusterID, EntityFaculty faculty);

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
		//Result InitializeGameDB(ServerConfig::DBCluster* pDBClusterCfg);


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
}; // namespace SF







