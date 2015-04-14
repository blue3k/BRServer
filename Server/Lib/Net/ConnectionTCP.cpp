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
#include "Net/NetMessage.h"
#include "Net/NetConst.h"

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
		:m_uiRecvTemUsed(0),
		m_lGuarantedSent(0),
		m_lGuarantedAck(0)
		, m_uiSendNetCtrlCount(0)
	{
		SetSocket( INVALID_SOCKET );
		m_bufRecvTem.resize( Const::PACKET_SIZE_MAX*2 );

		SetHeartbitTry(Const::TCP_HEARTBIT_START_TIME);
	}

	ConnectionTCP::~ConnectionTCP()
	{
		//Assert( SUCCEEDED( CloseConnection() ) );
		if( GetSocket() != INVALID_SOCKET )
			closesocket( GetSocket() );
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
				netTrace( Trace::TRC_WARN, "HackWarn : Invalid Connect packet CID:%0%, Addr %1%", GetCID(), GetConnectionInfo().Remote );
				netChk( CloseConnection() );
				netErr( E_UNEXPECTED );
			}

			if (GetConnectionState() == STATE_CONNECTING || GetConnectionState() == STATE_CONNECTED)// ServerTCP connection will occure this case
			{
				const MsgNetCtrlConnect *pNetCtrlCon = (const MsgNetCtrlConnect*)pNetCtrl;
				UINT ProtocolVersion = pNetCtrl->rtnMsgID.ID;
				NetClass RemoteClass = (NetClass)pNetCtrl->msgID.IDSeq.Sequence;
				m_ConnectInfo.Remote = pNetCtrlCon->Address;

				if (pNetCtrl->rtnMsgID.ID != BR::PROTOCOL_VERSION)
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
			netTrace( Trace::TRC_WARN, "HackWarn : Invalid packet CID:%0%, Addr %1%", GetCID(), GetConnectionInfo().Remote );
			netChk( CloseConnection() );
			netErr( E_UNEXPECTED );
			break;
		};


	Proc_End:

		if( pIMsg ) pIMsg->Release();

		return hr;
	}

	HRESULT ConnectionTCP::OnIOAccept( HRESULT hrRes, OVERLAPPED_BUFFER_ACCEPT *pOverAccept )
	{
		return E_NOTIMPL;
	}

	// called when reciving message
	HRESULT ConnectionTCP::OnIORecvCompleted( HRESULT hrRes, OVERLAPPED_BUFFER_READ *pIOBuffer, DWORD dwTransferred )
	{
		HRESULT hr = S_OK;

		WSABUF *pBuf = &pIOBuffer->wsaBuff;

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
				netTrace(TRC_TCPRECVRAW, "Recv Msg Failed, SvrTCP, CID %0%, hr=%1%", GetCID(), ArgHex32(hrRes));
				PendingRecv();
				break;
			};
		}
		else
		{
			netChk( OnRecv( dwTransferred, (BYTE*)pBuf[0].buf ) );
			PendingRecv();
		}

	Proc_End:

		DecPendingRecvCount();

		netTrace(TRC_TCPRECVRAW, "TCP Recv CID:%0%, pending:%1%, hr:%2%", GetCID(), GetPendingRecvCount(), ArgHex32(hr));

		return hr;
	}

	// called when Send completed
	HRESULT ConnectionTCP::OnIOSendCompleted( HRESULT hrRes, OVERLAPPED_BUFFER_WRITE *pIOBuffer, DWORD dwTransferred )
	{
		Util::SafeDeleteArray( pIOBuffer->pSendBuff );
		Util::SafeRelease( pIOBuffer->pMsgs );
		WSASystem::FreeBuffer( pIOBuffer );
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
		OVERLAPPED_BUFFER_READ *pOver = NULL;
		INT iErr = 0, iErr2 = 0;
		DWORD dwNumberOfByte = 0;

		if (GetConnectionState() == IConnection::STATE_DISCONNECTED)
			return E_NET_CONNECTION_CLOSED;


		pOver = GetRecvBuffer();

		//do{
			pOver->SetupRecvTCP( GetCID() );

			iErr = WSARecv(GetSocket(), &pOver->wsaBuff, 1, &dwNumberOfByte, &pOver->dwFlags, pOver, NULL);
			if( iErr == SOCKET_ERROR )
			{
				iErr2 = WSAGetLastError();
				switch( iErr2 )
				{
				case WSAECONNABORTED:
				case WSAECONNRESET:
				case WSAENETRESET:
				case WSAENOTCONN:
				case WSAENOTSOCK:
				case WSAESHUTDOWN:
					netTrace(Trace::TRC_WARN, "TCP Read failed, Connection Reset CID:%0%, err:%1%, pending:%2%", GetCID(), iErr2, GetPendingRecvCount());
					// Send fail by connection close
					// Need to disconnect
					CloseConnection();
					netErrSilent( E_NET_CONNECTION_CLOSED );
					break;
				default:
					netTrace( Trace::TRC_ERROR, "TCP Read IOCP Pending failed with CID %0%, err:%1%", GetCID(), iErr2 );
					netErr( E_NET_IO_RECV_FAIL );
					break;
				case WSA_IO_PENDING:
					// Recv is pended
					break;
				};
			}
			else
			{
				// IOCP will handle this
				//Assert(dwNumberOfByte > 0);
				//OnRecv(dwNumberOfByte, (BYTE*)pOver->wsaBuff.buf);
			}

			//if (dwNumberOfByte > 0)
			//{
			//	netTrace(Trace::TRC_ERROR, "PendingRecv CID:%0%, recvSize:%1%, iErr2:%2%", GetCID(), dwNumberOfByte, iErr2);
			//}

		//} while (iErr != SOCKET_ERROR && dwNumberOfByte > 0);

		IncPendingRecvCount();


	Proc_End:

		netTrace(TRC_TCPRECVRAW, "Pending Recv CID:%0%, pending:%1%, hr:%2%", GetCID(), GetPendingRecvCount(), ArgHex32(hr));

		return hr;
	}



	// Called on connection result
	void ConnectionTCP::OnConnectionResult( HRESULT hrConnect )
	{
		__super::OnConnectionResult( hrConnect );

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

		Assert(connectInfo.LocalClass != NetClass::Unknown);

		// ignore incomming socket value
		return __super::InitConnection( socket, connectInfo );
	}

	HRESULT ConnectionTCP::Connect()
	{
		HRESULT hr = S_OK;

		ResetZeroRecvCount();

		AssertRel(GetConnectionState() == STATE_CONNECTING || GetConnectionState() == STATE_DISCONNECTED);

		if (!CreateIoCompletionPort((HANDLE)GetSocket(), IOCPSystem::GetSystem().GetIOCP(), (ULONG_PTR)(IOCPSystem::IOCallBack*)this, 0))
		{
			netTrace(Trace::TRC_ERROR, "CreateIoCompletionPort Failed Peer TCP, hr = %0%", ArgHex32(GetLastHRESULT()));
			netErr(E_UNEXPECTED);
		}

		auto connResult = connect(GetSocket(), (SOCKADDR*)&GetRemoteSockAddr(), sizeof(GetRemoteSockAddr()));
		if (connResult == SOCKET_ERROR)
		{
			auto lastError = WSAGetLastError();
			if (lastError != WSAEWOULDBLOCK)
			{
				//netTrace(Trace::TRC_WARN, "Connection try is failed, RemoteID:%0%", GetConnectionInfo().RemoteID);
				hr = E_FAIL;
			}
		}

	Proc_End:

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

		HANDLE hEvtConnection = WSACreateEvent();
		if (hEvtConnection == WSA_INVALID_EVENT)
		{
			netTrace(Trace::TRC_ERROR, "Failed to create connection event handler err=%0%", WSAGetLastError());
			netErr(HRESULT_FROM_WIN32(WSAGetLastError()));
		}


		if (WSAEventSelect(GetSocket(), hEvtConnection, FD_CONNECT) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Failed to set connection event handler err=%0%", WSAGetLastError());
			netErr(HRESULT_FROM_WIN32(WSAGetLastError()));
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
				netTrace(Trace::TRC_ERROR, "Failed to set connection event handler err=%0%", WSAGetLastError());
				netErr(HRESULT_FROM_WIN32(WSAGetLastError()));
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
				netTrace(Trace::TRC_ERROR, "Failed to cancel connection event handler err=%0%", WSAGetLastError());
			}
		}


	Proc_End:


		if (hEvtConnection != WSA_INVALID_EVENT)
		{
			WSACloseEvent(hEvtConnection);
		}

		return hr;
	}

	// Close connection
	HRESULT ConnectionTCP::CloseConnection()
	{
		HRESULT hr = S_OK;

		//socket
		m_uiRecvTemUsed = 0;
		ResetZeroRecvCount();

		netChk( __super::CloseConnection() );

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

		netTrace(TRC_TCPRECVRAW, "TCP RecvBuf Len=%0%", uiBuffSize);

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
			netTrace( TRC_TCPNETCTRL, "TCP Ctrl Recv ip:%0%, msg:%1%, Len:%2%", 
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

			hr  = __super::OnRecv( pMsg );
			pMsg = NULL;
			netChk( hr );
		}

	Proc_End:

		Util::SafeRelease( pMsg );

		return hr;
	}

	// Send message to connected entity
	HRESULT ConnectionTCP::Send( Message::MessageData* &pMsg )
	{
		HRESULT hr = S_OK;

		if (GetConnectionState() == STATE_DISCONNECTED)
		{
			netErr(E_NET_NOT_CONNECTED);
		}

		if( pMsg->GetMessageSize() > (UINT)Const::INTER_PACKET_SIZE_MAX )
		{
			Util::SafeRelease( pMsg );
			netErr( E_NET_BADPACKET_TOOBIG );
		}

		PrintDebugMessage( "SendMsg ", pMsg );

		if( pMsg->GetMessageHeader()->msgID.IDs.Reliability )
		{
			m_lGuarantedSent.fetch_add(1, std::memory_order_relaxed);
		}
		else
		{
			// Drop if there is too many reliable packets are pending
			if( (m_lGuarantedSent - m_lGuarantedAck) > Const::TCP_GUARANT_PENDING_MAX )
				netErr( E_NET_SEND_FAIL );
		}

		pMsg->UpdateChecksumNEncrypt();

		m_PendingSend.fetch_add(1, std::memory_order_acquire);

		netChk( GetNet()->SendMsg( this, pMsg ) );

	Proc_End:

		return hr;
	}

	HRESULT ConnectionTCP::SendNetCtrl(UINT uiCtrlCode, UINT uiSequence, Message::MessageID msgID, UINT64 UID)
	{
		HRESULT hr = __super::SendNetCtrl(uiCtrlCode, uiSequence, msgID, UID);
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
			if ((INT)(ulTimeCur - m_ulNetCtrlTime) > (INT)GetConnectingTimeOut()) // connection time out
			{
				netTrace( TRC_CONNECTION, "Connecting Timeout CID:%0%", GetCID() );
				netChk( CloseConnection() );
			}
			else if( (INT)(ulTimeCur-m_ulNetCtrlTryTime) > Const::CONNECTION_RETRY_TIME ) // retry
			{
				m_ulNetCtrlTryTime = ulTimeCur;
				netChk(SendNetCtrl(PACKET_NETCTRL_CONNECT, (UINT)GetConnectionInfo().LocalClass, Message::MessageID(BR::PROTOCOL_VERSION), GetConnectionInfo().LocalID));
			}

			goto Proc_End;
			break;
		case IConnection::STATE_DISCONNECTING:
			if( (INT)(ulTimeCur-m_ulNetCtrlTime) > Const::SVR_DISCONNECT_TIMEOUT ) // connection time out
			{
				netTrace( TRC_CONNECTION, "Disconnecting Timeout CID:%0%", GetCID() );
				netChk( CloseConnection() );
			}

			m_ulNetCtrlTryTime = ulTimeCur;
			goto Proc_End;
			break;
		case IConnection::STATE_CONNECTED:
			if( (INT)(ulTimeCur-m_ulNetCtrlTime) > Const::HEARTBIT_TIMEOUT ) // connection time out
			{
				netTrace( TRC_CONNECTION, "Connection Timeout CID:%1%", GetCID() );

				netChk( CloseConnection() );
				m_ulNetCtrlTime = ulTimeCur;
				goto Proc_End;
			}
			else if( (INT)(ulTimeCur-m_ulNetCtrlTryTime) > GetHeartbitTry() ) // heartbit time
			{
				m_ulNetCtrlTryTime = ulTimeCur;
				netChk( SendNetCtrl( PACKET_NETCTRL_HEARTBIT, 0, msgIDTem ) );
			}
			break;
		};


	Proc_End:


		return hr;
	}
	
		
	// Initialize connection
	HRESULT ConnectionTCPClient::InitConnection( SOCKET socket, const ConnectionInformation &connectInfo )
	{
		HRESULT hr = __super::InitConnection( socket, connectInfo );
		SetLocalClass( NetClass::Client );

		return hr;
	}


	// Wait connection event
	HRESULT ConnectionTCPClient::WaitConnect()
	{
		HRESULT hr = S_OK;

		if(SUCCEEDED(__super::WaitConnect()))
		{
			Sleep(50);

			// Wait till conected state come
			while (GetConnectionState() == IConnection::STATE_CONNECTING)
			{
				Sleep(50);
				UpdateNetCtrl();
				if (GetConnectionState() == IConnection::STATE_DISCONNECTED)
				{
					netErr( HRESULT_FROM_WIN32(WSAGetLastError()) );
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
				netTrace( TRC_CONNECTION, "Connecting Timeout CID:%0%", GetCID() );
				netChk( CloseConnection() );
			}

			goto Proc_End;
			break;
		case IConnection::STATE_DISCONNECTING:
			if( (INT)(ulTimeCur-m_ulNetCtrlTime) > Const::DISCONNECT_TIMEOUT ) // connection time out
			{
				netTrace( TRC_CONNECTION, "Disconnecting Timeout CID:%0%", GetCID() );
				netChk( CloseConnection() );
			}

			m_ulNetCtrlTryTime = ulTimeCur;
			goto Proc_End;
			break;
		case IConnection::STATE_CONNECTED:
			if( (INT)(ulTimeCur-m_ulNetCtrlTime) > Const::HEARTBIT_TIMEOUT ) // connection time out
			{
				netTrace( TRC_CONNECTION, "Connection Timeout CID:%0%", GetCID() );

				netChk( CloseConnection() );
				m_ulNetCtrlTime = ulTimeCur;
				goto Proc_End;
			}
			break;
		};


	Proc_End:


		return hr;
	}



} // namespace Net
} // namespace BR


