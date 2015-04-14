////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : ServerSystem test
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include <gtest/gtest.h>
#include <ppl.h>
#include "Common/Thread.h"
#include "Common/MemoryPool.h"
#include "Common/HashTable.h"
#include "TestBase.h"



namespace BRTest
{

	class ServerSystemTest : public MyTestBase
	{
	protected:

		// Thread module test
		std::vector<BR::Thread*>	m_Threads;

		BR::SpinLock m_LockObject;

	public:

		void StopAllThread()
		{
			std::for_each( m_Threads.begin(), m_Threads.end(), []( BR::Thread* pThread )
			{
				if( pThread ) pThread->Stop( true );
				delete pThread;
			});
			m_Threads.clear();
		}

		// Remember that SetUp() is run immediately before a test starts.
		virtual void SetUp()
		{
			BR::MemoryPoolManager::Initialize();
			__super::SetUp();
		}

		// TearDown() is invoked immediately after a test finishes.  Here we
		virtual void TearDown()
		{
			__super::TearDown();

			StopAllThread();

			BR::MemoryPoolManager::Terminate();
		}
	};


}  // namespace BRTest


