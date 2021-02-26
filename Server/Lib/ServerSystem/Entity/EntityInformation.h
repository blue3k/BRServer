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
#include "MemoryManager/SFMemoryPool.h"
#include "Server/BrServerUtil.h"
#include "Variable/SFVariableTable.h"

namespace Json {
	class Value;
}

namespace SF {

	namespace Net
	{
		class Connection;
	}

	class MessageEndpoint;


	class ServerServiceBase;
	class ServerService;
	class ServerEntity;


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity Information class
	//

	class EntityInformation : public SharedObject
	{
	private:
		// Entity UID
		EntityUID	m_UID;

		// Entity name
		String	m_NodeName;
		StringCrc64 m_NodeNameCrc;

	protected:
		VariableTable m_CustomAttributes;

	public:
		EntityInformation(const EntityUID& entityUID);
		EntityInformation(const EntityUID& entityUID, const VariableTable& customAttributes);
		virtual ~EntityInformation();

		////////////////////////////////////////////////////////////////////////
		//
		//	Entity informations
		//

		// Entity UID
		inline EntityUID GetEntityUID() const;

		inline ServerID GetServerID() const;

		const VariableTable& GetCustomAttributes() const { return m_CustomAttributes; }


		// Entity Name
		const String& GetNodeName() const { return m_NodeName; }
		StringCrc64 GetNodeNameCrc() const { return m_NodeNameCrc; }
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

		// Server service membership
		ServiceStatus m_ServiceStatus = ServiceStatus::Offline;

		// Message endpoint
		SharedPointerT<MessageEndpoint> m_TargetEndpoint;


		// Service base cache
		ServerServiceBase* m_ServiceBase = nullptr;
		uint8_t	m_bufferForServiceBase[1024];


	public:
		ServerServiceInformation(GameID gameID, ClusterID clusterID, EntityUID entityUID, const SharedPointerT<MessageEndpoint> targetEndpoint, const Json::Value& customAttributes);
		ServerServiceInformation(GameID gameID, ClusterID clusterID, EntityUID entityUID, const SharedPointerT<MessageEndpoint> targetEndpoint, const VariableTable& customAttributes);
		~ServerServiceInformation();


		ClusterID GetClusterID() const { return m_ClusterID; }
		GameID GetGameID() const { return m_GameID; }


		const SharedPointerT<MessageEndpoint>& GetTargetEndpoint() const { return m_TargetEndpoint; }

		ServiceStatus GetServiceStatus() const { return m_ServiceStatus; }
		void SetServiceStatus(ServiceStatus value) { m_ServiceStatus = value; }

		ServerServiceBase* GetServiceBase() const { return m_ServiceBase; }
		void SetServiceBase(ServerServiceBase* value) { m_ServiceBase = value; }

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

} // namespace SF







