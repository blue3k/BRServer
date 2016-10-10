////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Net Server Peer implementation 
//	
//
////////////////////////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////////////////////////
//
//	Server Peer network class
//


// connection manager

// connection manager
ConnectionManager& ServerPeerTCP::GetConnectionManager()
{
	return m_ConnectionManager;
}

// Get Active(non-free) connection
CounterType ServerPeerTCP::GetActiveConnectionCount()
{
	return GetConnectionManager().GetNumActiveConnection();
}

// Close all connection
Result ServerPeerTCP::CloseAllConnection()
{
	return GetConnectionManager().PendingCloseAllConnection();
}
