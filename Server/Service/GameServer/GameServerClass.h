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

#include "Common/TypeDefs.h"
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
namespace GameServer
{

	class GameEntityManager;
	class EntityServerEntity;
	class GameServerEntity;



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
		const Svr::Config::GameCluster*	m_pGameClusterCfg;

		// game config preset
		UINT					m_PresetGameConfigID;
		BRCLASS_ATTRIBUTE_READONLY_PTR(GameConfigType*,PresetGameConfig);

		BRCLASS_ATTRIBUTE_READONLY(NetAddress, PublicNetAddressIPv4);

	protected:

		// Create entity manager
		virtual Svr::EntityManager* CreateEntityManager() override;
		virtual Svr::ServerEntity* CreateLoopbackEntity() override;

	public:
		GameServer();
		~GameServer();

		virtual MemoryAllocator& GetAllocator() override			{ return STDAllocator::GetInstance(); }


		
		// Update game config
		HRESULT UpdateGameConfig(UINT configPresetID);

		///////////////////////////////////////////////////////////////////////////////////
		//
		//	
		//

		// Get Login server config
		inline const Svr::Config::PublicServer* GetGameConfig();

		// Get net public
		inline Net::ServerMUDP* GetNetPublic();

		// Query Loopback entity
		inline GameServerEntity* GetLoopbackGameServerEntity();
		
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
		//HRESULT InitializeGameDB(Svr::Config::DBCluster* pDBClusterCfg);


		// Initialize private Network
		virtual HRESULT InitializeNetPublic() override;

		// Close Public Network
		virtual HRESULT CloseNetPublic() override;


	};


	// Query Game server instance
	inline GameServer* GetMyServer();

	// Query Loopback entity
	inline GameServerEntity* GetLoopbackGameServerEntity();

	// Get Entity Server
	//EntityServerEntity* GetEntityServer();


#include "GameServerClass.inl"


}; // namespace GameServer
}; // namespace BR







