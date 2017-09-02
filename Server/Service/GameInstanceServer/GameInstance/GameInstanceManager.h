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


#include "ServerSystem/ServiceEntity/Game/GameInstanceManagerServiceEntity.h"



namespace SF {
namespace ConspiracyGameInstanceServer {

	class GameInstanceEntity;

	

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	GameUser Entity manager
	//

	class GameInstanceManagerServiceEntity : public Svr::GameInstanceManagerServiceEntity
	{
	public:

	private:
		typedef Svr::GameInstanceManagerServiceEntity super;

	public:
		// Constructor/Destructor
		GameInstanceManagerServiceEntity( ClusterID clusterID, ClusterMembership initialMembership = ClusterMembership::StatusWatcher );
		virtual ~GameInstanceManagerServiceEntity();


		//////////////////////////////////////////////////////////////////////////
		//
		//	Game Instance operations
		//


		// Add new Entity
		//virtual Result CreateGameInstance(GameInsUID &gameUID, uint numBot, uint maxPlayer) override;

		// Called when a game instance is deleted
		//virtual Result FreeGameInstance(GameInsUID gameUID) override;

		virtual Result RegisterServiceMessageHandler(Svr::ServerEntity *pServerEntity) override;
	};



}; // namespace ConspiracyGameInstanceServer
}; // namespace SF







