////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves 
// 
// Author: Kyungkun Ko
//
// Description : Data Base service
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "SFAssert.h"


/////////////////////////////////////////////////////////////////////////////
//
// Database service
//


namespace SF {
	namespace DB {

		class DataSource;
		class Session;
	};

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	DatabaseService Class 
	//

	class DatabaseService
	{
	public:

		DatabaseService(){}
		virtual ~DatabaseService(){}

		// Report DB error
		virtual void ReportError(void* DBContext, Result hr, const char* className) {}

		// initialize DB source
		virtual Result	CreateDataSource(IHeap& memMgr, DB::DataSource*& pDBSource) { return ResultCode::NOT_IMPLEMENTED; }

		// close DB source
		virtual Result	CreateSession(IHeap& memMgr, DB::DataSource* pDBSource, DB::Session*& pSession) { return ResultCode::NOT_IMPLEMENTED; }

	};

} // namespace SF
