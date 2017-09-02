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


#include "SFTypedefs.h"

#include "Memory/SFMemory.h"
#include "Types/BrSvrTypes.h"
#include "Container/PageQueue.h"
#include "Util/LocalUIDGenerator.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "Transaction/MessageRoute.h"
#include "Container/HashTable.h"
#include "Container/Indexing.h"

#include "Entity/EntityInformation.h"
#include "ServiceEntity/ClusteredServiceEntity.h"
#include "PerformanceCounter/PerformanceCounter.h"



namespace SF {
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

		Svr::PerformanceCounterRaw < uint64_t > m_NumberOfInstance;

	public:

		GameInstanceManagerServiceEntity( ClusterID clusterID, ClusterMembership initialMembership = ClusterMembership::StatusWatcher );
		~GameInstanceManagerServiceEntity();


		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity operations
		//

		virtual Result InitializeEntity(EntityID newEntityID) override;

		virtual Result RegisterServiceMessageHandler( ServerEntity *pServerEntity ) override;

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
}; // namespace SF




