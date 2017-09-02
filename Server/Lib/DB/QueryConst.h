////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Madk
//
// Description : DB Query related constants
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"

namespace SF {
namespace DB {

	// binary sizes
	enum { 
		// this can affect to max name string
		BINSIZE_NAME				= 64,
		MAX_TRANSACTION_LOG_MESSAGE = 1024,

		MAX_QUERY_TIMEOUT = 1 * 60 * 1000,
	};

} // namespace DB
} // namespace SF

