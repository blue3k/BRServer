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

ServerID EntityInformation::GetServerID() const
{
	return m_UID.GetServerID();
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

