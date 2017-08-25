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


#include "stdafx.h"
#include "Common/StrUtil.h"
#include "Common/Trace.h"
#include "Common/Synchronization.h"
#include "Common/ResultCode/BRResultCodeCommon.h"

#include "Common/MemoryPool.h"
#include "Common/HashTable2.h"
#include "Common/Memory_Impl.h"
#include "Common/StackWalker.h"


namespace BR
{
#if WINDOWS
	#define CurrentProcessID GetCurrentProcess()
#else
#define CurrentProcessID 0
#endif

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

	Result PageAllocator::Alloc( void* &pPtr )
	{
		Result hr = ResultCode::SUCCESS;

		trcChk( ProcessorHeap::Alloc( m_szPageSize, pPtr ) );
		m_PageCount.fetch_add(1, std::memory_order_relaxed);

		AssertRel( ((intptr_t)pPtr & (BR_ALIGN_DOUBLE-1)) == 0 );


	Proc_End:

		return ResultCode::SUCCESS;
	}


	Result PageAllocator::Free( void* pPtr )
	{
		Result hr = ResultCode::SUCCESS;

		trcChk( ProcessorHeap::Free( pPtr ) );
		m_PageCount.fetch_sub(1, std::memory_order_relaxed);

	Proc_End:

		return ResultCode::SUCCESS;
	}


	
	// Suggest Page size
	size_t PageAllocator::SuggestElementCountPerPage( size_t HeaderSize, size_t ElementSize )
	{
#if WINDOWS
		// pit page size to system page size
		SYSTEM_INFO sysInfo;
		memset( &sysInfo, 0, sizeof(sysInfo) );
		GetSystemInfo( &sysInfo );

		size_t ActivePageSize = sysInfo.dwPageSize - HeaderSize - sizeof(MemBlockHdr);

#else
		size_t ActivePageSize = 4096 - HeaderSize - sizeof(MemBlockHdr);
#endif
		size_t SuggestedElementCount = ActivePageSize / ElementSize;
		SuggestedElementCount = (size_t)Util::NearPowerOf2((UINT)SuggestedElementCount);

		return SuggestedElementCount;
	}





	////////////////////////////////////////////////////////////////////////////////
	//
	//	Memory Pool by size
	//

	constexpr int32_t MemoryPool::POOL_ITEM_MIN;
	constexpr int32_t MemoryPool::POOL_ITEM_MAX;
	constexpr intptr_t MemoryPool::POOL_MEMMAGIC;
	constexpr uint32_t MemoryPool::MEMITEM_SIZE;


	// Constructor for singleton
	MemoryPool::MemoryPool( size_t AllocSize )
		: m_AllocSize(BR_ALLIGNUP(AllocSize,BR_ALIGN_DOUBLE) + MEMITEM_SIZE)
		, m_Allocator( DecidePageSize(BR_ALLIGNUP(AllocSize,BR_ALIGN_DOUBLE)) )
		, m_FreeIndex(0)
#ifdef ENABLE_MEMORY_TRACE
		, m_printAlocList(false)
#endif
	{
#ifdef ENABLE_MEMORY_TRACE
		memset(&m_AllocatedHead, 0, sizeof m_AllocatedHead);
#endif
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
		for(auto& itFreeList : m_FreeList)
			itFreeList.Clear();
	}

	// Allocate/Free
	Result MemoryPool::Alloc( void* &pPtr, const char* typeName )
	{
		MemItem *pMemItem = nullptr;
		auto& freeList = PickFreeList();
		StackPool::Item *pItem = freeList.Pop();
		if( pItem == nullptr )
		{
			// Allocate page if no free item
			void *pPage = nullptr;

#ifdef ENABLE_MEMORY_TRACE
			if (m_printAlocList)
			{
				PrintAllocatedList();
			}
#endif

			if( !(m_Allocator.Alloc( pPage )) )
				return ResultCode::OUT_OF_MEMORY;

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
				pMemItem->LatestThreadID = ThisThread::GetThreadID();
				memset( pMemItem->DataPtr(), 0xCD, GetAllocSize() );
#endif
				freeList.Push( pMemItem );
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
				pMemItem->StackTrace.PrintStackTrace(Trace::TRC_ERROR, CurrentProcessID);
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
#ifdef ENABLE_MEMORY_TRACE
			MutexScopeLock localLock(m_CriticalSection);

			// link this to the allocated header
			if (m_AllocatedHead.pNext != nullptr)
				((MemItem*)m_AllocatedHead.pNext)->pPrev = pMemItem;
			pMemItem->pNext = m_AllocatedHead.pNext;

			pMemItem->pPrev = &m_AllocatedHead;
			m_AllocatedHead.pNext = pMemItem;


			UINT32* pPtr32 = (UINT32*)pPtr;
			for( UINT offset = 0 ; offset < GetAllocSize(); offset += 4 )
			{
				AssertRel( *pPtr32++ == 0xCDCDCDCDL );
			}

			new(&pMemItem->StackTrace) CallStackTrace;
			StackWalker::CaptureCallStack(pMemItem->StackTrace);
			pMemItem->TypeName = typeName;
			pMemItem->LatestThreadID = ThisThread::GetThreadID();
#endif
		}

		//std::atomic_thread_fence(std::memory_order_seq_cst);

		m_AllocatedCount.fetch_add(1,std::memory_order_relaxed);

		return ResultCode::SUCCESS;
	}

	Result MemoryPool::Free( void* pPtr, const char* typeName )
	{
		MemItem *pMemItem = nullptr;

		if( pPtr == nullptr )
			return ResultCode::SUCCESS;

		auto& freeList = PickFreeList();

		pMemItem = (MemItem*)((BYTE*)pPtr - MEMITEM_SIZE);

		Assert( pMemItem->MemMagic == POOL_MEMMAGIC );
		//Assert( pMemItem->Using == 1 );
		if( //pMemItem->Using != 1
			pMemItem->pPrev == nullptr
			|| pMemItem->MemMagic != POOL_MEMMAGIC )
		{
			// Drop memory that causes the problem
#ifdef ENABLE_MEMORY_TRACE
			pMemItem->StackTrace.PrintStackTrace(Trace::TRC_ERROR, CurrentProcessID);
			new(&pMemItem->StackTrace) CallStackTrace;
			StackWalker::CaptureCallStack(pMemItem->StackTrace);
			pMemItem->TypeName = typeName;
			pMemItem->LatestThreadID = ThisThread::GetThreadID();
			pMemItem->StackTrace.PrintStackTrace(Trace::TRC_ERROR, CurrentProcessID);
			Assert(false);
#endif
		}
		else
		{
			//CounterType Using = pMemItem->Using.fetch_sub(1, std::memory_order_relaxed);
			//Assert( Using == 1 );
			//Assert( Using == pMemItem->Using );

			{
#ifdef ENABLE_MEMORY_TRACE
				MutexScopeLock localLock(m_CriticalSection);
				// link this to the allocated header
				if (pMemItem->pNext != nullptr)
					((MemItem*)pMemItem->pNext)->pPrev = pMemItem->pPrev;
				pMemItem->pPrev->pNext = pMemItem->pNext;

				pMemItem->pPrev = nullptr;
				pMemItem->pNext = nullptr;

				new(&pMemItem->StackTrace) CallStackTrace;
				StackWalker::CaptureCallStack(pMemItem->StackTrace);
				pMemItem->TypeName = typeName;
				pMemItem->LatestThreadID = ThisThread::GetThreadID();
				memset( pPtr, 0xCD, GetAllocSize() );
#endif
			}

			freeList.Push( pMemItem );
		}

		m_AllocatedCount.fetch_sub(1,std::memory_order_relaxed);


#ifdef ENABLE_MEMORY_TRACE
		if (m_printAlocList)
		{
			PrintAllocatedList();
		}
#endif

		return ResultCode::SUCCESS;
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

#ifdef ENABLE_MEMORY_TRACE
	void MemoryPool::PrintAllocatedList()
	{
		m_printAlocList = false;
		auto pCur = m_AllocatedHead.pNext;
		while (pCur != nullptr)
		{
			auto pMemCur = (MemItem*)pCur;
			pMemCur->StackTrace.PrintStackTrace(Trace::TRC_WARN, CurrentProcessID);
			pCur = pCur->pNext;
		}
	}
#endif

		
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Memory Pool manager
	//

	class MemoryPoolManagerImpl
	{
	public:
		
		// Max memory size for pool
		static constexpr size_t MAX_MEMORYSIZE_SHIFT = 8;
		static constexpr size_t MAX_MEMORYSIZE_POWEROF2 = 1 << MAX_MEMORYSIZE_SHIFT;
		// memory alignment size
		static constexpr size_t MAX_MEMORYPOOL_ALIGNMENT_SHIFT = 8;
		static constexpr size_t MAX_MEMORYPOOL_ALIGNMENT = 1 << MAX_MEMORYPOOL_ALIGNMENT_SHIFT;
		static constexpr size_t MAX_MEMORYPOOL_COUNT = MAX_MEMORYSIZE_SHIFT + 1;



		struct MemoryPoolItem
		{
			size_t MaxAllocationSize = 0;
			MemoryPool* pMemoryPool = nullptr;
		};

		// memory pool by size
		MemoryPoolItem	m_MemoryPoolbySize[MAX_MEMORYPOOL_COUNT];

		// Write lock to prevent duplicated addition try.
		TicketLock		m_LockForAdd;

	private:

		// Quantize memory size
		bool QuantizeAndFind( size_t memorySize, uint32_t& minBitShift, size_t &newSize )
		{
			Assert( memorySize < (size_t)std::numeric_limits<UINT32>::max() );

			minBitShift = Util::FindMinBitShift((uint32_t)memorySize);

			newSize = (size_t)1 << minBitShift;
			if( newSize > MAX_MEMORYSIZE_POWEROF2 )
			{
				// We don't use memory pool for this type
				newSize = BR_ALLIGNUP(memorySize,MAX_MEMORYPOOL_ALIGNMENT);
				return false;
			}
			else
			{
				return true;
			}
		}


	public:

		MemoryPoolManagerImpl()
		{
			int sizeShift = 1;
			for (auto& itPoolItem : m_MemoryPoolbySize)
			{
				itPoolItem.MaxAllocationSize = (size_t)1 << sizeShift;
				itPoolItem.pMemoryPool = new MemoryPool(itPoolItem.MaxAllocationSize);
				sizeShift++;
			}
		}

		~MemoryPoolManagerImpl()
		{
			// How can we delete them?
			for (auto& itPoolItem : m_MemoryPoolbySize)
			{
				delete itPoolItem.pMemoryPool;
			}
		}

		// Get memory pool
		Result GetMemoryPool( size_t allocationSize, MemoryPool* &pNewPool )
		{
			pNewPool = nullptr;

			uint32_t minBitShift;
			size_t newAllocationSize = allocationSize;
			if(!QuantizeAndFind(allocationSize, minBitShift, newAllocationSize))
				return ResultCode::FAIL;

			auto poolIndex = minBitShift;

			if(poolIndex >= countof(m_MemoryPoolbySize))
				return ResultCode::FAIL;

			assert(m_MemoryPoolbySize[poolIndex].MaxAllocationSize >= newAllocationSize);
			pNewPool = m_MemoryPoolbySize[poolIndex].pMemoryPool;

			return ResultCode::SUCCESS;

		}
	};


	// Max memory size for pool
	constexpr size_t MemoryPoolManagerImpl::MAX_MEMORYSIZE_SHIFT;
	constexpr size_t MemoryPoolManagerImpl::MAX_MEMORYSIZE_POWEROF2;
	// memory alignment size
	constexpr size_t MemoryPoolManagerImpl::MAX_MEMORYPOOL_ALIGNMENT_SHIFT;
	constexpr size_t MemoryPoolManagerImpl::MAX_MEMORYPOOL_ALIGNMENT;
	constexpr size_t MemoryPoolManagerImpl::MAX_MEMORYPOOL_COUNT;



	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	MemoryPoolManager
	//

	MemoryPoolManagerImpl* MemoryPoolManager::stm_pInstance = nullptr;

	MemoryPoolManager::MemoryPoolManager()
		: Component((UINT)LibCompoentIDs::MemoryPoolManager)
	{

	}

	MemoryPoolManager::~MemoryPoolManager()
	{

	}

	// Initialize server component
	Result MemoryPoolManager::InitializeComponent()
	{
		Result hr = Component::InitializeComponent();
		if (!(hr)) return hr;

		StackWalker::Initialize();
		stm_pInstance = new MemoryPoolManagerImpl;

		return hr;
	}

	// Terminate server component
	void MemoryPoolManager::TerminateComponent()
	{
		Component::TerminateComponent();

		StackWalker::Deinitialize();

		Util::SafeDelete(stm_pInstance);
	}



	// Get memory pool by size
	Result MemoryPoolManager::GetMemoryPoolBySize( size_t allocationSize, MemoryPool* &pPool )
	{
		if( stm_pInstance == nullptr )
			return ResultCode::E_NOT_INITIALIZED;

		AssertRel( stm_pInstance );

		return stm_pInstance->GetMemoryPool( allocationSize, pPool );
	}




};	// namespace BR


