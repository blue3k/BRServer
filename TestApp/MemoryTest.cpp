// TestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <gtest/gtest.h>
#include "Common/StrUtil.h"
#include "Common/BRRandom.h"
#include "Common/StrFormat.h"
#include "Common/PageQueue.h"
#include "Common/CircularQueue.h"
#include "Common/TimeUtil.h"
#include "MemoryTest.h"



using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;
using namespace ::BR;


namespace BRTest
{


class MemoryTestObject : public BR::MemoryPoolObject<MemoryTestObject>
{
public:

	INT64 Checksum;
	INT64 WorkerID;
	INT64 Index;

};



class MemoryTestObject2 : public BR::MemoryPoolObject<MemoryTestObject2>
{
public:

	INT64 Checksum;
	INT64 WorkerID;
	INT64 Index;

};


class MemoryTestObject3 : public BR::MemoryPoolObject<MemoryTestObject3>
{
public:

	INT64 Checksum;
	INT64 WorkerID;
	INT64 Index;

};





};


BR_MEMORYPOOL_IMPLEMENT(BRTest::MemoryTestObject);
BR_MEMORYPOOL_IMPLEMENT(BRTest::MemoryTestObject2);
BR_MEMORYPOOL_IMPLEMENT(BRTest::MemoryTestObject3);





namespace BRTest
{


TEST_F(MemoryTest, PagePool)
{
	const INT64 TEST_LENGTH	=		99999;
	const int PUSH_LIMIT	=		1000;
	const int ALLOC_THREAD_COUNT	=	20;
	const int FREE_THREAD_COUNT		=	1;

	BR::PageQueue<void*> Allocated( 128 );
	BR::PagePool Pool( 10 );
	BR::PageAllocator PageAloc( 10 );
	SyncCounter lPusherCount = 0;

	void *pPtr = NULL;
	EXPECT_HRESULT_SUCCEEDED( PageAloc.Alloc( pPtr ) );
	int iPageSize1 = (int)PageAloc.GetPageSize();
	memset( pPtr, 0, PageAloc.GetPageSize() );
	EXPECT_HRESULT_SUCCEEDED( PageAloc.Free( pPtr ) );

	EXPECT_HRESULT_SUCCEEDED( Pool.Alloc( pPtr ) );
	int iPageSize = (int)Pool.GetPageSize();
	memset( pPtr, 0, Pool.GetPageSize() );
	EXPECT_HRESULT_SUCCEEDED( Pool.Free( pPtr ) );
	Pool.Clear();

	for( INT64 ID = 0; ID < ALLOC_THREAD_COUNT; ID++ )
	{
		auto pTask = new BR::FunctorThread([&, ID](Thread* pThread)
		{
			lPusherCount.fetch_add(1,std::memory_order_relaxed);

			for(int i=0; i<TEST_LENGTH; i++)
			{
				void *pPtr = NULL;
				HRESULT hr = Pool.Alloc( pPtr );
				EXPECT_HRESULT_SUCCEEDED( hr );

				if( SUCCEEDED(hr) )
				{
					iPageSize = (int)Pool.GetPageSize();

					memset( pPtr, 0, iPageSize );

					_WriteBarrier();

					Allocated.Enqueue( pPtr );
				}
			}

			lPusherCount.fetch_sub(1, std::memory_order_relaxed);
		} );

		pTask->Start();
		m_Threads.push_back(pTask);

	};

	for( INT64 ID = 0; ID < FREE_THREAD_COUNT; ID++ )
	{
		auto pTask = new BR::FunctorThread([&, ID](Thread* pThread)
		{
			while(1)
			{
				void* pItem = NULL;
				if( (Allocated.Dequeue( pItem ) == S_OK) 
					&& pItem != NULL )
				{
					iPageSize = (int)Pool.GetPageSize();

					memset( pItem, -1, iPageSize );

					EXPECT_HRESULT_SUCCEEDED( Pool.Free( pItem ) );
				}
				else
				{
					if(lPusherCount == 0)
						break;

					ThisThread::SleepFor(DurationMS(10));
				}
			}
		});

		pTask->Start();
		m_Threads.push_back(pTask);
	};


	ThisThread::SleepFor(DurationMS(1000));

	while( lPusherCount != 0 )
	{
		ThisThread::SleepFor(DurationMS(1000));
	}

	StopAllThread();

	//// Clear
	//void* pItem;
	//while( Allocated.GetSize() != 0 && Allocated.Dequeue( pItem ) == S_OK )
	//{
	//	if( pItem )
	//		Pool.Free( pItem );
	//}

	Pool.Clear();
}


TEST_F(MemoryTest, MemoryPool)
{
	const INT64 TEST_LENGTH	=		99999;
	const int PUSH_LIMIT	=		100;
	const int ALLOC_THREAD_COUNT	=	20;
	const int FREE_THREAD_COUNT		=	1;

	BR::PageQueue<MemoryTestObject*> Allocated( 128 );
	SyncCounter lPusherCount = 0;

	MemoryTestObject::MemoryPoolCache(100);
	MemoryTestObject2::MemoryPoolCache(100);
	MemoryTestObject3::MemoryPoolCache(100);

	BR::CheckCtrMemory();

	for( INT64 ID = 0; ID < ALLOC_THREAD_COUNT; ID++ )
	{
		auto pTask = new BR::FunctorThread([&, ID](Thread* pThread)
		{
			lPusherCount.fetch_add(1, std::memory_order_relaxed);

			for(int i=0; i<TEST_LENGTH; i++)
			{
				MemoryTestObject* pNewItem = new MemoryTestObject;

				pNewItem->Index = i;
				pNewItem->WorkerID = ID;
				pNewItem->Checksum = (ID<<32) + i;

				_WriteBarrier();

				Allocated.Enqueue( pNewItem );

			}

			lPusherCount.fetch_sub(1, std::memory_order_relaxed);
		} );

		pTask->Start();
		m_Threads.push_back(pTask);

	};



	for( INT64 ID = 0; ID < FREE_THREAD_COUNT; ID++ )
	{
		auto pTask = new BR::FunctorThread([&, ID](Thread* pThread) 
		{

			while(1)
			{
				MemoryTestObject* pItem = NULL;
				if( (Allocated.Dequeue( pItem ) == S_OK) 
					&& pItem != NULL )
				{

					size_t d_aid = (pItem->Checksum - pItem->Index)>>32;
					EXPECT_EQ(d_aid, pItem->WorkerID);

					EXPECT_GE(pItem->WorkerID, 0);
					EXPECT_LT(pItem->WorkerID, ALLOC_THREAD_COUNT);
					EXPECT_GE(pItem->Index, 0);
					EXPECT_LT(pItem->Index, TEST_LENGTH);


					pItem->Index = -1;
					pItem->WorkerID = -1;
					pItem->Checksum = -1;

					delete pItem;
				}
				else
				{
					if(lPusherCount == 0)
						break;

					ThisThread::SleepFor(DurationMS(100));
				}
			}
		});

		pTask->Start();
		m_Threads.push_back(pTask);
	};


	ThisThread::SleepFor(DurationMS(1000));

	while( lPusherCount != 0 )
	{
		ThisThread::SleepFor(DurationMS(1000));
	}

	StopAllThread();

}


TEST_F(MemoryTest, CircularBufferAllocator)
{
	const INT TEST_LENGTH		=		99999999;
	const INT BUFFER_SIZE		=		512;
	const INT ALLOCATE_SIZE_MIN	=		4;
	const INT ALLOCATE_SIZE_MAX	=		20;

	BR::CircularBufferAllocator<BUFFER_SIZE,8> allocator(STDAllocator::GetInstance());
	BR::PageQueue<void*> testQueue;
	void *pPtr = nullptr;

	// random test
	for( INT test = 0; test < TEST_LENGTH; test++ )
	{
		int randVal = rand() % 100;
		if( randVal < 50 && testQueue.GetEnqueCount() > 0 && SUCCEEDED(testQueue.Dequeue( pPtr )) )
		{
			HRESULT hr = allocator.Free( pPtr );
			EXPECT_HRESULT_SUCCEEDED(hr);
			Assert(SUCCEEDED(hr));
		}
		else
		{
			bool mustSuccess = false;
			int allocSize = Util::Random.Rand(ALLOCATE_SIZE_MIN,ALLOCATE_SIZE_MAX);
			if( allocator.GetFreeMemorySize() > (allocSize*2) ) mustSuccess = true;

			HRESULT hr = allocator.Alloc( allocSize, pPtr );
			if( mustSuccess )
			{
				EXPECT_HRESULT_SUCCEEDED(hr);
				Assert(SUCCEEDED(hr));
			}
			if( SUCCEEDED(hr) )
			{
				testQueue.Enqueue( pPtr );
			}
		}
	};

	while(testQueue.GetEnqueCount() > 0 && SUCCEEDED(testQueue.Dequeue( pPtr )))
	{
		HRESULT hr = allocator.Free( pPtr );
			EXPECT_HRESULT_SUCCEEDED(hr);
			Assert(SUCCEEDED(hr));
	}
	EXPECT_EQ(BUFFER_SIZE,allocator.GetFreeMemorySize());
	EXPECT_EQ(0,testQueue.GetEnqueCount());


	// overflow test
	for( INT test = 0; test < TEST_LENGTH; test++ )
	{
		void *pPtr = nullptr;
		int randVal = rand() % 100;
		if( randVal < 30 && testQueue.GetEnqueCount() > 0 && SUCCEEDED(testQueue.Dequeue( pPtr )) )
		{
			HRESULT hr = allocator.Free( pPtr );
			EXPECT_HRESULT_SUCCEEDED(hr);
			Assert(SUCCEEDED(hr));
		}
		else
		{
			bool mustSuccess = false;
			int allocSize = Util::Random.Rand(ALLOCATE_SIZE_MIN,ALLOCATE_SIZE_MAX);
			if( allocator.GetFreeMemorySize() > (allocSize*2) ) mustSuccess = true;

			HRESULT hr = allocator.Alloc( allocSize, pPtr );
			if( mustSuccess )
			{
				EXPECT_HRESULT_SUCCEEDED(hr);
				Assert(SUCCEEDED(hr));
			}
			if( SUCCEEDED(hr) )
			{
				testQueue.Enqueue( pPtr );
			}
		}
	};
	
	while(testQueue.GetEnqueCount() > 0 && SUCCEEDED(testQueue.Dequeue( pPtr )))
	{
		HRESULT hr = allocator.Free( pPtr );
			EXPECT_HRESULT_SUCCEEDED(hr);
			Assert(SUCCEEDED(hr));
	}
	EXPECT_EQ(BUFFER_SIZE,allocator.GetFreeMemorySize());
	EXPECT_EQ(0,testQueue.GetEnqueCount());


	// underflow test
	for( INT test = 0; test < TEST_LENGTH; test++ )
	{
		void *pPtr = nullptr;
		int randVal = rand() % 100;
		if( randVal < 70 && testQueue.GetEnqueCount() > 0 && SUCCEEDED(testQueue.Dequeue( pPtr )) )
		{
			HRESULT hr = allocator.Free( pPtr );
			EXPECT_HRESULT_SUCCEEDED(hr);
			Assert(SUCCEEDED(hr));
		}
		else
		{
			bool mustSuccess = false;
			int allocSize = Util::Random.Rand(ALLOCATE_SIZE_MIN,ALLOCATE_SIZE_MAX);
			if( allocator.GetFreeMemorySize() > (allocSize*2) ) mustSuccess = true;

			HRESULT hr = allocator.Alloc( allocSize, pPtr );
			if( mustSuccess )
			{
				EXPECT_HRESULT_SUCCEEDED(hr);
				Assert(SUCCEEDED(hr));
			}
			if( SUCCEEDED(hr) )
			{
				testQueue.Enqueue( pPtr );
			}
		}
	};
	
	while(testQueue.GetEnqueCount() > 0 && SUCCEEDED(testQueue.Dequeue( pPtr )))
	{
		HRESULT hr = allocator.Free( pPtr );
			EXPECT_HRESULT_SUCCEEDED(hr);
			Assert(SUCCEEDED(hr));
	}
	EXPECT_EQ(BUFFER_SIZE,allocator.GetFreeMemorySize());
	EXPECT_EQ(0,testQueue.GetEnqueCount());

}


TEST_F(MemoryTest, CircularQueue)
{
	const INT TEST_LENGTH		=		99999999;
	const INT BUFFER_SIZE		=		512;
	const INT ALLOCATE_SIZE_MIN	=		4;
	const INT ALLOCATE_SIZE_MAX	=		20;

	BR::CircularQueue<int,BUFFER_SIZE> queue;
	BR::PageQueue<int> testQueue;
	int testVal = 0;
	int testVal2 = 0;

	// random test
	for( INT test = 1; test < TEST_LENGTH; test++ )
	{
		int randVal = rand() % 100;
		if( randVal < 50 && testQueue.GetEnqueCount() > 0 && SUCCEEDED(testQueue.Dequeue( testVal )) )
		{
			HRESULT hr = queue.Dequeue( testVal2 );
			EXPECT_HRESULT_SUCCEEDED(hr);
			Assert(SUCCEEDED(hr));
			Assert(testVal2 == testVal);
		}
		else
		{
			bool mustSuccess = false;
			int allocSize = Util::Random.Rand(ALLOCATE_SIZE_MIN,ALLOCATE_SIZE_MAX);
			if( queue.GetSize() < BUFFER_SIZE ) mustSuccess = true;

			HRESULT hr = queue.Enqueue( std::move(test) );
			if( mustSuccess )
			{
				EXPECT_HRESULT_SUCCEEDED(hr);
				Assert(SUCCEEDED(hr));
			}
			if( SUCCEEDED(hr) )
			{
				testQueue.Enqueue( test );
			}
		}
	};

	while(testQueue.GetEnqueCount() > 0 && SUCCEEDED(testQueue.Dequeue( testVal )))
	{
		HRESULT hr = queue.Dequeue( testVal2 );
		EXPECT_HRESULT_SUCCEEDED(hr);
		Assert(SUCCEEDED(hr));
		Assert( testVal == testVal2 );
	}
	EXPECT_EQ(0,queue.GetSize());
	EXPECT_EQ(0,testQueue.GetEnqueCount());

	
	// overflow test
	for( INT test = 1; test < TEST_LENGTH; test++ )
	{
		int randVal = rand() % 100;
		if( randVal < 30 && testQueue.GetEnqueCount() > 0 && SUCCEEDED(testQueue.Dequeue( testVal )) )
		{
			HRESULT hr = queue.Dequeue( testVal2 );
			EXPECT_HRESULT_SUCCEEDED(hr);
			Assert(SUCCEEDED(hr));
			Assert(testVal2 == testVal);
		}
		else
		{
			bool mustSuccess = false;
			int allocSize = Util::Random.Rand(ALLOCATE_SIZE_MIN,ALLOCATE_SIZE_MAX);
			if( queue.GetSize() < BUFFER_SIZE ) mustSuccess = true;

			HRESULT hr = queue.Enqueue( std::move(test) );
			if( mustSuccess )
			{
				EXPECT_HRESULT_SUCCEEDED(hr);
				Assert(SUCCEEDED(hr));
			}
			if( SUCCEEDED(hr) )
			{
				testQueue.Enqueue( test );
			}
		}
	};

	while(testQueue.GetEnqueCount() > 0 && SUCCEEDED(testQueue.Dequeue( testVal )))
	{
		HRESULT hr = queue.Dequeue( testVal2 );
		EXPECT_HRESULT_SUCCEEDED(hr);
		Assert(SUCCEEDED(hr));
		Assert( testVal == testVal2 );
	}
	EXPECT_EQ(0,queue.GetSize());
	EXPECT_EQ(0,testQueue.GetEnqueCount());


	// underflow test
	for( INT test = 1; test < TEST_LENGTH; test++ )
	{
		int randVal = rand() % 100;
		if( randVal < 70 && testQueue.GetEnqueCount() > 0 && SUCCEEDED(testQueue.Dequeue( testVal )) )
		{
			HRESULT hr = queue.Dequeue( testVal2 );
			EXPECT_HRESULT_SUCCEEDED(hr);
			Assert(SUCCEEDED(hr));
			Assert(testVal2 == testVal);
		}
		else
		{
			bool mustSuccess = false;
			int allocSize = Util::Random.Rand(ALLOCATE_SIZE_MIN,ALLOCATE_SIZE_MAX);
			if( queue.GetSize() < BUFFER_SIZE ) mustSuccess = true;

			HRESULT hr = queue.Enqueue( std::move(test) );
			if( mustSuccess )
			{
				EXPECT_HRESULT_SUCCEEDED(hr);
				Assert(SUCCEEDED(hr));
			}
			if( SUCCEEDED(hr) )
			{
				testQueue.Enqueue( test );
			}
		}
	};

	while(testQueue.GetEnqueCount() > 0 && SUCCEEDED(testQueue.Dequeue( testVal )))
	{
		HRESULT hr = queue.Dequeue( testVal2 );
		EXPECT_HRESULT_SUCCEEDED(hr);
		Assert(SUCCEEDED(hr));
		Assert( testVal == testVal2 );
	}
	EXPECT_EQ(0,queue.GetSize());
	EXPECT_EQ(0,testQueue.GetEnqueCount());


}



TEST_F(MemoryTest, StdPerformance)
{
	const UINT TEST_COUNT = 1000000;
	const UINT TEST_THREAD_COUNT = 10;

	BR::STDAllocator allocator;
	auto start = BR::Util::Time.GetRawTimeMs();
	for (UINT iTest = 0; iTest < TEST_COUNT; iTest++)
	{
		void *pPtr = nullptr;
		allocator.Alloc(123,pPtr);
		allocator.Free(pPtr);
	}
	auto end = BR::Util::Time.GetRawTimeMs();
	printf("My Heap: %d\n", (end - start).count());

	start = BR::Util::Time.GetRawTimeMs();
	for (UINT iTest = 0; iTest < TEST_COUNT; iTest++)
	{
		void *pPtr = nullptr;
		pPtr = malloc(123);
		free(pPtr);
	}
	end = BR::Util::Time.GetRawTimeMs();
	printf("STD Heap: %d\n", (end - start).count());


	std::vector<BR::FunctorThread*> workingThreadList;
	workingThreadList.reserve(TEST_THREAD_COUNT);
	std::atomic<LONG> workingThreads(0);
	start = BR::Util::Time.GetRawTimeMs();
	for (UINT iTest = 0; iTest < TEST_THREAD_COUNT; iTest++)
	{
		auto thread = new BR::FunctorThread([&](BR::Thread* pThread)
		{
			for (UINT iTest = 0; iTest < TEST_COUNT; iTest++)
			{
				void *pPtr = nullptr;
				allocator.Alloc(123, pPtr);
				allocator.Free(pPtr);
			}
			workingThreads.fetch_sub(1, std::memory_order_relaxed);
		});
		thread->Start();
		workingThreadList.push_back(thread);
		workingThreads.fetch_add(1, std::memory_order_relaxed);
	}

	while (workingThreads.load() > 0);
	end = BR::Util::Time.GetRawTimeMs();
	printf("Thread My Heap: %d\n", (end - start).count());
	for (auto itThread : workingThreadList)
	{
		itThread->Stop();
	}

	workingThreadList.clear();
	start = BR::Util::Time.GetRawTimeMs();
	for (UINT iTest = 0; iTest < TEST_THREAD_COUNT; iTest++)
	{
		auto thread = new BR::FunctorThread([&](BR::Thread* pThread)
		{
			for (UINT iTest = 0; iTest < TEST_COUNT; iTest++)
			{
				void *pPtr = nullptr;
				pPtr = malloc(123);
				free(pPtr);
			}
			workingThreads.fetch_sub(1, std::memory_order_release);
		});
		thread->Start();
		workingThreadList.push_back(thread);
		workingThreads.fetch_add(1, std::memory_order_release);
	}

	while (workingThreads.load() > 0);
	end = BR::Util::Time.GetRawTimeMs();
	printf("Thread STD Heap: %d\n", (end - start).count());
	for (auto itThread : workingThreadList)
	{
		itThread->Stop();
	}
	workingThreadList.clear();
}

TEST_F(MemoryTest, PagedQueueAllocation)
{
	const INT TEST_LENGTH = 999;
	const INT BUFFER_SIZE = 512;
	const INT ALLOCATE_SIZE_MIN = 4;
	const INT ALLOCATE_SIZE_MAX = 20;

	auto testQueue = new BR::PageQueue<int>(4);
	auto memoryPool = testQueue->GetMemoryPool();

	for (int iTest = 0; iTest < 131; iTest++)
	{
		testQueue->Enqueue(10);
	}

	testQueue->ClearQueue();

	delete testQueue;


	memoryPool->PrintAllocatedList();
}



}  // namespace BRTest
