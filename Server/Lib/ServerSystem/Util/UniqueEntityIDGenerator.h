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


#include "SFTypedefs.h"
#include "ServerLog/SvrLog.h"
#include "Container/SFStackPool.h"
#include "Multithread/SFSynchronization.h"
#include "MemoryManager/SFMemoryPool.h"
#include "Types/SFEngineTypedefs.h"


namespace SF {

	////////////////////////////////////////////////////////////////////////
	//
	//	UID generator
	//      - This class generates 
	//

	class UniqueEntityIDGenerator
	{
	public:

		// Item for stack pool
		struct Item : public StackPool::Item
		{
			uint UID;

			Item()
			{
				memset(this, 0, sizeof(Item));
			}
		};

	public:

		UniqueEntityIDGenerator();
		~UniqueEntityIDGenerator();

		// Generate New ID
		uint NewID();

		// Free Generated ID
		bool FreeID( uint uiID );

		// Reserve ID
		Result ReserveID( uint idToReserve );

		// Clear all IDs
		void ClearAllIDs();

	private:
		// Check ID integrety
		bool IsFreeID( uint id );

		// Mark as free id
		void MarkAsFree( uint id );

		// Mark as using id
		void MarkAsUsing( uint id );

	private:
		// Free UID list
		StackPool			m_FreeIDs;

		// ID allocation map
		enum {
			IDMASK_SHIFT		= 6,		// 2^6 = 64
			IDMASK_BITMASK		= 64 - 1,	// 
			IDMASK_MAXINDEX		= 1<<(EntityID::MAX_IDBIT-IDMASK_SHIFT),
		};
		std::atomic<uint64_t>		m_IDMask[IDMASK_MAXINDEX];

		// Counter for new ID
		SyncCounter			m_CounterForID;
	};


}; // namespace SF


