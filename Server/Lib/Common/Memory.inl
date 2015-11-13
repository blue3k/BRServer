////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Memory library
//	
//
////////////////////////////////////////////////////////////////////////////////



inline STDAllocator& STDAllocator::GetInstance()
{
	return Instance;
}


inline ProcessorAllocator& ProcessorAllocator::GetInstance()
{
	return Instance;
}



////////////////////////////////////////////////////////////////////////////////
//
//	static allocator interface
//   // allocate from the buffer first
//

template< size_t BufferSize >
StaticAllocator<BufferSize>::StaticAllocator(MemoryAllocator& overflowHeap)
	:m_OverflowHeap(overflowHeap)
	,m_AllocatePosition(0)
{
}

template< size_t BufferSize >
StaticAllocator<BufferSize>::~StaticAllocator()
{
}

// Allocate 
template< size_t BufferSize >
HRESULT StaticAllocator<BufferSize>::Alloc( size_t uiSize, void* &pPtr )
{
	if( (m_AllocatePosition + uiSize) < BufferSize )
	{
		pPtr = (void*)(m_AllocationBuffer + m_AllocatePosition);
		m_AllocatePosition += uiSize;
		return S_OK;
	}

	return m_OverflowHeap.Alloc( uiSize, pPtr );
}

template< size_t BufferSize >
bool StaticAllocator<BufferSize>::GetIsInStaticBuffer( void* pPtr )
{
	intptr_t ptr = (intptr_t)pPtr;
	intptr_t staticBuffer = (intptr_t)m_AllocationBuffer;
	return (ptr >= staticBuffer) && (ptr < (staticBuffer + (intptr_t)BufferSize));
}

// Reallocate
template< size_t BufferSize >
HRESULT StaticAllocator<BufferSize>::Realloc( size_t uiSize, void* &pPtr )
{
	bool bIsInStaticBuffer = GetIsInStaticBuffer(pPtr);

	if( bIsInStaticBuffer )
	{
		if( (m_AllocatePosition + uiSize) < BufferSize )
		{
			pPtr = (void*)(m_AllocationBuffer + m_AllocatePosition);
			m_AllocatePosition += uiSize;
			return S_OK;
		}
		else
		{
			// just drop the area
			return m_OverflowHeap.Alloc( uiSize, pPtr );
		}
	}

	return m_OverflowHeap.Alloc( uiSize, pPtr );
}

// Free
template< size_t BufferSize >
HRESULT StaticAllocator<BufferSize>::Free( void* pPtr )
{
	bool bIsInStaticBuffer = GetIsInStaticBuffer(pPtr);
	
	if( bIsInStaticBuffer )
	{
		// just drop the area
	}
	else
	{
		return m_OverflowHeap.Free( pPtr );
	}
	return S_OK;
}






////////////////////////////////////////////////////////////////////////////////
//
//	static allocator interface
//   // allocate from the buffer first
//

template< size_t BufferSize, size_t alignment >
CircularBufferAllocator<BufferSize,alignment>::CircularBufferAllocator(MemoryAllocator& overflowHeap)
	:m_OverflowHeap(overflowHeap)
	,m_AllocatePosition(0)
	,m_FreePosition(0)
	,m_FreeSize(BufferSize)
{
	// Because of the chunk header uses 32bit integer, the limit will be maximum of int32. Actually it's UINT32, but it's not a big deal
	static_assert(BufferSize < INT_MAX,"CircularBufferAllocator: too big buffer size");
}

template< size_t BufferSize, size_t alignment >
CircularBufferAllocator<BufferSize,alignment>::~CircularBufferAllocator()
{
}

// Allocate 
template< size_t BufferSize, size_t alignment >
HRESULT CircularBufferAllocator<BufferSize,alignment>::Alloc( size_t uiSize, void* &pPtr )
{
	assert(uiSize < BufferSize);

	MemoryChunkHeader *pChunk = nullptr;
	intptr_t allocationSize = (intptr_t)BR_ALLIGNUP( HeaderSize+uiSize, alignment );

	if( m_FreeSize < allocationSize )
		return m_OverflowHeap.Alloc(uiSize,pPtr);

	if( (m_AllocatePosition + allocationSize) < BufferSize )
	{
		pChunk = (MemoryChunkHeader*)(m_AllocationBuffer + m_AllocatePosition);

		// If the remain size on the edge isn't enough for next allocation, increase allocation size so that it include dummy area
		if( (m_AllocatePosition + HeaderSize) >= BufferSize )
		{
			allocationSize += BufferSize - (m_AllocatePosition+allocationSize);
		}
	}
	else
	{
		intptr_t remainSize = BufferSize-m_AllocatePosition;

		if( m_FreeSize < (allocationSize+remainSize) )
			return m_OverflowHeap.Alloc(uiSize,pPtr);

		// Add dummy area
		pChunk = (MemoryChunkHeader*)(m_AllocationBuffer + m_AllocatePosition); 
		pChunk->ChunkType = ChunkTypes::Dummy;
		pChunk->ChunkSize = (UINT32)remainSize;
		m_FreeSize -= remainSize;

		pChunk = (MemoryChunkHeader*)(m_AllocationBuffer);
		m_AllocatePosition = 0;
		AssertRel( this->m_AllocatePosition < this->m_FreePosition );

	#ifdef DEBUG
		Assert(SUCCEEDED(ValidateAllocatedChunks()));
	#endif
	}

	m_FreeSize -= allocationSize;

	AssertRel(this->m_FreeSize<=BufferSize && this->m_FreeSize >= 0);

	pChunk->ChunkType = ChunkTypes::Allocated;
	pChunk->ChunkSize = (UINT32)allocationSize;
	pPtr = (void*)((BYTE*)pChunk+HeaderSize);
	m_AllocatePosition += allocationSize;

#ifdef DEBUG
	Assert(SUCCEEDED(ValidateAllocatedChunks()));
#endif
	return S_OK;
}

template< size_t BufferSize, size_t alignment >
bool CircularBufferAllocator<BufferSize,alignment>::GetIsInStaticBuffer( void* pPtr )
{
	intptr_t ptr = (intptr_t)pPtr;
	intptr_t staticBuffer = (intptr_t)m_AllocationBuffer;
	return (ptr >= staticBuffer) && (ptr <= (staticBuffer + (intptr_t)BufferSize));
}

// Get free memory size in static buffer
template< size_t BufferSize, size_t alignment >
size_t CircularBufferAllocator<BufferSize,alignment>::GetFreeMemorySize()
{
	return m_FreeSize;
}


// Reallocate
template< size_t BufferSize, size_t alignment >
HRESULT CircularBufferAllocator<BufferSize,alignment>::Realloc( size_t uiSize, void* &pPtr )
{
	return E_NOTIMPL;
}

// Free
template< size_t BufferSize, size_t alignment >
HRESULT CircularBufferAllocator<BufferSize,alignment>::Free( void* pPtr )
{
	assert(pPtr != nullptr);
	if( pPtr == nullptr ) // null free
		return S_FALSE;

	bool bIsInStaticBuffer = GetIsInStaticBuffer(pPtr);

	if( bIsInStaticBuffer )
	{
		MemoryChunkHeader *pChunk = (MemoryChunkHeader*)((BYTE*)pPtr - HeaderSize);
		assert(m_FreePosition!=m_AllocatePosition || m_FreeSize == 0);

		intptr_t freePointer = (intptr_t)m_AllocationBuffer + m_FreePosition;
		if( freePointer > (intptr_t)pChunk )
		{
			Free( (void*)(freePointer + HeaderSize) );
		}

		assert( pChunk->ChunkType != ChunkTypes::Free );
		AssertRel(pChunk->ChunkType == ChunkTypes::Dummy || pChunk->ChunkType == ChunkTypes::Allocated);
		assert(m_FreePosition!=m_AllocatePosition || m_FreeSize == 0);
		if( pChunk->ChunkType != ChunkTypes::Dummy && pChunk->ChunkType != ChunkTypes::Allocated )
		{
			// Duplicate free? or broken memory
			return S_FALSE;
		}
		pChunk->ChunkType = ChunkTypes::Free;
		if( m_FreePosition != ((intptr_t)pChunk - (intptr_t)m_AllocationBuffer) )
		{
			// If this memory isn't exist in the free position, we should leave it for later
			return S_OK;
		}

		while( (pChunk->ChunkType == ChunkTypes::Free || pChunk->ChunkType == ChunkTypes::Dummy) && m_FreeSize <= BufferSize )
		{
			AssertRel((m_FreePosition == ((intptr_t)pChunk - (intptr_t)m_AllocationBuffer)) || m_FreeSize == 0);
			m_FreeSize += pChunk->ChunkSize;
			m_FreePosition += pChunk->ChunkSize;
			AssertRel(m_FreePosition <= BufferSize);
			if( m_FreePosition == BufferSize ) m_FreePosition = 0;

			if( m_FreeSize < BufferSize ) // Any chunk must be exist
			{
				pChunk = (MemoryChunkHeader*)(m_AllocationBuffer + m_FreePosition);
				if( pChunk->ChunkType != ChunkTypes::Free && pChunk->ChunkType != ChunkTypes::Dummy && pChunk->ChunkType != ChunkTypes::Allocated )
				{
					// Broken memory
					return E_UNEXPECTED;
				}

				// Validate allocated chunks
#ifdef DEBUG
				Assert(SUCCEEDED(ValidateAllocatedChunks()));
#endif
				Assert(m_FreePosition!=m_AllocatePosition);
				AssertRel(pChunk->ChunkType == ChunkTypes::Free || pChunk->ChunkType == ChunkTypes::Dummy || pChunk->ChunkType == ChunkTypes::Allocated);
			}
			else
			{
				break;
			}
		}

		AssertRel(m_FreeSize<=BufferSize);
	}
	else
	{
		return m_OverflowHeap.Free( pPtr );
	}

	return S_OK;
}

// Validate allocated chunks for debug
template< size_t BufferSize, size_t alignment >
HRESULT CircularBufferAllocator<BufferSize,alignment>::ValidateAllocatedChunks()
{
	MemoryChunkHeader* pChunk = nullptr;

	if(m_FreeSize < BufferSize)
	{
		intptr_t curPosition = m_FreePosition;
		do {
			pChunk = (MemoryChunkHeader*)(m_AllocationBuffer + curPosition);
			AssertRel(pChunk->ChunkType == ChunkTypes::Free || pChunk->ChunkType == ChunkTypes::Dummy || pChunk->ChunkType == ChunkTypes::Allocated);
			if( !(pChunk->ChunkType == ChunkTypes::Free || pChunk->ChunkType == ChunkTypes::Dummy || pChunk->ChunkType == ChunkTypes::Allocated) )
				return E_UNEXPECTED;
			curPosition += pChunk->ChunkSize;
			AssertRel( curPosition <= BufferSize );
			if( curPosition == BufferSize ) 
				curPosition = 0;
			AssertRel( GetIsInStaticBuffer(m_AllocationBuffer + curPosition) );
			if( !GetIsInStaticBuffer(m_AllocationBuffer + curPosition) )
				return E_UNEXPECTED;
		} while(curPosition != m_AllocatePosition);
	}

	return S_OK;
}

