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
#include "Factory.h"

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


namespace SF {
namespace DB {

	
	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Error handling
	//

	Result MYSQL_ToResult( int errorValue )
	{
		//switch( errorValue )
		//{
		//case 0:
		//	return ResultCode::SUCCESS;
		//case CR_SERVER_GONE_ERROR:
		//case CR_SERVER_LOST:
		//case CR_COMMANDS_OUT_OF_SYNC:
		//	return ResultCode::DB_CONNECTION_LOST;
		//case CR_OUT_OF_MEMORY:
		//	return ResultCode::OUT_OF_MEMORY;
		//};

		return ResultCode::UNEXPECTED;
	}

	
	
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
		if (m_pXSession)
		{
			m_pXSession->close();
			m_pXSession.reset();
		}
	}
	
	// return context value
	void* SessionMYSQL::GetContext()
	{
		return m_pXSession.get();
	}

	// Send a query
	Result SessionMYSQL::ProcessQuery( Query *pQuery )
	{
		Result hr = ResultCode::SUCCESS;
		QueryMYSQL* pMyQuery = static_cast<QueryMYSQL*>(pQuery);

		dbChkPtr(pMyQuery);
		dbChkPtr(m_pXSession);

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

			mysqlx::SqlStatement statement = m_pXSession->sql(pMyQuery->GetQueryString());

			pMyQuery->BindParameters(statement);

			mysqlx::SqlResult queryResult = std::forward<mysqlx::SqlResult>(statement.execute());

			pMyQuery->ParseResult(queryResult);

			if (pMyQuery->GetQueryOutputString().size() > 0)
			{
				auto queryOutputString = pMyQuery->GetQueryOutputString().c_str();
				defTrace(Debug3, "DB query result string ... {0}", pMyQuery->GetQueryOutputString());

				mysqlx::SqlResult outputResult = std::forward<mysqlx::SqlResult>(m_pXSession->sql(pMyQuery->GetQueryOutputString()).execute());

				pMyQuery->ParseOutput(outputResult);
			}
		}
		catch (std::exception& e)
		{
			// TODO: handle errors
			defTrace(Error, "DB query has failed ... {0}", e.what());
			hr = ResultCode::SVR_SERVICE_FAILED;
		}


	Proc_End:

		if( !(hr) )
		{
			if( pMyQuery )
			{
				defTrace( Error, "Query failed hr:0x{0:X8} : {1}", hr, pMyQuery->GetQueryString().c_str() );
			}
			else
			{
				defTrace( Error, "Query failed hr:0x{0:X8}", hr );
			}

			if( hr == ((Result)ResultCode::DB_CONNECTION_LOST))
			{
				defTrace( Warning, "DB connection is lost, recovering the connection... " );
				Result hrTem = OpenSession();
				if( !(hrTem) )
				{
					defTrace( Error, "DB connection recovery is failed ... {0:X8}", hrTem );
				}
			}
		}


		if( pMyQuery )
			pMyQuery->SetResult(hr);

		return hr;
	}

	// Open session
	Result SessionMYSQL::OpenSession()
	{
		Result hr = ResultCode::SUCCESS;

		dbChkPtr(GetDataSource());
		dbChkPtr( m_pMyDataSource = (DataSourceMYSQL*)GetDataSource() );

		try
		{
			CloseSession();

			Session::OpenSession();

			auto serverIP = mysqlx::string(m_pMyDataSource->GetServerIP().data());
			auto userId = mysqlx::string(m_pMyDataSource->GetUserID().data());
			auto password = mysqlx::string(m_pMyDataSource->GetPassword().data());

			m_pXSession.reset(new(GetEngineHeap()) mysqlx::Session(
				serverIP,
				m_pMyDataSource->GetServerPort(), 
				userId,
				password));

			String useDB;
			useDB.Format("USE {0};", m_pMyDataSource->GetDefaultDB());
			m_pXSession->sql(useDB.data()).execute();

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
			Factory::ErrorLog(nullptr,hr,typeid(*this).name());
			CloseSession();
		}

		return hr;
	}

	// Close session
	Result SessionMYSQL::CloseSession()
	{
		Session::CloseSession();

		if (m_pXSession != nullptr)
		{
			m_pXSession->close();
			m_pXSession.reset();
		}
		
		return ResultCode::SUCCESS;
	}

	Result SessionMYSQL::Ping()
	{
		Result hr = ResultCode::SUCCESS;

		if (m_pXSession == nullptr)
		{
			dbChk(OpenSession());
		}

	Proc_End:

		return hr;
	}


	
} // namespace DB
} // namespace SF

#endif // BRDB_USE_MYSQL
