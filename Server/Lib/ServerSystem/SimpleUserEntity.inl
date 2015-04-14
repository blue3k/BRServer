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






// Get Connection
Net::Connection* SimpleUserEntity::GetConnection()
{
	return (Net::Connection*)m_pConnection;
}





AccountID SimpleUserEntity::GetAccountID() const
{
	return m_AccountID;
}


// Get Auth ticket
AuthTicket SimpleUserEntity::GetAuthTicket() const
{
	return m_AuthTicket;
}

// Set Auth ticket
void SimpleUserEntity::SetAuthTicket( AuthTicket authTicket )
{
	m_AuthTicket = authTicket;
}




