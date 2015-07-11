////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : TCP Connection implementations
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "StdAfx.h"
#include "Common/Thread.h"
#include "Common/BrAssert.h"
#include "Common/TimeUtil.h"
#include "Common/HRESNet.h"
#include "Net/NetTrace.h"
#include "Net/Connection.h"
#include "Net/NetDef.h"
#include "Net/NetServer.h"
#include "Net/NetCtrl.h"
#include "Net/NetConst.h"
#include "Net/NetCtrl.h"

#include "Common/SvrPolicyID.h"

#include "Protocol/ProtocolVer.h"


BR_MEMORYPOOL_IMPLEMENT(Net::ConnectionPeerTCP);


// define if emulate packet loss
//#ifdef _DEBUG
//#define UDP_PACKETLOS_EMULATE
//#endif


#ifdef UDP_PACKETLOS_EMULATE
	#define UDP_PACKETLOS_RATE	5
	#define UDP_PACKETLOS_BOUND	1
#endif // #ifdef UDP_PACKETLOS_EMULATE



namespace BR {
namespace Net {



	////////////////////////////////////////////////////////////////////////////////
	//
	//	TCP Network client connection class
	//

	// Constructor
	ConnectionPeerTCP::ConnectionPeerTCP()
		:ConnectionTCP()
	{
		GetRecvBuffer()->SetupRecvTCP( GetCID() );
		SetHeartbitTry(Const::SVR_HEARTBIT_TIME_PEER);
		SetConnectingTimeOut(Const::SVR_CONNECTION_TIMEOUT);
	}

	ConnectionPeerTCP::~ConnectionPeerTCP()
	{
	}

	void ConnectionPeerTCP::OnConnectionResult(HRESULT hrConnect)
	{
		if (SUCCEEDED(hrConnect))
		{
			Assert(m_ConnectInfo.RemoteID != 0);
		}

		return __super::OnConnectionResult(hrConnect);
	}

	// Update net control, process connection heartbit, ... etc
	HRESULT ConnectionPeerTCP::UpdateNetCtrl()
	{
		HRESULT hr = S_OK;
		Message::MessageID msgIDTem;

		ULONG ulTimeCur = Util::Time.GetTimeMs();

		if( GetPendingRecvCount() == 0 
			&& GetConnectionState() != IConnection::STATE_DISCONNECTED)
		{
			PendingRecv();
		}

		// connect/disconnect process
		msgIDTem.ID = PACKET_NETCTRL_NONE;
		switch (GetConnectionState())
		{
		case IConnection::STATE_CONNECTING:
			if( (INT)(ulTimeCur-m_ulNetCtrlTime) > (INT)GetConnectingTimeOut() ) // connection time out
			{
				m_ulNetCtrlTryTime = ulTimeCur;
				netTrace(TRC_CONNECTION, "Connecting Timeout CID:%0%", GetCID());
				netChk( CloseConnection() );
			}
			else if( (INT)(ulTimeCur-m_ulNetCtrlTryTime) > Const::CONNECTION_RETRY_TIME ) // retry
			{
				m_ulNetCtrlTryTime = ulTimeCur;
				Assert(GetConnectionInfo().LocalClass != NetClass::Unknown);
				m_uiSendNetCtrlCount++;
				netChk( SendNetCtrl( PACKET_NETCTRL_CONNECT, (UINT)GetConnectionInfo().LocalClass, Message::MessageID( BR::PROTOCOL_VERSION ), GetConnectionInfo().LocalID ) );
			}

			goto Proc_End;
			break;
		case IConnection::STATE_DISCONNECTING:
			if( (INT)(ulTimeCur-m_ulNetCtrlTime) > Const::DISCONNECT_TIMEOUT ) // connection time out
			{
				m_ulNetCtrlTryTime = ulTimeCur;
				netTrace(TRC_CONNECTION, "Disconnecting Timeout CID:%0%", GetCID());
				netChk( CloseConnection() );
			}

			goto Proc_End;
			break;
		case IConnection::STATE_CONNECTED:
			if( (INT)(ulTimeCur-m_ulNetCtrlTime) > Const::HEARTBIT_TIMEOUT ) // connection time out
			{
				m_ulNetCtrlTime = ulTimeCur;
				netTrace(TRC_CONNECTION, "Connection Timeout CID:%1%", GetCID());

				netChk( CloseConnection() );
				goto Proc_End;
			}
			else if( (INT)(ulTimeCur-m_ulNetCtrlTryTime) > GetHeartbitTry() ) // heartbit time
			{
				m_ulNetCtrlTryTime = ulTimeCur;
				netChk(SendNetCtrl(PACKET_NETCTRL_HEARTBIT, 0, msgIDTem));
			}
			break;
		};


	Proc_End:


		return hr;
	}




} // namespace Net
} // namespace BR


