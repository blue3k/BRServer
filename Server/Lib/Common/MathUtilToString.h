////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Brave Game Math types
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/ToStringBase.h"
#include "Common/MathUtil.h"


namespace BR
{

	template<>
	inline HRESULT ToString( char*& pBuff, INT& iBuffLen, const Vector2& Data, int Option )
	{
		if( FAILED( StrUtil::StringCpyEx( pBuff, iBuffLen, "(" ) ) )
			return E_SYSTEM_FAIL;

		if( FAILED( ToString( pBuff, iBuffLen, Data.x, Option ) ) )
			return E_SYSTEM_FAIL;

		if( FAILED( StrUtil::StringCpyEx( pBuff, iBuffLen, "," ) ) )
			return E_SYSTEM_FAIL;

		if( FAILED( ToString( pBuff, iBuffLen, Data.y, Option ) ) )
			return E_SYSTEM_FAIL;

		if( FAILED( StrUtil::StringCpyEx( pBuff, iBuffLen, ")" ) ) )
			return E_SYSTEM_FAIL;

		return S_SYSTEM_OK;
	}

	





	
	template<>
	inline HRESULT ToString( char*& pBuff, INT& iBuffLen, const Vector3& Data, int Option )
	{
		if( FAILED( StrUtil::StringCpyEx( pBuff, iBuffLen, "(" ) ) )
			return E_SYSTEM_FAIL;

		if( FAILED( ToString( pBuff, iBuffLen, Data.x, Option ) ) )
			return E_SYSTEM_FAIL;

		if( FAILED( StrUtil::StringCpyEx( pBuff, iBuffLen, "," ) ) )
			return E_SYSTEM_FAIL;

		if( FAILED( ToString( pBuff, iBuffLen, Data.y, Option ) ) )
			return E_SYSTEM_FAIL;

		if( FAILED( StrUtil::StringCpyEx( pBuff, iBuffLen, "," ) ) )
			return E_SYSTEM_FAIL;
		
		if( FAILED( ToString( pBuff, iBuffLen, Data.z, Option ) ) )
			return E_SYSTEM_FAIL;

		if( FAILED( StrUtil::StringCpyEx( pBuff, iBuffLen, ")" ) ) )
			return E_SYSTEM_FAIL;

		return S_SYSTEM_OK;
	}



}; // namespace BR



