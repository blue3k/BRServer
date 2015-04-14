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


#include "StdAfx.h"
#include "Common/Typedefs.h"
#include "Common/StrUtil.h"
#include "DB/DBTrace.h"
#include "DB/DataSource.h"
#include "DB/Session.h"
#include "DB/Factory.h"


namespace BR {
namespace DB {


	Session::Session( DataSource* pDataSource )
		:m_pDataSource(pDataSource)
		,m_bIsOpened(false)
	{
		pNext = nullptr;
	}

	
	// Open session
	HRESULT Session::OpenSession()
	{
		HRESULT	hr = S_OK;

		m_bIsOpened = true;

	Proc_End:

		return hr;
	}

	// Close session
	HRESULT Session::CloseSession()
	{
		HRESULT	hr = S_OK;

		m_bIsOpened = false;

	Proc_End:

		return hr;
	}

	// Free a session
	HRESULT	Session::ReleaseSession()
	{
		HRESULT	hr = S_OK;

		dbChkPtr(m_pDataSource);

		m_pDataSource->FreeSession( this );

	Proc_End:

		return hr;
	}


} // namespace DB
} // namespace BR
