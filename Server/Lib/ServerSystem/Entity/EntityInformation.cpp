////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : Entity information
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"
#include "SFTypedefs.h"
#include "EntityInformation.h"
#include "Protocol/ServerService.h"
#include "ServerEntity/ServerEntity.h"
#include "Server/BrServerUtil.h"
#include "json/json.h"

namespace SF {





	//////////////////////////////////////////////////////////////////////////////
	//
	// Public Event handler, User connection manager
	//

	EntityInformation::EntityInformation(const EntityUID& entityUID)
		: m_UID(entityUID)
	{
	}

	EntityInformation::EntityInformation(const EntityUID& entityUID, const VariableTable& customAttributes)
		: m_UID(entityUID)
		, m_CustomAttributes(GetEngineHeap(), customAttributes)
	{
	}

	EntityInformation::~EntityInformation()
	{
	}




	//////////////////////////////////////////////////////////////////////////////
	//
	// ServerServiceInformation
	//

	ServerServiceInformation::ServerServiceInformation(GameID gameID, ClusterID clusterID, EntityUID entityUID, const SharedPointerT<MessageEndpoint> targetEndpoint, const Json::Value& customAttributes)
		: EntityInformation(entityUID)
		, m_GameID(gameID)
		, m_ClusterID(clusterID)
		, m_ServiceStatus(ServiceStatus::Offline)
		, m_TargetEndpoint(targetEndpoint)
		, m_ServiceBase(nullptr)
	{
		char nodeName[256];
		StrUtil::Format(nodeName, "{0}_{1}", GetEntityUID().GetServerID(), GetEntityUID().GetEntityID());
		SetNodeName(nodeName);

		static_assert(sizeof(ServerService) <= sizeof(ServerServiceInformation::m_bufferForServiceBase), "Not enough buffer size for serverservice instance");

		if (customAttributes.isObject())
		{
			for (Json::Value::const_iterator itr = customAttributes.begin(); itr != customAttributes.end(); itr++)
			{
				switch (itr->type())
				{
					case Json::intValue:
						m_CustomAttributes.SetValue(itr.name().c_str(), itr->asInt());
						break;
					case Json::uintValue:
						m_CustomAttributes.SetValue(itr.name().c_str(), itr->asUInt());
						break;
					case Json::realValue:
						m_CustomAttributes.SetValue(itr.name().c_str(), itr->asDouble());
						break;
					case Json::stringValue:
						m_CustomAttributes.SetValue(itr.name().c_str(), String(itr->asCString()));
						break;
					case Json::booleanValue:
						m_CustomAttributes.SetValue(itr.name().c_str(), itr->asBool());
						break;
					case Json::nullValue:
					default:
						break;
				}
			}
		}
	}

	ServerServiceInformation::ServerServiceInformation(GameID gameID, ClusterID clusterID, EntityUID entityUID, const SharedPointerT<MessageEndpoint> targetEndpoint, const VariableTable& customAttributes)
		: EntityInformation(entityUID, customAttributes)
		, m_GameID(gameID)
		, m_ClusterID(clusterID)
		, m_ServiceStatus(ServiceStatus::Offline)
		, m_TargetEndpoint(targetEndpoint)
		, m_ServiceBase(nullptr)
	{
		char nodeName[256];
		StrUtil::Format(nodeName, "{0}_{1}", GetEntityUID().GetServerID(), GetEntityUID().GetEntityID());
		SetNodeName(nodeName);

		static_assert(sizeof(ServerService) <= sizeof(ServerServiceInformation::m_bufferForServiceBase), "Not enough buffer size for serverservice instance");
	}

	ServerServiceInformation::~ServerServiceInformation()
	{
	}
	
	//// Get service information
	//void ServerServiceInformation::GetServiceInformation( ServiceInformation& serviceInformation )
	//{
	//	new(&serviceInformation) ServiceInformation( 
	//		GetEntityUID(), 
	//		GetClusterMembership(), 
	//		GetServiceStatus(), 
	//		routeAddress,
	//		GetServerEntity()->GetServerUpTime(), 
	//		GetWorkload() );
	//}

	RouteContext ServerServiceInformation::RouteContextFrom(TransactionID fromTrans)
	{
		return RouteContext(EntityUID(GetMyServerID(), fromTrans.GetEntityID()), GetEntityUID());
	}



	//////////////////////////////////////////////////////////////////////////
	//
	//	User Entity Information class
	//

	
	UserEntityInformation::UserEntityInformation(EntityUID& entityUID)
		: EntityInformation(entityUID)
		, m_AccountID(0)
		, m_AuthTicket(0)
	{
	}

	UserEntityInformation::~UserEntityInformation()
	{
	}
	

} // namespace SF

