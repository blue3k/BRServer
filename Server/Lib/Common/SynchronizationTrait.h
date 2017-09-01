////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 MadK
// 
// Author : KyungKun Ko
//
// Description : Thread Synchronize
//
////////////////////////////////////////////////////////////////////////////////



#pragma once

#include "Common/Typedefs.h"
#include "SFAssert.h"
#include "Common/BrMemory.h"
#include "Common/Synchronization.h"
#include "Common/SystemSynchronization.h"
#include "Common/SortedMap.h"
#include "Common/DualSortedMap.h"



namespace BR
{





	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Synchronization trait
	//

	// Synchronization trait, not thread safe
	struct ThreadSyncTraitNone
	{
		enum Consts {
			ThreadSafe = 0
		};

		typedef FakeTicketLock	TicketLockType;
	};

	// Synchronization trait, thread safe
	struct ThreadSyncTraitReadWrite
	{
		enum Consts {
			ThreadSafe = 1
		};
		typedef TicketLock	TicketLockType;
		//typedef SortedMap<class KeyType, class ValueType> BucketContainer;
	};



	template<class KeyType, class ValueType>
	struct ThreadSyncTraitNoneT
	{
		enum Consts {
			ThreadSafe = 0
		};
		typedef TicketLock	TicketLockType;
		typedef SortedMap<KeyType, ValueType> BucketContainer;
	};

	template<class KeyType, class ValueType>
	struct ThreadSyncTraitReadWriteT
	{
		enum Consts {
			ThreadSafe = 1
		};
		typedef TicketLock	TicketLockType;
		typedef DualSortedMap<KeyType, ValueType> BucketContainer;
	};

	// Synchronization trait, thread safe
	template<class KeyType, class ValueType>
	struct ThreadSyncTraitWriteT
	{
		enum Consts {
			ThreadSafe = 1
		};

		typedef WriteOnlyTicketLock	TicketLockType;
		typedef DualSortedMap<KeyType, ValueType> BucketContainer;
	};






}; // namespace BR

