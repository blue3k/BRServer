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




////////////////////////////////////////////////////////////////////////////////
//
//	Generic utility functions
//


// Swap byte of data
UINT16 SwapByte( UINT16 uiData )
{
	return ((uiData&0xFF00)>>8) | (uiData<<8);
}

UINT32 SwapByte( UINT32 uiData )
{
	return ( uiData<<24 ) | ((uiData&0xFF00)<<16) | ( uiData>>24 ) | ((uiData>>16)&0xFF00);
}

UINT64 SwapByte( UINT64 uiData )
{
	return (((UINT64)SwapByte((UINT32)uiData))<<32) | ((UINT64)SwapByte((UINT32)(uiData>>32)));
}


template<class DupType>
HRESULT ArrayDup( DupType* &pDest, INT iSrcCount, const DupType* pSrc )
{
	if( pDest != NULL )
	{
		delete[] pDest;
		pDest = NULL;
	}

	if( pSrc )
	{
		pDest = new DupType[ iSrcCount ];
		if( pDest == NULL )
			return E_SYSTEM_OUTOFMEMORY;

		memcpy( pDest, pSrc, iSrcCount*sizeof(DupType) );
	}

	return S_SYSTEM_OK;
}



// MemCopy Data to binary
template< size_t szDstSize, class DataType >
HRESULT MemCpy( BYTE (&Dest)[szDstSize], const DataType& Src )
{
	static_assert( szDstSize >= sizeof(DataType), "Invalid Binary data size" );
	memcpy( Dest, &Src, sizeof(DataType) );
	if( szDstSize < sizeof(DataType) )
		return E_SYSTEM_FAIL;
	return S_SYSTEM_OK;
}


// MemCopy Data From binary
template< size_t szDstSize, class DataType >
HRESULT MemCpy( DataType& Dest, const BYTE (&Src)[szDstSize] )
{
	static_assert( szDstSize >= sizeof(DataType), "Invalid Binary data size" );
	memcpy( &Dest, Src, sizeof(DataType) );
	if( szDstSize < sizeof(DataType) )
		return E_SYSTEM_FAIL;
	return S_SYSTEM_OK;
}


template<class ArrayType>
HRESULT SafeDeleteArray( ArrayType* &pArray )
{
	if( pArray != NULL )
	{
		delete[] pArray;
		pArray = NULL;
	}

	return S_SYSTEM_OK;
}

template<class ValueType>
HRESULT SafeDelete( ValueType* &pObj )
{
	if( pObj != nullptr )
	{
		delete pObj;
		pObj = nullptr;
	}

	return S_SYSTEM_OK;
}

template<class ValueType>
HRESULT SafeRelease( ValueType* &pObj )
{
	if( pObj != nullptr )
	{
		pObj->Release();
		pObj = nullptr;
	}

	return S_SYSTEM_OK;
}


template<class ValueType>
ValueType Min( ValueType val1, ValueType val2 )
{
	return ( ((val1) < (val2)) ? (val1) : (val2) );
}

template<class ValueType>
ValueType Max( ValueType val1, ValueType val2 )
{
	return ( ((val1) > (val2)) ? (val1) : (val2) );
}

template<class ValType>
ValType Abs( ValType value )
{
	return value > 0 ? value : -value;
}

