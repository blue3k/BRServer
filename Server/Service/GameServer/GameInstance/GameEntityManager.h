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

#include "Common/Typedefs.h"
#include "Common/ClassUtil.h"

#include "ServerSystem/EntityManager.h"



namespace BR {
namespace GameServer {

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
		Svr::PerformanceCounterRaw < UINT64 > m_NumberOfPlayers;

	public:
		// Constructor/Destructor
		GameEntityManager();
		virtual ~GameEntityManager();


		////////////////////////////////////////////////////////////////////////////
		//
		// TickTask add/remove proving
		//

		virtual HRESULT CreateEntity(ClusterID clusterID, EntityFaculty faculty, Svr::Entity* &pEntity) override;

		// Create new game instance
		//HRESULT CreateGamePlayer( PlayerID playerID, GamePlayerEntity* &pGamePlayer );

		virtual void OnEntityAdded(Svr::Entity* pEntity) override;
		virtual void OnEntityRemoved(Svr::Entity* pEntity) override;

		virtual void RegisterCounter() override;

		// Initialize TaskManager
		virtual HRESULT InitializeManager( UINT uiNumGroup );

		// Terminate TaskManager
		virtual HRESULT TerminateManager();

	};



}; // namespace GameServer
}; // namespace BR







