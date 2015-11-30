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

	enum class SockFamily
	{
		IPV4 = AF_INET,
		IPV6 = AF_INET6
	};

	enum class SockType
	{
		Stream = SOCK_STREAM,       // TCP
		DataGram  = SOCK_DGRAM,     // UDP
	};


	// Get lastest socket error 
	HRESULT GetLastWSAHRESULT();




	////////////////////////////////////////////////////////////////////////////////
	//
	// NetIO callback interface
	//

	class INetIOCallBack
	{
	public:

		INetIOCallBack() {}
		virtual ~INetIOCallBack() {}

		virtual HRESULT Accept(IOBUFFER_ACCEPT* &pAcceptInfo) { return E_NOTIMPL; };
		// called when New connection TCP accepted
		virtual HRESULT OnIOAccept(HRESULT hrRes, IOBUFFER_ACCEPT *pAcceptInfo) { return E_NOTIMPL; };

		virtual HRESULT Recv(IOBUFFER_READ* pIOBuffer) = 0;
		// called when reciving messag is completed
		virtual HRESULT OnIORecvCompleted(HRESULT hrRes, IOBUFFER_READ *pIOBuffer) = 0;

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


		///////////////////////////////////////////////////////////////////////////////
		// Socket handling 

		HRESULT RegisterSocket(SOCKET sock, INetIOCallBack* cbInstance, bool isListenSocket);

		SOCKET Socket(SockFamily domain, SockType type);
		void CloseSocket(SOCKET sock);

		HRESULT Accept(SOCKET sockListen, IOBUFFER_ACCEPT* pAccept);
		HRESULT HandleAcceptedSocket(SOCKET sockListen, IOBUFFER_ACCEPT* pAccept, sockaddr_in6& remoteAddr);

		HRESULT Recv(SOCKET sock, IOBUFFER_READ* pBuffer);
		HRESULT RecvFrom(SOCKET sock, IOBUFFER_READ* pBuffer);

		HRESULT Send(SOCKET sock, IOBUFFER_WRITE* pBuffer);
		HRESULT SendTo(SOCKET sock, const sockaddr_in6& dstAddress, IOBUFFER_WRITE* pBuffer);
	};


#include "NetSystem.inl"

} // namespace Net
} // namespace BR

#if WINDOWS
#include "Net/NetSystem_WinIOCP.h"
#else
#include "Net/NetSystem_EPOLL.h"
#endif


