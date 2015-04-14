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
#include <ppl.h>
#include "Common/Thread.h"
#include "Common/MemoryPool.h"
#include "Common/HashTable.h"
#include "TestBase.h"


using namespace ::BR;

namespace BRTest
{

	class HTTPTest : public MyTestBase
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
			BR::Trace::Initialize();
			BR::MemoryPoolManager::Initialize();
			__super::SetUp();
		}

		// TearDown() is invoked immediately after a test finishes.  Here we
		virtual void TearDown()
		{
			__super::TearDown();

			StopAllThread();
			BR::Trace::Uninitialize();
			BR::MemoryPoolManager::Terminate();
		}
	};


}  // namespace BRTest


