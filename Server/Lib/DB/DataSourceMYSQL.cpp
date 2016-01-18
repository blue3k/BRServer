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

#ifdef BRDB_USE_MYSQL

#include "Common/StrUtil.h"
#include "DBTrace.h"
#include "Factory.h"
#include "DataSourceMYSQL.h"


namespace BR {
namespace DB {

	
	DataSourceMYSQL::DataSourceMYSQL()
	{
	}


	// initialize DB source
	HRESULT	DataSourceMYSQL::InitializeDBSource( const std::string& strConnectionString, const std::string& strDBName, const std::string& strUserID, const std::string& strPassword )
	{
		HRESULT hr = S_OK;
		//const char *next_token = nullptr;
		//const char *strPos = nullptr;
		size_t idx;

		dbChk(DataSource::InitializeDBSource( strConnectionString, strDBName, strUserID, strPassword ) );

		idx = strConnectionString.find( ',', 0 );
		m_ServerIP = strConnectionString.substr( 0, idx );

		if( strConnectionString.size() <= (idx+1) )
		{
			dbErr(E_SYSTEM_INVALIDARG);
		}

		m_Port = atoi( strConnectionString.substr( idx+1, strConnectionString.size() ).c_str() );

	Proc_End:

		if(FAILED(hr))
		{
			Factory::ErrorLog( nullptr, hr, typeid(*this).name() );
		}

		return hr;
	}

	// reopen DB source
	HRESULT DataSourceMYSQL::Reopen()
	{
		HRESULT hr = S_OK;

	//Proc_End:

		return hr;
	}

	// close DB source
	HRESULT	DataSourceMYSQL::CloseDBSource()
	{
		return DataSource::CloseDBSource();
	}
	

} // namespace DB
} // namespace BR


#endif // BRDB_USE_MYSQL
