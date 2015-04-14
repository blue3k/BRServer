////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : MadK
//
// Description : Data source for OLEDB
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "DB/DataSource.h"
#include <atlcore.h>
#include <atldbcli.h>


namespace BR {
namespace DB {

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Data Source Class 
	//

	class DataSourceOLEDB : public DataSource
	{
	private:

		CDataSource		m_DataSource;

		CDBPropSet		m_PropSet;

	public:
		DataSourceOLEDB();
		virtual ~DataSourceOLEDB() { CloseDBSource(); }

		CDataSource& GetDataSource() { return m_DataSource; }

		// initialize DB source
		virtual HRESULT	InitializeDBSource( const std::string& strConnectionString, const std::string& strDBName, const std::string& strUserID, const std::string& strPassword );

		// reopen DB source
		virtual HRESULT Reopen();

		// close DB source
		virtual HRESULT	CloseDBSource();

	};

} // namespace DB
} // namespace BR
