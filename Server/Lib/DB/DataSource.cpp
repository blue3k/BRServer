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
	Result	DataSource::InitializeDBSource( const std::string& strConnectionString, const std::string& strDefaultDB, const std::string& strUserID, const std::string& strPassword )
	{
		m_strConnectionString = strConnectionString;
		m_strDefaultDB = strDefaultDB;
		m_strUserID = strUserID;
		m_strPassword = strPassword;

		m_Opened = true;

		return ResultCode::SUCCESS;
	}
	
	// close DB source
	Result	DataSource::CloseDBSource()
	{
		Result	hr = ResultCode::SUCCESS;

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
	Result	DataSource::AssignSession( Session* &pSession )
	{
		Result	hr = ResultCode::SUCCESS;

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
	Result	DataSource::FreeSession( Session* pSession )
	{
		Result	hr = ResultCode::SUCCESS;

		dbChkPtr(pSession);

		m_SessionPool.Push((StackPool::Item*)(pSession));
		pSession = nullptr;

	Proc_End:

		return hr;
	}


} // namespace DB
} // namespace BR
