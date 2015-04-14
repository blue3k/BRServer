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

#define ITEM typename SpinBufferMT<T, SIZE_BUFFER>::BLOCK


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
	
	// total waiting thread count
	CounterType nWaitingOrder = m_writeTicket.GetMyWaitingOrder(myTicket);
	while(nWaitingOrder > MAX_COUNTER)
	{
		if(nWaitingOrder - MAX_COUNTER > 10)
		{
			Sleep(3);
		}
		else
		{
			Sleep(0);
		}

		// Update Waiting Order value
		nWaitingOrder = m_writeTicket.GetMyWaitingOrder(myTicket);
	}

	int nWritePos = (myTicket-1) % SIZE_BUFFER;

	int nLockTry = 0;
	while( (_InterlockedCompareExchange64((LONG64*)&m_SpinBuffer[nWritePos].eBlockMode, ITEM::STATE_WRITE_LOCK, ITEM::STATE_FREE) != (ITEM::STATE_FREE)) )
	{
		nLockTry++;
		if( nLockTry%5 )
		{
			Sleep(0);
		}
	}
	
	// return buffer block
	return &m_SpinBuffer[nWritePos];
}

template <typename T, int SIZE_BUFFER>
void SpinBufferMT<T, SIZE_BUFFER>::Write_Unlock(ITEM* pItem)
{
	AssertRel(pItem->eBlockMode == ITEM::STATE_WRITE_LOCK);

	pItem->eBlockMode = ITEM::STATE_WRITE_UNLOCK;

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
	while(nWaitingCount > MAX_COUNTER )
	{
		if(nWaitingCount - MAX_COUNTER > 10)
		{
			Sleep(3);
		}
		else
		{
			Sleep(0);
		}

		// Update Waiting Order value
		nWaitingCount = m_readTicket.GetMyWaitingOrder(myTicket);
	}

	int nReadPos = (myTicket-1) % SIZE_BUFFER;
	int nLockTry = 0;

	while( (_InterlockedCompareExchange64((LONG64*)&m_SpinBuffer[nReadPos].eBlockMode, ITEM::STATE_READ_LOCK, ITEM::STATE_WRITE_UNLOCK)) != ITEM::STATE_WRITE_UNLOCK)
	{
		nLockTry++;
		if( nLockTry%5 )
		{
			Sleep(0);
		}
	}

	return &m_SpinBuffer[nReadPos];
}

template <typename T, int SIZE_BUFFER>
void SpinBufferMT<T, SIZE_BUFFER>::Read_Unlock(ITEM* pItem)
{
	AssertRel(pItem->eBlockMode == ITEM::STATE_READ_LOCK);

	pItem->eBlockMode = ITEM::STATE_FREE;

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
