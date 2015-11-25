////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : Paged queue
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/BrAssert.h"
#include "Common/MemoryPool.h"
#include "Common/Synchronization.h"
#include "Common/SystemSynchronization.h"
#include "Common/Trace.h"
#include "Common/TypeUtility.h"


namespace BR
{
	
		
	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Page Queue Class (n-by-one)
	//

	template <class DataType> 
	class PageQueue
	{
	private:

		enum { MaximumRetryInARow = 10 };

		struct Page;

		// Page Header
		struct PageHeader
		{
			Page* pNext;
			CounterType PageID;		// starting from 0

			SyncCounter	WriteCounter;
			SyncCounter ReadCounter;

			PageHeader() :pNext(nullptr), PageID(0), WriteCounter(0), ReadCounter(0) { }
			~PageHeader() { }
		};

		// Page
		struct Page
		{
			// Page header
			PageHeader Header;

			// Data Array start
			DataType Element[1];

			Page( CounterType InItemCount )
			{
				for( CounterType iEle = 1; iEle < InItemCount; iEle++ )
				{
					new (&Element[iEle]) DataType;
				}

				auto defaultValue = DefaultValue<DataType>();
				for( CounterType iEle = 0; iEle < InItemCount; iEle++ )
				{
					Element[iEle] = defaultValue;
				}
			}

			void DeleteThis(CounterType ItemCpacity)
			{
				for (CounterType iEle = 1; iEle < ItemCpacity; iEle++)
				{
					Element[iEle].DataType::~DataType();
				}
			}
		};

		size_t GetPageMemorySize() { return sizeof(Page)+m_NumberOfItemsPerPage*sizeof(DataType); }


	private:

		// page haeader pointer
		std::atomic<Page*>       m_EnqueuePage;
		std::atomic<Page*>       m_EnqueueNextPage;
		std::atomic<CounterType> m_EnqueuePageID;
		std::atomic<Page*>       m_DequeuePage;
		std::atomic<CounterType> m_DequeuePageID;

		// item_count per page
		CounterType m_NumberOfItemsPerPage;	

		// ticket to the entering write section
		SyncCounter m_EnqueueTicket;
		SyncCounter	m_DequeueTicket;

		// Page allocation index
		CounterType m_PageIndex;

		MemoryPool *m_pMemoryPool;

		BR::CriticalSection m_DequeueLock;

	private:

		void EnqueuePageMove(Page* pMyEnqueuePage, CounterType myPageID);

	public:
		// constructor / destructor
		PageQueue( int iDataPerPage = -1 );
		~PageQueue(void);

		MemoryPool* GetMemoryPool()												{ return m_pMemoryPool; }

		// item enque
		inline HRESULT Enqueue( const DataType& item);
		inline HRESULT Enqueue(DataType&& item);

		// item deque
		inline HRESULT Dequeue( DataType& item );

		// item deque with MT
		inline HRESULT DequeueMT( DataType& item, DurationMS uiCheckInterval = DurationMS(100) );


		// Just get first dequeue item if exist, not dequeue
		// This will not safe if use DequeueMT
		inline HRESULT GetFront( DataType& item );

		// Item count in queue
		inline CounterType GetEnqueCount() const;

		// Clear queue and remove all enqueued items
		// This operation is not thread safe
		void ClearQueue();

	private:
		// private function
		inline Page* AllocatePage();
		inline void FreePage(Page* pPage);

		inline HRESULT DequeuePageMove();
		inline HRESULT DequeuePageMoveMT();
	};

	#include "PageQueue.inl"



} // namespace BR
