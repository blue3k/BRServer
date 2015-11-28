////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
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



namespace BR {
namespace Net {

	class Connection;


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Overlapped I/O structures
	//

	struct IOBUFFER_WRITE;
	struct IOBUFFER_READ;
	struct IOBUFFER_ACCEPT;

	enum class IOBUFFER_OPERATION : UINT8
	{
		OP_NONE = 0,
		OP_TCPREAD,
		OP_TCPREADPENDING,
		OP_TCPWRITE,
		OP_TCPACCEPT,
		OP_UDPREAD,
		OP_UDPWRITE,
		OP_PEERUDPREAD,
		OP_PEERUDPWRITE,
	};


	// Get lastest socket error 
	HRESULT GetLastWSAHRESULT();


	//struct IOBUFFER : public WSAOVERLAPPED
	//{
	//	IOBUFFER_OPERATION Operation;

	//	// Clear Buffer
	//	void ClearBuffer();

	//};


	//// UDP/TCP read/write overlapped base
	//struct IOBUFFER_RWBASE : public IOBUFFER
	//{
	//	// IOCP buffer 
	//	WSABUF	wsaBuff;

	//	// Operated buffer size
	//	DWORD dwOperateSize;


	//	// Constructor
	//	IOBUFFER_RWBASE();
	//};


	//// UDP/TCP write overlapped
	//struct IOBUFFER_WRITE : public IOBUFFER_RWBASE, public MemoryPoolObject<IOBUFFER_WRITE>
	//{
	//	// Message pointer to send
	//	Message::MessageData *pMsgs;

	//	// Message buffer pointer to send
	//	BYTE *pSendBuff;

	//	// Constructor
	//	IOBUFFER_WRITE();
	//	~IOBUFFER_WRITE();

	//	// Initialize for IO
	//	inline void InitForIO();
	//	inline void InitMsg(Message::MessageData *pMsg);
	//	inline void InitBuff(UINT uiBuffSize, BYTE* pBuff);

	//	// Setup sending mode
	//	inline void SetupSendUDP(Message::MessageData *pMsg);
	//	inline void SetupSendUDP(UINT uiBuffSize, BYTE* pBuff);
	//	inline void SetupSendPeer(Message::MessageData *pMsg);
	//	inline void SetupSendPeer(UINT uiBuffSize, BYTE* pBuff);
	//	inline void SetupSendTCP(Message::MessageData *pMsg);
	//	inline void SetupSendTCP(UINT uiBuffSize, BYTE* pBuff);

	//};


	//// UDP/TCP read overlapped
	//struct IOBUFFER_READ : public IOBUFFER_RWBASE
	//{
	//	// Read flag
	//	DWORD dwFlags;

	//	// UDP Read from
	//	struct sockaddr_in6 From;

	//	// UDP Recv socket length
	//	INT iSockLen;

	//	// Recv connection ID for error check
	//	uintptr_t CID;

	//	// Recv buffer
	//	char buffer[Const::INTER_PACKET_SIZE_MAX];

	//	bool bIsPending;

	//	// constructor
	//	IOBUFFER_READ();
	//	~IOBUFFER_READ();

	//	// Initialize for IO
	//	inline void InitForIO();
	//	inline void InitRecv(uintptr_t iCID);

	//	// Setup recving mode
	//	inline void SetupRecvUDP(uintptr_t iCID);
	//	inline void SetupRecvPeer(uintptr_t iCID);
	//	inline void SetupRecvTCP(uintptr_t iCID);
	//	inline void SetupRecvTCPPending(uintptr_t iCID);

	//} ;


	//// TCP accept overlapped
	//struct IOBUFFER_ACCEPT : public IOBUFFER
	//{
	//	SOCKET sockAccept;
	//	BYTE pAcceptInfo[(sizeof(sockaddr_in6) + 16) * 2];

	//	// Constructor
	//	IOBUFFER_ACCEPT();
	//	~IOBUFFER_ACCEPT();

	//	// Setup accept
	//	inline void SetupAccept(SOCKET sock);

	//};



	////////////////////////////////////////////////////////////////////////////////
	//
	// NetIO callback interface
	//

	class INetIOCallBack
	{
	public:
		// called when New connection TCP accepted
		virtual HRESULT OnIOAccept(HRESULT hrRes, IOBUFFER_ACCEPT *pAcceptInfo) = 0;

		// called when reciving messag is completed
		virtual HRESULT OnIORecvCompleted(HRESULT hrRes, IOBUFFER_READ *pIOBuffer, DWORD dwTransferred) = 0;

		// called when send completed
		virtual HRESULT OnIOSendCompleted(HRESULT hrRes, IOBUFFER_WRITE *pIOBuffer, DWORD dwTransferred) = 0;
	};


	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network system
	//
	namespace NetSystem
	{
		// Open network system
		HRESULT OpenSystem( UINT uiOverBufferCount, UINT numRecvThread, UINT gatheringBufferSize );

		void CloseSystem();

		// Network buffer operation
		HRESULT AllocBuffer( IOBUFFER_WRITE* &pIOBuffer );
		HRESULT FreeBuffer( IOBUFFER_WRITE *pIOBuffer );

		HRESULT AllocGatheringBuffer( BYTE* &pBuffer, UINT& bufferSize );
		HRESULT FreeGatheringBuffer( BYTE *pBuffer );

		HRESULT RegisterSocket(SOCKET sock, INetIOCallBack* cbInstance);
	};


#include "NetSystem.inl"

} // namespace Net
} // namespace BR

#if WINDOWS
#include "Net/NetSystem_WinIOCP.h"
#else
#include "Net/NetSystem_EPOLL.h"
#endif


