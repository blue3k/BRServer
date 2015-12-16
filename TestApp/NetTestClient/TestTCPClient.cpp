// TestApp.cpp : Defines the entry point for the console application.
//


#include "Common/Typedefs.h"
#include "Common/Trace.h"
#include "Common/Memory.h"
#include "Common/MemoryPool.h"
#include "Net/NetDef.h"
#include "Net/NetServer.h"
#include "Net/NetServerPeerTCP.h"
#include "../TestCommon/TestBaseCommon.h"
#include "Protocol/Policy/ServerIPolicy.h"



using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;
using namespace BR;



class TCPClientTest : public testing::Test
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
	Net::ServerPeerTCP* m_pServer;
	TCPClientTest()
		: m_pServer(nullptr)
	{
	}

	NetClass GetNetClass() { return NetClass::Game; }

	virtual void SetUp()
	{
		m_pServer = new Net::ServerPeerTCP(CLIENTID, GetNetClass());
	}

	virtual void TearDown()
	{

		m_pServer->HostClose();
		Util::SafeDelete(m_pServer);
	}
};


TEST_F(TCPClientTest, Peer)
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

	for (int iClient = 0; iClient < MAX_CLIENT; iClient++)
	{
		Net::IConnection* pConnection = nullptr;
		defChk(m_pServer->RegisterServerConnection(CLIENTID, GetNetClass(), localAddr.strAddr, REMOTE_PORT, pConnection));

		defTrace(Trace::TRC_USER1, "Initialize connection CID:{0}, Addr:{1}:{2}", pConnection->GetCID(), pConnection->GetConnectionInfo().Remote.strAddr, pConnection->GetConnectionInfo().Remote.usPort);


		//defChk(m_pServer->GetConnectionManager().PendingConnection(pConnection));

		m_ConnectionList.push_back(SharedPointerT<Net::Connection>((Net::Connection*)pConnection));
	}


	startTime = Util::Time.GetTimeMs();
	while (bWaitingTest)
	{
		ThisThread::SleepFor(DurationMS(500));

		for (auto itConnection : m_ConnectionList)
		{
			itConnection->UpdateNetCtrl();
			if (itConnection->GetConnectionState() == Net::IConnection::ConnectionState::STATE_DISCONNECTED)
			{
				auto tcpConn = dynamic_cast<Net::ConnectionTCP*>(itConnection.GetObjectPtr());
				auto connectionInfo = tcpConn->GetConnectionInfo();
				hr = m_pServer->Connect(tcpConn, (UINT)connectionInfo.RemoteID, connectionInfo.RemoteClass, connectionInfo.Remote.strAddr, connectionInfo.Remote.usPort);
				EXPECT_HRESULT_SUCCEEDED(hr);
			}
			else if (itConnection->GetConnectionState() == Net::IConnection::ConnectionState::STATE_CONNECTED)
			{
				ServiceInformation serviceInfo(CLIENTID, ClusterMembership::StatusWatcher, ServiceStatus::Online, NetClass::Client, localAddr, Util::Time.GetTimeUTCSec(), 12);

				auto policy = itConnection->GetPolicy<Policy::IPolicyServer>();
				hr = policy->ServerConnectedC2SEvt(RouteContext(1, 2), serviceInfo, Util::Time.GetTimeMs().time_since_epoch().count(), localAddr, localAddr);
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
	}
	m_ConnectionList.clear();

}


