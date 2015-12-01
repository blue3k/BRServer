////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 MadK
// 
// Author : KyungKun Ko
//
// Description : Base type ToString implementation
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/BrAssert.h"
#include "Common/StrUtil.h"
#include "Common/ToStringBase.h"


namespace BR
{
	//////////////////////////////////////////////////////////////////////////////////
	//
	//	help operations
	//

	#define _AppendCharReturn( buf, length, charToAppend ) {*buf++ = charToAppend;	length--;	if(length <=0 ) return E_OUTOFMEMORY;}

	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	String manipulation helper
	//

	// Itoa customized version from VC
	template< class Type >
	HRESULT _MyIToA(
				Type val,
				char* &buf,
				INT& length,
				unsigned radix,
				bool bIsNeg,
				int iDigit = -1 // fixed digit count
				)
	{
		Assert( val >= 0 );

		// Validation
		if( buf == NULL || length <= 0
			|| radix < 2 || radix > 36 )
		{
			return E_INVALIDARG;
		}


		if ( bIsNeg )
		{
			// negative, so output '-' and negate
			*buf++ = '-';
			length--;
		}

		if( length <= 0 )
			return E_OUTOFMEMORY;


		char DigitBuffer[128];
		int iFilled = 0;
		do {
			// get next digit
			unsigned digval = (val % radix);
			val = (Type)( val/ radix );

			// convert to ascii and store
			if (digval > 9)
				DigitBuffer[iFilled] = (char) (digval - 10 + 'A');  // a letter
			else
				DigitBuffer[iFilled] = (char) (digval + '0');       // a digit

			iFilled++;
		} while (val > 0 && length > 0 && iFilled < (INT)countof(DigitBuffer));

		if( iFilled >= (INT)countof(DigitBuffer) ) // temp buffer is full
			return StrUtil::StringCpyEx( buf, length, "(Err:TooBigNumber)" );

		// digit limit
		if( iDigit > 0 )
		{
			if( iDigit >= iFilled )
			{
				for( ; iDigit > iFilled; iDigit-- )
				{
					_AppendCharReturn( buf, length, '0' );
				}
			}
			else
			{
				iDigit = 0;
			}
		}
		else
		{
			iDigit = iFilled;
		}

		// reverse copy to dest buffer
		for( ; iFilled > 0 && length > 0 && iDigit > 0; iDigit-- )
		{
			iFilled--;
			_AppendCharReturn( buf, length, DigitBuffer[iFilled] );
		}


		*buf = '\0';            // terminate string;

		return S_OK;
	}



	// 
	HRESULT _IToA( INT8 val,char* &buf, INT& length, unsigned radix, int iDigit )
	{
		bool bIsNeg = false;
		if( val < 0 )
		{
			val = -val;
			bIsNeg = true;
		}

		return _MyIToA( val, buf, length, radix, bIsNeg, iDigit );
	}

	HRESULT _IToA( UINT8 val,char* &buf, INT& length, unsigned radix, int iDigit )
	{
		bool bIsNeg = false;
		return _MyIToA( val, buf, length, radix, bIsNeg, iDigit );
	}

	HRESULT _IToA( INT16 val,char* &buf, INT& length, unsigned radix, int iDigit )
	{
		bool bIsNeg = false;
		if( val < 0 )
		{
			val = -val;
			bIsNeg = true;
		}

		return _MyIToA( val, buf, length, radix, bIsNeg, iDigit );
	}

	HRESULT _IToA( UINT16 val,char* &buf, INT& length, unsigned radix, int iDigit )
	{
		bool bIsNeg = false;
		return _MyIToA( val, buf, length, radix, bIsNeg, iDigit );
	}

	HRESULT _IToA( INT32 val,char* &buf, INT& length, unsigned radix, int iDigit )
	{
		bool bIsNeg = false;
		if(val < 0 && radix == 10) // use sign only for base 10 description
		{
			val = -val;
			bIsNeg = true;
		}

		return _MyIToA( val, buf, length, radix, bIsNeg, iDigit );
	}

	HRESULT _IToA( UINT32 val,char* &buf, INT& length, unsigned radix, int iDigit )
	{
		bool bIsNeg = false;
		return _MyIToA( val, buf, length, radix, bIsNeg, iDigit );
	}

	HRESULT _IToA( INT64 val,char* &buf, INT& length, unsigned radix, int iDigit )
	{
		bool bIsNeg = false;
		if( val < 0 )
		{
			val = -val;
			bIsNeg = true;
		}

		return _MyIToA( val, buf, length, radix, bIsNeg, iDigit );
	}

	HRESULT _IToA( UINT64 val,char* &buf, INT& length, unsigned radix, int iDigit )
	{
		bool bIsNeg = false;
		return _MyIToA( val, buf, length, radix, bIsNeg, iDigit );
	}


	HRESULT _FToA( double val,char* &buf, INT& length, unsigned digit, char Option )
	{
		unused(Option);
#if WINDOWS
		int Decimal = 0, sign = 0;
		char strMantisa[64];
		int digitCount = std::min( (int)digit, (int)(countof(strMantisa)) );

		if( digit < 0 )
			return E_INVALIDARG;

		if( length <= 0 )
			return E_OUTOFMEMORY;

		if( buf == NULL )
			return E_POINTER;

		if( _fcvt_s( strMantisa, countof(strMantisa), val, digitCount, &Decimal, &sign ) != 0 )
			return E_FAIL;

		// discard end zeros
		if( Decimal >= 0 )
			digitCount += Decimal;
		int iPos = 0;
		for( iPos = digitCount -1; iPos >= 0; iPos-- )
		{
			if( strMantisa[iPos] != '0' ) 
			{
				break;
			}
			else
				strMantisa[iPos] = '\0';
		}

		if( iPos < 0 )
			iPos = 0;// at least one zero
		digitCount = iPos+1;


		if( sign ) // negative
		{
			_AppendCharReturn( buf, length, '-' );
		}

		// 
		iPos = 0;
		if( Decimal < 0 ) // 
		{
			_AppendCharReturn( buf, length, '0' );
			_AppendCharReturn( buf, length, '.' );

			int ZeroPadding = std::min( -Decimal, (int)digit );
			for( int iPending = 0; iPending < ZeroPadding && length > 0; iPending++ )
			{
				_AppendCharReturn( buf, length, '0' );
			}
		}
		else
		{
			for( ; iPos < Decimal && length > 0; iPos++ )
			{
				_AppendCharReturn(buf,length,strMantisa[iPos]);
			}

			if( length > 0 )
			{
				_AppendCharReturn(buf,length,'.');
			}

			if( iPos >= digitCount ) // zero padding after dot, if no mantisa remains
				_AppendCharReturn(buf,length,'0');
		}


		for( ; iPos < digitCount && length > 0 && strMantisa[iPos] != 0; iPos++ )
		{
			_AppendCharReturn(buf,length,strMantisa[iPos]);
		}


#else
		auto resultLen = snprintf(buf, length, "%f", val);
		Assert(resultLen > length);
		buf += resultLen;
		length -= resultLen;
#endif

		// Null terminate
		if( length > 0 )
			*buf = '\0';
		else
			*(buf-1) = '\0';

		return S_OK;
	}


	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const INT8& iData, int Option)
	{
		int Radix = 10;
		if (Option == 'x' || Option == 'X')
			Radix = 16;

		_IToA(iData, pBuff, iBuffLen, Radix, -1);

		return S_OK;
	}

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const UINT8& iData, int Option)
	{
		int Radix = 10;
		if (Option == 'x' || Option == 'X')
			Radix = 16;

		_IToA(iData, pBuff, iBuffLen, Radix, -1);

		return S_OK;
	}

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const INT16& iData, int Option)
	{
		int Radix = 10;
		if (Option == 'x' || Option == 'X')
			Radix = 16;

		_IToA(iData, pBuff, iBuffLen, Radix, -1);

		return S_OK;
	}

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const UINT16& iData, int Option)
	{
		int Radix = 10;
		if (Option == 'x' || Option == 'X')
			Radix = 16;

		_IToA(iData, pBuff, iBuffLen, Radix, -1);

		return S_OK;
	}

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const INT32& iData, int Option)
	{
		int Radix = 10;
		if (Option == 'x' || Option == 'X')
			Radix = 16;

		_IToA(iData, pBuff, iBuffLen, Radix, -1);

		return S_OK;
	}

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const UINT32& iData, int Option)
	{
		int Radix = 10;
		if (Option == 'x' || Option == 'X')
			Radix = 16;

		_IToA(iData, pBuff, iBuffLen, Radix, -1);

		return S_OK;
	}

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const INT64& iData, int Option)
	{
		int Radix = 10;
		if (Option == 'x' || Option == 'X')
			Radix = 16;

		_IToA(iData, pBuff, iBuffLen, Radix, -1);

		return S_OK;
	}

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const UINT64& iData, int Option)
	{
		int Radix = 10;
		if (Option == 'x' || Option == 'X')
			Radix = 16;

		_IToA(iData, pBuff, iBuffLen, Radix, -1);

		return S_OK;
	}


	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const LONG& iData, int Option)
	{
		int Radix = 10;
		if (Option == 'x' || Option == 'X')
		{
			Radix = 16;
			_IToA((UINT32)iData, pBuff, iBuffLen, Radix, -1);
		}
		else
		{
			_IToA((INT32)iData, pBuff, iBuffLen, Radix, -1);
		}

		return S_OK;
	}

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const ULONG& iData, int Option)
	{
		int Radix = 10;
		if (Option == 'x' || Option == 'X')
			Radix = 16;

		_IToA((UINT32)iData, pBuff, iBuffLen, Radix, -1);

		return S_OK;
	}

#if WINDOWS
	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const char& Data, int Option)
	{
		if (iBuffLen <= 0)
			return E_OUTOFMEMORY;

		*pBuff++ = Data;
		iBuffLen--;

		if (iBuffLen > 0)
			pBuff[0] = '\0';
		else
			(pBuff - 1)[0] = '\0';

		unused(Option);

		return S_OK;
	}
#endif

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const LPCSTR& Data, int Option)
	{
		unused(Option);
		return StrUtil::StringCpyEx(pBuff, iBuffLen, Data);
	}

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const LPSTR& Data, int Option)
	{
		unused(Option);
		return StrUtil::StringCpyEx(pBuff, iBuffLen, Data);
	}


	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const wchar_t& Data, int Option)
	{
		unused(Option);
		if (iBuffLen <= 0)
			return E_OUTOFMEMORY;

		wchar_t string[2] = { Data, 0 };
		char DestBuff[16];

		if (FAILED(StrUtil::WCSToUTF8(string, DestBuff)))
			return E_FAIL;

		return StrUtil::StringCpyEx(pBuff, iBuffLen, DestBuff);
	}

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const LPCWSTR& Data, int Option)
	{
		char DestBuff[1024];

		unused(Option);

		if (Data == NULL)
			return E_POINTER;

		if (FAILED(StrUtil::WCSToUTF8(Data, DestBuff)))
			return E_FAIL;

		return StrUtil::StringCpyEx(pBuff, iBuffLen, DestBuff);
	}

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const LPWSTR& Data, int Option)
	{
		char DestBuff[1024];

		unused(Option);

		if (Data == NULL)
			return E_POINTER;

		if (FAILED(StrUtil::WCSToUTF8(Data, DestBuff)))
			return E_FAIL;

		return StrUtil::StringCpyEx(pBuff, iBuffLen, DestBuff);
	}


	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const std::string& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, Data.c_str(), Option);
	}

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const std::wstring& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, Data.c_str(), Option);
	}


	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const bool& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, Data ? "true" : "false", Option);
	}

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const float& Data, int Option)
	{
		int degit = Option >> 16;
		_FToA(Data, pBuff, iBuffLen, degit, 0);
		return S_OK;
	}

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const double& Data, int Option)
	{
		int degit = Option >> 16;
		_FToA(Data, pBuff, iBuffLen, degit, (char)(Option & 0xFFFF));

		return S_OK;
	}


	template<void*>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const PVOID& Data, int Option)
	{
		unused(Option);
		auto value = (UINT64)Data;

		StrUtil::StringCpyEx(pBuff, iBuffLen, "0x");

		_IToA(value, pBuff, iBuffLen, 16, sizeof(value) * 2);

		return S_OK;
	}


	template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<INT8>& Data, int Option);
	template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<UINT8>& Data, int Option);
	template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<INT16>& Data, int Option);
	template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<UINT16>& Data, int Option);
	template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<INT32>& Data, int Option);
	template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<UINT32>& Data, int Option);
	template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<INT64>& Data, int Option);
	template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<UINT64>& Data, int Option);



};	// namespace BR


