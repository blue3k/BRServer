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

#include "SFTypedefs.h"
#include "ServerLog/SvrLog.h"
#include "Thread/SFSynchronization.h"
#include "Types/BrSvrTypes.h"


namespace SF {

	////////////////////////////////////////////////////////////////////////
	//
	//	Global UID generator
	//      - Generated value is unique only in single server porcess
	//		- NOTE: This isn't thread safe for generation
	//

	class GlobalUIDGenerator
	{
	public:

		static constexpr CounterType MAX_INDEX_DIFF = 1 << (GlobalUID::BIT_ID - 2);

	private:
		uint m_ServerID;

		// Cached Time
		mutable UTCTimeStampSec m_time;

		// circular counter
		mutable SyncCounter m_CounterForID;
		mutable SyncCounter m_GeneratorSync;

	public:

		GlobalUIDGenerator();

		void SetServerID(uint serverID);

		// Generate New ID
		GlobalUID NewUID() const;

	};


}; // namespace BR


