////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game entity manager
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"


#include "Entity/EntityManager.h"



namespace SF {
namespace ConspiracyGameInstanceServer {

	class GameServer;
	class GameInstanceEntity;
	class GamePlayerEntity;
	class GamePartyEntity;



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	GameUser Entity manager
	//

	class GameEntityManager : public Svr::EntityManager
	{
	public:
		typedef Svr::EntityManager super;

	private:
		Svr::PerformanceCounterRaw < uint64_t > m_NumberOfPlayers;

	public:
		// Constructor/Destructor
		GameEntityManager();
		virtual ~GameEntityManager();


		////////////////////////////////////////////////////////////////////////////
		//
		// TickTask add/remove proving
		//

		virtual Result CreateEntity(ClusterID clusterID, EntityFaculty faculty, Svr::Entity* &pEntity) override;

		// Create new game instance
		//Result CreateGamePlayer( PlayerID playerID, GamePlayerEntity* &pGamePlayer );

		virtual void OnEntityAdded(Svr::Entity* pEntity) override;
		virtual void OnEntityRemoved(Svr::Entity* pEntity) override;

		virtual void RegisterCounter() override;

		// Initialize TaskManager
		virtual Result InitializeManager( uint uiNumGroup );

		// Terminate TaskManager
		virtual Result TerminateManager();

	};



}; // namespace GameServer
}; // namespace SF







