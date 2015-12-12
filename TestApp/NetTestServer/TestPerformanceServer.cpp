// TestApp.cpp : Defines the entry point for the console application.
//


#include "Common/Typedefs.h"
#include "Common/Trace.h"
#include "Common/Memory.h"
#include "Common/MemoryPool.h"
#include "Net/NetDef.h"
#include "Net/NetServer.h"
#include "../TestCommon/TestBaseCommon.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterServer.h"




using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;
using namespace BR;




class PerformanceServerTest : public testing::Test
{
protected:

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
		Svr::PerformanceCounterServer::Terminate();
	}
};


TEST_F(PerformanceServerTest, Connect)
{
	HRESULT hr = S_OK;
	NetAddress localAddr;
	bool bWaitingTest = true;

	EXPECT_HRESULT_SUCCEEDED(Net::GetLocalAddressIPv6(localAddr));

	hr = Svr::PerformanceCounterServer::Initialize(localAddr);
	EXPECT_HRESULT_SUCCEEDED(hr);
	defChk(hr);

	while (bWaitingTest)
	{
		DynamicArray<SharedPointerT<Svr::PerformanceCounterInstance>> instanceList;

		ThisThread::SleepFor(DurationMS(3000));

		Svr::PerformanceCounterServer::GetInstanceList(0, instanceList);
		for (auto itInstance : instanceList)
		{
			auto& counters = itInstance->GetCounters();

			defTrace(Trace::TRC_TRACE, "Instance {0}", itInstance->GetInstanceName());
			for (auto itCounter : counters)
			{
				defTrace(Trace::TRC_TRACE, "		Counter:{0}, {1:X16}", itCounter->GetCounterName(), itCounter->GetRawValue());
			}
		}

	}


Proc_End:
	
	Svr::PerformanceCounterServer::GetInstanceCount();

}




