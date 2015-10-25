////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Net server base type inline
//	
//
////////////////////////////////////////////////////////////////////////////////


// Set local address
void ServerNet::SetLocalAddress( const NetAddress& localAddr )
{
	m_LocalAddr = localAddr;

	Addr2SockAddr( m_LocalAddr, m_LocalSockAddr );
}


// Get Socket address
const sockaddr_in6& ServerNet::GetSocketAddr()
{
	return m_LocalSockAddr;
}

// Get local address
const NetAddress& ServerNet::GetLocalAddress()
{
	return m_LocalAddr;
}


// Set socket
void ServerNet::SetSocket( SOCKET sock )
{
	m_socket = sock;
}

// Get Socket
SOCKET ServerNet::GetSocket()
{
	return m_socket;
}
