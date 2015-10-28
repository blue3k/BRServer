// AVLTreeTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <gtest/gtest.h>
#include "Common/SystemSynchronization.h"
#include "Common/Thread.h"
#include "Common/DualSortedMap.h"
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



TEST_F(AVLTreeTest, DualSortedMapSimple)
{

	for (int iTest = 0; iTest < MAX_TEST_VALUE; iTest++)
	{
		TestValues[iTest] = MAX_TEST_VALUE - iTest;
	}

	BR::DualSortedMap<UINT,UINT> sortedMap;

	INT64 order = -1;
	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest], &order)));
		AssertRel(order == 0);
		UINT value = -1;
		AssertRel(SUCCEEDED(sortedMap.Remove(TestValues[iTest], value)));
		AssertRel(value == TestValues[iTest]);

		AssertRel(FAILED(sortedMap.Find(TestValues[iTest], value)));

		if ((rand()%10) > 5)
			AssertRel(SUCCEEDED(sortedMap.CommitChanges()));
	}

	AssertRel(SUCCEEDED(sortedMap.CommitChanges()));

	AssertRel(sortedMap.GetItemCount() == 0);

	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest], &order)));
		AssertRel(order == iTest);
	}

	AssertRel(SUCCEEDED(sortedMap.CommitChanges()));

	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		UINT value = -1;
		AssertRel(SUCCEEDED(sortedMap.Remove(TestValues[iTest], value)));
		AssertRel(value == TestValues[iTest]);
	}

	AssertRel(SUCCEEDED(sortedMap.CommitChanges()));

	AssertRel(sortedMap.GetItemCount() == 0);

	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest])));
	}

	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		UINT value = -1;
		AssertRel(FAILED(sortedMap.Find(TestValues[iTest], value)));
	}

	AssertRel(SUCCEEDED(sortedMap.CommitChanges()));

	for (int iTest = numberOfTest - 1; iTest >= 0; iTest--)
	{
		UINT value = -1;
		AssertRel(SUCCEEDED(sortedMap.Remove(TestValues[iTest], value)));
		AssertRel(value == TestValues[iTest]);
	}

	AssertRel(SUCCEEDED(sortedMap.CommitChanges()));

	AssertRel(sortedMap.GetItemCount() == 0);



	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest])));
	}

	AssertRel(SUCCEEDED(sortedMap.CommitChanges()));

	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		UINT value = -1;
		AssertRel(SUCCEEDED(sortedMap.Find(TestValues[iTest], value)));
		AssertRel(value == TestValues[iTest]);
	}

	for (int iTest = numberOfTest - 1; iTest >= 0; iTest--)
	{
		UINT value = -1;
		AssertRel(SUCCEEDED(sortedMap.Remove(TestValues[iTest], value)));
		AssertRel(value == TestValues[iTest]);
	}

	AssertRel(SUCCEEDED(sortedMap.CommitChanges()));
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

	AssertRel(SUCCEEDED(sortedMap.CommitChanges()));
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

	AssertRel(SUCCEEDED(sortedMap.CommitChanges()));
	AssertRel(sortedMap.GetItemCount() == 0);
}


TEST_F(AVLTreeTest, DualSortedMapOrderSequential)
{
	BR::DualSortedMap<UINT,UINT> sortedMap;

	for (int iTest = 0; iTest < MAX_TEST_VALUE; iTest++)
	{
		TestValues[iTest] = MAX_TEST_VALUE - iTest;
	}


	// test foreach
	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest])));
	}

	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		decltype(iTest) innerTest = 0;
		
		sortedMap.ForeachOrder(iTest, numberOfTest, [&](const UINT& key, const UINT& value)
		{
			AssertRel(TestValues[iTest + innerTest] == value);
			innerTest++;
			return true;
		});
		
		sortedMap.ForeachReverseOrder(iTest, numberOfTest, [&](const UINT& key, const UINT& value)
		{
			innerTest++;
			AssertRel(TestValues[iTest + innerTest] == value);
			return true;
		});
	}

	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
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
			innerTest--;
			AssertRel(TestValues[innerTest] == value);
			return true;
		});
	}

	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		UINT value = -1;
		AssertRel(SUCCEEDED(sortedMap.Remove(TestValues[iTest], value)));
		AssertRel(value == TestValues[iTest]);
	}

	AssertRel(SUCCEEDED(sortedMap.CommitChanges()));
	AssertRel(sortedMap.GetItemCount() == 0);


	for (int iTest = numberOfTest - 1; iTest >= 0; iTest--)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest])));
	}

	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		decltype(iTest) innerTest = 0;

		sortedMap.ForeachOrder(iTest, numberOfTest, [&](const UINT& key, const UINT& value)
		{
			AssertRel(TestValues[iTest + innerTest] == value);
			innerTest++;
			return true;
		});

		sortedMap.ForeachReverseOrder(iTest, numberOfTest, [&](const UINT& key, const UINT& value)
		{
			innerTest--;
			AssertRel(TestValues[iTest + innerTest] == value);
			return true;
		});
	}

	for (int iTest = numberOfTest - 1; iTest >= 0; iTest--)
	{
		UINT value = -1;
		AssertRel(SUCCEEDED(sortedMap.Remove(TestValues[iTest], value)));
		AssertRel(value == TestValues[iTest]);
	}

	AssertRel(SUCCEEDED(sortedMap.CommitChanges()));
	AssertRel(sortedMap.GetItemCount() == 0);
}

TEST_F(AVLTreeTest, DualSortedMapRandom)
{
	BR::DualSortedMap<UINT,UINT> sortedMap;

	// test foreach
	for (unsigned iTest = 0; iTest < numberOfTest; iTest++)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest])));
	}
	AssertRel(SUCCEEDED(sortedMap.CommitChanges()));

#ifdef DEBUG
	const unsigned offset = 1;
#else
	const unsigned offset = 601;
#endif

	for (unsigned iTest = 0; iTest < numberOfTest; iTest += offset)
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

	for (unsigned iTest = 0; iTest < numberOfTest; iTest += offset)
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
	AssertRel(SUCCEEDED(sortedMap.CommitChanges()));

	AssertRel(sortedMap.GetItemCount() == 0);
}

TEST_F(AVLTreeTest, DualSortedMapRandomForeach)
{
	BR::DualSortedMap<UINT,UINT> sortedMap;

	for (int iTest = numberOfTest - 1; iTest >= 0; iTest--)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest])));
	}
	AssertRel(SUCCEEDED(sortedMap.CommitChanges()));

#ifdef DEBUG
	const unsigned offset = 1;
#else
	const unsigned offset = 611;
#endif

	ULONG start = BR::Util::Time.GetRawTimeMs();
	ULONG end = BR::Util::Time.GetRawTimeMs();
	ULONG runningTime = 2 * 60 * 1000;
	int *Status = new int[numberOfTest];
	memset(Status, 0, sizeof(int)*numberOfTest);
	for (unsigned iTest = 0; (end - start) < runningTime; iTest++)
	{
		UINT value = -1;
		UINT testIndex = ((UINT)rand()) % numberOfTest;
		if (Status[testIndex] == 0)
		{
			Status[testIndex] = 1;
			AssertRel(SUCCEEDED(sortedMap.Remove(TestValues[testIndex], value)));
			AssertRel(value == TestValues[testIndex]);
		}
		else
		{
			Status[testIndex] = 0;
			AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[testIndex], TestValues[testIndex])));
		}

		if ((iTest + 1) % 10)
		{
			AssertRel(SUCCEEDED(sortedMap.CommitChanges()));
		}


		for (unsigned iTest = 0; iTest < numberOfTest; iTest += offset)
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

		end = BR::Util::Time.GetRawTimeMs();
	}

	delete[] Status;
}



TEST_F(AVLTreeTest, DualSortedMapThread)
{
	BR::DualSortedMap<UINT,UINT> sortedMap;
	int *Status = new int[numberOfTest];
	const int NUM_THREAD = 10;
	std::vector<BR::Thread*> threads;

	memset(Status, 0, sizeof(int)*numberOfTest);

	for (int iTest = numberOfTest - 1; iTest >= 0; iTest--)
	{
		AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[iTest], TestValues[iTest])));
	}
	AssertRel(SUCCEEDED(sortedMap.CommitChanges()));

	
	{
		long latestValue = -1;
		UINT testIndex = ((UINT)rand()) % numberOfTest;
		sortedMap.ForeachOrder(testIndex, 1000, [&](const UINT& key, const UINT& value)
		{
			if (latestValue != -1)
			{
				AssertRel(value <= (UINT)latestValue);
			}
			latestValue = value;
			return true;
		});
	}

	for (int iThread = 0; iThread < NUM_THREAD; iThread++)
	{
		auto thread = new BR::FunctorThread(
			[&](BR::Thread* pThread)
			{
				while (true)
				{
					ULONG loopInterval = pThread->UpdateInterval(10);

					if (pThread->CheckKillEvent(loopInterval))
					{
						// Kill Event signaled
						break;
					}

					long latestValue = -1;
					UINT testIndex = ((UINT)rand()) % numberOfTest;
					sortedMap.ForeachOrder(testIndex, 1000, [&](const UINT& key, const UINT& value)
					{
						if (latestValue != -1)
						{
							AssertRel(value <= (UINT)latestValue);
						}
						latestValue = value;
						return true;
					});
				}
			});

		thread->Start();
		threads.push_back(thread);
	}

	ULONG start = BR::Util::Time.GetRawTimeMs();
	ULONG end = BR::Util::Time.GetRawTimeMs();
#ifdef DEBUG
	ULONG runningTime = 10 * 60 * 1000;
#else
	ULONG runningTime = 30 * 60 * 1000;
#endif
	for (unsigned iTest = numberOfTest; (end - start) < runningTime; iTest++)
	{
		UINT value = -1;
		UINT testIndex = ((UINT)rand()) % numberOfTest;
		if (Status[testIndex] == 0)
		{
			Status[testIndex] = 1;
			AssertRel(SUCCEEDED(sortedMap.Remove(TestValues[testIndex], value)));
			AssertRel(value == TestValues[testIndex]);
		}
		else
		{
			Status[testIndex] = 0;
			AssertRel(SUCCEEDED(sortedMap.Insert(TestValues[testIndex], TestValues[testIndex])));
		}
		std::atomic_thread_fence(std::memory_order_acq_rel);
		if ((iTest + 1) % 10)
		{
			AssertRel(SUCCEEDED(sortedMap.CommitChanges()));
		}

		end = BR::Util::Time.GetRawTimeMs();
	}


	for (int iThread = 0; iThread < NUM_THREAD; iThread++)
	{
		threads[iThread]->Stop(true);
		delete threads[iThread];
	}
	threads.clear();

	delete[] Status;
}

