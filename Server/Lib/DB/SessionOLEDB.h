////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : MadK
//
// Description : Query worker class
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "DB/Session.h"
#include <atlcore.h>
#include <atldbcli.h>


namespace BR {
namespace DB {

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	DB Query Worker Class 
	//

	class SessionOLEDB : public Session
	{
	private:

		CSession	m_Session;

	public:
		// constructor / destructor
		SessionOLEDB(DataSource *pDataSource);
		virtual ~SessionOLEDB();
		
		// return context value
		virtual void* GetContext();

		// Send a query
		virtual HRESULT SendQuery( Query *pQuery );

		// Open session
		virtual HRESULT OpenSession();

		// Close session
		virtual HRESULT CloseSession();
	};

} // namespace DB
} // namespace BR
