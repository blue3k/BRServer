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

#include "SFTypedefs.h"
#include "BrSvrTypes.h"
#include "ServerSystem/SvrTypes.h"
#include "String/ToStringSvr.h"


namespace SF {
	



	inline Result ToString( char*& pBuff, INT& iBuffLen, const BR::EntityState& Data, int Option )
	{
		return ToStringEnum( pBuff, iBuffLen, Data );
	}

}; // namespace BR



