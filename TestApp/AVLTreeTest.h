////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Memroy test
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include <gtest/gtest.h>
#include <ppl.h>
#include "Common/Thread.h"
#include "Common/MemoryPool.h"
#include "TestBase.h"



	class AVLTreeTest : public BRTest::MyTestBase
	{
	protected:

#ifdef DEBUG
		const int MAX_TEST_VALUE = 10;
#else
		const int MAX_TEST_VALUE = 1000000;
#endif
#ifdef DEBUG
		const unsigned offset = 1;
#else
		const unsigned offset = 601;
#endif

		// Thread module test
		std::vector<BR::Thread*>	m_Threads;

		BR::SpinLock m_LockObject;

		unsigned numberOfTest;
		int *TestValues;

	public:

		AVLTreeTest()
			: numberOfTest(0)
			, TestValues(nullptr)
		{

		}

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
			numberOfTest = MAX_TEST_VALUE;
			TestValues = new int[MAX_TEST_VALUE];
			for (int iTest = 0; iTest < MAX_TEST_VALUE; iTest++)
			{
				TestValues[iTest] = rand() % MAX_TEST_VALUE;
			}

			__super::SetUp();
		}

		// TearDown() is invoked immediately after a test finishes.  Here we
		virtual void TearDown()
		{
			__super::TearDown();

			StopAllThread();
			delete[] TestValues;
			TestValues = nullptr;
		}
	};

