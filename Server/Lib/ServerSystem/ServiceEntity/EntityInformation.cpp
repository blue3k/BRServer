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
#include "Common/TypeDefs.h"
#include "EntityInformation.h"


BR_MEMORYPOOL_IMPLEMENT(Svr::UserEntityInformation);


namespace BR {
namespace Svr {


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
		:m_ClusterID(clusterID), m_ServerEntity(pServerEntity), m_ClusterMembership(membership)
		,m_ServiceStatus(ServiceStatus::Offline)
		,m_VotedCount(0)
		,m_Workload(0)
		,m_ServiceBase(nullptr)
	{
	}

	ServerServiceInformation::~ServerServiceInformation()
	{
	}
	
	// Get service information
	void ServerServiceInformation::GetServiceInformation( ServiceInformation & serviceInformation )
	{
		auto &connectionInfo = GetServerEntity()->GetConnection()->GetConnectionInfo();
		Assert(connectionInfo.RemoteClass != NetClass::Unknown);

		new(&serviceInformation) ServiceInformation( 
			GetEntityUID(), 
			GetClusterMembership(), 
			GetServiceStatus(), 
			connectionInfo.RemoteClass,
			connectionInfo.Remote, 
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
}; // namespace BR





