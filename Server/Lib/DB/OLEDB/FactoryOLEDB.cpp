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

#ifdef BRDB_USE_OLEDB

#include <atldbcli.h>
#include "Util/SFStrUtil.h"
#include "DBTrace.h"
#include "FactoryOLEDB.h"
#include "DataSourceOLEDB.h"
#include "SessionOLEDB.h"


namespace SF {
namespace DB {

	
	// Make this factory as the DB factory
	Result FactoryOLEDB::Instanciate()
	{
		Result hr = ResultCode::SUCCESS;

		dbMem( Factory::stm_pInstance = new FactoryOLEDB );

	Proc_End:

		return hr;
	}

	void FactoryOLEDB::ReportError( void* DBContext, Result hr, const char* className )
	{
		CDBErrorInfo myErrorInfo;
		uint64_t numRec = 0;
		BSTR myErrStr,mySource;
		ISQLErrorInfo *pISQLErrorInfo = NULL;

		LCID lcLocale = GetSystemDefaultLCID();
		myErrorInfo.GetErrorRecords(&numRec);

		if (numRec) 
		{
			myErrorInfo.GetAllErrorInfo( 0,lcLocale,&myErrStr, &mySource );
			dbTrace( Error, "DB Failed Error hr:{0:X8}, {1} : {2} ", hr, className, (LPCWSTR)myErrStr );
		}
	}

	// initialize DB source
	Result	FactoryOLEDB::CreateDataSource(IHeap& memMgr, DataSource* &pDBSource )
	{
		if( pDBSource = new(GetHeap()) DataSourceOLEDB(memMgr))
			return ResultCode::SUCCESS;

		return ResultCode::OUT_OF_MEMORY;
	}

	// close DB source
	Result	FactoryOLEDB::CreateSession(IHeap& memMgr, DataSource* pDBSource, Session* &pSession )
	{
		if( pSession = new(GetHeap()) SessionOLEDB(memMgr, pDBSource) )
			return ResultCode::SUCCESS;

		return ResultCode::OUT_OF_MEMORY;
	}

} // namespace DB
} // namespace SF

#else
void FactoryOLEDB_Dummy() {}
#endif // BRDB_USE_OLEDB
