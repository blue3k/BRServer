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


#include "ServerSystemPCH.h"
#include "SFTypedefs.h"
#include "EntityInformation.h"
#include "Protocol/ServerService/ServerService.h"
#include "ServerEntity/ServerEntity.h"
#include "Server/BrServerUtil.h"


namespace SF {





	//////////////////////////////////////////////////////////////////////////////
	//
	// Public Event handler, User connection manager
	//

	EntityInformation::EntityInformation(const EntityUID& entityUID)
		: m_UID(entityUID)
	{
	}

	EntityInformation::~EntityInformation()
	{
	}




	//////////////////////////////////////////////////////////////////////////////
	//
	// ServerServiceInformation
	//

	ServerServiceInformation::ServerServiceInformation(GameID gameID, ClusterID clusterID, EntityUID entityUID, const SharedPointerT<MessageEndpoint> targetEndpoint, ClusterMembership membership )
		: EntityInformation(entityUID)
		, m_GameID(gameID)
		, m_ClusterID(clusterID)
		, m_ClusterMembership(membership)
		, m_ServiceStatus(ServiceStatus::Offline)
		, m_TargetEndpoint(targetEndpoint)
		, m_Workload(0)
		, m_VotedCount(0)
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

