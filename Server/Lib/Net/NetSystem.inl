////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Network internal inline implementation
//	
//
////////////////////////////////////////////////////////////////////////////////








////////////////////////////////////////////////////////////////////////////////
//
//	Overlapped I/O structures
//



// Initialize for IO
void OVERLAPPED_BUFFER_WRITE::InitForIO()
{
	HANDLE hEventTemp = hEvent;
	memset( this, 0, sizeof(OVERLAPPED_BUFFER_WRITE) );
	hEvent = hEventTemp;
}

void OVERLAPPED_BUFFER_WRITE::InitMsg( Message::MessageData *pMsg )
{
	pMsgs = pMsg;
	pSendBuff = nullptr;
	wsaBuff.len = pMsg->GetMessageSize();
	wsaBuff.buf = (char*)pMsg->GetMessageBuff();
}

void OVERLAPPED_BUFFER_WRITE::InitBuff( UINT uiBuffSize, BYTE* pBuff )
{
	pMsgs = nullptr;
	wsaBuff.len = uiBuffSize;
	wsaBuff.buf = (char*)pBuff;
	pSendBuff = pBuff;
}

void OVERLAPPED_BUFFER_WRITE::SetupSendUDP( Message::MessageData *pMsg )
{
	InitForIO();

	InitMsg( pMsg );

	Operation = OP_UDPWRITE;
}

void OVERLAPPED_BUFFER_WRITE::SetupSendUDP( UINT uiBuffSize, BYTE* pBuff )
{
	InitForIO();

	InitBuff( uiBuffSize, pBuff );

	Operation = OP_UDPWRITE;
}

void OVERLAPPED_BUFFER_WRITE::SetupSendPeer( Message::MessageData *pMsg )
{
	InitForIO();

	InitMsg( pMsg );

	Operation = OP_PEERUDPWRITE;
}

void OVERLAPPED_BUFFER_WRITE::SetupSendPeer( UINT uiBuffSize, BYTE* pBuff )
{
	InitForIO();

	InitBuff( uiBuffSize, pBuff );

	Operation = OP_PEERUDPWRITE;
}

void OVERLAPPED_BUFFER_WRITE::SetupSendTCP( Message::MessageData *pMsg )
{
	InitForIO();

	InitMsg( pMsg );

	Operation = OP_TCPWRITE;
}

void OVERLAPPED_BUFFER_WRITE::SetupSendTCP( UINT uiBuffSize, BYTE* pBuff )
{
	InitForIO();

	InitBuff( uiBuffSize, pBuff );

	Operation = OP_TCPWRITE;
}



// Initialize for IO
void tag_OVERLAPPED_BUFFER_READ::InitForIO()
{
	HANDLE hEventTemp = hEvent;
	memset( this, 0, sizeof(OVERLAPPED_BUFFER_WRITE) );
	hEvent = hEventTemp;

	iSockLen = sizeof(sockaddr_in6);
}

void tag_OVERLAPPED_BUFFER_READ::InitRecv( uintptr_t iCID )
{
	InitForIO();
	wsaBuff.len = sizeof(buffer);
	wsaBuff.buf = buffer;
#ifdef DEBUG
	memset(buffer,0xCE,sizeof(buffer));
#endif
	CID = iCID;
}

// Setup recving mode
void tag_OVERLAPPED_BUFFER_READ::SetupRecvUDP( uintptr_t iCID )
{
	InitRecv( iCID );
	Operation = OP_UDPREAD;
}

void tag_OVERLAPPED_BUFFER_READ::SetupRecvPeer( uintptr_t iCID )
{
	InitRecv( iCID );
	Operation = OP_PEERUDPREAD;
}

void tag_OVERLAPPED_BUFFER_READ::SetupRecvTCP( uintptr_t iCID )
{
	InitRecv( iCID );
	Operation = OP_TCPREAD;
}

void tag_OVERLAPPED_BUFFER_READ::SetupRecvTCPPending( uintptr_t iCID )
{
	InitRecv( iCID );
	Operation = OP_TCPREADPENDING;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Accept overlapped
//


// Setup accept
void tag_OVERLAPPED_BUFFER_ACCEPT::SetupAccept( SOCKET sock )
{
	Operation = OVERLAPPED_BUFFER::OP_TCPACCEPT;
	sockAccept = sock;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Network IOCP thread worker
//


IOCPSystem::IOCPWorker::IOCPWorker()
{
}

IOCPSystem::IOCPWorker::~IOCPWorker()
{
}


void IOCPSystem::IOCPWorker::SetIOCPHandle( HANDLE hIOCP )
{
	m_hIOCP = hIOCP;
}




IOCPSystem::IOCPSystem& IOCPSystem::IOCPSystem::GetInstance()
{
	return stm_Instance;
}

// Query IOCP handle
HANDLE IOCPSystem::IOCPSystem::GetIOCP()
{
	return m_hIOCP;
}


