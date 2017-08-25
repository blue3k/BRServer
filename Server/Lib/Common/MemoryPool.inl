////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Memory Pool
//	
//
////////////////////////////////////////////////////////////////////////////////



namespace BR
{


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Memory Page allocator
	//

	CounterType PageAllocator::GetPageCount() const
	{
		return m_PageCount;
	}




	FORCEINLINE StackPool& MemoryPool::PickFreeList()
	{
		auto index = m_FreeIndex.fetch_add(1, std::memory_order_relaxed) & FREE_LIST_MASK;
		return m_FreeList[index];
	}


	//
	//////////////////////////////////////////////////////////////////////////////////
	////
	////	Memory pool by type template
	////
	//
	//template< class ObjectType >
	//MemoryPoolT<ObjectType>::MemoryPoolT()
	//{
	//}
	//
	//// allocate object
	//template< class ObjectType >
	//Result MemoryPoolT<ObjectType>::Alloc( ObjectType* &pObject )
	//{
	//}
	//
	//// free object
	//template< class ObjectType >
	//Result MemoryPoolT<ObjectType>::Free( ObjectType* pObject )
	//{
	//}

}