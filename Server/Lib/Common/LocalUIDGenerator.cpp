////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Local UID Generator
//
////////////////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include "Common/LocalUIDGenerator.h"




namespace BR {

	// Generate New ID
	LocalUID LocalUIDGenerator::NewUID()
	{
		LocalUID uid;

		uid.Time = _time32(nullptr);
		// we are going to use only 32 least significant bit, This should be enough while less than 4G IDs are generated for a sec.
		uid.ID = (UINT32)m_CounterForID.fetch_add(1, std::memory_order_relaxed) + 1;

		return uid;
	}

}; // namespace BR



