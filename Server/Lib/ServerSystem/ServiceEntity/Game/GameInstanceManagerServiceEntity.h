////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game instance service class definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Common/Typedefs.h"
#include "Common/ClassUtil.h"
#include "Common/Memory.h"
#include "Common/BrSvrTypes.h"
#include "Common/PageQueue.h"
#include "Common/LocalUIDGenerator.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "ServerSystem/MessageRoute.h"
#include "Common/HashTable.h"
#include "Common/Indexing.h"
#include "Common/StaticHashTable.h"

#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServiceEntity/ClusteredServiceEntity.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounter.h"



namespace BR {
namespace Svr {

	class Entity;
	class ServerEntity;
	class GameInstanceEntity;


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	GameInstanceManagerServiceEntity class
	//

	class GameInstanceManagerServiceEntity : public LoadbalanceClusterServiceEntity, public IServerComponent
	{
	public:

		enum { ComponentID = ServerComponentID_GameInstanceManagerService };

	protected:

		Svr::PerformanceCounterRaw < UINT64 > m_NumberOfInstance;

	public:

		GameInstanceManagerServiceEntity( ClusterID clusterID, ClusterMembership initialMembership = ClusterMembership::StatusWatcher );
		~GameInstanceManagerServiceEntity();


		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity operations
		//

		virtual Result InitializeEntity(EntityID newEntityID) override;

		virtual Result RegisterServiceMessageHandler( ServerEntity *pServerEntity );

		//////////////////////////////////////////////////////////////////////////
		//
		//	Game Instance operations
		//

		// Add new Entity
		virtual Result OnNewInstance(GameInstanceEntity* pGameInstance);

		// Called when a game instance is deleted
		virtual Result FreeGameInstance( GameInsUID gameUID );

	};





	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	MatchingQueueWatcherServiceEntity class
	//

	class GameInstanceManagerWatcherServiceEntity : public LoadbalanceClusterServiceEntity, public IServerComponent
	{
	public:

		enum { ComponentID = ServerComponentID_GameInstanceManagerService };

	private:

		static ClusterID ClusterIDFromGameID( GameID gameID );

	public:

		GameInstanceManagerWatcherServiceEntity( GameID gameID );
		~GameInstanceManagerWatcherServiceEntity();

	};



}; // namespace Svr
}; // namespace BR




