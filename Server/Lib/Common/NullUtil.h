////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Null Utility library
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"

namespace BR {
namespace Util {

	/////////////////////////////////////////////////////////////////////
	//
	// Null check function
	//

	template< class ObjectType >
	class Nullable
	{
	public:

		static void SetNull( ObjectType &Obj )
		{
			__if_exists( ObjectType::NullValue )
			{
				Obj = ObjectType::NullValue;
			}
			__if_not_exists(ObjectType::NullValue)
			{
				Obj = 0;
			}
		}

		static bool IsNull( const ObjectType &Obj )
		{
			__if_exists( ObjectType::NullValue )
			{
				return Obj == ObjectType::NullValue;
			}
			__if_not_exists(ObjectType::NullValue)
			{
				return Obj == 0;
			}
		}
		
	};




}; // namespace Util
} // namespace BR


