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
#include "Common/ResultCode/BRResultCodeSystem.h"


namespace BR {


	///////////////////////////////////////////////////////////////////////////////////
	//
	// ToString helper implementations
	//


	// 
	Result _IToA( INT8 val,char* &buf, INT& length, unsigned radix, int iDigit );
	Result _IToA( UINT8 val,char* &buf, INT& length, unsigned radix, int iDigit );
	Result _IToA( INT16 val,char* &buf, INT& length, unsigned radix, int iDigit );
	Result _IToA( UINT16 val,char* &buf, INT& length, unsigned radix, int iDigit );
	Result _IToA( INT32 val,char* &buf, INT& length, unsigned radix, int iDigit );
	Result _IToA( UINT32 val,char* &buf, INT& length, unsigned radix, int iDigit );
	Result _IToA( INT64 val,char* &buf, INT& length, unsigned radix, int iDigit );
	Result _IToA( UINT64 val,char* &buf, INT& length, unsigned radix, int iDigit );


	Result _FToA( double val,char* &buf, INT& length, unsigned digit, char Option );
	
	///////////////////////////////////////////////////////////////////////////////////
	//
	// Default tostring implementation
	//

#define TypeName2(type) type
#define TypeName(type)	TypeName2(type)

	
	template< class Type >
	inline Result ToString( char*& pBuff, INT& iBuffLen, const Type& iData, int Option )
	{
		static const char Unknown[]="UnknownType:";
		Result hr = ::BR::StrUtil::StringCpyEx( pBuff, iBuffLen, Unknown );
		if( SUCCEEDED(hr) )
			::BR::StrUtil::StringCpyEx( pBuff, iBuffLen, typeid(Type).name() );
#if WINDOWS
		// If you got this warnning fix type or add type ToString implementation
COMPILETIME_WARNING( "ToString Compiled with unknowntype" + typeid(Type).name() )
#endif
		Assert(!"InvalidType ToString");
		return SUCCEEDED(hr) ? ResultCode::FAIL : hr;
	}



	///////////////////////////////////////////////////////////////////////////////////
	//
	// Default type implementations
	//

	template<> Result ToString(char*& pBuff, INT& iBuffLen, const INT8& Data, int Option);
	template<> Result ToString(char*& pBuff, INT& iBuffLen, const UINT8& Data, int Option);

	template<> Result ToString(char*& pBuff, INT& iBuffLen, const INT16& Data, int Option);
	template<> Result ToString(char*& pBuff, INT& iBuffLen, const UINT16& Data, int Option);

	template<> Result ToString(char*& pBuff, INT& iBuffLen, const INT32& Data, int Option);
	template<> Result ToString(char*& pBuff, INT& iBuffLen, const UINT32& Data, int Option);

	template<> Result ToString(char*& pBuff, INT& iBuffLen, const INT64& Data, int Option);
	template<> Result ToString(char*& pBuff, INT& iBuffLen, const UINT64& Data, int Option);

	template<> Result ToString(char*& pBuff, INT& iBuffLen, const LONG& Data, int Option);
	template<> Result ToString(char*& pBuff, INT& iBuffLen, const ULONG& Data, int Option);

	template<> Result ToString(char*& pBuff, INT& iBuffLen, const char& Data, int Option);
	template<> Result ToString(char*& pBuff, INT& iBuffLen, const LPCSTR& Data, int Option);
	template<> Result ToString(char*& pBuff, INT& iBuffLen, const LPSTR& Data, int Option);
	template<> Result ToString(char*& pBuff, INT& iBuffLen, const wchar_t& Data, int Option);
	template<> Result ToString(char*& pBuff, INT& iBuffLen, const LPCWSTR& Data, int Option);
	template<> Result ToString(char*& pBuff, INT& iBuffLen, const LPWSTR& Data, int Option);

	template<> Result ToString(char*& pBuff, INT& iBuffLen, const std::string& Data, int Option);
	template<> Result ToString(char*& pBuff, INT& iBuffLen, const std::wstring& Data, int Option);

	template<> Result ToString(char*& pBuff, INT& iBuffLen, const bool& Data, int Option);

	template<> Result ToString(char*& pBuff, INT& iBuffLen, const float& Data, int Option);
	template<> Result ToString(char*& pBuff, INT& iBuffLen, const double& Data, int Option);

	template<> Result ToString(char*& pBuff, INT& iBuffLen, const PVOID& Data, int Option);

	template<> Result ToString(char*& pBuff, INT& iBuffLen, const BR::Result& Data, int Option);




	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Tostring helper
	//

	// specialized version
	inline Result ToStringHex( char*& pBuff, INT& iBuffLen, UINT32 iData, float MaxDigit, int Radix )
	{
		_IToA( (UINT32)iData, pBuff, iBuffLen, Radix, (int)MaxDigit );

		return ResultCode::SUCCESS;
	}

	inline Result ToStringHex( char*& pBuff, INT& iBuffLen, UINT64 iData, float MaxDigit, int Radix )
	{
		_IToA((UINT32)(iData>>32), pBuff, iBuffLen, Radix, (int)MaxDigit);
		StrUtil::StringCpyEx(pBuff, iBuffLen, ":");
		_IToA((UINT32)iData, pBuff, iBuffLen, Radix, (int)MaxDigit);

		return ResultCode::SUCCESS;
	}

	// enumeration to string
	template< class EnumType >
	inline Result ToStringEnum( char*& pBuff, INT& iBuffLen, EnumType Data, int Radix = 10 )
	{
		//if( FAILED( ToString( pBuff, iBuffLen, typeid(Data).name(), 0 ) ) )
		//	return ResultCode::FAIL;

		if( FAILED( StrUtil::StringCpyEx( pBuff, iBuffLen, "(" ) ) )
			return ResultCode::FAIL;

		_IToA( (UINT32)Data, pBuff, iBuffLen, Radix, -1 );

		if( FAILED( StrUtil::StringCpyEx( pBuff, iBuffLen, ")" ) ) )
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}

	// enumeration to string
	template< class ArrayType >
	inline Result ToStringArray( char*& pBuff, INT& iBuffLen, size_t szArray, const ArrayType* pData, int Radix = 10 )
	{
		if( pData == nullptr )
			return ResultCode::FAIL;

		if( FAILED(_IToA( (UINT32)szArray, pBuff, iBuffLen, 10, -1 )) )
			return ResultCode::FAIL;

		for( size_t uiIdx = 0; uiIdx < szArray; uiIdx++ )
		{
			if( FAILED( StrUtil::StringCpyEx( pBuff, iBuffLen, ":" ) ) )
				return ResultCode::FAIL;

			ToString( pBuff, iBuffLen, pData[uiIdx], 0 );
		}

		return ResultCode::SUCCESS;
	}


	// enumeration to string
	template< class ArrayType >
	inline Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<ArrayType>& pData, int Radix = 10)
	{
		auto szArray = pData.GetSize();

		if (FAILED(_IToA((UINT32)szArray, pBuff, iBuffLen, 10, -1)))
			return ResultCode::FAIL;

		for (size_t uiIdx = 0; uiIdx < szArray; uiIdx++)
		{
			if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
				return ResultCode::FAIL;

			ToString(pBuff, iBuffLen, pData[uiIdx], 0);
		}

		return ResultCode::SUCCESS;
	}

	extern template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<INT8>& Data, int Option);
	extern template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<UINT8>& Data, int Option);
	extern template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<INT16>& Data, int Option);
	extern template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<UINT16>& Data, int Option);
	extern template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<INT32>& Data, int Option);
	extern template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<UINT32>& Data, int Option);
	extern template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<INT64>& Data, int Option);
	extern template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<UINT64>& Data, int Option);
#if WINDOWS
	extern template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<LONG>& Data, int Option);
	extern template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<ULONG>& Data, int Option);
	extern template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<char>& Data, int Option);
#endif

	template<>
	inline Result ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<BYTE>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	template<>
	inline Result ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<UINT32>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

}; // namespace BR



