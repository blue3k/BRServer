////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves.
// 
// Author : KyungKun Ko
//
// Description : Net Client
//	
//
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
//	Client base class
//

// Add network event to queue
HRESULT Client::EnqueueNetEvent( INet::Event& evt )
{
	return m_NetEventQueue.Enqueue( evt );
}




////////////////////////////////////////////////////////////////////////////////
//
//	Client TCP class
//

// Get connection manager
ConnectionManager& ClientTCP::GetConnectionManager()
{
	return m_ConnectionManager;
}


////////////////////////////////////////////////////////////////////////////////
//
//	Client UDP class
//

// Get connection manager
ConnectionManager& ClientUDP::GetConnectionManager()
{
	return m_ConnectionManager;
}
