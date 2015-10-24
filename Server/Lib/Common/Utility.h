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


#pragma once

#include "Common/Typedefs.h"
#include "Common/ArrayUtil.h"

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
	HRESULT Base64Encode(size_t srcSize, const BYTE* bytes_to_encode, Array<BYTE> &destBuffer, BYTE dummyChar = '\0');
	HRESULT Base64Decode(size_t srcSize, const BYTE* bytes_to_decode, Array<BYTE> &destBuffer, BYTE dummyChar = '=');

	HRESULT Base64URLEncode(size_t srcSize, const BYTE* bytes_to_encode, Array<BYTE> &destBuffer, BYTE dummyChar = '\0');
	HRESULT Base64URLDecode(size_t srcSize, const BYTE* bytes_to_decode, Array<BYTE> &destBuffer, BYTE dummyChar = '=');

	// SHA 256 hashing
	HRESULT SHA256Hash(size_t srcSize, const BYTE* bytes_to_encode, Array<BYTE> &destBuffer);


	// Make power of 2 value from given number, will bigger then input
	UINT NearPowerOf2( UINT32 uiNumber );

	// Service Name
	const WCHAR* GetServiceName();
	const char* GetServiceNameA();
	void SetServiceName(const WCHAR* serviceName);
	void SetServiceNameA(const char* serviceName);

	// Module Name
	const WCHAR* GetModuleName();
	const char* GetModuleNameA();

	// Module Path
	const WCHAR* GetModulePath();
	const char* GetModulePathA();

	// Array duplication
	template<class DupType>
	HRESULT ArrayDup( DupType* &pDest, INT iSrcCount, const DupType* pSrc );

	// MemCopy Data to binary
	template< size_t szDstSize, class DataType >
	HRESULT MemCpy( BYTE (&Dest)[szDstSize], const DataType& Src );

	// MemCopy Data From binary
	template< size_t szDstSize, class DataType >
	HRESULT MemCpy( DataType& Dest, const BYTE (&Src)[szDstSize] );


	template<class ArrayType>
	HRESULT SafeDeleteArray( ArrayType* &pArray );

	template<class ValueType>
	HRESULT SafeDelete( ValueType* &pObj );

	template<class ValueType>
	HRESULT SafeRelease( ValueType* &pObj );

	template<class ValueType>
	ValueType Min( ValueType Objmin, ValueType Objmax );

	template<class ValueType>
	ValueType Max( ValueType Objmin, ValueType Objmax );

	template<class ValType>
	ValType Abs( ValType value );

	// Peek key from console
	HRESULT PeekKey( int &inputKey, HANDLE hConsole = INVALID_HANDLE_VALUE );

	
#include "Utility.inl"


} // namespace Util
} // namespace BR


