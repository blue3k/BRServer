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

void EntityInformation::SetName( const char* strName )
{
	StrUtil::StringDup( m_Name, strName );
}




Net::IConnection* ServerServiceInformation::GetConnection() const
{
	if( m_ServerEntity == nullptr ) return nullptr;

	return m_ServerEntity->GetConnection();
}

// check whether this service is available or not
bool ServerServiceInformation::IsServiceAvailable() const
{
	Assert(m_ServerEntity);
	return m_ServerEntity->GetConnection() && m_ServerEntity->GetConnection()->GetConnectionState() == Net::IConnection::STATE_CONNECTED;
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



///////////////////////////////////////////////////////////////////
//
//	Service base
//

// This is not a good place to place this implementatin but it's the only way to make it work
EntityUID ServerServiceBase::GetServiceEntityUID()
{
	return m_ServerServiceInformation->GetEntityUID();
}

