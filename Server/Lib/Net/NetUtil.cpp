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
#include "Common/ResultCode/BRResultCodeNet.h"
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
	Result SockAddr2Addr(const sockaddr_in6 &sockAddr, NetAddress &addr)
	{
		Assert(sockAddr.sin6_family == AF_INET6);
		addr.SocketFamily = SockFamily::IPV6;
		addr.strAddr[0] = '\0';
		auto result = inet_ntop(sockAddr.sin6_family, (void*)&sockAddr.sin6_addr, addr.strAddr, sizeof addr.strAddr);
		if (result == nullptr) return GetLastWSAResult();
		addr.usPort = ntohs(sockAddr.sin6_port);
		return ResultCode::SUCCESS;
	}

	Result SockAddr2Addr(const sockaddr_in &sockAddr, NetAddress &addr)
	{
		Assert(sockAddr.sin_family == AF_INET);
		addr.SocketFamily = SockFamily::IPV4;
		addr.strAddr[0] = '\0';
		auto result = inet_ntop(sockAddr.sin_family, (void*)&sockAddr.sin_addr, addr.strAddr, sizeof addr.strAddr);
		if (result == nullptr) return GetLastWSAResult();
		addr.usPort = ntohs(sockAddr.sin_port);
		return ResultCode::SUCCESS;
	}

	Result SockAddr2Addr(const sockaddr_storage &sockAddr, NetAddress &addr)
	{
		if (sockAddr.ss_family == AF_INET6)
		{
			return SockAddr2Addr(*(sockaddr_in6*)&sockAddr, addr);
		}
		else if(sockAddr.ss_family == AF_INET)
		{
			return SockAddr2Addr(*(sockaddr_in*)&sockAddr, addr);
		}
		else
		{
			// Not supported address family
			addr.SocketFamily = SockFamily::None;
			addr.strAddr[0] = '\0';
			addr.usPort = 0;
		}

		return ResultCode::SUCCESS;
	}

	Result Addr2SockAddr(const NetAddress &addr, sockaddr_in6 &sockAddr)
	{
		memset(&sockAddr, 0, sizeof(sockAddr));
		sockAddr.sin6_family = AF_INET6;
		sockAddr.sin6_port = htons(addr.usPort);
		int result = inet_pton(sockAddr.sin6_family, addr.strAddr, &sockAddr.sin6_addr);
		if (result != TRUE)
		{
			return ResultCode::FAIL;
		}
		else
			return ResultCode::SUCCESS;
	}

	Result Addr2SockAddr(const NetAddress &addr, sockaddr_in &sockAddr)
	{
		memset(&sockAddr, 0, sizeof(sockAddr));
		sockAddr.sin_family = AF_INET;
		sockAddr.sin_port = htons(addr.usPort);
		int result = inet_pton(sockAddr.sin_family, addr.strAddr, &sockAddr.sin_addr);
		if (result != TRUE)
		{
			return ResultCode::FAIL;
		}
		else
			return ResultCode::SUCCESS;
	}

	Result Addr2SockAddr(const NetAddress &addr, sockaddr_storage &sockAddr)
	{
		if (addr.SocketFamily == SockFamily::IPV6)
		{
			return Addr2SockAddr(addr, *(sockaddr_in6*)&sockAddr);
		}
		else
		{
			return Addr2SockAddr(addr, *(sockaddr_in*)&sockAddr);
		}
	}

	Result SetSockAddr(sockaddr_in6& sockAddr, const char *strAddr, USHORT usPort)
	{
		memset(&sockAddr, 0, sizeof(sockAddr));

		sockAddr.sin6_family = AF_INET6;
		sockAddr.sin6_port = htons(usPort);
		int result = inet_pton(sockAddr.sin6_family, strAddr, &sockAddr.sin6_addr);
		if (result != TRUE)
		{
			return ResultCode::FAIL;
		}

		return ResultCode::SUCCESS;
	}

	Result SetSockAddr(sockaddr_in& sockAddr, const char *strAddr, USHORT usPort)
	{
		memset(&sockAddr, 0, sizeof(sockAddr));

		sockAddr.sin_family = AF_INET;
		sockAddr.sin_port = htons(usPort);
		int result = inet_pton(sockAddr.sin_family, strAddr, &sockAddr.sin_addr);
		if (result != TRUE)
		{
			return ResultCode::FAIL;
		}

		return ResultCode::SUCCESS;
	}

	Result SetSockAddr(sockaddr_storage& sockAddr, const char *strAddr, USHORT usPort)
	{
		if (!(SetSockAddr(*(sockaddr_in6*)&sockAddr, strAddr, usPort)))
		{
			return SetSockAddr(*(sockaddr_in*)&sockAddr, strAddr, usPort);
		}
		return ResultCode::SUCCESS;
	}


	Result GetAnyBindAddr(const sockaddr_storage &sockAddr, sockaddr_storage&bindAddr)
	{
		bindAddr = sockAddr;
		if (bindAddr.ss_family == ToSockValue(SockFamily::IPV6))
		{
			auto bindSockaddr = (sockaddr_in6*)&bindAddr;
			bindSockaddr->sin6_addr = in6addr_any;
		}
		else
		{
			auto bindSockaddr = (sockaddr_in*)&bindAddr;
			bindSockaddr->sin_addr.s_addr = INADDR_ANY;
		}
		return ResultCode::SUCCESS;
	}

	Result SetLocalNetAddress(NetAddress &localAddr, const char *strLocalAddress, USHORT port)
	{
		StrUtil::StringCpy(localAddr.strAddr, strLocalAddress);
		localAddr.usPort = port;

		// validate local IP
		if (!(CheckLocalAddress(SockFamily::IPV6, localAddr)))
		{
			if (!(CheckLocalAddress(SockFamily::IPV4, localAddr)))
			{
				if ((GetLocalAddressIPv6(localAddr)))
				{
					if (CheckLocalAddress(SockFamily::IPV6, localAddr))
					{
						netTrace(Trace::TRC_ERROR, "Invalid Address, Using another address({0}) instead of given address", localAddr);
						localAddr.SocketFamily = SockFamily::IPV6;
					}
					else
					{
						netTrace(Trace::TRC_ERROR, "Invalid Address, expect a local IPV6 address");
						return ResultCode::E_NET_INVALID_ADDRESS;
					}
				}
				else
				{
					netTrace(Trace::TRC_ERROR, "Invalid Address, expect a local IPV6 address");
					return ResultCode::E_NET_INVALID_ADDRESS;
				}
			}
			else
			{
				localAddr.SocketFamily = SockFamily::IPV4;
			}
		}
		else
		{
			localAddr.SocketFamily = SockFamily::IPV6;
		}

		return ResultCode::SUCCESS;
	}

	Result SetNetAddress(NetAddress &netAddr, const char *strAddress, USHORT port)
	{
		sockaddr_storage sockAddr;

		Result hr = SetSockAddr(sockAddr, strAddress, port);
		if (!(hr))
			return hr;

		netAddr.SocketFamily = ToSockFamily(sockAddr.ss_family);
		hr = StrUtil::StringCpy(netAddr.strAddr, (INT)countof(netAddr.strAddr), strAddress);
		if (!(hr))
			return hr;

		netAddr.usPort = port;
		return ResultCode::SUCCESS;
	}


#if WINDOWS


	Result GetLocalAddress(SockFamily family, NetAddress &addr)
	{
		char tempBuffer[128];
		//Convert IPV6 to IPV4
		struct addrinfo hints, *res;
		bool bIsFound = false;

		// Convert remote address
		memset(&hints, 0, sizeof hints);
		hints.ai_family = ToSockValue(family);
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		auto error = getaddrinfo("", nullptr, &hints, &res);
		switch (error)
		{
		case 0:				break;
		case EAI_AGAIN:		return ResultCode::E_NET_TRY_AGAIN;
		case EAI_BADFLAGS:	return ResultCode::E_NET_BADFLAGS;
		case EAI_FAIL:		return ResultCode::FAIL;
		case EAI_FAMILY:	return ResultCode::E_NET_FAMILY;
		case EAI_MEMORY:	return ResultCode::OUT_OF_MEMORY;
		case EAI_NONAME:	return ResultCode::E_NET_HOST_NOT_FOUND;
		case EAI_SERVICE:	return ResultCode::E_NET_INVALID_SERVICE;
		case EAI_SOCKTYPE:	return ResultCode::E_NET_NOTSOCK;
		default:			return ResultCode::UNEXPECTED;
		}

		for (auto curAddr = res; curAddr != nullptr; curAddr = curAddr->ai_next)
		{
			if (curAddr->ai_family == AF_INET)
			{
				sockaddr_in* psockAddr4 = ((sockaddr_in*)curAddr->ai_addr);
				bIsFound = inet_ntop(AF_INET, &psockAddr4->sin_addr, tempBuffer, sizeof tempBuffer) != nullptr;
				break;
			}
			else if(curAddr->ai_family == AF_INET6)
			{
				sockaddr_in6* psockAddr6 = ((sockaddr_in6*)curAddr->ai_addr);
				if (!bIsFound || psockAddr6->sin6_scope_id == 0)
				{
					bIsFound = inet_ntop(AF_INET6, &psockAddr6->sin6_addr, tempBuffer, sizeof tempBuffer) != nullptr;
					if (bIsFound && psockAddr6->sin6_scope_id == 0)
						break;
				}
			}
		}
		freeaddrinfo(res);

		if (bIsFound)
		{
			addr.SocketFamily = family;
			StrUtil::StringCpy(addr.strAddr, tempBuffer);
		}

		return bIsFound ? ResultCode::SUCCESS : ResultCode::FAIL;
	}

	Result GetLocalAddressIPv4(NetAddress &addr)
	{
		return GetLocalAddress(SockFamily::IPV4, addr);
	}

	Result GetLocalAddressIPv6(NetAddress &addr)
	{
		return GetLocalAddress(SockFamily::IPV6, addr);
	}

	Result CheckLocalAddress(SockFamily family, NetAddress &addr)
	{
		//Convert IPV6
		struct addrinfo hints, *res;
		sockaddr_storage testSockAddr;
		bool bIsFound = false;

		addr.SocketFamily = family;
		Result hr = Addr2SockAddr(addr, testSockAddr);
		if (!(hr))
			return hr;


		switch (family)
		{
		case SockFamily::IPV4:
		{
			auto *pTestSockAddr = (struct sockaddr_in *)&testSockAddr;
			if (pTestSockAddr->sin_addr.s_addr == INADDR_ANY) return ResultCode::SUCCESS;
			break;
		}

		case SockFamily::IPV6:
		{
			auto *pTestSockAddr = (struct sockaddr_in6 *)&testSockAddr;
			auto& rawAddress = pTestSockAddr->sin6_addr.s6_addr;
			auto& rawAddressSrc = in6addr_any.s6_addr;
			int iAddr = 0;
			for (; iAddr < countof(rawAddress); iAddr++)
			{
				if (rawAddress[iAddr] != rawAddressSrc[iAddr]) break;
			}
			if (iAddr >= countof(rawAddress)) return ResultCode::SUCCESS;
			break;
		}
		}


		// Convert remote address
		memset(&hints, 0, sizeof hints);
		hints.ai_family = ToSockValue(family);
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_flags = AI_PASSIVE;
		auto error = getaddrinfo("", nullptr, &hints, &res);
		switch (error)
		{
		case 0:				break;
		case EAI_AGAIN:		return ResultCode::E_NET_TRY_AGAIN;
		case EAI_BADFLAGS:	return ResultCode::E_NET_BADFLAGS;
		case EAI_FAIL:		return ResultCode::FAIL;
		case EAI_FAMILY:	return ResultCode::E_NET_FAMILY;
		case EAI_MEMORY:	return ResultCode::OUT_OF_MEMORY;
		case EAI_NONAME:	return ResultCode::E_NET_HOST_NOT_FOUND;
		case EAI_SERVICE:	return ResultCode::E_NET_INVALID_SERVICE;
		case EAI_SOCKTYPE:	return ResultCode::E_NET_NOTSOCK;
		default:			return ResultCode::UNEXPECTED;
		}

		for (auto curAddr = res; curAddr != nullptr && !bIsFound; curAddr = curAddr->ai_next)
		{
			if (curAddr->ai_family != ToSockValue(family))
				continue;

			switch (family)
			{
			case SockFamily::IPV4:
			{
				auto *pTestSockAddr = (struct sockaddr_in *)&testSockAddr;
				struct sockaddr_in *psockAddr = (struct sockaddr_in *)curAddr->ai_addr;
				bIsFound = memcmp(&psockAddr->sin_addr, &pTestSockAddr->sin_addr, sizeof(pTestSockAddr->sin_addr)) == 0;
				break;
			}

			case SockFamily::IPV6:
			{
				auto *pTestSockAddr = (struct sockaddr_in6 *)&testSockAddr;
				struct sockaddr_in6 *psockAddr = (struct sockaddr_in6 *)curAddr->ai_addr;
				bIsFound = memcmp(&psockAddr->sin6_addr, &pTestSockAddr->sin6_addr, sizeof(pTestSockAddr->sin6_addr)) == 0;
				break;
			}

			default:
				continue;
			}

		}
		freeaddrinfo(res);

		return bIsFound ? ResultCode::SUCCESS : ResultCode::FAIL;
	}

#else


	Result GetLocalAddress(SockFamily family, NetAddress &addr)
	{
		struct ifaddrs *ifaddr = nullptr;
		char tempBuffer[NI_MAXHOST];
		bool bIsFound = false;

		if (getifaddrs(&ifaddr) == -1)
		{
			return GetLastWSAResult();
		}

		/* Walk through linked list, maintaining head pointer so we
		can free list later */
		int sockFamily = ToSockValue(family);
		for (struct ifaddrs *curAddr = ifaddr; curAddr != nullptr && !bIsFound; curAddr = curAddr->ifa_next)
		{
			if (curAddr->ifa_addr == nullptr)
				continue;

			if (!(curAddr->ifa_flags & IFF_UP))
				continue;

			if ((curAddr->ifa_flags & IFF_LOOPBACK))
				continue;

			if (curAddr->ifa_addr->sa_family != sockFamily)
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
				if (!bIsFound || psockAddr->sin6_scope_id == 0)
				{
					bIsFound = inet_ntop(AF_INET6, &psockAddr->sin6_addr, tempBuffer, sizeof tempBuffer) != nullptr;
					if (bIsFound && psockAddr->sin6_scope_id == 0)
						break;
				}
			}

			default:
				continue;
			}
		}

		freeifaddrs(ifaddr);

		if (bIsFound)
		{
			addr.SocketFamily = family;
			StrUtil::StringCpy(addr.strAddr, tempBuffer);
		}

		return bIsFound ? ResultCode::SUCCESS : ResultCode::FAIL;
	}

	Result GetLocalAddressIPv4(NetAddress &addr)
	{
		return GetLocalAddress(SockFamily::IPV4, addr);
	}

	Result GetLocalAddressIPv6(NetAddress &addr)
	{
		return GetLocalAddress(SockFamily::IPV6, addr);
	}

	Result CheckLocalAddress(SockFamily family, NetAddress &addr)
	{
		struct ifaddrs *ifaddr = nullptr;
		bool bIsFound = false;
		sockaddr_storage testSockAddr;

		// make sock addr
		memset(&testSockAddr, 0, sizeof testSockAddr);
		if (family == SockFamily::IPV4)
		{
			auto *pSockAddr = (struct sockaddr_in *)&testSockAddr;
			if (!(Addr2SockAddr(addr, *pSockAddr)))
				return ResultCode::FAIL;
		}
		else
		{
			auto *pSockAddr = (struct sockaddr_in6 *)&testSockAddr;
			if (!(Addr2SockAddr(addr, *pSockAddr)))
				return ResultCode::FAIL;
		}
			
		if (getifaddrs(&ifaddr) == -1)
		{
			return GetLastWSAResult();
		}

		/* Walk through linked list, maintaining head pointer so we
		can free list later */
		int sockFamily = ToSockValue(family);
		for (struct ifaddrs *curAddr = ifaddr; curAddr != nullptr && !bIsFound; curAddr = curAddr->ifa_next)
		{
			if (curAddr->ifa_addr == nullptr)
				continue;

			if (!(curAddr->ifa_flags & IFF_UP))
				continue;

			if ((curAddr->ifa_flags & IFF_LOOPBACK))
				continue;

			if (curAddr->ifa_addr->sa_family != sockFamily)
				continue;

			switch (sockFamily)
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

		return bIsFound ? ResultCode::SUCCESS : ResultCode::FAIL;

	}

#endif

	/////////////////////////////////////////////////////////////////////////////////
	//
	// Socket value
	//

	int ToSockValue(SockFamily family)
	{
		switch (family)
		{
		case SockFamily::IPV4: return AF_INET;
		case SockFamily::IPV6: return AF_INET6;
		default:
			return 0;
		}
	}

	int ToSockValue(SockType family)
	{
		switch (family)
		{
		case SockType::Stream: return SOCK_STREAM;
		case SockType::DataGram: return SOCK_DGRAM;
		default:
			return 0;
		}
	}

	SockFamily ToSockFamily(int family)
	{
		switch (family)
		{
		case AF_INET: return SockFamily::IPV4;
		case AF_INET6: return SockFamily::IPV6;
		default:
			return SockFamily::None;
		}
	}

	SockType ToSockType(int family)
	{
		switch (family)
		{
		case SOCK_STREAM: return SockType::Stream;
		default:
		case SOCK_DGRAM: return SockType::DataGram;
		}
	}


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

	bool operator == (const sockaddr_storage &op1, const sockaddr_storage &op2)
	{
		if (op1.ss_family != op2.ss_family)
			return false;

		if (op1.ss_family == AF_INET6)
		{
			auto op1SockAddr = (sockaddr_in6*)&op1;
			auto op2SockAddr = (sockaddr_in6*)&op2;
			return memcmp(&op1SockAddr->sin6_addr, &op2SockAddr->sin6_addr, sizeof op1SockAddr->sin6_addr) == 0
				&& op1SockAddr->sin6_port == op2SockAddr->sin6_port;
		}
		else
		{
			auto op1SockAddr = (sockaddr_in*)&op1;
			auto op2SockAddr = (sockaddr_in*)&op2;
			return op1SockAddr->sin_addr.s_addr == op2SockAddr->sin_addr.s_addr
				&& op1SockAddr->sin_port == op2SockAddr->sin_port;
		}
	}

	bool operator != (const sockaddr_storage &op1, const sockaddr_storage &op2)
	{
		if (op1.ss_family != op2.ss_family)
			return true;

		if (op1.ss_family == AF_INET6)
		{
			auto op1SockAddr = (sockaddr_in6*)&op1;
			auto op2SockAddr = (sockaddr_in6*)&op2;
			return memcmp(&op1SockAddr->sin6_addr, &op2SockAddr->sin6_addr, sizeof op1SockAddr->sin6_addr) != 0
				|| op1SockAddr->sin6_port != op2SockAddr->sin6_port;
		}
		else
		{
			auto op1SockAddr = (sockaddr_in*)&op1;
			auto op2SockAddr = (sockaddr_in*)&op2;
			return op1SockAddr->sin_addr.s_addr != op2SockAddr->sin_addr.s_addr
				|| op1SockAddr->sin_port != op2SockAddr->sin_port;
		}
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

		while (Dequeue(data))
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

		while (Dequeue(data))
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

	//Result WriteBufferQueueManager::ChooseQueue(WriteBufferQueue* &pQueue)
	//{
	//	auto newIndex = m_AssignIndex.fetch_add(1, std::memory_order_relaxed) % m_WriteBufferQueues.GetSize();
	//}

} // namespace Net
} // namespace BR

