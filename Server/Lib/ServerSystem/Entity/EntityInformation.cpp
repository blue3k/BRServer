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



namespace SF {
namespace Svr {





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

		static_assert(sizeof(ServerService) <= sizeof(ServerServiceInformation::m_bufferForServiceBase), "Not enough buffer size for serverservice instance");
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

	RouteContext ServerServiceInformation::RouteContextFrom(TransactionID fromTrans)
	{
		if (m_ServerEntity == nullptr)
			return RouteContext();

		return RouteContext(EntityUID(GetMyServerID(), fromTrans.GetEntityID()), m_ServerEntity->GetEntityUID());
	}

	const SharedPointerAtomicT<Net::Connection>& ServerServiceInformation::GetConnection() const
	{
		const static SharedPointerAtomicT<Net::Connection> Dummy;
		if (m_ServerEntity == nullptr) return Dummy;

		return m_ServerEntity->GetConnection();
	}

	// check whether this service is available or not
	bool ServerServiceInformation::IsServiceAvailable() const
	{
		Assert(m_ServerEntity);
		auto& pConn = GetConnection();
		return pConn != nullptr && pConn->GetConnectionState() == Net::ConnectionState::CONNECTED;
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






