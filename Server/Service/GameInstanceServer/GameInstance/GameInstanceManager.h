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

#include "Common/TypeDefs.h"
#include "Common/ClassUtil.h"

#include "ServerSystem/ServiceEntity/GameInstanceManagerServiceEntity.h"



namespace BR {
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

	public:
		// Constructor/Destructor
		GameInstanceManagerServiceEntity( ClusterID clusterID, ClusterMembership initialMembership = ClusterMembership::StatusWatcher );
		virtual ~GameInstanceManagerServiceEntity();


		//////////////////////////////////////////////////////////////////////////
		//
		//	Game Instance operations
		//


		// Add new Entity
		virtual HRESULT CreateGameInstance(GameInsUID &gameUID, UINT numBot, UINT maxPlayer) override;

		// Called when a game instance is deleted
		virtual HRESULT FreeGameInstance(GameInsUID gameUID) override;

		virtual HRESULT RegisterServiceMessageHandler(Svr::ServerEntity *pServerEntity) override;
	};



}; // namespace ConspiracyGameInstanceServer
}; // namespace BR







