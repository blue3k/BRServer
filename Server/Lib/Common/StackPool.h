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
#include "SFAssert.h"
#include "Common/Synchronization.h"
#include "Common/BrMemory.h"



namespace BR
{

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Stack base object pool with Multithread access
	//

	class StackPool
	{
	public:

		// Node that pool object must have or inherite
		struct Item
		{
			Item* pNext;
		};



		// Header


	private:

		// Header
		std::atomic<Item*> m_head;

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

