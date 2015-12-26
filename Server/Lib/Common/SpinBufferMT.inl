////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : Spin buffer for Multi thread
//
////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//
//	Spin Buffer Class
//

#define ITEM typename SpinBufferMT<T,SIZE_BUFFER>::BLOCK


template <typename T, int SIZE_BUFFER>
SpinBufferMT<T, SIZE_BUFFER>::SpinBufferMT(void)
{
}

template <typename T, int SIZE_BUFFER>
SpinBufferMT<T, SIZE_BUFFER>::~SpinBufferMT(void)
{
}

template <typename T, int SIZE_BUFFER>
ITEM* SpinBufferMT<T, SIZE_BUFFER>::Write_Lock()
{
	// lock ticketing
	CounterType myTicket = m_writeTicket.AcquireTicket();


	// Do not cross over the single cycle boundary
	while (((SignedCounterType)(myTicket - m_readTicket.GetWorkingCompleteCount())) >= SIZE_BUFFER)
	{
		ThisThread::SleepFor(DurationMS(0));
	}

	// total waiting thread count
	CounterType nWaitingOrder = m_writeTicket.GetMyWaitingOrder(myTicket);
	while(nWaitingOrder > MAX_COUNTER)
	{
		if(nWaitingOrder - MAX_COUNTER > 10)
		{
			ThisThread::SleepFor(DurationMS(3));
		}
		else
		{
			ThisThread::SleepFor(DurationMS(0));
		}

		// Update Waiting Order value
		nWaitingOrder = m_writeTicket.GetMyWaitingOrder(myTicket);
	}

	int nWritePos = (myTicket-1) % SIZE_BUFFER;

	int nLockTry = 0;
	ITEM_STATE expectedFreeState = ITEM_STATE::STATE_FREE;
	while(!m_SpinBuffer[nWritePos].BlockMode.compare_exchange_weak(expectedFreeState, ITEM_STATE::STATE_WRITE_LOCK, std::memory_order_release, std::memory_order_relaxed))
	{
		nLockTry++;
		if( nLockTry%5 )
		{
			ThisThread::SleepFor(DurationMS(0));
		}
		expectedFreeState = ITEM_STATE::STATE_FREE;
	}
	
	Assert(m_SpinBuffer[nWritePos].BlockMode.load(std::memory_order_relaxed) == ITEM_STATE::STATE_WRITE_LOCK);

	// return buffer block
	return &m_SpinBuffer[nWritePos];
}

template <typename T, int SIZE_BUFFER>
void SpinBufferMT<T,SIZE_BUFFER>::Write_Unlock(ITEM* pItem)
{
	Assert(pItem->BlockMode.load(std::memory_order_relaxed) == ITEM_STATE::STATE_WRITE_LOCK);

	pItem->BlockMode.store(ITEM_STATE::STATE_WRITE_UNLOCK, std::memory_order_release);

	// increase now servig thread count
	m_writeTicket.ReleaseTicket();
}

template <typename T, int SIZE_BUFFER>
ITEM* SpinBufferMT<T, SIZE_BUFFER>::Read_Lock()
{
	// lock ticketing...
	CounterType myTicket = m_readTicket.AcquireTicket();

	// total waiting thread count
	CounterType nWaitingCount = m_readTicket.GetMyWaitingOrder(myTicket);
	while(nWaitingCount > (SIZE_BUFFER>>1))
	{
		if((nWaitingCount - MAX_COUNTER) > 10)
		{
			ThisThread::SleepFor(DurationMS(3));
		}
		else
		{
			ThisThread::SleepFor(DurationMS(0));
		}

		// Update Waiting Order value
		nWaitingCount = m_readTicket.GetMyWaitingOrder(myTicket);
	}

	int nReadPos = (myTicket-1) % SIZE_BUFFER;
	int nLockTry = 0;
	auto expected = ITEM_STATE::STATE_WRITE_UNLOCK;
	while(!m_SpinBuffer[nReadPos].BlockMode.compare_exchange_weak(expected, ITEM_STATE::STATE_READ_LOCK, std::memory_order_release, std::memory_order_relaxed))
	{
		nLockTry++;
		if( nLockTry%5 )
		{
			ThisThread::SleepFor(DurationMS(0));
		}
		expected = ITEM_STATE::STATE_WRITE_UNLOCK;
	}

	return &m_SpinBuffer[nReadPos];
}

template <typename T, int SIZE_BUFFER>
void SpinBufferMT<T, SIZE_BUFFER>::Read_Unlock(ITEM* pItem)
{
	AssertRel(pItem->BlockMode.load(std::memory_order_relaxed) == ITEM_STATE::STATE_READ_LOCK);

	pItem->BlockMode.store(ITEM_STATE::STATE_FREE, std::memory_order_release);

	m_readTicket.ReleaseTicket();
}

// size of buffer writing complet and get ready for reading
template <typename T, int SIZE_BUFFER>
CounterType SpinBufferMT<T, SIZE_BUFFER>::GetReadableCount()
{
	// Already reading started count....
	CounterType nReadWorking = m_readTicket.GetNowWorkingCount();
	
	// Writing Complete Count...
	CounterType	nWriteComplete = m_writeTicket.GetWorkingCompleteCount();

	AssertRel((nWriteComplete - nReadWorking) >= 0);

	return nWriteComplete - nReadWorking;
}

#undef ITEM 
