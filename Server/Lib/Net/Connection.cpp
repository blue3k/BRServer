////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves.
// 
// Author : KyungKun Ko
//
// Description : Net Connection implementation
//	
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common/Thread.h"
#include "Common/BrAssert.h"
#include "Common/Utility.h"
#include "Common/ResultCode/BRResultCodeNet.h"

#include "Net/NetConst.h"
#include "Net/NetSystem.h"
#include "Net/NetTrace.h"
#include "Net/Connection.h"
#include "Net/NetServer.h"
#include "Net/NetCtrl.h"
#include "Net/NetCtrl.h"
#include "Common/TimeUtil.h"
#include "Net/NetUtil.h"
#include "Net/NetSystem.h"
#include "Protocol/ProtocolVer.h"

#include "Protocol/Policy/ServerNetPolicy.h"
#include "Protocol/Policy/ClusterServerNetPolicy.h"
#include "Protocol/Policy/EntityServerNetPolicy.h"
#include "Protocol/Policy/GameMasterServerNetPolicy.h"
#include "Protocol/Policy/GameNetPolicy.h"
#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"
#include "Protocol/Policy/GamePartyNetPolicy.h"
#include "Protocol/Policy/PartyMatchingNetPolicy.h"
#include "Protocol/Policy/PartyMatchingQueueNetPolicy.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"
#include "Protocol/Policy/GameInstanceManagerNetPolicy.h"
#include "Protocol/Policy/GamePartyManagerNetPolicy.h"
#include "Protocol/Policy/LoginNetPolicy.h"
#include "Protocol/Policy/LoginServerNetPolicy.h"
#include "Protocol/Policy/RankingNetPolicy.h"
#include "Protocol/Policy/MonitoringNetPolicy.h"


// define if emulate packet loss
//#ifdef _DEBUG
//#define UDP_PACKETLOS_EMULATE
//#endif


#ifdef UDP_PACKETLOS_EMULATE
	#define UDP_PACKETLOS_RATE	5
	#define UDP_PACKETLOS_BOUND	1
#endif // #ifdef UDP_PACKETLOS_EMULATE


namespace BR {

	template class SharedPointerT <Net::Connection>;
	template class WeakPointerT < Net::Connection >;

namespace Net {

	// Create policy if not exist
	HRESULT IConnection::CreatePolicy( UINT uiPolicy )
	{
		HRESULT hr = S_SYSTEM_OK;

		if( uiPolicy >= POLICY_NETMAX )
			netErr( E_SYSTEM_INVALIDARG );

		if( m_pPolicy[uiPolicy] == NULL )// try create
		{ 
			switch( uiPolicy )
			{
			case POLICY_LOGIN:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetPolicyLogin( this ) );
				break;
			case POLICY_SVR_LOGIN:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetSvrPolicyLogin( this ) );
				break;

			case POLICY_GAME:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetPolicyGame( this ) );
				break;
			case POLICY_SVR_GAME:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetSvrPolicyGame( this ) );
				break;

			case POLICY_SERVER:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetPolicyServer( this ) );
				break;
			case POLICY_SVR_SERVER:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetSvrPolicyServer( this ) );
				break;

			case POLICY_LOGINSERVER:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetPolicyLoginServer( this ) );
				break;
			case POLICY_SVR_LOGINSERVER:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetSvrPolicyLoginServer( this ) );
				break;

			case POLICY_GAMESERVER:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetPolicyGameServer( this ) );
				break;
			case POLICY_SVR_GAMESERVER:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetSvrPolicyGameServer( this ) );
				break;
				
			case POLICY_GAMEMASTERSERVER:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetPolicyGameMasterServer( this ) );
				break;
			case POLICY_SVR_GAMEMASTERSERVER:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetSvrPolicyGameMasterServer( this ) );
				break;

			case POLICY_ENTITYSERVER:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetPolicyEntityServer( this ) );
				break;
			case POLICY_SVR_ENTITYSERVER:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetSvrPolicyEntityServer( this ) );
				break;

			case POLICY_GAMEPARTY:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetPolicyGameParty( this ) );
				break;
			case POLICY_SVR_GAMEPARTY:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetSvrPolicyGameParty( this ) );
				break;

			case POLICY_CLUSTERSERVER:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetPolicyClusterServer( this ) );
				break;
			case POLICY_SVR_CLUSTERSERVER:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetSvrPolicyClusterServer( this ) );
				break;

			case POLICY_PARTYMATCHING:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetPolicyPartyMatching( this ) );
				break;
			case POLICY_SVR_PARTYMATCHING:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetSvrPolicyPartyMatching( this ) );
				break;

			case POLICY_PARTYMATCHINGQUEUE:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetPolicyPartyMatchingQueue( this ) );
				break;
			case POLICY_SVR_PARTYMATCHINGQUEUE:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetSvrPolicyPartyMatchingQueue( this ) );
				break;

			case POLICY_GAMEINSTANCE:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetPolicyGameInstance( this ) );
				break;
			case POLICY_SVR_GAMEINSTANCE:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetSvrPolicyGameInstance( this ) );
				break;

			case POLICY_GAMEINSTANCEMANAGER:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetPolicyGameInstanceManager( this ) );
				break;
			case POLICY_SVR_GAMEINSTANCEMANAGER:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetSvrPolicyGameInstanceManager( this ) );
				break;

			case POLICY_GAMEPARTYMANAGER:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetPolicyGamePartyManager( this ) );
				break;
			case POLICY_SVR_GAMEPARTYMANAGER:
				netMem( m_pPolicy[uiPolicy] = new Policy::NetSvrPolicyGamePartyManager( this ) );
				break;

			case POLICY_RANKING:
				netMem(m_pPolicy[uiPolicy] = new Policy::NetPolicyRanking(this));
				break;
			case POLICY_SVR_RANKING:
				netMem(m_pPolicy[uiPolicy] = new Policy::NetSvrPolicyRanking(this));
				break;

			case POLICY_MONITORING:
				netMem(m_pPolicy[uiPolicy] = new Policy::NetPolicyMonitoring(this));
				break;
			case POLICY_SVR_MONITORING:
				netMem(m_pPolicy[uiPolicy] = new Policy::NetSvrPolicyMonitoring(this));
				break;

			default:
				netErr( E_SYSTEM_INVALIDARG );
				break;
			};
		}

	Proc_End:

		return hr;
	}



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network connection base class
	//


	Connection::Connection()
		: m_PendingSend(0)
		, m_socket(INVALID_SOCKET)
		, m_ulHeartbitTry(1000)
		, m_ulConnectingTimeOut(15 * 1000)
		, m_usSeqNone(0)
		, m_ulZeroLengthRecvCount(0)
		, m_lPendingRecvCount(0)
		, m_ulNetCtrlTime(DurationMS(0))
		, m_ulNetCtrlTryTime(DurationMS(0))
	{
		memset( &m_Statistic, 0, sizeof(m_Statistic) );
	}

	Connection::~Connection()
	{
		// Not need with abstract class
		m_RecvQueue.ClearQueue();
		m_SendGuaQueue.ClearQueue();
	}
	
	// Close connections socket handle
	void Connection::CloseSocket()
	{
		if (m_socket != INVALID_SOCKET)
		{
			NetSystem::CloseSocket(m_socket);
			SetSocket(INVALID_SOCKET);
		}
	}



	HRESULT Connection::ClearQueues()
	{
		m_RecvQueue.ClearQueue();
		m_SendGuaQueue.ClearQueue();

		// When the queue is cleared these synchronization variables need to be cleared
		m_usSeqNone = 0;
		return S_SYSTEM_OK;
	}


	// Make Ack packet and enqueue to SendNetCtrlqueue
	HRESULT Connection::SendNetCtrl( UINT uiCtrlCode, UINT uiSequence, Message::MessageID msgID, UINT64 UID )
	{
		HRESULT hr = S_SYSTEM_OK;
		HRESULT hrTem;
		MsgNetCtrl *pAckMsg = nullptr;
		Message::MessageData *pMsg = nullptr;

		if (uiCtrlCode == PACKET_NETCTRL_CONNECT || UID != 0)
		{
			netMem( pMsg = Message::MessageData::NewMessage( uiCtrlCode, sizeof(MsgNetCtrlConnect) ) );
			MsgNetCtrlConnect *pConMsg = (MsgNetCtrlConnect*)pMsg->GetMessageBuff();
			pConMsg->Address = GetConnectionInfo().Local;
			pConMsg->PeerUID = UID;
		}
		else
		{
			netMem( pMsg = Message::MessageData::NewMessage( uiCtrlCode, sizeof(MsgNetCtrl) ) );
		}

		pMsg->GetMessageHeader()->msgID.IDs.Mobile = false;

		pAckMsg = (MsgNetCtrl*)pMsg->GetMessageBuff();
		pAckMsg->msgID.IDSeq.Sequence = uiSequence;
		pAckMsg->rtnMsgID = msgID;

		pMsg->UpdateChecksum();



		hrTem = SendRaw(pMsg);
		//hrTem = GetNet()->SendMsg( this, pMsg );
		if( FAILED(hrTem) )
		{
			netTrace( TRC_GUARREANTEDCTRL, "NetCtrl Send failed : CID:{0}, msg:{1:X8}, seq:{2}, hr={3:X8}", 
							GetCID(), 
							msgID.ID, 
							uiSequence, 
							hrTem );

			// ignore io send fail except connection closed
			if( hrTem == ((HRESULT)E_NET_CONNECTION_CLOSED) )
			{
				goto Proc_End;
			}
		}

	Proc_End:

		return hr;
	}


	// Message count currently in recv queue
	SysUInt Connection::GetRecvMessageCount()
	{
		return m_RecvQueue.GetEnqueCount();
	}

	// Called on connection result
	void Connection::OnConnectionResult( HRESULT hrConnect )
	{
		EnqueueConnectionEvent( IConnection::Event( IConnection::Event::EVT_CONNECTION_RESULT, hrConnect)  );


		if( FAILED(hrConnect) )
		{
			netTrace(TRC_CONNECTION, "Connection failed CID:{0}, Dst={1}:{2}, hr={3:X8}", GetCID(), GetConnectionInfo().Remote.strAddr, GetConnectionInfo().Remote.usPort, hrConnect);
			if (GetConnectionState() != IConnection::STATE_DISCONNECTED)
			{
				CloseConnection();
			}
		}
		else
		{
			INet::Event netEvent(INet::Event::EVT_NEW_CONNECTION, this);

			netTrace( TRC_CONNECTION, "Connected CID:{0}, Dst={1}:{2}", GetCID(), GetConnectionInfo().Remote.strAddr, GetConnectionInfo().Remote.usPort );
			if (GetConnectionState() == IConnection::STATE_CONNECTING)
			{
				SetConnectionState(IConnection::STATE_CONNECTED);
				Assert(m_ConnectInfo.RemoteClass != NetClass::Unknown);

				EnqueueConnectionEvent(IConnection::Event(IConnection::Event::EVT_STATE_CHANGE, GetConnectionState()));

				GetNet()->EnqueueNetEvent(netEvent);
			}
		}
	}

	// Initialize packet synchronization
	HRESULT Connection::InitSynchronization()
	{
		UpdateConnectionTime();

		m_usSeqNone = 0;

		m_RecvQueue.ClearQueue();

		return S_SYSTEM_OK;
	}

	// Initialize connection
	HRESULT Connection::InitConnection( SOCKET socket, const ConnectionInformation &connectInfo )
	{
		HRESULT hr = S_SYSTEM_OK;

		netAssert(GetConnectionState() == STATE_DISCONNECTED);
		// Except client everybody should have port number when it gets here
		Assert(connectInfo.RemoteClass == NetClass::Client || connectInfo.Remote.usPort != 0);
		if (GetConnectionState() != STATE_DISCONNECTED)
			netChk( CloseConnection() );

		// event handler need to be reassigned after initconnection is called
		SetEventHandler(nullptr);

		m_EventQueue.ClearQueue();

		Assert(connectInfo.LocalClass != NetClass::Unknown);
		Assert(connectInfo.LocalID != 0);
		m_ConnectInfo = connectInfo;

		SetConnectionState(STATE_CONNECTING);

		netChk( InitSynchronization() );

		m_ulZeroLengthRecvCount = 0;
		m_lPendingRecvCount = 0;
		m_PendingSend = 0;


		memset( &m_Statistic, 0, sizeof(m_Statistic) );
		m_Statistic.ThroughputStartMS = Util::Time.GetTimeMs();

		m_socket = socket;

		SetSockAddr(m_sockAddrRemote, connectInfo.Remote.strAddr, connectInfo.Remote.usPort);

		m_ulNetCtrlTryTime = Util::Time.GetTimeMs();
		m_ulNetCtrlTime = Util::Time.GetTimeMs();

		EnqueueConnectionEvent(IConnection::Event(IConnection::Event::EVT_STATE_CHANGE, GetConnectionState()));

	Proc_End:

		netTrace(TRC_CONNECTION, "InitConnection CID:{0}, Addr:{1}:{2} hr:{3:X8}", GetCID(), connectInfo.Remote.strAddr, connectInfo.Remote.usPort, hr);

		return hr;
	}

	// Disconnect connection
	HRESULT Connection::Disconnect(const char* reason)
	{
		HRESULT hr = S_SYSTEM_OK;

		if( GetConnectionState() != STATE_DISCONNECTING 
			&& GetConnectionState() != STATE_DISCONNECTED)
		{
			if (reason == nullptr)
				reason = "Unknown";

			SetConnectionState(STATE_DISCONNECTING);

			EnqueueConnectionEvent(IConnection::Event(IConnection::Event::EVT_STATE_CHANGE, GetConnectionState()));

			netTrace( TRC_CONNECTION, "Entering Disconnect CID:{0}, reason:{1}", GetCID(), reason );
		}

	//Proc_End:

		return hr;
	}


	// Close connection
	HRESULT Connection::CloseConnection()
	{
		HRESULT hr = S_SYSTEM_OK;

		if (GetConnectionState() == IConnection::STATE_DISCONNECTED)
			goto Proc_End;

		SetConnectionState(STATE_DISCONNECTED);

		m_ulNetCtrlTime = m_ulNetCtrlTryTime = Util::Time.GetTimeMs();

		UpdateConnectionTime();
		//m_tConnectionTime = 0;

		netTrace( TRC_CONNECTION, "Connection Closed CID:{0}, Addr:{1}:{2}", GetCID(), GetConnectionInfo().Remote.strAddr, GetConnectionInfo().Remote.usPort );

		EnqueueConnectionEvent(IConnection::Event(IConnection::Event::EVT_DISCONNECTED, GetConnectionState()));

	Proc_End:

		return hr;
	}

	//// Get Recived message
	//HRESULT Connection::GetRecv( Message::MessageData* &pMsg )
	//{
	//	return m_RecvQueue.Dequeue( pMsg );
	//}


	HRESULT Connection::OnRecv( Message::MessageData *pMsg )
	{
		HRESULT hr = S_SYSTEM_OK;
	
		if (pMsg == nullptr)
			return hr;

		auto msgID = pMsg->GetMessageHeader()->msgID;

		// All messages must be decrypted before came here
		Assert(!msgID.IDs.Encrypted);

		m_Statistic.ThroughputBytes += pMsg->GetMessageSize();

		PrintDebugMessage( "RecvMsg:", pMsg );

		Assert( MemoryPool::CheckMemoryHeader( pMsg ) );
		Assert(pMsg->GetDataLength() == 0 || pMsg->GetMessageHeader()->Crc32 != 0);

		if (GetEventHandler() == nullptr )
		{
			netChk(GetRecvQueue().Enqueue(pMsg));
		}
		else if (FAILED(GetEventHandler()->OnRecvMessage(this, pMsg)))
		{
			netTrace(TRC_GUARREANTEDCTRL, "Failed to route a message to recv msg:{0}", msgID);
			if (pMsg != nullptr)
			{
				netChk(GetRecvQueue().Enqueue(pMsg));
			}
		}

		pMsg = nullptr;


	Proc_End:

		Assert(FAILED(hr) || pMsg == nullptr);
		Util::SafeRelease( pMsg );

		return hr;
	}


	// Query connection event
	HRESULT Connection::DequeueConnectionEvent( Event& curEvent )
	{
		return m_EventQueue.Dequeue( curEvent );
	}

	// Add network event to queue
	HRESULT Connection::EnqueueConnectionEvent(const IConnection::Event &evt)
	{
		if (GetEventHandler())
		{
			GetEventHandler()->OnConnectionEvent(this, evt);
		}
		else
		{
			return m_EventQueue.Enqueue(evt);
		}
		//return m_EventQueue.Enqueue( evt );

		return S_SYSTEM_OK;
	}


	// Get received Message
	HRESULT Connection::GetRecvMessage( Message::MessageData* &pIMsg )
	{
		HRESULT hr = S_SYSTEM_OK;

		pIMsg = nullptr;

		if( FAILED(GetRecvQueue().Dequeue( pIMsg )) )
		{
			hr = E_SYSTEM_FAIL;
			goto Proc_End;
		}

		{
			Message::MessageHeader* pMsgHeader = pIMsg->GetMessageHeader();
			UINT uiPolicy = pMsgHeader->msgID.IDs.Policy;
			if (uiPolicy == 0
				|| uiPolicy >= POLICY_NETMAX) // invalid policy
			{
				netErr(E_NET_BADPACKET_NOTEXPECTED);
			}
		}

	Proc_End:

		if( FAILED(hr) )
			Util::SafeRelease( pIMsg );

		return hr;
	}


	
} // namespace Net
} // namespace BR


