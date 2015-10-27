// TestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <gtest/gtest.h>
#include "Common/StrUtil.h"
#include "Common/StrFormat.h"
#include "Common/SharedPointer.h"
#include "HashTableTest.h"
#include "Common/HashTable2.h"
#include "Common/TimeUtil.h"


using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;
using namespace ::BR;

#ifdef DEBUG
#define TestScale 1
#else
#define TestScale 10
#endif

namespace BRTest
{

const INT64 TEST_COUNT = TestScale * 99999;
const INT64 MAX_NUMBER = TestScale * TestScale * TestScale * TestScale * TestScale * TestScale * 1000;

struct TestMapNode2
{
public:
	INT64 Value;
};


class TestMapNodeShared : public BR::SharedObject
{
public:
	volatile INT64 Value;
};

TEST_F(HashTableTest, HashTable2_NonUnique)
{
	

	typedef BR::Hash::HashTable2<	INT64, TestMapNode2*,
									BR::Hash::NonUniqueKeyTrait, BR::ThreadSyncTraitNone
									> TestTableType;

	TestTableType TestMap;
	std::unordered_map<int,int> checkSet;


	for (INT64 iTest = 0; iTest < TEST_COUNT; iTest++)
	{
		int value = rand() % MAX_NUMBER;
		auto itFound = checkSet.find(value);

		TestMapNode2 *pNewNode = new TestMapNode2;
		memset( pNewNode, 0, sizeof(TestMapNode2) );
		pNewNode->Value = value;
		if( itFound == checkSet.end() )
		{
			checkSet.insert( std::make_pair(value,1) );
		}
		else
		{
			int Count = checkSet[value];
			checkSet[value] = Count+1;
		}
		HRESULT hrRes = TestMap.Insert(pNewNode->Value, pNewNode);
		AssertRel(SUCCEEDED(hrRes));
		AssertRel(SUCCEEDED(hrRes));
	}


	for (int iTest = 0; iTest < TEST_COUNT; iTest++)
	{
		int value = rand() % MAX_NUMBER;
		auto itFound = checkSet.find(value);
		TestMapNode2 *pNode = nullptr;

		if( itFound == checkSet.end() )
		{
			AssertRel(FAILED(TestMap.Find(value, pNode)));
		}
		else
		{
			AssertRel(SUCCEEDED(TestMap.Find(value, pNode)));
			AssertRel(value == pNode->Value);
			EXPECT_EQ(value, pNode->Value);
		}
	}

	auto itCheck = checkSet.begin();
	for( ; itCheck != checkSet.end(); itCheck++ )
	{
		HRESULT hr = S_OK;
		TestMapNode2* pNode = nullptr;
		hr = TestMap.Find( itCheck->first, pNode);
		AssertRel(SUCCEEDED(hr));

		if( FAILED(hr) )
			continue;
		TestMapNode2* pErased = nullptr;
		hr = TestMap.Erase( pNode->Value, pErased );
		AssertRel(pNode == pErased);
		AssertRel(SUCCEEDED(hr));
		if( FAILED(hr) )
			continue;
		delete pNode;
	}
}



TEST_F(HashTableTest, HashTable2_Unique)
{
	typedef BR::Hash::HashTable2<	INT64, TestMapNode2*,
										BR::Hash::UniqueKeyTrait, BR::ThreadSyncTraitNone
										> TestTableType;

	TestTableType TestMap;
	std::unordered_set<int> checkSet;
	TestMapNode2* pNode = nullptr;

	for (INT64 iTest = 0; iTest < TEST_COUNT; iTest++)
	{
		int value = rand() % MAX_NUMBER;
		auto itFound = checkSet.find(value);

		if( itFound == checkSet.end() )
		{
			TestMapNode2 *pNewNode = new TestMapNode2;
			memset( pNewNode, 0, sizeof(TestMapNode2) );
			pNewNode->Value = value;
			checkSet.insert( value );
			AssertRel(SUCCEEDED(TestMap.Insert(value, pNewNode)));
		}
	}

	for (INT64 iTest = 0; iTest < TEST_COUNT; iTest++)
	{
		int value = rand() % MAX_NUMBER;
		auto itFound = checkSet.find(value);

		if( itFound == checkSet.end() )
		{
			AssertRel(FAILED(TestMap.Find(value, pNode)));
		}
		else
		{
			AssertRel(SUCCEEDED(TestMap.Find(value, pNode)));
		}
	}

	auto itCheck = checkSet.begin();
	for( ; itCheck != checkSet.end(); itCheck++ )
	{
		AssertRel(SUCCEEDED(TestMap.Find(*itCheck, pNode)));
		TestMapNode2 *pErased;
		AssertRel(SUCCEEDED(TestMap.Erase(pNode->Value, pErased)));
		AssertRel(pNode == pErased);
		delete pNode;
	}

}


TEST_F(HashTableTest, HashTable2_UniqueMT)
{
	const int READ_THREAD_COUNT = 10;


	SyncCounter numberOfItems;
	
	typedef BR::Hash::HashTable2<	INT64, SharedPointerT<TestMapNodeShared>,
										BR::Hash::UniqueKeyTrait, BR::ThreadSyncTraitMT
										> TestTableType;

	Concurrency::concurrent_unordered_map<INT64, SharedPointerT<TestMapNodeShared>> m_TestMap;
	TestTableType TestMap;
	TestMapNodeShared* pNode = nullptr;


	for( INT64 ID = 0; ID < READ_THREAD_COUNT; ID++ )
	{
		FunctorThread *pTask = new BR::FunctorThread([&, ID](Thread *pThread)
		{
			while (!pThread->CheckKillEvent(5))
			{
				int value = rand() % MAX_NUMBER;
				SharedPointerT<TestMapNodeShared> pFound = nullptr;

				if( SUCCEEDED(TestMap.Find( value, pFound )) )
				{
					ThisThread::SleepFor(DurationMS(10));
					AssertRel(value == pFound->Value);
					EXPECT_EQ(value, pFound->Value);
				}
				else
				{
				}
			}

		} );
		pTask->Start();
		m_Threads.push_back(pTask);
	};


	for (INT64 i = 0; i<(TEST_COUNT); i++)
	{
		INT64 value = (rand() % MAX_NUMBER) + 1;
		TestMapNodeShared *pNewNode = new TestMapNodeShared;
		pNewNode->Value = value;
		if (SUCCEEDED(TestMap.Insert(value, pNewNode)))
		{
			m_TestMap.insert(std::make_pair(value, pNewNode));
			numberOfItems.fetch_add(1, std::memory_order_relaxed);
		}
		else
		{
			//delete pNewNode;
		}
	}


	ULONG start = BR::Util::Time.GetRawTimeMs();
	ULONG end = BR::Util::Time.GetRawTimeMs();
#ifdef DEBUG
	ULONG runningTime = 1 * 60 * 1000;
#else
	ULONG runningTime = 5 * 60 * 1000;
#endif
	for (; (end - start) < runningTime;)
	{
		INT64 value = (rand() % MAX_NUMBER) + 1;
		INT64 operation = rand() % MAX_NUMBER;

		SharedPointerT<TestMapNodeShared> pFound = nullptr;

		if (SUCCEEDED(TestMap.Find(value, pFound)))
		{
			EXPECT_EQ(value, pFound->Value);
			AssertRel(value == pFound->Value);
			SharedPointerT<TestMapNodeShared> pErased = nullptr;
			if (SUCCEEDED(TestMap.Erase((INT64)pFound->Value, pErased)))
			{
				SharedPointerT<TestMapNodeShared> pErased2 = nullptr;
				AssertRel(FAILED(TestMap.Erase((INT64)pFound->Value, pErased2)));
				AssertRel(pErased == pFound);
				AssertRel(pErased->Value == pFound->Value);

				auto itFound = m_TestMap.find(value);
				AssertRel(itFound->second == pErased);
				m_TestMap.unsafe_erase(value);

				numberOfItems.fetch_sub(1, std::memory_order_relaxed);
			}
			else
			{
				//AssertRel(false);
				TestMap.Erase((INT64)pFound->Value, pErased);
			}
		}
		else
		{
			TestMapNodeShared *pNewNode = new TestMapNodeShared;
			pNewNode->Value = value;
			if (SUCCEEDED(TestMap.Insert(value, pNewNode)))
			{
				m_TestMap.insert(std::make_pair(value, pNewNode));
				numberOfItems.fetch_add(1, std::memory_order_relaxed);
			}
			else
			{
				//delete pNewNode;
			}
		}

		end = BR::Util::Time.GetRawTimeMs();
	}


	ThisThread::SleepFor(DurationMS(1000));

	StopAllThread();

	AssertRel(TestMap.GetItemCount() == numberOfItems.load(std::memory_order_relaxed));

	TestMap.Clear();
	AssertRel(TestMap.GetItemCount() == 0);
	EXPECT_EQ(0, TestMap.GetItemCount());
}


TEST_F(HashTableTest, HashTable2_UniqueWriteMT)
{
	const int READ_THREAD_COUNT = 10;
	const int WRITE_THREAD_COUNT = 10;


	SyncCounter workCounterRead, workCounterWrite, numberOfItems;

	typedef BR::Hash::HashTable2<	INT64, SharedPointerT<TestMapNodeShared>,
		BR::Hash::UniqueKeyTrait, BR::ThreadSyncTraitMT
	> TestTableType;


	TestTableType TestMap(1);
	TestMapNodeShared* pNode = nullptr;


	for (INT64 ID = 0; ID < READ_THREAD_COUNT; ID++)
	{
		FunctorThread *pTask = new BR::FunctorThread([&, ID](Thread *pThread)
		{
			workCounterRead.fetch_add(1, std::memory_order_relaxed);

			while (!pThread->CheckKillEvent(5))
			{
				int value = rand() % MAX_NUMBER;
				SharedPointerT<TestMapNodeShared> pFound = nullptr;

				if (SUCCEEDED(TestMap.Find(value, pFound)))
				{
					ThisThread::SleepFor(DurationMS(10));
					AssertRel(value == pFound->Value);
					EXPECT_EQ(value, pFound->Value);
				}
				else
				{
				}
			}

			workCounterRead.fetch_sub(1, std::memory_order_relaxed);

		});
		pTask->Start();
		m_Threads.push_back(pTask);
	};


	for (INT64 i = 0; i<(TEST_COUNT); i++)
	{
		INT64 value = (rand() % MAX_NUMBER) + 1;
		TestMapNodeShared *pNewNode = new TestMapNodeShared;
		pNewNode->Value = value;
		if (SUCCEEDED(TestMap.Insert(value, pNewNode)))
		{
			numberOfItems.fetch_add(1, std::memory_order_relaxed);
		}
		else
		{
			//delete pNewNode;
		}
	}


	ULONG start = BR::Util::Time.GetRawTimeMs();
	ULONG end = BR::Util::Time.GetRawTimeMs();
#ifdef DEBUG
	ULONG runningTime = 5 * 1000;
#else
	//ULONG runningTime = 30 * 60 * 1000;
	ULONG runningTime = 60 * 1000;
#endif


	for (int iThread = 0; iThread < WRITE_THREAD_COUNT; iThread++)
	{
		FunctorThread *pTask = new BR::FunctorThread([&](Thread *pThread)
		{
			workCounterWrite.fetch_add(1, std::memory_order_acquire);

			while (!pThread->CheckKillEvent(0))
			{
				INT64 value = (rand() % MAX_NUMBER) + 1;
				INT64 operation = rand() % MAX_NUMBER;

				SharedPointerT<TestMapNodeShared> pFound = nullptr;

				if (SUCCEEDED(TestMap.Find(value, pFound)))
				{
					AssertRel(pFound != nullptr);
					EXPECT_EQ(value, pFound->Value);
					AssertRel(value == pFound->Value);
					SharedPointerT<TestMapNodeShared> pErased = nullptr;
					if (SUCCEEDED(TestMap.Erase((INT64)pFound->Value, pErased)))
					{
						AssertRel(pErased->Value == pFound->Value);
						numberOfItems.fetch_sub(1, std::memory_order_relaxed);
					}
					else
					{
						//AssertRel(false);
						//TestMap.Erase(pFound->Value, pErased);
					}
				}
				else
				{
					TestMapNodeShared *pNewNode = new TestMapNodeShared;
					pNewNode->Value = value;
					if (SUCCEEDED(TestMap.Insert(value, pNewNode)))
					{
						numberOfItems.fetch_add(1, std::memory_order_relaxed);
					}
				}

			}

			workCounterWrite.fetch_sub(1, std::memory_order_release);
		});
		pTask->Start();
		m_Threads.push_back(pTask);
	}


	ThisThread::SleepFor(DurationMS(runningTime));

	StopAllThread();

	AssertRel(TestMap.GetItemCount() == numberOfItems.load(std::memory_order_relaxed));

	TestMap.Clear();
	AssertRel(TestMap.GetItemCount() == 0);
	EXPECT_EQ(0, TestMap.GetItemCount());
}


TEST_F(HashTableTest, HashTable2_PerfRead)
{
	const int READ_THREAD_COUNT = 10;

	SyncCounter numberOfItems;
	SyncCounter readCount(0), writeCount(0);

	typedef BR::Hash::HashTable2<	INT64, SharedPointerT<TestMapNodeShared>,
		BR::Hash::UniqueKeyTrait, BR::ThreadSyncTraitMT
	> TestTableType;

	Concurrency::concurrent_unordered_map<INT64, SharedPointerT<TestMapNodeShared>> m_TestMap;
	TestTableType TestMap(1024);
	TestMapNodeShared* pNode = nullptr;


	for (INT64 ID = 0; ID < READ_THREAD_COUNT; ID++)
	{
		FunctorThread *pTask = new BR::FunctorThread([&, ID](Thread *pThread)
		{
			while (!pThread->CheckKillEvent(0))
			{
				int value = rand() % MAX_NUMBER;
				SharedPointerT<TestMapNodeShared> pFound = nullptr;

				if (SUCCEEDED(TestMap.Find(value, pFound)))
				{
					AssertRel(value == pFound->Value);
					EXPECT_EQ(value, pFound->Value);
				}
				else
				{
				}
				readCount.fetch_add(1, std::memory_order_relaxed);
			}

		});
		pTask->Start();
		m_Threads.push_back(pTask);
	};


	for (INT64 i = 0; i<(TEST_COUNT); i++)
	{
		int value = rand() % MAX_NUMBER;
		TestMapNodeShared *pNewNode = new TestMapNodeShared;
		pNewNode->Value = value;
		if (SUCCEEDED(TestMap.Insert(value, pNewNode)))
		{
			m_TestMap.insert(std::make_pair(value, pNewNode));
			numberOfItems.fetch_add(1, std::memory_order_relaxed);
		}
		else
		{
			//delete pNewNode;
		}
		writeCount.fetch_add(1, std::memory_order_relaxed);
	}


	ULONG start = BR::Util::Time.GetRawTimeMs();
	ULONG end = BR::Util::Time.GetRawTimeMs();
#ifdef DEBUG
	ULONG runningTime = 1 * 60 * 1000;
#else
	ULONG runningTime = 10 * 60 * 1000;
#endif
	for (; (end - start) < runningTime;)
	{
		INT64 value = rand() % MAX_NUMBER;
		INT64 operation = rand() % MAX_NUMBER;

		SharedPointerT<TestMapNodeShared> pFound = nullptr;

		if (SUCCEEDED(TestMap.Find(value, pFound)))
		{
			AssertRel(pFound != nullptr);
			EXPECT_EQ(value, pFound->Value);
			AssertRel(value == pFound->Value);
			SharedPointerT<TestMapNodeShared> pErased = nullptr;
			if (SUCCEEDED(TestMap.Erase((INT64)pFound->Value, pErased)))
			{
				SharedPointerT<TestMapNodeShared> pErased2 = nullptr;
				AssertRel(FAILED(TestMap.Erase((INT64)pFound->Value, pErased2)));
				AssertRel(pErased == pFound);
				AssertRel(pErased->Value == pFound->Value);

				auto itFound = m_TestMap.find(value);
				AssertRel(itFound->second == pErased);
				m_TestMap.unsafe_erase(value);

				numberOfItems.fetch_sub(1, std::memory_order_relaxed);
			}
			else
			{
				//AssertRel(false);
				TestMap.Erase((INT64)pFound->Value, pErased);
			}
		}
		else
		{
			TestMapNodeShared *pNewNode = new TestMapNodeShared;
			pNewNode->Value = value;
			if (SUCCEEDED(TestMap.Insert(value, pNewNode)))
			{
				m_TestMap.insert(std::make_pair(value, pNewNode));
				numberOfItems.fetch_add(1, std::memory_order_relaxed);
			}
			else
			{
				//delete pNewNode;
			}
		}

		writeCount.fetch_add(1, std::memory_order_relaxed);
		end = BR::Util::Time.GetRawTimeMs();
	}


	ThisThread::SleepFor(DurationMS(1000));

	StopAllThread();

	AssertRel(TestMap.GetItemCount() == numberOfItems.load(std::memory_order_relaxed));

	TestMap.Clear();
	AssertRel(TestMap.GetItemCount() == 0);
	EXPECT_EQ(0, TestMap.GetItemCount());

	printf("Read: %llu, Write: %llu\n", readCount.load(std::memory_order_acquire), writeCount.load(std::memory_order_acquire));
}


TEST_F(HashTableTest, HashTable2_PerfReadWriteMT)
{
	const int READ_THREAD_COUNT = 8;
	const int WRITE_THREAD_COUNT = 8;


	SyncCounter numberOfItems;
	SyncCounter readCount(0), writeCount(0);

	typedef BR::Hash::HashTable2<	INT64, SharedPointerT<TestMapNodeShared>,
		BR::Hash::UniqueKeyTrait, BR::ThreadSyncTraitMT
	> TestTableType;


	TestTableType TestMap(3);
	TestMapNodeShared* pNode = nullptr;


	for (INT64 ID = 0; ID < READ_THREAD_COUNT; ID++)
	{
		FunctorThread *pTask = new BR::FunctorThread([&, ID](Thread *pThread)
		{
			while (!pThread->CheckKillEvent(0))
			{
				int value = rand() % MAX_NUMBER;
				SharedPointerT<TestMapNodeShared> pFound = nullptr;

				if (SUCCEEDED(TestMap.Find(value, pFound)))
				{
					AssertRel(value == pFound->Value);
				}
				else
				{
				}
				readCount.fetch_add(1, std::memory_order_relaxed);
			}

		});
		pTask->Start();
		m_Threads.push_back(pTask);
	};


	for (INT64 i = 0; i<(TEST_COUNT); i++)
	{
		int value = rand() % MAX_NUMBER;
		TestMapNodeShared *pNewNode = new TestMapNodeShared;
		pNewNode->Value = value;
		if (SUCCEEDED(TestMap.Insert(value, pNewNode)))
		{
			numberOfItems.fetch_add(1, std::memory_order_relaxed);
		}
		else
		{
			//delete pNewNode;
		}
		writeCount.fetch_add(1, std::memory_order_relaxed);
	}


	ULONG start = BR::Util::Time.GetRawTimeMs();
	ULONG end = BR::Util::Time.GetRawTimeMs();
#ifdef DEBUG
	ULONG runningTime = 1 * 60 * 1000;
#else
	ULONG runningTime = 10 * 60 * 1000;
#endif


	for (int iThread = 0; iThread < WRITE_THREAD_COUNT; iThread++)
	{
		FunctorThread *pTask = new BR::FunctorThread([&](Thread *pThread)
		{
			while (!pThread->CheckKillEvent(0))
			{
				INT64 value = rand() % MAX_NUMBER;
				INT64 operation = rand() % MAX_NUMBER;

				SharedPointerT<TestMapNodeShared> pFound = nullptr;

				if (SUCCEEDED(TestMap.Find(value, pFound)))
				{
					EXPECT_EQ(value, pFound->Value);
					AssertRel(value == pFound->Value);
					SharedPointerT<TestMapNodeShared> pErased = nullptr;
					if (SUCCEEDED(TestMap.Erase((INT64)pFound->Value, pErased)))
					{
						AssertRel(pErased->Value == pFound->Value);
						numberOfItems.fetch_sub(1, std::memory_order_relaxed);
					}
					else
					{
						//AssertRel(false);
						//TestMap.Erase(pFound->Value, pErased);
					}
				}
				else
				{
					TestMapNodeShared *pNewNode = new TestMapNodeShared;
					pNewNode->Value = value;
					if (SUCCEEDED(TestMap.Insert(value, pNewNode)))
					{
						numberOfItems.fetch_add(1, std::memory_order_relaxed);
					}
				}
				writeCount.fetch_add(1, std::memory_order_relaxed);
			}
		});
		pTask->Start();
		m_Threads.push_back(pTask);
	}



	ThisThread::SleepFor(DurationMS(runningTime));

	StopAllThread();

	AssertRel(TestMap.GetItemCount() == numberOfItems.load(std::memory_order_relaxed));

	TestMap.Clear();
	AssertRel(TestMap.GetItemCount() == 0);
	EXPECT_EQ(0, TestMap.GetItemCount());

	printf("Read: %llu, Write: %llu\n", readCount.load(std::memory_order_acquire), writeCount.load(std::memory_order_acquire));
}


}  // namespace BRTest
