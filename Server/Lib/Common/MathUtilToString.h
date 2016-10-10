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
	inline Result ToString( char*& pBuff, INT& iBuffLen, const Vector2& Data, int Option )
	{
		if( FAILED( StrUtil::StringCpyEx( pBuff, iBuffLen, "(" ) ) )
			return ResultCode::FAIL;

		if( FAILED( ToString( pBuff, iBuffLen, Data.x, Option ) ) )
			return ResultCode::FAIL;

		if( FAILED( StrUtil::StringCpyEx( pBuff, iBuffLen, "," ) ) )
			return ResultCode::FAIL;

		if( FAILED( ToString( pBuff, iBuffLen, Data.y, Option ) ) )
			return ResultCode::FAIL;

		if( FAILED( StrUtil::StringCpyEx( pBuff, iBuffLen, ")" ) ) )
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}

	





	
	template<>
	inline Result ToString( char*& pBuff, INT& iBuffLen, const Vector3& Data, int Option )
	{
		if( FAILED( StrUtil::StringCpyEx( pBuff, iBuffLen, "(" ) ) )
			return ResultCode::FAIL;

		if( FAILED( ToString( pBuff, iBuffLen, Data.x, Option ) ) )
			return ResultCode::FAIL;

		if( FAILED( StrUtil::StringCpyEx( pBuff, iBuffLen, "," ) ) )
			return ResultCode::FAIL;

		if( FAILED( ToString( pBuff, iBuffLen, Data.y, Option ) ) )
			return ResultCode::FAIL;

		if( FAILED( StrUtil::StringCpyEx( pBuff, iBuffLen, "," ) ) )
			return ResultCode::FAIL;
		
		if( FAILED( ToString( pBuff, iBuffLen, Data.z, Option ) ) )
			return ResultCode::FAIL;

		if( FAILED( StrUtil::StringCpyEx( pBuff, iBuffLen, ")" ) ) )
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}



}; // namespace BR



