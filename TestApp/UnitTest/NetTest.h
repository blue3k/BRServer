////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 TheBraves
// 
// Author : KyungKun Ko
//
// Description : Net test
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include <gtest/gtest.h>
#include "Net/NetDef.h"
#include "TestBase.h"

#ifdef DEBUG
#define TestScale 1
#else
#define TestScale 10
#endif

using namespace ::BR;

namespace BRTest
{

	class NetTest : public MyTestBase
	{
	public:


	protected:

		// Network interface
		BR::Net::INet*	m_pNet;

		// Network interface
		BR::Net::IClient*	m_pNetClient;

		// Connection interface
		BR::Net::IConnection *m_pIConnection;
		BR::Net::IConnection *m_pIConnectionWorld;


		// AccountID
		BR::AccountID	m_AccountID;
		BR::AuthTicket	m_AuthTicket;

	public:


		// Remember that SetUp() is run immediately before a test starts.
		virtual void SetUp()
		{
			m_pNet = nullptr;
			m_pNetClient = nullptr;
			m_pIConnection = nullptr;
			m_pIConnectionWorld = nullptr;

			m_AccountID = 0;
			m_AuthTicket = 0;

			__super::SetUp();
		}

		// TearDown() is invoked immediately after a test finishes.  Here we
		virtual void TearDown()
		{
			if( m_pNetClient )
			{
				if( m_pIConnection )
					m_pNetClient->ReleaseConnection( m_pIConnection );
				if( m_pIConnectionWorld )
					m_pNetClient->ReleaseConnection( m_pIConnectionWorld );
			}

			m_pIConnection = nullptr;
			m_pIConnectionWorld = nullptr;
			m_pNet = nullptr;

			Util::SafeRelease( m_pNetClient );

			__super::TearDown();

		}
	};


}  // namespace BRTest


