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
#include "Common/Synchronization.h"



namespace BR
{

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Linked list
	//

	template< class DataType, class SyncTrait = BR::ThreadSyncTraitNone >
	class LinkedList
	{
		// MT Safe. TODO
		//if( SyncTrait::ThreadSafe )
		//{
		//	#include "BRLinkedList_MT.inl"
		//}
		//else // MT not safe
		//{
			#include "BRLinkedList_ST.inl"
		//}
	};

}; // namespace BR

