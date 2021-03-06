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
#include "Multithread/SFSynchronization.h"
#include "Types/BrSvrTypes.h"


namespace SF {

	////////////////////////////////////////////////////////////////////////
	//
	//	Global UID generator
	//      - Generated value is unique only in single server porcess
	//

	class LocalUIDGenerator
	{
	private:

		// circular counter
		SyncCounter m_CounterForID;

	public:

		// Generate New ID
		LocalUID NewUID();

	};


}; // namespace BR


