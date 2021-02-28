////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves 
// 
// Author: Kyungkun Ko
//
// Description : Procees Requested DB Query
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "DBPch.h"


#include "DBPch.h"
#include "SFTypedefs.h"

#ifdef BRDB_USE_MYSQL


#include "SFTypedefs.h"
#include "Util/SFStrUtil.h"
#include "Util/SFStringFormat.h"
#include "DBTrace.h"
#include "DataSourceMYSQL.h"
#include "SessionMYSQL.h"
#include "Query.h"
#include "QueryMYSQL.h"
#include "ResultCode/SFResultCodeSvr.h"
#include "ResultCode/SFResultCodeDB.h"
#include "Service/ServerService.h"


namespace SF {
namespace DB {

	
	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Error handling
	//

	
	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Session class
	//

	// constructor / destructor
	SessionMYSQL::SessionMYSQL(IHeap& memMgr, DataSourceMYSQL *pDataSource)
		:Session(pDataSource)
		, m_pMyDataSource(pDataSource)
		, m_Synced(0)
	{
	}

	SessionMYSQL::~SessionMYSQL()
	{
	}

	// return context value
	void* SessionMYSQL::GetContext()
	{
		return m_pMyDataSource;
	}

	// Send a query
	Result SessionMYSQL::ProcessQuery( Query *pQuery )
	{
		QueryMYSQL* pMyQuery = static_cast<QueryMYSQL*>(pQuery);

		ScopeContext hr([this, &pMyQuery](Result hr)
			{

				if (!hr)
				{
					if (pMyQuery)
					{
						defTrace(Error, "Query failed hr:0x{0:X8} : {1}", hr, pMyQuery->GetQueryString().c_str());
					}
					else
					{
						defTrace(Error, "Query failed hr:0x{0:X8}", hr);
					}
				}

				if (pMyQuery)
					pMyQuery->SetResult(hr);

			});

		dbCheckPtr(pMyQuery);
		dbCheckPtr(m_pMyDataSource);

		auto* pClient = m_pMyDataSource->GetSQLClient();
		dbCheckPtr(pClient);

		if( m_Synced != m_InitSync )
		{
			m_Synced = m_InitSync;

			//mysql_options(m_mySQL, MYSQL_SET_CHARSET_NAME, "utf8");
			//mysql_options(m_mySQL, MYSQL_INIT_COMMAND, "SET NAMES utf8");
			// NOTE: need to clear statement pool
		}

		try
		{
			auto queryString = pMyQuery->GetQueryString().c_str();
			defTrace(Debug3, "Sending DB query ... {0}", pMyQuery->GetQueryString());

			auto XSession = pClient->getSession();
			mysqlx::SqlStatement statement = XSession.sql(pMyQuery->GetQueryString());

			pMyQuery->BindParameters(statement);

			mysqlx::SqlResult queryResult = std::forward<mysqlx::SqlResult>(statement.execute());

			pMyQuery->ParseResult(queryResult);

			if (pMyQuery->GetQueryOutputString().size() > 0)
			{
				auto queryOutputString = pMyQuery->GetQueryOutputString().c_str();
				defTrace(Debug3, "DB query result string ... {0}", pMyQuery->GetQueryOutputString());

				mysqlx::SqlResult outputResult = std::forward<mysqlx::SqlResult>(XSession.sql(pMyQuery->GetQueryOutputString()).execute());

				pMyQuery->ParseOutput(outputResult);
			}
		}
		catch (std::exception& e)
		{
			// TODO: handle errors
			defTrace(Error, "DB query has failed ... {0}", e.what());
			hr = ResultCode::SVR_SERVICE_FAILED;
		}


		return hr;
	}

	// Open session
	Result SessionMYSQL::OpenSession()
	{
		Result hr = ResultCode::SUCCESS;

		dbCheckPtr(GetDataSource());
		dbCheckPtr(m_pMyDataSource = static_cast<DataSourceMYSQL*>(GetDataSource()));

		auto* pClient = m_pMyDataSource->GetSQLClient();
		dbCheckPtr(pClient);

		try
		{
			CloseSession();

			Session::OpenSession();

			// TODO:
			//mysql_options(m_mySQL, MYSQL_SET_CHARSET_NAME, "utf8");
			//mysql_options(m_mySQL, MYSQL_INIT_COMMAND, "SET NAMES utf8");

			m_InitSync.fetch_add(1, std::memory_order_relaxed);
		}
		catch (std::exception& e)
		{
			dbTrace(Error, "DB Failed Error :{0}", e.what());
			hr = ResultCode::UNEXPECTED;
		}


	Proc_End:

		if( !(hr) )
		{
			dbTrace(Error, "SessionMYSQL: Failed to open a session", hr);
			CloseSession();
		}

		return hr;
	}

	// Close session
	Result SessionMYSQL::CloseSession()
	{
		Session::CloseSession();
		
		return ResultCode::SUCCESS;
	}

	Result SessionMYSQL::Ping()
	{
		Result hr = ResultCode::SUCCESS;

		return hr;
	}


	
} // namespace DB
} // namespace SF

#endif // BRDB_USE_MYSQL
