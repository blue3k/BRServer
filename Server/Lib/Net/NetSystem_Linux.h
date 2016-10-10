////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : KyungKun Ko
//
// Description : Network classes definitions
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Net/NetDef.h"
#include "Net/NetConst.h"
#include "Common/StrUtil.h"
#include "Common/HashTable.h"
#include "Common/MemoryPool.h"
#include "Common/Thread.h"
#include "Common/BrBaseTypes.h"
#include "Net/NetUtil.h"

#if LINUX



namespace BR {
namespace Net {

	class Connection;
	enum class IOBUFFER_OPERATION : UINT8;

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Overlapped I/O structures
	//

	struct IOBUFFER
	{
		IOBUFFER_OPERATION Operation;

		// Clear Buffer
		void ClearBuffer();

	};


	// UDP/TCP read/write overlapped base
	struct IOBUFFER_RWBASE : public IOBUFFER
	{
		// transferred buffer size
		DWORD TransferredSize;

		union {
			// UDP Read from
			struct sockaddr_storage From;
			struct sockaddr_storage To;
		} NetAddr;


		// Constructor
		IOBUFFER_RWBASE();
	};


	// UDP/TCP write overlapped
	struct IOBUFFER_WRITE : public IOBUFFER_RWBASE, public MemoryPoolObject<IOBUFFER_WRITE>
	{
		SOCKET SockWrite;

		// Sending raw buffer, always has send buffer data pointer and size
		UINT RawSendSize;
		BYTE* pRawSendBuffer;

		// Message pointer to send, when send message data
		Message::MessageData *pMsgs;

		// Message buffer pointer to send, when send buffer data
		BYTE *pSendBuff;

		// Constructor
		IOBUFFER_WRITE();
		~IOBUFFER_WRITE();

		// Initialize for IO
		inline void InitForIO(SOCKET sockWrite);
		inline void InitMsg(Message::MessageData *pMsg);
		inline void InitBuff(UINT uiBuffSize, BYTE* pBuff);

		// Setup sending mode
		inline void SetupSendUDP(SOCKET sockWrite, const sockaddr_storage& to, Message::MessageData *pMsg);
		inline void SetupSendUDP(SOCKET sockWrite, const sockaddr_storage& to, UINT uiBuffSize, BYTE* pBuff);
		inline void SetupSendTCP(Message::MessageData *pMsg);
		inline void SetupSendTCP(UINT uiBuffSize, BYTE* pBuff);

	};


	// UDP/TCP read overlapped
	struct IOBUFFER_READ : public IOBUFFER_RWBASE, public MemoryPoolObject<IOBUFFER_READ>
	{
		// UDP Recv socket length
		socklen_t iSockLen;

		// Recv connection ID for error check
		uintptr_t CID;

		// Recv buffer
		char buffer[Const::INTER_PACKET_SIZE_MAX];

		std::atomic<bool> bIsPending;

		// constructor
		IOBUFFER_READ();
		~IOBUFFER_READ();

		// Initialize for IO
		inline void InitForIO();
		inline void InitRecv(uintptr_t iCID);

		// Setup recving mode
		inline void SetupRecvUDP(uintptr_t iCID);
		inline void SetupRecvTCP(uintptr_t iCID);

		Result SetPendingTrue();
		Result SetPendingFalse();
	};


	// TCP accept overlapped
	struct IOBUFFER_ACCEPT : public IOBUFFER, public MemoryPoolObject<IOBUFFER_ACCEPT>
	{
		SOCKET sockAccept;

		sockaddr_storage sockAddr;

		// Constructor
		IOBUFFER_ACCEPT();
		~IOBUFFER_ACCEPT();

		// Setup accept
		inline void SetupAccept(SOCKET sock);
	};



#include "NetSystem_Linux.inl"

} // namespace Net
} // namespace BR


#endif
