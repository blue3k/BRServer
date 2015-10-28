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


#include "StdAfx.h"
#include "Common/Typedefs.h"
#include "Factory.h"

#ifdef BRDB_USE_MYSQL

#include "Common/Typedefs.h"
#include "Common/StrUtil.h"
#include "DBTrace.h"
#include "FactoryMYSQL.h"
#include "DataSourceMYSQL.h"
#include "SessionMYSQL.h"


#pragma comment(lib, "libmysql.lib")

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
	HRESULT FactoryMYSQL::Instanciate()
	{
		HRESULT hr = S_OK;

		dbMem( Factory::stm_pInstance = new FactoryMYSQL );

	Proc_End:

		return hr;
	}

	void FactoryMYSQL::ReportError( void* DBContext, HRESULT hr, const char* className )
	{
		MYSQL *pSql = DBContext ? (MYSQL*)DBContext : nullptr;

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
	HRESULT	FactoryMYSQL::CreateDataSource( DataSource* &pDBSource )
	{
		if( pDBSource = new DataSourceMYSQL )
			return S_OK;

		return E_OUTOFMEMORY;
	}

	// close DB source
	HRESULT	FactoryMYSQL::CreateSession( DataSource* pDBSource, Session* &pSession )
	{
		if( pSession = new SessionMYSQL((DataSourceMYSQL*)pDBSource) )
			return S_OK;

		return E_OUTOFMEMORY;
	}

} // namespace DB
} // namespace BR

#endif // BRDB_USE_MYSQL
