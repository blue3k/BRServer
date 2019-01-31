////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : KyungKun Ko
//
// Description : Concurrent container test
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include <gtest/gtest.h>
#include "Common/Thread.h"
#include "TestBase.h"



namespace BRTest
{

	class ConcurrentContainerTest : public MyTestBase
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
			MyTestBase::SetUp();
		}

		// TearDown() is invoked immediately after a test finishes.  Here we
		virtual void TearDown()
		{
			MyTestBase::TearDown();

			StopAllThread();

		}
	};


}  // namespace BRTest


