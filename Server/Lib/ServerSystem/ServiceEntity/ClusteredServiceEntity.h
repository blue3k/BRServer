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
#include "Memory/MemoryPool.h"
#include "Util/TimeUtil.h"
#include "Types/BrSvrTypes.h"
#include "ServerSystem/ServiceEntity/ServiceEntity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "Container/HashTable.h"
#include "Container/Indexing.h"
#include "Container/StaticHashTable.h"

#include "ServerSystem/ServiceEntity/EntityInformation.h"


namespace SF {
namespace Svr {

	class Entity;
	class ServerEntity;


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ClusteredServiceEntity class
	//


	class ClusteredServiceEntity : public ServiceEntity
	{
	public:

		enum {
			MASTER_CHECK_TIME = 3000,
		};

		class ServiceTableItem : public ServerServiceInformation, public MemoryPoolObject<ServiceTableItem>
		{
		public:
			// Hash table mapping Item
			typedef DoubleLinkedList<uint64_t>::Node TableItemType;
			TableItemType m_TableNode;

			// For a Ordered list
			TableItemType m_ListNode;

		private:

		public:

			// Constructor with constructor
			ServiceTableItem( ClusterID clusterID, ServerEntity* pServerEntity, ClusterMembership membership )
				:ServerServiceInformation(clusterID, pServerEntity, membership)
			{
				memset(&m_TableNode, 0, sizeof(m_TableNode) );
				memset(&m_ListNode, 0, sizeof(m_ListNode) );
			}

		};


		typedef ServiceTableItem::TableItemType TableItemType;
		typedef StaticHashTable< uint64_t, ServiceTableItem,
									MapItemConverter<ServiceTableItem,TableItemType,&ServiceTableItem::m_TableNode>,
									UniqueKeyTrait, ThreadSyncTraitNoneT<uint64_t, ServiceTableItem>
									> ServiceEntityUIDMap;

	private:

		// Master instance entity UID
		EntityUID m_MasterUID;

		// Cluster ID
		ClusterID m_ClusterID;

		// Cluster type
		ClusterType m_ClusterType;

		// Cluster member ship of this instance
		ClusterMembership m_ClusterMembership;

		ServiceStatus m_ServiceStatus;

		uint m_Workload;

		// Cluster member instance for itself
		ServiceTableItem* m_MyServiceInfo = nullptr;

		// Service UID map
		ServiceEntityUIDMap				m_ServiceEntityUIDMap;

		// Service watcher list
		ServiceEntityUIDMap				m_WatcherUIDMap;

		ServerEntity* m_ServerEntity = nullptr;

		// Is voting going on?
		bool m_IsInVoting = false;

		bool m_Initialized = false;

	public:

		// Check connectivity of two service
		static bool CheckConnectivity( NetClass netClass1, ClusterMembership membership1, NetClass netClass2, ClusterMembership membership2 );

		bool CheckConnectivity( NetClass netClass2, ClusterMembership membership2 );

	public:

		ClusteredServiceEntity( ClusterType clusterType, ClusterID clusterID, ClusterMembership initialMembership = ClusterMembership::StatusWatcher, ServerEntity* pServerEntity = nullptr );
		virtual ~ClusteredServiceEntity();

		EntityUID GetMasterUID() { return m_MasterUID; }
		void SetMasterUID(EntityUID value) { m_MasterUID = value; }

		ClusterID GetClusterID() { return m_ClusterID; }
		void SetClusterID(ClusterID value) { m_ClusterID = value; }

		ClusterType GetClusterType() { return m_ClusterType; }
		void SetClusterType(ClusterType value) { m_ClusterType = value; }

		ClusterMembership GetClusterMembership() { return m_ClusterMembership; }
		void SetClusterMembership(ClusterMembership value) { m_ClusterMembership = value; }

		ServiceStatus GetServiceStatus() { return m_ServiceStatus; }
		void SetServiceStatus(ServiceStatus value) { m_ServiceStatus = value; }

		uint GetWorkload() { return m_Workload; }
		void SetWorkload(uint value) { m_Workload = value; }

		uint GetWorkload() { return m_Workload; }
		void SetWorkload(uint value) { m_Workload = value; }

		ServiceTableItem* GetMyServiceInfo() { return m_MyServiceInfo; }

		ServerEntity* GetServerEntity() { return m_ServerEntity; }
		void SetServerEntity(ServerEntity* value) { m_ServerEntity = value; }

		bool GetIsInVoting() { return m_IsInVoting; }

		bool GetInitialized() { return m_Initialized; }


		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity 
		//

		// Initialize entity to proceed new connection
		virtual Result InitializeEntity( EntityID newEntityID ) override;

		virtual Result StartInitializeTransaction();

		// clear transaction
		virtual Result ClearEntity() override;

		virtual Result UpdateOnMasterManager() { return ResultCode::SUCCESS; }

		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;
		virtual Result ProcessTransaction(Transaction* &pTrans) override;

		//////////////////////////////////////////////////////////////////////////
		//
		//	Voting information
		//


		void StartVoting();
		void EndVoting();

		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity informations
		//


		// Change workload
		Result SetWorkload( uint workload );

		// Change service status
		Result SetServiceStatus( ServiceStatus newStatus );

		size_t GetNumberOfServices()				{ return m_ServiceEntityUIDMap.size() + m_WatcherUIDMap.size(); }
		size_t GetNumberOfNonWatcherServices()		{ return m_ServiceEntityUIDMap.size(); }
		size_t GetNumberOfAvailableServices();

		// Register Entity by Given serverID
		virtual Result NewServerService( EntityUID entityUID, ServerEntity *pServerEntity, ClusterMembership membership, ServiceStatus status, ServerServiceInformation* &pService );

		// Find Service information by ServiceID
		Result FindService( EntityUID entityUID, ServerServiceInformation* &pService );

		// Find random service, maybe lowest workload service
		virtual Result FindRandomService( ServerServiceInformation* &pService );

		// Foreach service
		Result ForEach( std::function<void(ServerServiceInformation*)> func );

		Result ForEachNonWatcher( std::function<void(ServerServiceInformation*)> func );

		// Assign master to given UID
		Result SetMaster( EntityUID entityUID );
		// SetMaster + broadcast
		Result AssignMaster( EntityUID entityUID );

		template<class ServiceType>
		Result GetService( ServiceType* &pService );

		// Sync data to target 
		virtual Result SyncDataToTarget( EntityUID entityUID );

	};


	class WatcherClusteredServiceEntity : public ClusteredServiceEntity
	{
	public:

		WatcherClusteredServiceEntity( ClusterType clusterType, ClusterID clusterID, ClusterMembership initialMembership = ClusterMembership::StatusWatcher, ServerEntity* pServerEntity = nullptr );
	};

	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ReplicaClusterServiceEntity class
	//


	class ReplicaClusterServiceEntity : public ClusteredServiceEntity
	{
	private:
		// Master check timer
		Util::TimeStampTimer m_MasterCheckTimer;

	public:

		ReplicaClusterServiceEntity( ClusterID clusterID, ClusterMembership initialMembership = ClusterMembership::StatusWatcher, ServerEntity* pServerEntity = nullptr );

		virtual Result UpdateOnMasterManager() override;

		// Register message handler for this component
		virtual Result RegisterServiceMessageHandler( ServerEntity *pServerEntity ) override;

		////////////////////////////////////////////////////////////////////////////////////
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;
	};
	


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	FreeReplicaClusterServiceEntity class
	//


	class FreeReplicaClusterServiceEntity : public ClusteredServiceEntity
	{
	public:

	private:

	public:

		FreeReplicaClusterServiceEntity( ClusterID clusterID, ClusterMembership initialMembership = ClusterMembership::StatusWatcher, ServerEntity* pServerEntity = nullptr );

		// Register message handler for this component
		//virtual Result RegisterServiceMessageHandler( ServerEntity *pServerEntity ) override;

		////////////////////////////////////////////////////////////////////////////////////
	};
	

	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	RingClusterServiceEntity class
	//


	class RingClusterServiceEntity : public ClusteredServiceEntity
	{
	public:

		typedef OrderedLinkedList<uint64_t> OrderedServiceList;

	private:
		// Ordered service list
		OrderedServiceList	m_ServiceList;

		CriticalSection m_ListLock;

		// round robine query
		ServerServiceInformation*	m_pCurrentQueryService;

	public:
		RingClusterServiceEntity( ClusterID clusterID, ClusterMembership initialMembership = ClusterMembership::StatusWatcher, ServerEntity* pServerEntity = nullptr );

		// Override so that we can make ring linked list
		Result NewServerService( EntityUID entityUID, ServerEntity *pServerEntity, ClusterMembership membership, ServiceStatus status, ServerServiceInformation* &pService );

		// Get next ring token
		Result GetNextRing( ServerServiceInformation* pService, ServerServiceInformation* &pNextService );

		// Get a service form the ring
		Result GetService( ServerServiceInformation* &pService );
	};
	

	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ShardedClusterServiceEntity class
	//


	class ShardedClusterServiceEntity : public ClusteredServiceEntity
	{
	private:

		// This will be to hash modulation number if it assigned.
		BRCLASS_ATTRIBUTE(uint,HashMod);

	public:

		// Constructor
		ShardedClusterServiceEntity( ClusterID clusterID, ClusterMembership initialMembership = ClusterMembership::StatusWatcher, ServerEntity* pServerEntity = nullptr );

		// Hash the key value
		virtual uint KeyHash( uint64_t key );

		// Get Service shard by key
		virtual Result GetShard( uint64_t key, ServerServiceInformation* &pService );

		////////////////////////////////////////////////////////////////////////////////////
	};
	


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	LoadbalanceClusterServiceEntity class
	//


	class LoadbalanceClusterServiceEntity : public ClusteredServiceEntity
	{
	public:

		typedef OrderedLinkedList<uint64_t> OrderedServiceList;

		Util::TimeStampTimer m_WorkloadCheckTimer;

	private:
		// Ordered service list
		OrderedServiceList	m_ServiceList;

		// round robine query
		ServerServiceInformation*	m_pCurrentQueryService;

	protected:

		// Game instance count in this entity
		SyncCounter			m_LocalWorkload;

	public:

		// Constructor
		LoadbalanceClusterServiceEntity( ClusterID clusterID, ClusterMembership initialMembership = ClusterMembership::StatusWatcher, ServerEntity* pServerEntity = nullptr );

		// Initialize entity to proceed new connection
		virtual Result InitializeEntity( EntityID newEntityID );


		// Override so that we can make ring linked list
		Result NewServerService( EntityUID entityUID, ServerEntity *pServerEntity, ClusterMembership membership, ServiceStatus status, ServerServiceInformation* &pService );

		// Get a service form the ring
		Result GetService( ServerServiceInformation* &pService );

		////////////////////////////////////////////////////////////////////////////////////
		virtual Result TickUpdate(TimerAction *pAction = nullptr);
	};


#include "ClusteredServiceEntity.inl"


}; // namespace Svr
}; // namespace SF




