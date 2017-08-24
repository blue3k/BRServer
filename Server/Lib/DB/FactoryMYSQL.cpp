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
#include "Common/Typedefs.h"
#include "Factory.h"

#ifdef BRDB_USE_MYSQL

#include "Common/Typedefs.h"
#include "Common/StrUtil.h"
#include "DBTrace.h"
#include "FactoryMYSQL.h"
#include "DataSourceMYSQL.h"
#include "SessionMYSQL.h"

#if WINDOWS
#pragma comment(lib, "libmysql.lib")
#else
#endif

namespace BR {
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
		if (hr == Result(ResultCode::E_INVALID_ENTITY))
			return;

		if( pSql )
		{
			const char* errorString = mysql_error(pSql);
			AssertRel(errorString!=nullptr);
			dbTrace( Trace::TRC_ERROR, "DB Failed Error hr:{0:X8}, {1} : {2} ", hr, className, errorString );
		}
		else
		{
			dbTrace( Trace::TRC_ERROR, "DB Failed Error hr:{0:X8}, {1}", hr, className );
		}
	}

	// initialize DB source
	Result	FactoryMYSQL::CreateDataSource( DataSource* &pDBSource )
	{
		if( (pDBSource = new DataSourceMYSQL) != nullptr )
			return ResultCode::SUCCESS;

		return ResultCode::OUT_OF_MEMORY;
	}

	// close DB source
	Result	FactoryMYSQL::CreateSession( DataSource* pDBSource, Session* &pSession )
	{
		if( (pSession = new SessionMYSQL((DataSourceMYSQL*)pDBSource)) != nullptr)
			return ResultCode::SUCCESS;

		return ResultCode::OUT_OF_MEMORY;
	}

} // namespace DB
} // namespace BR

#endif // BRDB_USE_MYSQL
