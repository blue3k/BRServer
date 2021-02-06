////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Remote Player class
//	
////////////////////////////////////////////////////////////////////////////////


// Get route context
inline RouteContext PlayerEntityInformation::GetRouteContext( EntityUID uidFrom )
{
	return RouteContext( uidFrom, m_PlayerEntityUID );
}




