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




// String duplication, szDest will destroyded if exist, and new memory will be allocated
HRESULT StringDup( char* &szDest, const char* szSrc )
{
	if( szDest != NULL )
	{
		delete[] szDest;
		szDest = NULL;
	}

	if( szSrc )
	{
		INT iLen = (INT)strlen(szSrc)+1;
		szDest = new char[ iLen ];
		if( szDest == NULL )
			return E_OUTOFMEMORY;

		memcpy( szDest, szSrc, iLen*sizeof(char) );
	}

	return S_OK;
}

HRESULT StringDup( WCHAR* &szDest, const WCHAR* szSrc )
{
	if( szDest != NULL )
	{
		delete[] szDest;
		szDest = NULL;
	}

	if( szSrc )
	{
		INT iLen = (INT)wcslen(szSrc)+1;
		szDest = new WCHAR[ iLen ];
		if( szDest == NULL )
			return E_OUTOFMEMORY;

		memcpy( szDest, szSrc, iLen*sizeof(WCHAR) );
	}

	return S_OK;
}


// String copy to szDest. if szDest is NULL, operation failed
HRESULT StringCpyEx( char* &szDest, INT& iBuffLen, const char* szSrc )
{
	if( szSrc && szDest )
	{
		register char curChar;
		while( iBuffLen > 0 && (curChar = *szSrc++) != 0 )
		{
			*szDest++ = curChar;
			iBuffLen--;
		}

		if( iBuffLen == 0 )// force null terminate
			*(szDest -1) = '\0';
		else
			*(szDest) = '\0';
	}
	else
	{
		if( szDest && iBuffLen > 0 )
			szDest[0] = '\0';
	}

	return S_OK;
}

HRESULT StringCpy( char* szDest, INT iBuffLen, const char* szSrc )
{
	if( szSrc && szDest )
	{
		register char curChar;
		while( iBuffLen > 0 && (curChar = *szSrc++) != 0 )
		{
			*szDest++ = curChar;
			iBuffLen--;
		}

		if( iBuffLen == 0 )// force null terminate
			*(szDest -1) = '\0';
		else
			szDest[0] = '\0';
	}
	else
	{
		if( szDest && iBuffLen > 0 )
			szDest[0] = '\0';
	}

	return S_OK;
}

HRESULT StringCpy( WCHAR* szDest, INT iBuffLen, const WCHAR* szSrc )
{
	if( szSrc )
	{
		register WCHAR curChar;
		while( iBuffLen > 0 && (curChar = *szSrc++) != 0 )
		{
			*szDest++ = curChar;
			iBuffLen--;
		}

		if( iBuffLen == 0 )// force null terminate
			*(szDest -1) = '\0';
		else
			szDest[0] = '\0';
	}
	else
	{
		if( szDest && iBuffLen > 0 )
			szDest[0] = L'\0';
	}

	return S_OK;
}

template<INT iBuffLen>
HRESULT StringCpy( char (&szDest)[iBuffLen], const char *szSrc )
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

	return S_OK;
}

template<INT iBuffLen>
HRESULT StringCpy( WCHAR (&szDest)[iBuffLen], const WCHAR *szSrc )
{
	if( szSrc )
	{
		INT iLen = (INT)wcslen(szSrc)+1;
		iLen = iBuffLen < iLen ? iBuffLen : iLen;

		if( szDest && iBuffLen > 0 )
		{
			memcpy( szDest, szSrc, iLen*sizeof(WCHAR) );
			if( iLen > 0 )
				szDest[iLen-1] = L'\0';
		}
	}
	else
	{
		if( szDest && iBuffLen > 0 )
			szDest[0] = L'\0';
	}

	return S_OK;
}



template<INT iBuffLen>
HRESULT WCSToMBCS( const WCHAR* strWCS, char (&strMBCS)[iBuffLen] )
{
	return WCSToMBCS( strWCS, strMBCS, iBuffLen );
}




template<INT iBuffLen>
HRESULT WCSToUTF8( const WCHAR* strWCS, char (&strUTF8)[iBuffLen] )
{
	return WCSToUTF8( strWCS, strUTF8, iBuffLen );
}


template<int iBuffLen>
HRESULT MBCSToWCS( const char *strMBCS, WCHAR (&strWCS)[iBuffLen] )
{
	return MBCSToWCS( strMBCS, strWCS, iBuffLen );
}

template<INT iBuffLen>
HRESULT MBCSToUTF8( const char *strMBCS, char (&strUTF8)[iBuffLen] )
{
	return MBCSToUTF8( strMBCS, strUTF8, iBuffLen );
}

template<INT iBuffLen>
HRESULT UTF8ToWCS( const char *strUTF8, WCHAR (&strWCS)[iBuffLen] )
{
	return UTF8ToWCS( strUTF8, strWCS, iBuffLen );
}

template<INT iBuffLen>
HRESULT UTF8ToMBCS( const char *strUTF8, char (&strMBCS)[iBuffLen] )
{
	return UTF8ToMBCS( strUTF8, strMBCS, iBuffLen );
}

template<INT iBuffLen>
HRESULT UTF8ToUTF8Bin( BYTE (&UTF8BinBuff)[iBuffLen], const char* strSrc )
{
	UTF8Bin *pUTF8 = (UTF8Bin*)UTF8BinBuff;
	pUTF8->IsNull = 0;
	pUTF8->Length = (UINT32)strlen(strSrc);

	return StrUtil::StringCpy( (char*)pUTF8->stringData, iBuffLen - sizeof(UTF8Bin), strSrc );
}

HRESULT UTF8BinToUTF8( char* strDst, INT dstLen, const BYTE* strSrc )
{
	HRESULT hr = S_OK;

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

int StringCmp( const WCHAR* szSrc, INT iSrcBuffLen, const WCHAR* szDest, INT iDestBuffLen )
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
int StringCmp( WCHAR (&szSrc)[iSrcBuffLen], WCHAR (&szDest)[iDestBuffLen] )
{
	return StringCmp( szSrc, iSrcBuffLen, szDest, iDestBuffLen );
}

template<INT iDestBuffLen>
int StringCmp( const WCHAR* szSrc, INT iSrcBuffLen, WCHAR (&szDest)[iDestBuffLen] )
{
	return StringCmp( szSrc, iSrcBuffLen, szDest, iDestBuffLen );
}

template<INT iSrcBuffLen>
int StringCmp( WCHAR (&szSrc)[iSrcBuffLen], const WCHAR* szDest, INT iDestBuffLen )
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

int StringCmpLwr( const WCHAR* szSrc, INT iSrcBuffLen, const WCHAR* szDest, INT iDestBuffLen )
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
		WCHAR src = (szSrc[0] >= L'A' && szSrc[0] <= L'Z') ? (szSrc[0] - L'A' + L'a') : szSrc[0];
		WCHAR dst = (szDest[0] >= L'A' && szDest[0] <= L'Z') ? (szDest[0] - L'A' + L'a') : szDest[0];
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
int StringCmpLwr( WCHAR (&szSrc)[iSrcBuffLen], WCHAR (&szDest)[iDestBuffLen] )
{
	return StringCmpLwr( szSrc, iSrcBuffLen, szDest, iDestBuffLen );
}

template<INT iDestBuffLen>
int StringCmpLwr( const WCHAR* szSrc, INT iSrcBuffLen, WCHAR (&szDest)[iDestBuffLen] )
{
	return StringCmpLwr( szSrc, iSrcBuffLen, szDest, iDestBuffLen );
}

template<INT iSrcBuffLen>
int StringCmpLwr( WCHAR (&szSrc)[iSrcBuffLen], const WCHAR* szDest, INT iDestBuffLen )
{
	return StringCmpLwr( szSrc, iSrcBuffLen, szDest, iDestBuffLen );
}





// String convert to lower case 
// szSrc(In/Out)		: Source string to convert
HRESULT StringLwr( char* szSrc, INT iBuffLen )
{
	if( szSrc == NULL )
		return E_INVALIDARG;

	if( iBuffLen <= 0 ) iBuffLen = (INT)strlen(szSrc);

	for( INT iConv = 0; iConv < iBuffLen && szSrc[0] != '\0'; iConv++ )
	{
		if( szSrc[0] >= 'A' && szSrc[0] <= 'Z' )
			szSrc[0] = szSrc[0] - 'A' + 'a';

		szSrc++;
	}

	return S_OK;
}

HRESULT StringLwr( WCHAR* szSrc, INT iBuffLen )
{
	if( szSrc == NULL )
		return E_INVALIDARG;

	if( iBuffLen <= 0 ) iBuffLen = (INT)wcslen(szSrc);

	for( INT iConv = 0; iConv < iBuffLen && szSrc[0] != L'\0'; iConv++ )
	{
		if( szSrc[0] >= L'A' && szSrc[0] <= L'Z' )
			szSrc[0] = szSrc[0] - L'A' + L'a';

		szSrc++;
	}

	return S_OK;
}

template<INT iBuffLen>
HRESULT StringLwr( char (&szSrc)[iBuffLen] )
{
	return StringLwr( szSrc, iBuffLen );
}

template<INT iBuffLen>
HRESULT StringLwr( WCHAR (&szSrc)[iBuffLen] )
{
	return StringLwr( szSrc, iBuffLen );
}


// String convert to lower case with reallocation, if source string longer then testination buffer
// szSrc				: Source string to convert
// szDest(In/Out)		: Buffer pointer reference
// iBuffLen(In/Out)		: Destination buffer size. if buffer is small to take all source message, then reallocated
HRESULT StringLwr( char* &szDest, INT &iBuffLen, const char* szSrc )
{
	HRESULT hr = S_OK;

	if( szSrc == NULL )
		return E_INVALIDARG;


	INT iSrcBuffLen = (INT)strlen( szSrc ) + 1;

	if( iBuffLen >= iSrcBuffLen )
	{
		hr = StringCpy( szDest, iBuffLen, szSrc );
		if( FAILED(hr) ) return hr;
	}
	else
	{
		hr = StringDup( szDest, szSrc );
		if( FAILED(hr) ) return hr;
		iBuffLen = iSrcBuffLen;
	}

	return StringLwr( szDest, iBuffLen );
}

HRESULT StringLwr(WCHAR* &szDest, INT &iBuffLen, const WCHAR* szSrc)
{
	HRESULT hr = S_OK;

	if (szSrc == NULL)
		return E_INVALIDARG;


	INT iSrcBuffLen = (INT)wcslen(szSrc) + 1;

	if (iBuffLen >= iSrcBuffLen)
	{
		hr = StringCpy(szDest, iBuffLen, szSrc);
		if (FAILED(hr)) return hr;
	}
	else
	{
		hr = StringDup(szDest, szSrc);
		if (FAILED(hr)) return hr;
		iBuffLen = iSrcBuffLen;
	}

	return StringLwr(szDest, iBuffLen);
}

// String convert to lower case with truncate, if source string longer then testination buffer
// szSrc				: Source string to convert
// szDest(In)			: Buffer pointer to copy
template<INT iBuffLen>
HRESULT StringLwr( char (&szDest)[iBuffLen], const char *szSrc )
{
	HRESULT hr = S_OK;

	if( szSrc == NULL )
		return E_INVALIDARG;


	INT iSrcBuffLen = (INT)strlen( szSrc ) + 1;
	INT iConvLen = std::min(iBuffLen,iSrcBuffLen);

	if( iBuffLen >= iSrcBuffLen )
	{
		hr = StringCpy( szDest, iConvLen, szSrc );
		if( FAILED(hr) ) return hr;
	}

	return StringLwr( szDest, iConvLen );
}

template<INT iBuffLen>
HRESULT StringLwr( WCHAR (&wszDest)[iBuffLen], const WCHAR *wszSrc )
{
	HRESULT hr = S_OK;

	if(wszSrc == NULL )
		return E_INVALIDARG;


	INT iSrcBuffLen = (INT)wcslen(wszSrc) + 1;
	INT iConvLen = std::min(iBuffLen,iSrcBuffLen);

	if( iBuffLen >= iSrcBuffLen )
	{
		hr = StringCpy(wszDest, iConvLen, wszSrc);
		if( FAILED(hr) ) return hr;
	}

	return StringLwr(wszDest, iConvLen );
}





// String convert to lower case 
// szSrc(In/Out)		: Source string to convert
HRESULT StringUpr( char* szSrc, INT iBuffLen )
{
	if( szSrc == NULL )
		return E_INVALIDARG;

	if( iBuffLen <= 0 ) iBuffLen = (INT)strlen(szSrc);

	for( INT iConv = 0; iConv < iBuffLen && szSrc[0] != '\0'; iConv++ )
	{
		if( szSrc[0] >= 'a' && szSrc[0] <= 'z' )
			szSrc[0] = szSrc[0] - 'a' + 'A';

		szSrc++;
	}

	return S_OK;
}

HRESULT StringUpr( WCHAR* szSrc, INT iBuffLen )
{
	if( szSrc == NULL )
		return E_INVALIDARG;

	if( iBuffLen <= 0 ) iBuffLen = (INT)wcslen(szSrc);

	for( INT iConv = 0; iConv < iBuffLen && szSrc[0] != L'\0'; iConv++ )
	{
		if( szSrc[0] >= 'a' && szSrc[0] <= 'z' )
			szSrc[0] = szSrc[0] - 'a' + 'A';

		szSrc++;
	}

	return S_OK;
}

template<INT iBuffLen>
HRESULT StringUpr( char (&szSrc)[iBuffLen] )
{
	return StringUpr( szSrc, iBuffLen );
}

template<INT iBuffLen>
HRESULT StringUpr( WCHAR (&szSrc)[iBuffLen] )
{
	return StringUpr( szSrc, iBuffLen );
}


// String convert to lower case with reallocation, if source string longer then testination buffer
// szSrc				: Source string to convert
// szDest(In/Out)		: Buffer pointer reference
// iBuffLen(In/Out)		: Destination buffer size. if buffer is small to take all source message, then reallocated
HRESULT StringUpr( char* &szDest, INT &iBuffLen, const char* szSrc )
{
	HRESULT hr = S_OK;

	if( szSrc == NULL )
		return E_INVALIDARG;


	INT iSrcBuffLen = (INT)strlen( szSrc ) + 1;

	if( iBuffLen >= iSrcBuffLen )
	{
		hr = StringCpy( szDest, iBuffLen, szSrc );
		if( FAILED(hr) ) return hr;
	}
	else
	{
		hr = StringDup( szDest, szSrc );
		if( FAILED(hr) ) return hr;
		iBuffLen = iSrcBuffLen;
	}

	return StringUpr( szDest, iBuffLen );
}

HRESULT StringUpr( WCHAR* &szDest, INT &iBuffLen, const WCHAR* szSrc )
{
	HRESULT hr = S_OK;

	if( szSrc == NULL )
		return E_INVALIDARG;


	INT iSrcBuffLen = (INT)wcslen( szSrc ) + 1;

	if( iBuffLen >= iSrcBuffLen )
	{
		hr = StringCpy( szDest, iBuffLen, szSrc );
		if( FAILED(hr) ) return hr;
	}
	else
	{
		hr = StringDup( szDest, szSrc );
		if( FAILED(hr) ) return hr;
		iBuffLen = iSrcBuffLen;
	}

	return StringUpr( szDest, iBuffLen );
}

// String convert to lower case with truncate, if source string longer then testination buffer
// szSrc				: Source string to convert
// szDest(In)			: Buffer pointer to copy
template<INT iBuffLen>
HRESULT StringUpr( char (&szDest)[iBuffLen], const char *szSrc )
{
	HRESULT hr = S_OK;

	if( szSrc == NULL )
		return E_INVALIDARG;


	INT iSrcBuffLen = (INT)strlen( szSrc ) + 1;
	INT iConvLen = std::min(iBuffLen,iSrcBuffLen);

	if( iBuffLen >= iSrcBuffLen )
	{
		hr = StringCpy( szDest, iConvLen, szSrc );
		if( FAILED(hr) ) return hr;
	}

	return StringUpr( szDest, iConvLen );
}

template<INT iBuffLen>
HRESULT StringUpr( WCHAR (&wszDest)[iBuffLen], const WCHAR *wszSrc )
{
	HRESULT hr = S_OK;

	if(wszSrc == NULL )
		return E_INVALIDARG;


	INT iSrcBuffLen = (INT)wcslen(wszSrc) + 1;
	INT iConvLen = std::min(iBuffLen,iSrcBuffLen);

	if( iBuffLen >= iSrcBuffLen )
	{
		hr = StringCpy(wszDest, iConvLen, wszSrc);
		if( FAILED(hr) ) return hr;
	}

	return StringUpr(wszDest, iConvLen );
}



// Free string with NULL check
void SafeDelete( char* &szStr )
{
	if( szStr ) delete[] szStr;
}

void SafeDelete( WCHAR* &szStr )
{
	if( szStr ) delete[] szStr;
}









