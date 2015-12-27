////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
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
void IOBUFFER_WRITE::InitForIO(SOCKET sockWrite)
{
	memset(this, 0, sizeof(IOBUFFER_WRITE));
	SockWrite = sockWrite;
}

void IOBUFFER_WRITE::InitMsg(Message::MessageData *pMsg)
{
	pMsgs = pMsg;
	pSendBuff = nullptr;

	RawSendSize = pMsg->GetMessageSize();
	pRawSendBuffer = pMsg->GetMessageBuff();
}

void IOBUFFER_WRITE::InitBuff(UINT uiBuffSize, BYTE* pBuff)
{
	pMsgs = nullptr;
	pSendBuff = pBuff;

	RawSendSize = uiBuffSize;
	pRawSendBuffer = pBuff;
}

void IOBUFFER_WRITE::SetupSendUDP(SOCKET sockWrite, const sockaddr_storage& to, Message::MessageData *pMsg)
{
	InitForIO(sockWrite);

	InitMsg(pMsg);

	NetAddr.To = to;

	Operation = IOBUFFER_OPERATION::OP_UDPWRITE;
}

void IOBUFFER_WRITE::SetupSendUDP(SOCKET sockWrite, const sockaddr_storage& to, UINT uiBuffSize, BYTE* pBuff)
{
	InitForIO(sockWrite);

	InitBuff(uiBuffSize, pBuff);

	NetAddr.To = to;

	Operation = IOBUFFER_OPERATION::OP_UDPWRITE;
}

void IOBUFFER_WRITE::SetupSendTCP(Message::MessageData *pMsg)
{
	InitForIO(0);

	InitMsg(pMsg);

	Operation = IOBUFFER_OPERATION::OP_TCPWRITE;
}

void IOBUFFER_WRITE::SetupSendTCP(UINT uiBuffSize, BYTE* pBuff)
{
	InitForIO(0);

	InitBuff(uiBuffSize, pBuff);

	Operation = IOBUFFER_OPERATION::OP_TCPWRITE;
}



// Initialize for IO
void IOBUFFER_READ::InitForIO()
{
	memset(this, 0, sizeof(IOBUFFER_READ));
	iSockLen = sizeof(NetAddr.From);
}

void IOBUFFER_READ::InitRecv(uintptr_t iCID)
{
	InitForIO();
#ifdef DEBUG
	memset(buffer, 0xCE, sizeof(buffer));
#endif
	CID = iCID;
}

// Setup recving mode
void IOBUFFER_READ::SetupRecvUDP(uintptr_t iCID)
{
	InitRecv(iCID);
	Operation = IOBUFFER_OPERATION::OP_UDPREAD;
}

void IOBUFFER_READ::SetupRecvTCP(uintptr_t iCID)
{
	InitRecv(iCID);
	Operation = IOBUFFER_OPERATION::OP_TCPREAD;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Accept overlapped
//


// Setup accept
void IOBUFFER_ACCEPT::SetupAccept(SOCKET sock)
{
	Operation = IOBUFFER_OPERATION::OP_TCPACCEPT;
	sockAccept = sock;
}





