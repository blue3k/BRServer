﻿////////////////////////////////////////////////////////////////////////////////
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
#include "Common/Trace.h"
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

	static inline void SkipSpace(const char*& szFormating)
	{
		for (char curChar = *szFormating; curChar && (curChar == ' ' && curChar != '	'); curChar = *szFormating++);
	}

	static inline double ReadNumber(char& curChar, const char*& szFormating)
	{
		double fNumber = 0;
		for (curChar = *szFormating++; curChar && (curChar != '%' && curChar != '}' && curChar != '{' && curChar != ':'); curChar = *szFormating++)
		{
			if (curChar == '.')
				break;

			if (curChar == '\0')
				return fNumber;

			if (curChar < '0' || curChar > '9')
				continue;

			register int iArgTem = ChToInt(curChar);
			if (iArgTem >= 0)
				fNumber = fNumber * 10 + iArgTem;
		}

		if (curChar != '.') return fNumber;

		double fExponent = 0.1;
		for (curChar = *szFormating++; curChar && (curChar != '%' && curChar != '}' && curChar != ':'); curChar = *szFormating++)
		{
			if (curChar < '0' || curChar > '9')
				continue;

			if (curChar == '\0')
				return fNumber;

			register int iArgTem = ChToInt(curChar);
			if (iArgTem >= 0)
				fNumber += (double)iArgTem * fExponent;
		}

		return fNumber;
	}

	// Internal format routine
	HRESULT Format_Internal( char*& szBuffer, INT& iBuffLen, const char* szFormating, int iNumArg, Argument* Args )
	{
		if( szBuffer == nullptr || iBuffLen == 0 )
			return E_POINTER;

		register char curChar;

		for( curChar = *szFormating++; curChar && iBuffLen > 0; curChar = *szFormating++ )
		{
			if( curChar == '%' || curChar == '{')
			{
				int iArg = 0;

				SkipSpace(szFormating);

				// read argument index
				iArg = (int)(ReadNumber(curChar, szFormating) + 0.1);
				if (curChar == '{')
				{
					*szBuffer++ = curChar;
					iBuffLen--;
					continue;
				}

				// read option
				char option = '\0';
				double digits = -1;
				if (curChar == ':')
				{
					if (curChar == '\0')
						return E_INVALIDARG;

					SkipSpace(szFormating);

					if (curChar == '\0')
						return E_INVALIDARG;

					curChar = *szFormating++;

					if (curChar == '\0')
						return E_INVALIDARG;

					option = curChar;

					digits = ReadNumber(curChar, szFormating);
				}

				if( curChar == '\0' )
					return E_INVALIDARG;

				if (iArg < iNumArg)
				{
					Args[iArg].MakeString(szBuffer, iBuffLen, option, (float)digits);
				}
				else
				{
					StrUtil::StringCpyEx( szBuffer, iBuffLen, "(Null)" );
				}
			}
			else if (curChar == '}')
			{
				// Skip the first closing bracket
				curChar = *szFormating++;
				if (curChar == '}')
				{
					*szBuffer++ = curChar;
					iBuffLen--;
				}
				else
				{
					// ignore other case
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
			auto orgDestSize = destSize;

			convertedSize = 0;

			iconv_t context = iconv_open(destCode, srcCode);
			if (context == (iconv_t)-1)
			{
				hr = E_FAIL;
				goto Proc_End;
			}

#if LINUX
			{
				// On linux, source buffer can be changed.
				char srcBuffer[2048];
				char *srcTemp = sizeof(srcBuffer) >= srcSize ? srcBuffer : new char[srcSize];
				defChkSilent(StrUtil::StringCpy(srcTemp, srcSize, src));

				convertedSize = iconv(context, &srcTemp, &srcSize, &dest, &destSize); // linux version uses char** for src
			}
#else
			convertedSize = iconv(context, &src, &srcSize, &dest, &destSize); // linux version uses char** for src
#endif
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
			else
			{
				convertedSize = orgDestSize - destSize;
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

	//// Unicode to MBCS string conversion
	//HRESULT WCSToMBCS(const WCHAR* strWCS, char *strMBCS, INT iBuffLen)
	//{
	//	size_t convertedSize;

	//	if (strWCS == nullptr || strMBCS == nullptr)
	//		return E_INVALIDARG;

	//	HRESULT hr = ModuleIconv.Convert("", (char*)strMBCS, iBuffLen, "UTF-16", (const char*)strWCS, wcslen(strWCS)*sizeof(wchar_t), convertedSize);
	//	if (FAILED(hr)) return hr;

	//	if (iBuffLen >= 1)
	//	{
	//		iBuffLen = std::min((INT)convertedSize + 1, iBuffLen) - 1;
	//		strMBCS[iBuffLen] = '\0';
	//	}

	//	return S_OK;
	//}

	//HRESULT WCSToMBCS(const std::wstring &strWCS, std::string &strMBCS)
	//{
	//	size_t convertedSize;
	//	char stringBuffer[4*1024];

	//	if (strWCS.c_str() == nullptr)
	//	{
	//		strMBCS = "";
	//		return S_OK;
	//	}

	//	HRESULT hr = ModuleIconv.Convert("", (char*)stringBuffer, countof(stringBuffer), "UTF-16", (const char*)strWCS.c_str(), strWCS.length(), convertedSize);
	//	if (FAILED(hr)) return hr;

	//	auto lastPos = std::min(convertedSize + 1, countof(stringBuffer)) - 1;
	//	stringBuffer[lastPos] = '\0';

	//	strMBCS = stringBuffer;

	//	return S_OK;
	//}

	// Unicode to UTF8 string conversion
	HRESULT WCSToUTF8(const WCHAR* strWCS, char *strUTF8, INT iBuffLen)
	{
		size_t convertedSize;

		if (strWCS == nullptr || strUTF8 == nullptr)
			return E_INVALIDARG;

		HRESULT hr = ModuleIconv.Convert("UTF-8", (char*)strUTF8, iBuffLen, "UTF-16LE", (const char*)strWCS, wcslen(strWCS)*sizeof(wchar_t), convertedSize);
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

		HRESULT hr = ModuleIconv.Convert("UTF-8", (char*)stringBuffer, countof(stringBuffer), "UTF-16LE", (const char*)strWCS.c_str(), strWCS.length(), convertedSize);
		if (FAILED(hr)) return hr;

		auto lastPos = std::min(convertedSize + 1, countof(stringBuffer)) - 1;
		stringBuffer[lastPos] = '\0';

		strUTF8 = stringBuffer;

		return S_OK;
	}

	//// MBCS to Unicode string conversion
	//HRESULT MBCSToWCS(const char *strMBCS, WCHAR* strWCS, INT iBuffLen)
	//{
	//	size_t convertedSize;

	//	if (strWCS == nullptr || strMBCS == nullptr)
	//		return E_INVALIDARG;

	//	HRESULT hr = ModuleIconv.Convert("UTF-16", (char*)strWCS, iBuffLen, "", (const char*)strMBCS, strlen(strMBCS), convertedSize);
	//	if (FAILED(hr)) return hr;

	//	if (iBuffLen >= 1)
	//	{
	//		iBuffLen = std::min((INT)convertedSize + 1, iBuffLen) - 1;
	//		strWCS[iBuffLen] = '\0';
	//	}

	//	return S_OK;
	//}

	//HRESULT MBCSToWCS(const std::string &strMBCS, std::wstring &strWCS)
	//{
	//	size_t convertedSize;
	//	wchar_t stringBuffer[4 * 1024];

	//	if (strWCS.c_str() == nullptr)
	//	{
	//		strWCS = L"";
	//		return S_OK;
	//	}

	//	HRESULT hr = ModuleIconv.Convert("UTF-16", (char*)stringBuffer, countof(stringBuffer), "", (const char*)strMBCS.c_str(), strMBCS.length(), convertedSize);
	//	if (FAILED(hr)) return hr;

	//	auto lastPos = std::min(convertedSize + 1, countof(stringBuffer)) - 1;
	//	stringBuffer[lastPos] = '\0';

	//	strWCS = stringBuffer;

	//	return S_OK;
	//}


	// UTF8 to Unicode string conversion
	HRESULT UTF8ToWCS(const char *strUTF8, WCHAR* strWCS, INT iBuffLen)
	{
		size_t convertedSize;

		if (strWCS == nullptr || strUTF8 == nullptr)
			return E_INVALIDARG;

		HRESULT hr = ModuleIconv.Convert("UTF-16LE", (char*)strWCS, iBuffLen, "UTF-8", (const char*)strUTF8, strlen(strUTF8)+1, convertedSize);
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

		HRESULT hr = ModuleIconv.Convert("UTF-16LE", (char*)stringBuffer, countof(stringBuffer), "UTF-8", (const char*)strUTF8.c_str(), strUTF8.length(), convertedSize);
		if (FAILED(hr)) return hr;

		auto lastPos = std::min(convertedSize + 1, countof(stringBuffer)) - 1;
		stringBuffer[lastPos] = '\0';

		strWCS = stringBuffer;

		return S_OK;
	}


	//// MBCS to Unicode string conversion
	//HRESULT MBCSToUTF8(const char *strMBCS, char* strUTF8, INT iBuffLen)
	//{
	//	size_t convertedSize;

	//	if (strUTF8 == nullptr || strMBCS == nullptr)
	//		return E_INVALIDARG;

	//	HRESULT hr = ModuleIconv.Convert("UTF-8", (char*)strUTF8, iBuffLen, "", (const char*)strMBCS, strlen(strMBCS), convertedSize);
	//	if (FAILED(hr)) return hr;

	//	if (iBuffLen >= 1)
	//	{
	//		iBuffLen = std::min((INT)convertedSize + 1, iBuffLen) - 1;
	//		strUTF8[iBuffLen] = '\0';
	//	}

	//	return S_OK;
	//}

	//HRESULT MBCSToUTF8(const std::string &strMBCS, std::string &strUTF8)
	//{
	//	size_t convertedSize;
	//	char stringBuffer[4 * 1024];

	//	if (strUTF8.c_str() == nullptr)
	//	{
	//		strUTF8 = "";
	//		return S_OK;
	//	}

	//	HRESULT hr = ModuleIconv.Convert("UTF-16", (char*)stringBuffer, countof(stringBuffer), "", (const char*)strMBCS.c_str(), strMBCS.length(), convertedSize);
	//	if (FAILED(hr)) return hr;

	//	auto lastPos = std::min(convertedSize + 1, countof(stringBuffer)) - 1;
	//	stringBuffer[lastPos] = '\0';

	//	strUTF8 = stringBuffer;

	//	return S_OK;
	//}


	//// UTF8 to MBCS string conversion
	//HRESULT UTF8ToMBCS(const char *strUTF8, char* strMBCS, INT iBuffLen)
	//{
	//	size_t convertedSize;

	//	if (strUTF8 == nullptr || strMBCS == nullptr)
	//		return E_INVALIDARG;

	//	HRESULT hr = ModuleIconv.Convert("UTF-8", (char*)strMBCS, iBuffLen, "", (const char*)strUTF8, strlen(strUTF8), convertedSize);
	//	if (FAILED(hr)) return hr;

	//	if (iBuffLen >= 1)
	//	{
	//		iBuffLen = std::min((INT)convertedSize + 1, iBuffLen) - 1;
	//		strMBCS[iBuffLen] = '\0';
	//	}

	//	return S_OK;
	//}

	//HRESULT UTF8ToMBCS(const std::string& strUTF8, std::string& strMBCS)
	//{
	//	size_t convertedSize;
	//	char stringBuffer[4 * 1024];

	//	if (strMBCS.c_str() == nullptr)
	//	{
	//		strMBCS = "";
	//		return S_OK;
	//	}

	//	HRESULT hr = ModuleIconv.Convert("UTF-16", (char*)stringBuffer, countof(stringBuffer), "", (const char*)strUTF8.c_str(), strUTF8.length(), convertedSize);
	//	if (FAILED(hr)) return hr;

	//	auto lastPos = std::min(convertedSize + 1, countof(stringBuffer)) - 1;
	//	stringBuffer[lastPos] = '\0';

	//	strMBCS = stringBuffer;

	//	return S_OK;
	//}


};	// namespace Svr
} // namespace BR

