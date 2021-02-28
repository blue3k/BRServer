////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves 
// 
// Author: Kyungkun Ko
//
// Description : DB Factory interface
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "SFAssert.h"
#include "Object/SFSharedObject.h"
#include "Service/DatabaseService.h"


namespace SF {
namespace DB {

	class DataSource;
	class Session;


	//////////////////////////////////////////////////////////////////////////////////
	//
	//	DB Factory Class 
	//

	class FactoryOLEDB : public DatabaseService
	{
	public:

		virtual void ReportError(void* DBContext, Result hr, const char* className) override;

		// initialize DB source
		virtual Result	CreateDataSource(IHeap& memMgr, DataSource* &pDBSource) override;

		// close DB source
		virtual Result	CreateSession(IHeap& memMgr, DataSource* pDBSource, Session* &pSession) override;
	};

} // namespace DB
} // namespace SF
