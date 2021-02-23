////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author: Kyungkun Ko
//
// Description : Procees Requested DB Query
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "DBPch.h"
#include "SFTypedefs.h"
#include "Util/SFStrUtil.h"
#include "DB/QueryWorker.h"
#include "DB/DBTrace.h"
#include "DB/DataSource.h"
#include "DB/Session.h"
#include "DB/DBConfig.h"
#include "Service/ServerService.h"

namespace SF {
namespace DB {

	// initialize DB source
	Result	DataSource::InitializeDBSource( const String& strConnectionString, const String& strDefaultDB, const String& strUserID, const String& strPassword )
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

		StackPool::Item *pItem = m_SessionPool.Pop();
		while (pItem != nullptr) 
		{
			IHeap::Delete((Session*)pItem);
			pItem = m_SessionPool.Pop();
		};

		m_Opened = false;

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
			dbChk(Service::Database->CreateSession(GetHeap(), this,pSession));
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
} // namespace SF
