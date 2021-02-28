////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author: Kyungkun Ko
//
// Description : Data source for OLEDB
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "DB/DataSource.h"
#include <atlcore.h>
#include <atldbcli.h>


namespace SF {
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
		virtual Result	InitializeDBSource( const std::string& strConnectionString, const std::string& strDBName, const std::string& strUserID, const std::string& strPassword );

		// reopen DB source
		virtual Result Reopen();

		// close DB source
		virtual Result	CloseDBSource();

	};

} // namespace DB
} // namespace SF
