////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : String Format utility
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/BrAssert.h"
#include "String/Argument.h"

namespace BR {
namespace StrUtil {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	String formating helper
	//

	// Format string with argument list
	// Modified buffer pointer and remain buffer size returned to original parameter
	Result Format_Internal( char*& szBuffer, INT& BuffLen, const char* szFormating, int iNumArg, Argument* Args );


	// 
	inline Result Format( char* szBuffer, INT& BuffLen, const char* strFormat )
	{
		return Format_Internal(szBuffer, BuffLen, strFormat, 0, nullptr);
	}

	// 
	template< class ...ArgTypes >
	inline Result Format(char* szBuffer, INT& BuffLen, const char* strFormat, ArgTypes... args)
	{
		Argument arguments[sizeof...(args)] = { Arg<ArgTypes>(args)... };
		return Format_Internal(szBuffer, BuffLen, strFormat, sizeof...(args), arguments);
	}

	// 
	template< int BuffLen >
	Result Format(char(&szBuffer)[BuffLen], const char* strFormat)
	{
		char* pTempBuff = szBuffer;
		INT BuffReamin = BuffLen ;
		return Format_Internal(pTempBuff, BuffReamin, strFormat, 0, nullptr);
	}

	template< int BuffLen, class ...ArgTypes >
	Result Format(char(&szBuffer)[BuffLen], const char* strFormat, ArgTypes... args)
	{
		Argument arguments[sizeof...(args)] = { Arg<ArgTypes>(args)... };

		char* pTempBuff = szBuffer;
		INT BuffReamin = BuffLen;
		return Format_Internal(pTempBuff, BuffReamin, strFormat, sizeof...(args), arguments);
	}


}; // namespace StrUtil
} // namespace BR


