////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : MadK
//
// Description : Procees Requested DB Query
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Factory.h"

#ifdef BRDB_USE_MYSQL

#include "SFTypedefs.h"
#include "String/StrUtil.h"
#include "DBTrace.h"
#include "FactoryMYSQL.h"
#include "DataSourceMYSQL.h"
#include "SessionMYSQL.h"

#if WINDOWS
#pragma comment(lib, "libmysql.lib")
#else
#endif

namespace SF {
namespace DB {


	FactoryMYSQL::FactoryMYSQL()
	{
		mysql_library_init(0,NULL,NULL);
	}

	FactoryMYSQL::~FactoryMYSQL()
	{
		mysql_library_end();
	}
	
	// Make this factory as the DB factory
	Result FactoryMYSQL::Instanciate()
	{
		Result hr = ResultCode::SUCCESS;

		dbMem( Factory::stm_pInstance = new FactoryMYSQL );

	Proc_End:

		return hr;
	}

	void FactoryMYSQL::ReportError( void* DBContext, Result hr, const char* className )
	{
		MYSQL *pSql = DBContext ? (MYSQL*)DBContext : nullptr;

		// Skip routing error
		if (hr == Result(ResultCode::INVALID_ENTITY))
			return;

		if( pSql )
		{
			const char* errorString = mysql_error(pSql);
			AssertRel(errorString!=nullptr);
			dbTrace( Error, "DB Failed Error hr:{0:X8}, {1} : {2} ", hr, className, errorString );
		}
		else
		{
			dbTrace( Error, "DB Failed Error hr:{0:X8}, {1}", hr, className );
		}
	}

	// initialize DB source
	Result	FactoryMYSQL::CreateDataSource(IHeap& memMgr, DataSource* &pDBSource )
	{
		if( (pDBSource = new(memMgr) DataSourceMYSQL(memMgr)) != nullptr )
			return ResultCode::SUCCESS;

		return ResultCode::OUT_OF_MEMORY;
	}

	// close DB source
	Result	FactoryMYSQL::CreateSession(IHeap& memMgr, DataSource* pDBSource, Session* &pSession )
	{
		if( (pSession = new(memMgr) SessionMYSQL(memMgr, (DataSourceMYSQL*)pDBSource)) != nullptr)
			return ResultCode::SUCCESS;

		return ResultCode::OUT_OF_MEMORY;
	}

} // namespace DB
} // namespace SF

#endif // BRDB_USE_MYSQL
