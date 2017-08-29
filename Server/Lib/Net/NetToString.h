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

#include "Common/Typedefs.h"
#include "String/ToString.h"
#include "Net/NetDef.h"


namespace BR {
	
	template<> Result ToString(char*& pBuff, INT& iBuffLen, const Net::IConnection::ConnectionState& Data, int Option);
	template<> Result ToString(char*& pBuff, INT& iBuffLen, const sockaddr_in6& Data, int Option);
	template<> Result ToString(char*& pBuff, INT& iBuffLen, const sockaddr_storage& Data, int Option);
	template<> Result ToString(char*& pBuff, INT& iBuffLen, const Net::IConnection::ConnectionInformation& Data, int Option);
	template<> Result ToString(char*& pBuff, INT& iBuffLen, const NetClass& Data, int Option);
	

} // namespace BR


