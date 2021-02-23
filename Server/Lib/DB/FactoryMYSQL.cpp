////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author: Kyungkun Ko
//
// Description : Procees Requested DB Query
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "DBPch.h"
#include "SFTypedefs.h"
#include "DB/DBConfig.h"

#ifdef BRDB_USE_MYSQL

#include "SFTypedefs.h"
#include "Util/SFStrUtil.h"
#include "DBTrace.h"
#include "FactoryMYSQL.h"
#include "DataSourceMYSQL.h"
#include "SessionMYSQL.h"


namespace SF {
namespace DB {


	FactoryMYSQL::FactoryMYSQL()
	{
	}

	FactoryMYSQL::~FactoryMYSQL()
	{
	}

	void FactoryMYSQL::ReportError(void* DBContext, Result hr, const char* className)
	{
		// Skip routing error
		if (hr == Result(ResultCode::INVALID_ENTITY))
			return;

		dbTrace( Error, "DB Failed Error hr:{0:X8}, {1}", hr, className );
	}

	// initialize DB source
	Result	FactoryMYSQL::CreateDataSource(IHeap& memMgr, DataSource* &pDBSource)
	{
		if( (pDBSource = new(memMgr) DataSourceMYSQL(memMgr)) != nullptr )
			return ResultCode::SUCCESS;

		return ResultCode::OUT_OF_MEMORY;
	}

	// close DB source
	Result	FactoryMYSQL::CreateSession(IHeap& memMgr, DataSource* pDBSource, Session* &pSession)
	{
		if( (pSession = new(memMgr) SessionMYSQL(memMgr, (DataSourceMYSQL*)pDBSource)) != nullptr)
			return ResultCode::SUCCESS;

		return ResultCode::OUT_OF_MEMORY;
	}

} // namespace DB
} // namespace SF

#endif // BRDB_USE_MYSQL
