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


#include "stdafx.h"
#include "Common/StrUtil.h"
#include "Common/ToString.h"
#include "Common/Argument.h"
#include "Common/StrFormat.h"
#include "Common/Utility.h"
#include "Common/HRESCommon.h"

#include "iconv.h"


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
		if( szBuffer == nullptr || iBuffLen == 0 )
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

				if (iArg < iNumArg)
				{
					Args[iArg].MakeString(szBuffer, iBuffLen);
				}
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



	/////////////////////////////////////////////////////////////////////////////
	//
	//	iconv system
	//

	class ModuleIconv_impl
	{
	private:


	public:
		ModuleIconv_impl()
		{
			// Make iconv detect system locale correctly
			setlocale(LC_ALL, "");
		}

		~ModuleIconv_impl()
		{
		}


		// remain buffer size will be in destSize
		HRESULT Convert(const char* destCode, char* dest, size_t destSize, const char* srcCode, const char* src, size_t srcSize, size_t& convertedSize)
		{
			HRESULT hr = S_OK;

			convertedSize = 0;

			iconv_t context = iconv_open(destCode, srcCode);
			if (context == (iconv_t)-1)
			{
				hr = E_FAIL;
				goto Proc_End;
			}

			convertedSize = iconv(context, &src, &srcSize, &dest, &destSize);
			if (convertedSize == (size_t)-1)
			{
				switch (errno)
				{
				case E2BIG:
					hr = E_OUTOFMEMORY;
					break;
				case EILSEQ:
					hr = E_INVALID_STR_DATA;
					break;
				case EINVAL:
					hr = E_OUTOFMEMORY;
					break;
				default:
					hr = E_UNEXPECTED;
					break;
				}

				convertedSize = 0;
			}

		Proc_End:

			if (context != (iconv_t)-1)
			{
				iconv_close(context);
			}

			return hr;
		}
	};

	ModuleIconv_impl ModuleIconv;

	/////////////////////////////////////////////////////////////////////////////
	//
	//	String convert
	//

	// Unicode to MBCS string conversion
	HRESULT WCSToMBCS(const WCHAR* strWCS, char *strMBCS, INT iBuffLen)
	{
		size_t convertedSize;

		if (strWCS == nullptr || strMBCS == nullptr)
			return E_INVALIDARG;

		HRESULT hr = ModuleIconv.Convert("", (char*)strMBCS, iBuffLen, "UTF?32", (const char*)strWCS, wcslen(strWCS), convertedSize);
		if (FAILED(hr)) return hr;

		if (iBuffLen >= 1)
		{
			iBuffLen = std::min((INT)convertedSize + 1, iBuffLen) - 1;
			strMBCS[iBuffLen] = '\0';
		}

		return S_OK;
	}

	HRESULT WCSToMBCS(const std::wstring &strWCS, std::string &strMBCS)
	{
		size_t convertedSize;
		char stringBuffer[4*1024];

		if (strWCS.c_str() == nullptr)
		{
			strMBCS = "";
			return S_OK;
		}

		HRESULT hr = ModuleIconv.Convert("", (char*)stringBuffer, countof(stringBuffer), "UTF?32", (const char*)strWCS.c_str(), strWCS.length(), convertedSize);
		if (FAILED(hr)) return hr;

		auto lastPos = std::min(convertedSize + 1, countof(stringBuffer)) - 1;
		stringBuffer[lastPos] = '\0';

		strMBCS = stringBuffer;

		return S_OK;
	}

	// Unicode to UTF8 string conversion
	HRESULT WCSToUTF8(const WCHAR* strWCS, char *strUTF8, INT iBuffLen)
	{
		size_t convertedSize;

		if (strWCS == nullptr || strUTF8 == nullptr)
			return E_INVALIDARG;

		HRESULT hr = ModuleIconv.Convert("UTF?8", (char*)strUTF8, iBuffLen, "UTF?32", (const char*)strWCS, wcslen(strWCS), convertedSize);
		if (FAILED(hr)) return hr;

		if (iBuffLen >= 1)
		{
			iBuffLen = std::min((INT)convertedSize + 1, iBuffLen) - 1;
			strUTF8[iBuffLen] = '\0';
		}

		return S_OK;
	}

	HRESULT WCSToUTF8(const std::wstring &strWCS, std::string &strUTF8)
	{
		size_t convertedSize;
		char stringBuffer[4 * 1024];

		if (strWCS.c_str() == nullptr)
		{
			strUTF8 = "";
			return S_OK;
		}

		HRESULT hr = ModuleIconv.Convert("UTF?8", (char*)stringBuffer, countof(stringBuffer), "UTF?32", (const char*)strWCS.c_str(), strWCS.length(), convertedSize);
		if (FAILED(hr)) return hr;

		auto lastPos = std::min(convertedSize + 1, countof(stringBuffer)) - 1;
		stringBuffer[lastPos] = '\0';

		strUTF8 = stringBuffer;

		return S_OK;
	}

	// MBCS to Unicode string conversion
	HRESULT MBCSToWCS(const char *strMBCS, WCHAR* strWCS, INT iBuffLen)
	{
		size_t convertedSize;

		if (strWCS == nullptr || strMBCS == nullptr)
			return E_INVALIDARG;

		HRESULT hr = ModuleIconv.Convert("UTF?32", (char*)strWCS, iBuffLen, "", (const char*)strMBCS, strlen(strMBCS), convertedSize);
		if (FAILED(hr)) return hr;

		if (iBuffLen >= 1)
		{
			iBuffLen = std::min((INT)convertedSize + 1, iBuffLen) - 1;
			strWCS[iBuffLen] = '\0';
		}

		return S_OK;
	}

	HRESULT MBCSToWCS(const std::string &strMBCS, std::wstring &strWCS)
	{
		size_t convertedSize;
		wchar_t stringBuffer[4 * 1024];

		if (strWCS.c_str() == nullptr)
		{
			strWCS = L"";
			return S_OK;
		}

		HRESULT hr = ModuleIconv.Convert("UTF?32", (char*)stringBuffer, countof(stringBuffer), "", (const char*)strMBCS.c_str(), strMBCS.length(), convertedSize);
		if (FAILED(hr)) return hr;

		auto lastPos = std::min(convertedSize + 1, countof(stringBuffer)) - 1;
		stringBuffer[lastPos] = '\0';

		strWCS = stringBuffer;

		return S_OK;
	}


	// UTF8 to Unicode string conversion
	HRESULT UTF8ToWCS(const char *strUTF8, WCHAR* strWCS, INT iBuffLen)
	{
		size_t convertedSize;

		if (strWCS == nullptr || strUTF8 == nullptr)
			return E_INVALIDARG;

		HRESULT hr = ModuleIconv.Convert("UTF?32", (char*)strWCS, iBuffLen, "UTF-8", (const char*)strUTF8, strlen(strUTF8), convertedSize);
		if (FAILED(hr)) return hr;

		if (iBuffLen >= 1)
		{
			iBuffLen = std::min((INT)convertedSize + 1, iBuffLen) - 1;
			strWCS[iBuffLen] = '\0';
		}

		return S_OK;
	}

	HRESULT UTF8ToWCS(const std::string& strUTF8, std::wstring& strWCS)
	{
		size_t convertedSize;
		wchar_t stringBuffer[4 * 1024];

		if (strWCS.c_str() == nullptr)
		{
			strWCS = L"";
			return S_OK;
		}

		HRESULT hr = ModuleIconv.Convert("UTF?32", (char*)stringBuffer, countof(stringBuffer), "UTF-8", (const char*)strUTF8.c_str(), strUTF8.length(), convertedSize);
		if (FAILED(hr)) return hr;

		auto lastPos = std::min(convertedSize + 1, countof(stringBuffer)) - 1;
		stringBuffer[lastPos] = '\0';

		strWCS = stringBuffer;

		return S_OK;
	}


	// MBCS to Unicode string conversion
	HRESULT MBCSToUTF8(const char *strMBCS, char* strUTF8, INT iBuffLen)
	{
		size_t convertedSize;

		if (strUTF8 == nullptr || strMBCS == nullptr)
			return E_INVALIDARG;

		HRESULT hr = ModuleIconv.Convert("UTF?8", (char*)strUTF8, iBuffLen, "", (const char*)strMBCS, strlen(strMBCS), convertedSize);
		if (FAILED(hr)) return hr;

		if (iBuffLen >= 1)
		{
			iBuffLen = std::min((INT)convertedSize + 1, iBuffLen) - 1;
			strUTF8[iBuffLen] = '\0';
		}

		return S_OK;
	}

	HRESULT MBCSToUTF8(const std::string &strMBCS, std::string &strUTF8)
	{
		size_t convertedSize;
		char stringBuffer[4 * 1024];

		if (strUTF8.c_str() == nullptr)
		{
			strUTF8 = "";
			return S_OK;
		}

		HRESULT hr = ModuleIconv.Convert("UTF?32", (char*)stringBuffer, countof(stringBuffer), "", (const char*)strMBCS.c_str(), strMBCS.length(), convertedSize);
		if (FAILED(hr)) return hr;

		auto lastPos = std::min(convertedSize + 1, countof(stringBuffer)) - 1;
		stringBuffer[lastPos] = '\0';

		strUTF8 = stringBuffer;

		return S_OK;
	}


	// UTF8 to MBCS string conversion
	HRESULT UTF8ToMBCS(const char *strUTF8, char* strMBCS, INT iBuffLen)
	{
		size_t convertedSize;

		if (strUTF8 == nullptr || strMBCS == nullptr)
			return E_INVALIDARG;

		HRESULT hr = ModuleIconv.Convert("UTF?8", (char*)strMBCS, iBuffLen, "", (const char*)strUTF8, strlen(strUTF8), convertedSize);
		if (FAILED(hr)) return hr;

		if (iBuffLen >= 1)
		{
			iBuffLen = std::min((INT)convertedSize + 1, iBuffLen) - 1;
			strMBCS[iBuffLen] = '\0';
		}

		return S_OK;
	}

	HRESULT UTF8ToMBCS(const std::string& strUTF8, std::string& strMBCS)
	{
		size_t convertedSize;
		char stringBuffer[4 * 1024];

		if (strMBCS.c_str() == nullptr)
		{
			strMBCS = "";
			return S_OK;
		}

		HRESULT hr = ModuleIconv.Convert("UTF?32", (char*)stringBuffer, countof(stringBuffer), "", (const char*)strUTF8.c_str(), strUTF8.length(), convertedSize);
		if (FAILED(hr)) return hr;

		auto lastPos = std::min(convertedSize + 1, countof(stringBuffer)) - 1;
		stringBuffer[lastPos] = '\0';

		strMBCS = stringBuffer;

		return S_OK;
	}


};	// namespace Svr
} // namespace BR

