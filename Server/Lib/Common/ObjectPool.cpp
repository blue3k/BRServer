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

#include "Common/ObjectPool.h"
#include "Common/MemLog.h"
#include "Common/HashTable.h"



namespace BR
{

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Object Pool(Multi-thread)
	//

	// Constructor for singleton
	ObjectPool::ObjectPool(size_t objectSize)
		: m_AllocSize(BR_ALLIGNUP(objectSize, BR_ALIGN_DOUBLE) + MEMITEM_SIZE)
		, m_CallDestructor(false)
		, m_FreeList(nullptr)
		, m_Allocator(DecidePageSize(BR_ALLIGNUP(objectSize, BR_ALIGN_DOUBLE)))
		//, m_PendingFreeCount(0)
		//, m_PendingFreeList(nullptr)
		//, m_PendingFreeListTail(nullptr)
	{
	}

	ObjectPool::~ObjectPool()
	{
		ForceClear();
	}

	// Decide Page size
	size_t ObjectPool::DecidePageSize(size_t AllocSize)
	{
		m_AllocSize = AllocSize + MEMITEM_SIZE;
		size_t suggestedCount = PageAllocator::SuggestElementCountPerPage(0, m_AllocSize);

		suggestedCount = Util::NearPowerOf2((UINT)suggestedCount);

		// if too small or big count
		if (suggestedCount < POOL_ITEM_MIN) suggestedCount = POOL_ITEM_MIN;
		if (suggestedCount > POOL_ITEM_MAX) suggestedCount = POOL_ITEM_MAX;

		m_AllocCountPerPage = suggestedCount;

		return suggestedCount * m_AllocSize;
	}

	// force clear memory heap
	void ObjectPool::ForceClear()
	{
		if (m_CallDestructor)
		{
			auto curObject = m_FreeList;
			while (curObject != nullptr)
			{
				curObject->pObject->~ObjectPoolObject();
				curObject = curObject->pNext;
			}
		}
		m_FreeList = nullptr;
		m_Allocator.Clear();
	}

	// Allocate/Free
	HRESULT ObjectPool::Alloc(ObjectPoolObject* &pPtr)
	{
		ObjectItem *pMemItem = nullptr;
		if (!USE_DIRECTFREE)
		{
			pMemItem = m_FreeList;
			if (pMemItem == nullptr)
			{
				// Allocate page if no free item
				void *pPage = nullptr;

				if (FAILED(m_Allocator.Alloc(pPage)))
					return E_SYSTEM_OUTOFMEMORY;

				pMemItem = (ObjectItem*)pPage;
				for (size_t iItem = 1; iItem < m_AllocCountPerPage; iItem++)
				{
					pMemItem->pNext = nullptr;
					pMemItem->Using = 0;
					pMemItem->pObject = nullptr;

					pMemItem->pNext = m_FreeList;
					m_FreeList = pMemItem;

					pMemItem = (ObjectItem*)((BYTE*)pMemItem + m_AllocSize);
				}
				pMemItem->pNext = nullptr;
				pMemItem->Using = 0;
				pMemItem->pObject = OnNewObject((BYTE*)pMemItem + MEMITEM_SIZE);
			}
			else
			{
				m_FreeList = m_FreeList->pNext;

				Assert(pMemItem->Using == 0);
				if (pMemItem->Using != 0)
				{
					// Drop this memory and try another alloc
					return Alloc(pPtr);
				}
			}
		}
		else
		{
			pMemItem = new(new BYTE[m_AllocSize]) ObjectItem();
		}

		pMemItem->pNext = nullptr;
		if (pMemItem->pObject == nullptr)
		{
			pMemItem->pObject = OnNewObject((BYTE*)pMemItem + MEMITEM_SIZE);
		}
		pPtr = pMemItem->pObject;
		pMemItem->Using++;
		Assert(pMemItem->Using == 1);

		pPtr->Reuse();

		m_AllocatedCount.fetch_add(1, std::memory_order_relaxed);

		return S_OK;
	}

	HRESULT ObjectPool::Free(ObjectPoolObject* pPtr)
	{
		ObjectItem *pMemItem = nullptr;

		if (pPtr == nullptr)
			return S_OK;

		pPtr->Dispose();

		pMemItem = (ObjectItem*)((BYTE*)pPtr - MEMITEM_SIZE);

		Assert(pMemItem->Using == 1);
		if (pMemItem->Using != 1)
		{
			// Drop memory that cause problem
			Assert(false);
		}
		else
		{
			pMemItem->Using--;
			Assert(pMemItem->Using == 0);
			AssertRel(pMemItem->pNext == nullptr && pMemItem != m_FreeList);
			pMemItem->pNext = m_FreeList;
			if (!USE_DIRECTFREE)
			{
				m_FreeList = pMemItem;
			}
			else
			{
				delete[] (BYTE*)pMemItem;
			}
		}

		m_AllocatedCount.fetch_sub(1, std::memory_order_relaxed);

		return S_OK;
	}



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Object Pool(Multi-thread)
	//

	// Constructor for singleton
	ObjectPoolMT::ObjectPoolMT( size_t AllocSize )
		:m_AllocSize(BR_ALLIGNUP(AllocSize,BR_ALIGN_DOUBLE) + MEMITEM_SIZE),
		m_Allocator( DecidePageSize(BR_ALLIGNUP(AllocSize,BR_ALIGN_DOUBLE)) )
	{
	}

	ObjectPoolMT::~ObjectPoolMT()
	{
	}

	// Decide Page size
	size_t ObjectPoolMT::DecidePageSize( size_t AllocSize )
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
	void ObjectPoolMT::ForceClear()
	{
		m_FreeList.for_each( []( StackPool::Item *pItem )
		{
			MemItem *pMemItem = (MemItem*)pItem;
			pMemItem->pObject->~ObjectPoolObject();
		});
		m_FreeList.Clear();
		m_Allocator.Clear();
	}

	// Allocate/Free
	HRESULT ObjectPoolMT::Alloc( ObjectPoolObject* &pPtr )
	{
		MemItem *pMemItem = nullptr;
		StackPool::Item *pItem = m_FreeList.Pop();
		if( pItem == nullptr )
		{
			// Allocate page if no free item
			void *pPage = nullptr;

			if( FAILED(m_Allocator.Alloc( pPage )) )
				return E_SYSTEM_OUTOFMEMORY;

			pMemItem = (MemItem*)pPage;
			for( size_t iItem = 1; iItem < m_AllocCountPerPage; iItem++ )
			{
				pMemItem->pNext = nullptr;
				pMemItem->MemMagic = POOL_MEMMAGIC;
				pMemItem->Using = 0;
				pMemItem->pObject = OnNewObject( (BYTE*)pMemItem+MEMITEM_SIZE );
				m_FreeList.Push( pMemItem );
				pMemItem = (MemItem*)((BYTE*)pMemItem + m_AllocSize);
			}
			pMemItem->pNext = nullptr;
			pMemItem->MemMagic = POOL_MEMMAGIC;
			pMemItem->Using = 0;
			pMemItem->pObject = OnNewObject( (BYTE*)pMemItem+MEMITEM_SIZE );
		}
		else
		{
			pMemItem = (MemItem*)pItem;
			Assert( pMemItem->Using == 0 );
			Assert( pMemItem->MemMagic == POOL_MEMMAGIC );
			if( pMemItem->Using != 0 
				|| pMemItem->MemMagic != POOL_MEMMAGIC )
			{
				// Drop this memory and try another alloc
				return Alloc( pPtr );
			}
		}

		pPtr = pMemItem->pObject;
		CounterType Using = pMemItem->Using.fetch_add(1,std::memory_order_relaxed) + 1;
		Assert( Using == 1 );
		//Assert( Using == pMemItem->Using );

		pPtr->Reuse();

		//_WriteBarrier();
		std::atomic_thread_fence(std::memory_order_seq_cst);

		m_AllocatedCount.fetch_add(1,std::memory_order_relaxed);

		return S_OK;
	}

	HRESULT ObjectPoolMT::Free( ObjectPoolObject* pPtr )
	{
		MemItem *pMemItem = nullptr;

		if( pPtr == nullptr )
			return S_OK;

		pPtr->Dispose();

		pMemItem = (MemItem*)((BYTE*)pPtr - MEMITEM_SIZE);

		Assert( pMemItem->MemMagic == POOL_MEMMAGIC );
		Assert( pMemItem->Using == 1 );
		if( pMemItem->Using != 1
			|| pMemItem->MemMagic != POOL_MEMMAGIC )
		{
			// Drop memory that cause problem
		}
		else
		{
			CounterType Using = pMemItem->Using.fetch_sub(1,std::memory_order_relaxed) - 1;
			Assert( Using == 0 );
			//Assert( Using == pMemItem->Using );

			m_FreeList.Push( pMemItem );
		}

		
		m_AllocatedCount.fetch_sub(1, std::memory_order_relaxed);

		return S_OK;
	}


		



};	// namespace BR


