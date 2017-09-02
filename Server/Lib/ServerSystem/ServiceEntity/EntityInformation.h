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
#include "Memory/MemoryPool.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/ServerEntity.h"
//#include "ServerSystem/ServerService/ServerService.h"


namespace SF {
namespace Svr {

	class ServerServiceBase;
	class ServerService;

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
		char*	m_Name;

	public:
		EntityInformation();
		virtual ~EntityInformation();

		////////////////////////////////////////////////////////////////////////
		//
		//	Entity informations
		//

		// Entity UID
		inline EntityUID GetEntityUID() const;
		inline void SetEntityUID( EntityUID entityUID );

		inline ServerID GetServerID() const;

		// Entity Name
		inline const char* GetName() const;
		inline void SetName( const char* strName );

	};




	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity Generic Information class
	//

	class ServerServiceInformation : public EntityInformation
	{
	private:

		// Service Cluster ID
		ClusterID m_ClusterID;

		// Server entity which is the service is running
		ServerEntity* m_ServerEntity = nullptr;

		// Server service membership
		ClusterMembership m_ClusterMembership = ClusterMembership::Watcher;

		// Server service membership
		ServiceStatus m_ServiceStatus = ServiceStatus::Offline;

		// Server Up time, this will be synched to the server entity when the service object is created or synchronized
		//BRCLASS_ATTRIBUTE(uint64_t,ServerUpTime);

		// workload
		uint m_Workload = 0;

		// Voted count
		uint m_VotedCount = 0;

		// Service base cache
		ServerServiceBase* m_ServiceBase = nullptr;
		uint8_t	m_bufferForServiceBase[1024];


	public:
		ServerServiceInformation(ClusterID clusterID, ServerEntity* pServerEntity, ClusterMembership membership);
		~ServerServiceInformation();

		ClusterID GetClusterID() { return m_ClusterID; }

		ServerEntity* GetServerEntity() { return m_ServerEntity; }

		ClusterMembership GetClusterMembership() { return m_ClusterMembership; }

		ServiceStatus GetServiceStatus() { return m_ServiceStatus; }
		void SetServiceStatus(ServiceStatus value) { m_ServiceStatus = value; }

		uint GetWorkload() { return m_Workload; }
		void SetWorkload(uint value) { m_Workload = value; }

		uint GetVotedCount() { return m_VotedCount; }
		void SetVotedCount(uint value) { m_VotedCount = value; }

		ServerServiceBase* GetServiceBase() { return m_ServiceBase; }
		void SetServiceBase(ServerServiceBase* value) { m_ServiceBase = value; }


		// Get connection
		SF::Net::Connection* GetConnection() const;

		// Get service information
		void GetServiceInformation( ServiceInformation & serviceInformation );

		// Get Policy
		template< class PolicyType >
		FORCEINLINE PolicyType* GetPolicy() const
		{
			return GetConnection()->GetPolicy<PolicyType>();
		}

		// Setup route context from input transaction to the service entity
		RouteContext RouteContextFrom(TransactionID fromTrans)
		{
			if (m_ServerEntity == nullptr)
				return RouteContext();

			return RouteContext(EntityUID(GetMyServerID(), fromTrans.GetEntityID()), m_ServerEntity->GetEntityUID());
		}

		template< class ServiceType >
		FORCEINLINE ServiceType* GetService()
		{
			if( m_ServiceBase == nullptr )
			{
				m_ServiceBase = reinterpret_cast<ServerServiceBase*>(new(m_bufferForServiceBase) ServiceType(this));
			}

			ValidateServiceInstance(ServiceType::ID_SERVICEPOLICY);

			return (ServiceType*)m_ServiceBase;
		}

		void ValidateServiceInstance(UINT serviceID);

		// check whether this service is available or not
		bool IsServiceAvailable() const;
	};




	//////////////////////////////////////////////////////////////////////////
	//
	//	User Entity Information class
	//

	class UserEntityInformation : public EntityInformation, public MemoryPoolObject<UserEntityInformation>
	{
	private:
		// User entity information
		AccountID		m_AccountID;

		// Authorize ticket
		AuthTicket		m_AuthTicket;

	public:
		// Constructor
		UserEntityInformation();
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







