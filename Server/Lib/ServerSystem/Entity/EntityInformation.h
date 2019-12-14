////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Entity information
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Memory/SFMemoryPool.h"
//#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
//#include "ServerEntity/ServerEntity.h"
//#include "ServerService/ServerService.h"


namespace SF {

	namespace Net
	{
		class Connection;
	}


namespace Svr {

	class ServerServiceBase;
	class ServerService;
	class ServerEntity;


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity Information class
	//

	class EntityInformation
	{
	private:
		// Entity UID
		EntityUID	m_UID;

		// Entity name
		String	m_NodeName;
		FixedString m_NodeNameCrc;

	public:
		EntityInformation(const EntityUID& entityUID);
		virtual ~EntityInformation();

		////////////////////////////////////////////////////////////////////////
		//
		//	Entity informations
		//

		// Entity UID
		inline EntityUID GetEntityUID() const;

		inline ServerID GetServerID() const;

		// Entity Name
		const String& GetNodeName() const { return m_NodeName; }
		FixedString GetNodeNameCrc() const { return m_NodeNameCrc; }
		void SetNodeName(const char* strName) { m_NodeName = strName; m_NodeNameCrc = strName; }

	};




	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity Generic Information class
	//

	class ServerServiceInformation : public EntityInformation
	{
	private:

		GameID m_GameID;

		// Service Cluster ID
		ClusterID m_ClusterID;

		// Server entity which is the service is running
		ServerEntity* m_ServerEntity = nullptr;

		// Server service membership
		ClusterMembership m_ClusterMembership = ClusterMembership::Slave;

		// Server service membership
		ServiceStatus m_ServiceStatus = ServiceStatus::Offline;

		// Server Up time, this will be synced to the server entity when the service object is created or synchronized
		//uint64_t, m_ServerUpTime;

		// workload
		uint m_Workload = 0;

		// Voted count
		uint m_VotedCount = 0;

		// Service base cache
		ServerServiceBase* m_ServiceBase = nullptr;
		uint8_t	m_bufferForServiceBase[1024];


	public:
		ServerServiceInformation(GameID gameID, ClusterID clusterID, EntityUID entityUID, ServerEntity* pServerEntity, ClusterMembership membership);
		~ServerServiceInformation();


		ClusterID GetClusterID() const { return m_ClusterID; }
		GameID GetGameID() const { return m_GameID; }

		ServerEntity* GetServerEntity() const { return m_ServerEntity; }

		ClusterMembership GetClusterMembership() const { return m_ClusterMembership; }
		void SetClusterMembership(ClusterMembership value) { m_ClusterMembership = value; }

		ServiceStatus GetServiceStatus() const { return m_ServiceStatus; }
		void SetServiceStatus(ServiceStatus value) { m_ServiceStatus = value; }

		uint GetWorkload() const { return m_Workload; }
		void SetWorkload(uint value) { m_Workload = value; }

		uint GetVotedCount() const { return m_VotedCount; }
		void SetVotedCount(uint value) { m_VotedCount = value; }

		ServerServiceBase* GetServiceBase() const { return m_ServiceBase; }
		void SetServiceBase(ServerServiceBase* value) { m_ServiceBase = value; }


		// Get connection
		const SharedPointerAtomicT<Net::Connection>& GetConnection() const;

		// Get service information
		void GetServiceInformation( ServiceInformation & serviceInformation );


		// Setup route context from input transaction to the service entity
		RouteContext RouteContextFrom(TransactionID fromTrans);


		template< class ServiceType >
		SF_FORCEINLINE ServiceType* GetService()
		{
			if( m_ServiceBase == nullptr )
			{
				m_ServiceBase = dynamic_cast<ServerServiceBase*>(new(m_bufferForServiceBase) ServiceType(this));
			}

			return (ServiceType*)m_ServiceBase;
		}

		// check whether this service is available or not
		bool IsServiceAvailable() const;
	};




	//////////////////////////////////////////////////////////////////////////
	//
	//	User Entity Information class
	//

	class UserEntityInformation : public EntityInformation
	{
	private:
		// User entity information
		AccountID		m_AccountID;

		// Authorize ticket
		AuthTicket		m_AuthTicket;

	public:
		// Constructor
		UserEntityInformation(EntityUID& entityUID);
		~UserEntityInformation();

		// AccountID
		inline AccountID GetAccountID() const;
		inline void SetAccountID( AccountID ticket );

		// Authorize ticket
		inline AuthTicket GetAuthTicket() const;
		inline void SetAuthTicket( AuthTicket ticket );

	};




#include "EntityInformation.inl"

} // namespace Svr
} // namespace SF







