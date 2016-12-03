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




template<INT iBuffLen>
Result StringCpy( char (&szDest)[iBuffLen], const char *szSrc )
{
	if( szSrc )
	{
		INT iLen = (INT)strlen(szSrc)+1;
		iLen = iBuffLen < iLen ? iBuffLen : iLen;

		if( szDest && iBuffLen > 0 )
		{
			memcpy( szDest, szSrc, iLen*sizeof(char) );
			if( iLen > 0 )
				szDest[iLen-1] = '\0';
		}
	}
	else
	{
		if( szDest && iBuffLen > 0 )
			szDest[0] = '\0';
	}

	return ResultCode::SUCCESS;
}

template<INT iBuffLen>
Result StringCpy( wchar_t (&szDest)[iBuffLen], const wchar_t *szSrc )
{
	if( szSrc )
	{
		INT iLen = (INT)wcslen(szSrc)+1;
		iLen = iBuffLen < iLen ? iBuffLen : iLen;

		if( szDest && iBuffLen > 0 )
		{
			memcpy( szDest, szSrc, iLen*sizeof(wchar_t) );
			if( iLen > 0 )
				szDest[iLen-1] = L'\0';
		}
	}
	else
	{
		if( szDest && iBuffLen > 0 )
			szDest[0] = L'\0';
	}

	return ResultCode::SUCCESS;
}


//
//template<INT iBuffLen>
//Result WCSToMBCS( const wchar_t* strWCS, char (&strMBCS)[iBuffLen] )
//{
//	return WCSToMBCS( strWCS, strMBCS, iBuffLen );
//}




template<INT iBuffLen>
Result WCSToUTF8( const wchar_t* strWCS, char (&strUTF8)[iBuffLen] )
{
	return WCSToUTF8( strWCS, strUTF8, iBuffLen );
}
//
//
//template<int iBuffLen>
//Result MBCSToWCS( const char *strMBCS, wchar_t (&strWCS)[iBuffLen] )
//{
//	return MBCSToWCS( strMBCS, strWCS, iBuffLen );
//}
//
//template<INT iBuffLen>
//Result MBCSToUTF8( const char *strMBCS, char (&strUTF8)[iBuffLen] )
//{
//	return MBCSToUTF8( strMBCS, strUTF8, iBuffLen );
//}

template<INT iBuffLen>
Result UTF8ToWCS( const char *strUTF8, wchar_t (&strWCS)[iBuffLen] )
{
	return UTF8ToWCS( strUTF8, strWCS, iBuffLen );
}
//
//template<INT iBuffLen>
//Result UTF8ToMBCS( const char *strUTF8, char (&strMBCS)[iBuffLen] )
//{
//	return UTF8ToMBCS( strUTF8, strMBCS, iBuffLen );
//}

template<INT iBuffLen>
Result UTF8ToUTF8Bin( BYTE (&UTF8BinBuff)[iBuffLen], const char* strSrc )
{
	UTF8Bin *pUTF8 = (UTF8Bin*)UTF8BinBuff;
	pUTF8->IsNull = 0;
	pUTF8->Length = (UINT32)strlen(strSrc);

	return StrUtil::StringCpy( (char*)pUTF8->stringData, iBuffLen - sizeof(UTF8Bin), strSrc );
}

Result UTF8BinToUTF8( char* strDst, INT dstLen, const BYTE* strSrc )
{
	Result hr = ResultCode::SUCCESS;

	UTF8Bin *pUTF8 = (UTF8Bin*)strSrc;

	if( pUTF8->IsNull )
		StringCpy( strDst, dstLen, "" );
	else
	{
		StringCpy( strDst, dstLen, pUTF8->stringData );
	}

	return hr;
}



// String compare
int StringCmp( const char* szSrc, INT iSrcBuffLen, const char* szDest, INT iDestBuffLen )
{
	if( szSrc == NULL && szDest == NULL )
		return 0;

	if( szSrc == NULL )
		return -1;

	if( szDest == NULL )
		return 1;

	if( iSrcBuffLen <= 0 )	iSrcBuffLen = (int)strlen( szSrc );
	if( iDestBuffLen <= 0 )	iDestBuffLen = (int)strlen( szDest );

	INT MaxTry = std::min(iSrcBuffLen, iDestBuffLen);

	return StringCmp( szSrc, szDest, MaxTry );
}

// String compare
int StringCmp( const char* szSrc, const char* szDest, INT MaxTry )
{
	if( szSrc == NULL && szDest == NULL )
		return 0;

	if( szSrc == NULL )
		return -1;

	if( szDest == NULL )
		return 1;


	for( INT iComp = 0; iComp < MaxTry && szSrc[0] != '\0' && szDest[0] != '\0'; iComp++ )
	{
		int iDiff = szSrc[0] - szDest[0];
		if( iDiff != 0 )
			return iDiff;

		szSrc++;
		szDest++;
	}

	if( szSrc[0] == '\0' && szDest[0] == '\0' )
		return 0;

	if( szSrc[0] == '\0' )
		return -1;

	if( szDest[0] == '\0' )
		return 1;

	return 0;
}

int StringCmp( const wchar_t* szSrc, INT iSrcBuffLen, const wchar_t* szDest, INT iDestBuffLen )
{
	if( szSrc == NULL && szDest == NULL )
		return 0;

	if( szSrc == NULL )
		return -1;

	if( szDest == NULL )
		return 1;

	if( iSrcBuffLen <= 0 )	iSrcBuffLen = (int)wcslen( szSrc );
	if( iDestBuffLen <= 0 )	iDestBuffLen = (int)wcslen( szDest );

	INT iCompLen = std::min(iSrcBuffLen, iDestBuffLen);

	for( INT iComp = 0; iComp < iCompLen && szSrc[0] != L'\0' && szDest[0] != L'\0'; iComp++ )
	{
		int iDiff = szSrc[0] - szDest[0];
		if( iDiff != 0 )
			return iDiff;

		szSrc++;
		szDest++;
	}

	if( szSrc[0] == L'\0' && szDest[0] == L'\0' )
		return 0;

	if( szSrc[0] == L'\0' )
		return -1;

	if( szDest[0] == L'\0' )
		return 1;

	return 0;
}

template<INT iSrcBuffLen, INT iDestBuffLen>
int StringCmp( char (&szSrc)[iSrcBuffLen], char (&szDest)[iDestBuffLen] )
{
	return StringCmp( szSrc, iSrcBuffLen, szDest, iDestBuffLen );
}

template<INT iDestBuffLen>
int StringCmp( const char* szSrc, INT iSrcBuffLen, char (&szDest)[iDestBuffLen] )
{
	return StringCmp( szSrc, iSrcBuffLen, szDest, iDestBuffLen );
}

template<INT iSrcBuffLen>
int StringCmp( char (&szSrc)[iSrcBuffLen], const char* szDest, INT iDestBuffLen )
{
	return StringCmp( szSrc, iSrcBuffLen, szDest, iDestBuffLen );
}


template<INT iSrcBuffLen, INT iDestBuffLen>
int StringCmp( wchar_t (&szSrc)[iSrcBuffLen], wchar_t (&szDest)[iDestBuffLen] )
{
	return StringCmp( szSrc, iSrcBuffLen, szDest, iDestBuffLen );
}

template<INT iDestBuffLen>
int StringCmp( const wchar_t* szSrc, INT iSrcBuffLen, wchar_t (&szDest)[iDestBuffLen] )
{
	return StringCmp( szSrc, iSrcBuffLen, szDest, iDestBuffLen );
}

template<INT iSrcBuffLen>
int StringCmp( wchar_t (&szSrc)[iSrcBuffLen], const wchar_t* szDest, INT iDestBuffLen )
{
	return StringCmp( szSrc, iSrcBuffLen, szDest, iDestBuffLen );
}




// String compare with lower case
int StringCmpLwr( const char* szSrc, INT iSrcBuffLen, const char* szDest, INT iDestBuffLen )
{
	if (szSrc == nullptr && szDest == nullptr)
		return 0;

	if (szSrc == nullptr)
		return -1;

	if (szDest == nullptr)
		return 1;

	if( iSrcBuffLen <= 0 )	iSrcBuffLen = (INT)strlen( szSrc );
	if( iDestBuffLen <= 0 )	iDestBuffLen = (INT)strlen( szDest );

	INT iCompLen = std::min(iSrcBuffLen, iDestBuffLen);

	for( INT iComp = 0; iComp < iCompLen && szSrc[0] != '\0' && szDest[0] != '\0'; iComp++ )
	{
		char src = (szSrc[0] >= 'A' && szSrc[0] <= 'Z') ? (szSrc[0] - 'A' + 'a') : szSrc[0];
		char dst = (szDest[0] >= 'A' && szDest[0] <= 'Z') ? (szDest[0] - 'A' + 'a') : szDest[0];
		int iDiff = src - dst;
		if( iDiff != 0 )
			return iDiff;

		szSrc++;
		szDest++;
	}

	if( szSrc[0] == '\0' && szDest[0] == '\0' )
		return 0;

	if( szSrc[0] == '\0' )
		return -1;

	if( szDest[0] == '\0' )
		return 1;

	return 0;
}

int StringCmpLwr( const wchar_t* szSrc, INT iSrcBuffLen, const wchar_t* szDest, INT iDestBuffLen )
{
	if (szSrc == nullptr && szDest == nullptr)
		return 0;

	if (szSrc == nullptr)
		return -1;

	if (szDest == nullptr)
		return 1;

	if( iSrcBuffLen <= 0 )	iSrcBuffLen = (INT)wcslen( szSrc );
	if( iDestBuffLen <= 0 )	iDestBuffLen = (INT)wcslen( szDest );

	INT iCompLen = std::min(iSrcBuffLen, iDestBuffLen);

	for( INT iComp = 0; iComp < iCompLen && szSrc[0] != L'\0' && szDest[0] != L'\0'; iComp++ )
	{
		wchar_t src = (szSrc[0] >= L'A' && szSrc[0] <= L'Z') ? (szSrc[0] - L'A' + L'a') : szSrc[0];
		wchar_t dst = (szDest[0] >= L'A' && szDest[0] <= L'Z') ? (szDest[0] - L'A' + L'a') : szDest[0];
		int iDiff = src - dst;
		if( iDiff != 0 )
			return iDiff;

		szSrc++;
		szDest++;
	}

	if( szSrc[0] == L'\0' && szDest[0] == L'\0' )
		return 0;

	if( szSrc[0] == L'\0' )
		return -1;

	if( szDest[0] == L'\0' )
		return 1;

	return 0;
}

template<INT iSrcBuffLen, INT iDestBuffLen>
int StringCmpLwr( char (&szSrc)[iSrcBuffLen], char (&szDest)[iDestBuffLen] )
{
	return StringCmpLwr( szSrc, iSrcBuffLen, szDest, iDestBuffLen );
}

template<INT iDestBuffLen>
int StringCmpLwr( const char* szSrc, INT iSrcBuffLen, char (&szDest)[iDestBuffLen] )
{
	return StringCmpLwr( szSrc, iSrcBuffLen, szDest, iDestBuffLen );
}

template<INT iSrcBuffLen>
int StringCmpLwr( char (&szSrc)[iSrcBuffLen], const char* szDest, INT iDestBuffLen )
{
	return StringCmpLwr( szSrc, iSrcBuffLen, szDest, iDestBuffLen );
}


template<INT iSrcBuffLen, INT iDestBuffLen>
int StringCmpLwr( wchar_t (&szSrc)[iSrcBuffLen], wchar_t (&szDest)[iDestBuffLen] )
{
	return StringCmpLwr( szSrc, iSrcBuffLen, szDest, iDestBuffLen );
}

template<INT iDestBuffLen>
int StringCmpLwr( const wchar_t* szSrc, INT iSrcBuffLen, wchar_t (&szDest)[iDestBuffLen] )
{
	return StringCmpLwr( szSrc, iSrcBuffLen, szDest, iDestBuffLen );
}

template<INT iSrcBuffLen>
int StringCmpLwr( wchar_t (&szSrc)[iSrcBuffLen], const wchar_t* szDest, INT iDestBuffLen )
{
	return StringCmpLwr( szSrc, iSrcBuffLen, szDest, iDestBuffLen );
}





// String convert to lower case 
// szSrc(In/Out)		: Source string to convert
Result StringLwr( char* szSrc, INT iBuffLen )
{
	if( szSrc == NULL )
		return ResultCode::INVALID_ARG;

	if( iBuffLen <= 0 ) iBuffLen = (INT)strlen(szSrc);

	for( INT iConv = 0; iConv < iBuffLen && szSrc[0] != '\0'; iConv++ )
	{
		if( szSrc[0] >= 'A' && szSrc[0] <= 'Z' )
			szSrc[0] = szSrc[0] - 'A' + 'a';

		szSrc++;
	}

	return ResultCode::SUCCESS;
}

Result StringLwr( wchar_t* szSrc, INT iBuffLen )
{
	if( szSrc == NULL )
		return ResultCode::INVALID_ARG;

	if( iBuffLen <= 0 ) iBuffLen = (INT)wcslen(szSrc);

	for( INT iConv = 0; iConv < iBuffLen && szSrc[0] != L'\0'; iConv++ )
	{
		if( szSrc[0] >= L'A' && szSrc[0] <= L'Z' )
			szSrc[0] = szSrc[0] - L'A' + L'a';

		szSrc++;
	}

	return ResultCode::SUCCESS;
}

template<INT iBuffLen>
Result StringLwr( char (&szSrc)[iBuffLen] )
{
	return StringLwr( szSrc, iBuffLen );
}

template<INT iBuffLen>
Result StringLwr( wchar_t (&szSrc)[iBuffLen] )
{
	return StringLwr( szSrc, iBuffLen );
}


// String convert to lower case with reallocation, if source string longer then testination buffer
// szSrc				: Source string to convert
// szDest(In/Out)		: Buffer pointer reference
// iBuffLen(In/Out)		: Destination buffer size. if buffer is small to take all source message, then reallocated
Result StringLwr( char* &szDest, INT &iBuffLen, const char* szSrc )
{
	Result hr = ResultCode::SUCCESS;

	if( szSrc == NULL )
		return ResultCode::INVALID_ARG;


	INT iSrcBuffLen = (INT)strlen( szSrc ) + 1;

	if( iBuffLen >= iSrcBuffLen )
	{
		hr = StringCpy( szDest, iBuffLen, szSrc );
		if( !(hr) ) return hr;
	}
	else
	{
		hr = StringDup( szDest, szSrc );
		if( !(hr) ) return hr;
		iBuffLen = iSrcBuffLen;
	}

	return StringLwr( szDest, iBuffLen );
}

Result StringLwr(wchar_t* &szDest, INT &iBuffLen, const wchar_t* szSrc)
{
	Result hr = ResultCode::SUCCESS;

	if (szSrc == NULL)
		return ResultCode::INVALID_ARG;


	INT iSrcBuffLen = (INT)wcslen(szSrc) + 1;

	if (iBuffLen >= iSrcBuffLen)
	{
		hr = StringCpy(szDest, iBuffLen, szSrc);
		if (!(hr)) return hr;
	}
	else
	{
		hr = StringDup(szDest, szSrc);
		if (!(hr)) return hr;
		iBuffLen = iSrcBuffLen;
	}

	return StringLwr(szDest, iBuffLen);
}


Result StringLwrEx(char* &szDest, INT &iBuffLen, const char* szSrc)
{
	Result hr = ResultCode::SUCCESS;

	if (szSrc == NULL)
		return ResultCode::INVALID_ARG;


	hr = StringCpyEx(szDest, iBuffLen, szSrc);
	if (!(hr)) return hr;

	return StringLwr(szDest, iBuffLen);
}

Result StringLwrEx(wchar_t* &szDest, INT &iBuffLen, const wchar_t* szSrc)
{
	Result hr = ResultCode::SUCCESS;

	if (szSrc == NULL)
		return ResultCode::INVALID_ARG;


	hr = StringCpyEx(szDest, iBuffLen, szSrc);
	if (!(hr)) return hr;

	return StringLwr(szDest, iBuffLen);
}

// String convert to lower case with truncate, if source string longer then testination buffer
// szSrc				: Source string to convert
// szDest(In)			: Buffer pointer to copy
template<INT iBuffLen>
Result StringLwr( char (&szDest)[iBuffLen], const char *szSrc )
{
	Result hr = ResultCode::SUCCESS;

	if( szSrc == NULL )
		return ResultCode::INVALID_ARG;


	INT iSrcBuffLen = (INT)strlen( szSrc ) + 1;
	INT iConvLen = std::min(iBuffLen,iSrcBuffLen);

	if( iBuffLen >= iSrcBuffLen )
	{
		hr = StringCpy( szDest, iConvLen, szSrc );
		if( !(hr) ) return hr;
	}

	return StringLwr( szDest, iConvLen );
}

template<INT iBuffLen>
Result StringLwr( wchar_t (&wszDest)[iBuffLen], const wchar_t *wszSrc )
{
	Result hr = ResultCode::SUCCESS;

	if(wszSrc == NULL )
		return ResultCode::INVALID_ARG;


	INT iSrcBuffLen = (INT)wcslen(wszSrc) + 1;
	INT iConvLen = std::min(iBuffLen,iSrcBuffLen);

	if( iBuffLen >= iSrcBuffLen )
	{
		hr = StringCpy(wszDest, iConvLen, wszSrc);
		if( !(hr) ) return hr;
	}

	return StringLwr(wszDest, iConvLen );
}





// String convert to lower case 
// szSrc(In/Out)		: Source string to convert
inline Result StringUpr( char* szSrc, INT iBuffLen )
{
	if( szSrc == NULL )
		return ResultCode::INVALID_ARG;

	if( iBuffLen <= 0 ) iBuffLen = (INT)strlen(szSrc);

	for( INT iConv = 0; iConv < iBuffLen && szSrc[0] != '\0'; iConv++ )
	{
		if( szSrc[0] >= 'a' && szSrc[0] <= 'z' )
			szSrc[0] = szSrc[0] - 'a' + 'A';

		szSrc++;
	}

	return ResultCode::SUCCESS;
}

inline Result StringUpr( wchar_t* szSrc, INT iBuffLen )
{
	if( szSrc == NULL )
		return ResultCode::INVALID_ARG;

	if( iBuffLen <= 0 ) iBuffLen = (INT)wcslen(szSrc);

	for( INT iConv = 0; iConv < iBuffLen && szSrc[0] != L'\0'; iConv++ )
	{
		if( szSrc[0] >= 'a' && szSrc[0] <= 'z' )
			szSrc[0] = szSrc[0] - 'a' + 'A';

		szSrc++;
	}

	return ResultCode::SUCCESS;
}

template<INT iBuffLen>
inline Result StringUpr( char (&szSrc)[iBuffLen] )
{
	return StringUpr( szSrc, iBuffLen );
}

template<INT iBuffLen>
inline Result StringUpr( wchar_t (&szSrc)[iBuffLen] )
{
	return StringUpr( szSrc, iBuffLen );
}


// String convert to lower case with reallocation, if source string longer then testination buffer
// szSrc				: Source string to convert
// szDest(In/Out)		: Buffer pointer reference
// iBuffLen(In/Out)		: Destination buffer size. if buffer is small to take all source message, then reallocated
inline Result StringUpr( char* &szDest, INT &iBuffLen, const char* szSrc )
{
	Result hr = ResultCode::SUCCESS;

	if( szSrc == NULL )
		return ResultCode::INVALID_ARG;


	INT iSrcBuffLen = (INT)strlen( szSrc ) + 1;

	if( iBuffLen >= iSrcBuffLen )
	{
		hr = StringCpy( szDest, iBuffLen, szSrc );
		if( !(hr) ) return hr;
	}
	else
	{
		hr = StringDup( szDest, szSrc );
		if( !(hr) ) return hr;
		iBuffLen = iSrcBuffLen;
	}

	return StringUpr( szDest, iBuffLen );
}

inline Result StringUpr( wchar_t* &szDest, INT &iBuffLen, const wchar_t* szSrc )
{
	Result hr = ResultCode::SUCCESS;

	if( szSrc == NULL )
		return ResultCode::INVALID_ARG;


	INT iSrcBuffLen = (INT)wcslen( szSrc ) + 1;

	if( iBuffLen >= iSrcBuffLen )
	{
		hr = StringCpy( szDest, iBuffLen, szSrc );
		if( !(hr) ) return hr;
	}
	else
	{
		hr = StringDup( szDest, szSrc );
		if( !(hr) ) return hr;
		iBuffLen = iSrcBuffLen;
	}

	return StringUpr( szDest, iBuffLen );
}

// String convert to lower case with truncate, if source string longer then testination buffer
// szSrc				: Source string to convert
// szDest(In)			: Buffer pointer to copy
template<INT iBuffLen>
inline Result StringUpr( char (&szDest)[iBuffLen], const char *szSrc )
{
	Result hr = ResultCode::SUCCESS;

	if( szSrc == NULL )
		return ResultCode::INVALID_ARG;


	INT iSrcBuffLen = (INT)strlen( szSrc ) + 1;
	INT iConvLen = std::min(iBuffLen,iSrcBuffLen);

	if( iBuffLen >= iSrcBuffLen )
	{
		hr = StringCpy( szDest, iConvLen, szSrc );
		if( !(hr) ) return hr;
	}

	return StringUpr( szDest, iConvLen );
}

template<INT iBuffLen>
inline Result StringUpr( wchar_t (&wszDest)[iBuffLen], const wchar_t *wszSrc )
{
	Result hr = ResultCode::SUCCESS;

	if(wszSrc == NULL )
		return ResultCode::INVALID_ARG;


	INT iSrcBuffLen = (INT)wcslen(wszSrc) + 1;
	INT iConvLen = std::min(iBuffLen,iSrcBuffLen);

	if( iBuffLen >= iSrcBuffLen )
	{
		hr = StringCpy(wszDest, iConvLen, wszSrc);
		if( !(hr) ) return hr;
	}

	return StringUpr(wszDest, iConvLen );
}


inline bool IsNullOrEmpty(const char* szStr)
{
	return szStr == nullptr || szStr[0] == '\0';
}

inline bool IsNullOrEmpty(const wchar_t* szStr)
{
	return szStr == nullptr || szStr[0] == L'\0';
}



// Free string with NULL check
inline void SafeDelete( char* &szStr )
{
	if( szStr ) delete[] szStr;
}

inline void SafeDelete( wchar_t* &szStr )
{
	if( szStr ) delete[] szStr;
}









