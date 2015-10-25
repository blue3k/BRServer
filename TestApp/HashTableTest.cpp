// TestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <gtest/gtest.h>
#include "Common/StrUtil.h"
#include "Common/StrFormat.h"
#include "Common/PageQueue.h"
#include "HashTableTest.h"
#include "Common/OrderedLinkedList.h"
#include "Common/HashTable.h"



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


TEST_F(HashTableTest, HashTable_NonUnique)
{
	const int MAX_NUMBER	= 100;
	const int MAX_TEST = TestScale*1000;

	
	struct TestMapNode
	{
	public:
		int Value;
	};


	typedef BR::Hash::HashTable<	TestMapNode*, 
									BR::Indexing::MemData<TestMapNode,int,&TestMapNode::Value>,
									BR::Hash::NonUniqueKeyTrait, BR::ThreadSyncTraitNone
									> TestTableType;

	TestTableType TestMap;
	std::unordered_map<int,int> checkSet;
	TestTableType::iterator itCur;

	for( int iTest = 0; iTest < MAX_TEST; iTest++ )
	{
		int value = rand() % MAX_NUMBER;
		auto itFound = checkSet.find(value);

		TestMapNode *pNewNode = new TestMapNode;
		memset( pNewNode, 0, sizeof(TestMapNode) );
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
		HRESULT hrRes = TestMap.insert(pNewNode);
		EXPECT_HRESULT_SUCCEEDED(hrRes);
		AssertRel(SUCCEEDED(hrRes));
	}

	
	for( int iTest = 0; iTest < MAX_TEST; iTest++ )
	{
		int value = rand() % MAX_NUMBER;
		auto itFound = checkSet.find(value);

		if( itFound == checkSet.end() )
		{
			EXPECT_HRESULT_FAILED( TestMap.find( value, itCur) );
		}
		else
		{
			EXPECT_HRESULT_SUCCEEDED( TestMap.find( value, itCur) );
		}
	}

	auto itCheck = checkSet.begin();
	for( ; itCheck != checkSet.end(); itCheck++ )
	{
		HRESULT hr = S_OK;
		hr = TestMap.find( itCheck->first, itCur);
		EXPECT_HRESULT_SUCCEEDED(  hr );

		if( FAILED(hr) )
			continue;
		TestMapNode *pNode = *itCur;

		hr = TestMap.erase( itCur );
		EXPECT_HRESULT_SUCCEEDED( hr );
		if( FAILED(hr) )
			continue;
		delete pNode;
	}
}



TEST_F(HashTableTest, HashTable_Unique)
{
	const int MAX_NUMBER	= 100;
	const int MAX_TEST = TestScale * 1000;

	
	struct TestMapNode
	{
	public:
		int Value;
	};


	typedef BR::Hash::HashTable<	TestMapNode*, 
										BR::Indexing::MemData<TestMapNode,int,&TestMapNode::Value>,
										BR::Hash::UniqueKeyTrait, BR::ThreadSyncTraitNone
										> TestTableType;

	TestTableType TestMap;
	std::unordered_set<int> checkSet;
	TestTableType::iterator itCur;

	for( int iTest = 0; iTest < MAX_TEST; iTest++ )
	{
		int value = rand() % MAX_NUMBER;
		auto itFound = checkSet.find(value);

		if( itFound == checkSet.end() )
		{
			TestMapNode *pNewNode = new TestMapNode;
			memset( pNewNode, 0, sizeof(TestMapNode) );
			pNewNode->Value = value;
			checkSet.insert( value );
			EXPECT_HRESULT_SUCCEEDED( TestMap.insert( pNewNode ) );
		}
	}


	for( int iTest = 0; iTest < MAX_TEST; iTest++ )
	{
		int value = rand() % MAX_NUMBER;
		auto itFound = checkSet.find(value);

		if( itFound == checkSet.end() )
		{
			EXPECT_HRESULT_FAILED( TestMap.find( value, itCur) );
		}
		else
		{
			EXPECT_HRESULT_SUCCEEDED( TestMap.find( value, itCur) );
		}
	}

	auto itCheck = checkSet.begin();
	for( ; itCheck != checkSet.end(); itCheck++ )
	{
		EXPECT_HRESULT_SUCCEEDED( TestMap.find( *itCheck, itCur) );
		TestMapNode *pNode = *itCur;
		EXPECT_HRESULT_SUCCEEDED( TestMap.erase( itCur ) );
		delete pNode;
	}

}


TEST_F(HashTableTest, HashTable_UniqueMT)
{
	const int READ_THREAD_COUNT = 10;
	const int WRITE_THREAD_COUNT = 5;
	const int MAX_NUMBER	= 500;
	const int TEST_COUNT		= TestScale*9999;


	SyncCounter workCounterRead, workCounterWrite, numberOfItems;

	
	struct TestMapNode
	{
	public:
		int Value;
	};


	typedef BR::Hash::HashTable<	TestMapNode*, 
										BR::Indexing::MemData<TestMapNode,int,&TestMapNode::Value>,
										BR::Hash::UniqueKeyTrait, BR::ThreadSyncTraitMT
										> TestTableType;

	TestTableType TestMap;
	TestTableType::iterator itCur;

	for( INT64 ID = 0; ID < READ_THREAD_COUNT; ID++ )
	{
		FunctorThread *pTask = new BR::FunctorThread([&, ID](Thread *pThread)
		{
			workCounterRead.fetch_add(1,std::memory_order_relaxed);

			while (!pThread->CheckKillEvent(5))
			{
				int value = rand() % MAX_NUMBER;
				TestTableType::iterator itFound;

				if( SUCCEEDED(TestMap.find( value, itFound )) )
				{
					Sleep(10);
					EXPECT_EQ(value,itFound->Value);
					Assert(value == itFound->Value);
				}
			}

			workCounterRead.fetch_sub(1, std::memory_order_relaxed);

		} );
		pTask->Start();
		m_Threads.push_back(pTask);
	};

	
	for( INT64 ID = 0; ID < WRITE_THREAD_COUNT; ID++ )
	{
		auto pTask = new BR::FunctorThread([&, ID](Thread* pThread)
		{
			workCounterWrite.fetch_add(1,std::memory_order_relaxed);

			for(int i=0; i<(TEST_COUNT); i++)
			{
				int value = rand() % MAX_NUMBER;
				TestMapNode *pNewNode = new TestMapNode;
				memset( pNewNode, 0, sizeof(TestMapNode) );
				pNewNode->Value = value;
				if( SUCCEEDED(TestMap.insert( pNewNode )) )
				{
					numberOfItems.fetch_add(1,std::memory_order_relaxed);
				}

			}

			workCounterWrite.fetch_sub(1,std::memory_order_relaxed);

		} );
		pTask->Start();
		m_Threads.push_back(pTask);
	};
	
	Sleep(2000);

	for( INT64 ID = 0; ID < WRITE_THREAD_COUNT; ID++ )
	{
		auto pTask = new BR::FunctorThread([&, ID](Thread* pThread)
		{
			workCounterWrite.fetch_add(1,std::memory_order_relaxed);

			for(int i=0; i<(TEST_COUNT); i++)
			{
				int value = rand() % MAX_NUMBER;

				TestTableType::iterator itFound;

				if( SUCCEEDED(TestMap.find( value, itFound )) )
				{
					EXPECT_EQ(value,itFound->Value);
					AssertRel(value == itFound->Value);

					if (SUCCEEDED(TestMap.erase(itFound)))
						numberOfItems.fetch_sub(1,std::memory_order_relaxed);
				}

				if (numberOfItems < 10 && workCounterWrite.load(std::memory_order_relaxed) > WRITE_THREAD_COUNT)
					Sleep(10);
			}

			workCounterWrite.fetch_sub(1,std::memory_order_relaxed);

		} );
		pTask->Start();
		m_Threads.push_back(pTask);
	};
	

	while((workCounterWrite) > 0 ) Sleep(1000);

	Sleep(1000);

	StopAllThread();

	//do 
	{
		TestTableType::iterator itCur = TestMap.begin();
		if( itCur.IsValid() )
		{
			numberOfItems.fetch_sub(1,std::memory_order_relaxed);
			delete *itCur;
			TestMap.erase(itCur);
		}
	}
	EXPECT_EQ(0, TestMap.size());


	EXPECT_EQ(0,numberOfItems);
}


}  // namespace BRTest