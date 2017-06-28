////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : MadK
//
// Description : Session
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/StrUtil.h"
#include "DB/DBTrace.h"
#include "DB/DataSource.h"
#include "DB/Session.h"
#include "DB/Factory.h"


namespace BR {
namespace DB {


	Session::Session( DataSource* pDataSource )
		: m_bIsOpened(false)
		, m_pDataSource(pDataSource)
	{
		pNext = nullptr;
	}

	
	// Open session
	Result Session::OpenSession()
	{
		Result	hr = ResultCode::SUCCESS;

		m_bIsOpened = true;

	//Proc_End:

		return hr;
	}

	// Close session
	Result Session::CloseSession()
	{
		Result	hr = ResultCode::SUCCESS;

		m_bIsOpened = false;

	//Proc_End:

		return hr;
	}

	// Free a session
	Result	Session::ReleaseSession()
	{
		Result	hr = ResultCode::SUCCESS;

		dbChkPtr(m_pDataSource);

		m_pDataSource->FreeSession( this );

	Proc_End:

		return hr;
	}


} // namespace DB
} // namespace BR
