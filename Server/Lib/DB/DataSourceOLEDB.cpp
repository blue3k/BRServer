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


#include "StdAfx.h"
#include "Common/Typedefs.h"
#include "Factory.h"

#ifdef BRDB_USE_OLEDB

#include <atldbcli.h>
#include "Common/StrUtil.h"
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
	HRESULT	DataSourceOLEDB::InitializeDBSource( const std::string& strConnectionString, const std::string& strDBName, const std::string& strUserID, const std::string& strPassword )
	{
		HRESULT hr = S_OK;

		dbChk( __super::InitializeDBSource( strConnectionString, strDBName, strUserID, strPassword ) );

		m_PropSet = CDBPropSet(DBPROPSET_DBINIT);
		m_PropSet.AddProperty(DBPROP_INIT_DATASOURCE, A2BSTR(m_strConnectionString.c_str()));
		m_PropSet.AddProperty(DBPROP_INIT_CATALOG, A2BSTR(m_strDefaultDB.c_str()));
		m_PropSet.AddProperty(DBPROP_AUTH_USERID, A2BSTR(m_strUserID.c_str()));
		m_PropSet.AddProperty(DBPROP_AUTH_PASSWORD, A2BSTR(m_strPassword.c_str()));

		dbChk( m_DataSource.Open( L"SQLOLEDB", &m_PropSet, 1) );

	Proc_End:

		if(FAILED(hr))
		{
			Factory::ErrorLog( nullptr, hr, typeid(*this).name() );
			m_DataSource.Close();
		}

		return hr;
	}

	// reopen DB source
	HRESULT DataSourceOLEDB::Reopen()
	{
		HRESULT hr = S_OK;

		CloseDBSource();

		dbChk( m_DataSource.Open( L"SQLOLEDB", &m_PropSet, 1) );

	Proc_End:

		return hr;
	}

	// close DB source
	HRESULT	DataSourceOLEDB::CloseDBSource()
	{
		__super::CloseDBSource();
		m_DataSource.Close();

		return S_OK;
	}
	

} // namespace DB
} // namespace BR

#endif // BRDB_USE_OLEDB
