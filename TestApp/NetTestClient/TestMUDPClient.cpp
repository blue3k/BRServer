// TestApp.cpp : Defines the entry point for the console application.
//


#include "Common/Typedefs.h"
#include "Common/Trace.h"
#include "Common/Memory.h"
#include "Common/MemoryPool.h"
#include "Net/NetDef.h"
#include "Net/NetServer.h"
#include "Net/NetClient.h"
#include "../TestCommon/TestBaseCommon.h"
#include "Protocol/Policy/GameIPolicy.h"




using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;
using namespace BR;




class MUDPClientTest : public testing::Test
{
protected:

	enum {
		SERVERID = 3,
		CLIENTID = 5,
		MAX_CLIENT = 1,
		LOCAL_PORT = 52000,
		REMOTE_PORT = 52001,
	};

	std::vector<SharedPointerT<Net::Connection>> m_ConnectionList;
	Net::ClientMUDP* m_Client;

	MUDPClientTest()
		: m_Client(nullptr)
	{
	}

	NetClass GetNetClass() { return NetClass::Game; }

	virtual void SetUp()
	{
		m_Client = new Net::ClientMUDP;
	}

	virtual void TearDown()
	{
		Util::SafeDelete(m_Client);
	}
};


TEST_F(MUDPClientTest, Connect)
{
	HRESULT hr = S_OK;
	NetAddress localAddr;
	Net::IConnection::ConnectionInformation connectionInfo;
	bool bWaitingTest = true;
	TimeStampMS startTime, endTime;


	EXPECT_HRESULT_SUCCEEDED(Net::GetLocalAddressIPv6(localAddr));


	for (int iClient = 0; iClient < MAX_CLIENT; iClient++)
	{
		NetAddress destAddress;
		Net::IConnection* pConnection = nullptr;

		Net::SetNetAddress(destAddress, localAddr.strAddr, REMOTE_PORT);
		defChk(m_Client->ConnectCli(destAddress, pConnection));

		defTrace(Trace::TRC_USER1, "Initialize connection CID:{0}, Addr:{1}:{2}", pConnection->GetCID(), pConnection->GetConnectionInfo().Remote.strAddr, pConnection->GetConnectionInfo().Remote.usPort);

		m_ConnectionList.push_back(SharedPointerT<Net::Connection>((Net::Connection*)pConnection));
	}


	startTime = Util::Time.GetTimeMs();
	while (bWaitingTest)
	{
		ThisThread::SleepFor(DurationMS(500));

		for (auto itConnection : m_ConnectionList)
		{
			if (itConnection->GetConnectionState() == Net::IConnection::ConnectionState::STATE_DISCONNECTED)
			{
			}
			else if (itConnection->GetConnectionState() == Net::IConnection::ConnectionState::STATE_CONNECTED)
			{
				auto policy = itConnection->GetPolicy<Policy::IPolicyGame>();
				hr = policy->HeartBitC2SEvt();
				EXPECT_HRESULT_SUCCEEDED(hr);
			}
		}


		if (Util::TimeSince(startTime) > DurationMS(2 * 60 * 60 * 1000))
			break;
	}


Proc_End:

	for (auto itConnection : m_ConnectionList)
	{
		itConnection->CloseConnection();
		m_Client->ReleaseConnection(itConnection.GetObjectPtr());
	}
	m_ConnectionList.clear();

}


