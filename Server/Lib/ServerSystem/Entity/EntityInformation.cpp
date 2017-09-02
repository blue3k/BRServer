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

	EntityInformation::EntityInformation()
		:m_UID(0),
		m_Name(nullptr)
	{
	}

	EntityInformation::~EntityInformation()
	{
		Util::SafeDelete( m_Name );
	}




	//////////////////////////////////////////////////////////////////////////////
	//
	// ServerServiceInformation
	//

	ServerServiceInformation::ServerServiceInformation( ClusterID clusterID, ServerEntity* pServerEntity, ClusterMembership membership )
		: m_ClusterID(clusterID)
		, m_ServerEntity(pServerEntity)
		, m_ClusterMembership(membership)
		, m_ServiceStatus(ServiceStatus::Offline)
		, m_Workload(0)
		, m_VotedCount(0)
		, m_ServiceBase(nullptr)
	{
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

	
	UserEntityInformation::UserEntityInformation()
		:m_AccountID(0),
		m_AuthTicket(0)
	{
	}

	UserEntityInformation::~UserEntityInformation()
	{
	}
	


}; // namespace Svr
}; // namespace SF






