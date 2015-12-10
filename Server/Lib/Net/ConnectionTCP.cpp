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



#include "stdafx.h"
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


BR_MEMORYPOOL_IMPLEMENT(Net::ConnectionTCPClient);
BR_MEMORYPOOL_IMPLEMENT(Net::ConnectionTCPServer);


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
	//	TCP Network connection class
	//

	// Constructor
	ConnectionTCP::ConnectionTCP()
		: m_lGuarantedSent(0)
		, m_lGuarantedAck(0)
		, m_uiRecvTemUsed(0)
		, m_uiSendNetCtrlCount(0)
		, m_isClientSide(false)
		, m_isActuallyConnected(true)
	{
		SetSocket( INVALID_SOCKET );
		m_bufRecvTem.resize( Const::PACKET_SIZE_MAX*2 );

		SetHeartbitTry(Const::TCP_HEARTBIT_START_TIME);

		SetWriteQueue(new WriteBufferQueue);
	}

	ConnectionTCP::~ConnectionTCP()
	{
		//Assert( SUCCEEDED( CloseConnection() ) );
		if( GetSocket() != INVALID_SOCKET )
			NetSystem::CloseSocket( GetSocket() );

		if (GetWriteQueue()) delete GetWriteQueue();
	}



	// Process network control message
	HRESULT ConnectionTCP::ProcNetCtrl( const MsgNetCtrl* pNetCtrl )
	{
		HRESULT hr = S_OK;
		Message::MessageData *pIMsg = NULL;

		switch( pNetCtrl->msgID.IDs.MsgCode )
		{
		case NetCtrlCode_Ack:
			if( pNetCtrl->rtnMsgID.IDs.Type == Message::MSGTYPE_NETCONTROL )
			{
				switch( pNetCtrl->rtnMsgID.IDs.MsgCode )
				{
				case NetCtrlCode_Disconnect:
					if (GetConnectionState() != IConnection::STATE_DISCONNECTED)
						netChk( CloseConnection() );
					break;
				case NetCtrlCode_HeartBit:
					m_ulNetCtrlTime = Util::Time.GetTimeMs();
					break;
				case NetCtrlCode_Connect:
					if (GetConnectionState() == STATE_CONNECTING)
					{
						NetClass RemoteClass = (NetClass)pNetCtrl->msgID.IDSeq.Sequence;
						Assert(pNetCtrl->Length == sizeof(MsgNetCtrlConnect));
						auto pConnectCtrl = ((MsgNetCtrlConnect*)pNetCtrl);
						m_ConnectInfo.SetRemoteInfo(RemoteClass, pConnectCtrl->PeerUID);
						m_ConnectInfo.Remote = pConnectCtrl->Address;
						OnConnectionResult(S_OK);
					}
					break;
				};
			}
			else
			{
				// Remove from Guranted queue
				if( pNetCtrl->rtnMsgID.IDs.Reliability )
				{
					m_lGuarantedAck.fetch_add(1, std::memory_order_relaxed);
				}
			}

			break;
		case NetCtrlCode_Nack:
			if( pNetCtrl->rtnMsgID.IDs.Type == Message::MSGTYPE_NETCONTROL )// connecting process
			{
				switch( pNetCtrl->rtnMsgID.IDs.MsgCode )
				{
				case NetCtrlCode_Disconnect:
					break;
				case NetCtrlCode_Connect:
					// Protocol version mismatch
					OnConnectionResult( E_NET_PROTOCOL_VERSION_MISMATCH );
					netChk( Disconnect() );
					break;
				case NetCtrlCode_HeartBit:
					break;
				default:
					break;
				};
			}
			break;
		case NetCtrlCode_HeartBit:
			m_ulNetCtrlTime = Util::Time.GetTimeMs();
			netChk(SendNetCtrl(PACKET_NETCTRL_ACK, pNetCtrl->msgID.IDSeq.Sequence, pNetCtrl->msgID));
			break;
		case NetCtrlCode_Connect:
		{
			m_ulNetCtrlTime = Util::Time.GetTimeMs();

			if( pNetCtrl->Length < sizeof(MsgNetCtrlConnect) )
			{
				netTrace( Trace::TRC_WARN, "HackWarn : Invalid Connect packet CID:{0}, Addr {1}", GetCID(), GetConnectionInfo().Remote );
				netChk( CloseConnection() );
				netErr( E_UNEXPECTED );
			}

			if (GetConnectionState() == STATE_CONNECTING || GetConnectionState() == STATE_CONNECTED)// ServerTCP connection will occure this case
			{
				const MsgNetCtrlConnect *pNetCtrlCon = (const MsgNetCtrlConnect*)pNetCtrl;
				UINT recvProtocolVersion = pNetCtrl->rtnMsgID.ID;
				NetClass RemoteClass = (NetClass)pNetCtrl->msgID.IDSeq.Sequence;
				m_ConnectInfo.Remote = pNetCtrlCon->Address;

				if (recvProtocolVersion != BR_PROTOCOL_VERSION)
				{
					netChk(SendNetCtrl(PACKET_NETCTRL_NACK, pNetCtrl->msgID.IDSeq.Sequence, pNetCtrl->msgID));
					if (GetConnectionState() != STATE_CONNECTED)
						OnConnectionResult( E_NET_PROTOCOL_VERSION_MISMATCH );
					netChk( Disconnect() );
				}
				else if( GetConnectionInfo().RemoteClass != NetClass::Unknown && RemoteClass != GetConnectionInfo().RemoteClass )
				{
					netChk(SendNetCtrl(PACKET_NETCTRL_NACK, pNetCtrl->msgID.IDSeq.Sequence, pNetCtrl->msgID));
					if (GetConnectionState() != STATE_CONNECTED)
						OnConnectionResult(E_NET_INVALID_NETCLASS);
					netChk( Disconnect() );
				}
				else
				{
					Assert(GetConnectionInfo().LocalClass != NetClass::Unknown);
					netChk(SendNetCtrl(PACKET_NETCTRL_ACK, (UINT)GetConnectionInfo().LocalClass, pNetCtrl->msgID, GetConnectionInfo().LocalID));
					if (GetConnectionState() != STATE_CONNECTED)
					{
						m_ConnectInfo.SetRemoteInfo(RemoteClass, pNetCtrlCon->PeerUID);
						OnConnectionResult(S_OK);
					}
				}
			}
			break;
		}
		case NetCtrlCode_Disconnect:
			netChk(SendNetCtrl(PACKET_NETCTRL_ACK, pNetCtrl->msgID.IDSeq.Sequence, pNetCtrl->msgID));
			netChk(SendNetCtrl(PACKET_NETCTRL_DISCONNECT, pNetCtrl->msgID.IDSeq.Sequence, pNetCtrl->msgID));
			netChk( CloseConnection() );
			break;
		default:
			netTrace( Trace::TRC_WARN, "HackWarn : Invalid packet CID:{0}, Addr {1}", GetCID(), GetConnectionInfo().Remote );
			netChk( CloseConnection() );
			netErr( E_UNEXPECTED );
			break;
		};


	Proc_End:

		if( pIMsg ) pIMsg->Release();

		return hr;
	}

	HRESULT ConnectionTCP::Recv(IOBUFFER_READ* pIOBuffer)
	{
		HRESULT hr = S_OK, hrErr = S_OK;

		if (GetConnectionState() == IConnection::STATE_DISCONNECTED)
			return E_NET_CONNECTION_CLOSED;

		netChkPtr(pIOBuffer);
		pIOBuffer->SetupRecvTCP(GetCID());

		hrErr = NetSystem::Recv(GetSocket(), pIOBuffer);
		if (FAILED(hrErr))
		{
			switch (hrErr)
			{
			case E_NET_CONNABORTED:
			case E_NET_CONNRESET:
			case E_NET_NETRESET:
			case E_NET_NOTCONN:
			case E_NET_NOTSOCK:
			case E_NET_SHUTDOWN:
				netTrace(Trace::TRC_WARN, "TCP Read failed, Connection Reset CID:{0}, err:{1:X8}, pending:{2}", GetCID(), hrErr, GetPendingRecvCount());
				// Send fail by connection close
				// Need to disconnect
				CloseConnection();
				netErrSilent(E_NET_CONNECTION_CLOSED);
				break;
			default:
				netTrace(Trace::TRC_ERROR, "TCP Read IOCP Pending failed with CID {0}, err:{1:X8}", GetCID(), hrErr);
				netErr(E_NET_IO_RECV_FAIL);
				break;
			case E_NET_IO_PENDING:
			case E_NET_WOULDBLOCK:
				// Recv is pended
				break;
			case E_NET_TRY_AGAIN:
				// try again
				break;
			};
		}
		else
		{
			// IOCP will handle this
			//Assert(dwNumberOfByte > 0);
			//OnRecv(dwNumberOfByte, (BYTE*)pOver->wsaBuff.buf);
		}


	Proc_End:

		return hr;
	}

	// called when reciving message
	HRESULT ConnectionTCP::OnIORecvCompleted( HRESULT hrRes, IOBUFFER_READ *pIOBuffer )
	{
		HRESULT hr = S_OK;

		if( pIOBuffer->CID != GetCID() )
			netErr( E_INVALIDARG );

		if( FAILED( hrRes ) )
		{
			switch( hrRes )
			{
			case E_NET_CONNECTION_CLOSED:
			case E_NET_IO_ABORTED:
				netChk( CloseConnection() );
				break;
			default:
				netTrace(TRC_TCPRECVRAW, "Recv Msg Failed, SvrTCP, CID {0}, hr={1:X8}", GetCID(), hrRes);
				PendingRecv();
				break;
			};
		}
		else
		{
			netChk( OnRecv(pIOBuffer->TransferredSize, (BYTE*)pIOBuffer->buffer) );
			PendingRecv();
		}

	Proc_End:

		DecPendingRecvCount();

		netTrace(TRC_TCPRECVRAW, "TCP Recv CID:{0}, pending:{1}, hr:{2:X8}", GetCID(), GetPendingRecvCount(), hr);

		return hr;
	}

	// called when Send completed
	HRESULT ConnectionTCP::OnIOSendCompleted( HRESULT hrRes, IOBUFFER_WRITE *pIOBuffer )
	{
		Util::SafeDeleteArray( pIOBuffer->pSendBuff );
		Util::SafeRelease( pIOBuffer->pMsgs );
		NetSystem::FreeBuffer( pIOBuffer );
		m_PendingSend.fetch_sub(1, std::memory_order_release);
		return S_OK;
	}

	// Clear Queue
	HRESULT ConnectionTCP::ClearQueues()
	{
		return S_OK;
	}

	// Pending recv New one
	HRESULT ConnectionTCP::PendingRecv()
	{
		HRESULT hr = S_OK;
		IOBUFFER_READ *pOver = nullptr;

		if (!NetSystem::IsProactorSystem())
			return S_OK;

		// On client side, we need to check writable status by calling connect again
		if (m_isClientSide && !m_isActuallyConnected && GetConnectionState() == IConnection::ConnectionState::STATE_CONNECTING)
		{
			m_isActuallyConnected = Connect() == S_OK;
			if (!m_isActuallyConnected)
				return S_OK;
		}

		pOver = GetRecvBuffer();
		netChk(Recv(pOver));

		IncPendingRecvCount();


	Proc_End:

		netTrace(TRC_TCPRECVRAW, "Pending Recv CID:{0}, pending:{1}, hr:{2:X8}", GetCID(), GetPendingRecvCount(), hr);

		return hr;
	}



	// Called on connection result
	void ConnectionTCP::OnConnectionResult( HRESULT hrConnect )
	{
		Connection::OnConnectionResult( hrConnect );

		if( SUCCEEDED( hrConnect ) )
		{
		}
		else
		{
			CloseConnection();
		}

	}


	// Initialize connection
	HRESULT ConnectionTCP::InitConnection( SOCKET socket, const ConnectionInformation &connectInfo )
	{
		if (GetSocket() != INVALID_SOCKET)
		{
			CloseSocket();
		}

		m_lGuarantedSent = 0;
		m_lGuarantedAck = 0;

		m_uiSendNetCtrlCount = 0;
		m_uiRecvTemUsed = 0;

		m_isClientSide = false;
		m_isActuallyConnected = true;

		Assert(connectInfo.LocalClass != NetClass::Unknown);

		// ignore incomming socket value
		return Connection::InitConnection( socket, connectInfo );
	}

	HRESULT ConnectionTCP::Connect()
	{
		HRESULT hr = S_OK;
		int connResult;

		ResetZeroRecvCount();

		AssertRel(GetConnectionState() == STATE_CONNECTING || GetConnectionState() == STATE_DISCONNECTED);

		connResult = connect(GetSocket(), (sockaddr*)&GetRemoteSockAddr(), sizeof(GetRemoteSockAddr()));
		if (connResult == SOCKET_ERROR)
		{
			auto lastError = GetLastWSAHRESULT();
			switch (lastError)
			{
			case E_NET_INPROGRESS:
			case E_NET_WOULDBLOCK:  // First call need to wait
			case E_NET_ALREADY:		// called again, still need to wait
				hr = S_FALSE;
				break;
			case E_NET_ISCONN:		// Connection estabalished
				hr = S_OK;
				break;
			default:
				netTrace(Trace::TRC_WARN, "Connection try is failed, RemoteID:{0}, hr:{1:X8}", GetConnectionInfo().RemoteID, lastError);
				hr = lastError;
			}
		}

		m_isClientSide = true;
		m_isActuallyConnected = false; // only client side need to check this condition

	//Proc_End:

		if (FAILED(hr))
		{
			SetConnectionState(STATE_DISCONNECTED);
			CloseSocket();
		}

		return hr;

	}

	// Wait connection event
	HRESULT ConnectionTCP::WaitConnect()
	{
		HRESULT hr = S_OK;

		// disable wait for non-windows platform
#if WINDOWS
		HANDLE hEvtConnection = WSACreateEvent();
		if (hEvtConnection == WSA_INVALID_EVENT)
		{
			netTrace(Trace::TRC_ERROR, "Failed to create connection event handler err={0:X8}", GetLastWSAHRESULT());
			netErr(GetLastWSAHRESULT());
		}


		if (WSAEventSelect(GetSocket(), hEvtConnection, FD_CONNECT) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Failed to set connection event handler err={0:X8}", GetLastWSAHRESULT());
			netErr(GetLastWSAHRESULT());
		}

		DWORD dwWaitRes = WSAWaitForMultipleEvents(1, &hEvtConnection, TRUE, Const::CONNECTION_TIMEOUT, TRUE);

		if (dwWaitRes == WSA_WAIT_TIMEOUT || dwWaitRes == WSA_WAIT_FAILED)
		{
			OnConnectionResult(E_NET_CONNECTION_TIMEOUT);
			hr = E_NET_CONNECTION_TIMEOUT;
		}
		else
		{

			if (WSAEventSelect(GetSocket(), hEvtConnection, FD_WRITE | FD_READ) == SOCKET_ERROR)
			{
				netTrace(Trace::TRC_ERROR, "Failed to set connection event handler err={0:X8}", GetLastWSAHRESULT());
				netErr(GetLastWSAHRESULT());
			}

			DWORD dwWaitRes2 = WSAWaitForMultipleEvents(1, &hEvtConnection, TRUE, Const::CONNECTION_TIMEOUT, TRUE);

			if (dwWaitRes2 == WSA_WAIT_TIMEOUT || dwWaitRes2 == WSA_WAIT_FAILED)
			{
				OnConnectionResult(E_NET_CONNECTION_TIMEOUT);
				netErr(E_NET_CONNECTION_TIMEOUT);
			}

			if (WSAEventSelect(GetSocket(), hEvtConnection, 0) == SOCKET_ERROR)
			{
				// just reporting this error
				netTrace(Trace::TRC_ERROR, "Failed to cancel connection event handler err={0:X8}", GetLastWSAHRESULT());
			}
		}

	Proc_End:


		if (hEvtConnection != WSA_INVALID_EVENT)
		{
			WSACloseEvent(hEvtConnection);
		}
#endif

		return hr;
	}

	// Close connection
	HRESULT ConnectionTCP::CloseConnection()
	{
		HRESULT hr = S_OK;

		//socket
		m_uiRecvTemUsed = 0;
		ResetZeroRecvCount();

		netChk(Connection::CloseConnection() );

		if (GetConnectionState() == IConnection::STATE_DISCONNECTED)
			goto Proc_End;

		CloseSocket();

	Proc_End:

		return hr;
	}



	// called when incomming message occure
	HRESULT ConnectionTCP::OnRecv( UINT uiBuffSize, const BYTE* pBuff )
	{
		HRESULT hr = S_OK;
		Message::MessageData *pMsg = NULL;

		netTrace(TRC_TCPRECVRAW, "TCP RecvBuf Len={0}", uiBuffSize);

		if( uiBuffSize == 0 )
		{
			IncZeroRecvCount();
			if( GetZeroRecvCount() > (ULONG)Const::CONNECTION_ZEROPACKET_MAX )
			{
				//Disconnect();
			}
			goto Proc_End;
		}

		while( uiBuffSize > 0 )
		{
			Message::MessageHeader *pMsgHdr = NULL;

			ResetZeroRecvCount();

			if( m_uiRecvTemUsed == 0 )
			{
				pMsgHdr = (Message::MessageHeader*)pBuff;
				if( uiBuffSize < sizeof(Message::MessageHeader) || uiBuffSize < pMsgHdr->Length ) // too small to make packet
				{
					// send all data to recv temporary buffer and return
					m_uiRecvTemUsed = uiBuffSize;
					// relocate buffer if too small
					if( m_bufRecvTem.size() < uiBuffSize )
						m_bufRecvTem.resize( ((uiBuffSize+m_bufRecvTem.size()) / m_bufRecvTem.size()) * m_bufRecvTem.size() );

					memcpy( m_bufRecvTem.data(), pBuff, uiBuffSize );

					uiBuffSize = 0;
					break;
				}

				if( pMsgHdr->Length < sizeof(Message::MessageHeader) )
				{
					// too small invalid packet
					netErr( E_UNEXPECTED );
				}

				netMem( pMsg = Message::MessageData::NewMessage( pMsgHdr->msgID.ID, pMsgHdr->Length, pBuff ) );

				hr = OnRecv( pMsg );
				pMsg = NULL;
				netChk( hr );

				uiBuffSize -= pMsgHdr->Length;
				pBuff += pMsgHdr->Length;
			}
			else
			{
				UINT uiCopySize = 0;

				// copy header
				if( m_uiRecvTemUsed < sizeof(Message::MessageHeader) )
				{
					uiCopySize = sizeof(Message::MessageHeader) - m_uiRecvTemUsed;
					if( uiBuffSize < uiCopySize )
					{
						// buffer too small to make header. append data and return
						memcpy( m_bufRecvTem.data() + m_uiRecvTemUsed, pBuff, uiBuffSize );
						m_uiRecvTemUsed += uiBuffSize;
						break;
					}

					memcpy( m_bufRecvTem.data() + m_uiRecvTemUsed, pBuff, uiCopySize );

					uiBuffSize -= uiCopySize;
					pBuff += uiCopySize;
					m_uiRecvTemUsed += uiCopySize;

					if( m_uiRecvTemUsed < sizeof(Message::MessageHeader) )
						break;
				}

				pMsgHdr = (Message::MessageHeader*)m_bufRecvTem.data();

				// if Temporary buffer is too small then reallocate
				if( m_bufRecvTem.size() < pMsgHdr->Length )
					m_bufRecvTem.resize( pMsgHdr->Length );

				// append remain body
				if( pMsgHdr->Length < m_uiRecvTemUsed )
				{
					netErr( E_NET_BADPACKET_SIZE );
				}

				uiCopySize = pMsgHdr->Length - m_uiRecvTemUsed;
				if( uiBuffSize < uiCopySize )
				{
					// buffer too small to make body. append data and return
					memcpy( m_bufRecvTem.data() + m_uiRecvTemUsed, pBuff, uiBuffSize );

					m_uiRecvTemUsed += uiBuffSize;

					// remain data will be occure at next parts of packet
					break;
				}

				// append remain body
				memcpy( m_bufRecvTem.data() + m_uiRecvTemUsed, pBuff, uiCopySize );
				uiBuffSize -= uiCopySize;
				pBuff += uiCopySize;
				m_uiRecvTemUsed = 0;

				netMem( pMsg = Message::MessageData::NewMessage( pMsgHdr->msgID.ID, pMsgHdr->Length, m_bufRecvTem.data() ) );

				hr = OnRecv( pMsg );
				pMsg = NULL;
				netChk( hr );
			}

		}

	Proc_End:

		Util::SafeRelease( pMsg );

		return hr;
	}



	HRESULT ConnectionTCP::OnRecv( Message::MessageData *pMsg )
	{
		HRESULT hr = S_OK;
		Message::MessageHeader *pMsgHeader = pMsg->GetMessageHeader();

		if( pMsgHeader->msgID.IDs.Type == Message::MSGTYPE_NETCONTROL )
		{
			netTrace( TRC_TCPNETCTRL, "TCP Ctrl Recv ip:{0}, msg:{1}, Len:%2%", 
				GetConnectionInfo().Remote, 
				pMsgHeader->msgID, pMsgHeader->Length );

			netChk( ProcNetCtrl( (MsgNetCtrl*)pMsgHeader ) );
		}
		else
		{
			netChk( pMsg->ValidateChecksumNDecrypt() );

			if( pMsgHeader->msgID.IDs.Reliability )
			{
				SendNetCtrl(PACKET_NETCTRL_ACK, pMsgHeader->msgID.IDSeq.Sequence, pMsgHeader->msgID);
			}

			hr  = Connection::OnRecv( pMsg );
			pMsg = NULL;
			netChk( hr );
		}

	Proc_End:

		Util::SafeRelease( pMsg );

		return hr;
	}

	// Send message to connection with network device
	HRESULT ConnectionTCP::SendBuffer(IOBUFFER_WRITE *pSendBuffer)
	{
		HRESULT hr = S_OK, hrErr = S_OK;

		netChkPtr(pSendBuffer);

		hrErr = NetSystem::Send(GetSocket(), pSendBuffer);
		switch (hrErr)
		{
		case S_OK:
		case E_NET_IO_PENDING:
		case E_NET_WOULDBLOCK:
			break;
		case E_NET_TRY_AGAIN:
			hr = hrErr;
			break;
		case E_NET_CONNABORTED:
		case E_NET_CONNRESET:
		case E_NET_NETRESET:
		case E_NET_NOTCONN:
		case E_NET_NOTSOCK:
		case E_NET_SHUTDOWN:
			// Send fail by connection close
			// Need to disconnect
			Disconnect();
			hr = E_NET_CONNECTION_CLOSED;
			goto Proc_End;
			break;
		default:
			netErr(E_NET_IO_SEND_FAIL);
			break;
		};

	Proc_End:

		if (FAILED(hr))
		{
			//if (pSendBuffer)
			//{
			//	Util::SafeRelease(pSendBuffer->pMsgs);
			//	Net::NetSystem::FreeBuffer(pSendBuffer);
			//}

			if (hr != E_NET_IO_SEND_FAIL)
			{
				netTrace(Trace::TRC_ERROR, "TCP Send Failed, CID:{3}, ip:{0}, err:{1:X8}, hr:{2:X8}", GetConnectionInfo().Remote, hrErr, hr, GetCID());
			}
			else
				return S_OK;
		}
		else
		{
			//netTrace(TRC_TCPSENDRAW, "TCP Send Raw CID:{1}, ip:{0}", GetConnectionInfo().Remote, GetCID());
		}


		return hr;
	}


	// Send message to connected entity
	HRESULT ConnectionTCP::Send( Message::MessageData* &pMsg )
	{
		HRESULT hr = S_OK;
		IOBUFFER_WRITE *pSendBuffer = nullptr;
		Message::MessageID msgID;

		if (GetConnectionState() == STATE_DISCONNECTED)
			return E_NET_NOT_CONNECTED;

		Message::MessageHeader* pMsgHeader = pMsg->GetMessageHeader();
		msgID = pMsgHeader->msgID;

		if ((pMsgHeader->msgID.IDs.Type != Message::MSGTYPE_NETCONTROL && GetConnectionState() == STATE_DISCONNECTING)
			|| GetConnectionState() == STATE_DISCONNECTED)
		{
			// Send fail by connection closed
			Util::SafeRelease(pMsg);
			goto Proc_End;
		}

		if( pMsg->GetMessageSize() > (UINT)Const::INTER_PACKET_SIZE_MAX )
		{
			Util::SafeRelease( pMsg );
			netErr( E_NET_BADPACKET_TOOBIG );
		}

		PrintDebugMessage( "SendMsg ", pMsg );

		if( !pMsg->GetMessageHeader()->msgID.IDs.Reliability
			&& (m_lGuarantedSent - m_lGuarantedAck) > Const::TCP_GUARANT_PENDING_MAX )
		{
			// Drop if there is too many reliable packets are pending
			netErr( E_NET_SEND_FAIL );
		}

		m_lGuarantedSent.fetch_add(1, std::memory_order_relaxed);

		pMsg->UpdateChecksumNEncrypt();

		m_PendingSend.fetch_add(1, std::memory_order_acquire);

		netChk(Net::NetSystem::AllocBuffer(pSendBuffer));
		pSendBuffer->SetupSendTCP(pMsg);

		if (NetSystem::IsProactorSystem())
		{
			netChk(SendBuffer(pSendBuffer));
		}
		else
		{
			netChk(EnqueueBuffer(pSendBuffer));
		}

		pMsg = nullptr;

	Proc_End:

		if(FAILED(hr))
		{
			Util::SafeRelease(pMsg);
		}
		else
		{
			if (msgID.IDs.Type == Message::MSGTYPE_NETCONTROL)
			{
				netTrace(TRC_TCPNETCTRL, "TCP Ctrl CID:{2}, ip:{0}, msg:{1}", GetConnectionInfo().Remote, msgID, GetCID());
			}
			else
			{
				netTrace(TRC_TCPSENDRAW, "TCP Send CID:{2}, ip:{0}, msg:{1}", GetConnectionInfo().Remote, msgID, GetCID());
			}
		}

		return hr;
	}

	HRESULT ConnectionTCP::SendNetCtrl(UINT uiCtrlCode, UINT uiSequence, Message::MessageID msgID, UINT64 UID)
	{
		HRESULT hr = Connection::SendNetCtrl(uiCtrlCode, uiSequence, msgID, UID);
		if (SUCCEEDED(hr))
		{
			m_PendingSend.fetch_add(1, std::memory_order_acquire);
		}

		return hr;
	}



	////////////////////////////////////////////////////////////////////////////////
	//
	//	TCP Network client connection class
	//

	// Constructor
	ConnectionTCPClient::ConnectionTCPClient()
		:ConnectionTCP()
	{
		GetRecvBuffer()->SetupRecvTCP( GetCID() );
	}

	ConnectionTCPClient::~ConnectionTCPClient()
	{
	}


	// Update net control, process connection heartbit, ... etc
	HRESULT ConnectionTCPClient::UpdateNetCtrl()
	{
		HRESULT hr = S_OK;
		Message::MessageID msgIDTem;

		TimeStampMS ulTimeCur = Util::Time.GetTimeMs();

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
			if ((ulTimeCur - m_ulNetCtrlTime) > DurationMS((INT)GetConnectingTimeOut())) // connection time out
			{
				netTrace( TRC_CONNECTION, "Connecting Timeout CID:{0}", GetCID() );
				netChk( CloseConnection() );
			}
			else if( (ulTimeCur-m_ulNetCtrlTryTime) > DurationMS(Const::CONNECTION_RETRY_TIME) ) // retry
			{
				m_ulNetCtrlTryTime = ulTimeCur;
				netChk(SendNetCtrl(PACKET_NETCTRL_CONNECT, (UINT)GetConnectionInfo().LocalClass, Message::MessageID(BR_PROTOCOL_VERSION), GetConnectionInfo().LocalID));
			}

			goto Proc_End;
			break;
		case IConnection::STATE_DISCONNECTING:
			if( (ulTimeCur-m_ulNetCtrlTime) > DurationMS(Const::SVR_DISCONNECT_TIMEOUT) ) // connection time out
			{
				netTrace( TRC_CONNECTION, "Disconnecting Timeout CID:{0}", GetCID() );
				netChk( CloseConnection() );
			}

			m_ulNetCtrlTryTime = ulTimeCur;
			goto Proc_End;
			break;
		case IConnection::STATE_CONNECTED:
			if( (ulTimeCur-m_ulNetCtrlTime) > DurationMS(Const::HEARTBIT_TIMEOUT) ) // connection time out
			{
				netTrace( TRC_CONNECTION, "Connection Timeout CID:{1}", GetCID() );

				netChk( CloseConnection() );
				m_ulNetCtrlTime = ulTimeCur;
				goto Proc_End;
			}
			else if( (ulTimeCur-m_ulNetCtrlTryTime) > DurationMS(GetHeartbitTry()) ) // heartbit time
			{
				m_ulNetCtrlTryTime = ulTimeCur;
				netChk( SendNetCtrl( PACKET_NETCTRL_HEARTBIT, 0, msgIDTem ) );
			}
			break;
		default:
			break;
		};


	Proc_End:


		return hr;
	}
	
		
	// Initialize connection
	HRESULT ConnectionTCPClient::InitConnection( SOCKET socket, const ConnectionInformation &connectInfo )
	{
		HRESULT hr = ConnectionTCP::InitConnection( socket, connectInfo );
		SetLocalClass( NetClass::Client );

		return hr;
	}


	// Wait connection event
	HRESULT ConnectionTCPClient::WaitConnect()
	{
		HRESULT hr = S_OK;

		if(SUCCEEDED(ConnectionTCP::WaitConnect()))
		{
			ThisThread::SleepFor(DurationMS(50));

			// Wait till conected state come
			while (GetConnectionState() == IConnection::STATE_CONNECTING)
			{
				ThisThread::SleepFor(DurationMS(50));
				UpdateNetCtrl();
				if (GetConnectionState() == IConnection::STATE_DISCONNECTED)
				{
					netErr(GetLastWSAHRESULT() );
				}
			};
		}


	Proc_End:

		return hr;
	}


	////////////////////////////////////////////////////////////////////////////////
	//
	//	TCP Network client connection class
	//

	// Constructor
	ConnectionTCPServer::ConnectionTCPServer()
		:ConnectionTCP()
	{
		SetHeartbitTry(Const::SVR_HEARTBIT_TIME_PEER);
	}

	ConnectionTCPServer::~ConnectionTCPServer()
	{
	}


	// Update net control, process connection heartbit, ... etc
	HRESULT ConnectionTCPServer::UpdateNetCtrl()
	{
		HRESULT hr = S_OK;
		Message::MessageID msgIDTem;

		TimeStampMS ulTimeCur = Util::Time.GetTimeMs();

		if( NetSystem::IsProactorSystem()
			&& GetPendingRecvCount() == 0 
			&& GetConnectionState() != IConnection::STATE_DISCONNECTED)
		{
			PendingRecv();
		}

		// connect/disconnect process
		msgIDTem.ID = PACKET_NETCTRL_NONE;
		switch (GetConnectionState())
		{
		case IConnection::STATE_CONNECTING:
			if( (ulTimeCur-m_ulNetCtrlTime) > DurationMS(GetConnectingTimeOut()) ) // connection time out
			{
				netTrace( TRC_CONNECTION, "Connecting Timeout CID:{0}", GetCID() );
				netChk( CloseConnection() );
			}

			goto Proc_End;
			break;
		case IConnection::STATE_DISCONNECTING:
			if( (ulTimeCur-m_ulNetCtrlTime) > DurationMS(Const::DISCONNECT_TIMEOUT) ) // connection time out
			{
				netTrace( TRC_CONNECTION, "Disconnecting Timeout CID:{0}", GetCID() );
				netChk( CloseConnection() );
			}

			m_ulNetCtrlTryTime = ulTimeCur;
			goto Proc_End;
			break;
		case IConnection::STATE_CONNECTED:
			if( (ulTimeCur-m_ulNetCtrlTime) > DurationMS(Const::HEARTBIT_TIMEOUT) ) // connection time out
			{
				netTrace( TRC_CONNECTION, "Connection Timeout CID:{0}", GetCID() );

				netChk( CloseConnection() );
				m_ulNetCtrlTime = ulTimeCur;
				goto Proc_End;
			}
			break;
		default:
			break;
		};


	Proc_End:


		return hr;
	}



} // namespace Net
} // namespace BR


