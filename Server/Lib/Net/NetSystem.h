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
		OP_TCPWRITE,
		OP_TCPACCEPT,
		OP_UDPREAD,
		OP_UDPWRITE,
	};


	// Get lastest socket error 
	HRESULT GetLastWSAHRESULT();




	////////////////////////////////////////////////////////////////////////////////
	//
	// NetIO  interface
	//

	class INetIOCallBack
	{
	public:

		struct CBFlags
		{
			UINT32 IsListenSocket : 1;

			CBFlags()
				: IsListenSocket(0)
			{}
		};

	private:

		CBFlags m_CBFlags;

		WriteBufferQueue* m_pWriteQueues;

	public:

		INetIOCallBack() : m_pWriteQueues(nullptr) {}
		virtual ~INetIOCallBack() {}

		const CBFlags& GetIOFlags() const { return m_CBFlags; }
		CBFlags& GetIOFlagsEditable() { return m_CBFlags; }
		virtual SOCKET GetIOSocket() = 0;

		// Write queue
		WriteBufferQueue* GetWriteQueue() { return m_pWriteQueues; }
		void SetWriteQueue(WriteBufferQueue* writeQueue) { Assert(writeQueue != nullptr && m_pWriteQueues == nullptr); m_pWriteQueues = writeQueue; }


		// Send message to connection with network device
		virtual HRESULT SendBuffer(IOBUFFER_WRITE *pSendBuffer) = 0;
		virtual HRESULT EnqueueBuffer(IOBUFFER_WRITE *pSendBuffer);


		virtual HRESULT Accept(IOBUFFER_ACCEPT* &pAcceptInfo) { return E_NOTIMPL; };
		// called when New connection TCP accepted
		virtual HRESULT OnIOAccept(HRESULT hrRes, IOBUFFER_ACCEPT *pAcceptInfo) { return E_NOTIMPL; };

		virtual HRESULT Recv(IOBUFFER_READ* pIOBuffer) = 0;
		// called when reciving messag is completed
		virtual HRESULT OnIORecvCompleted(HRESULT hrRes, IOBUFFER_READ* &pIOBuffer) = 0;

		virtual HRESULT OnSendReady() = 0;
		virtual HRESULT ProcessSendQueue();
		// called when send completed
		virtual HRESULT OnIOSendCompleted(HRESULT hrRes, IOBUFFER_WRITE *pIOBuffer) = 0;
	};





	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network system
	//
	namespace NetSystem
	{
		inline bool IsProactorSystem()
		{
#if WINDOWS  // IOCP
			return true;
#else        // EPOLL/KQUEUE
			return false;
#endif
		}

		// Open network system
		HRESULT OpenSystem( UINT uiOverBufferCount, UINT numRecvThread, UINT gatheringBufferSize );

		void CloseSystem();

		// Network buffer operation
		HRESULT AllocBuffer( IOBUFFER_WRITE* &pIOBuffer );
		HRESULT FreeBuffer( IOBUFFER_WRITE *pIOBuffer );

		HRESULT AllocGatheringBuffer( BYTE* &pBuffer, UINT& bufferSize );
		HRESULT FreeGatheringBuffer( BYTE *pBuffer );


		WriteBufferQueue* GetWriteBufferQueue();


		///////////////////////////////////////////////////////////////////////////////
		// Socket handling 

		HRESULT RegisterSocket(SockType sockType, INetIOCallBack* cbInstance);
		HRESULT UnregisterSocket(SockType sockType, INetIOCallBack* cbInstance);
		HRESULT RegisterSharedSocket(SockType sockType, INetIOCallBack* cbInstance);

		SOCKET Socket(SockFamily domain, SockType type);
		void CloseSocket(SOCKET sock);

		HRESULT Accept(SOCKET sockListen, IOBUFFER_ACCEPT* pAccept);
		HRESULT HandleAcceptedSocket(SOCKET sockListen, IOBUFFER_ACCEPT* pAccept, sockaddr_storage& remoteAddr);

		HRESULT Recv(SOCKET sock, IOBUFFER_READ* pBuffer);
		HRESULT RecvFrom(SOCKET sock, IOBUFFER_READ* pBuffer);

		HRESULT Send(SOCKET sock, IOBUFFER_WRITE* pBuffer);
		HRESULT SendTo(SOCKET sock, IOBUFFER_WRITE* pBuffer);
	};


#include "NetSystem.inl"

} // namespace Net
} // namespace BR

#if WINDOWS
#include "Net/NetSystem_WinIOCP.h"
#else
#include "Net/NetSystem_Linux.h"
#endif


