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

#ifdef BRDB_USE_OLEDB

#include <atldbcli.h>
#include "Common/StrUtil.h"
#include "DBTrace.h"
#include "FactoryOLEDB.h"
#include "DataSourceOLEDB.h"
#include "SessionOLEDB.h"


namespace BR {
namespace DB {

	
	// Make this factory as the DB factory
	HRESULT FactoryOLEDB::Instanciate()
	{
		HRESULT hr = S_OK;

		dbMem( Factory::stm_pInstance = new FactoryOLEDB );

	Proc_End:

		return hr;
	}

	void FactoryOLEDB::ReportError( void* DBContext, HRESULT hr, const char* className )
	{
		CDBErrorInfo myErrorInfo;
		ULONG numRec = 0;
		BSTR myErrStr,mySource;
		ISQLErrorInfo *pISQLErrorInfo = NULL;

		LCID lcLocale = GetSystemDefaultLCID();
		myErrorInfo.GetErrorRecords(&numRec);

		if (numRec) 
		{
			myErrorInfo.GetAllErrorInfo( 0,lcLocale,&myErrStr, &mySource );
			dbTrace( Trace::TRC_ERROR, "DB Failed Error hr:{0:X8}, %1% : %2% ", hr, className, (LPCWSTR)myErrStr );
		}
	}

	// initialize DB source
	HRESULT	FactoryOLEDB::CreateDataSource( DataSource* &pDBSource )
	{
		if( pDBSource = new DataSourceOLEDB )
			return S_OK;

		return E_OUTOFMEMORY;
	}

	// close DB source
	HRESULT	FactoryOLEDB::CreateSession( DataSource* pDBSource, Session* &pSession )
	{
		if( pSession = new SessionOLEDB(pDBSource) )
			return S_OK;

		return E_OUTOFMEMORY;
	}

} // namespace DB
} // namespace BR

#else
void FactoryOLEDB_Dummy() {}
#endif // BRDB_USE_OLEDB
