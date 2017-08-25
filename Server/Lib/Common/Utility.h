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
	inline uint16_t SwapByte(uint16_t uiData );
	inline uint32_t SwapByte(uint32_t uiData );
	inline uint64_t SwapByte(uint64_t uiData );


	// Make crc32 checksum
	uint32_t Crc32( size_t uiBuffSize, const BYTE* pBuff );
	uint32_t Crc32(uint32_t uiCrc32, size_t uiBuffSize, const BYTE* pBuff );

	// Make crc32 checksum and encrypt
	uint32_t Crc32NEncrypt( size_t uiBuffSize, BYTE* pBuff );

	// Make crc32 checksum and decrypt
	uint32_t Crc32NDecrypt( size_t uiBuffSize, BYTE* pBuff );


	// Base 64 encode/decode
	Result Base64Encode(size_t srcSize, const BYTE* bytes_to_encode, Array<BYTE> &destBuffer, BYTE dummyChar = '\0');
	Result Base64Decode(size_t srcSize, const BYTE* bytes_to_decode, Array<BYTE> &destBuffer, BYTE dummyChar = '=');

	Result Base64URLEncode(size_t srcSize, const BYTE* bytes_to_encode, Array<BYTE> &destBuffer, BYTE dummyChar = '\0');
	Result Base64URLDecode(size_t srcSize, const BYTE* bytes_to_decode, Array<BYTE> &destBuffer, BYTE dummyChar = '=');

	// SHA 256 hashing
	Result SHA256Hash(size_t srcSize, const BYTE* bytes_to_encode, Array<BYTE> &destBuffer);

	// Find min bit can embrace the number
	uint32_t FindMinBitShift(uint32_t number);

	// Make power of 2 value from given number, will bigger then input
		uint32_t NearPowerOf2(uint32_t uiNumber );

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

	void SetModulePath(const char* customModulePath, const char* moduleName);

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

	// Android doesn't support log2
	template<class ValType>
	inline ValType Log2(ValType x) { return log(x) / 1.4426950408889634; }

	// Peek key from console
	//Result PeekKey( int &inputKey, NativeHandle hConsole = INVALID_NATIVE_HANDLE_VALUE);

	
#include "Utility.inl"


} // namespace Util
} // namespace BR


