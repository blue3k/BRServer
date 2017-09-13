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




////////////////////////////////////////////////////////////////////////
//
//	Entity informations
//

// Entity ID
EntityUID EntityInformation::GetEntityUID() const
{
	return m_UID;
}

void EntityInformation::SetEntityUID( EntityUID entityUID )
{
	m_UID = entityUID;
}

ServerID EntityInformation::GetServerID() const
{
	return m_UID.GetServerID();
}



// Entity Name
const char* EntityInformation::GetName() const
{
	return m_Name;
}

void EntityInformation::SetName(IMemoryManager& memMgr, const char* strName )
{
	StrUtil::StringDup(memMgr, m_Name, strName );
}




inline const SharedPointerT<Net::Connection>& ServerServiceInformation::GetConnection() const
{
	const static SharedPointerT<Net::Connection> Dummy;
	if( m_ServerEntity == nullptr ) return Dummy;

	return m_ServerEntity->GetConnection();
}

// check whether this service is available or not
inline bool ServerServiceInformation::IsServiceAvailable() const
{
	Assert(m_ServerEntity);
	auto& pConn = GetConnection();
	return pConn != nullptr && pConn->GetConnectionState() == Net::ConnectionState::CONNECTED;
}



// AccountID
AccountID UserEntityInformation::GetAccountID() const
{
	return m_AccountID;
}

void UserEntityInformation::SetAccountID( AccountID ticket )
{
	m_AccountID = ticket;
}
		
// Authorize ticket
AuthTicket UserEntityInformation::GetAuthTicket() const
{
	return m_AuthTicket;
}

void UserEntityInformation::SetAuthTicket( AuthTicket ticket )
{
	m_AuthTicket = ticket;
}
