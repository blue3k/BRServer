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

#include <sys/epoll.h>



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


		// Constructor
		IOBUFFER_RWBASE();
	};


	// UDP/TCP write overlapped
	struct IOBUFFER_WRITE : public IOBUFFER_RWBASE, public MemoryPoolObject<IOBUFFER_WRITE>
	{
		// Sending raw buffer
		UINT RawSendSize;
		BYTE* pRawSendBuffer;

		// Message pointer to send
		Message::MessageData *pMsgs;

		// Message buffer pointer to send
		BYTE *pSendBuff;

		// Constructor
		IOBUFFER_WRITE();
		~IOBUFFER_WRITE();

		// Initialize for IO
		inline void InitForIO();
		inline void InitMsg(Message::MessageData *pMsg);
		inline void InitBuff(UINT uiBuffSize, BYTE* pBuff);

		// Setup sending mode
		inline void SetupSendUDP(Message::MessageData *pMsg);
		inline void SetupSendUDP(UINT uiBuffSize, BYTE* pBuff);
		inline void SetupSendPeer(Message::MessageData *pMsg);
		inline void SetupSendPeer(UINT uiBuffSize, BYTE* pBuff);
		inline void SetupSendTCP(Message::MessageData *pMsg);
		inline void SetupSendTCP(UINT uiBuffSize, BYTE* pBuff);

	};


	// UDP/TCP read overlapped
	struct IOBUFFER_READ : public IOBUFFER_RWBASE
	{
		// UDP Read from
		struct sockaddr_in6 From;

		// UDP Recv socket length
		socklen_t iSockLen;

		// Recv connection ID for error check
		uintptr_t CID;

		// Recv buffer
		char buffer[Const::INTER_PACKET_SIZE_MAX];

		bool bIsPending;

		// constructor
		IOBUFFER_READ();
		~IOBUFFER_READ();

		// Initialize for IO
		inline void InitForIO();
		inline void InitRecv(uintptr_t iCID);

		// Setup recving mode
		inline void SetupRecvUDP(uintptr_t iCID);
		inline void SetupRecvPeer(uintptr_t iCID);
		inline void SetupRecvTCP(uintptr_t iCID);
		inline void SetupRecvTCPPending(uintptr_t iCID);
	};


	// TCP accept overlapped
	struct IOBUFFER_ACCEPT : public IOBUFFER
	{
		SOCKET sockAccept;

		sockaddr_in6 sockAddr;

		// Constructor
		IOBUFFER_ACCEPT();
		~IOBUFFER_ACCEPT();

		// Setup accept
		inline void SetupAccept(SOCKET sock);
	};




	////////////////////////////////////////////////////////////////////////////////
	//
	//	EPOLL thread worker
	//
	class EPOLLWorker : public Thread
	{
	public:

		enum {
			MAX_EPOLL_EVENTS = 100,
			MAX_EPOLL_WAIT = 500,
		};

	private:
		// Epoll handle
		int m_hEpoll;

	public:
		// Constructor/destructor
		EPOLLWorker(int hEpoll);

		~EPOLLWorker();

		virtual void Run() override;


		HRESULT HandleAccept(SOCKET sock, INetIOCallBack* pCallBack);
		HRESULT HandleRW(SOCKET sock, unsigned int events, INetIOCallBack* pCallBack);
	};



#include "NetSystem_EPOLL.inl"

} // namespace Net
} // namespace BR


#endif
