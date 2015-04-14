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


