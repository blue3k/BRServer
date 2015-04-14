////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Test base
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include <gtest/gtest.h>
#include "Common/Trace.h"
#include "Common/StrUtil.h"
#include "Common/MemLog.h"
#include "Common/MemoryPool.h"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

namespace BRTest
{

	// base test 
	class MyTestBase : public testing::Test
	{
	public:
		// Do memory logging
		static bool cfg_MemLog;
	
	protected:

	public:
		MyTestBase()
		{
		}

		// Remember that SetUp() is run immediately before a test starts.
		virtual void SetUp()
		{
			BR::MemoryPoolManager::Initialize();
		}

		// TearDown() is invoked immediately after a test finishes.  Here we
		virtual void TearDown()
		{
			BR::MemoryPoolManager::Terminate();
		}
	};


	// Time based test
	class TimedTest : public MyTestBase
	{
	protected:
		// Expected execution time
		time_t ExpectedTime;

		// The UTC time (in seconds) when the test starts
		time_t start_time_;

	public:
		TimedTest()
			:ExpectedTime(5)
		{
		}

		// set expected execution time
		void SetExpectedTime( time_t NewExpectedTime )
		{
			ExpectedTime = NewExpectedTime;
		}

		// Remember that SetUp() is run immediately before a test starts.
		// This is a good place to record the start time.
		virtual void SetUp()
		{
			start_time_ = time(NULL);
		}

		// TearDown() is invoked immediately after a test finishes.  Here we
		// check if the test was too slow.
		virtual void TearDown()
		{
			// Gets the time when the test finishes
			const time_t end_time = time(NULL);

			// Asserts that the test took no more than ~5 seconds.  Did you
			// know that you can use assertions in SetUp() and TearDown() as
			// well?
			EXPECT_TRUE(end_time - start_time_ <= ExpectedTime) << "The test took too long.";
		}
	};


	// Scoped memory logging 
	class ScopedMemLog
	{
	public:
		ScopedMemLog()
		{
			if( MyTestBase::cfg_MemLog && BR::GetMemLogger() )
			{
				BR::GetMemLogger()->SetLogMask( 0xFFFFFFFF );
				BR::GetMemLogger()->StartDifferenceCheck();
			}
		}

		~ScopedMemLog()
		{
			if( MyTestBase::cfg_MemLog && BR::GetMemLogger() )
			{
				BR::GetMemLogger()->ReportDifference();
				BR::GetMemLogger()->StopDifferenceCheck();
				BR::GetMemLogger()->SetLogMask( 0 );
			}
		}
	};

#ifdef MEMCHECKSCOPE
	#undef MEMCHECKSCOPE
#endif
#define MEMCHECKSCOPE() BRTest::ScopedMemLog __MemLogScope;
}  // namespace
