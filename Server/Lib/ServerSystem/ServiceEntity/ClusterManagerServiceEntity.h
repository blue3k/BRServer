////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Component class definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"

#include "Memory/SFMemory.h"
#include "Types/BrSvrTypes.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "Container/HashTable.h"
#include "Container/Indexing.h"

#include "Entity/EntityInformation.h"
#include "ServiceEntity/ClusteredServiceEntity.h"


namespace SF {
namespace Svr {

	class Entity;
	class ServerEntity;



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ServerServiceComponentEntity class
	//

	class ClusterManagerServiceEntity : public ReplicaClusterServiceEntity, public IServerComponent
	{
	public:

		enum { ComponentID = ServerComponentID_ClusterManagerService };

		// Server Entity table
		typedef HashTable<	uint, ClusteredServiceEntity*
									> 
				ClusterIDMap;

		// cluster ID map
		ClusterIDMap			m_ClusterIDMap;



	public:
		// Constructor/Destructor
		ClusterManagerServiceEntity( ClusterMembership initialMembership = ClusterMembership::StatusWatcher);
		virtual ~ClusterManagerServiceEntity();

		virtual Result InitializeEntity( EntityID newEntityID ) override;

		// Initialize not initialized cluster entities
		// This need to be called after clusterManagerService is initialized
		Result InitializeNotInitializedClusterEntities();

		// Get cluster service entity
		Result GetClusterServiceEntity( ClusterID clusterID, ClusteredServiceEntity* &pServiceEntity );

		// Add cluster service entity
		Result AddClusterServiceEntity( ClusteredServiceEntity* pServiceEntity );

		// Create a watcher for cluster
		Result CreateWatcherForCluster( ClusterID clusterID, ClusterType clusterType, ClusteredServiceEntity* &pServiceEntity );


		// Sync data to target 
		virtual Result SyncDataToTarget( EntityUID entityUID ) override;

		virtual Result RegisterServiceMessageHandler( ServerEntity *pServerEntity ) override;

		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;
	};




}; // namespace Svr
}; // namespace SF




