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
#include "DataSourceOLEDB.h"
#include "SessionOLEDB.h"
#include "QueryOLEDB.h"


namespace BR {
namespace DB {

	
	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Query helper functions
	//
	
	// Send query
	template< class CmdClass >
	Result SendQueryT(CSession& session, CmdClass& cmd)
	{
		dbTrace( BR::DB::TRC_QUERYSTR, "Query {0}", cmd.GetQueryString() );
		Result hr = cmd.Open(session, cmd.GetQueryString(), NULL, NULL, DBGUID_DBSQL, false);

		if( hr == DB_E_CANNOTCONNECT ) hr = ResultCode::E_DB_CONNECTION_FAILED;
		cmd.SetResult(hr);

		if( !(hr) ) {
			BR::DB::ReportError( hr, typeid(cmd).name() );
		}

		cmd.Close();

		return hr;
	}

	// send rowset Query
	template< class CmdClass >
	Result SendQueryRowsetT(CSession& session, CmdClass& cmd)
	{
		dbTrace( BR::DB::TRC_QUERYSTR, "Query {0}", cmd.GetQueryString() );
		Result hr = cmd.Open(session, cmd.GetQueryString(), NULL, NULL, DBGUID_DEFAULT, true);
		if( !(hr) ) {
			return ResultCode::FAIL;
		}

		while(cmd.MoveNext() == Result(ResultCode::SUCCESS)) {
			cmd.m_RowsetResult.push_back(cmd);	
		}
		
		if( hr == DB_E_CANNOTCONNECT ) hr = ResultCode::E_DB_CONNECTION_FAILED;
		cmd.SetResult(hr);

		if( !(hr) ) {
			ReportError( hr, typeid(cmd).name() );
		}

		cmd.Close();

		return hr;
	}



	// constructor / destructor
	SessionOLEDB::SessionOLEDB(DataSource *pDataSource)
		:Session(pDataSource)
	{
	}

	SessionOLEDB::~SessionOLEDB()
	{
	}
	
	// return context value
	void* SessionOLEDB::GetContext()
	{
		return nullptr;
	}

	// Send a query
	Result SessionOLEDB::SendQuery( Query *pQuery )
	{
		Result hr = ResultCode::SUCCESS;

		if( pQuery->GetHasRowSetResult() )
		{
			dbChk( SendQueryRowsetT(m_Session, *pQuery) );
		}
		else
		{
			dbChk( SendQueryT(m_Session, *pQuery) );
		}

	Proc_End:

		return hr;
	}

	// Open session
	Result SessionOLEDB::OpenSession()
	{
		Session::OpenSession();

		dbChkPtr(GetDataSource());
		DataSourceOLEDB &mySource = *(DataSourceOLEDB*)GetDataSource();

		return m_Session.Open( mySource.GetDataSource() );
	}

	// Close session
	Result SessionOLEDB::CloseSession()
	{
		Session::CloseSession();

		m_Session.Close();
		return ResultCode::SUCCESS;
	}

	

} // namespace DB
} // namespace BR


#else
void SessionOLEDB_Dummy() {}
#endif // BRDB_USE_OLEDB
