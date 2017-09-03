////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Local UID Generator
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Util/LocalUIDGenerator.h"
#include "Util/TimeUtil.h"




namespace SF {

	// Generate New ID
	LocalUID LocalUIDGenerator::NewUID()
	{
		LocalUID uid;

		uid.Time = Util::Time.GetRawUTCSec().time_since_epoch().count();
		// we are going to use only 32 least significant bit, This should be enough while less than 4G IDs are generated for a sec.
		uid.ID = (uint32_t)m_CounterForID.fetch_add(1, std::memory_order_relaxed) + 1;

		return uid;
	}

}; // namespace BR



