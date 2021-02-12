////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : DB test
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "TestBase.h"
#include "DB/Factory.h"


namespace BRTest
{
	using namespace SF;

	class DBTest : public MyTestBase
	{

	protected:

		SF::DB::DBClusterManager* m_pTestDB = nullptr;

		String m_TestDBConnectionString = "SFTestDB.com,41170";
		String m_TestDBUserId = "blue3k";
		String m_TestDBPassword = "111";

		// Thread module test
		std::vector<SF::Thread*>	m_Threads;

		SpinLock m_LockObject;

	public:


		void StopAllThread()
		{
			std::for_each( m_Threads.begin(), m_Threads.end(), []( Thread* pThread )
			{
				if( pThread ) pThread->Stop( true );
				IHeap::Delete(pThread);
			});
			m_Threads.clear();
		}

		// Remember that SetUp() is run immediately before a test starts.
		virtual void SetUp()
		{
			MyTestBase::SetUp();
			SF::DB::Factory::InitializeDBFactory();
		}

		// TearDown() is invoked immediately after a test finishes.  Here we
		virtual void TearDown()
		{
			MyTestBase::TearDown();

			StopAllThread();

			if (m_pTestDB != nullptr)
			{
				m_pTestDB->TerminateDB();
				IHeap::Delete(m_pTestDB);
			}
			m_pTestDB = nullptr;

			DB::Factory::TerminateDBFactory();
		}
	};


}  // namespace BRTest


