////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : MadK
//
// Description : Data source for MYSQL
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Factory.h"

#ifdef BRDB_USE_OLEDB

#include <atldbcli.h>
#include "String/StrUtil.h"
#include "DBTrace.h"
#include "DataSourceOLEDB.h"
#include "FactoryOLEDB.h"


namespace BR {
namespace DB {

	
	DataSourceOLEDB::DataSourceOLEDB()
		:m_PropSet(DBPROPSET_DBINIT)
	{
	}


	// initialize DB source
	Result	DataSourceOLEDB::InitializeDBSource( const std::string& strConnectionString, const std::string& strDBName, const std::string& strUserID, const std::string& strPassword )
	{
		Result hr = ResultCode::SUCCESS;

		dbChk(DataSource::InitializeDBSource( strConnectionString, strDBName, strUserID, strPassword ) );

		m_PropSet = CDBPropSet(DBPROPSET_DBINIT);
		m_PropSet.AddProperty(DBPROP_INIT_DATASOURCE, A2BSTR(m_strConnectionString.c_str()));
		m_PropSet.AddProperty(DBPROP_INIT_CATALOG, A2BSTR(m_strDefaultDB.c_str()));
		m_PropSet.AddProperty(DBPROP_AUTH_USERID, A2BSTR(m_strUserID.c_str()));
		m_PropSet.AddProperty(DBPROP_AUTH_PASSWORD, A2BSTR(m_strPassword.c_str()));

		dbChk( m_DataSource.Open( L"SQLOLEDB", &m_PropSet, 1) );

	Proc_End:

		if(!(hr))
		{
			Factory::ErrorLog( nullptr, hr, typeid(*this).name() );
			m_DataSource.Close();
		}

		return hr;
	}

	// reopen DB source
	Result DataSourceOLEDB::Reopen()
	{
		Result hr = ResultCode::SUCCESS;

		CloseDBSource();

		dbChk( m_DataSource.Open( L"SQLOLEDB", &m_PropSet, 1) );

	Proc_End:

		return hr;
	}

	// close DB source
	Result	DataSourceOLEDB::CloseDBSource()
	{
		DataSource::CloseDBSource();
		m_DataSource.Close();

		return ResultCode::SUCCESS;
	}
	

} // namespace DB
} // namespace BR

#else
void DataSourceOLEDB_Dummy() {}
#endif // BRDB_USE_OLEDB
