////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : KyungKun Ko
//
// Description : Network type tostring 
//	
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "String/ToString.h"
#include "Net/NetDef.h"


namespace SF {
	
	Result ToString(char*& pBuff, INT& iBuffLen, const Net::IConnection::ConnectionState& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const sockaddr_in6& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const sockaddr_storage& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const Net::IConnection::ConnectionInformation& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const NetClass& Data, int Option);
	

} // namespace SF


