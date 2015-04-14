////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Net Server implementation 
//	
//
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
//	Server base class
//






////////////////////////////////////////////////////////////////////////////////
//
//	Server base class
//



// Get pending accept count
CounterType ServerTCP::GetPendingAccept()
{
	return m_pAcceptBuffer ? m_pAcceptBuffer->GetUsedBufferCount() : 0;
}




// connection manager
ConnectionManager& ServerPublicTCP::GetConnectionManager()
{
	return m_ConnectionManager;
}

// Get Active(non-free) connection
CounterType ServerPublicTCP::GetActiveConnectionCount()
{
	return GetConnectionManager().GetNumActiveConnection();
}

// Close all connection
HRESULT ServerPublicTCP::CloseAllConnection()
{
	return GetConnectionManager().PendingCloseAllConnection();
}
