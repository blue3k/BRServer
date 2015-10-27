// TestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <gtest/gtest.h>
#include "Common/StrUtil.h"
#include "Common/StrFormat.h"
#include "Common/PageQueue.h"
#include "Common/Trace.h"
#include "DBTest.h"
#include "DB/AccountDB.h"
#include "ServerSystem/Transaction.h"
#include "DB/AccountQuery.h"


using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;
using namespace ::BR;
using namespace ::BR::DB;

namespace BRTest
{

	class TestDBServer : public AccountDB
	{
	public:

		TestDBServer()
			:m_Counter(1)
		{

		}

		BR::SyncCounter m_Counter;

		// Route query result to entity
		virtual HRESULT RouteResult(Query* &pQuery)
		{
			HRESULT hr = S_OK;
			BR::Svr::TransactionResult *pRes = (BR::Svr::TransactionResult*)pQuery;

			m_Counter.fetch_sub(1,std::memory_order_relaxed);

			defChk( pRes->GetHRESULT() );

			switch( pRes->GetMsgID().IDs.MsgCode )
			{
			case BR::DB::MCODE_QueryCreateUser:
				{
					QueryCreateUserCmd* pCmd = (QueryCreateUserCmd*)pRes;
					defTrace( Trace::TRC_DBG1, "QueryCreateUserCmd %0%, %1%, Res:%2%", pCmd->UserName, pCmd->Password, pCmd->Result );
				}
				break;
			case BR::DB::MCODE_QueryLogin:
				{
					QueryLoginCmd* pCmd = (QueryLoginCmd*)pRes;
					defTrace( Trace::TRC_DBG1, "QueryLoginCmd %0%, %1%, %2%, Res%3%", pCmd->UserName, pCmd->Password, pCmd->AccountID, pCmd->Result );
				}
				break;
			case BR::DB::MCODE_QueryFacebookCreateUser:
				{
					QueryFacebookCreateUserCmd* pCmd = (QueryFacebookCreateUserCmd*)pRes;
					defTrace( Trace::TRC_DBG1, "QueryFacebookCreateUserCmd %0%, %1%, Res:%2%", pCmd->FBUserID, pCmd->EMail, pCmd->Result );
				}
				break;
			case BR::DB::MCODE_QueryFacebookLogin:
				{
					QueryFacebookLoginCmd* pCmd = (QueryFacebookLoginCmd*)pRes;

					defTrace( Trace::TRC_DBG1, "QueryFacebookLoginCmd %0%, %1%, Res:%2%", pCmd->FBUserID, pCmd->AccountID, pCmd->Result );
				}
				break;
			};

		Proc_End:

			AssertRel(SUCCEEDED(hr));

			pQuery = nullptr;
			Util::SafeDelete( pRes );

			return hr;
		}

		const char* GetRandomUserName()
		{
			static char NameBuffer[1024];
			StrUtil::Format( NameBuffer, "TestUserName%0%", rand()%100 );
			return NameBuffer;
		}

		const char* GetRandomFBUserName()
		{
			static char NameBuffer[1024];
			StrUtil::Format( NameBuffer, "TestFBUserName%0%", rand()%100 );
			return NameBuffer;
		}
		
		const UINT64 GetRandomFBUserID()
		{
			return rand()%50;
		}
		
		const char* GetRandomFBEMail()
		{
			static char NameBuffer[1024];
			StrUtil::Format( NameBuffer, "fakeUser%0%@gmail.com", rand()%100 );
			return NameBuffer;
		}
		
		const char* GetRandomNickName()
		{
			static char NameBuffer[1024];
			StrUtil::Format( NameBuffer, "TestFBUserName%0%", rand()%100 );
			return NameBuffer;
		}

	};

TEST_F(DBTest, DBManager)
{
	const int MAX_NUMBER	= 100;
	const int MAX_TEST		= 1000;

	TestDBServer *pTestDB = new TestDBServer;
	HRESULT hr = S_OK;
	hr = reinterpret_cast<DB::QueryManager*>(pTestDB)->InitializeDB( 1 );
	EXPECT_HRESULT_SUCCEEDED( hr );
	if( FAILED(hr) )
		return;

	reinterpret_cast<DB::QueryManager*>(pTestDB)->AddDBSource( 
				0,
				"",
				"127.0.0.1,11000", 
				"dbAccount", 
				"brave", "dydwk12#" );


	srand( Util::Time.GetRawTimeMs() );

	std::vector<std::function<void()>> queryList;

	queryList.push_back( [&pTestDB](){ pTestDB->UserList(pTestDB->m_Counter.fetch_add(1,std::memory_order_relaxed)); } );
	queryList.push_back( [&pTestDB](){
		const char* userName = pTestDB->GetRandomUserName();
		pTestDB->CreateUser(pTestDB->m_Counter.fetch_add(1,std::memory_order_relaxed),
							userName, userName
							);
	} );
	queryList.push_back( [&pTestDB](){
		const char* userName = pTestDB->GetRandomUserName();
		pTestDB->LogIn(pTestDB->m_Counter.fetch_add(1,std::memory_order_relaxed),
							userName, userName );
	} );
	queryList.push_back( [&pTestDB](){
			pTestDB->FacebookCreateUser(pTestDB->m_Counter.fetch_add(1,std::memory_order_relaxed),
			pTestDB->GetRandomFBUserID(), pTestDB->GetRandomFBEMail(), pTestDB->GetRandomNickName() );
	} );
	queryList.push_back( [&pTestDB](){
		pTestDB->FacebookLogIn(pTestDB->m_Counter.fetch_add(1,std::memory_order_relaxed),
			pTestDB->GetRandomFBUserID() );
	} );


	for( int iTest = 0; iTest < 10000; iTest++ )
	{
		queryList[rand()%queryList.size()]();
	}

	while( pTestDB->m_Counter > 0 )
	{
		ThisThread::SleepFor(DurationMS(100));
	}

	reinterpret_cast<DB::QueryManager*>(pTestDB)->TerminateDB();

	delete pTestDB;
}



TEST_F(DBTest, DBQueryLogin)
{
	TestDBServer *pTestDB = new TestDBServer;
	HRESULT hr = S_OK;
	hr = reinterpret_cast<DB::QueryManager*>(pTestDB)->InitializeDB( 1 );
	EXPECT_HRESULT_SUCCEEDED( hr );
	if( FAILED(hr) )
		return;

	reinterpret_cast<DB::QueryManager*>(pTestDB)->AddDBSource( 
				0,
				"",
				"127.0.0.1,11000", 
				"dbAccount", 
				"brave", "dydwk12#" );


	pTestDB->LogIn(pTestDB->m_Counter.fetch_add(1,std::memory_order_relaxed), "Tester1", "Tester1" );

	while( pTestDB->m_Counter > 0 )
	{
		ThisThread::SleepFor(DurationMS(100));
	}

	reinterpret_cast<DB::QueryManager*>(pTestDB)->TerminateDB();

	delete pTestDB;
}

TEST_F(DBTest, DBQueryLoginByFB)
{
	TestDBServer *pTestDB = new TestDBServer;
	HRESULT hr = S_OK;
	hr = reinterpret_cast<DB::QueryManager*>(pTestDB)->InitializeDB( 1 );
	EXPECT_HRESULT_SUCCEEDED( hr );
	if( FAILED(hr) )
		return;

	reinterpret_cast<DB::QueryManager*>(pTestDB)->AddDBSource( 
				0,
				"",
				"127.0.0.1,11000", 
				"dbAccount", 
				"brave", "dydwk12#" );


	pTestDB->FacebookLogIn(pTestDB->m_Counter.fetch_add(1,std::memory_order_relaxed), 100001894965105 );

	while( pTestDB->m_Counter > 0 )
	{
		ThisThread::SleepFor(DurationMS(100));
	}

	reinterpret_cast<DB::QueryManager*>(pTestDB)->TerminateDB();

	delete pTestDB;
}

TEST_F(DBTest, DBQuerySetNick)
{
	const wchar_t strTestString[] = L"¤©1¤©1";

	TestDBServer *pTestDB = new TestDBServer;
	HRESULT hr = S_OK;
	hr = reinterpret_cast<DB::QueryManager*>(pTestDB)->InitializeDB( 1 );
	EXPECT_HRESULT_SUCCEEDED( hr );
	if( FAILED(hr) )
		return;

	reinterpret_cast<DB::QueryManager*>(pTestDB)->AddDBSource( 
				0,
				"",
				"127.0.0.1,11000", 
				"dbAccount", 
				"brave", "dydwk12#" );


	char strTest[1024], strTest2[1024];

	StrUtil::WCSToUTF8( strTestString, strTest );
	StrUtil::WCSToMBCS( strTestString, strTest2 );

	//pTestDB->SetN(pTestDB->m_Counter.fetch_add(1,std::memory_order_relaxed), 21124, strTest );

	//while( pTestDB->m_Counter > 0 )
	//{
	//	ThisThread::SleepFor(DurationMS(100));
	//}

	reinterpret_cast<DB::QueryManager*>(pTestDB)->TerminateDB();

	delete pTestDB;
}

//
//TEST_F(DBTest, DBQueryGetPlayerInfo)
//{
//	const wchar_t strTestString[] = L"¤©1¤©1";
//
//	TestDBServer *pTestDB = new TestDBServer;
//	HRESULT hr = S_OK;
//	hr = reinterpret_cast<DB::QueryManager*>(pTestDB)->InitializeDB( 1 );
//	EXPECT_HRESULT_SUCCEEDED( hr );
//	if( FAILED(hr) )
//		return;
//
//	reinterpret_cast<DB::QueryManager*>(pTestDB)->AddDBSource( 
//				0,
//				"",
//				"127.0.0.1,11000", 
//				"dbAccount", 
//				"brave", "dydwk12#" );
//
//
//	char strTest[1024], strTest2[1024];
//
//	StrUtil::WCSToUTF8( strTestString, strTest );
//	StrUtil::WCSToMBCS( strTestString, strTest2 );
//
//	pTestDB->SetNickName(pTestDB->m_Counter.fetch_add(1,std::memory_order_relaxed), 21124, strTest );
//
//	while( pTestDB->m_Counter > 0 )
//	{
//		ThisThread::SleepFor(DurationMS(100));
//	}
//
//	reinterpret_cast<DB::QueryManager*>(pTestDB)->TerminateDB();
//
//	delete pTestDB;
//}


}  // namespace BRTest
