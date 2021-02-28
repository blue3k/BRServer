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

		// Query Loopback entity
		inline Svr::ServerEntity* GetLoopbackGameServerEntity();


		Svr::Entity* CreateEntity(ClusterID clusterID, EntityFaculty faculty);

		//////////////////////////////////////////////////////////////////////////
		//
		//	Public Network event handling
		//


		// Initialize server resource
		virtual Result InitializeServerResource() override;

		// Close server and release resource
		virtual Result CloseServerResource() override;


		// Initialize private Network
		virtual Result InitializeEntities() override;

		// Close Private Network
		virtual Result CloseEntities() override;


		// create remote entity by class
		virtual Result CreateServerEntity( Svr::ServerEntity* &pServerEntity ) override;


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


} // namespace GameServer
} // namespace SF







