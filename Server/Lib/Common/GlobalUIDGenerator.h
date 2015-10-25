////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Local UID Generator
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/Trace.h"
#include "Common/Synchronize.h"
#include "Common/BrSvrTypes.h"


namespace BR {

	////////////////////////////////////////////////////////////////////////
	//
	//	Global UID generator
	//      - Generated value is unique only in single server porcess
	//		- NOTE: This isn't thread safe for generation
	//

	class GlobalUIDGenerator
	{
	public:

		enum { 
			LIMIT_TIME_DIFF		= 1 << GlobalUID::BIT_ID,
			UPDATE_TIME_DIFF	= 1 << (GlobalUID::BIT_ID-2),
		};

	private:
		UINT m_ServerID;

		// Cached Time
		mutable UINT32 m_time;

		// circular counter
		mutable SyncCounter m_CounterForID;
		mutable SyncCounter m_GeneratorSync;

	public:

		GlobalUIDGenerator();

		inline void SetServerID( UINT serverID ) { m_ServerID = serverID; AssertRel( serverID < 256 ); }

		// Generate New ID
		GlobalUID NewUID() const;

	};


}; // namespace BR

