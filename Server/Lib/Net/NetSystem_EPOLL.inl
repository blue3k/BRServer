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
void IOBUFFER_WRITE::InitForIO()
{
	memset(this, 0, sizeof(IOBUFFER_WRITE));
}

void IOBUFFER_WRITE::InitMsg(Message::MessageData *pMsg)
{
	pMsgs = pMsg;
	pSendBuff = nullptr;

	pMsg->GetLengthNDataPtr(RawSendSize, pRawSendBuffer);
}

void IOBUFFER_WRITE::InitBuff(UINT uiBuffSize, BYTE* pBuff)
{
	pMsgs = nullptr;
	pSendBuff = pBuff;

	RawSendSize = uiBuffSize;
	pRawSendBuffer = pBuff;
}

void IOBUFFER_WRITE::SetupSendUDP(Message::MessageData *pMsg)
{
	InitForIO();

	InitMsg(pMsg);

	Operation = IOBUFFER_OPERATION::OP_UDPWRITE;
}

void IOBUFFER_WRITE::SetupSendUDP(UINT uiBuffSize, BYTE* pBuff)
{
	InitForIO();

	InitBuff(uiBuffSize, pBuff);

	Operation = IOBUFFER_OPERATION::OP_UDPWRITE;
}

void IOBUFFER_WRITE::SetupSendPeer(Message::MessageData *pMsg)
{
	InitForIO();

	InitMsg(pMsg);

	Operation = IOBUFFER_OPERATION::OP_PEERUDPWRITE;
}

void IOBUFFER_WRITE::SetupSendPeer(UINT uiBuffSize, BYTE* pBuff)
{
	InitForIO();

	InitBuff(uiBuffSize, pBuff);

	Operation = IOBUFFER_OPERATION::OP_PEERUDPWRITE;
}

void IOBUFFER_WRITE::SetupSendTCP(Message::MessageData *pMsg)
{
	InitForIO();

	InitMsg(pMsg);

	Operation = IOBUFFER_OPERATION::OP_TCPWRITE;
}

void IOBUFFER_WRITE::SetupSendTCP(UINT uiBuffSize, BYTE* pBuff)
{
	InitForIO();

	InitBuff(uiBuffSize, pBuff);

	Operation = IOBUFFER_OPERATION::OP_TCPWRITE;
}



// Initialize for IO
void IOBUFFER_READ::InitForIO()
{
	memset(this, 0, sizeof(IOBUFFER_READ));
	iSockLen = sizeof(From);
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

void IOBUFFER_READ::SetupRecvPeer(uintptr_t iCID)
{
	InitRecv(iCID);
	Operation = IOBUFFER_OPERATION::OP_PEERUDPREAD;
}

void IOBUFFER_READ::SetupRecvTCP(uintptr_t iCID)
{
	InitRecv(iCID);
	Operation = IOBUFFER_OPERATION::OP_TCPREAD;
}

void IOBUFFER_READ::SetupRecvTCPPending(uintptr_t iCID)
{
	InitRecv(iCID);
	Operation = IOBUFFER_OPERATION::OP_TCPREADPENDING;
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





