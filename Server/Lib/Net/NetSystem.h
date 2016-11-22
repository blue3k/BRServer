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
	Result GetLastWSAResult();




	////////////////////////////////////////////////////////////////////////////////
	//
	// NetIO  interface
	//

	class INetIOCallBack
	{
	public:

		struct CBFlags
		{
			// Is listen only socket?
			uint32_t IsListenSocket : 1;

			CBFlags()
				: IsListenSocket(0)
			{}

			~CBFlags()
			{
			}
		};

		enum class IOStatus : uint32_t
		{
			None,
			Registered
		};

	private:

		CBFlags m_CBFlags;

		SockType m_IOSockType;

		// Is registered to IO sub system
		std::atomic<IOStatus> m_IOStatus;

		WriteBufferQueue* m_pWriteQueues;

		// Assigned IO worker thread index
		int m_AssignedIOWorker;

		// Link to original socket variable
		const SOCKET &m_IOSocket;

	public:

		INetIOCallBack(const SOCKET &IOSocketVariable);

		virtual ~INetIOCallBack();

		int GetAssignedIOWorker() { return m_AssignedIOWorker; }
		void SetAssignedIOWorker(int assignedIOWorker);

		// casting, a bit faster than dynamic cast
		//virtual SharedObject* AsSharedObject() { return nullptr; }
		bool GetIsIORegistered() { return m_IOStatus.load(std::memory_order_relaxed) == IOStatus::Registered; }
		void OnIORegistered(SockType ioSockType);
		void OnIOUnregistered();

		SockType GetIOSockType() { return m_IOSockType; }

		const CBFlags& GetIOFlags() const { return m_CBFlags; }
		CBFlags& GetIOFlagsEditable() { return m_CBFlags; }
		SOCKET GetIOSocket() { return m_IOSocket; }

		// Write queue
		WriteBufferQueue* GetWriteQueue() { return m_pWriteQueues; }
		void SetWriteQueue(WriteBufferQueue* writeQueue) { Assert(writeQueue != nullptr && m_pWriteQueues == nullptr); m_pWriteQueues = writeQueue; }


		// Send message to connection with network device
		virtual Result SendBuffer(IOBUFFER_WRITE *pSendBuffer) = 0;
		virtual Result EnqueueBuffer(IOBUFFER_WRITE *pSendBuffer);


		virtual Result Accept(IOBUFFER_ACCEPT* &pAcceptInfo) { return ResultCode::NOT_IMPLEMENTED; };
		// called when New connection TCP accepted
		virtual Result OnIOAccept(Result hrRes, IOBUFFER_ACCEPT *pAcceptInfo) { return ResultCode::NOT_IMPLEMENTED; };

		virtual Result Recv(IOBUFFER_READ* pIOBuffer) = 0;
		// called when reciving messag is completed
		virtual Result OnIORecvCompleted(Result hrRes, IOBUFFER_READ* &pIOBuffer) = 0;

		virtual Result OnSendReady() = 0;
		virtual Result ProcessSendQueue();
		// called when send completed
		virtual Result OnIOSendCompleted(Result hrRes, IOBUFFER_WRITE *pIOBuffer) = 0;
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
		Result OpenSystem( UINT uiOverBufferCount, UINT numRecvThread, UINT gatheringBufferSize );

		void CloseSystem();

		// Network buffer operation
		Result AllocBuffer( IOBUFFER_WRITE* &pIOBuffer );
		Result FreeBuffer( IOBUFFER_WRITE *pIOBuffer );

		Result AllocGatheringBuffer( BYTE* &pBuffer, UINT& bufferSize );
		Result FreeGatheringBuffer( BYTE *pBuffer );


		WriteBufferQueue* GetWriteBufferQueue();


		///////////////////////////////////////////////////////////////////////////////
		// Socket handling 

		Result RegisterSocket(SockType sockType, INetIOCallBack* cbInstance);
		Result UnregisterSocket(INetIOCallBack* cbInstance);
		//Result RegisterSharedSocket(SockType sockType, INetIOCallBack* cbInstance);

		SOCKET Socket(SockFamily domain, SockType type);
		void CloseSocket(SOCKET sock);

		Result Accept(SOCKET sockListen, IOBUFFER_ACCEPT* pAccept);
		Result HandleAcceptedSocket(SOCKET sockListen, IOBUFFER_ACCEPT* pAccept, sockaddr_storage& remoteAddr);

		Result Recv(SOCKET sock, IOBUFFER_READ* pBuffer);
		Result RecvFrom(SOCKET sock, IOBUFFER_READ* pBuffer);

		Result Send(SOCKET sock, IOBUFFER_WRITE* pBuffer);
		Result SendTo(SOCKET sock, IOBUFFER_WRITE* pBuffer);
	};


#include "NetSystem.inl"

} // namespace Net
} // namespace BR

#if WINDOWS
#include "Net/NetSystem_WinIOCP.h"
#else
#include "Net/NetSystem_Linux.h"
#endif


