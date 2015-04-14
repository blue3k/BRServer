////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : utility
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include "Common/StrUtil.h"
#include "Common/Trace.h"
#include "Common/Synchronize.h"
#include "Common/HRESCommon.h"

#include "Common/MemoryPool.h"
#include "Common/MemLog.h"
#include "Common/HashTable2.h"
#include "Common/Memory_Impl.h"
#include "Common/StackWalker.h"


namespace BR
{

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Memory Page allocator
	//

	// Constructor
	PageAllocator::PageAllocator( size_t PageSize )
		:m_szPageSize(PageSize)
	{
	}

	PageAllocator::PageAllocator( size_t HeaderSize, size_t ElementSize )
	{
		m_szPageSize = HeaderSize + SuggestElementCountPerPage(HeaderSize,ElementSize) * ElementSize;
	}

	PageAllocator::~PageAllocator()
	{
		Clear();
	}

	void PageAllocator::Clear()
	{
	}
	
	size_t PageAllocator::GetPageSize() const
	{
		return m_szPageSize;
	}

	HRESULT PageAllocator::Alloc( void* &pPtr )
	{
		HRESULT hr = S_OK;

		trcChk( ProcessorHeap::Alloc( m_szPageSize, pPtr ) );
		m_PageCount.fetch_add(1, std::memory_order_relaxed);

		AssertRel( ((intptr_t)pPtr & (BR_ALIGN_DOUBLE-1)) == 0 );


	Proc_End:

		return S_OK;
	}


	HRESULT PageAllocator::Free( void* pPtr )
	{
		HRESULT hr = S_OK;

		trcChk( ProcessorHeap::Free( pPtr ) );
		m_PageCount.fetch_sub(1, std::memory_order_relaxed);

	Proc_End:

		return S_OK;
	}


	
	// Suggest Page size
	size_t PageAllocator::SuggestElementCountPerPage( size_t HeaderSize, size_t ElementSize )
	{
		// pit page size to system page size
		SYSTEM_INFO sysInfo;
		memset( &sysInfo, 0, sizeof(sysInfo) );
		GetSystemInfo( &sysInfo );

		size_t ActivePageSize = sysInfo.dwPageSize - HeaderSize - sizeof(BR::MemBlockHdr);

		size_t SuggestedElementCount = ActivePageSize/ElementSize;
		SuggestedElementCount = (size_t)Util::NearPowerOf2( (UINT)SuggestedElementCount );

		return SuggestedElementCount;
	}



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Memory Page pool
	//

	// Constructor
	PagePool::PagePool( size_t PageSize )
		:PageAllocator( PageSize + PAGEITEM_SIZE )
	{
	}

	PagePool::PagePool( size_t HeaderSize, size_t ElementSize )
		:PageAllocator( HeaderSize + PAGEITEM_SIZE, ElementSize )
	{
	}

	PagePool::~PagePool()
	{
		Clear();
	}

	// Clear
	void PagePool::Clear()
	{
		PagePool *pAllocator = this;
		m_FreePages.for_each( [&] ( BR::StackPool::Item *pItem )
		{
			pAllocator->OrgFree( pItem );
		});
		m_FreePages.Clear();
	}

	size_t PagePool::GetPageSize() const
	{
		return __super::GetPageSize() - sizeof(PagePool::PageItem);
	}

	// Alloc/Free page method
	HRESULT PagePool::Alloc( void* &pPtr )
	{
		void *pRes = nullptr;
		PageItem *pItem = nullptr;

		pItem = (PageItem*)m_FreePages.Pop();
		if( pItem == nullptr )
		{
			HRESULT hr = __super::Alloc( pRes );
			if( FAILED(hr) ) return hr;

			pItem = (PageItem*)pRes;
		}
		pItem->Magic = MAGIC_PAGEPOOL;
		pItem->pNext = nullptr;

		pPtr = (void*)((BYTE*)pItem + PAGEITEM_SIZE);
		AssertRel( ((intptr_t)pPtr & (BR_ALIGN_DOUBLE-1)) == 0 );

		return S_OK;
	}

	HRESULT PagePool::Free( void* pPtr )
	{
		PageItem *pItem = nullptr;

		if( pPtr == nullptr )
			return E_INVALIDARG;

		pItem = (PageItem*)((BYTE*)pPtr - PAGEITEM_SIZE);
		AssertRel( pItem->Magic == (intptr_t)MAGIC_PAGEPOOL );
		pItem->pNext = nullptr;

		m_FreePages.Push( pItem );

		return S_OK;
	}

	HRESULT PagePool::OrgFree( void* pPtr )
	{
		return __super::Free( pPtr );
	}


	
	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Memory Pool by size
	//



	// Constructor for singleton
	MemoryPool::MemoryPool( size_t AllocSize )
		: m_AllocSize(BR_ALLIGNUP(AllocSize,BR_ALIGN_DOUBLE) + MEMITEM_SIZE)
		, m_Allocator( DecidePageSize(BR_ALLIGNUP(AllocSize,BR_ALIGN_DOUBLE)) )
		, m_printAlocList(false)
	{
		memset(&m_AllocatedHead, 0, sizeof m_AllocatedHead);
	}

	MemoryPool::~MemoryPool()
	{
	}

	// Decide Page size
	size_t MemoryPool::DecidePageSize( size_t AllocSize )
	{
		m_AllocSize = AllocSize + MEMITEM_SIZE;
		size_t suggestedCount = PageAllocator::SuggestElementCountPerPage( 0, m_AllocSize );

		suggestedCount = Util::NearPowerOf2((UINT)suggestedCount);

		// if too small or big count
		if( suggestedCount < POOL_ITEM_MIN ) suggestedCount = POOL_ITEM_MIN;
		if( suggestedCount > POOL_ITEM_MAX ) suggestedCount = POOL_ITEM_MAX;

		m_AllocCountPerPage = suggestedCount;

		return suggestedCount * m_AllocSize;
	}


	// force clear memory heap
	void MemoryPool::ForceClear()
	{
		// this will lead to memory leak, so this should be called only when the application is being closed
		m_Allocator.Clear();
		m_FreeList.Clear();
	}

	// Allocate/Free
	HRESULT MemoryPool::Alloc( void* &pPtr, const char* typeName )
	{
		MemItem *pMemItem = nullptr;
		BR::StackPool::Item *pItem = m_FreeList.Pop();
		if( pItem == nullptr )
		{
			// Allocate page if no free item
			void *pPage = nullptr;


			if (m_printAlocList)
			{
				PrintAllocatedList();
			}


			if( FAILED(m_Allocator.Alloc( pPage )) )
				return E_OUTOFMEMORY;

			pMemItem = (MemItem*)pPage;
			for( size_t iItem = 1; iItem < m_AllocCountPerPage; iItem++ )
			{
				pMemItem->pNext = nullptr;
				pMemItem->MemMagic = POOL_MEMMAGIC;
				//pMemItem->Using.store(0, std::memory_order_relaxed);
				pMemItem->pPrev = nullptr;
#ifdef ENABLE_MEMORY_TRACE
				new(&pMemItem->StackTrace) CallStackTrace;
				StackWalker::CaptureCallStack(pMemItem->StackTrace);
				pMemItem->TypeName = typeName;
				pMemItem->LatestThreadID = GetCurrentThreadId();
				memset( pMemItem->DataPtr(), 0xCD, GetAllocSize() );
#endif
				m_FreeList.Push( pMemItem );
				pMemItem = (MemItem*)((BYTE*)pMemItem + m_AllocSize);
			}
			pMemItem->pNext = nullptr;
			pMemItem->MemMagic = POOL_MEMMAGIC;
			pMemItem->pPrev = nullptr;
			//pMemItem->Using.store(0, std::memory_order_relaxed);
#ifdef ENABLE_MEMORY_TRACE
			pMemItem->TypeName = typeName;
			memset( pMemItem->DataPtr(), 0xCD, GetAllocSize() );
#endif
		}
		else
		{
			pMemItem = (MemItem*)pItem;
			Assert( pMemItem->MemMagic == POOL_MEMMAGIC );
			//Assert(pMemItem->pPrev == nullptr);
			if (pMemItem->pPrev != nullptr
				|| pMemItem->MemMagic != POOL_MEMMAGIC )
			{
#ifdef ENABLE_MEMORY_TRACE
				pMemItem->StackTrace.PrintStackTrace(Trace::TRC_ERROR, GetCurrentProcess());
#endif
				Assert(false);
				// Drop this memory and try another alloc
				return Alloc( pPtr, typeName );
			}
		}

		pPtr = (void*)pMemItem->DataPtr();
		AssertRel( ((intptr_t)pPtr & (BR_ALIGN_DOUBLE-1)) == 0 );
		//CounterType Using = pMemItem->Using.fetch_add(1, std::memory_order_relaxed);
		//Assert( Using == 0 );
		//Assert( Using == pMemItem->Using );
		Assert(pMemItem->pPrev == nullptr);

		{
			MutexScopeLock localLock(m_CriticalSection);

			// link this to the allocated header
			if (m_AllocatedHead.pNext != nullptr)
				((MemItem*)m_AllocatedHead.pNext)->pPrev = pMemItem;
			pMemItem->pNext = m_AllocatedHead.pNext;

			pMemItem->pPrev = &m_AllocatedHead;
			m_AllocatedHead.pNext = pMemItem;

#ifdef ENABLE_MEMORY_TRACE

			UINT32* pPtr32 = (UINT32*)pPtr;
			for( UINT offset = 0 ; offset < GetAllocSize(); offset += 4 )
			{
				AssertRel( *pPtr32++ == 0xCDCDCDCDL );
			}

			new(&pMemItem->StackTrace) CallStackTrace;
			StackWalker::CaptureCallStack(pMemItem->StackTrace);
			pMemItem->TypeName = typeName;
			pMemItem->LatestThreadID = GetCurrentThreadId();
#endif
		}

		//_WriteBarrier();
		std::atomic_thread_fence(std::memory_order_seq_cst);

		m_AllocatedCount.fetch_add(1,std::memory_order_relaxed);

		return S_OK;
	}

	HRESULT MemoryPool::Free( void* pPtr, const char* typeName )
	{
		MemItem *pMemItem = nullptr;

		if( pPtr == nullptr )
			return S_OK;

		pMemItem = (MemItem*)((BYTE*)pPtr - MEMITEM_SIZE);

		Assert( pMemItem->MemMagic == POOL_MEMMAGIC );
		//Assert( pMemItem->Using == 1 );
		if( //pMemItem->Using != 1
			pMemItem->pPrev == nullptr
			|| pMemItem->MemMagic != POOL_MEMMAGIC )
		{
			// Drop memory that causes the problem
#ifdef ENABLE_MEMORY_TRACE
			pMemItem->StackTrace.PrintStackTrace(Trace::TRC_ERROR, GetCurrentProcess());
			new(&pMemItem->StackTrace) CallStackTrace;
			StackWalker::CaptureCallStack(pMemItem->StackTrace);
			pMemItem->TypeName = typeName;
			pMemItem->LatestThreadID = GetCurrentThreadId();
			pMemItem->StackTrace.PrintStackTrace(Trace::TRC_ERROR, GetCurrentProcess());
			Assert(false);
#endif
		}
		else
		{
			//CounterType Using = pMemItem->Using.fetch_sub(1, std::memory_order_relaxed);
			//Assert( Using == 1 );
			//Assert( Using == pMemItem->Using );

			{
				MutexScopeLock localLock(m_CriticalSection);
				// link this to the allocated header
				if (pMemItem->pNext != nullptr)
					((MemItem*)pMemItem->pNext)->pPrev = pMemItem->pPrev;
				pMemItem->pPrev->pNext = pMemItem->pNext;
				pMemItem->pPrev = nullptr;

#ifdef ENABLE_MEMORY_TRACE
				new(&pMemItem->StackTrace) CallStackTrace;
				StackWalker::CaptureCallStack(pMemItem->StackTrace);
				pMemItem->TypeName = typeName;
				pMemItem->LatestThreadID = GetCurrentThreadId();
				memset( pPtr, 0xCD, GetAllocSize() );
#endif
			}
			//_WriteBarrier();
			std::atomic_thread_fence(std::memory_order_seq_cst);

			m_FreeList.Push( pMemItem );
		}

		m_AllocatedCount.fetch_sub(1,std::memory_order_relaxed);


		if (m_printAlocList)
		{
			PrintAllocatedList();
		}

		return S_OK;
	}

	// Check pooled memory block header
	bool MemoryPool::CheckMemoryHeader(void* pPtr)
	{
		MemItem *pMemItem = nullptr;

		if( pPtr == nullptr )
			return true;

		pMemItem = (MemItem*)((BYTE*)pPtr - MEMITEM_SIZE);

		//auto Using = pMemItem->Using.load(std::memory_order_relaxed);
		bool result = pMemItem->pPrev != nullptr && pMemItem->MemMagic == POOL_MEMMAGIC;
		Assert( result );

		return result;
	}

	void MemoryPool::PrintAllocatedList()
	{
		m_printAlocList = false;
		auto pCur = m_AllocatedHead.pNext;
#ifdef ENABLE_MEMORY_TRACE
		while (pCur != nullptr)
		{
			auto pMemCur = (MemItem*)pCur;
			pMemCur->StackTrace.PrintStackTrace(Trace::TRC_WARN, GetCurrentProcess());
			pCur = pCur->pNext;
		}
#endif
	}

		
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Memory Pool manager
	//

	class MemoryPoolManagerImpl
	{
	public:
		enum {
			// Max memory size for pool
			MAX_MEMORYSIZE_POWEROF2			= 1<<8,
			// memory alignment size
			MAX_MEMORYPOOL_ALIGNMENT_SHIFT	= 8,
			MAX_MEMORYPOOL_ALIGNMENT		= 1<<MAX_MEMORYPOOL_ALIGNMENT_SHIFT,
		};

		// Memory pool table type
		typedef BR::Hash::HashTable2< size_t, MemoryPool* > MemoryPoolMap;

		// memory pool by size
		MemoryPoolMap	m_MemoryPoolbySize;

		// Write lock to prevent duplicated addition try.
		TicketLock		m_LockForAdd;

	private:

		// Quantize memory size
		size_t GetQuantizedMemorySize( size_t memorySize )
		{
			Assert( memorySize < (size_t)std::numeric_limits<UINT32>::max() );

			size_t newSize = Util::NearPowerOf2( (UINT32)memorySize );
			if( newSize > MAX_MEMORYSIZE_POWEROF2 )
			{
				// allign memory size
				newSize = BR_ALLIGNUP(memorySize,MAX_MEMORYPOOL_ALIGNMENT);
			}
			return newSize;
		}

		// Add Memory pool
		MemoryPool* AddMemoryPool( size_t allocationSize )
		{
			TicketScopeLock lock( TicketLock::LOCK_EXCLUSIVE, m_LockForAdd );

			// if it's already exist, exit
			MemoryPool* pFound = nullptr;
			if (SUCCEEDED(m_MemoryPoolbySize.Find(allocationSize, pFound)))
			{
				return pFound;
			}

			MemoryPool *pNewPool = new MemoryPool( allocationSize );
			AssertRel(pNewPool);
			m_MemoryPoolbySize.Insert(allocationSize, pNewPool);

			return pNewPool;
		}

	public:

		MemoryPoolManagerImpl()
		{
		}

		~MemoryPoolManagerImpl()
		{
			// How can we delete them?
			//while( m_MemoryPoolbySize.GetItemCount() > 0 )
			//{
			//	auto itCur = m_MemoryPoolbySize.();
			//	if( itCur == m_MemoryPoolbySize.end() ) break;

			//	auto pItem = *itCur;

			//	m_MemoryPoolbySize.erase(itCur);

			//	pItem->ForceClear();
			//	delete pItem;
			//}

			m_MemoryPoolbySize.Clear();
		}

		// Get memory pool
		HRESULT GetMemoryPool( size_t allocationSize, MemoryPool* &pNewPool )
		{
			allocationSize = GetQuantizedMemorySize(allocationSize);

			MemoryPool* pFound = nullptr;
			if (FAILED(m_MemoryPoolbySize.Find(allocationSize, pFound)))
			{
				pNewPool = AddMemoryPool( allocationSize );
			}
			else
			{
				pNewPool = pFound;
			}

			Assert( pNewPool->GetAllocSize() == allocationSize );

			return S_OK;

		}
	};

	MemoryPoolManagerImpl *MemoryPoolManager::stm_pInstance = nullptr;


	// Initialize and Terminate memory pool
	HRESULT MemoryPoolManager::Initialize()
	{
		if( stm_pInstance == nullptr )
			stm_pInstance = new MemoryPoolManagerImpl;

		StackWalker::Initialize();

		return S_OK;
	}

	HRESULT MemoryPoolManager::Terminate()
	{
		if( stm_pInstance == nullptr )
			return S_OK;

		delete stm_pInstance;
		stm_pInstance = nullptr;

		StackWalker::Deinitialize();

		return S_OK;
	}

	// Get memory pool by size
	HRESULT MemoryPoolManager::GetMemoryPoolBySize( size_t allocationSize, MemoryPool* &pPool )
	{
		if( stm_pInstance == nullptr )
			return E_NOT_INITIALIZED;

		AssertRel( stm_pInstance );

		return stm_pInstance->GetMemoryPool( allocationSize, pPool );
	}




};	// namespace BR


