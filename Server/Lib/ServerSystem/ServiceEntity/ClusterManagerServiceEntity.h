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


#include "Common/Typedefs.h"
#include "Common/ClassUtil.h"
#include "Common/Memory.h"
#include "Common/BrSvrTypes.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "Common/HashTable.h"
#include "Common/Indexing.h"
#include "Common/StaticHashTable.h"

#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServiceEntity/ClusteredServiceEntity.h"


namespace BR {
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
		typedef Hash::HashTable<	UINT, ClusteredServiceEntity*
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

		virtual Result TickUpdate(Svr::TimerAction *pAction = nullptr) override;
	};




}; // namespace Svr
}; // namespace BR




