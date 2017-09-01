////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : MadK
//
// Description : DB DataSource interface
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Thread/Thread.h"
#include "Container/StackPool.h"


namespace BR {
namespace DB {

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Data Source Class 
	//

	class Query;
	class DBClusterManager;
	class Session;

	class DataSource
	{

	protected:

		// service connection string
		std::string			m_strConnectionString;
		// DB Name
		std::string			m_strDefaultDB;
		// User ID
		std::string			m_strUserID;
		// password
		std::string			m_strPassword;

		// is opened
		bool m_Opened;

		// Session pool
		StackPool			m_SessionPool;

	public:
		DataSource(): m_Opened(false) {}
		virtual ~DataSource() {}

		bool GetOpened() { return m_Opened; }

		const std::string& GetConnectionString()					{ return m_strConnectionString; }
		const std::string& GetDefaultDB()							{ return m_strDefaultDB; }
		const std::string& GetUserID()								{ return m_strUserID; }
		const std::string& GetPassword()							{ return m_strPassword; }


		// initialize DB source. This will open source if necessary
		virtual Result	InitializeDBSource( const std::string& strConnectionString, const std::string& strDefaultDB, const std::string& strUserID, const std::string& strPassword );

		// reopen DB source
		virtual Result Reopen() = 0;

		// close DB source
		virtual Result	CloseDBSource();

		// Assign a session
		virtual Result	AssignSession( Session* &pSession );

		// Free a session
		virtual Result	FreeSession( Session* pSession );

	};

} // namespace DB
} // namespace BR
