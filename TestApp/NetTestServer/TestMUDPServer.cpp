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
		MAX_CLIENT = 1,
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
		m_pServer = new Net::ServerMUDP(SERVERID, GetNetClass());
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
	Net::Connection *pConnection = nullptr;
	Net::IConnection::ConnectionInformation connectionInfo;
	bool bWaitingTest = true;


	EXPECT_HRESULT_SUCCEEDED(Net::GetLocalAddressIPv6(localAddr));

	hr = m_pServer->HostOpen(GetNetClass(), localAddr.strAddr, localAddr.usPort);
	EXPECT_HRESULT_SUCCEEDED(hr);
	defChk(hr);


	m_pServer->SetIsEnableAccept(true);

	for (int iClient = 0; iClient < MAX_CLIENT; iClient++)
	{
		defChkPtr(pConnection = m_pServer->GetConnectionManager().NewConnection());

		memset(&connectionInfo, 0, sizeof(connectionInfo));
		connectionInfo.SetLocalInfo(GetNetClass(), m_pServer->GetLocalAddress(), SERVERID);
		connectionInfo.SetRemoteInfo(NetClass::Client, iClient+1);

		defChk(pConnection->InitConnection(m_pServer->GetSocket(), connectionInfo));
		defTrace(Trace::TRC_USER1, "Initialize connection CID:{0}, Addr:{1}:{2}", pConnection->GetCID(), pConnection->GetConnectionInfo().Remote.strAddr, pConnection->GetConnectionInfo().Remote.usPort);

		defChk(m_pServer->GetConnectionManager().PendingConnection(pConnection));

		m_ConnectionList.push_back(pConnection);
	}

	while (bWaitingTest)
	{
		size_t iDisconnected = 0;
		for (auto itConnection : m_ConnectionList)
		{
			itConnection->UpdateNetCtrl();
			if (itConnection->GetConnectionState() == Net::IConnection::ConnectionState::STATE_DISCONNECTED) iDisconnected++;
		}

		if (iDisconnected == m_ConnectionList.size())
		{
			break;
		}
	}

Proc_End:

	for (auto itConnection : m_ConnectionList)
	{
		itConnection->CloseConnection();
	}
	m_ConnectionList.clear();

}

