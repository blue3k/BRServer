////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : String manipulation utility
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include "Common/StrUtil.h"
#include "Common/ToString.h"
#include "Common/Argument.h"
#include "Common/StrFormat.h"

namespace BR {
namespace StrUtil {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	String formating helper
	//

	static inline int ChToInt( char ch )
	{
		register int iChar = ch;
		iChar -= '0';
		if( iChar >= 0 && iChar < 10 )
			return iChar;

		return -1;
	}
	
	// Internal format routine
	HRESULT Format_Internal( char*& szBuffer, INT& iBuffLen, const char* szFormating, int iNumArg, BR::Argument* Args )
	{
		if( szBuffer == NULL || iBuffLen == 0 )
			return E_POINTER;

		register char curChar;

		for( curChar = *szFormating++; curChar && iBuffLen > 0; curChar = *szFormating++ )
		{
			if( curChar == (char)'%' )
			{
				int iArg = 0;
				for( curChar = *szFormating++; curChar && curChar != '%'; curChar = *szFormating++ )
				{
					if( curChar < '0' || curChar > '9' ) 
						continue;

					register int iArgTem = ChToInt( curChar );
					if( iArgTem >= 0 )
						iArg = iArg*10 + iArgTem;
				}

				if( curChar == '\0' )
					return E_INVALIDARG;

				if( iArg < iNumArg )
					Args[iArg].MakeString( szBuffer, iBuffLen );
				else
				{
					StrUtil::StringCpyEx( szBuffer, iBuffLen, "(Null)" );
				}
			}
			else
			{
				*szBuffer++ = curChar;
				iBuffLen--;
			}
		}

		// Force Null terminate
		if( iBuffLen == 0 )
			(szBuffer-1)[0] = 0;
		else
			szBuffer[0] = 0;

		return S_OK;
	}


/*
	void Format( char* szBuffer, int iBuffLen, const char* szFormating, ... )
	{
		va_list vaArgs;
		va_start( vaArgs, szFormating );
		FormatMessageA(FORMAT_MESSAGE_FROM_STRING, szFormating, 0, 0, szBuffer, iBuffLen, (va_list *) &vaArgs);
		va_end( vaArgs );
	}

	void Format( wchar_t* wszBuffer, int iBuffLen, const wchar_t* wszFormating, ... )
	{
		va_list vaArgs;
		va_start( vaArgs, wszFormating );
		FormatMessageW(FORMAT_MESSAGE_FROM_STRING, wszFormating, 0, 0, wszBuffer, iBuffLen, (va_list *) &vaArgs);
		va_end( vaArgs );
	}
*/

	
	/////////////////////////////////////////////////////////////////////////////
	//
	//	Helper data
	//

	const char* EmptyString = "";

};	// namespace Svr
} // namespace BR

