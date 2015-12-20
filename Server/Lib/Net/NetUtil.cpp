////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : KyungKun Ko
//
// Description : Net base type definitions. 
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/HRESNet.h"
#include "Common/TimeUtil.h"
#include "Common/Utility.h"
#include "Common/SpinSharedBuffer.h"
#include "Common/MemoryPool.h"
#include "Net/NetTrace.h"
#include "Net/NetUtil.h"
#include "Net/NetSystem.h"

#if !WINDOWS
#include <net/if.h>
#include <ifaddrs.h>
#endif


namespace BR {
namespace Net {

	static_assert(INET6_ADDRSTRLEN <= NetAddress::MAX_NETNAME, "Net address length must be same with client");

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network Utility functions
	//
	HRESULT SockAddr2Addr(const sockaddr_in6 &sockAddr, NetAddress &addr)
	{
		Assert(sockAddr.sin6_family == AF_INET6);
		addr.strAddr[0] = '\0';
		auto result = inet_ntop(sockAddr.sin6_family, (void*)&sockAddr.sin6_addr, addr.strAddr, sizeof addr.strAddr);
		if (result == nullptr) return GetLastWSAHRESULT();
		addr.usPort = ntohs(sockAddr.sin6_port);
		return S_OK;
	}

	HRESULT SockAddr2Addr(const sockaddr_in &sockAddr, NetAddress &addr)
	{
		Assert(sockAddr.sin_family == AF_INET);
		addr.strAddr[0] = '\0';
		auto result = inet_ntop(sockAddr.sin_family, (void*)&sockAddr.sin_addr, addr.strAddr, sizeof addr.strAddr);
		if (result == nullptr) return GetLastWSAHRESULT();
		addr.usPort = ntohs(sockAddr.sin_port);
		return S_OK;
	}

	HRESULT Addr2SockAddr(const NetAddress &addr, sockaddr_in6 &sockAddr)
	{
		memset(&sockAddr, 0, sizeof(sockAddr));
		sockAddr.sin6_family = AF_INET6;
		sockAddr.sin6_port = htons(addr.usPort);
		int result = inet_pton(sockAddr.sin6_family, addr.strAddr, &sockAddr.sin6_addr);
		if (result != TRUE)
		{
			return E_FAIL;
		}
		else
			return S_OK;
	}

	HRESULT Addr2SockAddr(const NetAddress &addr, sockaddr_in &sockAddr)
	{
		memset(&sockAddr, 0, sizeof(sockAddr));
		sockAddr.sin_family = AF_INET;
		sockAddr.sin_port = htons(addr.usPort);
		int result = inet_pton(sockAddr.sin_family, addr.strAddr, &sockAddr.sin_addr);
		if (result != TRUE)
		{
			return E_FAIL;
		}
		else
			return S_OK;
	}

	HRESULT SetSockAddr(sockaddr_in6& sockAddr, const char *strAddr, USHORT usPort)
	{
		memset(&sockAddr, 0, sizeof(sockAddr));

		sockAddr.sin6_family = AF_INET6;
		sockAddr.sin6_port = htons(usPort);
		int result = inet_pton(sockAddr.sin6_family, strAddr, &sockAddr.sin6_addr);
		if (result != TRUE)
		{
			return E_FAIL;
		}

		return S_OK;
	}

	HRESULT SetSockAddr(sockaddr_in& sockAddr, const char *strAddr, USHORT usPort)
	{
		memset(&sockAddr, 0, sizeof(sockAddr));

		sockAddr.sin_family = AF_INET;
		sockAddr.sin_port = htons(usPort);
		int result = inet_pton(sockAddr.sin_family, strAddr, &sockAddr.sin_addr);
		if (result != TRUE)
		{
			return E_FAIL;
		}

		return S_OK;
	}


#if WINDOWS
	HRESULT GetLocalAddressIPv4(NetAddress &addr)
	{
		char tempBuffer[128];
		//Convert IPV6 to IPV4
		struct addrinfo hints, *res;
		bool bIsFound = false;

		// Convert remote address
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_INET;  // use IPv4
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		auto error = getaddrinfo("", nullptr, &hints, &res);
		switch (error)
		{
		case 0:				break;
		case EAI_AGAIN:		return E_NET_TRY_AGAIN;
		case EAI_BADFLAGS:	return E_NET_BADFLAGS;
		case EAI_FAIL:		return E_FAIL;
		case EAI_FAMILY:	return E_NET_FAMILY;
		case EAI_MEMORY:	return E_OUTOFMEMORY;
		case EAI_NONAME:	return E_NET_HOST_NOT_FOUND;
		case EAI_SERVICE:	return E_NET_INVALID_SERVICE;
		case EAI_SOCKTYPE:	return E_NET_NOTSOCK;
		default:			return E_UNEXPECTED;
		}

		for (auto curAddr = res; curAddr != nullptr; curAddr = curAddr->ai_next)
		{
			if (curAddr->ai_family == AF_INET)
			{
				sockaddr_in* psockAddr4 = ((sockaddr_in*)curAddr->ai_addr);
				bIsFound = inet_ntop(AF_INET, &psockAddr4->sin_addr, tempBuffer, sizeof tempBuffer) != nullptr;
				break;
			}
		}
		freeaddrinfo(res);

		if (bIsFound)
		{
			StrUtil::StringCpy(addr.strAddr, tempBuffer);
		}

		return bIsFound ? S_OK : E_FAIL;
	}

	HRESULT GetLocalAddressIPv6(NetAddress &addr)
	{
		char tempBuffer[256];
		//Convert IPV6
		struct addrinfo hints, *res;
		bool bIsFound = false;

		// Convert remote address
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_INET6;  // use IPv6
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_flags = AI_PASSIVE;
		auto error = getaddrinfo("", nullptr, &hints, &res);
		switch (error)
		{
		case 0:				break;
		case EAI_AGAIN:		return E_NET_TRY_AGAIN;
		case EAI_BADFLAGS:	return E_NET_BADFLAGS;
		case EAI_FAIL:		return E_FAIL;
		case EAI_FAMILY:	return E_NET_FAMILY;
		case EAI_MEMORY:	return E_OUTOFMEMORY;
		case EAI_NONAME:	return E_NET_HOST_NOT_FOUND;
		case EAI_SERVICE:	return E_NET_INVALID_SERVICE;
		case EAI_SOCKTYPE:	return E_NET_NOTSOCK;
		default:			return E_UNEXPECTED;
		}

		for (auto curAddr = res; curAddr != nullptr; curAddr = curAddr->ai_next)
		{
			if (curAddr->ai_family == AF_INET6)
			{
				sockaddr_in6* psockAddr6 = ((sockaddr_in6*)curAddr->ai_addr);
				if (inet_ntop(AF_INET6, &psockAddr6->sin6_addr, tempBuffer, sizeof(tempBuffer)) != nullptr)
				{
					bIsFound = true;
					break;
				}
			}
		}
		freeaddrinfo(res);

		if (bIsFound)
		{
			StrUtil::StringCpy(addr.strAddr, tempBuffer);
		}

		return bIsFound ? S_OK : E_FAIL;
	}

#else


	//HRESULT GetLocalAddress(SockFamily family, NetAddress &addr)
	//{
	//	char tempBuffer[NI_MAXHOST];
	//	//Convert IPV6
	//	struct addrinfo hints, *res;
	//	bool bIsFound = false;

	//	// Convert remote address
	//	memset(&hints, 0, sizeof hints);
	//	hints.ai_family = (int)family;  // use IPv6
	//	hints.ai_socktype = SOCK_DGRAM;
	//	hints.ai_flags = AI_PASSIVE;
	//	auto error = getaddrinfo(nullptr, "1111", &hints, &res);
	//	switch (error)
	//	{
	//	case 0:				break;
	//	case EAI_AGAIN:		return E_NET_TRY_AGAIN;
	//	case EAI_BADFLAGS:	return E_NET_BADFLAGS;
	//	case EAI_FAIL:		return E_FAIL;
	//	case EAI_FAMILY:	return E_NET_FAMILY;
	//	case EAI_MEMORY:	return E_OUTOFMEMORY;
	//	case EAI_NONAME:	return E_NET_HOST_NOT_FOUND;
	//	case EAI_SERVICE:	return E_NET_INVALID_SERVICE;
	//	case EAI_SOCKTYPE:	return E_NET_NOTSOCK;
	//	default:			return E_UNEXPECTED;
	//	}

	//	for (auto curAddr = res; curAddr != nullptr; curAddr = curAddr->ai_next)
	//	{
	//		switch (curAddr->ai_family)
	//		{
	//		case AF_INET:
	//		{
	//			struct sockaddr_in *psockAddr = (struct sockaddr_in *)curAddr->ai_addr;
	//			bIsFound = inet_ntop(AF_INET, &psockAddr->sin_addr, tempBuffer, sizeof tempBuffer) != nullptr;
	//			break;
	//		}

	//		case AF_INET6:
	//		{
	//			struct sockaddr_in6 *psockAddr = (struct sockaddr_in6 *)curAddr->ai_addr;
	//			inet_ntop(AF_INET6, &psockAddr->sin6_addr, tempBuffer, sizeof tempBuffer) != nullptr;
	//			break;
	//		}
	//		}
	//	}
	//	freeaddrinfo(res);

	//	if (bIsFound)
	//	{
	//		StrUtil::StringCpy(addr.strAddr, tempBuffer);
	//	}

	//	return bIsFound ? S_OK : E_FAIL;
	//}

	HRESULT GetLocalAddress(SockFamily family, NetAddress &addr)
	{
		struct ifaddrs *ifaddr = nullptr;
		char tempBuffer[NI_MAXHOST];
		bool bIsFound = false;

		if (getifaddrs(&ifaddr) == -1)
		{
			return GetLastWSAHRESULT();
		}

		/* Walk through linked list, maintaining head pointer so we
		can free list later */

		for (struct ifaddrs *curAddr = ifaddr; curAddr != nullptr && !bIsFound; curAddr = curAddr->ifa_next)
		{
			if (curAddr->ifa_addr == nullptr)
				continue;

			if (!(curAddr->ifa_flags & IFF_UP))
				continue;

			if ((curAddr->ifa_flags & IFF_LOOPBACK))
				continue;

			if (curAddr->ifa_addr->sa_family != (int)family)
				continue;

			switch (curAddr->ifa_addr->sa_family)
			{
			case AF_INET:
			{
				struct sockaddr_in *psockAddr = (struct sockaddr_in *)curAddr->ifa_addr;
				bIsFound = inet_ntop(AF_INET, &psockAddr->sin_addr, tempBuffer, sizeof tempBuffer) != nullptr;
				break;
			}

			case AF_INET6:
			{
				struct sockaddr_in6 *psockAddr = (struct sockaddr_in6 *)curAddr->ifa_addr;
				bIsFound = inet_ntop(AF_INET6, &psockAddr->sin6_addr, tempBuffer, sizeof tempBuffer) != nullptr;
				break;
			}

			default:
				continue;
			}
		}

		freeifaddrs(ifaddr);

		if (bIsFound)
		{
			StrUtil::StringCpy(addr.strAddr, tempBuffer);
		}

		return bIsFound ? S_OK : E_FAIL;
	}

	HRESULT GetLocalAddressIPv4(NetAddress &addr)
	{
		return GetLocalAddress(SockFamily::IPV4, addr);
	}

	HRESULT GetLocalAddressIPv6(NetAddress &addr)
	{
		return GetLocalAddress(SockFamily::IPV6, addr);
	}

	HRESULT CheckLocalAddress(SockFamily family, NetAddress &addr)
	{
		struct ifaddrs *ifaddr = nullptr;
		bool bIsFound = false;
		sockaddr_storage testSockAddr;

		// make sock addr
		memset(&testSockAddr, 0, sizeof testSockAddr);
		if (family == SockFamily::IPV4)
		{
			auto *pSockAddr = (struct sockaddr_in *)&testSockAddr;
			if (FAILED(Addr2SockAddr(addr, *pSockAddr)))
				return E_FAIL;
		}
		else
		{
			auto *pSockAddr = (struct sockaddr_in6 *)&testSockAddr;
			if (FAILED(Addr2SockAddr(addr, *pSockAddr)))
				return E_FAIL;
		}
			
		if (getifaddrs(&ifaddr) == -1)
		{
			return GetLastWSAHRESULT();
		}

		/* Walk through linked list, maintaining head pointer so we
		can free list later */

		for (struct ifaddrs *curAddr = ifaddr; curAddr != nullptr && !bIsFound; curAddr = curAddr->ifa_next)
		{
			if (curAddr->ifa_addr == nullptr)
				continue;

			if (!(curAddr->ifa_flags & IFF_UP))
				continue;

			if ((curAddr->ifa_flags & IFF_LOOPBACK))
				continue;

			if (curAddr->ifa_addr->sa_family != (int)family)
				continue;

			switch ((int)family)
			{
			case AF_INET:
			{
				auto *pTestSockAddr = (struct sockaddr_in *)&testSockAddr;
				struct sockaddr_in *psockAddr = (struct sockaddr_in *)curAddr->ifa_addr;
				bIsFound = memcmp(&psockAddr->sin_addr, &pTestSockAddr->sin_addr, sizeof(pTestSockAddr->sin_addr)) == 0;
				break;
			}

			case AF_INET6:
			{
				auto *pTestSockAddr = (struct sockaddr_in6 *)&testSockAddr;
				struct sockaddr_in6 *psockAddr = (struct sockaddr_in6 *)curAddr->ifa_addr;
				bIsFound = memcmp(&psockAddr->sin6_addr, &pTestSockAddr->sin6_addr, sizeof(pTestSockAddr->sin6_addr)) == 0;
				break;
			}

			default:
				continue;
			}
		}

		freeifaddrs(ifaddr);

		return bIsFound ? S_OK : E_FAIL;

	}

#endif


	bool operator == (const sockaddr_in6 &op1, const sockaddr_in6 &op2)
	{
		return memcmp(&op1.sin6_addr, &op2.sin6_addr, sizeof op1.sin6_addr) == 0
			&& op1.sin6_port == op2.sin6_port;
	}

	bool operator != (const sockaddr_in6 &op1, const sockaddr_in6 &op2)
	{
		return memcmp(&op1.sin6_addr, &op2.sin6_addr, sizeof op1.sin6_addr) != 0
			|| op1.sin6_port != op2.sin6_port;
		//return op1.sin_addr.S_un.S_addr != op2.sin_addr.S_un.S_addr
		//	|| op1.sin_port != op2.sin_port;
	}

	bool operator == (const NetAddress &op1, const NetAddress &op2)
	{
		return strncmp(op1.strAddr, op2.strAddr, 16) == 0 && op1.usPort == op2.usPort;
	}

	bool operator != (const NetAddress &op1, const NetAddress &op2)
	{
		return strncmp(op1.strAddr, op2.strAddr, 16) != 0 || op1.usPort != op2.usPort;
	}



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Packet message queue class
	//

	MsgQueue::MsgQueue(UINT uiNumElePerPage)
		:PageQueue<Message::MessageData*>(uiNumElePerPage)
	{
	}

	MsgQueue::~MsgQueue()
	{
		ClearQueue();
	}

	// Clear queue element
	void MsgQueue::ClearQueue()
	{
		Message::MessageData* data = NULL;

		while (Dequeue(data) == S_OK)
		{
			if (data) data->Release();
		}
	}




	WriteBufferQueue::WriteBufferQueue(UINT uiNumElePerPage)
		:PageQueue<IOBUFFER_WRITE*>(uiNumElePerPage)
	{
	}

	WriteBufferQueue::~WriteBufferQueue()
	{
		ClearQueue();
	}

	// Clear queue element
	void WriteBufferQueue::ClearQueue()
	{
		IOBUFFER_WRITE* data = NULL;

		while (Dequeue(data) == S_OK)
		{
			Util::SafeDelete(data);
		}
	}




	//WriteBufferQueueManager::WriteBufferQueueManager(UINT numQueue, UINT uiNumElePerPage)
	//	: m_AssignIndex(0)
	//{
	//	m_WriteBufferQueues.SetSize(numQueue);
	//	for (UINT iQueue = 0; iQueue < m_WriteBufferQueues.GetSize(); iQueue++)
	//	{
	//		m_WriteBufferQueues[iQueue] = new WriteBufferQueue(uiNumElePerPage);
	//	}
	//}

	//WriteBufferQueueManager::~WriteBufferQueueManager()
	//{
	//	Clear();
	//}

	//// Clear queue element
	//void WriteBufferQueueManager::Clear()
	//{
	//	m_AssignIndex = 0;
	//	for (UINT iQueue = 0; iQueue < m_WriteBufferQueues.GetSize(); iQueue++)
	//	{
	//		m_WriteBufferQueues[iQueue]->ClearQueue();
	//		delete m_WriteBufferQueues[iQueue];
	//	}

	//	m_WriteBufferQueues.Clear();
	//}

	//HRESULT WriteBufferQueueManager::ChooseQueue(WriteBufferQueue* &pQueue)
	//{
	//	auto newIndex = m_AssignIndex.fetch_add(1, std::memory_order_relaxed) % m_WriteBufferQueues.GetSize();
	//}

} // namespace Net
} // namespace BR

