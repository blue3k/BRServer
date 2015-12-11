// TestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <gtest/gtest.h>
#include "Common/StrUtil.h"
#include "Common/BRRandom.h"
#include "Common/StrFormat.h"
#include "Common/PageQueue.h"
#include "Common/TimeUtil.h"
#include "Common/CircularQueue.h"
#include "Common/Thread.h"
#include "Common/SharedPointer.h"
#include "Common/SharedReferenceManager.h"
#include "ThreadTest.h"

#include "ServerSystem/Task.h"
#include "ServerSystem/Entity.h"


using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;
using namespace ::BR;

using namespace ::BR::Svr;

#ifdef DEBUG
#define TestScale 1
#else
#define TestScale 10
#endif

namespace BR
{

	class WorkingEntity : public SharedObject
	{
	private:
		UINT m_WorkerThreadID;
		UINT m_EntityID;


	public:
		struct TestData {
			CounterType Item;
		};
		TestData *pItem;

		UINT TestID;

		WorkingEntity(UINT entityID)
			: m_WorkerThreadID(0)
			, m_EntityID(entityID)
		{
			pItem = new TestData;
			pItem->Item = 1;
		}

		inline UINT GetWorkerThreadID()                   { return m_WorkerThreadID; }

		HRESULT AssignWorkerThreadID(UINT workerThreadID)
		{
			if (m_WorkerThreadID != 0)
				return E_UNEXPECTED;

			m_WorkerThreadID = workerThreadID;

			return S_OK;
		}

		virtual void Dispose() override
		{
			delete pItem;
			pItem = nullptr;
		}
	};


	class TaskWorkerThread : public Thread
	{
	public:
		static SyncCounter WorkedItems;
		static SyncCounter SkippedWorkedItems;

		TaskWorkerThread()
			: m_TaskWorkerInterval(DurationMS(1))
		{
		}

		~TaskWorkerThread()
		{
		}

		HRESULT PushTask(WeakPointerT<WorkingEntity> taskItem)
		{
			if (taskItem == nullptr)
				return E_INVALIDARG;

			return m_WorkItemQueue.Enqueue(taskItem);
		}

		CounterType GetQueuenWorkCount()
		{
			return m_WorkItemQueue.GetEnqueCount();
		}

		SyncCounter PrintCount;

		virtual void Run() override
		{
			PrintCount = 0;

			while (true)
			{
				auto loopInterval = UpdateInterval(m_TaskWorkerInterval);

				if (CheckKillEvent(loopInterval))
				{
					// Kill Event signaled
					break;
				}

				WeakPointerT<WorkingEntity> workItem;
				while (SUCCEEDED(m_WorkItemQueue.Dequeue(workItem)))
				{
					SharedPointerT<WorkingEntity> workObj;
					workItem.GetSharedPointer(workObj);


					// If disposed skip item
					if (workObj == nullptr)
					{
						SkippedWorkedItems.fetch_add(1,std::memory_order_relaxed);
						continue;
					}

					// Do some works
					workObj->pItem->Item = workObj->GetReferenceCount();
					// let other threads do some job
					const int RandMax = 1000;
					auto randVal = Util::Random.Rand(RandMax);
					if (randVal > ((RandMax * 2) / 3))
					{
						ThisThread::SleepFor(DurationMS(0));
					}
					else
					{
						auto randVal2 = Util::Max(randVal, (unsigned)50);
						for (unsigned int iRand = 0; iRand < randVal; iRand++)
						{
							randVal2 += Util::Random.Rand(100);
						}

						if (((PrintCount.fetch_add(1, std::memory_order_relaxed) + 1) % 10000) == 0)
						{
							printf("Processing %d", randVal2);
						}
					}
					workObj->pItem->Item = workObj->GetReferenceCount();

					WorkedItems.fetch_add(1, std::memory_order_relaxed);
				}
			}
		}

	private:

		// Task worker mnimum interval
		DurationMS m_TaskWorkerInterval;

		// Task queue
		PageQueue<WeakPointerT<WorkingEntity>> m_WorkItemQueue;
	};

	SyncCounter TaskWorkerThread::WorkedItems = 0;
	SyncCounter TaskWorkerThread::SkippedWorkedItems = 0;


	class EntityTaskManager
	{
	private:

		UINT m_NumberOfWorker;
		UINT m_NumberOfTestEntity;

		UINT m_LatestAssignedWorkerID;

		std::vector<TaskWorkerThread*> m_Workers;

		SharedReferenceManager m_ReferenceManager;
		std::unordered_map<UINT, SharedPointerT<WorkingEntity>> m_EntityManager;

	public:

		EntityTaskManager()
			: m_NumberOfWorker(5)
			, m_NumberOfTestEntity(100)
			, m_LatestAssignedWorkerID(0)
		{

		}

		~EntityTaskManager()
		{

		}

		HRESULT InitializeTaskManager()
		{
			for (UINT worker = 0; worker < m_NumberOfWorker; worker++)
			{
				auto pWorker = new TaskWorkerThread;

				m_Workers.push_back(pWorker);

				pWorker->Start();
			}

			// setup test entities
			for (UINT entity = 0; entity < m_NumberOfTestEntity; entity++)
			{
				UINT entityID = entity + 1;
				auto entityPtr = new WorkingEntity(entityID);
				m_ReferenceManager.RegisterSharedObject(entityPtr);
				m_EntityManager.insert(std::make_pair(entityID, entityPtr));
			}

			return S_OK;
		}

		void TerminateTaskManager()
		{
			std::for_each(m_Workers.begin(), m_Workers.end(), [](TaskWorkerThread* pWorker)
			{
				pWorker->Stop(true);

				delete pWorker;
			});

			m_Workers.clear();
		}

		// Enqueue event
		HRESULT TestEnqueueEvent()
		{
			auto entityID = Util::Random.Rand(m_NumberOfTestEntity);
			auto itEntity = m_EntityManager.find(entityID);
			UINT workID = 0;
			SharedPointerT<WorkingEntity> entity;
			if (itEntity == m_EntityManager.end())
			{
				auto newObject = new WorkingEntity(entityID);
				newObject->TestID = entityID;
				m_ReferenceManager.RegisterSharedObject(newObject);

				entity = SharedPointerT<WorkingEntity>(newObject);
				m_EntityManager.insert(std::make_pair(entityID, entity));
			}
			else
			{
				entity = itEntity->second;
			}

			auto isDelete = Util::Random.Rand(100) > 70;
			if (entity->GetWorkerThreadID() == 0)
			{
				// round robin
				m_LatestAssignedWorkerID = (m_LatestAssignedWorkerID + 1) % m_Workers.size();
				entity->AssignWorkerThreadID(m_LatestAssignedWorkerID);
			}

			workID = entity->GetWorkerThreadID();

			WeakPointerT<WorkingEntity> weakPointer;
			weakPointer = entity;

			m_Workers[workID]->PushTask(weakPointer);

			if (isDelete && itEntity != m_EntityManager.end())
			{
				m_EntityManager.erase(itEntity);
			}
			else
			{
				while (m_Workers[workID]->GetQueuenWorkCount() > 100)
				{
					ThisThread::SleepFor(DurationMS(10));
				}
			}

			return S_OK;
		}

		void Update()
		{
			m_ReferenceManager.UpdateReferenceManager();
		}

	};


}



namespace BRTest
{





};


//BR_MEMORYPOOL_IMPLEMENT(BRTest::MemoryTestObject);
//BR_MEMORYPOOL_IMPLEMENT(BRTest::MemoryTestObject2);
//BR_MEMORYPOOL_IMPLEMENT(BRTest::MemoryTestObject3);
//




namespace BRTest
{


BR::EntityTaskManager* entityManager = nullptr;
TEST_F(ThreadTest, EntityTaskManager)
{
	//const INT64 TEST_LENGTH = 999999;
	const INT64 TEST_LENGTH = TestScale * 9999999;

	entityManager = new BR::EntityTaskManager;
	entityManager->InitializeTaskManager();

	for (INT64 ID = 0; ID < TEST_LENGTH; ID++)
	{
		entityManager->TestEnqueueEvent();
		entityManager->Update();
	};

	while ((TaskWorkerThread::WorkedItems + TaskWorkerThread::SkippedWorkedItems) < TEST_LENGTH)
	{
		ThisThread::SleepFor(DurationMS(1000));
		entityManager->Update();
	}

	entityManager->Update();
	entityManager->TerminateTaskManager();
}


TEST_F(ThreadTest, PagedQueue)
{
	const INT64 TEST_LENGTH = TestScale * 514;

	BR::PageQueue<INT64> queue;

	for (INT64 ID = 1; ID < TEST_LENGTH; ID++)
	{
		queue.Enqueue(ID);
	};

	INT64 testID = 1;
	INT64 item;
	while (SUCCEEDED(queue.Dequeue(item)) )
	{
		EXPECT_EQ(item, testID);
		Assert(item == testID);
		testID++;
	}

}


TEST_F(ThreadTest, PagedQueueThreadEnqueue)
{
	const UINT64 TEST_LENGTH = TestScale * 9999999;
	const int NUM_THREAD = 10;

	BR::PageQueue<INT64> queue(128);
	SyncCounter itemCounter;
	CounterType *testArray = new CounterType[TEST_LENGTH];
	memset(testArray, 0, sizeof(CounterType)*TEST_LENGTH);

	for (UINT worker = 0; worker < NUM_THREAD; worker++)
	{
		auto pWorker = new FunctorThread([&](Thread* pThread)
		{
			do
			{
				auto count = itemCounter.fetch_add(1, std::memory_order_relaxed)+1;
				if (count <= TEST_LENGTH)
					queue.Enqueue(count);
			} while (itemCounter < TEST_LENGTH);
		});

		pWorker->Start();
		m_Threads.push_back(pWorker);
	}

	std::for_each(m_Threads.begin(), m_Threads.end(), [](BR::Thread* pThread)
	{
		if (pThread) pThread->Stop(false);
		delete pThread;
	});
	m_Threads.clear();


	INT64 item;
	while (SUCCEEDED(queue.Dequeue(item)))
	{
		AssertRel(testArray[item-1] == 0);
		testArray[item-1] = 1;
	}

	for (int item = 0; item < TEST_LENGTH; item++)
	{
		EXPECT_EQ(1, testArray[item]);
		AssertRel(testArray[item] == 1);
	}

	delete testArray;
}


TEST_F(ThreadTest, PagedQueueThreadEnqueueDequeue)
{
	const UINT64 TEST_LENGTH = TestScale * 999999;
	const int NUM_THREAD = 20;

	BR::PageQueue<INT64> queue(4);
	SyncCounter itemCounter;
	BR::SyncCounter workDone;
	CounterType *testArray = new CounterType[TEST_LENGTH];
	memset(testArray, 0, sizeof(CounterType)*TEST_LENGTH);

	for (UINT worker = 0; worker < NUM_THREAD; worker++)
	{
		auto pWorker = new FunctorThread([&](Thread* pThread)
		{
			do
			{
				auto count = itemCounter.fetch_add(1, std::memory_order_relaxed) + 1;
				if (count <= TEST_LENGTH)
					queue.Enqueue(count);
			} while (itemCounter < TEST_LENGTH);

			workDone.fetch_add(1, std::memory_order_relaxed);
		});

		pWorker->Start();
		m_Threads.push_back(pWorker);
	}



	INT64 item;
	UINT64 readCount = 0;
	while (readCount < TEST_LENGTH || workDone.load(std::memory_order_relaxed) < NUM_THREAD)
	{
		if (SUCCEEDED(queue.Dequeue(item)))
		{
			item--;
			AssertRel(testArray[item] == 0);
			testArray[item] = 1;
			readCount++;
		}
	}


	std::for_each(m_Threads.begin(), m_Threads.end(), [](BR::Thread* pThread)
	{
		if (pThread) pThread->Stop(false);
		delete pThread;
	});
	m_Threads.clear();


	for (int item = 0; item < TEST_LENGTH; item++)
	{
		EXPECT_EQ(1, testArray[item]);
		AssertRel(testArray[item] == 1);
	}

	delete testArray;
}

TEST_F(ThreadTest, PagedQueueThreadEnqueueThreadDequeue)
{
	const UINT64 TEST_LENGTH = TestScale * 99999;
	const int NUM_THREAD = 20;

	BR::PageQueue<INT64> queue(4);
	SyncCounter itemCounter;
	BR::SyncCounter workDone;
	SyncCounter *testArray = new SyncCounter[TEST_LENGTH];
	memset(testArray, 0, sizeof(SyncCounter)*TEST_LENGTH);

	for (UINT worker = 0; worker < NUM_THREAD; worker++)
	{
		auto pWorker = new FunctorThread([&](Thread* pThread)
		{
			do
			{
				auto count = itemCounter.fetch_add(1, std::memory_order_relaxed) + 1;
				if (count <= TEST_LENGTH)
					queue.Enqueue(count);
			} while (itemCounter < TEST_LENGTH);

			workDone.fetch_add(1, std::memory_order_relaxed);
		});

		pWorker->Start();
		m_Threads.push_back(pWorker);
	}

	SyncCounter readCount = 0;
	for (UINT worker = 0; worker < NUM_THREAD; worker++)
	{
		auto pWorker = new FunctorThread([&](Thread* pThread)
		{
			INT64 item;
			auto readOrder = readCount.fetch_add(1,std::memory_order_relaxed);
			while (readOrder < TEST_LENGTH)
			{
				if (SUCCEEDED(queue.DequeueMT(item)))
				{
					item--;
					CounterType expected = 0;
					while (!testArray[item].compare_exchange_weak(expected, 1, std::memory_order_relaxed, std::memory_order_relaxed))
					{
						AssertRel(expected == 0);
						expected = 0;
					}
				}
				readOrder = readCount.fetch_add(1, std::memory_order_relaxed);
			}

			workDone.fetch_add(1, std::memory_order_relaxed);
		});

		pWorker->Start();
		m_Threads.push_back(pWorker);
	}


	std::for_each(m_Threads.begin(), m_Threads.end(), [](BR::Thread* pThread)
	{
		if (pThread) pThread->Stop(false);
		delete pThread;
	});
	m_Threads.clear();


	for (int item = 0; item < TEST_LENGTH; item++)
	{
		EXPECT_EQ(1, testArray[item]);
		AssertRel(testArray[item] == 1);
	}

	delete testArray;
}


TEST_F(ThreadTest, PagedQueue_PerformanceCompare_PageQueue)
{
	const UINT64 TEST_LENGTH = TestScale * 999999;
	const int NUM_THREAD = 10;

	std::vector<BR::Thread*> threads;
	BR::PageQueue<INT64> queue(1024);
	BR::SyncCounter itemCounter;
	BR::SyncCounter workDone;
	CounterType *testArray = new CounterType[TEST_LENGTH];
	memset(testArray, 0, sizeof(CounterType)*TEST_LENGTH);


	auto start = BR::Util::Time.GetRawTimeMs();


	for (UINT worker = 0; worker < NUM_THREAD; worker++)
	{
		auto pWorker = new BR::FunctorThread([&](Thread* pThread)
		{
			do
			{
				auto count = itemCounter.fetch_add(1, std::memory_order_relaxed) + 1;
				if (count <= TEST_LENGTH)
					AssertRel(SUCCEEDED(queue.Enqueue(count)));
			} while (itemCounter < TEST_LENGTH);

			workDone.fetch_add(1);
		});

		pWorker->Start();
		threads.push_back(pWorker);
	}


	INT64 item;
	UINT64 readCount = 0;
	while (readCount < TEST_LENGTH || workDone.load(std::memory_order_relaxed) < NUM_THREAD)
	{
		if (SUCCEEDED(queue.Dequeue(item)))
		{
			item--;
			AssertRel(testArray[item] == 0);
			testArray[item] = 1;
			readCount++;
		}
	}

	std::for_each(threads.begin(), threads.end(), [](BR::Thread* pThread)
	{
		if (pThread) pThread->Stop(false);
		delete pThread;
	});
	threads.clear();

	auto end = BR::Util::Time.GetRawTimeMs();
	printf("PageQueue Execution Time:      %15d\n", (end - start).count());

	delete testArray;
}

TEST_F(ThreadTest, PagedQueue_PerformanceCompare_Concurrent)
{
	const UINT64 TEST_LENGTH = TestScale * 999999;
	const int NUM_THREAD = 10;

	std::vector<BR::Thread*> threads;
	Concurrency::concurrent_queue<INT64> queue;
	BR::SyncCounter itemCounter;
	BR::SyncCounter workDone;
	CounterType *testArray = new CounterType[TEST_LENGTH];
	memset(testArray, 0, sizeof(CounterType)*TEST_LENGTH);


	auto start = BR::Util::Time.GetRawTimeMs();


	for (UINT worker = 0; worker < NUM_THREAD; worker++)
	{
		auto pWorker = new BR::FunctorThread([&](Thread* pThread)
		{
			do
			{
				auto count = itemCounter.fetch_add(1, std::memory_order_relaxed) + 1;
				if (count <= TEST_LENGTH)
					queue.push(count);
			} while (itemCounter < TEST_LENGTH);

			workDone.fetch_add(1);
		});

		pWorker->Start();
		threads.push_back(pWorker);
	}


	INT64 item;
	UINT64 readCount = 0;
	while (readCount < TEST_LENGTH || workDone.load(std::memory_order_relaxed) < NUM_THREAD)
	{
		if (queue.try_pop(item))
		{
			item--;
			AssertRel(testArray[item] == 0);
			testArray[item] = 1;
			readCount++;
		}
	}

	std::for_each(threads.begin(), threads.end(), [](BR::Thread* pThread)
	{
		if (pThread) pThread->Stop(false);
		delete pThread;
	});
	threads.clear();

	auto end = BR::Util::Time.GetRawTimeMs();
	printf("concurrent_queue Execution Time: %15d\n", (end - start).count());

	delete testArray;
}





}  // namespace BRTest
