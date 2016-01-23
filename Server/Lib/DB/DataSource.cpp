////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : MadK
//
// Description : Procees Requested DB Query
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/StrUtil.h"
#include "DB/QueryWorker.h"
#include "DB/DBTrace.h"
#include "DB/DataSource.h"
#include "DB/Session.h"
#include "DB/Factory.h"


namespace BR {
namespace DB {

	// initialize DB source
	HRESULT	DataSource::InitializeDBSource( const std::string& strConnectionString, const std::string& strDefaultDB, const std::string& strUserID, const std::string& strPassword )
	{
		m_strConnectionString = strConnectionString;
		m_strDefaultDB = strDefaultDB;
		m_strUserID = strUserID;
		m_strPassword = strPassword;

		m_Opened = true;

		return S_SYSTEM_OK;
	}
	
	// close DB source
	HRESULT	DataSource::CloseDBSource()
	{
		HRESULT	hr = S_SYSTEM_OK;

		StackPool::Item *pItem = nullptr;
		do{
			pItem = m_SessionPool.Pop();
			
			delete (Session*)pItem;
		} while( pItem != nullptr );

		m_Opened = false;

	//Proc_End:

		return hr;
	}

	// Assign a session
	HRESULT	DataSource::AssignSession( Session* &pSession )
	{
		HRESULT	hr = S_SYSTEM_OK;

		pSession = nullptr;

		auto pItem = m_SessionPool.Pop();
		if( pItem == nullptr )
		{
			dbChk( Factory::GetInstance().CreateSession(this,pSession) );
		}
		else
		{
			pSession = (Session*)pItem;
			dbChk(pSession->Ping());
		}

	Proc_End:

		return hr;
	}

	// Free a session
	HRESULT	DataSource::FreeSession( Session* pSession )
	{
		HRESULT	hr = S_SYSTEM_OK;

		dbChkPtr(pSession);

		m_SessionPool.Push((StackPool::Item*)(pSession));
		pSession = nullptr;

	Proc_End:

		return hr;
	}


} // namespace DB
} // namespace BR
