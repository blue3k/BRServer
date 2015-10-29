////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : utility
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/TypeUtility.h"




namespace BR {

	template<>
	INT64 DefaultValue<INT64>() { return 0; }

	template<>
	UINT64 DefaultValue<UINT64>() { return 0; }

	template<>
	INT32 DefaultValue<INT32>() { return 0; }

	template<>
	UINT32 DefaultValue<UINT32>() { return 0; }


} // namespace BR


