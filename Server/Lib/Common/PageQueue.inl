////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : Paged queue
//
////////////////////////////////////////////////////////////////////////////////


template <class DataObject>
PageQueue<DataObject>::PageQueue( int iDataPerPage )
	: m_NumberOfItemsPerPage(0)
	, m_PageIndex(0)
	, m_pMemoryPool(nullptr)
{
	// get page item count
	if (iDataPerPage <= 0)
		iDataPerPage = 2048;

	m_NumberOfItemsPerPage = iDataPerPage;


	// calculate allignment
	size_t szDataAllign = __alignof(DataObject);
	size_t szPageHdr = sizeof(PageHeader);

	szPageHdr = ((szPageHdr + szDataAllign - 1) / szDataAllign ) * szDataAllign;

	m_EnqueueTicket = 0;
	m_DequeueTicket = 0;

	m_EnqueuePage = nullptr;
	m_EnqueueNextPage = nullptr;
	m_DequeuePage = nullptr;
	m_EnqueuePageID = 0;
	m_DequeuePageID = 0;

	MemoryPoolManager::GetMemoryPoolBySize(GetPageMemorySize(), m_pMemoryPool);

	// fix up to maximum available power of two size
	if (m_pMemoryPool != nullptr)
	{
		auto elementAreaSize = m_pMemoryPool->GetAllocSize() - sizeof(Page)- sizeof(DataObject);
		auto maximumNumberOfElementPerPage = elementAreaSize / sizeof(DataObject);
		auto nearPower = Util::NearPowerOf2((UINT32)maximumNumberOfElementPerPage);
		if (nearPower > maximumNumberOfElementPerPage) nearPower >>= 1;
		m_NumberOfItemsPerPage = nearPower;
	}
	else
	{
		auto nearPower = Util::NearPowerOf2((UINT32)m_NumberOfItemsPerPage);
		m_NumberOfItemsPerPage = nearPower;
	}

	auto newPage = AllocatePage();
	newPage->Header.pNext = AllocatePage();
	m_EnqueuePage.store(newPage, std::memory_order_relaxed);
	m_EnqueuePageID.store(0, std::memory_order_relaxed);
	m_EnqueueNextPage.store(newPage->Header.pNext, std::memory_order_relaxed);
	// two pointer is initialized pointing to default Page
	m_DequeuePage = newPage;
}


template <class DataObject>
PageQueue<DataObject>::~PageQueue(void)
{
	Page* pCurPage = (Page*)m_DequeuePage;

	while( pCurPage )
	{
		Page *pNext = pCurPage->Header.pNext;
		FreePage(pCurPage);
		//m_pPageAllocator->Free( pCurPage );
		pCurPage = pNext;
	}

}

template <class DataObject>
typename PageQueue<DataObject>::Page* PageQueue<DataObject>::AllocatePage()
{
	void *pNewPageBuff = NULL;

	if (m_pMemoryPool)
		m_pMemoryPool->Alloc(pNewPageBuff, "PageQueueBuffer");
	else
		pNewPageBuff = new BYTE[GetPageMemorySize()];

	Page *pNewPage = new(pNewPageBuff) Page(m_NumberOfItemsPerPage);
	pNewPage->Header.PageID = m_PageIndex++;

	return pNewPage;
}

template <class DataObject>
void PageQueue<DataObject>::FreePage(Page* pPage)
{
	if (pPage == nullptr) return;
	
	pPage->DeleteThis(m_NumberOfItemsPerPage);

	if (m_pMemoryPool)
		m_pMemoryPool->Free(pPage, "PageQueueBuffer");
	else
		delete (BYTE*)pPage;
}

template <class DataObject>
HRESULT PageQueue<DataObject>::DequeuePageMove()
{
	// We use Page ID to sync check of HeadPage then pointer
	Page* curDequeue = m_DequeuePage.load(std::memory_order_relaxed);
	if (//curDequeue->Header.PageID == m_EnqueuePageID.load(std::memory_order_acquire) || 
		curDequeue == m_EnqueuePage.load(std::memory_order_relaxed))
		return E_FAIL;

	if (curDequeue->Header.WriteCounter.load(std::memory_order_relaxed) <= m_NumberOfItemsPerPage)
		return E_FAIL;

	if (curDequeue->Header.pNext == nullptr) return E_FAIL;

	// change the tail header pointer
	m_DequeuePage.store(curDequeue->Header.pNext,std::memory_order_release);

	FreePage(curDequeue);

	return S_OK;
}

template <class DataObject>
HRESULT PageQueue<DataObject>::DequeuePageMoveMT()
{
	// We use Page ID to sync check of HeadPage then pointer
	Page* curDequeue = m_DequeuePage.load(std::memory_order_relaxed);
	if (//curDequeue->Header.PageID == m_EnqueuePageID.load(std::memory_order_acquire) || 
		curDequeue == m_EnqueuePage.load(std::memory_order_relaxed))
		return E_FAIL;

	if (curDequeue->Header.WriteCounter.load(std::memory_order_relaxed) <= m_NumberOfItemsPerPage)
		return E_FAIL;

	if (curDequeue->Header.pNext == nullptr)
		return E_FAIL;

	// change the tail header pointer
	Page* pNewDequeueHead = curDequeue->Header.pNext;
	m_DequeuePage.store(pNewDequeueHead, std::memory_order_relaxed);
	m_DequeuePageID.store(pNewDequeueHead->Header.PageID, std::memory_order_release);

	FreePage(curDequeue);

	return S_OK;
}

template <class DataObject>
void PageQueue<DataObject>::EnqueuePageMove(Page* pMyEnqueuePage, CounterType myPageID)
{
	int nLockTry = 0;

	while (pMyEnqueuePage->Header.PageID != m_EnqueuePageID.load(std::memory_order_acquire)
		|| pMyEnqueuePage != m_EnqueuePage.load(std::memory_order_relaxed) )
	{
		nLockTry++;
		if (nLockTry % MaximumRetryInARow)
		{
			ThisThread::SleepFor(DurationMS(0));
		}
	}

	Page* newHead = pMyEnqueuePage->Header.pNext;
	if (newHead->Header.pNext == nullptr)
	{
		newHead->Header.pNext = AllocatePage();
		std::atomic_thread_fence(std::memory_order_release);
	}

	AssertRel((pMyEnqueuePage->Header.PageID + 1) == newHead->Header.PageID);

	// Mark page move is done
	pMyEnqueuePage->Header.WriteCounter.fetch_add(1, std::memory_order_relaxed);

	// order is mather
	m_EnqueuePage.store(newHead, std::memory_order_relaxed);
	m_EnqueueNextPage.store(newHead->Header.pNext, std::memory_order_release);
	// This page id is used for synchronize point to page change
	m_EnqueuePageID.store(newHead->Header.PageID, std::memory_order_release);
}

template <class DataObject>
HRESULT PageQueue<DataObject>::Enqueue(const DataObject& item)
{
	return Enqueue(std::forward<DataObject>(const_cast<DataObject&>(item)));
}

template <class DataObject>
HRESULT PageQueue<DataObject>::Enqueue( DataObject&& item )
{
	HRESULT hr = S_OK;

	Assert(item != nullptr);
	if (item == nullptr)
		return E_FAIL;

	// total ticket number
	CounterType myTicket = m_EnqueueTicket.fetch_add(1, std::memory_order_relaxed);// m_EnqueueTicket.AcquireTicket() - 1;

	// my pertinent PageID...starting from 0
	CounterType myPageID = (myTicket) / m_NumberOfItemsPerPage;

	// my order in this Page...starting from 0
	CounterType myCellID = (myTicket) % m_NumberOfItemsPerPage;

	int iTry = 0;
	auto diffPageID = (SignedCounterType)(myPageID - m_EnqueuePageID.load(std::memory_order_relaxed));
	Page* pMyEnqueuePage = nullptr;
	while (diffPageID != 0)
	{
		iTry++;
		if (iTry % MaximumRetryInARow)
			ThisThread::SleepFor(DurationMS(0));

		// can write if next page ready
		if (diffPageID == 1) // If next page can be mine
		{
			pMyEnqueuePage = m_EnqueueNextPage.load(std::memory_order_seq_cst);
			if (pMyEnqueuePage != nullptr)
			{
				if (pMyEnqueuePage->Header.PageID == myPageID)
				{
					break;
				}
				pMyEnqueuePage = nullptr;
			}
		}
		else
		{
			// I'm far away to be able to enqueue an item
			//ThisThread::SleepFor(DurationMS(0));
		}

		// try again
		diffPageID = (SignedCounterType)(myPageID - m_EnqueuePageID.load(std::memory_order_relaxed));
	}
	if (pMyEnqueuePage == nullptr)
		pMyEnqueuePage = m_EnqueuePage.load(std::memory_order_seq_cst);

	Assert(pMyEnqueuePage->Header.PageID == myPageID);

	// this page is appropriate to write....write to this page
	pMyEnqueuePage->Element[myCellID] = std::forward<DataObject>(item);

	CounterType WriteCount = pMyEnqueuePage->Header.WriteCounter.fetch_add(1, std::memory_order_relaxed) + 1;

	Assert(WriteCount <= m_NumberOfItemsPerPage);

	if (WriteCount == m_NumberOfItemsPerPage)
	{
		// The last guy handle page move
		EnqueuePageMove(pMyEnqueuePage, myPageID);
	}

Proc_End:

	return S_OK;
}

template <class DataObject>
HRESULT PageQueue<DataObject>::Dequeue( DataObject& item )
{
	// empty state / readcount is bigger than written count
	if (m_DequeueTicket.load(std::memory_order_relaxed) >= m_EnqueueTicket.load(std::memory_order_relaxed)) return E_FAIL;

	Page* pCurPage = m_DequeuePage.load(std::memory_order_relaxed);
	if (pCurPage->Header.WriteCounter.load(std::memory_order_relaxed) == 0) return E_FAIL;

	// read all of the page items...delete used page
	if (pCurPage->Header.ReadCounter.load(std::memory_order_relaxed) == m_NumberOfItemsPerPage)
	{
		// shorten queue size (delete a used page)
		HRESULT hr = DequeuePageMove();

		if(hr != S_OK) 
		{	
			return hr;
		}
	}

	CounterType myDequeueTicket = m_DequeueTicket.fetch_add(1, std::memory_order_relaxed);

	// my order in this Page...starting from 0
	CounterType myCellID = (myDequeueTicket) % m_NumberOfItemsPerPage;

	// my pertinent PageID...starting from 0
	CounterType myPageID = (myDequeueTicket) / m_NumberOfItemsPerPage;

	// Read the data & clear that read position 
	int LockTry = 0;
	Page* pMyPage = m_DequeuePage.load(std::memory_order_relaxed);
	while (pMyPage->Element[myCellID] == nullptr)
	{
		LockTry++;
		if (LockTry % MaximumRetryInARow)
		{
			ThisThread::SleepFor(DurationMS(0));
		}
	}

	Assert(pMyPage->Element[myCellID] != nullptr);

	item = std::forward<DataObject>(pMyPage->Element[myCellID]);

	pMyPage->Element[myCellID] = nullptr;

	// When the queue is used with shared pointer these order is very important
	std::atomic_thread_fence(std::memory_order_release);

	// increment item read count
	pMyPage->Header.ReadCounter.fetch_add(1, std::memory_order_relaxed);

	return S_OK;
}

template <class DataObject>
HRESULT PageQueue<DataObject>::DequeueMT( DataObject& item, unsigned int uiCheckInterval )
{
	HRESULT hr = S_OK;

	// total ticket number
	CounterType myDequeueTicket = m_DequeueTicket.fetch_add(1,std::memory_order_relaxed);

	// my order in this Page...starting from 0
	CounterType myCellID = (myDequeueTicket) % m_NumberOfItemsPerPage;

	// my pertinent PageID...starting from 0
	CounterType myPageID = (myDequeueTicket) / m_NumberOfItemsPerPage;

	// Page ID circulation can be occure so change to subtraction
	while (myPageID != m_DequeuePageID.load(std::memory_order_relaxed))
	{
		ThisThread::SleepFor(DurationMS(3));
	}

	Page* pMyDequeuePage = m_DequeuePage.load(std::memory_order_relaxed);

	// Read the data & clear that read position 
	int LockTry = 0;
	while( pMyDequeuePage->Element[myCellID] == nullptr )
	{
		LockTry++;
		if( LockTry%5 )
		{
			ThisThread::SleepFor(DurationMS(uiCheckInterval));
		}
		//_ReadBarrier();
		std::atomic_thread_fence(std::memory_order_consume);
	}

	item = pMyDequeuePage->Element[myCellID];

	std::atomic_thread_fence(std::memory_order_release);

	pMyDequeuePage->Element[myCellID] = nullptr;

	// increment item read count
	CounterType ReadCount = pMyDequeuePage->Header.ReadCounter.fetch_add(1,std::memory_order_relaxed) + 1;
	AssertRel( ReadCount <= m_NumberOfItemsPerPage );

	if(ReadCount == m_NumberOfItemsPerPage) 
	{
		hr = DequeuePageMoveMT();
		AssertRel(SUCCEEDED(hr));
	}

	return hr;
}



// Just get first item
// This will not safe if use DequeueMT
template <class DataObject>
HRESULT PageQueue<DataObject>::GetFront( DataObject& item )
{
	// empty state / readcount is bigger than written count
	if (m_DequeueTicket.load(std::memory_order_relaxed) >= m_EnqueueTicket.load(std::memory_order_relaxed)) return E_FAIL;

	Page* pCurPage = m_DequeuePage.load(std::memory_order_relaxed);
	if (pCurPage->Header.WriteCounter.load(std::memory_order_relaxed) == 0) return E_FAIL;

	// read all of the page items...delete used page
	if (pCurPage->Header.ReadCounter.load(std::memory_order_relaxed) == m_NumberOfItemsPerPage)
	{
		// shorten queue size (delete a used page)
		HRESULT hr = DequeuePageMove();

		if (hr != S_OK)
		{
			return hr;
		}
	}

	CounterType myDequeueTicket = m_DequeueTicket.load(std::memory_order_relaxed);

	// my order in this Page...starting from 0
	CounterType myCellID = (myDequeueTicket) % m_NumberOfItemsPerPage;

	// my pertinent PageID...starting from 0
	CounterType myPageID = (myDequeueTicket) / m_NumberOfItemsPerPage;

	// Read the data & clear that read position 
	int LockTry = 0;
	Page* pMyPage = m_DequeuePage.load(std::memory_order_relaxed);
	while (pMyPage->Element[myCellID] == nullptr)
	{
		LockTry++;
		if (LockTry % 10)
		{
			ThisThread::SleepFor(DurationMS(0));
		}
	}

	item = pMyPage->Element[myCellID];

	return S_OK;
}

template <class DataObject>
inline CounterType PageQueue<DataObject>::GetEnqueCount() const
{
	return m_EnqueueTicket.load(std::memory_order_relaxed) - m_DequeueTicket.load(std::memory_order_relaxed);
}

// Clear queue and remove all enqueued items
// This operation is not thread safe
template <class DataObject>
void PageQueue<DataObject>::ClearQueue()
{
	m_PageIndex = 1;

	auto enqueueNext = m_EnqueueNextPage.load(std::memory_order_relaxed);
	auto headPage = m_EnqueuePage.load(std::memory_order_relaxed);
	while (m_DequeuePage != headPage)
	{
		// this page is to free.
		Page* pFreePage = (Page*)m_DequeuePage;

		// change the tail header pointer
		m_DequeuePage = pFreePage->Header.pNext;

		FreePage(pFreePage);
	}

	// if it's not deleted, reuse it
	if (enqueueNext)
	{
		enqueueNext->DeleteThis(m_NumberOfItemsPerPage);
		new((void*)enqueueNext) Page(m_NumberOfItemsPerPage);
		enqueueNext->Header.PageID = 1;
		m_PageIndex++;
	}
	else
	{
		enqueueNext = AllocatePage();
	}

	headPage->DeleteThis(m_NumberOfItemsPerPage);
	headPage = new((void*)headPage) Page(m_NumberOfItemsPerPage);
	headPage->Header.PageID = 0;
	headPage->Header.pNext = enqueueNext;
	m_EnqueueNextPage = headPage->Header.pNext;

	Assert(headPage->Header.pNext->Header.PageID == 1);

	// reset counters
	m_EnqueuePageID = 0;
	m_EnqueueTicket = 0;
	m_DequeueTicket = 0;
}

