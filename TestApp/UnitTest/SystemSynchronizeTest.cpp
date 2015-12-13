// TestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <gtest/gtest.h>
#include "Common/StrUtil.h"
#include "Common/BrRandom.h"
#include "Common/StrFormat.h"
#include "Common/PageQueue.h"
#include "Common/TimeUtil.h"
#include "Common/CircularQueue.h"
#include "Common/Thread.h"
#include "Common/SharedPointer.h"
#include "Common/SharedReferenceManager.h"
#include "SystemSynchronizeTest.h"

#include "Common/SystemSynchronization.h"


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


TEST_F(SystemSynchronizationTest, Mutex)
{
	const UINT64 TEST_LENGTH = TestScale * 999999;
	const INT64 NUM_THREAD = 10;

	SyncCounter workerCounter(0);
	std::atomic<int> testData(0);
	std::atomic<int> testIndex(0);
	BR::Mutex dataLock;

	for (UINT worker = 0; worker < NUM_THREAD; worker++)
	{
		auto pWorker = new FunctorThread([&dataLock, &testData, &testIndex, &workerCounter, worker, TEST_LENGTH](Thread* pThread)
		{
			workerCounter.fetch_add(1, std::memory_order_relaxed);
			for (UINT64 iTest = 0; iTest < TEST_LENGTH; iTest++)
			{
				BR::MutexScopeLock scopeLock(dataLock);
				auto read = testData.load(std::memory_order_acquire);
				testData.store(read + 1, std::memory_order_release);
				auto indexData = testIndex.fetch_add(1, std::memory_order_relaxed);
				AssertRel(indexData == read);
			}

			workerCounter.fetch_sub(1, std::memory_order_relaxed);
		});

		pWorker->Start();
		m_Threads.push_back(pWorker);
	}

	ThisThread::SleepFor(DurationMS(10));

	// wait writer threads
	while (workerCounter.load(std::memory_order_relaxed) > 0)
	{
		ThisThread::SleepFor(DurationMS(1000));
	}
}



TEST_F(SystemSynchronizationTest, CriticalSection)
{
	const UINT64 TEST_LENGTH = TestScale * 999999;
	const INT64 NUM_THREAD = 10;

	SyncCounter workerCounter(0);
	std::atomic<int> testData(0);
	std::atomic<int> testIndex(0);
	BR::CriticalSection dataLock;

	for (UINT worker = 0; worker < NUM_THREAD; worker++)
	{
		auto pWorker = new FunctorThread([&dataLock, &testData, &testIndex, &workerCounter, worker, TEST_LENGTH](Thread* pThread)
		{
			workerCounter.fetch_add(1, std::memory_order_relaxed);
			for (UINT64 iTest = 0; iTest < TEST_LENGTH; iTest++)
			{
				BR::MutexScopeLock scopeLock(dataLock);
				auto read = testData.load(std::memory_order_acquire);
				testData.store(read + 1, std::memory_order_release);
				auto indexData = testIndex.fetch_add(1, std::memory_order_relaxed);
				AssertRel(indexData == read);
			}

			workerCounter.fetch_sub(1, std::memory_order_relaxed);
		});

		pWorker->Start();
		m_Threads.push_back(pWorker);
	}

	ThisThread::SleepFor(DurationMS(10));

	// wait writer threads
	while (workerCounter.load(std::memory_order_relaxed) > 0)
	{
		ThisThread::SleepFor(DurationMS(1000));
	}
}

TEST_F(SystemSynchronizationTest, Event)
{
	const UINT64 TEST_LENGTH = TestScale * 999999;
	const INT64 NUM_THREAD = 10;

	SyncCounter workerCounter(0);
	std::atomic<int> testData(0);
	std::atomic<int> testIndex(0);
	BR::Event dataEvent, workDoneEvent;

	dataEvent.Reset();
	workDoneEvent.Reset();

	for (UINT worker = 0; worker < NUM_THREAD; worker++)
	{
		auto pWorker = new FunctorThread([&dataEvent, &workDoneEvent, &testData, &testIndex, &workerCounter, worker, TEST_LENGTH](Thread* pThread)
		{
			workerCounter.fetch_add(1, std::memory_order_relaxed);
			for (UINT64 iTest = 0; iTest < TEST_LENGTH; iTest++)
			{
				while (!dataEvent.WaitEvent(-1));

				auto read = testData.load(std::memory_order_acquire);
				testData.store(read + 1, std::memory_order_release);
				auto indexData = testIndex.load(std::memory_order_relaxed);
				AssertRel(indexData == read);

				workDoneEvent.Set();
			}

			workerCounter.fetch_sub(1, std::memory_order_relaxed);
		});

		pWorker->Start();
		m_Threads.push_back(pWorker);
	}

	ThisThread::SleepFor(DurationMS(10));


	for (INT64 iTest = 0; iTest < (TEST_LENGTH*NUM_THREAD); iTest++)
	{
		dataEvent.Set();
		while (!workDoneEvent.WaitEvent(-1));

		auto read = testData.load(std::memory_order_acquire);
		auto indexData = testIndex.fetch_add(1, std::memory_order_relaxed) + 1;
		AssertRel(indexData == read);
	}


	// wait writer threads
	while (workerCounter.load(std::memory_order_relaxed) > 0)
	{
		ThisThread::SleepFor(DurationMS(1000));
	}
}



}  // namespace BRTest

