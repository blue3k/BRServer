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



namespace BR {
namespace Net {



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network Utility functions
	//
	void SockAddr2Addr( const sockaddr_in6 &sockAddr, NetAddress &addr );
	void Addr2SockAddr( const NetAddress &addr, sockaddr_in6 &sockAddr );
	void SetSockAddr( sockaddr_in6& sockAddr, const char *strAddr, USHORT usPort );
	bool GetLocalAddressIPv4(NetAddress &addr);
	bool GetLocalAddressIPv6(NetAddress &addr);

	bool operator == ( const sockaddr_in6 &op1, const sockaddr_in6 &op2 );
	bool operator != ( const sockaddr_in6 &op1, const sockaddr_in6 &op2 );

	bool operator == ( const NetAddress &op1, const NetAddress &op2 );
	bool operator != ( const NetAddress &op1, const NetAddress &op2 );




} // namespace Net
} // namespace BR


