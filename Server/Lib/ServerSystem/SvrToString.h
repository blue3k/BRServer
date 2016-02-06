////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : KyungKun Ko
//
// Description : Server system type tostring 
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/BrSvrTypes.h"
#include "ServerSystem/SvrTypes.h"
#include "Common/ToStringSvr.h"


namespace BR {
	



	template<>
	inline HRESULT ToString( char*& pBuff, INT& iBuffLen, const EntityState& Data, int Option )
	{
		return ToStringEnum( pBuff, iBuffLen, Data );
	}

}; // namespace BR



