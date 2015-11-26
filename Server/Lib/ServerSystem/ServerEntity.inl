////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Remote Entity inline implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Remote Entity class
//





// return about initial connection or not
bool ServerEntity::IsInitialConnection()
{
	return m_bIsInitialConnect;
}

// Get Connection
inline Net::IConnection* ServerEntity::GetConnection()
{
	Net::IConnection* pConn = (Net::IConnection*)m_pConnLocal;
	auto result = (pConn == nullptr || pConn->GetConnectionState() == Net::IConnection::STATE_DISCONNECTED) ? (Net::IConnection*)m_pConnRemote : pConn;

	// if there is no active connection then return anyone no null
	if (result == nullptr)
		result = pConn;
	if (result == nullptr)
		result = (Net::IConnection*)m_pConnRemote;

	return result;
}

template< class PolicyType >
PolicyType* ServerEntity::GetPolicy()
{
	Net::IConnection* pConn = GetConnection();
	if (pConn == nullptr)
	{
		return nullptr;
	}

	return pConn->GetPolicy<PolicyType>();
}


// Get/Set Class Name
ServerEntity::ServerEntityClass ServerEntity::GetRemoteClass() const
{
	auto pConn = (const Net::IConnection*)m_pConnLocal;
	if (pConn)
		return pConn->GetConnectionInfo().RemoteClass;

	pConn = (const Net::IConnection*)m_pConnRemote;
	if (pConn)
		return pConn->GetConnectionInfo().RemoteClass;

	return NetClass::Unknown;
}

// Get public net address
inline const NetAddress& ServerEntity::GetPublicNetAddress() const
{
	return m_NetPublic;
}

inline void ServerEntity::SetPublicNetAddress( const NetAddress& netAddr )
{
	m_NetPublic = netAddr;
}

inline const NetAddress& ServerEntity::GetPrivateNetAddress() const
{
	return m_NetPrivate;
}

inline void ServerEntity::SetPrivateNetAddress(const NetAddress& netAddr)
{
	m_NetPrivate = netAddr;
}


// Get ServerID
inline ServerID ServerEntity::GetServerID() const
{
	return m_ServerID;
}

inline void ServerEntity::SetServerID( ServerID svrID )
{
	m_ServerID = svrID;
}




