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

#include "StdAfx.h"
#include "Common/Thread.h"
#include "Common/BrAssert.h"
#include "Common/Utility.h"
#include "Common/HRESNet.h"
#include "Net/NetConst.h"
#include "Net/NetSystem.h"
#include "Net/NetTrace.h"
#include "Net/Connection.h"
#include "Net/NetServer.h"
#include "Net/NetMessage.h"
#include "Common/TimeUtil.h"

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
namespace Net {


	template class SharedPointerT <Connection>;
	template class WeakPointerT < Connection >;
	
	// Create policy if not exist
	HRESULT IConnection::CreatePolicy( UINT uiPolicy )
	{
		HRESULT hr = S_OK;

		if( uiPolicy >= BR::POLICY_NETMAX )
			netErr( E_INVALIDARG );

		if( m_pPolicy[uiPolicy] == NULL )// try create
		{ 
			switch( uiPolicy )
			{
			case BR::POLICY_LOGIN:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetPolicyLogin( this ) );
				break;
			case BR::POLICY_SVR_LOGIN:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetSvrPolicyLogin( this ) );
				break;

			case BR::POLICY_GAME:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetPolicyGame( this ) );
				break;
			case BR::POLICY_SVR_GAME:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetSvrPolicyGame( this ) );
				break;

			case BR::POLICY_SERVER:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetPolicyServer( this ) );
				break;
			case BR::POLICY_SVR_SERVER:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetSvrPolicyServer( this ) );
				break;

			case BR::POLICY_LOGINSERVER:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetPolicyLoginServer( this ) );
				break;
			case BR::POLICY_SVR_LOGINSERVER:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetSvrPolicyLoginServer( this ) );
				break;

			case BR::POLICY_GAMESERVER:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetPolicyGameServer( this ) );
				break;
			case BR::POLICY_SVR_GAMESERVER:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetSvrPolicyGameServer( this ) );
				break;
				
			case BR::POLICY_GAMEMASTERSERVER:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetPolicyGameMasterServer( this ) );
				break;
			case BR::POLICY_SVR_GAMEMASTERSERVER:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetSvrPolicyGameMasterServer( this ) );
				break;

			case BR::POLICY_ENTITYSERVER:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetPolicyEntityServer( this ) );
				break;
			case BR::POLICY_SVR_ENTITYSERVER:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetSvrPolicyEntityServer( this ) );
				break;

			case BR::POLICY_GAMEPARTY:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetPolicyGameParty( this ) );
				break;
			case BR::POLICY_SVR_GAMEPARTY:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetSvrPolicyGameParty( this ) );
				break;

			case BR::POLICY_CLUSTERSERVER:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetPolicyClusterServer( this ) );
				break;
			case BR::POLICY_SVR_CLUSTERSERVER:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetSvrPolicyClusterServer( this ) );
				break;

			case BR::POLICY_PARTYMATCHING:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetPolicyPartyMatching( this ) );
				break;
			case BR::POLICY_SVR_PARTYMATCHING:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetSvrPolicyPartyMatching( this ) );
				break;

			case BR::POLICY_PARTYMATCHINGQUEUE:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetPolicyPartyMatchingQueue( this ) );
				break;
			case BR::POLICY_SVR_PARTYMATCHINGQUEUE:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetSvrPolicyPartyMatchingQueue( this ) );
				break;

			case BR::POLICY_GAMEINSTANCE:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetPolicyGameInstance( this ) );
				break;
			case BR::POLICY_SVR_GAMEINSTANCE:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetSvrPolicyGameInstance( this ) );
				break;

			case BR::POLICY_GAMEINSTANCEMANAGER:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetPolicyGameInstanceManager( this ) );
				break;
			case BR::POLICY_SVR_GAMEINSTANCEMANAGER:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetSvrPolicyGameInstanceManager( this ) );
				break;

			case BR::POLICY_GAMEPARTYMANAGER:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetPolicyGamePartyManager( this ) );
				break;
			case BR::POLICY_SVR_GAMEPARTYMANAGER:
				netMem( m_pPolicy[uiPolicy] = new BR::Policy::NetSvrPolicyGamePartyManager( this ) );
				break;

			case BR::POLICY_RANKING:
				netMem(m_pPolicy[uiPolicy] = new BR::Policy::NetPolicyRanking(this));
				break;
			case BR::POLICY_SVR_RANKING:
				netMem(m_pPolicy[uiPolicy] = new BR::Policy::NetSvrPolicyRanking(this));
				break;

			case BR::POLICY_MONITORING:
				netMem(m_pPolicy[uiPolicy] = new BR::Policy::NetPolicyMonitoring(this));
				break;
			case BR::POLICY_SVR_MONITORING:
				netMem(m_pPolicy[uiPolicy] = new BR::Policy::NetSvrPolicyMonitoring(this));
				break;

			default:
				netErr( E_INVALIDARG );
				break;
			};
		}

	Proc_End:

		return hr;
	}


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Packet message queue class
	//

	MsgQueue::MsgQueue( UINT uiNumElePerPage )
		:BR::PageQueue<Message::MessageData*>( uiNumElePerPage )
	{
	}

	MsgQueue::~MsgQueue()
	{
		ClearQueue();
	}

	// Clear queue element
	void MsgQueue::ClearQueue()
	{
		Message::MessageData* data = NULL;
		HRESULT hr = S_OK;
		while( Dequeue( data ) == S_OK )
		{
			if( data ) data->Release();
		}
	}


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network connection base class
	//


	Connection::Connection()
		: m_socket(INVALID_SOCKET)
		, m_usSeqNone(0)
		, m_ulNetCtrlTime(0)
		, m_ulNetCtrlTryTime(0)
		, m_ulHeartbitTry(1000)
		, m_ulConnectingTimeOut(15*1000)
		, m_ulZeroLengthRecvCount(0)
		, m_lPendingRecvCount(0)
		, m_PendingSend(0)
	{
		memset( &m_Statistic, 0, sizeof(m_Statistic) );
	}

	Connection::~Connection()
	{
		// Not need with abstract class
		m_RecvQueue.ClearQueue();

//#ifdef BR_SERVER
//		for( INT iPolicy = 0; iPolicy < BR::POLICY_NETMAX; iPolicy++ )
//#else
//		for( INT iPolicy = 0; iPolicy < BR::POLICY_CLIENTNETMAX; iPolicy++ )
//#endif
//		{
//			Util::SafeDelete(m_pPolicy[iPolicy]);
//		}
	}
	

	//// from iSharedObj
	//void Connection::DeleteThis() const
	//{
	//	Assert(GetRefCount() <= 0);
	//	//AssertRel(!"Do not use this" );
	//	delete this;
	//}


	HRESULT Connection::ClearQueues()
	{
		m_RecvQueue.ClearQueue();
		//m_EventQueue.ClearQueue();

		// When the queue is cleared these synchronization variables need to be cleared
		m_usSeqNone = 0;
		return S_OK;
	}


	// Make Ack packet and enqueue to SendNetCtrlqueue
	HRESULT Connection::SendNetCtrl( UINT uiCtrlCode, UINT uiSequence, Message::MessageID msgID, UINT64 UID )
	{
		HRESULT hr = S_OK;

		MsgNetCtrl *pAckMsg = NULL;
		Message::MessageData *pMsg = NULL;

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

		HRESULT hrTem = GetNet()->SendMsg( this, pMsg );
		if( FAILED(hrTem) )
		{
			netTrace( TRC_GUARREANTEDCTRL, "NetCtrl Send failed : CID:%0%, msg:%1%, seq:%2%, hr=%3%", 
							GetCID(), 
							BR::Arg<UINT32>(msgID.ID,-1,16), 
							uiSequence, 
							BR::Arg<UINT32>(hrTem,-1,16) );

			// ignore io send fail except connection closed
			if( hrTem == E_NET_CONNECTION_CLOSED )
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
			netTrace(TRC_CONNECTION, "Connection failed CID:%0%, Dst=%1%:%2%, hr=%3%", GetCID(), GetConnectionInfo().Remote.strAddr, GetConnectionInfo().Remote.usPort, BR::Arg<UINT32>(hrConnect, -1, 16));
			if (GetConnectionState() != IConnection::STATE_DISCONNECTED)
			{
				CloseConnection();
			}
		}
		else
		{
			netTrace( TRC_CONNECTION, "Connected CID:%0%, Dst=%1%:%2%", GetCID(), GetConnectionInfo().Remote.strAddr, GetConnectionInfo().Remote.usPort, BR::Arg<UINT32>(hrConnect,-1,16) );
			if (GetConnectionState() == IConnection::STATE_CONNECTING)
			{
				SetConnectionState(IConnection::STATE_CONNECTED);
				Assert(m_ConnectInfo.RemoteClass != NetClass::Unknown);

				EnqueueConnectionEvent(IConnection::Event(IConnection::Event::EVT_STATE_CHANGE, GetConnectionState()));

				GetNet()->EnqueueNetEvent( INet::Event( INet::Event::EVT_NEW_CONNECTION, this ) );
			}
		}
	}

	// Initialize packet synchronization
	HRESULT Connection::InitSynchronization()
	{
		UpdateConnectionTime();

		m_usSeqNone = 0;

		m_RecvQueue.ClearQueue();

		return S_OK;
	}

	// Initialize connection
	HRESULT Connection::InitConnection( SOCKET socket, const ConnectionInformation &connectInfo )
	{
		HRESULT hr = S_OK;

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

		netTrace(TRC_CONNECTION, "InitConnection CID:%0%, Addr:%1%:%2% hr:%3%", GetCID(), connectInfo.Remote.strAddr, connectInfo.Remote.usPort, ArgHex32(hr));

		return hr;
	}

	// Disconnect connection
	HRESULT Connection::Disconnect()
	{
		HRESULT hr = S_OK;

		if( GetConnectionState() != STATE_DISCONNECTING 
			&& GetConnectionState() != STATE_DISCONNECTED)
		{
			SetConnectionState(STATE_DISCONNECTING);

			EnqueueConnectionEvent(IConnection::Event(IConnection::Event::EVT_STATE_CHANGE, GetConnectionState()));

			netTrace( TRC_CONNECTION, "Entering Disconnect CID:%0%", GetCID() );
		}

	Proc_End:

		return hr;
	}


	// Close connection
	HRESULT Connection::CloseConnection()
	{
		HRESULT hr = S_OK;

		if (GetConnectionState() == IConnection::STATE_DISCONNECTED)
			goto Proc_End;

		SetConnectionState(STATE_DISCONNECTED);

		m_ulNetCtrlTime = m_ulNetCtrlTryTime = Util::Time.GetTimeMs();

		UpdateConnectionTime();
		//m_tConnectionTime = 0;

		netTrace( TRC_CONNECTION, "Connection Closed CID:%0%, Addr:%1%:%2%", GetCID(), GetConnectionInfo().Remote.strAddr, GetConnectionInfo().Remote.usPort );

		EnqueueConnectionEvent(IConnection::Event(IConnection::Event::EVT_DISCONNECTED, GetConnectionState()));

	Proc_End:

		return hr;
	}

	// Get Recived message
	HRESULT Connection::GetRecv( Message::MessageData* &pMsg )
	{
		return m_RecvQueue.Dequeue( pMsg );
	}


	HRESULT Connection::OnRecv( Message::MessageData *pMsg )
	{
		HRESULT hr = S_OK;
	
		if (pMsg == nullptr)
			return hr;

		auto msgID = pMsg->GetMessageHeader()->msgID;

		// All messages must be decrypted before came here
		Assert(!msgID.IDs.Encrypted);

		m_Statistic.ThroughputBytes += pMsg->GetMessageSize();

		PrintDebugMessage( "RecvMsg:", pMsg );

		Assert( MemoryPool::CheckMemoryHeader( pMsg ) );
		Assert(pMsg->GetMessageHeader()->Crc32 != 0);

		if (GetEventHandler() == nullptr )
		{
			netChk(GetRecvQueue().Enqueue(pMsg));
		}
		else if (FAILED(GetEventHandler()->OnRecvMessage(this, pMsg)))
		{
			netTrace(TRC_GUARREANTEDCTRL, "Failed to route a message to recv msg:%0%", msgID);
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

		return S_OK;
	}


	// Get received Message
	HRESULT Connection::GetRecvMessage( Message::MessageData* &pIMsg )
	{
		HRESULT hr = S_OK;

		pIMsg = nullptr;

		if( FAILED(GetRecvQueue().Dequeue( pIMsg )) )
		{
			hr = E_FAIL;
			goto Proc_End;
		}

		Message::MessageHeader* pMsgHeader = pIMsg->GetMessageHeader();
		UINT uiPolicy = pMsgHeader->msgID.IDs.Policy;
		if( uiPolicy == 0 
			|| uiPolicy >= BR::POLICY_NETMAX ) // invalid policy
		{
			netErr( E_NET_BADPACKET_NOTEXPECTED );
		}

	Proc_End:

		if( FAILED(hr) )
			Util::SafeRelease( pIMsg );

		return hr;
	}


	
} // namespace Net
} // namespace BR


