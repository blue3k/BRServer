////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Remote Player class
//	
////////////////////////////////////////////////////////////////////////////////


// Check server status
inline bool PlayerEntityInformation::CheckServerStatus()
{
	if( m_ServerEntity == nullptr )
		return false;

	// Server rebooted since this connection is made. We need to clean up because the player will not in the same server
	if( m_LatestServerUpTime != m_ServerEntity->GetServerUpTime() )
	{
		m_ServerEntity = nullptr;
		return false;
	}

	return true;
}



// Get route context
inline RouteContext PlayerEntityInformation::GetRouteContext( EntityUID uidFrom )
{
	return RouteContext( uidFrom, m_PlayerEntityUID );
}




