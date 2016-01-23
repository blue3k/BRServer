////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Ticket queue
//	
//
////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//
//	Internal operations
//

// Assign Item
template< class DataObject, class TTicketType >
HRESULT TicketQueue<DataObject,TTicketType>::AssignItem( const DataObject&& Data, ItemTicket* &pItem )
{
	pItem = new ItemTicket(Data);

	if( pItem == nullptr )
		return E_SYSTEM_POINTER;

	HRESULT hr = m_TicketMap.insert( pItem );
	if( FAILED(hr) )
		Util::SafeDelete( pItem );
	return hr;
}


template< class DataObject, class TTicketType >
HRESULT TicketQueue<DataObject,TTicketType>::AssignItem( DataObject&& Data, ItemTicket* &pItem )
{
	pItem = new ItemTicket(Data);

	if( pItem == nullptr )
		return E_SYSTEM_POINTER;

	HRESULT hr = m_TicketMap.insert( pItem );
	if( FAILED(hr) )
		Util::SafeDelete( pItem );
	return hr;
}

// Drop Item
template< class DataObject, class TTicketType >
HRESULT TicketQueue<DataObject,TTicketType>::ReleaseItem( ItemTicket* pItem )
{
	if( pItem == nullptr ) return E_SYSTEM_POINTER;

	m_TicketMap.erase( pItem );

	Util::SafeDelete(pItem);
}




template< class DataObject, class TTicketType >
TicketQueue<DataObject,TTicketType>::TicketQueue( int iDataPerPage, int iBucketSize )
	:m_Queue(iDataPerPage),
	m_TicketMap(iBucketSize)
{
}

template< class DataObject, class TTicketType >
TicketQueue<DataObject,TTicketType>::~TicketQueue()
{
	ClearQueue();
}


//////////////////////////////////////////////////////////////////////////
//
//	
//

// item enque
template< class DataObject, class TTicketType >
HRESULT TicketQueue<DataObject,TTicketType>::Enqueue( const DataObject& newData, TicketType& hTicket )
{
	HRESULT hr = S_SYSTEM_OK;
	ItemTicket *pItem = nullptr;

	svrChk( AssignItem( newData, pItem ) );

Proc_End:
	if( FAILED(hr) )
		ReleaseItem( pItem );
	else
		hTicket = pItem;
	return hr;
}

template< class DataObject, class TTicketType >
HRESULT TicketQueue<DataObject,TTicketType>::Enqueue( DataObject&& newData, TicketType& hTicket )
{
	HRESULT hr = S_SYSTEM_OK;
	ItemTicket *pItem = nullptr;

	svrChk( AssignItem( newData, pItem ) );
	svrChk( m_Queue.Enqueue( pItem ) );

Proc_End:
	if( FAILED(hr) )
		ReleaseItem( pItem );
	else
		hTicket = pItem;
	return hr;
}

// item deque
template< class DataObject, class TTicketType >
HRESULT TicketQueue<DataObject,TTicketType>::Dequeue( DataObject& Data )
{
	HRESULT hr = S_SYSTEM_OK;
	ItemTicket *pItem = nullptr;

	svrChk( AssignItem( newData, pItem ) );

Proc_End:
	if( pItem )
	{
		Data = pItem->GetData();
		ReleaseItem( pItem );
	}
	return hr;
}

// drop item in queue of that ticket
template< class DataObject, class TTicketType >
HRESULT TicketQueue<DataObject,TTicketType>::DropItem( TicketType hTicket )
{
	HRESULT hr = S_SYSTEM_OK;
	TicketMap::Iterator itItem;

	svrChk( m_TicketMap.find( (ItemTicket*)hTicket, itItem ) );

	itItem->SetDroped( true );

Proc_End:
	return hr;
}

// Just get first dequeue item if exist, not dequeue
// This will not safe if use DequeueMT
template< class DataObject, class TTicketType >
HRESULT TicketQueue<DataObject,TTicketType>::GetFront( DataObject& item )
{
	ItemTicket* pItem = nullptr;
	while( m_Queue.GetEnqueCount() > 0 && SUCCEEDED(m_Queue.GetFront(pItem)) )
	{
		if( pItem->GetDroped() )
			Util::SafeDelete(pItem);
		else
			return pItem->GetData();
	}}

// Item count in queue
template< class DataObject, class TTicketType >
CounterType TicketQueue<DataObject,TTicketType>::GetEnqueCount() const
{
	return m_Queue.GetEnqueCount();
}

// Clear queue and remove all enqueued items
// This operation is not thread safe
template< class DataObject, class TTicketType >
void TicketQueue<DataObject,TTicketType>::ClearQueue()
{
	ItemTicket* pItem = nullptr;
	while( m_Queue.GetEnqueCount() > 0 )
	{
		if( SUCCEEDED(m_Queue.Dequeue(pItem)) )
		{
			Util::SafeDelete(pItem);
		}
	}

	m_TicketMap.clear();
}



