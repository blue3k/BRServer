////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : hash trait
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/BrAssert.h"


namespace BR {
namespace Hash {

		/////////////////////////////////////////////////////////////////////////////////
		//
		//	trait definition, used for UniqueKey or non UniqueKey trait
		//

		struct UniqueKeyTrait
		{
			const bool UniqueKey = true;
		};

		struct NonUniqueKeyTrait
		{
			const bool UniqueKey = false;
		};

		// Hash map data mappter
		template< typename KeyType, typename ItemType >
		struct MapItem
		{
			KeyType Key;
			ItemType Data;
		};
		

}; // namespace Hash
}; // namespace BR




