////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun, Ko
//
// Description : Spin shared buffer for multithired thread 
//
////////////////////////////////////////////////////////////////////////////////


template <class ItemType>
SpinSharedBuffer<ItemType>::SpinSharedBuffer( UINT BufferCount )
	:m_SpinBuffer(nullptr),
	m_BufferCount(0)
{
	if( BufferCount > 0 )
		SetBufferCount( BufferCount );
}

template <class ItemType>
SpinSharedBuffer<ItemType>::~SpinSharedBuffer(void)
{
	if( m_SpinBuffer )
		delete[] m_SpinBuffer;
}

// Set buffer count, all pervious buffer data will be lost
template <class ItemType>
HRESULT SpinSharedBuffer<ItemType>::SetBufferCount(  UINT BufferCount  )
{
	if( BufferCount == 0 )
		return E_INVALIDARG;

	if( m_SpinBuffer )
		delete[] m_SpinBuffer;

	m_BufferCount = Util::NearPowerOf2( BufferCount );

	m_SpinBuffer = new Buffer[m_BufferCount];
	if( m_SpinBuffer == nullptr )
		return E_OUTOFMEMORY;

	return S_OK;
}

// Get buffer count
template <class ItemType>
UINT SpinSharedBuffer<ItemType>::GetBufferCount()
{
	return m_BufferCount;
}

// Get used buffer count
template <class ItemType>
CounterType SpinSharedBuffer<ItemType>::GetUsedBufferCount()
{
	return m_UsedBufferCount;
}

// Try to get one buffer while given times
template <class ItemType>
HRESULT SpinSharedBuffer<ItemType>::TryAllocBuffer( INT iTryCount, ItemType* &pBuffer )
{
	if( m_SpinBuffer == nullptr )
		return E_INVALIDARG;

	// get access ticket
	CounterType myTicket = m_AccessPosition.fetch_add(1, std::memory_order_relaxed) + 1;

	// Calculate access position
	CounterType accessPos = myTicket % m_BufferCount;

	INT iTry = 0;

	do {
		// If i didn't get it's access right then reticketing new one
		while( m_SpinBuffer[accessPos].State != Buffer::STATE_FREE )
		{
			// Try limit
			iTry++;
			if( iTry > iTryCount )
				return E_FAIL;

			// if enguaged to maximum buffer state
			if( m_UsedBufferCount >= m_BufferCount )
				return E_OUTOFMEMORY;

			myTicket = m_AccessPosition.fetch_add(1, std::memory_order_relaxed) + 1;
			accessPos = myTicket % m_BufferCount;
		}
	} while( _InterlockedCompareExchange( &m_SpinBuffer[accessPos].State, Buffer::STATE_USE, Buffer::STATE_FREE ) != Buffer::STATE_FREE );

	pBuffer = &m_SpinBuffer[accessPos].Data;

	// Increase used buffer count
	m_UsedBufferCount.fetch_add(1, std::memory_order_relaxed);

	// Writing success!
	return S_OK; 
}

// Get free buffer
template <class ItemType>
HRESULT SpinSharedBuffer<ItemType>::AllocBuffer( ItemType* &pBuffer )
{
		if( m_SpinBuffer == nullptr )
		return E_INVALIDARG;

	// get access ticket
		CounterType myTicket = m_AccessPosition.fetch_add(1, std::memory_order_relaxed) + 1;

	// Calculate access position
	CounterType accessPos = myTicket % m_BufferCount;

	INT iTry = 0;

	do {
		// If i didn't get it's access right then reticketing new one
		while( m_SpinBuffer[accessPos].State != Buffer::STATE_FREE )
		{
			iTry++;
			// So many try sleep some
			if( iTry > 5 )
				Sleep(0);


			myTicket = m_AccessPosition.fetch_add(1, std::memory_order_relaxed) + 1;
			accessPos = myTicket % m_BufferCount;
		}
	} while( _InterlockedCompareExchange( &m_SpinBuffer[accessPos].State, Buffer::STATE_USE, Buffer::STATE_FREE ) != Buffer::STATE_FREE );

	// Increase used buffer count
	m_UsedBufferCount.Increment();

	// Writing success!
	return S_OK; 
}

// Free given buffer
template <class ItemType>
HRESULT SpinSharedBuffer<ItemType>::FreeBuffer( ItemType* pBuffer )
{
	if( pBuffer == NULL )
		return E_FAIL;

	// Offset calculation for Buffer
	Buffer *pBufferPtr = (Buffer*)( ((BYTE*)pBuffer) + (intptr_t)(&m_SpinBuffer[0]) - (intptr_t)(&m_SpinBuffer[0].Data) );

	if( pBufferPtr < m_SpinBuffer || pBufferPtr > (m_SpinBuffer + m_BufferCount) )
		return E_INVALIDARG;

	pBufferPtr->State = Buffer::STATE_FREE;

	m_UsedBufferCount.fetch_sub(1, std::memory_order_relaxed);

	return S_OK;
}


