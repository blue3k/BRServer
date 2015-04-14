////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Object Pool 
//
////////////////////////////////////////////////////////////////////////////////

#pragma once


#include "Common/Typedefs.h"
#include "Common/MemoryPool.h"


namespace BR
{


	

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Object Pool object implementation
	//

	class ObjectPoolObject
	{
	public:
		// Constructor, will be called only once
		ObjectPoolObject(){}
		// Destructor, clear all your dynamic allocated memory in Dispose() otherwise it's going to stay all the time.
		virtual ~ObjectPoolObject(){}

		// Called when the object is going to be reused
		virtual HRESULT Reuse(){ return S_OK; }
		// Called when the object is going to be disposed
		virtual HRESULT Dispose() { return S_OK; }
	};


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Object Pool
	//

	class ObjectPool
	{
	public:
		enum {
			// Pool item count range per page
			POOL_ITEM_MIN = 16,
			POOL_ITEM_MAX = 4096,

			POOL_MEMMAGIC = 0xABABABABL
		};

		// free stack item
		struct ObjectItem
		{
			int Using;
			ObjectItem *pNext;
			ObjectPoolObject *pObject;
		};

		enum {
			MEMITEM_SIZE = sizeof(ObjectItem),
			USE_DIRECTFREE = 0
		};


	private:

		// allocation unit size
		size_t	m_AllocSize;

		// Item count per page
		size_t m_AllocCountPerPage;

		// Allocated item count
		BR::SyncCounter	m_AllocatedCount;

		// Page allocate
		PageAllocator m_Allocator;

		// Free item stack
		ObjectItem*	m_FreeList;

		// 
		bool m_CallDestructor;

	private:

		// Decide Page size
		size_t DecidePageSize(size_t AllocSize);

	public:
		// Constructor for singleton
		ObjectPool(size_t objectSize);
		virtual ~ObjectPool();

		// Get allocation size
		size_t GetAllocSize() const						{ return m_AllocSize - MEMITEM_SIZE; }
		size_t GetActualAllocSize() const				{ return m_AllocSize; }

		// Call destructory when an object is actually freed
		void SetCallDestructor(bool callDestructor)		{ m_CallDestructor = callDestructor; }
		bool GetCallDestructor() const					{ return m_CallDestructor; }

		// force clear memory heap
		void ForceClear();

		// called when a object is created
		virtual ObjectPoolObject* OnNewObject(void* pNewObject) = 0;

		// Allocate/Free
		HRESULT Alloc(ObjectPoolObject* &pPtr);
		HRESULT Free(ObjectPoolObject* pPtr);
	};


	template< class ObjectType >
	class ObjectPoolT : public ObjectPool
	{
	public:
		ObjectPoolT()
			:ObjectPool(sizeof(ObjectType))
		{
		}

		virtual ~ObjectPoolT()
		{
		}

		virtual ObjectPoolObject* OnNewObject(void* pNewObject)
		{
			return new(pNewObject)ObjectType;
		}

		HRESULT Alloc(ObjectType* &pPtr)
		{
			ObjectPoolObject *pObj = nullptr;
			HRESULT hr = __super::Alloc(pObj);
			pPtr = (ObjectType*)(pObj);
			return hr;
		}

		HRESULT Free(ObjectType* pPtr)
		{
			return __super::Free(pPtr);
		}
	};


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Object Pool(Multithread)
	//

	BR_DECLARE_ALIGN class ObjectPoolMT
	{
	public:
		enum {
			// Pool item count range per page
			POOL_ITEM_MIN = 8,
			POOL_ITEM_MAX = 1024,

			POOL_MEMMAGIC = 0xABABABABL
		};

		// free stack item
		struct MemItem : public StackPool::Item
		{
			intptr_t MemMagic;
			SyncCounter Using;
			ObjectPoolObject *pObject;
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
		BR::SyncCounter	m_AllocatedCount;

		// Page allocate
		PageAllocator m_Allocator;

		// Free item stack
		BR::StackPool	m_FreeList;

	private:

		// Decide Page size
		size_t DecidePageSize( size_t AllocSize );

	public:
		// Constructor for singleton
		ObjectPoolMT(size_t objectSize);
		virtual ~ObjectPoolMT();

		// Get allocation size
		size_t GetAllocSize() const			{ return m_AllocSize - MEMITEM_SIZE; }
		size_t GetActualAllocSize() const	{ return m_AllocSize; }

		// force clear memory heap
		void ForceClear();

		// called when a object is created
		virtual ObjectPoolObject* OnNewObject( void* pNewObject ) = 0;

		// Allocate/Free
		HRESULT Alloc( ObjectPoolObject* &pPtr );
		HRESULT Free( ObjectPoolObject* pPtr );
	};


	template< class ObjectType >
	class ObjectPoolMTT : public ObjectPoolMT
	{
	public:
		ObjectPoolMTT()
			:ObjectPoolMT(sizeof(ObjectType))
		{
		}

		virtual ~ObjectPoolMTT()
		{
		}

		virtual ObjectPoolObject* OnNewObject( void* pNewObject )
		{
			return new(pNewObject) ObjectType;
		}

		HRESULT Alloc( ObjectType* &pPtr )
		{
			ObjectPoolObject *pObj = nullptr;
			HRESULT hr = __super::Alloc( pObj );
			pPtr = (ObjectType*)(pObj);
			return hr;
		}

		HRESULT Free( ObjectType* pPtr )
		{
			return __super::Free( pPtr );
		}
	};

	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Object Pool object implementation
	//


	template< class ObjectType >
	class ObjectPoolObjectT : public ObjectPoolObject
	{
	public:
		typedef ObjectPoolMTT<ObjectType> MyObjectPool;

	protected:
		// Memory pool that assigned with this class
		static MyObjectPool* stm_ObjectPool;

		ObjectPoolObjectT() {}
		virtual ~ObjectPoolObjectT() {}

		friend class ObjectPoolMTT<ObjectType>;

	public:

		void Release()
		{
			FreeObject((ObjectType*)this);
		}

		static ObjectType* AllocateObject()
		{
			Assert(stm_ObjectPool != nullptr);
			if( stm_ObjectPool == nullptr )
				return nullptr;

			ObjectType *pPtr = nullptr;
			stm_ObjectPool->Alloc( pPtr );
			return (ObjectType*)pPtr;
		}

		static void FreeObject( ObjectType* pObj )
		{
			Assert(stm_ObjectPool != nullptr);
			if( stm_ObjectPool == nullptr )
				return;

			stm_ObjectPool->Free( pObj );
		}
	};


	#define BR_OBJECTPOOL_IMPLEMENT(className) \
		namespace BR {\
		ObjectPoolMTT<className>* ObjectPoolObjectT<className>::stm_ObjectPool = new ObjectPoolMTT<className>; \
		};\





#include "ObjectPool.inl"


}; // namespace BR



