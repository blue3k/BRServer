// AVLTreeTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <gtest/gtest.h>
#include "Common/SystemSynchronization.h"
#include "Common/Thread.h"
#include "Common/DualSortedMap.h"
#include "Common/TimeUtil.h"
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




TEST_F(AVLTreeTest, DualSortedMapPerfTestSimple)
{
	BR::DualSortedMap<UINT,UINT> sortedMap;
	TimeStampMS start, end;

	start = BR::Util::Time.GetRawTimeMs();
	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest])));
	}
	AssertRel(SUCCEEDED(sortedMap.CommitChanges()));
	end = BR::Util::Time.GetRawTimeMs();
	printf("Insert %d items, %dms\n", numberOfTest, (end - start).count());

	start = BR::Util::Time.GetRawTimeMs();
	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		UINT value = -1;
		AssertRel(SUCCEEDED(sortedMap.Remove(TestValues[iTest], value)));
		AssertRel((INT)value == TestValues[iTest]);
	}
	AssertRel(SUCCEEDED(sortedMap.CommitChanges()));
	end = BR::Util::Time.GetRawTimeMs();
	printf("Remove  %d items, %dms\n", numberOfTest, (end - start).count());


	start = BR::Util::Time.GetRawTimeMs();
	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest])));
		AssertRel(SUCCEEDED(sortedMap.CommitChanges()));
	}
	end = BR::Util::Time.GetRawTimeMs();
	printf("Insert with commit %d items, %dms\n", numberOfTest, (end - start).count());

	start = BR::Util::Time.GetRawTimeMs();
	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		UINT value = -1;
		AssertRel(SUCCEEDED(sortedMap.Remove(TestValues[iTest], value)));
		AssertRel((INT)value == TestValues[iTest]);
		AssertRel(SUCCEEDED(sortedMap.CommitChanges()));
	}
	end = BR::Util::Time.GetRawTimeMs();
	printf("Remove with commit  %d items, %dms\n", numberOfTest, (end - start).count());
}


TEST_F(AVLTreeTest, DualSortedMapPerfTestRead)
{
	BR::DualSortedMap<UINT,UINT> sortedMap;
	TimeStampMS start, end;

	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest])));
	}
	AssertRel(SUCCEEDED(sortedMap.CommitChanges()));

	start = BR::Util::Time.GetRawTimeMs();
	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		UINT value = -1;
		AssertRel(SUCCEEDED(sortedMap.Find(TestValues[iTest], value)));
		AssertRel((INT)value == TestValues[iTest]);
	}
	end = BR::Util::Time.GetRawTimeMs();
	printf("Find %d items, %dms\n", numberOfTest, (end - start).count());


	start = BR::Util::Time.GetRawTimeMs();
	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		//UINT value = -1;
		long latestValue = -1;

		sortedMap.ForeachOrder(0, 100, [&](const UINT& key, const UINT& value)
		{
			if (latestValue != -1)
			{
				AssertRel(value <= (UINT)latestValue);
			}
			latestValue = value;
			return true;
		});
	}
	end = BR::Util::Time.GetRawTimeMs();
	printf("Query 100 items in a row in %d items %d times, %dms\n", numberOfTest, numberOfTest, (end - start).count());
}

TEST_F(AVLTreeTest, DualSortedMapPerfTest1000)
{
	BR::DualSortedMap<UINT,UINT> sortedMap;
	TimeStampMS start, end;

	int iTest = 0;
	for (; iTest < std::max(0, (int)numberOfTest - 1000); iTest++)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest])));
	}
	AssertRel(SUCCEEDED(sortedMap.CommitChanges()));

	start = BR::Util::Time.GetRawTimeMs();
	for (; iTest < (int)numberOfTest; iTest++)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest])));
	}
	AssertRel(SUCCEEDED(sortedMap.CommitChanges()));
	end = BR::Util::Time.GetRawTimeMs();
	printf("Insert last 1000 items, %dms\n", (end - start).count());

	start = BR::Util::Time.GetRawTimeMs();
	for (iTest = 0; iTest < std::min(1000, (int)numberOfTest); iTest++)
	{
		UINT value = -1;
		AssertRel(SUCCEEDED(sortedMap.Remove(TestValues[iTest], value)));
		AssertRel((INT)value == TestValues[iTest]);
	}
	AssertRel(SUCCEEDED(sortedMap.CommitChanges()));
	end = BR::Util::Time.GetRawTimeMs();
	printf("Remove first 1000 items, %dms\n", (end - start).count());

	start = BR::Util::Time.GetRawTimeMs();
	for (; iTest < (int)numberOfTest; iTest++)
	{
		UINT value = -1;
		AssertRel(SUCCEEDED(sortedMap.Remove(TestValues[iTest], value)));
		AssertRel((INT)value == TestValues[iTest]);
	}
	AssertRel(SUCCEEDED(sortedMap.CommitChanges()));
	end = BR::Util::Time.GetRawTimeMs();
	printf("Remove remain items, %dms\n", (end - start).count());
}

