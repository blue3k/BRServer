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
#include "Factory.h"

#ifdef BRDB_USE_OLEDB

#include <atldbcli.h>
#include "String/SFStrUtil.h"
#include "DBTrace.h"
#include "DataSourceOLEDB.h"
#include "SessionOLEDB.h"
#include "QueryOLEDB.h"


namespace SF {
namespace DB {

	
	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Query helper functions
	//
	
	// Send query
	template< class CmdClass >
	Result SendQueryT(CSession& session, CmdClass& cmd)
	{
		dbTrace( SF::DB::TRC_QUERYSTR, "Query {0}", cmd.GetQueryString() );
		Result hr = cmd.Open(session, cmd.GetQueryString(), NULL, NULL, DBGUID_DBSQL, false);

		if( hr == DB_E_CANNOTCONNECT ) hr = ResultCode::DB_CONNECTION_FAILED;
		cmd.SetResult(hr);

		if( !(hr) ) {
			SF::DB::ReportError( hr, typeid(cmd).name() );
		}

		cmd.Close();

		return hr;
	}

	// send rowset Query
	template< class CmdClass >
	Result SendQueryRowsetT(CSession& session, CmdClass& cmd)
	{
		dbTrace( SF::DB::TRC_QUERYSTR, "Query {0}", cmd.GetQueryString() );
		Result hr = cmd.Open(session, cmd.GetQueryString(), NULL, NULL, DBGUID_DEFAULT, true);
		if( !(hr) ) {
			return ResultCode::FAIL;
		}

		while(cmd.MoveNext() == Result(ResultCode::SUCCESS)) {
			cmd.RowsetResults.push_back(cmd);
		}
		
		if( hr == DB_E_CANNOTCONNECT ) hr = ResultCode::DB_CONNECTION_FAILED;
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
	Result SessionOLEDB::ProcessQuery( Query *pQuery )
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
} // namespace SF


#else
void SessionOLEDB_Dummy() {}
#endif // BRDB_USE_OLEDB
