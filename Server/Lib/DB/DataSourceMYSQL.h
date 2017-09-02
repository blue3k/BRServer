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


#pragma once


#include "DB/DataSource.h"


namespace SF {
namespace DB {

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Data Source Class 
	//

	class DataSourceMYSQL : public DataSource
	{
	private:

		// server ip address
		std::string m_ServerIP;

		// server port
		uint m_Port;

	public:
		DataSourceMYSQL();
		virtual ~DataSourceMYSQL() { CloseDBSource(); }

		const std::string& GetServerIP()			{ return m_ServerIP; }
		uint GetServerPort()						{ return m_Port; }

		// initialize DB source
		virtual Result	InitializeDBSource( const std::string& strConnectionString, const std::string& strDBName, const std::string& strUserID, const std::string& strPassword );

		// reopen DB source
		virtual Result Reopen();

		// close DB source
		virtual Result	CloseDBSource();

	};

} // namespace DB
} // namespace SF
