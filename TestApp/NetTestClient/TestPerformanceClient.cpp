// TestApp.cpp : Defines the entry point for the console application.
//


#include "Common/Typedefs.h"
#include "Common/Trace.h"
#include "Common/Memory.h"
#include "Common/MemoryPool.h"
#include "Net/NetDef.h"
#include "Net/NetServer.h"
#include "../TestCommon/TestBaseCommon.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterClient.h"




using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;
using namespace BR;




class PerformanceClientTest : public testing::Test
{
protected:
	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
		Svr::PerformanceCounterClient::Terminate();
	}
};


TEST_F(PerformanceClientTest, Connect)
{
	HRESULT hr = S_OK;
	NetAddress localAddr;
	bool bWaitingTest = true;
	DynamicArray<Svr::PerformanceCounterRaw < UINT64 >*> counterList;

	EXPECT_HRESULT_SUCCEEDED(Net::GetLocalAddressIPv6(localAddr));

	hr = Svr::PerformanceCounterClient::Initialize(localAddr);
	EXPECT_HRESULT_SUCCEEDED(hr);
	defChk(hr);



	for (int iCounter = 0; iCounter < 3; iCounter++)
	{
		char counterName[128];
		StrUtil::Format(counterName, "Counter_{0}", iCounter);
		counterList.push_back(new Svr::PerformanceCounterRaw < UINT64 >(counterName));
	}


	while (bWaitingTest)
	{

		ThisThread::SleepFor(DurationMS(1000));

		for (auto itCounter : counterList)
		{
			itCounter->Inc();
		}
	}


Proc_End:

	for (auto itCounter : counterList)
	{
		delete itCounter;
	}
	counterList.Clear();
}


