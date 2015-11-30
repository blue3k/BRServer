////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves.
// 
// Author : KyungKun Ko
//
// Description : Net connection definitions. 
//	
//
////////////////////////////////////////////////////////////////////////////////






////////////////////////////////////////////////////////////////////////////////
//
//	Network connection base class
//



// Set socket handle
void Connection::SetSocket( SOCKET socket )
{
	m_socket = socket;
}


inline CounterType Connection::GetConnectionEventCount()
{
	return m_EventQueue.GetEnqueCount();
}

//
//SyncCounter& Connection::GetPendingJobCounter()
//{
//	return m_PendingJobCount;
//}

// Get socket handle
SOCKET Connection::GetSocket() const
{
	return m_socket;
}


// Get remote address
const sockaddr_in6& Connection::GetRemoteSockAddr() const
{
	return m_sockAddrRemote;
}

// Get Recv queue
MsgQueue& Connection::GetRecvQueue()
{
	return m_RecvQueue;
}

USHORT Connection::NewSeqNone()
{
	return m_usSeqNone.fetch_add(1,std::memory_order_relaxed) + 1;
}


// Heartbit time
ULONG Connection::GetHeartbitTry()
{
	return m_ulHeartbitTry;
}

void Connection::SetHeartbitTry( ULONG ulHeartbitTry )
{
	m_ulHeartbitTry = ulHeartbitTry;
}

ULONG Connection::GetConnectingTimeOut()
{
	return m_ulConnectingTimeOut;
}

void Connection::SetConnectingTimeOut( ULONG ulConnectingTimeOut )
{
	m_ulConnectingTimeOut = ulConnectingTimeOut;
}


// Get zero recv count
ULONG Connection::GetZeroRecvCount()
{
	return m_ulZeroLengthRecvCount;
}

void Connection::IncZeroRecvCount()
{
	m_ulZeroLengthRecvCount++;
}

void Connection::ResetZeroRecvCount()
{
	m_ulZeroLengthRecvCount = 0;
}


// Get pending recv count
LONG Connection::GetPendingRecvCount()
{
	return m_lPendingRecvCount.load(std::memory_order_relaxed);
}

void Connection::IncPendingRecvCount()
{
	m_lPendingRecvCount.fetch_add(1, std::memory_order_relaxed);
}

void Connection::DecPendingRecvCount()
{
	if (!NetSystem::IsProactorSystem()) return;

	m_lPendingRecvCount.fetch_sub(1, std::memory_order_relaxed);
}

void Connection::ResetPendingRecvCount()
{
	m_lPendingRecvCount = 0;
}

inline LONG Connection::GetPendingSendCount()
{
	return m_PendingSend.load(std::memory_order_relaxed);
}



