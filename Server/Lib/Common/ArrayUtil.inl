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



////////////////////////////////////////////////////////////////////////////////////////////
//
//	static buffer class
//

// Static buffer
template< class DataType >
Array<DataType>::Array()
	:m_Size(0),
	m_AllocatedSize(0),
	m_IncreaseSize(10),
	m_pDataPtr(nullptr),
	m_ConserveDataOnResize(true)
{
}

template< class DataType >
Array<DataType>::~Array()
{
}


// Update Data pointer
template< class DataType >
void Array<DataType>::SetBuffPtr( size_t AllocatedSize, DataType *pDataPtr )
{
	AssertRel( AllocatedSize>=m_Size );

	m_AllocatedSize = AllocatedSize;
	if (m_ConserveDataOnResize && m_pDataPtr != pDataPtr)
	{
		for( size_t iData=0; iData < m_Size; iData++ )
			pDataPtr[iData] = m_pDataPtr[iData];
	}
	m_pDataPtr = pDataPtr;
}


// Get Size
template< class DataType >
size_t Array<DataType>::GetSize() const
{
	return m_Size;
}

// set Reserve size
template< class DataType >
HRESULT Array<DataType>::SetSize( size_t szNewSize )
{
	auto hr = Reserve(szNewSize);
	if (SUCCEEDED(hr))
		m_Size = szNewSize;
	return hr;
}

// Clear array, buffer size not reallocated, buffer data didn't erased, clear manually if need
template< class DataType >
void Array<DataType>::Clear()
{
	m_Size = 0;
}

// ConserveDataOnResize
template< class DataType >
bool Array<DataType>::GetConserveDataOnResize() const
{
	return m_ConserveDataOnResize;
}

template< class DataType >
void Array<DataType>::SetConserveDataOnResize(bool conserveDataOnResize)
{
	m_ConserveDataOnResize = conserveDataOnResize;
}


// Get Current allocated Size
template< class DataType >
size_t Array<DataType>::GetAllocatedSize() const
{
	return m_AllocatedSize;
}
		
// Get Array Increase Size
template< class DataType >
size_t Array<DataType>::GetIncreaseSize() const
{
	return m_IncreaseSize;
}

template< class DataType >
void Array<DataType>::SetIncreaseSize( size_t szNewIncSize )
{
	Assert( szNewIncSize > 0 );

	// At least increase one
	if( szNewIncSize == 0 )
		szNewIncSize = 1;

	m_IncreaseSize = szNewIncSize;
}

// Get data pointer
template< class DataType >
const DataType* Array<DataType>::data() const
{
	return m_pDataPtr;
}
template< class DataType >
DataType* Array<DataType>::data()
{
	return m_pDataPtr;
}


// push_back
template< class DataType >
inline HRESULT Array<DataType>::push_back( const DataType& NewData )
{
	HRESULT hr = S_OK;
	if( GetSize() ==  GetAllocatedSize() )
	{
		hr = IncreaseSize();
		if( FAILED(hr) ) return hr;
	}

	Assert( GetSize() <  GetAllocatedSize() );

	m_pDataPtr[m_Size] = NewData;
	m_Size++;

	return hr;
}

//
//template< class DataType >
//HRESULT Array<DataType>::operator +=( const DataType& NewData )
//{
//	HRESULT hr = S_OK;
//	if( GetSize() ==  GetAllocatedSize() )
//	{
//		hr = IncreaseSize();
//		if( FAILED(hr) ) return hr;
//	}
//
//	Assert( GetSize() <  GetAllocatedSize() );
//
//	m_pDataPtr[m_Size] = NewData;
//	m_Size++;
//
//	return hr;
//}

template< class DataType >
inline HRESULT Array<DataType>::AddItems(size_t numItems, const DataType* NewData)
{
	HRESULT hr = S_OK;
	auto newSize = numItems + GetSize();
	if (newSize > GetAllocatedSize())
	{
		hr = Reserve(newSize + GetIncreaseSize());
		if( FAILED(hr) ) return hr;
	}

	Assert(newSize <=  GetAllocatedSize());

	for (UINT iItem = 0; iItem < numItems; iItem++)
	{
		m_pDataPtr[m_Size] = NewData[iItem];
		m_Size++;
	}

	return hr;
}


// Remove element
template< class DataType >
HRESULT Array<DataType>::RemoveAt( INT iIndex )
{
	if (iIndex < 0 || (decltype(m_Size))iIndex >= m_Size)
		return E_INVALIDARG;

	for (INT iIdx = iIndex + 1; (decltype(m_Size))iIdx < m_Size; iIdx++)
	{
		m_pDataPtr[iIdx-1] = m_pDataPtr[iIdx];
	}
	m_Size--;

	return S_OK;
}

template< class DataType >
HRESULT Array<DataType>::RemoveItem( const DataType& RemoveData )
{
	INT iIdx = FindItem( RemoveData );

	if( iIdx >= 0 )
		return RemoveAt( iIdx );

	return S_OK;
}

// find
template< class DataType >
INT Array<DataType>::FindItem( const DataType& FindData )
{
	for (INT iIdx = 0; (decltype(m_Size))iIdx < m_Size; iIdx++)
	{
		if( m_pDataPtr[iIdx] == FindData )
			return iIdx;
	}
	return -1;
}


// Element access operator
template< class DataType >
DataType& Array<DataType>::operator []( size_t iElement ) const
{
	// This is abnormal operation
	AssertRel(iElement < m_Size);
	return m_pDataPtr[iElement];
}


template< class DataType >
const DataType& Array<DataType>::GetAt(UINT iElement) const
{
	// This is abnormal operation
	AssertRel(iElement < m_Size);
	return m_pDataPtr[iElement];
}

// Called for reallocation
template< class DataType >
HRESULT Array<DataType>::IncreaseSize()
{
	size_t szNewSize = GetAllocatedSize() + GetIncreaseSize();
	Assert(szNewSize > GetAllocatedSize());

	return Reserve(szNewSize);
}

template< class DataType >
DataType& Array<DataType>::operator []( size_t iElement )
{
	// This is abnormal operation
	Assert(iElement < m_Size);
	// Auto resize if not assert at above
	while( iElement >= m_Size )
	{
		AssertRel( iElement < (USHRT_MAX*2) ); // if too big
		IncreaseSize();
		m_Size = std::min( iElement+1, GetAllocatedSize() );
	}

	return m_pDataPtr[iElement];
}




////////////////////////////////////////////////////////////////////////////////////////////
//
//	Static Array Variable class
//


template< class DataType, size_t DefaultBufferSize >
StaticArray<DataType,DefaultBufferSize>::StaticArray()
	:m_pAllocatedBuffer(nullptr)
{
	__super::SetBuffPtr( DefaultBufferSize, m_pDefautlBuffer );
}


template< class DataType, size_t DefaultBufferSize >
StaticArray<DataType,DefaultBufferSize>::~StaticArray()
{
	if( m_pAllocatedBuffer )
		delete[] m_pAllocatedBuffer;
}

// reallocate
template< class DataType, size_t DefaultBufferSize >
HRESULT StaticArray<DataType, DefaultBufferSize>::Reserve(size_t szNewSize)
{
	DataType *pNewBuffer = nullptr;
	DataType *pOldBuffer = nullptr;

	if( szNewSize <= GetAllocatedSize() )
		return S_FALSE;

	pNewBuffer = new DataType[szNewSize];
	if( pNewBuffer == nullptr )
		return E_OUTOFMEMORY;
	SetBuffPtr( szNewSize, pNewBuffer );

	pOldBuffer = m_pAllocatedBuffer;
	m_pAllocatedBuffer = pNewBuffer;
	if( pOldBuffer )
	{
		delete[] pOldBuffer;
	}

	return S_OK;
}




////////////////////////////////////////////////////////////////////////////////////////////
//
//	Dynamic Array Variable class
//


template< class DataType >
DynamicArray<DataType>::DynamicArray(size_t increaseSize)
:m_pAllocatedBuffer(nullptr)
{
	SetIncreaseSize(increaseSize);
}


template< class DataType >
DynamicArray<DataType>::~DynamicArray()
{
	if (m_pAllocatedBuffer)
		delete[] m_pAllocatedBuffer;
}

// reallocate
template< class DataType >
HRESULT DynamicArray<DataType>::Reserve(size_t szNewSize)
{
	DataType *pNewBuffer = nullptr;
	DataType *pOldBuffer = nullptr;

	if (szNewSize <= GetAllocatedSize())
		return S_FALSE;

	pNewBuffer = new DataType[szNewSize];
	if (pNewBuffer == nullptr)
		return E_OUTOFMEMORY;
	SetBuffPtr(szNewSize, pNewBuffer);

	pOldBuffer = m_pAllocatedBuffer;
	m_pAllocatedBuffer = pNewBuffer;
	if (pOldBuffer)
	{
		delete[] pOldBuffer;
	}

	return S_OK;
}



template< class DataType >
LinkedArray<DataType>::LinkedArray()
{

}

template< class DataType >
LinkedArray<DataType>::LinkedArray(UINT maxDataCount, UINT dataCount, DataType* pDataPtr)
{
	SetLinkedBuffer(maxDataCount, dataCount, pDataPtr);
}

template< class DataType >
LinkedArray<DataType>::~LinkedArray()
{
}

template< class DataType >
void LinkedArray<DataType>::SetLinkedBuffer(UINT maxDataCount, UINT dataCount, DataType* pDataPtr)
{
	__super::SetBuffPtr(maxDataCount, pDataPtr);
	SetSize(dataCount);
}

template< class DataType >
void LinkedArray<DataType>::SetLinkedBuffer(const Array<DataType>& srcLink)
{
	__super::SetBuffPtr(srcLink.GetAllocatedSize(), const_cast<BYTE*>(srcLink.data()));
	SetSize(srcLink.GetSize());
	memcpy(data(), srcLink.data(), sizeof(DataType) * srcLink.GetSize());
}

template< class DataType >
HRESULT LinkedArray<DataType>::Reserve(size_t szReserv)
{
	if (szReserv <= GetAllocatedSize())
		return S_OK;

	return E_FAIL;
}




