// AVLTreeTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <gtest/gtest.h>
#include "Common/SystemSynchronization.h"
#include "Common/Thread.h"
#include "Common/SortedMap.h"
#include "Common/TimeUtil.h"
#include "windows.h"
#include "stdio.h"
#include "tchar.h"
#include <unordered_map>

#include "AVLTreeTest.h"


using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;
using namespace ::BR;





extern void TestSortedMap_PerfTest(unsigned numberOfTest, const int *TestValues)
{
	BR::SortedMap<UINT,UINT> sortedMap;
	TimeStampMS start, end;

	start = BR::Util::Time.GetRawTimeMs();
	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest])));
	}
	end = BR::Util::Time.GetRawTimeMs();
	printf("Insert %d items, %dms\n", numberOfTest, (end - start).count());

	start = BR::Util::Time.GetRawTimeMs();
	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		UINT value = -1;
		AssertRel(SUCCEEDED(sortedMap.Remove(TestValues[iTest], value)));
		AssertRel(value == TestValues[iTest]);
	}
	end = BR::Util::Time.GetRawTimeMs();
	printf("Remove  %d items, %dms\n", numberOfTest, (end - start).count());


	unsigned iTest = 0;
	for (; iTest < numberOfTest - 1000; iTest++)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest])));
	}
	start = BR::Util::Time.GetRawTimeMs();
	for (; iTest < numberOfTest; iTest++)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest])));
	}
	end = BR::Util::Time.GetRawTimeMs();
	printf("Insert last 1000 items, %dms\n", (end - start).count());

	start = BR::Util::Time.GetRawTimeMs();
	for (iTest = 0; iTest < std::min((unsigned)1000, numberOfTest); iTest++)
	{
		UINT value = -1;
		AssertRel(SUCCEEDED(sortedMap.Remove(TestValues[iTest], value)));
		AssertRel(value == TestValues[iTest]);
	}
	end = BR::Util::Time.GetRawTimeMs();
	printf("Remove first 1000 items, %dms\n", (end - start).count());

	start = BR::Util::Time.GetRawTimeMs();
	for (; iTest < numberOfTest; iTest++)
	{
		UINT value = -1;
		AssertRel(SUCCEEDED(sortedMap.Remove(TestValues[iTest], value)));
		AssertRel(value == TestValues[iTest]);
	}
	end = BR::Util::Time.GetRawTimeMs();
	printf("Remove remain items, %dms\n", (end - start).count());
}

