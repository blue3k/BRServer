////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : TaeHoon Jung
//
// Description : Linked list
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/BrAssert.h"
#include "Common/Synchronize.h"



namespace BR
{

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Linked list
	//

	template< class DataType, class SyncTrait = BR::ThreadSyncTraitNone >
	class LinkedList
	{
		// MT Safe
		__if_exists( SyncTrait::ThreadSafe )
		{
			#include "BRLinkedList_MT.inl"
		}

		// MT not safe
		__if_not_exists( SyncTrait::ThreadSafe )
		{
			#include "BRLinkedList_ST.inl"
		}
	};

}; // namespace BR
