////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : KyungKun Ko
//
// Description : Utility library
//	
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"


namespace BR {

	template <class DataType>
	DataType DefaultValue() { return nullptr; }

	template<>
	INT64 DefaultValue<INT64>() { return 0; }

	template<>
	UINT64 DefaultValue<UINT64>() { return 0; }

	template<>
	INT32 DefaultValue<INT32>() { return 0; }

	template<>
	UINT32 DefaultValue<UINT32>() { return 0; }

} // namespace BR


