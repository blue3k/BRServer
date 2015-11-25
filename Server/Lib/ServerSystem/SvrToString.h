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
	inline HRESULT ToString( char*& pBuff, INT& iBuffLen, const TransactionID& Data, int Option )
	{
		if( FAILED( ToString( pBuff, iBuffLen, Data.GetEntityID(), Option ) ) )
			return E_FAIL;

		if( FAILED( StrUtil::StringCpyEx( pBuff, iBuffLen, ":" ) ) )
			return E_FAIL;

		if( FAILED( _IToA( (UINT32)Data.GetTransactionIndex(), pBuff, iBuffLen, 10, -1 ) ) )
			return E_FAIL;

		return S_OK;
	}


	template<>
	inline HRESULT ToString( char*& pBuff, INT& iBuffLen, const BR::EntityState& Data, int Option )
	{
		return BR::ToStringEnum( pBuff, iBuffLen, Data );
	}

}; // namespace BR



