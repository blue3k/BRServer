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


#pragma once


#include "Common/Typedefs.h"
#include "Common/ResultCode/BRResultCodeSystem.h"

#include "ServerSystem/SvrTrace.h"
#include "Common/Utility.h"
#include "Common/MemoryPool.h"
#include "Common/HashTable.h"
#include "Common/Indexing.h"
#include "Common/PageQueue.h"
#include "Common/UniqueEntityIDGenerator.h"



namespace BR {
namespace Svr {

	//////////////////////////////////////////////////////////////////////////////
	//
	// Entity manager class
	//

	template< class DataObject, class TTicketType = void* >
	class TicketQueue
	{
	public:

		// Ticket type
		typedef TTicketType TicketType;


		//////////////////////////////////////////////////////////////////////////////
		//
		// Ticket class
		//

		class ItemTicket : public BR::MemoryPoolObject<ItemTicket>
		{
		private:
			// Ticket ID
			BRCLASS_ATTRIBUTE_CONST(bool, Droped)

			// Data
			BRCLASS_ATTRIBUTE_RVALUE(DataObject, Data)

		private:

			ItemTicket( DataType&& Data ):m_Droped(false), m_Data(Data)	{}
			~ItemTicket()	{}

			// make hashing module can convert to hash id
			operator intptr_t() { return (intptr_t)this; }

			friend class TicketQueue;

		public:
		};


		// Ticket Hash
		typedef BR::Hash::HashTable<	ItemTicket*, 
											BR::Indexing::Self<ItemTicket,intptr_t>
											> TicketMap;

	private:
		// Map for ticketing
		TicketMap					m_TicketMap;

		// Ticket Queue Data
		BR::PageQueue<ItemTicket*>	m_Queue;



		//////////////////////////////////////////////////////////////////////////
		//
		//	Internal operations
		//

		// Assign Item
		Result AssignItem( const DataObject&& Data, ItemTicket* &pItem );
		Result AssignItem( DataObject&& Data, ItemTicket* &pItem );

		// Release Item
		Result ReleaseItem( ItemTicket* pItem );


	public:

		TicketQueue( int iDataPerPage = -1, int iBucketSize = 1024 );
		virtual ~TicketQueue();


		//////////////////////////////////////////////////////////////////////////
		//
		//	
		//

		// item enque
		inline Result Enqueue( const DataObject& newData, TicketType& hTicket );
		inline Result Enqueue( DataObject&& newData, TicketType& hTicket );

		// item deque
		inline Result Dequeue( DataObject& Data );

		// drop item in queue of that ticket
		inline Result DropItem( TicketType hTicket );

		// Just get first dequeue item if exist, not dequeue
		// This will not safe if use DequeueMT
		inline Result GetFront( DataObject& item );

		// Item count in queue
		inline CounterType GetEnqueCount() const;

		// Clear queue and remove all enqueued items
		// This operation is not thread safe
		void ClearQueue();

	};

#include "TicketQueue.inl"


}; // namespace Svr
}; // namespace BR


