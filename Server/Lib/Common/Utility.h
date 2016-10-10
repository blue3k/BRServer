////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Utility library
//	
//
////////////////////////////////////////////////////////////////////////////////

#include "Common/Typedefs.h"
#include "Common/ArrayUtil.h"

#pragma once


namespace BR {
namespace Util {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Generic utility macros
	//

	#define BR_MAKEMERSION(verMajor,verMinor)	(((UINT32)(verMajor)<<16) | ((UINT32)(verMajor)))


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Generic utility functions
	//
	// Swap byte of data
	inline UINT16 SwapByte( UINT16 uiData );
	inline UINT32 SwapByte( UINT32 uiData );
	inline UINT64 SwapByte( UINT64 uiData );


	// Make crc32 checksum
	UINT32 Crc32( size_t uiBuffSize, const BYTE* pBuff );
	UINT32 Crc32( UINT32 uiCrc32, size_t uiBuffSize, const BYTE* pBuff );

	// Make crc32 checksum and encrypt
	UINT32 Crc32NEncrypt( size_t uiBuffSize, BYTE* pBuff );

	// Make crc32 checksum and decrypt
	UINT32 Crc32NDecrypt( size_t uiBuffSize, BYTE* pBuff );


	// Base 64 encode/decode
	Result Base64Encode(size_t srcSize, const BYTE* bytes_to_encode, Array<BYTE> &destBuffer, BYTE dummyChar = '\0');
	Result Base64Decode(size_t srcSize, const BYTE* bytes_to_decode, Array<BYTE> &destBuffer, BYTE dummyChar = '=');

	Result Base64URLEncode(size_t srcSize, const BYTE* bytes_to_encode, Array<BYTE> &destBuffer, BYTE dummyChar = '\0');
	Result Base64URLDecode(size_t srcSize, const BYTE* bytes_to_decode, Array<BYTE> &destBuffer, BYTE dummyChar = '=');

	// SHA 256 hashing
	Result SHA256Hash(size_t srcSize, const BYTE* bytes_to_encode, Array<BYTE> &destBuffer);


	// Make power of 2 value from given number, will bigger then input
	UINT NearPowerOf2( UINT32 uiNumber );

	// Service Name
	const WCHAR* GetServiceName();
	const char* GetServiceNameA();
	void SetServiceName(const WCHAR* serviceName);
	void SetServiceName(const char* serviceName);

	// Module Name
	const WCHAR* GetModuleName();
	const char* GetModuleNameA();

	// Module Path
	const WCHAR* GetModulePath();
	const char* GetModulePathA();

	void SetModulePathA(const char* customModulePath, const char* moduleName);

	// Array duplication
	template<class DupType>
	Result ArrayDup( DupType* &pDest, INT iSrcCount, const DupType* pSrc );

	// MemCopy Data to binary
	template< size_t szDstSize, class DataType >
	Result MemCpy( BYTE (&Dest)[szDstSize], const DataType& Src );

	// MemCopy Data From binary
	template< size_t szDstSize, class DataType >
	Result MemCpy( DataType& Dest, const BYTE (&Src)[szDstSize] );


	template<class ArrayType>
	Result SafeDeleteArray( ArrayType* &pArray );

	template<class ValueType>
	Result SafeDelete( ValueType* &pObj );

	template<class ValueType>
	Result SafeRelease( ValueType* &pObj );

	template<class ValueType>
	ValueType Min( ValueType Objmin, ValueType Objmax );

	template<class ValueType>
	ValueType Max( ValueType Objmin, ValueType Objmax );

	template<class ValType>
	ValType Abs( ValType value );

	// Peek key from console
	//Result PeekKey( int &inputKey, NativeHandle hConsole = INVALID_NATIVE_HANDLE_VALUE);

	
#include "Utility.inl"


} // namespace Util
} // namespace BR


