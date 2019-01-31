////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Login server inline
//	
////////////////////////////////////////////////////////////////////////////////




// Get net public
BR::Net::ServerMUDP* LoginServer::GetNetPublic()
{
	return m_pNetPublic;
}


// Query Game server instance
LoginServer* GetMyServer()
{
	return BR_DYNAMIC_CAST(LoginServer*,Svr::BrServer::GetInstance());
}




