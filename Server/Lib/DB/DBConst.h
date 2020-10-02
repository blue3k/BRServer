////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author: Kyungkun Ko
//
// Description : DB constant definitions
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"


namespace SF {
	namespace DB {

		namespace Const
		{

			// Default sharding buckets
			constexpr int DEFAULT_SHARDING_BUCKETS = 128;

			// 
			constexpr int DB_MAIN_THREAD_INTERVAL = 1;
			constexpr int QUERYWORKER_MAX = 20;

			constexpr int DB_WORKER_JOB_WAITING_MAX = 5000;

			constexpr int DB_MANAGER_UPDATE_SHARDLIST_LIMIT = 5000;

			// retry connection every 500 ms
			constexpr int CONNECTION_RETRY_TIME = 500;

			// temporary setting buffer size
			constexpr int MAX_DBCONFIGLOAD = 64;

			// User Name Max
			constexpr int MAX_USERNAME = 128;

			constexpr int MAX_PLATFORM_NAME = 64;
			constexpr int MAX_PURCHASEID = 64;
			constexpr int MAX_PURCHASETOKEN = 512;

			// Notification text Max
			constexpr int MAX_NOTIFICATION_TEXT = 512;

			// User email Max
			constexpr int MAX_EMAIL = 128;
			constexpr int MAX_CELLPHONE = 64;
			constexpr int MAX_COMPLITIONSTATE = 16;

			// Password Max
			constexpr int MAX_PASSWORD = 64;

			// User Name Max
			constexpr int MAX_GCMKEY = 512;
		}

	} // namespace DB
} // namespace SF
