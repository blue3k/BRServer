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
			enum Consts {
				UniqueKey = 1
			};
		};

		struct NonUniqueKeyTrait
		{
			enum Consts {
				UniqueKey = 0
			};
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




