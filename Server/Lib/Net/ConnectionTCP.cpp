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
#include "Common/ResultCode/BRResultCodeNet.h"
#include "Common/ResultCode/BRResultCodeSvr.h"
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
		: INetIOCallBack(GetSocket())
		, m_lGuarantedSent(0)
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
		//Assert( ( CloseConnection() ) );
		if (GetSocket() != INVALID_SOCKET)
		{
			AssertRel(!GetIsIORegistered());
			AssertRel(GetAssignedIOWorker() == -1);

			NetSystem::UnregisterSocket(this);

			NetSystem::CloseSocket(GetSocket());
		}

		if (GetWriteQueue()) delete GetWriteQueue();
	}



	// Process network control message
	Result ConnectionTCP::ProcNetCtrl( const MsgNetCtrl* pNetCtrl )
	{
		Result hr = ResultCode::SUCCESS;
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
						OnConnectionResult(ResultCode::SUCCESS);
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
					OnConnectionResult( ResultCode::E_NET_PROTOCOL_VERSION_MISMATCH );
					netChk( Disconnect("Protocol mismatch") );
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
				netErr( ResultCode::UNEXPECTED );
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
						OnConnectionResult( ResultCode::E_NET_PROTOCOL_VERSION_MISMATCH );
					netChk( Disconnect("Protocol mismatch") );
				}
				else if( GetConnectionInfo().RemoteClass != NetClass::Unknown && RemoteClass != GetConnectionInfo().RemoteClass )
				{
					netChk(SendNetCtrl(PACKET_NETCTRL_NACK, pNetCtrl->msgID.IDSeq.Sequence, pNetCtrl->msgID));
					if (GetConnectionState() != STATE_CONNECTED)
						OnConnectionResult(ResultCode::E_NET_INVALID_NETCLASS);
					netChk( Disconnect("Invalid netclass") );
				}
				else
				{
					Assert(GetConnectionInfo().LocalClass != NetClass::Unknown);
					netChk(SendNetCtrl(PACKET_NETCTRL_ACK, (UINT)GetConnectionInfo().LocalClass, pNetCtrl->msgID, GetConnectionInfo().LocalID));
					if (GetConnectionState() != STATE_CONNECTED)
					{
						m_ConnectInfo.SetRemoteInfo(RemoteClass, pNetCtrlCon->PeerUID);
						OnConnectionResult(ResultCode::SUCCESS);
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
			netErr( ResultCode::UNEXPECTED );
			break;
		};


	Proc_End:

		if( pIMsg ) pIMsg->Release();

		return hr;
	}

	Result ConnectionTCP::Recv(IOBUFFER_READ* pIOBuffer)
	{
		Result hr = ResultCode::SUCCESS, hrErr = ResultCode::SUCCESS;

		if (GetConnectionState() == IConnection::STATE_DISCONNECTED)
			return ResultCode::E_NET_CONNECTION_CLOSED;

		netChkPtr(pIOBuffer);
		pIOBuffer->SetupRecvTCP(GetCID());

		hrErr = NetSystem::Recv(GetSocket(), pIOBuffer);
		switch ((int32_t)hrErr)
		{
		case ResultCode::E_NET_CONNABORTED:
		case ResultCode::E_NET_CONNRESET:
		case ResultCode::E_NET_NETRESET:
		case ResultCode::E_NET_NOTSOCK:
		case ResultCode::E_NET_SHUTDOWN:
			netTrace(Trace::TRC_WARN, "TCP Read failed, Connection Reset CID:{0}, err:{1:X8}, pending:{2}", GetCID(), hrErr, GetPendingRecvCount());
			// Send fail by connection close
			// Need to disconnect
			CloseConnection();
			netErrSilent(ResultCode::E_NET_CONNECTION_CLOSED);
			break;
		default:
			//netTrace(Trace::TRC_ERROR, "TCP Recv failed with CID {0}, err:{1:X8}", GetCID(), hrErr);
			netErrSilent(hrErr);
			break;
		case ResultCode::E_NET_IO_PENDING:
		case ResultCode::E_NET_WOULDBLOCK:
			// Recv is pended
			hr = hrErr;
			break;
		case ResultCode::E_NET_TRY_AGAIN:
		case ResultCode::E_NET_NOTCONN:
			// try again
			hr = hrErr;
			break;
		case ResultCode::SUCCESS_FALSE:
			hr = ResultCode::E_NET_TRY_AGAIN;
			break;
		case ResultCode::SUCCESS:
			break;
		};


	Proc_End:

		return hr;
	}

	// called when reciving message
	Result ConnectionTCP::OnIORecvCompleted( Result hrRes, IOBUFFER_READ* &pIOBuffer )
	{
		Result hr = ResultCode::SUCCESS;

		if(pIOBuffer != nullptr && pIOBuffer->CID != GetCID() )
			netErr( ResultCode::INVALID_ARG );

		Assert(!NetSystem::IsProactorSystem() || pIOBuffer->bIsPending.load(std::memory_order_relaxed));

		if( !( hrRes ) )
		{
			switch((int32_t)hrRes )
			{
			case ResultCode::E_NET_CONNECTION_CLOSED:
			case ResultCode::E_NET_IO_ABORTED:
				netChk( CloseConnection() );
				break;
			default:
				netTrace(TRC_TCPRECVRAW, "Recv Msg Failed, SvrTCP, CID {0}, hr={1:X8}", GetCID(), hrRes);
				break;
			};
		}
		else
		{
			netChkPtr(pIOBuffer);
			netChk( OnRecv(pIOBuffer->TransferredSize, (BYTE*)pIOBuffer->buffer) );
		}

	Proc_End:


		if (!NetSystem::IsProactorSystem())
		{
			Util::SafeDelete(pIOBuffer);
		}
		else
		{
			pIOBuffer->SetPendingFalse();
		}

		//DecPendingRecvCount();
		// Update will process new pending recv
		PendingRecv();
		//AssertRel(!NetSystem::IsProactorSystem() || GetPendingRecvCount() == 1);

		netTrace(TRC_TCPRECVRAW, "TCP Recv CID:{0}, pending:{1}, hr:{2:X8}", GetCID(), GetPendingRecvCount(), hr);

		return hr;
	}

	Result ConnectionTCP::OnSendReady()
	{
		Result hr;
		if (GetEventHandler())
		{
			auto hrRes = GetEventHandler()->OnNetSendReadyMessage(this);
			if (hrRes != Result(ResultCode::E_SVR_INVALID_TASK_GROUPID))
			{
				netChk(hrRes);
			}
		}
		// process directly
		else
		{
			netChk(ProcessSendQueue());
		}

	Proc_End:

		return hr;
	}

	// called when Send completed
	Result ConnectionTCP::OnIOSendCompleted( Result hrRes, IOBUFFER_WRITE *pIOBuffer )
	{
		Util::SafeDeleteArray( pIOBuffer->pSendBuff );
		Util::SafeRelease( pIOBuffer->pMsgs );
		NetSystem::FreeBuffer( pIOBuffer );
		m_PendingSend.fetch_sub(1, std::memory_order_release);
		return ResultCode::SUCCESS;
	}

	// Clear Queue
	Result ConnectionTCP::ClearQueues()
	{
		return ResultCode::SUCCESS;
	}

	// Pending recv New one
	Result ConnectionTCP::PendingRecv()
	{
		Result hr = ResultCode::SUCCESS;
		IOBUFFER_READ *pOver = nullptr;

		if (!NetSystem::IsProactorSystem())
			return ResultCode::SUCCESS;

		if (GetConnectionState() == ConnectionState::STATE_DISCONNECTED)
			return ResultCode::SUCCESS;

		// On client side, we need to check writable status by calling connect again
		if (m_isClientSide && !m_isActuallyConnected && GetConnectionState() == ConnectionState::STATE_CONNECTING)
		{
			m_isActuallyConnected = Connect();
			if (!m_isActuallyConnected)
				return ResultCode::SUCCESS;
		}

		// For TCP, we need only single buffer is in waiting read operation
		pOver = GetRecvBuffer();
		hr = pOver->SetPendingTrue();
		if (!(hr))
			return ResultCode::SUCCESS;

		hr = Recv(pOver);
		if (!(hr) && hr != Result(ResultCode::E_NET_IO_PENDING))
		{
			netTrace(Trace::TRC_WARN, "Pending Recv failed, CID:{0}, pending:{1}, hr:{2:X8}", GetCID(), GetPendingRecvCount(), hr);
			//Assert(false);
			// Failed, release pending flag
			pOver->SetPendingFalse();
		}
		else
		{
			netTrace(TRC_TCPRECVRAW, "Pending Recv CID:{0}, pending:{1}, hr:{2:X8}", GetCID(), GetPendingRecvCount(), hr);
		}

	//Proc_End:


		return hr;
	}



	// Called on connection result
	void ConnectionTCP::OnConnectionResult( Result hrConnect )
	{
		Connection::OnConnectionResult( hrConnect );

		if( ( hrConnect ) )
		{
		}
		else
		{
			CloseConnection();
		}

	}


	// Initialize connection
	Result ConnectionTCP::InitConnection( SOCKET socket, const ConnectionInformation &connectInfo )
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

		int flag = 1;
		int result = setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));
		if (result < 0)
		{
			return GetLastWSAResult();
		}

		// ignore incomming socket value
		return Connection::InitConnection( socket, connectInfo );
	}

	Result ConnectionTCP::Connect()
	{
		Result hr = ResultCode::SUCCESS;
		Result hrConResult;
		int connResult;

		ResetZeroRecvCount();

		if (GetConnectionState() != STATE_CONNECTING && GetConnectionState() != STATE_DISCONNECTED)
		{
			netTrace(Trace::TRC_ERROR, "Invalid connection state to try connect {0}", GetConnectionState());
			//AssertRel(GetConnectionState() == STATE_CONNECTING || GetConnectionState() == STATE_DISCONNECTED);
			netErrSilent(ResultCode::E_NET_INVALID_CONNECTION_STATE);
		}

		connResult = connect(GetSocket(), (sockaddr*)&GetRemoteSockAddr(), GetRemoteSockAddrSize());
		if (connResult == SOCKET_ERROR)
		{
			hrConResult = GetLastWSAResult();
			switch ((int32_t)hrConResult)
			{
			case ResultCode::E_NET_INPROGRESS:
			case ResultCode::E_NET_WOULDBLOCK:  // First call need to wait
			case ResultCode::E_NET_ALREADY:		// called again, still need to wait
				hr = ResultCode::SUCCESS_FALSE;
				break;
			case ResultCode::E_NET_ISCONN:		// Connection estabalished
				hr = ResultCode::SUCCESS;
				break;
			default:
				netTrace(Trace::TRC_WARN, "Connection try is failed, RemoteAddr:{0}, RemoteID:{1}, hr:{2:X8}", GetConnectionInfo().Remote, GetConnectionInfo().RemoteID, hrConResult);
				hr = hrConResult;
			}
		}

		m_isClientSide = true;
		m_isActuallyConnected = false; // only client side need to check this condition

	Proc_End:

		netTrace(Net::TRC_CONNECTION, "Connect sock:{0}, to:{1}, hrCon:{2}, hr:{3}", GetSocket(), GetConnectionInfo().Remote, hrConResult, hr);

		if (!(hr))
		{
			SetConnectionState(STATE_DISCONNECTED);
			CloseSocket();
		}

		return hr;

	}

	// Wait connection event
	Result ConnectionTCP::WaitConnect()
	{
		Result hr = ResultCode::SUCCESS;

		// disable wait for non-windows platform
#if WINDOWS
		HANDLE hEvtConnection = WSACreateEvent();
		if (hEvtConnection == WSA_INVALID_EVENT)
		{
			netTrace(Trace::TRC_ERROR, "Failed to create connection event handler err={0:X8}", GetLastWSAResult());
			netErr(GetLastWSAResult());
		}


		if (WSAEventSelect(GetSocket(), hEvtConnection, FD_CONNECT) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Failed to set connection event handler err={0:X8}", GetLastWSAResult());
			netErr(GetLastWSAResult());
		}

		DWORD dwWaitRes = WSAWaitForMultipleEvents(1, &hEvtConnection, TRUE, Const::CONNECTION_TIMEOUT, TRUE);

		if (dwWaitRes == WSA_WAIT_TIMEOUT || dwWaitRes == WSA_WAIT_FAILED)
		{
			OnConnectionResult(ResultCode::E_NET_CONNECTION_TIMEOUT);
			hr = ResultCode::E_NET_CONNECTION_TIMEOUT;
		}
		else
		{

			if (WSAEventSelect(GetSocket(), hEvtConnection, FD_WRITE | FD_READ) == SOCKET_ERROR)
			{
				netTrace(Trace::TRC_ERROR, "Failed to set connection event handler err={0:X8}", GetLastWSAResult());
				netErr(GetLastWSAResult());
			}

			DWORD dwWaitRes2 = WSAWaitForMultipleEvents(1, &hEvtConnection, TRUE, Const::CONNECTION_TIMEOUT, TRUE);

			if (dwWaitRes2 == WSA_WAIT_TIMEOUT || dwWaitRes2 == WSA_WAIT_FAILED)
			{
				OnConnectionResult(ResultCode::E_NET_CONNECTION_TIMEOUT);
				netErr(ResultCode::E_NET_CONNECTION_TIMEOUT);
			}

			if (WSAEventSelect(GetSocket(), hEvtConnection, 0) == SOCKET_ERROR)
			{
				// just reporting this error
				netTrace(Trace::TRC_ERROR, "Failed to cancel connection event handler err={0:X8}", GetLastWSAResult());
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
	Result ConnectionTCP::CloseConnection()
	{
		Result hr = ResultCode::SUCCESS;

		//socket
		m_uiRecvTemUsed = 0;
		ResetZeroRecvCount();

		netChk(Connection::CloseConnection() );

		CloseSocket();

		if (GetConnectionState() == IConnection::STATE_DISCONNECTED)
			goto Proc_End;
		

	Proc_End:

		return hr;
	}

	void ConnectionTCP::CloseSocket()
	{
		if (GetSocket() == INVALID_SOCKET) return;

		NetSystem::UnregisterSocket(this);

		Connection::CloseSocket();
	}



	// called when incomming message occure
	Result ConnectionTCP::OnRecv( UINT uiBuffSize, const BYTE* pBuff )
	{
		Result hr = ResultCode::SUCCESS;
		Message::MessageData *pMsg = NULL;

		netTrace(TRC_TCPRECVRAW, "TCP RecvBuf Len={0}", uiBuffSize);

		if( uiBuffSize == 0 )
		{
			IncZeroRecvCount();
			if( GetZeroRecvCount() > (ULONG)Const::CONNECTION_ZEROPACKET_MAX )
			{
				//Disconnect("Too many zero packets");
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
					netErr( ResultCode::UNEXPECTED );
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
					netErr( ResultCode::E_NET_BADPACKET_SIZE );
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



	Result ConnectionTCP::OnRecv( Message::MessageData *pMsg )
	{
		Result hr = ResultCode::SUCCESS;
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
	Result ConnectionTCP::SendBuffer(IOBUFFER_WRITE *pSendBuffer)
	{
		Result hr = ResultCode::SUCCESS, hrErr = ResultCode::SUCCESS;

		netChkPtr(pSendBuffer);

		hrErr = NetSystem::Send(GetSocket(), pSendBuffer);
		switch ((int32_t)hrErr)
		{
		case ResultCode::SUCCESS:
		case ResultCode::E_NET_IO_PENDING:
		case ResultCode::E_NET_WOULDBLOCK:
			break;
		case ResultCode::E_NET_TRY_AGAIN:
		case ResultCode::E_NET_NOTCONN:
			hr = hrErr;
			break;
		case ResultCode::E_NET_CONNABORTED:
		case ResultCode::E_NET_CONNRESET:
		case ResultCode::E_NET_NETRESET:
		case ResultCode::E_NET_NOTSOCK:
		case ResultCode::E_NET_SHUTDOWN:
			// Send fail by connection close
			// Need to disconnect
			Disconnect("Send failed by error");
			hr = ResultCode::E_NET_CONNECTION_CLOSED;
			goto Proc_End;
			break;
		default:
			netErr(ResultCode::E_NET_IO_SEND_FAIL);
			break;
		};

	Proc_End:

		switch ((int32_t)hr)
		{
		case ResultCode::SUCCESS:
		case ResultCode::SUCCESS_FALSE:
			break;
		case ResultCode::E_NET_IO_SEND_FAIL:
			return ResultCode::SUCCESS;
		case ResultCode::E_NET_TRY_AGAIN:
			break;
		case ResultCode::E_NET_NOTCONN:
			m_isActuallyConnected = Connect();
		default:
			netTrace(Trace::TRC_ERROR, "TCP Send Failed, CID:{3}, sock:{4}, ip:{0}, err:{1:X8}, hr:{2:X8}", GetConnectionInfo().Remote, hrErr, hr, GetCID(), GetSocket());
			break;
		}


		return hr;
	}


	Result ConnectionTCP::SendRaw(Message::MessageData* &pMsg)
	{
		Result hr = ResultCode::SUCCESS;
		IOBUFFER_WRITE *pSendBuffer = nullptr;

		netChkPtr(pMsg);

		netChk(Net::NetSystem::AllocBuffer(pSendBuffer));
		pSendBuffer->SetupSendTCP(pMsg);

		if (NetSystem::IsProactorSystem())
		{
			netChk(SendBuffer(pSendBuffer));
		}
		else
		{
			netChk(EnqueueBuffer(pSendBuffer));
			ProcessSendQueue();
		}
		pMsg = nullptr;
		pSendBuffer = nullptr;

	Proc_End:

		if (pSendBuffer != nullptr)
		{
			NetSystem::FreeBuffer(pSendBuffer);
		}

		return hr;
	}

	// Send message to connected entity
	Result ConnectionTCP::Send( Message::MessageData* &pMsg )
	{
		Result hr = ResultCode::SUCCESS;
		Message::MessageID msgID;

		if (GetConnectionState() == STATE_DISCONNECTED)
			return ResultCode::E_NET_NOT_CONNECTED;

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
			netErr( ResultCode::E_NET_BADPACKET_TOOBIG );
		}

		PrintDebugMessage( "SendMsg ", pMsg );

		if( !pMsg->GetMessageHeader()->msgID.IDs.Reliability
			&& (m_lGuarantedSent - m_lGuarantedAck) > Const::TCP_GUARANT_PENDING_MAX )
		{
			// Drop if there is too many reliable packets are pending
			netErr( ResultCode::E_NET_SEND_FAIL );
		}

		m_lGuarantedSent.fetch_add(1, std::memory_order_relaxed);

		pMsg->UpdateChecksumNEncrypt();

		m_PendingSend.fetch_add(1, std::memory_order_acquire);

		netChk(SendRaw(pMsg));

		pMsg = nullptr;

	Proc_End:

		if(!(hr))
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

	Result ConnectionTCP::SendNetCtrl(UINT uiCtrlCode, UINT uiSequence, Message::MessageID msgID, UINT64 UID)
	{
		Result hr = Connection::SendNetCtrl(uiCtrlCode, uiSequence, msgID, UID);
		if ((hr))
		{
			m_PendingSend.fetch_add(1, std::memory_order_acquire);
		}

		return hr;
	}

	// Update send queue, Reliable UDP
	Result ConnectionTCP::UpdateSendQueue()
	{
		Assert(false);
		return ResultCode::NOT_IMPLEMENTED;
	}

	// Update Send buffer Queue, TCP and UDP client connection
	Result ConnectionTCP::UpdateSendBufferQueue()
	{
		return ProcessSendQueue();
	}



	////////////////////////////////////////////////////////////////////////////////
	//
	//	TCP Network client connection class
	//

	// Constructor
	ConnectionTCPClient::ConnectionTCPClient()
		:ConnectionTCP()
	{
		//GetRecvBuffer()->SetupRecvTCP( GetCID() );
	}

	ConnectionTCPClient::~ConnectionTCPClient()
	{
	}


	// Update net control, process connection heartbit, ... etc
	Result ConnectionTCPClient::UpdateNetCtrl()
	{
		Result hr = ResultCode::SUCCESS;
		Message::MessageID msgIDTem;

		TimeStampMS ulTimeCur = Util::Time.GetTimeMs();

		if( GetConnectionState() != IConnection::STATE_DISCONNECTED)
		{
			if (!GetIsIORegistered())
			{
				netTrace(TRC_CONNECTION, "Close connection because it's kicked from net IO, CID:{0}", GetCID());
				netChk(CloseConnection());
			}
			else
			{
				PendingRecv();
			}
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
	Result ConnectionTCPClient::InitConnection( SOCKET socket, const ConnectionInformation &connectInfo )
	{
		Result hr = ConnectionTCP::InitConnection( socket, connectInfo );
		SetLocalClass( NetClass::Client );

		return hr;
	}


	// Wait connection event
	Result ConnectionTCPClient::WaitConnect()
	{
		Result hr = ResultCode::SUCCESS;

		if((ConnectionTCP::WaitConnect()))
		{
			ThisThread::SleepFor(DurationMS(50));

			// Wait till conected state come
			while (GetConnectionState() == IConnection::STATE_CONNECTING)
			{
				ThisThread::SleepFor(DurationMS(50));
				UpdateNetCtrl();
				if (GetConnectionState() == IConnection::STATE_DISCONNECTED)
				{
					netErr(GetLastWSAResult() );
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
	Result ConnectionTCPServer::UpdateNetCtrl()
	{
		Result hr = ResultCode::SUCCESS;
		Message::MessageID msgIDTem;

		TimeStampMS ulTimeCur = Util::Time.GetTimeMs();

		if( GetConnectionState() != IConnection::STATE_DISCONNECTED)
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


