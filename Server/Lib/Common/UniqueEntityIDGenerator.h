////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : UID Generator
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Common/Typedefs.h"
#include "Common/Trace.h"
#include "Common/StackPool.h"
#include "Common/Synchronization.h"
#include "Common/MemoryPool.h"
#include "Common/BrBaseTypes.h"


namespace BR {

	////////////////////////////////////////////////////////////////////////
	//
	//	UID generator
	//      - This class generates 
	//

	class UniqueEntityIDGenerator
	{
	public:

		// Item for stack pool
		struct Item : public StackPool::Item, public MemoryPoolObject<Item>
		{
			UINT UID;

			Item()
			{
				memset(this, 0, sizeof(Item));
			}
		};

	public:

		UniqueEntityIDGenerator();
		~UniqueEntityIDGenerator();

		// Generate New ID
		UINT NewID();

		// Free Generated ID
		bool FreeID( UINT uiID );

		// Reserve ID
		HRESULT ReserveID( UINT idToReserve );

		// Clear all IDs
		void ClearAllIDs();

	private:
		// Check ID integrety
		bool IsFreeID( UINT id );

		// Mark as free id
		void MarkAsFree( UINT id );

		// Mark as using id
		void MarkAsUsing( UINT id );

	private:
		// Free UID list
		StackPool			m_FreeIDs;

		// ID allocation map
		enum {
			IDMASK_SHIFT		= 6,		// 2^6 = 64
			IDMASK_BITMASK		= 64 - 1,	// 
			IDMASK_MAXINDEX		= 1<<(EntityID::MAX_IDBIT-IDMASK_SHIFT),
		};
		std::atomic<UINT64>		m_IDMask[IDMASK_MAXINDEX];

		// Counter for new ID
		SyncCounter			m_CounterForID;
	};


}; // namespace BR


