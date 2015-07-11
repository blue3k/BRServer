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


#include "StdAfx.h"
#include "Common/Typedefs.h"
#include "Factory.h"

#ifdef BRDB_USE_MYSQL


#include "Common/Typedefs.h"
#include "Common/StrUtil.h"
#include "DBTrace.h"
#include "DataSourceMYSQL.h"
#include "SessionMYSQL.h"
#include "Query.h"
#include "QueryMYSQL.h"
#include "Common/HRESSvrSys.h"

#include "errmsg.h"


namespace BR {
namespace DB {

	
	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Error handling
	//

	HRESULT MYSQL_ToHRESULT( int errorValue )
	{
		switch( errorValue )
		{
		case 0:
			return S_OK;
		case CR_SERVER_GONE_ERROR:
		case CR_SERVER_LOST:
		case CR_COMMANDS_OUT_OF_SYNC:
			return E_DB_CONNECTION_LOST;
		case CR_OUT_OF_MEMORY:
			return E_OUTOFMEMORY;
		};

		return E_UNEXPECTED;
	}

	
	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Statement pool class
	//

	StatementPoolMYSQL::StatementPoolMYSQL()
	{
	}

	StatementPoolMYSQL::~StatementPoolMYSQL()
	{
		ClearStatementPools();
	}

	// Cleanup the pool
	HRESULT StatementPoolMYSQL::ClearStatementPools()
	{
		for( auto iter = m_StatementPoolMap.begin(); iter != m_StatementPoolMap.end(); ++iter )
		{
			StackPool* pPool = iter->second;
			pPool->for_each( []( StackPool::Item *pItem )
			{
				PoolItem *pStmtItem = (PoolItem*)pItem;
				delete pStmtItem;
			});
			pPool->Clear();
			delete pPool;
		}

		m_StatementPoolMap.clear();

		return S_OK;
	}

	// Get free statement
	StatementMYSQL* StatementPoolMYSQL::GetFreeStatement( const char* queryString )
	{
		auto itFound = m_StatementPoolMap.find((intptr_t)queryString);
		if( itFound == m_StatementPoolMap.end() )
		{
			return new StatementMYSQL(queryString);
		}

		StackPool* pPool = itFound->second;
		StackPool::Item *pFreeItem = pPool->Pop();
		if( pFreeItem )
		{
			return (StatementMYSQL*)pFreeItem;
		}

		return new StatementMYSQL(queryString);
	}

	// free statement
	HRESULT StatementPoolMYSQL::FreeStatement( StatementMYSQL* &pStatement )
	{
		HRESULT hr = S_OK;
		StatementPoolMap::iterator itFound;

		dbChkPtr(pStatement);
		dbChkPtr(pStatement->GetQueryString());

		intptr_t key = (intptr_t)pStatement->GetQueryString();

		StackPool* pPool = nullptr;

		itFound = m_StatementPoolMap.find(key);
		if( itFound == m_StatementPoolMap.end() )
		{
			pPool = new StackPool;

			if( m_StatementPoolMap.insert( std::make_pair(key,pPool) ).second == false )
				dbErr(E_UNEXPECTED);
		}
		else
		{
			pPool = itFound->second;
		}

		pPool->Push( pStatement );
		pStatement = nullptr;

	Proc_End:

		// just delete when we meet a unexpected status
		Util::SafeDelete( pStatement );

		return hr;
	}

	
	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Session class
	//

	// constructor / destructor
	SessionMYSQL::SessionMYSQL(DataSourceMYSQL *pDataSource)
		:Session(pDataSource)
		,m_pMyDataSource(pDataSource)
		,m_mySQL(nullptr)
		,m_Synced(0)
	{
	}

	SessionMYSQL::~SessionMYSQL()
	{
	}
	
	// return context value
	void* SessionMYSQL::GetContext()
	{
		return m_mySQL;
	}

	// Send a query
	HRESULT SessionMYSQL::SendQuery( Query *pQuery )
	{
		HRESULT hr = S_OK;
		QueryMYSQL* pMyQuery = BR_DYNAMIC_CAST(QueryMYSQL*,pQuery);
		StatementMYSQL *pStatement = nullptr;

		dbChkPtr(pMyQuery);
		dbChkPtr(m_mySQL);

		if( m_Synced != m_InitSync )
		{
			mysql_thread_init();

			m_Synced = m_InitSync;

			//mysql_options(m_mySQL, MYSQL_SET_CHARSET_NAME, "utf8");
			//mysql_options(m_mySQL, MYSQL_INIT_COMMAND, "SET NAMES utf8");
			// NOTE: need to clear statement pool
		}


		//dbMem( pStatement = new StatementMYSQL(pMyQuery->GetQueryString()) );
		dbMem( pStatement = m_StatementPool.GetFreeStatement(pMyQuery->GetQueryString()) );

		if( !pStatement->IsPrepared() )
		{
			dbChk( pStatement->PrepareState( m_mySQL, m_Synced ) );
		}

		dbChk( pStatement->Bind( pMyQuery ) );

		hr = pStatement->Execute();
		if (hr == E_DB_CONNECTION_LOST) goto Proc_End;
		dbChk(hr);

		dbChk( pStatement->PatchResults( pMyQuery ) );

		dbChk( m_StatementPool.FreeStatement( pStatement ) );

	Proc_End:

		if( FAILED(hr) )
		{
			if( pMyQuery )
			{
				defTrace( Trace::TRC_ERROR, "Query failed hr:0x%0% : %1% : %2%", BR::ArgHex32(hr), typeid(*pMyQuery).name(), pMyQuery->GetQueryString() );
			}
			else
			{
				defTrace( Trace::TRC_ERROR, "Query failed hr:0x%0%", BR::ArgHex32(hr) );
			}

			if( hr == E_DB_CONNECTION_LOST )
			{
				defTrace( Trace::TRC_WARN, "DB connection is lost, recovering the connection... " );
				HRESULT hrTem = OpenSession();
				if( FAILED(hrTem) )
				{
					defTrace( Trace::TRC_ERROR, "DB connection recovery is failed ... %0%", ArgHex32(hrTem) );
				}
			}
		}


		Util::SafeDelete(pStatement);

		if( pMyQuery )
			pMyQuery->SetResult(hr);

		return hr;
	}

	// Open session
	HRESULT SessionMYSQL::OpenSession()
	{
		HRESULT hr = S_OK;

		dbChkPtr(GetDataSource());
		dbChkPtr( m_pMyDataSource = (DataSourceMYSQL*)GetDataSource() );

		CloseSession();

		__super::OpenSession();

		dbMem( m_mySQL = mysql_init(NULL) );

		mysql_options(m_mySQL, MYSQL_SET_CHARSET_NAME, "utf8");
		mysql_options(m_mySQL, MYSQL_INIT_COMMAND, "SET NAMES utf8");

		if (mysql_real_connect(m_mySQL,
			m_pMyDataSource->GetServerIP().c_str(),
			m_pMyDataSource->GetUserID().c_str(), m_pMyDataSource->GetPassword().c_str(),
			m_pMyDataSource->GetDefaultDB().c_str(), 
			m_pMyDataSource->GetServerPort(), 
			NULL, CLIENT_MULTI_STATEMENTS) == nullptr) 
		{
			dbErr(E_DB_CONNECTION_FAILED);
		}


		m_InitSync.fetch_add(1, std::memory_order_relaxed);

	Proc_End:

		if( FAILED(hr) )
		{
			Factory::ErrorLog(m_mySQL,hr,typeid(*this).name());
			CloseSession();
		}

		return hr;
	}

	// Close session
	HRESULT SessionMYSQL::CloseSession()
	{
		__super::CloseSession();

		m_StatementPool.ClearStatementPools();

		if( m_mySQL )
		{
			mysql_thread_end();
			mysql_close(m_mySQL);
			m_mySQL = nullptr;
		}
		return S_OK;
	}



	
	//////////////////////////////////////////////////////////////////////////////////
	//
	//	DB statement class
	//


	StatementMYSQL::StatementMYSQL( const char* queryString )
		:m_QueryString(queryString)
		,m_StateInitSync(0)
		,m_Stmt(nullptr)
		,m_Context(nullptr)
		,m_pParameter(nullptr)
	{
		AssertRel(m_QueryString!=nullptr);
	}

	
	// prepared
	bool StatementMYSQL::IsPrepared()
	{
		return m_Stmt != nullptr;
	}

	// Prepare statement
	HRESULT StatementMYSQL::PrepareState( MYSQL *pContext, CounterType syncInit )
	{
		HRESULT hr = S_OK;

		m_Context = pContext;
		m_StateInitSync = syncInit;

		dbChkPtr( m_Stmt = mysql_stmt_init(m_Context) );
		int rc = mysql_stmt_prepare(m_Stmt, m_QueryString, (uint) strlen(m_QueryString));
		if ( rc )
		{
			dbErr(MYSQL_ToHRESULT(mysql_stmt_errno(m_Stmt)) );
			//dbErr(E_DB_STATEMENT_PREPARE_FAILED);
		}

	Proc_End:

		if( FAILED(hr) )
		{
			Factory::ErrorLog( pContext, hr, m_Stmt ? mysql_stmt_error(m_Stmt) : typeid(*this).name() );
			Clear();
		}

		return hr;
	}


	// Bind query instance
	HRESULT StatementMYSQL::Bind( QueryMYSQL *pMyQuery )
	{
		HRESULT hr = S_OK;

		m_pParameter = nullptr;

		dbChkPtr( m_Stmt );

		int paramCount = mysql_stmt_param_count(m_Stmt);

		Assert( paramCount == pMyQuery->GetParameterCount() );


		if( pMyQuery->GetParameterCount() > 0 )
		{
			dbChkPtr( m_pParameter = pMyQuery->BuildParameter() );
			int rc = mysql_stmt_bind_param(m_Stmt, m_pParameter);
			if( rc != 0 ) dbErr(MYSQL_ToHRESULT(mysql_stmt_errno(m_Stmt)) );//dbErr(E_DB_PARAMETER_BIND_FAILED);
		}

	Proc_End:
		
		if( FAILED(hr) )
		{
			Factory::ErrorLog( m_Context, hr, m_Stmt ? mysql_stmt_error(m_Stmt) : typeid(*this).name() );
		}

		return hr;
	}

	// Execute
	HRESULT StatementMYSQL::Execute()
	{
		HRESULT hr = S_OK;

		dbChkPtr( m_Stmt );

		int rc = mysql_stmt_execute(m_Stmt);
		if( rc != 0 )
		{
			hr = MYSQL_ToHRESULT(mysql_stmt_errno(m_Stmt));
		}

	Proc_End:
		
		if( FAILED(hr) )
		{
			Factory::ErrorLog( m_Context, hr, m_Stmt ? mysql_stmt_error(m_Stmt) : typeid(*this).name() );
		}

		return hr;
	}

	// Patch result
	HRESULT StatementMYSQL::PatchResults( QueryMYSQL *pMyQuery )
	{
		HRESULT hr = S_OK;
		MYSQL_BIND *pResults = nullptr;
		int resultStatus = 0, rc;

		dbChkPtr( m_Stmt );
		dbChkPtr( m_Context );
		
		while(resultStatus == 0)
		{
			int num_fields = mysql_stmt_field_count(m_Stmt);

			if (num_fields > 0)
			{
				const int ResultMax = 32;
				MYSQL_BIND *pResults = nullptr;
				bool bOutParamBind = (m_Context->server_status & SERVER_PS_OUT_PARAMS) != 0;
				if( bOutParamBind )
				{
					pResults = m_pParameter + pMyQuery->GetInputParameterCount();
					if( (pMyQuery->GetParameterCount() - pMyQuery->GetInputParameterCount()) != num_fields )
					{
						dbTrace( Trace::TRC_ERROR, "Database output count is mismatched. Query: %0%, %2% is specified, %1% is expected", pMyQuery->GetQueryString(), num_fields, (pMyQuery->GetParameterCount() - pMyQuery->GetInputParameterCount()));
						dbErr(E_DB_RESULT_COUNT_MISMATCH);
					}
				}
				else
				{
					pResults = pMyQuery->BuildResult();
					if( pMyQuery->GetResultCount() != num_fields )
					{
						dbTrace( Trace::TRC_ERROR, "Database result column count is mismatched. Query: %0%, %2% is specified, %1% is expected", pMyQuery->GetQueryString(), num_fields, pMyQuery->GetResultCount() );
						dbErr(E_DB_RESULT_COUNT_MISMATCH);
					}
				}

				MYSQL_RES *rs_metadata= mysql_stmt_result_metadata(m_Stmt);
				dbChkPtr(rs_metadata);
				MYSQL_FIELD *fields = mysql_fetch_fields(rs_metadata);
				for (int i = 0; i < num_fields; ++i)
				{
					if( !MYSQL_IsCompatibleType( pResults[i].buffer_type, fields[i].type ) )
					{
						dbTrace( Trace::TRC_ERROR, "Database result column type is mismatched. Query: %0%", pMyQuery->GetQueryString() );
						dbTrace( Trace::TRC_ERROR, "DB column info: %4%.%0%.%1%, %4%.%2%.%3%, %5%: binded Type:%d, Required Type:%d",
							(const char *) fields[i].table,
							(const char *) fields[i].name,
							(const char *) fields[i].org_table,
							(const char *) fields[i].org_name,
							(const char *) fields[i].db,
							(const char *) fields[i].catalog,
							pResults[i].buffer_type,
							fields[i].type
							);
						
						dbErr(E_DB_RESULT_FIELDTYPE_MISMATCH);

					}
					//pResults[i].buffer_type = fields[i].type;
				}

				rc = mysql_stmt_bind_result(m_Stmt, pResults);
				if( rc != 0 ) dbErr(E_DB_RESULT_PATCH_FAILED);

				do {
					pMyQuery->PrepareResultColumn();
					rc = mysql_stmt_fetch(m_Stmt);
					if( rc == 0 && !bOutParamBind )
						pMyQuery->OnNewResultColumn();
				}while( rc == 0 );
			}
			else
			{
				rc = mysql_stmt_store_result(m_Stmt);
				if( rc != 0 ) dbErr(E_DB_RESULT_PATCH_FAILED);
			}

			rc = mysql_stmt_free_result(m_Stmt);
			if( rc != 0 ) dbErr(E_DB_RESULT_PATCH_FAILED);

			resultStatus = mysql_stmt_next_result(m_Stmt);
		}


	Proc_End:
		
		if( FAILED(hr) )
		{
			Factory::ErrorLog( m_Context, hr, m_Stmt ? mysql_stmt_error(m_Stmt) : typeid(*this).name() );
		}

		// Clean up results if exist
		if( m_Stmt )
		{
			while(resultStatus == 0)
			{
				mysql_stmt_store_result(m_Stmt);
				mysql_stmt_free_result(m_Stmt);

				resultStatus = mysql_stmt_next_result(m_Stmt);
			}
		}

		return hr;
	}

	// Clear prepared statement
	void StatementMYSQL::Clear()
	{
		if( m_Stmt )
		{
			mysql_stmt_close(m_Stmt);
			m_Stmt = nullptr;
		}
		m_Context = nullptr;
	}


} // namespace DB
} // namespace BR

#endif // BRDB_USE_MYSQL
