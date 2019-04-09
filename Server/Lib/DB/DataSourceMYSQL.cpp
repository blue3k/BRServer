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

#ifdef BRDB_USE_MYSQL

#include "String/SFStrUtil.h"
#include "DBTrace.h"
#include "Factory.h"
#include "DataSourceMYSQL.h"


namespace SF {
namespace DB {

	
	DataSourceMYSQL::DataSourceMYSQL(IHeap& memMgr)
		: DataSource(memMgr)
	{
	}


	// initialize DB source
	Result	DataSourceMYSQL::InitializeDBSource( const String& strConnectionString, const String& strDBName, const String& strUserID, const String& strPassword )
	{
		Result hr = ResultCode::SUCCESS;
		//const char *next_token = nullptr;
		//const char *strPos = nullptr;
		size_t idx;

		dbChk(DataSource::InitializeDBSource( strConnectionString, strDBName, strUserID, strPassword ) );

		idx = strConnectionString.IndexOf( ',' );
		m_ServerIP = strConnectionString.SubString( 0, (int)idx );

		if( strConnectionString.size() <= (idx+1) )
		{
			dbErr(ResultCode::INVALID_ARG);
		}

		m_Port = atoi( strConnectionString.SubString((int)idx+1, (int)strConnectionString.size() ) );

	Proc_End:

		if(!(hr))
		{
			Factory::ErrorLog( nullptr, hr, typeid(*this).name() );
		}

		return hr;
	}

	// reopen DB source
	Result DataSourceMYSQL::Reopen()
	{
		Result hr = ResultCode::SUCCESS;

	//Proc_End:

		return hr;
	}

	// close DB source
	Result	DataSourceMYSQL::CloseDBSource()
	{
		m_ServerIP = String::Empty;
		return DataSource::CloseDBSource();
	}
	

} // namespace DB
} // namespace SF


#endif // BRDB_USE_MYSQL
