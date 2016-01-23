////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : UID Generator
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/UniqueEntityIDGenerator.h"


BR_MEMORYPOOL_IMPLEMENT(UniqueEntityIDGenerator::Item);


namespace BR {

	UniqueEntityIDGenerator::UniqueEntityIDGenerator()
		:m_CounterForID(1)
	{
		memset( m_IDMask, 0, sizeof(m_IDMask) );
	}

	UniqueEntityIDGenerator::~UniqueEntityIDGenerator()
	{
	}

	// Generate New ID
	UINT UniqueEntityIDGenerator::NewID()
	{
		UINT uiNewID = 0;
		Item *pItem = (Item*)m_FreeIDs.Pop();

		if( pItem )
		{
			uiNewID = pItem->UID;
			delete pItem;
		}
		else
		{
			do {
				// gen a new ID
				uiNewID = (UINT)m_CounterForID.fetch_add(1, std::memory_order_relaxed);
			} while( !IsFreeID(uiNewID) );

			// maximum entityID bit, This case shouldn't be happened
			AssertRel( uiNewID > 0 && uiNewID < (1<<EntityID::MAX_IDBIT) );
		}

		AssertRel( IsFreeID(uiNewID) );
		MarkAsUsing(uiNewID);

		return uiNewID;
	}

	// Free Generated ID
	bool UniqueEntityIDGenerator::FreeID( UINT uiID )
	{
		UINT index = uiID >> IDMASK_SHIFT;

		AssertRel(index < IDMASK_MAXINDEX);
		if (index >= IDMASK_MAXINDEX)
		{
			// Invalid ID release
			defTrace(Trace::TRC_DBG1, "Invalid ID release : {0}", uiID);
			return false;
		}

		if (IsFreeID(uiID))
		{
			//Assert(0);
			// duplicated ID free
			defTrace(Trace::TRC_DBG1, "Duplicated ID free : {0}", uiID);
			return false;
		}

		Item *pItem = new Item;

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
			delete pMyItem;
		});
		m_FreeIDs.Clear();
		m_CounterForID = 0;
		memset( m_IDMask, 0, sizeof(m_IDMask) );
	}

	// Reserve ID
	HRESULT UniqueEntityIDGenerator::ReserveID( UINT idToReserve )
	{
		if( !IsFreeID(idToReserve) )
			return E_SYSTEM_FAIL;

		MarkAsUsing(idToReserve);
		return S_SYSTEM_OK;
	}

	// Check ID integrety
	bool UniqueEntityIDGenerator::IsFreeID( UINT id )
	{
		UINT index = id >> IDMASK_SHIFT;
		UINT bit = id & IDMASK_BITMASK;

		AssertRel( index < IDMASK_MAXINDEX );

		// bit must be 0
		return (m_IDMask[index].load(std::memory_order_relaxed) & ((UINT64)1 << bit)) == 0;
	}

	// Mark as free id
	void UniqueEntityIDGenerator::MarkAsFree( UINT id )
	{
		UINT index = id >> IDMASK_SHIFT;
		UINT bit = id & IDMASK_BITMASK;
		UINT64 bitMask = (UINT64)1<<bit;

		AssertRel( index < IDMASK_MAXINDEX );
		AssertRel( (m_IDMask[index].load(std::memory_order_relaxed) & bitMask) != 0 );

		// clear the bit
		//m_IDMask[index] &= ~bitMask;
		m_IDMask[index].fetch_and(~bitMask, std::memory_order_release);
	}

	// Mark as using id
	void UniqueEntityIDGenerator::MarkAsUsing( UINT id )
	{
		UINT index = id >> IDMASK_SHIFT;
		UINT bit = id & IDMASK_BITMASK;
		UINT64 bitMask = (UINT64)1<<bit;

		AssertRel( index < IDMASK_MAXINDEX );
		AssertRel((m_IDMask[index].load(std::memory_order_relaxed) & bitMask) == 0);

		// set the bit
		//m_IDMask[index] |= bitMask;
		m_IDMask[index].fetch_or(bitMask, std::memory_order_release);
	}
	
}; // namespace BR



