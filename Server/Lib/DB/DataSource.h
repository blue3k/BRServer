////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author: Kyungkun Ko
//
// Description : DB DataSource interface
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Multithread/SFThread.h"
#include "Container/SFStackPool.h"
#include "Util/SFString.h"


namespace SF {
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

		IHeap& m_Heap;

		// service connection string
		String			m_strConnectionString;
		// DB Name
		String			m_strDefaultDB;
		// User ID
		String			m_strUserID;
		// password
		String			m_strPassword;

		// is opened
		bool m_Opened;

		// Session pool
		StackPool			m_SessionPool;

	public:
		DataSource(IHeap& memMgr)
			: m_Heap(memMgr)
			, m_Opened(false)
		{}
		virtual ~DataSource() {}

		IHeap& GetHeap() { return m_Heap; }

		bool GetOpened() { return m_Opened; }

		const String& GetConnectionString()					{ return m_strConnectionString; }
		const String& GetDefaultDB()							{ return m_strDefaultDB; }
		const String& GetUserID()								{ return m_strUserID; }
		const String& GetPassword()							{ return m_strPassword; }


		// initialize DB source. This will open source if necessary
		virtual Result	InitializeDBSource( const String& strConnectionString, const String& strDefaultDB, const String& strUserID, const String& strPassword );

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
} // namespace SF
