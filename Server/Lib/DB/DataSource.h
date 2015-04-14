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


#include "Common/Thread.h"
#include "Common/StackPool.h"
#include "Common/Classutil.h"

namespace BR {
namespace DB {

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Data Source Class 
	//

	class Query;
	class QueryManager;
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
		BRCLASS_ATTRIBUTE_READONLY(bool,Opened);

		// Session pool
		StackPool			m_SessionPool;

	public:
		DataSource(): m_Opened(false) {}
		virtual ~DataSource() {}


		const std::string& GetConnectionString()					{ return m_strConnectionString; }
		const std::string& GetDefaultDB()							{ return m_strDefaultDB; }
		const std::string& GetUserID()								{ return m_strUserID; }
		const std::string& GetPassword()							{ return m_strPassword; }


		// initialize DB source. This will open source if necessary
		virtual HRESULT	InitializeDBSource( const std::string& strConnectionString, const std::string& strDefaultDB, const std::string& strUserID, const std::string& strPassword );

		// reopen DB source
		virtual HRESULT Reopen() = 0;

		// close DB source
		virtual HRESULT	CloseDBSource();

		// Assign a session
		virtual HRESULT	AssignSession( Session* &pSession );

		// Free a session
		virtual HRESULT	FreeSession( Session* pSession );

	};

} // namespace DB
} // namespace BR
