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
	HRESULT SendQueryT(CSession& session, CmdClass& cmd)
	{
		dbTrace( BR::DB::TRC_QUERYSTR, "Query %0%", cmd.GetQueryString() );
		HRESULT hr = cmd.Open(session, cmd.GetQueryString(), NULL, NULL, DBGUID_DBSQL, false);

		if( hr == DB_E_CANNOTCONNECT ) hr = E_DB_CONNECTION_FAILED;
		cmd.SetResult(hr);

		if( FAILED(hr) ) {
			BR::DB::ReportError( hr, typeid(cmd).name() );
		}

		cmd.Close();

		return hr;
	}

	// send rowset Query
	template< class CmdClass >
	HRESULT SendQueryRowsetT(CSession& session, CmdClass& cmd)
	{
		dbTrace( BR::DB::TRC_QUERYSTR, "Query %0%", cmd.GetQueryString() );
		HRESULT hr = cmd.Open(session, cmd.GetQueryString(), NULL, NULL, DBGUID_DEFAULT, true);
		if( FAILED(hr) ) {
			return E_FAIL;
		}

		while(cmd.MoveNext() == S_OK) {
			cmd.m_RowsetResult.push_back(cmd);	
		}
		
		if( hr == DB_E_CANNOTCONNECT ) hr = E_DB_CONNECTION_FAILED;
		cmd.SetResult(hr);

		if( FAILED(hr) ) {
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
	HRESULT SessionOLEDB::SendQuery( Query *pQuery )
	{
		HRESULT hr = S_OK;

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
	HRESULT SessionOLEDB::OpenSession()
	{
		__super::OpenSession();

		dbChkPtr(GetDataSource());
		DataSourceOLEDB &mySource = *(DataSourceOLEDB*)GetDataSource();

		return m_Session.Open( mySource.GetDataSource() );
	}

	// Close session
	HRESULT SessionOLEDB::CloseSession()
	{
		__super::CloseSession();

		m_Session.Close();
		return S_OK;
	}

	

} // namespace DB
} // namespace BR


#else
void SessionOLEDB_Dummy() {}
#endif // BRDB_USE_OLEDB
