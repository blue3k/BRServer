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

#include "Common/TypeDefs.h"
#include "Common/MemoryPool.h"
#include "Common/ClassUtil.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/ServerService/ServerService.h"


namespace BR {
namespace Svr {

	class ServerServiceBase;


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
		BYTE	m_bufferForServiceBase[sizeof(ServerService)];


	public:
		ServerServiceInformation(ClusterID clusterID, ServerEntity* pServerEntity, ClusterMembership membership);
		~ServerServiceInformation();

		// Get connection
		FORCEINLINE Net::IConnection* GetConnection() const;

		// Get service information
		void GetServiceInformation( ServiceInformation & serviceInformation );

		// Get Policy
		template< class PolicyType >
		FORCEINLINE PolicyType* GetPolicy() const
		{
			return GetConnection()->GetPolicy<PolicyType>();
		}

		template< class ServiceType >
		FORCEINLINE ServiceType* GetService()
		{
			if( m_ServiceBase == nullptr )
			{
				m_ServiceBase = reinterpret_cast<ServerServiceBase*>(new(m_bufferForServiceBase) ServiceType(this));
				static_assert(sizeof(m_bufferForServiceBase) == sizeof(ServiceType), "Invalid buffer size for service base");
			}

			AssertRel( m_ServiceBase->GetPolicyID() == ServiceType::ID_SERVICEPOLICY );

			return (ServiceType*)m_ServiceBase;
		}

		// check whether this service is available or not
		FORCEINLINE bool IsServiceAvailable() const;
	};


	////////////////////////////////////////////////////////////////////////////
	////
	////	Entity Generic Information class
	////

	//class ServerInformation : public EntityInformation, public MemoryPoolObject<ServerInformation>
	//{
	//public:


	//	// server service list
	//	typedef std::vector<ServerServiceInformation*> ServerServiceList;

	//private:

	//	// Cluster ID
	//	BRCLASS_ATTRIBUTE_READONLY(ClusterID,ClusterID);

	//	// Service list for this server
	//	ServerServiceList	m_ServerServices;

	//public:
	//	ServerInformation();
	//	~ServerInformation();

	//	// Add service
	//	HRESULT AddService( ServerServiceInformation* pService );
	//};
	//

	////////////////////////////////////////////////////////////////////////////
	////
	////	Entity Generic Information class
	////

	//class ServerClusterInformation : public MemoryPoolObject<ServerClusterInformation>
	//{
	//public:
	//	// server service list
	//	typedef std::vector<ServerInformation*> ServerList;

	//private:
	//	// Cluster ID
	//	BRCLASS_ATTRIBUTE_READONLY(ClusterID,ClusterID);

	//	// Service list for this server
	//	ServerList	m_Servers;

	//public:
	//	ServerClusterInformation();
	//	~ServerClusterInformation();

	//	// Add server
	//	HRESULT AddServer( ServerInformation* pServer );
	//};


	////////////////////////////////////////////////////////////////////////////
	////
	////	Entity Generic Information class
	////

	//class ServerServiceClusterInformation : public MemoryPoolObject<ServerServiceClusterInformation>
	//{
	//public:
	//	// server service list
	//	typedef std::vector<ServerServiceInformation*> ServiceList;

	//private:
	//	// Cluster ID
	//	BRCLASS_ATTRIBUTE_READONLY(ClusterID,ClusterID);

	//	// Service list for this server
	//	ServiceList	m_Services;

	//public:
	//	ServerServiceClusterInformation();
	//	~ServerServiceClusterInformation();

	//	// Add service
	//	HRESULT AddService( ServerServiceInformation* pService );
	//};



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







