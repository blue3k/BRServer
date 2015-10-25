////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Thread utilities Stack base object pool
//
////////////////////////////////////////////////////////////////////////////////






StackPool::Header::Header()
	:pHead(nullptr),
	TSeqID(0)
{
	AssertRel( ((intptr_t)this & (BR_ALIGN_DOUBLE-1)) == 0 );
}


// Foreach functor
// NOTE: This mehtod not thread safe
template< class Functor >
void StackPool::for_each( Functor func )
{
	Item *pItem = (Item*)m_head.pHead;
	while( pItem )
	{
		Item* pNext = pItem->pNext;
		func( pItem );
		pItem = pNext;
	}
}

CounterType StackPool::GetSize() const
{
	return m_lSize;
}

