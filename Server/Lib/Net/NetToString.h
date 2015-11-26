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
#include "Common/ToString.h"
#include "Net/NetDef.h"


namespace BR {
	
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const Net::IConnection::ConnectionState& Data, int Option);

	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const sockaddr_in6& Data, int Option);
	
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const Net::IConnection::ConnectionInformation& Data, int Option);
		
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const NetClass& Data, int Option);
	

} // namespace BR


