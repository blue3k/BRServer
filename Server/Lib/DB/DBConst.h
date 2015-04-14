////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : DB constant definitions
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"


namespace BR {
namespace DB {

	namespace Const
	{
		enum {

			// Default sharding buckets
			DEFAULT_SHARDING_BUCKETS = 128,

			// 
			DB_MAIN_THREAD_INTERVAL	= 1, 
			QUERYWORKER_MAX			= 20,

			DB_WORKER_JOB_WAITING_MAX  = 5000,

			DB_MANAGER_UPDATE_SHARDLIST_LIMIT = 5000,

			// retry connection every 500 ms
			CONNECTION_RETRY_TIME	= 500,

			// temporary setting buffer size
			MAX_DBCONFIGLOAD		= 64,					

			// User Name Max
			MAX_USERNAME			= 64,

			// Notification text Max
			MAX_NOTIFICATION_TEXT	= 512,

			// User email Max
			MAX_EMAIL				= 128,

			// Password Max
			MAX_PASSWORD			= 64,

			// User Name Max
			MAX_GCMKEY				= 512,

		};
	}

} // namespace DB
} // namespace BR
