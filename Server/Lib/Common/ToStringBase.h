////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : Base type ToString implementation
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/StrUtil.h"
#include "Common/ArrayUtil.h"


namespace BR {


	///////////////////////////////////////////////////////////////////////////////////
	//
	// ToString helper implementations
	//


	// 
	HRESULT _IToA( INT8 val,char* &buf, INT& length, unsigned radix, int iDigit );
	HRESULT _IToA( UINT8 val,char* &buf, INT& length, unsigned radix, int iDigit );
	HRESULT _IToA( INT16 val,char* &buf, INT& length, unsigned radix, int iDigit );
	HRESULT _IToA( UINT16 val,char* &buf, INT& length, unsigned radix, int iDigit );
	HRESULT _IToA( INT32 val,char* &buf, INT& length, unsigned radix, int iDigit );
	HRESULT _IToA( UINT32 val,char* &buf, INT& length, unsigned radix, int iDigit );
	HRESULT _IToA( INT64 val,char* &buf, INT& length, unsigned radix, int iDigit );
	HRESULT _IToA( UINT64 val,char* &buf, INT& length, unsigned radix, int iDigit );


	HRESULT _FToA( double val,char* &buf, INT& length, unsigned digit, char Option );
	
	///////////////////////////////////////////////////////////////////////////////////
	//
	// Default tostring implementation
	//

#define TypeName2(type) type
#define TypeName(type)	TypeName2(type)

	
	template< class Type >
	inline HRESULT ToString( char*& pBuff, INT& iBuffLen, const Type& iData, int Option )
	{
		static const char Unknown[]="UnknownType:";
		HRESULT hr = ::BR::StrUtil::StringCpyEx( pBuff, iBuffLen, Unknown );
		if( SUCCEEDED(hr) )
			::BR::StrUtil::StringCpyEx( pBuff, iBuffLen, typeid(Type).name() );

		// If you got this warnning fix type or add type ToString implementation
COMPILETIME_WARNING( "ToString Compiled with unknowntype" + typeid(Type).name() )

		//Assert(!"InvalidType ToString");
		return SUCCEEDED(hr) ? E_FAIL : hr;
	}



	///////////////////////////////////////////////////////////////////////////////////
	//
	// Default type implementations
	//

	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const INT8& Data, int Option);
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const UINT8& Data, int Option);

	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const INT16& Data, int Option);
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const UINT16& Data, int Option);

	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const INT32& Data, int Option);
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const UINT32& Data, int Option);

	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const INT64& Data, int Option);
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const UINT64& Data, int Option);

	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const LONG& Data, int Option);
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const ULONG& Data, int Option);

	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const char& Data, int Option);
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const LPCSTR& Data, int Option);
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const LPSTR& Data, int Option);
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const wchar_t& Data, int Option);
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const LPCWSTR& Data, int Option);
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const LPWSTR& Data, int Option);

	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const std::string& Data, int Option);
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const std::wstring& Data, int Option);

	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const float& Data, int Option);
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const double& Data, int Option);

	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const PVOID& Data, int Option);




	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Tostring helper
	//

	// specialized version
	inline HRESULT ToStringHex( char*& pBuff, INT& iBuffLen, UINT32 iData, float MaxDigit, int Radix )
	{
		_IToA( (UINT32)iData, pBuff, iBuffLen, Radix, (int)floor(MaxDigit) );

		return S_OK;
	}

	inline HRESULT ToStringHex( char*& pBuff, INT& iBuffLen, UINT64 iData, float MaxDigit, int Radix )
	{
		_IToA((UINT32)(iData>>32), pBuff, iBuffLen, Radix, (int)floor(MaxDigit));
		StrUtil::StringCpyEx(pBuff, iBuffLen, ":");
		_IToA((UINT32)iData, pBuff, iBuffLen, Radix, (int)floor(MaxDigit));

		return S_OK;
	}

	// enumeration to string
	template< class EnumType >
	inline HRESULT ToStringEnum( char*& pBuff, INT& iBuffLen, EnumType Data, int Radix = 10 )
	{
		//if( FAILED( ToString( pBuff, iBuffLen, typeid(Data).name(), 0 ) ) )
		//	return E_FAIL;

		if( FAILED( StrUtil::StringCpyEx( pBuff, iBuffLen, "(" ) ) )
			return E_FAIL;

		_IToA( (UINT32)Data, pBuff, iBuffLen, Radix, -1 );

		if( FAILED( StrUtil::StringCpyEx( pBuff, iBuffLen, ")" ) ) )
			return E_FAIL;

		return S_OK;
	}

	// enumeration to string
	template< class ArrayType >
	inline HRESULT ToStringArray( char*& pBuff, INT& iBuffLen, size_t szArray, const ArrayType* pData, int Radix = 10 )
	{
		if( pData == nullptr )
			return E_FAIL;

		if( FAILED(_IToA( szArray, pBuff, iBuffLen, 10, -1 )) )
			return E_FAIL;

		for( size_t uiIdx = 0; uiIdx < szArray; uiIdx++ )
		{
			if( FAILED( StrUtil::StringCpyEx( pBuff, iBuffLen, ":" ) ) )
				return E_FAIL;

			ToString( pBuff, iBuffLen, pData[uiIdx], 0 );
		}

		return S_OK;
	}


	// enumeration to string
	template< class ArrayType >
	inline HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<ArrayType>& pData, int Radix = 10)
	{
		auto szArray = pData.GetSize();

		if (FAILED(_IToA(szArray, pBuff, iBuffLen, 10, -1)))
			return E_FAIL;

		for (size_t uiIdx = 0; uiIdx < szArray; uiIdx++)
		{
			if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
				return E_FAIL;

			ToString(pBuff, iBuffLen, pData[uiIdx], 0);
		}

		return S_OK;
	}

	extern template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<INT8>& Data, int Option);
	extern template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<UINT8>& Data, int Option);
	extern template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<INT16>& Data, int Option);
	extern template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<UINT16>& Data, int Option);
	extern template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<INT32>& Data, int Option);
	extern template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<UINT32>& Data, int Option);
	extern template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<INT64>& Data, int Option);
	extern template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<UINT64>& Data, int Option);

	template<>
	inline HRESULT ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<BYTE>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	template<>
	inline HRESULT ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<UINT32>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

}; // namespace BR



