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
#include "Common/PageQueue.h"




namespace BR {

	enum class SockFamily : UINT8;

namespace Net {

	struct IOBUFFER_WRITE;

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network Utility functions
	//
	HRESULT SockAddr2Addr( const sockaddr_in6 &sockAddr, NetAddress &addr );
	HRESULT SockAddr2Addr(const sockaddr_in &sockAddr, NetAddress &addr);
	HRESULT SockAddr2Addr(const sockaddr_storage &sockAddr, NetAddress &addr);
	HRESULT Addr2SockAddr( const NetAddress &addr, sockaddr_in6 &sockAddr );
	HRESULT Addr2SockAddr(const NetAddress &addr, sockaddr_in &sockAddr);
	HRESULT Addr2SockAddr(const NetAddress &addr, sockaddr_storage &sockAddr);
	HRESULT SetSockAddr( sockaddr_in6& sockAddr, const char *strAddr, USHORT usPort );
	HRESULT SetSockAddr(sockaddr_in& sockAddr, const char *strAddr, USHORT usPort);
	HRESULT SetSockAddr(sockaddr_storage& sockAddr, const char *strAddr, USHORT usPort);
	HRESULT GetLocalAddress(SockFamily family, NetAddress &addr);
	HRESULT GetLocalAddressIPv4(NetAddress &addr);
	HRESULT GetLocalAddressIPv6(NetAddress &addr);

	// Make bind addr with any address
	HRESULT GetAnyBindAddr(const sockaddr_storage &sockAddr, sockaddr_storage&bindAddr);

	// Set local net address with family check
	HRESULT SetLocalNetAddress(NetAddress &netAddr, const char *strLocalAddress, USHORT port);

	// Set net address with family check
	HRESULT SetNetAddress(NetAddress &netAddr, const char *strAddress, USHORT port);

	inline size_t GetSockAddrSize(const sockaddr_storage &sockAddr) { return sockAddr.ss_family == AF_INET6 ? sizeof(sockaddr_in6) : sizeof(sockaddr_in); }

	// Validate local IP
	HRESULT CheckLocalAddress(SockFamily family, NetAddress &addr);

	/////////////////////////////////////////////////////////////////////////////////
	//
	// Socket value
	//

	int ToSockValue(SockFamily family);
	int ToSockValue(SockType family);

	SockFamily ToSockFamily(int family);
	SockType ToSockType(int family);

	bool operator == ( const sockaddr_in6 &op1, const sockaddr_in6 &op2 );
	bool operator != ( const sockaddr_in6 &op1, const sockaddr_in6 &op2 );

	bool operator == (const sockaddr_storage &op1, const sockaddr_storage &op2);
	bool operator != (const sockaddr_storage &op1, const sockaddr_storage &op2);

	bool operator == ( const NetAddress &op1, const NetAddress &op2 );
	bool operator != ( const NetAddress &op1, const NetAddress &op2 );



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Packet message queue class
	//

	class MsgQueue : public PageQueue<Message::MessageData*>
	{
	public:
		MsgQueue(UINT uiNumElePerPage = 512);
		virtual ~MsgQueue();

		// Clear queue element
		virtual void ClearQueue();
	};


	// Write buffer queue
	class WriteBufferQueue : public PageQueue<IOBUFFER_WRITE*>
	{
	public:
		WriteBufferQueue(UINT uiNumElePerPage = 512);
		virtual ~WriteBufferQueue();

		// Clear queue element
		void ClearQueue();
	};



	////////////////////////////////////////////////////////////////////////////////
	//
	//	
	//

	void RegisterConnectionDebugMessage();


} // namespace Net
} // namespace BR



