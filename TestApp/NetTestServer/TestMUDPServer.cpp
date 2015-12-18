// TestApp.cpp : Defines the entry point for the console application.
//


#include "Common/Typedefs.h"
#include "Common/Trace.h"
#include "Common/Memory.h"
#include "Common/MemoryPool.h"
#include "Net/NetDef.h"
#include "Net/NetUtil.h"
#include "Net/NetServer.h"
#include "Net/NetServerUDP.h"
#include "../TestCommon/TestBaseCommon.h"
#include "Protocol/Message/GameMsgClass.h"




using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;
using namespace BR;




class MUDPServerTest : public testing::Test
{
protected:

	enum {
		SERVERID = 3,
		CLIENTID = 5,
		MAX_CLIENT = 1,
		LOCAL_PORT = 52001,
		REMOTE_PORT = 52000,
	};

	std::vector<SharedPointerT<Net::Connection>> m_ConnectionList;
	Net::ServerMUDP* m_pServer;
	MUDPServerTest()
		: m_pServer(nullptr)
	{
	}

	NetClass GetNetClass() { return NetClass::Game; }

	virtual void SetUp()
	{
		m_pServer = new Net::ServerMUDP(CLIENTID, GetNetClass());
	}

	virtual void TearDown()
	{
		m_pServer->HostClose();
		Util::SafeDelete(m_pServer);
	}
};


TEST_F(MUDPServerTest, Connect)
{
	HRESULT hr = S_OK;
	NetAddress localAddr;
	Net::IConnection::ConnectionInformation connectionInfo;
	bool bWaitingTest = true;
	TimeStampMS startTime, endTime;


	EXPECT_HRESULT_SUCCEEDED(Net::GetLocalAddressIPv6(localAddr));
	localAddr.usPort = LOCAL_PORT;

	hr = m_pServer->HostOpen(GetNetClass(), localAddr.strAddr, localAddr.usPort);
	EXPECT_HRESULT_SUCCEEDED(hr);
	defChk(hr);


	m_pServer->SetIsEnableAccept(true);



	startTime = Util::Time.GetTimeMs();
	while (bWaitingTest)
	{
		ThisThread::SleepFor(DurationMS(500));

		// Update server events
		do
		{
			Net::INet::Event curNetEvent;
			Net::Connection* pNewConn = nullptr;
			hr = m_pServer->DequeueNetEvent(curNetEvent);
			if (FAILED(hr)) break;

			switch (curNetEvent.EventType)
			{
			case Net::INet::Event::EVT_NEW_CONNECTION:
				if (curNetEvent.EventConnection == nullptr)
					break;

				pNewConn = dynamic_cast<Net::Connection*>(curNetEvent.EventConnection);
				m_ConnectionList.push_back(pNewConn);
				break;
			default:
				break;
			};
		} while (1);


		if (Util::TimeSince(startTime) > DurationMS(2 * 60 * 60 * 1000))
			break;
	}


Proc_End:

	for (auto itConnection : m_ConnectionList)
	{
		itConnection->CloseConnection();
	}
	m_ConnectionList.clear();

}

