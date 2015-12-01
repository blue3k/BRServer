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




namespace BR {
namespace Net {

	static_assert(INET6_ADDRSTRLEN <= NetAddress::MAX_NETNAME, "Net address length must be same with client");

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network Utility functions
	//
	void SockAddr2Addr(const sockaddr_in6 &sockAddr, NetAddress &addr)
	{
		//char ipstr[INET6_ADDRSTRLEN] = "";
		addr.strAddr[0] = '\0';
		inet_ntop(sockAddr.sin6_family, (void*)&sockAddr.sin6_addr, addr.strAddr, sizeof addr.strAddr);

		addr.usPort = ntohs(sockAddr.sin6_port);
	}

	void Addr2SockAddr(const NetAddress &addr, sockaddr_in6 &sockAddr)
	{
		memset(&sockAddr, 0, sizeof(sockAddr));
		sockAddr.sin6_family = AF_INET6;
		sockAddr.sin6_port = htons(addr.usPort);
		int result = inet_pton(sockAddr.sin6_family, addr.strAddr, &sockAddr.sin6_addr);
		if (result != TRUE)
		{
			sockAddr.sin6_family = AF_INET;
			result = inet_pton(sockAddr.sin6_family, addr.strAddr, &sockAddr.sin6_addr);
			Assert(result == TRUE);
		}
	}

	void SetSockAddr(sockaddr_in6& sockAddr, const char *strAddr, USHORT usPort)
	{
		memset(&sockAddr, 0, sizeof(sockAddr));

		sockAddr.sin6_family = AF_INET6;
		sockAddr.sin6_port = htons(usPort);
		int result = inet_pton(sockAddr.sin6_family, strAddr, &sockAddr.sin6_addr);
		if (result != TRUE)
		{
			sockAddr.sin6_family = AF_INET;
			result = inet_pton(sockAddr.sin6_family, strAddr, &sockAddr.sin6_addr);
			Assert(strAddr == nullptr || strAddr[0] == '\0' || result == TRUE);
		}
	}


	bool GetLocalAddress(NetAddress &addr)
	{
		char tempBuffer[128];
		//Convert IPV6 to IPV4
		struct addrinfo hints, *res;
		bool bIsFound = false, bIsFound6 = false;

		// Convert remote address
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		auto error = getaddrinfo("", nullptr, &hints, &res);
		if (error != 0)
			return false;

		for (auto curAddr = res; curAddr != nullptr; curAddr = curAddr->ai_next)
		{
			switch (curAddr->ai_family)
			{
			case AF_INET:
			{
				sockaddr_in* psockAddr4 = ((sockaddr_in*)curAddr->ai_addr);
				bIsFound = inet_ntop(AF_INET, &psockAddr4->sin_addr, tempBuffer, sizeof tempBuffer) != nullptr;
			}
			break;
			case AF_INET6:
			{
				sockaddr_in6* psockAddr6 = ((sockaddr_in6*)curAddr->ai_addr);
				bIsFound6 = inet_ntop(AF_INET, &psockAddr6->sin6_addr, tempBuffer, sizeof tempBuffer) != nullptr;
			}
			break;
			}

			if (bIsFound6) break;
		}
		freeaddrinfo(res);

		if (bIsFound || bIsFound6)
		{
			StrUtil::StringCpy(addr.strAddr, tempBuffer);
			return true;
		}

		return false;
	}

	bool GetLocalAddressIPv4(NetAddress &addr)
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
		if (error != 0)
			return false;

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

		return bIsFound;
	}

	bool GetLocalAddressIPv6(NetAddress &addr)
	{
		char tempBuffer[128];
		//Convert IPV6
		struct addrinfo hints, *res;
		bool bIsFound = false;

		// Convert remote address
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_INET6;  // use IPv6
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_flags = AI_PASSIVE;
		auto error = getaddrinfo("", nullptr, &hints, &res);
		if (error != 0)
			return false;

		for (auto curAddr = res; curAddr != nullptr; curAddr = curAddr->ai_next)
		{
			if (curAddr->ai_family == AF_INET6)
			{
				sockaddr_in6* psockAddr6 = ((sockaddr_in6*)curAddr->ai_addr);
				if (inet_ntop(AF_INET6, &psockAddr6->sin6_addr, tempBuffer, sizeof tempBuffer) != nullptr)
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

		return bIsFound;
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

	bool operator == (const NetAddress &op1, const NetAddress &op2)
	{
		return strncmp(op1.strAddr, op2.strAddr, 16) == 0 && op1.usPort == op2.usPort;
	}

	bool operator != (const NetAddress &op1, const NetAddress &op2)
	{
		return strncmp(op1.strAddr, op2.strAddr, 16) != 0 || op1.usPort != op2.usPort;
	}





} // namespace Net
} // namespace BR

