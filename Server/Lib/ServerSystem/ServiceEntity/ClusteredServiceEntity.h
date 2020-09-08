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
#include "Memory/SFMemoryPool.h"
#include "Util/SFTimeUtil.h"
#include "Types/BrSvrTypes.h"
#include "ServiceEntity/ServiceEntity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "Container/SFHashTable.h"
#include "Container/SFIndexing.h"
#include "Container/SFStaticHashTable.h"

#include "Entity/EntityInformation.h"


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

	private:

		// Master instance entity UID
		EntityUID m_MasterUID;

		// Game id
		GameID m_GameID;

		// Cluster ID
		ClusterID m_ClusterID;
		StringCrc64 m_ClusterName;

		bool m_ActivelyConnectRemote = false;

		// Cluster type
		ClusterType m_ClusterType;

		// Cluster member ship of this instance
		ClusterMembership m_ClusterMembership;

		ServiceStatus m_ServiceStatus;

		uint m_Workload;


		// Cluster member instance for itself
		ServerServiceInformation* m_MyServiceInfo = nullptr;

		// Service UID map
		//ServiceEntityUIDMap			m_ServiceEntityUIDMap;

		ServerEntity* m_ServerEntity = nullptr;

		// Is voting going on?
		bool m_IsInVoting = false;

		bool m_Initialized = false;

	public:

		// Check connectivity of two service
		//static bool CheckConnectivity( NetClass netClass1, ClusterMembership membership1, NetClass netClass2, ClusterMembership membership2 );

		//bool CheckConnectivity( NetClass netClass2, ClusterMembership membership2 );

	public:

		ClusteredServiceEntity( ClusterType clusterType, GameID gameID, ClusterID clusterID, ClusterMembership initialMembership = ClusterMembership::Slave, ServerEntity* pServerEntity = nullptr );
		virtual ~ClusteredServiceEntity();

		bool GetActivelyConnectRemote() { return m_ActivelyConnectRemote; }
		void SetActivelyConnectRemote(bool value) { m_ActivelyConnectRemote = value; }

		EntityUID GetMasterUID() const { return m_MasterUID; }
		void SetMasterUID(EntityUID value) { m_MasterUID = value; }

		GameID GetGameID() const { return m_GameID; }

		ClusterID GetClusterID() const { return m_ClusterID; }
		//void SetClusterID(ClusterID value) { m_ClusterID = value; }

		StringCrc64 GetClusterName() const { return m_ClusterName; }
		//void SetClusterName(StringCrc64 value) { m_ClusterName = value; }

		ClusterType GetClusterType() const { return m_ClusterType; }
		void SetClusterType(ClusterType value) { m_ClusterType = value; }

		ClusterMembership GetClusterMembership() const { return m_ClusterMembership; }
		void SetClusterMembership(ClusterMembership value) { m_ClusterMembership = value; }

		ServiceStatus GetServiceStatus() const { return m_ServiceStatus; }
		// Change service status
		Result SetServiceStatus(ServiceStatus newStatus);

		uint GetWorkload() { return m_Workload; }
		// Change workload
		Result SetWorkload(uint workload);


		ServerServiceInformation* GetMyServiceInfo() { return m_MyServiceInfo; }
		void SetMyServiceInfo(ServerServiceInformation* value) { m_MyServiceInfo = value; }

		ServerEntity* GetServerEntity() { return m_ServerEntity; }
		void SetServerEntity(ServerEntity* value) { m_ServerEntity = value; }

		bool GetIsInVoting() { return m_IsInVoting; }

		bool GetInitialized() { return m_Initialized; }
		void SetInitialized(bool value) { m_Initialized = value; }

		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity 
		//

		// Initialize entity to proceed new connection
		virtual Result InitializeEntity( EntityID newEntityID ) override;

		virtual Result StartInitializeTransaction();

		// clear transaction
		virtual Result ClearEntity() override;


		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;
		virtual Result ProcessTransaction(TransactionPtr &pTrans) override;

		////////////////////////////////////////////////////////////////////////////
		////
		////	Voting information
		////


		//void StartVoting();
		//void EndVoting();


		// Assign master to given UID
		Result SetMaster( EntityUID entityUID );

		// Sync data to target 
		virtual Result SyncDataToTarget( EntityUID entityUID );

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

		ReplicaClusterServiceEntity(GameID gameID, ClusterID clusterID, ClusterMembership initialMembership = ClusterMembership::Slave, ServerEntity* pServerEntity = nullptr );


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

		FreeReplicaClusterServiceEntity(GameID gameID, ClusterID clusterID, ClusterMembership initialMembership = ClusterMembership::Slave, ServerEntity* pServerEntity = nullptr );

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
		//OrderedServiceList	m_ServiceList;

		//CriticalSection m_ListLock;

		// round robin query
		//ServerServiceInformation*	m_pCurrentQueryService;

	public:
		RingClusterServiceEntity(GameID gameID, ClusterID clusterID, ClusterMembership initialMembership = ClusterMembership::Slave, ServerEntity* pServerEntity = nullptr );

	};
	

	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ShardedClusterServiceEntity class
	//


	class ShardedClusterServiceEntity : public ClusteredServiceEntity
	{
	private:


	public:

		// Constructor
		ShardedClusterServiceEntity( GameID gameID, ClusterID clusterID, ClusterMembership initialMembership = ClusterMembership::Slave, ServerEntity* pServerEntity = nullptr );

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
		LoadbalanceClusterServiceEntity(GameID gameID, ClusterID clusterID, ClusterMembership initialMembership = ClusterMembership::Slave, ServerEntity* pServerEntity = nullptr );

		// Initialize entity to proceed new connection
		virtual Result InitializeEntity( EntityID newEntityID );


		////////////////////////////////////////////////////////////////////////////////////
		virtual Result TickUpdate(TimerAction *pAction = nullptr);
	};


#include "ClusteredServiceEntity.inl"


}; // namespace Svr
}; // namespace SF




