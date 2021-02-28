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
#include "DB/DBConfig.h"

#ifdef BRDB_USE_MYSQL

#include "Util/SFStrUtil.h"
#include "DBTrace.h"
#include "Service/ServerService.h"
#include "DataSourceMYSQL.h"


namespace SF {
namespace DB {

	
	DataSourceMYSQL::DataSourceMYSQL(IHeap& memMgr)
		: DataSource(memMgr)
	{
	}

	DataSourceMYSQL::~DataSourceMYSQL()
	{
		if (m_Client)
		{
			m_Client->close();
			IHeap::Delete(m_Client);
			m_Client = nullptr;
		}
	}


	// initialize DB source
	Result	DataSourceMYSQL::InitializeDBSource( const String& strConnectionString, const String& strDBName, const String& strUserID, const String& strPassword )
	{
		ScopeContext hr([](Result hr) 
			{
				if (!hr)
				{
					Service::Database->ReportError(nullptr, hr, "DataSourceMYSQL");
				}
			});
		size_t idx;

		dbCheck(DataSource::InitializeDBSource( strConnectionString, strDBName, strUserID, strPassword ) );

		idx = strConnectionString.IndexOf( ',' );
		m_ServerIP = strConnectionString.SubString( 0, (int)idx );

		if( strConnectionString.size() <= (idx+1) )
		{
			dbCheck(ResultCode::INVALID_ARG);
		}

		m_Port = atoi( strConnectionString.SubString((int)idx+1, (int)strConnectionString.size() ) );

		m_Client = new(GetHeap()) mysqlx::Client(
			mysqlx::SessionOption::HOST, mysqlx::string(m_ServerIP.data()),
			mysqlx::SessionOption::PORT, m_Port,
			mysqlx::SessionOption::USER, mysqlx::string(strUserID.data()),
			mysqlx::SessionOption::PWD, mysqlx::string(strPassword.data()),
			mysqlx::SessionOption::DB, mysqlx::string(strDBName.data()),
			mysqlx::ClientOption::POOLING, 1,
			mysqlx::ClientOption::POOL_MAX_SIZE, 20); // TODO: move to config

		return hr;
	}

	// reopen DB source
	Result DataSourceMYSQL::Reopen()
	{
		Result hr = ResultCode::SUCCESS;

		return hr;
	}

	// close DB source
	Result	DataSourceMYSQL::CloseDBSource()
	{
		m_ServerIP = String_Empty;
		if (m_Client)
		{
			m_Client->close();
			IHeap::Delete(m_Client);
			m_Client = nullptr;
		}
		return DataSource::CloseDBSource();
	}
	

} // namespace DB
} // namespace SF


#endif // BRDB_USE_MYSQL
