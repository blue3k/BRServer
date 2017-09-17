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


#include "stdafx.h"
#include "SFTypedefs.h"
#include "EntityInformation.h"
#include "Protocol/ServerService/ServerService.h"


SF_MEMORYPOOL_IMPLEMENT(SF::Svr::UserEntityInformation);


namespace SF {
namespace Svr {

	static_assert(sizeof(ServerService) <= sizeof(ServerServiceInformation::m_bufferForServiceBase), "Not enough buffer size for serverservice instance");




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

	ServerServiceInformation::ServerServiceInformation(GameID gameID, ClusterID clusterID, EntityUID entityUID, ServerEntity* pServerEntity, ClusterMembership membership )
		: EntityInformation(entityUID)
		, m_GameID(gameID)
		, m_ClusterID(clusterID)
		, m_ServerEntity(pServerEntity)
		, m_ClusterMembership(membership)
		, m_ServiceStatus(ServiceStatus::Offline)
		, m_Workload(0)
		, m_VotedCount(0)
		, m_ServiceBase(nullptr)
	{
		char nodeName[256];
		StrUtil::Format(nodeName, "{0}_{1}", GetEntityUID().GetServerID(), GetEntityUID().GetEntityID());
		SetNodeName(nodeName);
	}

	ServerServiceInformation::~ServerServiceInformation()
	{
	}
	
	// Get service information
	void ServerServiceInformation::GetServiceInformation( ServiceInformation & serviceInformation )
	{
		auto &remoteInfo = GetServerEntity()->GetConnection()->GetRemoteInfo();
		Assert(remoteInfo.PeerClass != NetClass::Unknown);

		new(&serviceInformation) ServiceInformation( 
			GetEntityUID(), 
			GetClusterMembership(), 
			GetServiceStatus(), 
			remoteInfo.PeerClass,
			remoteInfo.PeerAddress,
			GetServerEntity()->GetServerUpTime(), 
			GetWorkload() );
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
	


}; // namespace Svr
}; // namespace SF






