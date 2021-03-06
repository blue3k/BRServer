////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : UID Generator
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"
#include "Util/UniqueEntityIDGenerator.h"




namespace SF {

	UniqueEntityIDGenerator::UniqueEntityIDGenerator()
		:m_CounterForID(1)
	{
		memset( m_IDMask, 0, sizeof(m_IDMask) );
	}

	UniqueEntityIDGenerator::~UniqueEntityIDGenerator()
	{
	}

	// Generate New ID
	uint UniqueEntityIDGenerator::NewID()
	{
		uint uiNewID = 0;
		Item *pItem = (Item*)m_FreeIDs.Pop();

		if( pItem )
		{
			uiNewID = pItem->UID;
			IHeap::Delete(pItem);
		}
		else
		{
			do {
				// gen a new ID
				uiNewID = (uint)m_CounterForID.fetch_add(1, std::memory_order_relaxed);
			} while( !IsFreeID(uiNewID) );

			// maximum entityID bit, This case shouldn't be happened
			AssertRel( uiNewID > 0 && uiNewID < (1<<EntityID::MAX_IDBIT) );
		}

		AssertRel( IsFreeID(uiNewID) );
		MarkAsUsing(uiNewID);

		return uiNewID;
	}

	// Free Generated ID
	bool UniqueEntityIDGenerator::FreeID( uint uiID )
	{
		uint index = uiID >> IDMASK_SHIFT;

		AssertRel(index < IDMASK_MAXINDEX);
		if (index >= IDMASK_MAXINDEX)
		{
			// Invalid ID release
			defTrace(Debug1, "Invalid ID release : {0}", uiID);
			return false;
		}

		if (IsFreeID(uiID))
		{
			//Assert(0);
			// duplicated ID free
			defTrace(Debug1, "Duplicated ID free : {0}", uiID);
			return false;
		}

		Item *pItem = new(GetSystemHeap()) Item;

		pItem->UID = uiID;

		MarkAsFree(uiID);
		m_FreeIDs.Push(pItem);

		return true;
	}


	// Clear all IDs
	void UniqueEntityIDGenerator::ClearAllIDs()
	{
		m_FreeIDs.for_each( [](StackPool::Item *pItem) 
		{
			Item* pMyItem = (Item*)pItem;
			IHeap::Delete(pMyItem);
		});
		m_FreeIDs.Clear();
		m_CounterForID = 0;
		memset( m_IDMask, 0, sizeof(m_IDMask) );
	}

	// Reserve ID
	Result UniqueEntityIDGenerator::ReserveID( uint idToReserve )
	{
		if( !IsFreeID(idToReserve) )
			return ResultCode::FAIL;

		MarkAsUsing(idToReserve);
		return ResultCode::SUCCESS;
	}

	// Check ID integrety
	bool UniqueEntityIDGenerator::IsFreeID( uint id )
	{
		uint index = id >> IDMASK_SHIFT;
		uint bit = id & IDMASK_BITMASK;

		AssertRel( index < IDMASK_MAXINDEX );

		// bit must be 0
		return (m_IDMask[index].load(std::memory_order_relaxed) & ((uint64_t)1 << bit)) == 0;
	}

	// Mark as free id
	void UniqueEntityIDGenerator::MarkAsFree( uint id )
	{
		uint index = id >> IDMASK_SHIFT;
		uint bit = id & IDMASK_BITMASK;
		uint64_t bitMask = (uint64_t)1<<bit;

		AssertRel( index < IDMASK_MAXINDEX );
		AssertRel( (m_IDMask[index].load(std::memory_order_relaxed) & bitMask) != 0 );

		// clear the bit
		//m_IDMask[index] &= ~bitMask;
		m_IDMask[index].fetch_and(~bitMask, std::memory_order_release);
	}

	// Mark as using id
	void UniqueEntityIDGenerator::MarkAsUsing( uint id )
	{
		uint index = id >> IDMASK_SHIFT;
		uint bit = id & IDMASK_BITMASK;
		uint64_t bitMask = (uint64_t)1<<bit;

		AssertRel( index < IDMASK_MAXINDEX );
		AssertRel((m_IDMask[index].load(std::memory_order_relaxed) & bitMask) == 0);

		// set the bit
		//m_IDMask[index] |= bitMask;
		m_IDMask[index].fetch_or(bitMask, std::memory_order_release);
	}
	
}; // namespace SF



