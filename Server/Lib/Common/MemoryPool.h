////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Memory Pool 
//
////////////////////////////////////////////////////////////////////////////////

#pragma once


#include "Common/Typedefs.h"
#include "Common/BrLibComponents.h"
#include "Common/Synchronization.h"
#include "Common/StackPool.h"
#include "Common/StackWalker.h"
#include "Common/Synchronization.h"
#include "Common/SystemSynchronization.h"

//#define ENABLE_MEMORY_TRACE

namespace BR
{
	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Memory Page allocator
	//

	class PageAllocator
	{
	private:

		// Allocation Page size
		size_t	m_szPageSize;

		// Allocated Page Count
		SyncCounter	m_PageCount;

	public:
		// Constructor
		PageAllocator( size_t PageSize = 8*1024 );
		PageAllocator( size_t HeaderSize, size_t ElementSize );/////////
		virtual ~PageAllocator();

		// Clear
		void Clear();

		// Get page size
		virtual size_t GetPageSize() const;

		// Get page count that currently allocated
		inline CounterType GetPageCount() const;

		// Alloc/Free page method
		virtual Result Alloc( void* &pPtr );
		virtual Result Free( void* pPtr );

		// Suggest Page size
		static size_t SuggestElementCountPerPage( size_t HeaderSize, size_t ElementSize );
	};









	////////////////////////////////////////////////////////////////////////////////
	//
	//	Memory Pool
	//

	class MemoryPool
	{
	public:
		enum {
			// Pool item count range per page
			POOL_ITEM_MIN = 8,
			POOL_ITEM_MAX = 1024,

			POOL_MEMMAGIC = 0xCBCBCBCBL
		};

		// free stack item
		struct MemItem : public StackPool::Item
		{
			intptr_t MemMagic;
			Item* pPrev;
			//SyncCounter Using;
#ifdef ENABLE_MEMORY_TRACE
			CallStackTrace StackTrace;
			const char* TypeName;
			ThreadID LatestThreadID;
#endif

			void* DataPtr()
			{
				return ((BYTE*)this+MEMITEM_SIZE);
			}

			MemItem()
			{
				memset(this, 0, sizeof(MemItem));
			}
		};

		enum {
			MEMITEM_SIZE = BR_ALLIGNUP(sizeof(MemItem),BR_ALIGN_DOUBLE)
		};


	private:

		// allocation unit size
		size_t	m_AllocSize;

		// Item count per page
		size_t m_AllocCountPerPage;

		// Allocated item count
		SyncCounter	m_AllocatedCount;

		// Page allocate
		PageAllocator m_Allocator;

		// Free item stack
		StackPool	m_FreeList;

#ifdef ENABLE_MEMORY_TRACE
		BR::CriticalSection m_CriticalSection;
		MemItem m_AllocatedHead;
		bool m_printAlocList;
#endif

	private:

		// Decide Page size
		size_t DecidePageSize( size_t AllocSize );

	public:
		// Constructor for singleton
		MemoryPool( size_t AllocSize );
		virtual ~MemoryPool();

		// Get allocation size
		size_t GetAllocSize() const			{ return m_AllocSize - MEMITEM_SIZE; }
		size_t GetActualAllocSize() const	{ return m_AllocSize; }

		// force clear memory heap
		void ForceClear();

		// Allocate/Free
		Result Alloc( void* &pPtr, const char* typeName );
		Result Free( void* pPtr, const char* typeName );

		// Check pooled memory block header
		static bool CheckMemoryHeader(void* pPtr);

		void PrintAllocatedList();
	};




	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Memory Pool manager
	//

	class MemoryPoolManagerImpl;
	class MemoryPoolManager : public Component
	{
	public:
		enum {
			ComponentID = (UINT)LibCompoentIDs::MemoryPoolManager
		};

	private:

		static class MemoryPoolManagerImpl* stm_pInstance;

	public:

		MemoryPoolManager();
		~MemoryPoolManager();

		// Initialize server component
		virtual Result InitializeComponent() override;
		// Terminate server component
		virtual void TerminateComponent() override;

		// Initialize and Terminate memory pool
		static Result Initialize();
		static Result Terminate();

		// Get memory pool by size
		static Result GetMemoryPoolBySize( size_t allocationSize, MemoryPool* &pPool );
	};



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Memory Pool Object
	//
	


	template< class ObjectType >
	class MemoryPoolObject
	{
	private:
		// Memory pool that assigned with this class
		static MemoryPool* stm_MemoryPool;

	public:

		static void* operator new( std::size_t size )
		{
			Assert(sizeof(ObjectType)==size);
			if( stm_MemoryPool == nullptr )
			{
				MemoryPoolManager::GetMemoryPoolBySize( sizeof(ObjectType), stm_MemoryPool );
				Assert(stm_MemoryPool != nullptr);
			}
			void *pPtr = NULL;
			stm_MemoryPool->Alloc( pPtr, typeid(ObjectType).name() );
			return pPtr;
		}

		static void operator delete(void *pPtr )
		{
			stm_MemoryPool->Free( pPtr, typeid(ObjectType).name() );
		}


		// Cache object to memory pool
		static void MemoryPoolCache( int iCountToCache )
		{
			std::vector<ObjectType*> CacheTest;
			for( int iCache = 0; iCache < iCountToCache; iCache++ )
			{
				CacheTest.push_back( new ObjectType );
			}

			std::for_each( CacheTest.begin(), CacheTest.end(), [](ObjectType* pObj)
			{
				if( pObj )
					delete pObj;
			});
		}
	};
	


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Memory Pool Container
	//


	template< class ObjectType >
	class MemoryPoolContainer
	{
	private:
		// Memory pool that assigned with this class
		static MemoryPool* stm_MemoryPool;

	public:

		static void* operator new( std::size_t size )
		{
			Assert(sizeof(ObjectType)==size);
			if( stm_MemoryPool == nullptr )
			{
				MemoryPoolManager::GetMemoryPoolBySize( sizeof(ObjectType), stm_MemoryPool );
				Assert(stm_MemoryPool != nullptr);
			}
			void *pPtr = NULL;
			stm_MemoryPool->Alloc( pPtr, typeid(ObjectType).name() );
			return pPtr;
		}

		static void operator delete(void *pPtr)
		{
			stm_MemoryPool->Free( pPtr, typeid(ObjectType).name() );
		}

		// Cache object to memory pool
		static void MemoryPoolCache( int iCountToCache )
		{
			std::vector<ObjectType*> CacheTest;
			for( int iCache = 0; iCache < iCountToCache; iCache++ )
			{
				CacheTest.push_back( new ObjectType );
			}

			std::for_each( CacheTest.begin(), CacheTest.end(), [](ObjectType* pObj)
			{
				if( pObj )
					delete pObj;
			});
		}
	};


	#define BR_MEMORYPOOL_IMPLEMENT(className) \
		namespace BR {\
			template<>\
			MemoryPool* MemoryPoolObject<className>::stm_MemoryPool = nullptr;\
		};\





#include "MemoryPool.inl"


}; // namespace BR



