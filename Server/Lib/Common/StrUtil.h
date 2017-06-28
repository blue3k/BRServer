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
#include "Common/ResultCode/BRResultCodeSystem.h"


namespace BR {
namespace StrUtil {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	String manipulation helper
	//

	// String duplication, szDest will destroyded if exist, and new memory will be allocated
	Result StringDup( char* &szDest, const char* szSrc );
	Result StringDup( wchar_t* &szDest, const wchar_t* szSrc );

	// copy string and update dest pointer and length
	Result StringCpyEx( char* &szDest, INT& iBuffLen, const char* szSrc ); // copy with modifed parameter
	Result StringCpyEx(wchar_t* &szDest, INT& iBuffLen, const wchar_t* szSrc); // copy with modifed parameter

	// String copy to szDest. if szDest is NULL, operation failed
	// Simple copy with boundary check
	Result StringCpy( char* szDest, INT iBuffLen, const char* szSrc );
	Result StringCpy( wchar_t* szDest, INT iBuffLen, const wchar_t* szSrc );
	template<INT iBuffLen>
	inline Result StringCpy( char (&szDest)[iBuffLen], const char *szSrc );
	template<INT iBuffLen>
	inline Result StringCpy( wchar_t (&wszDest)[iBuffLen], const wchar_t *wszSrc );


	//// Unicode to MBCS string conversion
	//Result WCSToMBCS( const wchar_t* strWCS, char *strMBCS, INT iBuffLen );
	//template<INT iBuffLen>
	//Result WCSToMBCS( const wchar_t* strWCS, char (&strMBCS)[iBuffLen] );
	//Result WCSToMBCS( const std::wstring &strWCS, std::string &strMBCS );

	// Unicode to UTF8 string conversion
	Result WCSToUTF8( const wchar_t* strWCS, char *strUTF8, INT iBuffLen );
	template<INT iBuffLen>
	Result WCSToUTF8( const wchar_t* strWCS, char (&strUTF8)[iBuffLen] );
	Result WCSToUTF8( const std::wstring &strWCS, std::string &strUTF8 );

	//// MBCS to Unicode string conversion
	//Result MBCSToWCS( const char *strMBCS, wchar_t* strWCS, INT iBuffLen );
	//template<INT iBuffLen>
	//Result MBCSToWCS( const char *strMBCS, wchar_t (&strWCS)[iBuffLen] );
	//Result MBCSToWCS( const std::string &strMBCS, std::wstring &strWCS );

	//// MBCS to UTF8 string conversion
	//Result MBCSToUTF8( const char *strMBCS, char* strUTF8, INT iBuffLen );
	//template<INT iBuffLen>
	//Result MBCSToUTF8( const char *strMBCS, char (&strUTF8)[iBuffLen] );
	//Result MBCSToUTF8( const std::string &strMBCS, std::string &strUTF8 );

	// UTF8 to Unicode string conversion
	Result UTF8ToWCS( const char *strUTF8, wchar_t* strWCS, INT iBuffLen );
	template<INT iBuffLen>
	Result UTF8ToWCS( const char *strUTF8, wchar_t (&strWCS)[iBuffLen] );
	Result UTF8ToWCS( const std::string& strUTF8, std::wstring& strWCS );

	//// UTF8 to MBCS string conversion
	//Result UTF8ToMBCS( const char *strUTF8, char* strMBCS, INT iBuffLen );
	//template<INT iBuffLen>
	//Result UTF8ToMBCS( const char *strUTF8, char (&strMBCS)[iBuffLen] );
	//Result UTF8ToMBCS( const std::string& strUTF8, std::string& strMBCS );


	// String compare
	inline int StringCmp( const char* szSrc, const char* szDest, INT MaxTry );
	inline int StringCmp( const char* szSrc, INT iSrcBuffLen, const char* szDest, INT iDestBuffLen );
	inline int StringCmp( const wchar_t* szSrc, INT iSrcBuffLen, const wchar_t* szDest, INT iDestBuffLen );
	template<INT iSrcBuffLen, INT iDestBuffLen>
	inline int StringCmp( char (&szSrc)[iSrcBuffLen], char (&szDest)[iDestBuffLen] );
	template<INT iDestBuffLen>
	inline int StringCmp( const char* szSrc, INT iSrcBuffLen, char (&szDest)[iDestBuffLen] );
	template<INT iSrcBuffLen>
	inline int StringCmp( char (&szSrc)[iSrcBuffLen], const char* szDest, INT iDestBuffLen );

	template<INT iSrcBuffLen, INT iDestBuffLen>
	inline int StringCmp( wchar_t (&szSrc)[iSrcBuffLen], wchar_t (&szDest)[iDestBuffLen] );
	template<INT iDestBuffLen>
	inline int StringCmp( const wchar_t* szSrc, INT iSrcBuffLen, wchar_t (&szDest)[iDestBuffLen] );
	template<INT iSrcBuffLen>
	inline int StringCmp( wchar_t (&szSrc)[iSrcBuffLen], const wchar_t* szDest, INT iDestBuffLen );


	// String compare with lower case
	inline int StringCmpLwr( const char* szSrc, INT iSrcBuffLen, const char* szDest, INT iDestBuffLen );
	inline int StringCmpLwr( const wchar_t* szSrc, INT iSrcBuffLen, const wchar_t* szDest, INT iDestBuffLen );
	template<INT iSrcBuffLen, INT iDestBuffLen>
	inline int StringCmpLwr( char (&szSrc)[iSrcBuffLen], char (&szDest)[iDestBuffLen] );
	template<INT iDestBuffLen>
	inline int StringCmpLwr( const char* szSrc, INT iSrcBuffLen, char (&szDest)[iDestBuffLen] );
	template<INT iSrcBuffLen>
	inline int StringCmpLwr( char (&szSrc)[iSrcBuffLen], const char* szDest, INT iDestBuffLen );

	template<int iSrcBuffLen, INT iDestBuffLen>
	inline int StringCmpLwr( wchar_t (&szSrc)[iSrcBuffLen], wchar_t (&szDest)[iDestBuffLen] );
	template<int iDestBuffLen>
	inline int StringCmpLwr( const wchar_t* szSrc, INT iSrcBuffLen, wchar_t (&szDest)[iDestBuffLen] );
	template<int iSrcBuffLen>
	inline int StringCmpLwr( wchar_t (&szSrc)[iSrcBuffLen], const wchar_t* szDest, INT iDestBuffLen );


	// String convert to lower case 
	// szSrc(In/Out)		: Source string to convert
	inline Result StringLwr( char* szSrc, INT iBuffLen );
	inline Result StringLwr( wchar_t* szSrc, INT iBuffLen );
	template<INT iBuffLen>
	inline Result StringLwr( char (&szSrc)[iBuffLen] );
	template<INT iBuffLen>
	inline Result StringLwr( wchar_t (&szSrc)[iBuffLen] );

	// String convert to lower case with reallocation with destination pointer and length update
	// szSrc				: Source string to convert
	// szDest(In/Out)		: Buffer pointer reference
	// iBuffLen(In/Out)		: Destination buffer size. if buffer is small to take all source message, then reallocated
	inline Result StringLwr( char* &szDest, INT &iBuffLen, const char* szSrc );
	inline Result StringLwr( wchar_t* &szDest, INT &iBuffLen, const wchar_t* szSrc );

	// Update desitnation pointer and size update
	inline Result StringLwrEx(char* &szDest, INT &iBuffLen, const char* szSrc);
	inline Result StringLwrEx(wchar_t* &szDest, INT &iBuffLen, const wchar_t* szSrc);

	// String convert to lower case with truncate, if source string longer then testination buffer
	// szSrc				: Source string to convert
	// szDest(In)			: Buffer pointer to copy
	template<INT iBuffLen>
	inline Result StringLwr( char (&szDest)[iBuffLen], const char *szSrc );
	template<INT iBuffLen>
	inline Result StringLwr( wchar_t (&wszDest)[iBuffLen], const wchar_t *wszSrc );


	// String convert to uppercase 
	// szSrc(In/Out)		: Source string to convert
	Result StringUpr( char* szSrc, INT iBuffLen );
	Result StringUpr( wchar_t* szSrc, INT iBuffLen );
	template<INT iBuffLen>
	Result StringUpr( char (&szSrc)[iBuffLen] );
	template<INT iBuffLen>
	Result StringUpr( wchar_t (&szSrc)[iBuffLen] );

	// String convert to uppercase with reallocation, if source string longer then testination buffer
	// szSrc				: Source string to convert
	// szDest(In/Out)		: Buffer pointer reference
	// iBuffLen(In/Out)		: Destination buffer size. if buffer is small to take all source message, then reallocated
	Result StringUpr( char* &szDest, INT &iBuffLen, const char* szSrc );
	Result StringUpr( wchar_t* &szDest, INT &iBuffLen, const wchar_t* szSrc );

	// String convert to uppercase with truncate, if source string longer then testination buffer
	// szSrc				: Source string to convert
	// szDest(In)			: Buffer pointer to copy
	template<INT iBuffLen>
	Result StringUpr( char (&szDest)[iBuffLen], const char *szSrc );
	template<INT iBuffLen>
	Result StringUpr( wchar_t (&wszDest)[iBuffLen], const wchar_t *wszSrc );


	bool IsNullOrEmpty(const char* szStr);
	bool IsNullOrEmpty(const wchar_t* szStr);

	// Free string with NULL check
	void SafeDelete( char* &szStr );
	void SafeDelete( wchar_t* &szStr );
	

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
	inline Result UTF8ToUTF8Bin( BYTE (&UTF8BinBuff)[iBuffLen], const char* strSrc );

	inline Result UTF8BinToUTF8( char* strDst, INT dstLen, const BYTE* strSrc );


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Helper data
	//

	extern const char* EmptyString;

#include "StrUtil.inl"


} // namespace StrUtil
} // namespace BR


