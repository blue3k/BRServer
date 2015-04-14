// AVLTreeTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <gtest/gtest.h>
#include "Common/SystemSynchronize.h"
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





TEST_F(AVLTreeTest, SortedMapSimple)
//void TestSortedMap_Simple(unsigned numberOfTest, const int *TestValues)
{

	for (int iTest = 0; iTest < MAX_TEST_VALUE; iTest++)
	{
		TestValues[iTest] = MAX_TEST_VALUE - iTest;
	}

	BR::SortedMap<UINT,UINT> sortedMap;

	INT64 order = -1;
	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest], &order)));
		AssertRel(order == 0);
		UINT value = -1;
		AssertRel(SUCCEEDED(sortedMap.Remove(TestValues[iTest], value)));
		AssertRel(value == TestValues[iTest]);
	}

	AssertRel(sortedMap.GetItemCount() == 0);

	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest], &order)));
		AssertRel(order == iTest);
	}

	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		UINT value = -1;
		AssertRel(SUCCEEDED(sortedMap.Remove(TestValues[iTest], value)));
		AssertRel(value == TestValues[iTest]);
	}

	AssertRel(sortedMap.GetItemCount() == 0);

	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest])));
	}

	for (int iTest = numberOfTest - 1; iTest >= 0; iTest--)
	{
		UINT value = -1;
		AssertRel(SUCCEEDED(sortedMap.Remove(TestValues[iTest], value)));
		AssertRel(value == TestValues[iTest]);
	}

	AssertRel(sortedMap.GetItemCount() == 0);

	for (int iTest = numberOfTest - 1; iTest >= 0; iTest--)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest])));
	}

	for (int iTest = numberOfTest - 1; iTest >= 0; iTest--)
	{
		UINT value = -1;
		AssertRel(SUCCEEDED(sortedMap.Remove(TestValues[iTest], value)));
		AssertRel(value == TestValues[iTest]);
	}

	AssertRel(sortedMap.GetItemCount() == 0);

	for (int iTest = numberOfTest - 1; iTest >= 0; iTest--)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest])));
	}

	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		UINT value = -1;
		AssertRel(SUCCEEDED(sortedMap.Remove(TestValues[iTest], value)));
		AssertRel(value == TestValues[iTest]);
	}

	AssertRel(sortedMap.GetItemCount() == 0);
}



TEST_F(AVLTreeTest, SortedMapOrderSequential)
{
	BR::SortedMap<UINT,UINT> sortedMap;

	for (int iTest = 0; iTest < MAX_TEST_VALUE; iTest++)
	{
		TestValues[iTest] = MAX_TEST_VALUE - iTest;
	}

	// test foreach
	INT64 order = -1;
	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest], &order)));
		AssertRel(order == iTest);
	}

	for (unsigned iTest = 0; iTest < numberOfTest; iTest += offset)
	{
		decltype(iTest) innerTest = 0;

		sortedMap.ForeachOrder(iTest, numberOfTest, [&](const UINT& key, const UINT& value) -> bool
		{
			AssertRel(TestValues[iTest + innerTest] == value);
			innerTest++;
			return true;
		});

		sortedMap.ForeachReverseOrder(iTest, numberOfTest, [&](const UINT& key, const UINT& value) -> bool
		{
			innerTest--;
			AssertRel(TestValues[iTest + innerTest] == value);
			return true;
		});
	}

	for (unsigned iTest = 0; iTest < numberOfTest; iTest += offset)
	{
		decltype(iTest) innerTest = 0;

		sortedMap.ForeachOrder(0, numberOfTest - iTest, [&](const UINT& key, const UINT& value)
		{
			AssertRel(TestValues[innerTest] == value);
			innerTest++;
			return true;
		});

		sortedMap.ForeachReverseOrder(0, numberOfTest - iTest, [&](const UINT& key, const UINT& value)
		{
			AssertRel(TestValues[innerTest] == value);
			innerTest++;
			return true;
		});
	}

	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		UINT value = -1;
		AssertRel(SUCCEEDED(sortedMap.Remove(TestValues[iTest], value)));
		AssertRel(value == TestValues[iTest]);
	}

	AssertRel(sortedMap.GetItemCount() == 0);


	for (int iTest = numberOfTest - 1; iTest >= 0; iTest--)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest])));
	}

	for (unsigned iTest = 0; iTest < numberOfTest; iTest += offset)
	{
		decltype(iTest) innerTest = 0;

		sortedMap.ForeachOrder(iTest, numberOfTest, [&](const UINT& key, const UINT& value)
		{
			AssertRel(TestValues[iTest + innerTest] == value);
			innerTest++;
			return true;
		});
	}

	for (int iTest = numberOfTest - 1; iTest >= 0; iTest--)
	{
		UINT value = -1;
		AssertRel(SUCCEEDED(sortedMap.Remove(TestValues[iTest], value)));
		AssertRel(value == TestValues[iTest]);
	}

	AssertRel(sortedMap.GetItemCount() == 0);
}

TEST_F(AVLTreeTest, SortedMapOrderRandom)
//void TestSortedMap_OrderRandomInput(unsigned numberOfTest, const int *TestValues)
{
	BR::SortedMap<UINT,UINT> sortedMap;

	// test foreach
	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest])));
	}

	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		long latestValue = -1;

		sortedMap.ForeachOrder(iTest, numberOfTest, [&](const UINT& key, const UINT& value)
		{
			if (latestValue != -1)
			{
				AssertRel(value <= (UINT)latestValue);
			}
			latestValue = value;
			return true;
		});

		latestValue = -1;
		sortedMap.ForeachReverseOrder(iTest, numberOfTest, [&](const UINT& key, const UINT& value)
		{
			if (latestValue != -1)
			{
				AssertRel(value >= (UINT)latestValue);
			}
			latestValue = value;
			return true;
		});
	}

	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		long latestValue = -1;

		sortedMap.ForeachOrder(0, numberOfTest - iTest, [&](const UINT& key, const UINT& value)
		{
			if (latestValue != -1)
			{
				AssertRel(value <= (UINT)latestValue);
			}
			latestValue = value;
			return true;
		});

		latestValue = -1;
		sortedMap.ForeachReverseOrder(0, numberOfTest - iTest, [&](const UINT& key, const UINT& value)
		{
			if (latestValue != -1)
			{
				AssertRel(value >= (UINT)latestValue);
			}
			latestValue = value;
			return true;
		});
	}

	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		UINT value = -1;
		AssertRel(SUCCEEDED(sortedMap.Remove(TestValues[iTest], value)));
		AssertRel(value == TestValues[iTest]);
	}

	AssertRel(sortedMap.GetItemCount() == 0);


	for (int iTest = numberOfTest - 1; iTest >= 0; iTest--)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest])));
	}

	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		long latestValue = -1;

		sortedMap.ForeachOrder(iTest, numberOfTest, [&](const UINT& key, const UINT& value)
		{
			if (latestValue != -1)
			{
				AssertRel(value <= (UINT)latestValue);
			}
			latestValue = value;
			return true;
		});
	}

	for (int iTest = numberOfTest - 1; iTest >= 0; iTest--)
	{
		UINT value = -1;
		AssertRel(SUCCEEDED(sortedMap.Remove(TestValues[iTest], value)));
		AssertRel(value == TestValues[iTest]);
	}

	AssertRel(sortedMap.GetItemCount() == 0);
}

