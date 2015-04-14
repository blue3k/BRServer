////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 MadK
// 
// Author : KyungKun Ko
//
// Description : Thread utilities Stack base object pool
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/BrAssert.h"
#include "Common/Synchronize.h"
#include "Common/memory.h"



namespace BR
{

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Stack base object pool with Multithread access
	//

	BR_DECLARE_ALIGN class StackPool
	{
	public:

		// Node that pool object must have or inherite
		struct Item
		{
			Item* pNext;
		};



		// Header

#pragma pack(push)
#pragma pack(BR_ALIGN_DOUBLE)
		BR_DECLARE_ALIGN_DOUBLE struct Header
		{
			volatile Item* pHead;
			volatile CounterType TSeqID;

			inline Header();
		};

	private:

		// Header
		Header m_head;
#pragma pack(pop)

		// Size
		SyncCounter m_lSize;

		// thread ticketing ID
		Ticketing	m_Ticket;


	public:

		// Constructor
		StackPool();
		virtual ~StackPool();

		// Clear
		// NOTE: This mehtod is not thread safe
		void Clear();

		// Foreach functor
		// NOTE: This mehtod is not thread safe
		template< class Functor >
		void for_each( Functor func );

		// GetSize
		inline CounterType GetSize() const;

		// Pop item 
		Item* Pop();

		// Push item
		void Push( Item* pEntry );

	};


	

#include "StackPool.inl"


}; // namespace BR

