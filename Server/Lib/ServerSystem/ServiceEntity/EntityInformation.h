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
#include "Common/ClassUtil.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/ServerEntity.h"
//#include "ServerSystem/ServerService/ServerService.h"


namespace BR {
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
		BRCLASS_ATTRIBUTE_READONLY(ClusterID,ClusterID);

		// Server entity which is the service is running
		BRCLASS_ATTRIBUTE_READONLY_PTR(ServerEntity*,ServerEntity);

		// Server service membership
		BRCLASS_ATTRIBUTE(ClusterMembership,ClusterMembership);

		// Server service membership
		BRCLASS_ATTRIBUTE(ServiceStatus,ServiceStatus);

		// Server Up time, this will be synched to the server entity when the service object is created or synchronized
		//BRCLASS_ATTRIBUTE(ULONGLONG,ServerUpTime);

		// workload
		BRCLASS_ATTRIBUTE(UINT,Workload);

		// Voted count
		BRCLASS_ATTRIBUTE(UINT,VotedCount);

		// Service base cache
		BRCLASS_ATTRIBUTE_READONLY_PTR(ServerServiceBase*,ServiceBase);
		uint8_t	m_bufferForServiceBase[1024];


	public:
		ServerServiceInformation(ClusterID clusterID, ServerEntity* pServerEntity, ClusterMembership membership);
		~ServerServiceInformation();

		// Get connection
		Net::IConnection* GetConnection() const;

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

		// Autorize ticket
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
} // namespace BR







