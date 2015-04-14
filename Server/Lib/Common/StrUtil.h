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
#include <string>
#include "Common/Utility.h"


namespace BR {
namespace StrUtil {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	String manipulation helper
	//

	// String duplication, szDest will destroyded if exist, and new memory will be allocated
	inline HRESULT StringDup( char* &szDest, const char* szSrc );
	inline HRESULT StringDup( WCHAR* &szDest, const WCHAR* szSrc );

	// String copy to szDest. if szDest is NULL, operation failed
	inline HRESULT StringCpyEx( char* &szDest, INT& iBuffLen, const char* szSrc ); // copy with modifed parameter
	inline HRESULT StringCpy( char* szDest, INT iBuffLen, const char* szSrc );
	inline HRESULT StringCpy( WCHAR* szDest, INT iBuffLen, const WCHAR* szSrc );
	template<INT iBuffLen>
	inline HRESULT StringCpy( char (&szDest)[iBuffLen], const char *szSrc );
	template<INT iBuffLen>
	inline HRESULT StringCpy( WCHAR (&wszDest)[iBuffLen], const WCHAR *wszSrc );


	// Unicode to MBCS string conversion
	inline HRESULT WCSToMBCS( const WCHAR* strWCS, char *strMBCS, INT iBuffLen );
	template<INT iBuffLen>
	inline HRESULT WCSToMBCS( const WCHAR* strWCS, char (&strMBCS)[iBuffLen] );
	inline HRESULT WCSToMBCS( const std::wstring &strWCS, std::string &strMBCS );

	// Unicode to UTF8 string conversion
	inline HRESULT WCSToUTF8( const WCHAR* strWCS, char *strUTF8, INT iBuffLen );
	template<INT iBuffLen>
	inline HRESULT WCSToUTF8( const WCHAR* strWCS, char (&strUTF8)[iBuffLen] );
	inline HRESULT WCSToUTF8( const std::wstring &strWCS, std::string &strUTF8 );

	// MBCS to Unicode string conversion
	inline HRESULT MBCSToWCS( const char *strMBCS, WCHAR* strWCS, INT iBuffLen );
	template<INT iBuffLen>
	inline HRESULT MBCSToWCS( const char *strMBCS, WCHAR (&strWCS)[iBuffLen] );
	inline HRESULT MBCSToWCS( const std::string &strMBCS, std::wstring &strWCS );

	// MBCS to UTF8 string conversion
	inline HRESULT MBCSToUTF8( const char *strMBCS, char* strUTF8, INT iBuffLen );
	template<INT iBuffLen>
	inline HRESULT MBCSToUTF8( const char *strMBCS, char (&strUTF8)[iBuffLen] );
	inline HRESULT MBCSToUTF8( const std::string &strMBCS, std::string &strUTF8 );

	// UTF8 to Unicode string conversion
	inline HRESULT UTF8ToWCS( const char *strUTF8, WCHAR* strWCS, INT iBuffLen );
	template<INT iBuffLen>
	inline HRESULT UTF8ToWCS( const char *strUTF8, WCHAR (&strWCS)[iBuffLen] );
	inline HRESULT UTF8ToWCS( const std::string& strUTF8, std::wstring& strWCS );

	// UTF8 to MBCS string conversion
	inline HRESULT UTF8ToMBCS( const char *strUTF8, char* strMBCS, INT iBuffLen );
	template<INT iBuffLen>
	inline HRESULT UTF8ToMBCS( const char *strUTF8, char (&strMBCS)[iBuffLen] );
	inline HRESULT UTF8ToMBCS( const std::string& strUTF8, std::string& strMBCS );


	// String compare
	inline int StringCmp( const char* szSrc, const char* szDest, INT MaxTry );
	inline int StringCmp( const char* szSrc, INT iSrcBuffLen, const char* szDest, INT iDestBuffLen );
	inline int StringCmp( const WCHAR* szSrc, INT iSrcBuffLen, const WCHAR* szDest, INT iDestBuffLen );
	template<INT iSrcBuffLen, INT iDestBuffLen>
	inline int StringCmp( char (&szSrc)[iSrcBuffLen], char (&szDest)[iDestBuffLen] );
	template<INT iDestBuffLen>
	inline int StringCmp( const char* szSrc, INT iSrcBuffLen, char (&szDest)[iDestBuffLen] );
	template<INT iSrcBuffLen>
	inline int StringCmp( char (&szSrc)[iSrcBuffLen], const char* szDest, INT iDestBuffLen );

	template<INT iSrcBuffLen, INT iDestBuffLen>
	inline int StringCmp( WCHAR (&szSrc)[iSrcBuffLen], WCHAR (&szDest)[iDestBuffLen] );
	template<INT iDestBuffLen>
	inline int StringCmp( const WCHAR* szSrc, INT iSrcBuffLen, WCHAR (&szDest)[iDestBuffLen] );
	template<INT iSrcBuffLen>
	inline int StringCmp( WCHAR (&szSrc)[iSrcBuffLen], const WCHAR* szDest, INT iDestBuffLen );


	// String compare with lower case
	inline int StringCmpLwr( const char* szSrc, INT iSrcBuffLen, const char* szDest, INT iDestBuffLen );
	inline int StringCmpLwr( const WCHAR* szSrc, INT iSrcBuffLen, const WCHAR* szDest, INT iDestBuffLen );
	template<INT iSrcBuffLen, INT iDestBuffLen>
	inline int StringCmpLwr( char (&szSrc)[iSrcBuffLen], char (&szDest)[iDestBuffLen] );
	template<INT iDestBuffLen>
	inline int StringCmpLwr( const char* szSrc, INT iSrcBuffLen, char (&szDest)[iDestBuffLen] );
	template<INT iSrcBuffLen>
	inline int StringCmpLwr( char (&szSrc)[iSrcBuffLen], const char* szDest, INT iDestBuffLen );

	template<int iSrcBuffLen, INT iDestBuffLen>
	inline int StringCmpLwr( WCHAR (&szSrc)[iSrcBuffLen], WCHAR (&szDest)[iDestBuffLen] );
	template<int iDestBuffLen>
	inline int StringCmpLwr( const WCHAR* szSrc, INT iSrcBuffLen, WCHAR (&szDest)[iDestBuffLen] );
	template<int iSrcBuffLen>
	inline int StringCmpLwr( WCHAR (&szSrc)[iSrcBuffLen], const WCHAR* szDest, INT iDestBuffLen );


	// String convert to lower case 
	// szSrc(In/Out)		: Source string to convert
	inline HRESULT StringLwr( char* szSrc, INT iBuffLen );
	inline HRESULT StringLwr( WCHAR* szSrc, INT iBuffLen );
	template<INT iBuffLen>
	inline HRESULT StringLwr( char (&szSrc)[iBuffLen] );
	template<INT iBuffLen>
	inline HRESULT StringLwr( WCHAR (&szSrc)[iBuffLen] );

	// String convert to lower case with reallocation, if source string longer then testination buffer
	// szSrc				: Source string to convert
	// szDest(In/Out)		: Buffer pointer reference
	// iBuffLen(In/Out)		: Destination buffer size. if buffer is small to take all source message, then reallocated
	inline HRESULT StringLwr( char* &szDest, INT &iBuffLen, const char* szSrc );
	inline HRESULT StringLwr( WCHAR* &szDest, INT &iBuffLen, const WCHAR* szSrc );

	// String convert to lower case with truncate, if source string longer then testination buffer
	// szSrc				: Source string to convert
	// szDest(In)			: Buffer pointer to copy
	template<INT iBuffLen>
	inline HRESULT StringLwr( char (&szDest)[iBuffLen], const char *szSrc );
	template<INT iBuffLen>
	inline HRESULT StringLwr( WCHAR (&wszDest)[iBuffLen], const WCHAR *wszSrc );


	// String convert to uppercase 
	// szSrc(In/Out)		: Source string to convert
	inline HRESULT StringUpr( char* szSrc, INT iBuffLen );
	inline HRESULT StringUpr( WCHAR* szSrc, INT iBuffLen );
	template<INT iBuffLen>
	inline HRESULT StringUpr( char (&szSrc)[iBuffLen] );
	template<INT iBuffLen>
	inline HRESULT StringUpr( WCHAR (&szSrc)[iBuffLen] );

	// String convert to uppercase with reallocation, if source string longer then testination buffer
	// szSrc				: Source string to convert
	// szDest(In/Out)		: Buffer pointer reference
	// iBuffLen(In/Out)		: Destination buffer size. if buffer is small to take all source message, then reallocated
	inline HRESULT StringUpr( char* &szDest, INT &iBuffLen, const char* szSrc );
	inline HRESULT StringUpr( WCHAR* &szDest, INT &iBuffLen, const WCHAR* szSrc );

	// String convert to uppercase with truncate, if source string longer then testination buffer
	// szSrc				: Source string to convert
	// szDest(In)			: Buffer pointer to copy
	template<INT iBuffLen>
	inline HRESULT StringUpr( char (&szDest)[iBuffLen], const char *szSrc );
	template<INT iBuffLen>
	inline HRESULT StringUpr( WCHAR (&wszDest)[iBuffLen], const WCHAR *wszSrc );



	// Free string with NULL check
	inline void SafeDelete( char* &szStr );
	inline void SafeDelete( WCHAR* &szStr );
	

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	//	UTF8 binary string
	//

#pragma pack (push)
#pragma pack (1)
	struct UTF8Bin
	{
		BYTE IsNull;
		UINT32 Length;
		char stringData[1];
	};
#pragma pack(pop)

	// UTF8 to UTF8String(Database UTF8 Format) string conversion
	template<INT iBuffLen>
	inline HRESULT UTF8ToUTF8Bin( BYTE (&UTF8BinBuff)[iBuffLen], const char* strSrc );

	inline HRESULT UTF8BinToUTF8( char* strDst, INT dstLen, const BYTE* strSrc );


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Helper data
	//

	extern const char* EmptyString;

#include "StrUtil.inl"


}; // namespace StrUtil
} // namespace BR


