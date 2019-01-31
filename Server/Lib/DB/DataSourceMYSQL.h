////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author: Kyungkun Ko
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
		String m_ServerIP;

		// server port
		uint m_Port;

	public:
		DataSourceMYSQL(IHeap& memMgr);
		virtual ~DataSourceMYSQL() { CloseDBSource(); }

		const String& GetServerIP()			{ return m_ServerIP; }
		uint GetServerPort()						{ return m_Port; }

		// initialize DB source
		virtual Result	InitializeDBSource( const String& strConnectionString, const String& strDBName, const String& strUserID, const String& strPassword );

		// reopen DB source
		virtual Result Reopen();

		// close DB source
		virtual Result	CloseDBSource();

	};

} // namespace DB
} // namespace SF
