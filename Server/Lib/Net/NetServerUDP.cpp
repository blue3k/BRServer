////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Network Server implementation 
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Common/HRESNet.h"
#include "Net/NetConst.h"
#include "Net/NetSystem.h"
#include "Common/Thread.h"
#include "Common/StrUtil.h"
#include "Net/Connection.h"
#include "Net/NetTrace.h"
#include "Net/NetDef.h"
#include "Net/NetServer.h"
#include "Protocol/ProtocolVer.h"
#include "Net/NetServerUDP.h"
#include "Net/NetCtrl.h"




namespace BR {
namespace Net {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	UDP Server class
	//

	ServerUDPBase::ServerUDPBase(ServerID InServerID, NetClass localClass)
		:Server(InServerID, localClass),
		m_pRecvBuffers(nullptr)
	{
		// We will share the write queue in net system for UDP
		//SetWriteQueue(new WriteBufferQueue);
	}

	ServerUDPBase::~ServerUDPBase()
	{
		if( m_pRecvBuffers )
			delete[] m_pRecvBuffers;

		if (GetWriteQueue()) delete GetWriteQueue();
	}

	// Close all connection
	HRESULT ServerUDPBase::CloseAllConnection()
	{
		return GetConnectionManager().PendingCloseAllConnection();
	}

	HRESULT ServerUDPBase::Recv(IOBUFFER_READ* pIOBuffer)
	{
		HRESULT hr = S_OK, hrErr = S_OK;

		netChkPtr(pIOBuffer);

		pIOBuffer->SetupRecvUDP(0);

		hrErr = NetSystem::RecvFrom(GetSocket(), pIOBuffer);
		switch (hrErr)
		{
		case S_OK:
		case E_NET_IO_PENDING:
		case E_NET_TRY_AGAIN:
		case E_NET_WOULDBLOCK:
			hr = hrErr;
			goto Proc_End;// success
			break;
		case E_NET_NETUNREACH:
		case E_NET_CONNABORTED:
		case E_NET_CONNRESET:
		case E_NET_NETRESET:
			// some remove has problem with continue connection
			netTrace(TRC_NETCTRL, "UDP Remote has connection error err={0:X8}, {1}", hrErr, pIOBuffer->NetAddr.From);
			//break;
		default:
			// Unknown error
			netTrace(TRC_RECVRAW, "UDP Read Pending failed err={0:X8}", hrErr);
			//netErr( HRESULT_FROM_WIN32(iErr2) );
			break;
		};

	Proc_End:

		return hr;
	}


	HRESULT ServerUDPBase::OnSendReady()
	{
		return ProcessSendQueue();
	}

	HRESULT ServerUDPBase::SendBuffer(IOBUFFER_WRITE *pSendBuffer)
	{
		HRESULT hr = S_OK, hrErr = S_OK;
		auto addrTo = pSendBuffer->NetAddr.To;

		hrErr = NetSystem::SendTo(GetSocket(), pSendBuffer);
		switch (hrErr)
		{
		case E_NET_TRY_AGAIN:
			break;
		case S_OK:
		case E_NET_IO_PENDING:
		case E_NET_WOULDBLOCK:
			break;
		case E_NET_CONNABORTED:
		case E_NET_CONNRESET:
		case E_NET_NETRESET:
		case E_NET_NOTCONN:
		case E_NET_NOTSOCK:
		case E_NET_SHUTDOWN:
			// Send fail by connection close
			// Need to disconnect
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
				netTrace(Trace::TRC_ERROR, "UDP Send Failed, ip:{0}, err:{1:X8}, hr:{2:X8}", addrTo, hrErr, hr);
			}
			else
			{
				netTrace(Net::TRC_SENDRAW, "UDP Send Failed, ip:{0}, err:{1:X8}, hr:{2:X8}", addrTo, hrErr, hr);
				return S_OK;
			}
		}
		else
		{
			//if (msgID.IDs.Type == Message::MSGTYPE_NETCONTROL)
			//{
			//	netTrace(TRC_NETCTRL, "UDP SendCtrl ip:{0}, msg:{1}, Len:{2}", addrTo);
			//}
			//else
			//{
			//	netTrace(TRC_SENDRAW, "UDP Send ip:{0}, msg:{1}, Len:{2}", addrTo);
			//}
		}

		return hr;
	}
	
	// called when send completed
	HRESULT ServerUDPBase::OnIOSendCompleted( HRESULT hrRes, IOBUFFER_WRITE *pIOBuffer )
	{
		NetSystem::FreeGatheringBuffer(pIOBuffer->pSendBuff);
		Util::SafeRelease( pIOBuffer->pMsgs );
		NetSystem::FreeBuffer( pIOBuffer );
		return S_OK;
	}

	
	// Get Active(non-free) connection
	CounterType ServerUDPBase::GetActiveConnectionCount()
	{
		return GetConnectionManager().GetNumActiveConnection();
	}

	// Open host and start listen
	HRESULT ServerUDPBase::HostOpen( NetClass netCls, const char *strLocalIP, USHORT usLocalPort )
	{
		HRESULT hr = S_OK;
		SOCKET socket = INVALID_SOCKET;
		INT iOptValue;
		bool bOptValue = 0;
		sockaddr_in6 bindAddr;
		INet::Event netEvent(INet::Event::EVT_NET_INITIALIZED);

		if( GetSocket() != INVALID_SOCKET )
			return S_OK;


		netChk(Server::HostOpen( netCls, strLocalIP, usLocalPort ) );

		netTrace(Trace::TRC_TRACE, "Open Server UDP Host {0}:{1}", strLocalIP, usLocalPort );

		//socket = WSASocket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP, nullptr, 0, WSA_FLAG_OVERLAPPED);
		socket = NetSystem::Socket(SockFamily::IPV6, SockType::DataGram);
		if( socket == INVALID_SOCKET )
		{
			netTrace(Trace::TRC_ERROR, "Failed to Open Server Socket {0:X8}", GetLastWSAHRESULT());
			netErr( E_UNEXPECTED );
		}

		iOptValue = Const::SVR_RECV_BUFFER_SIZE;
		if( setsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_RCVBUF = {0}, err = {1:X8}", iOptValue, GetLastWSAHRESULT() );
			netErr( E_UNEXPECTED );
		}

		iOptValue = Const::SVR_SEND_BUFFER_SIZE;
		if( setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_SNDBUF = {0}, err = {1:X8}", iOptValue, GetLastWSAHRESULT() );
			netErr( E_UNEXPECTED );
		}

		iOptValue = 0;
#if WINDOWS
		{
			INT iOptLen = sizeof(iOptValue);
			if (getsockopt(socket, SOL_SOCKET, SO_MAX_MSG_SIZE, (char *)&iOptValue, &iOptLen) == SOCKET_ERROR)
			{
				netTrace(Trace::TRC_ERROR, "Failed to get socket option SO_MAX_MSG_SIZE = {0}, err = {1:X8}", iOptValue, GetLastWSAHRESULT());
				netErr(E_UNEXPECTED);
			}
		}
#endif
		if( iOptValue < Const::PACKET_SIZE_MAX )
		{
			netTrace(Trace::TRC_WARN, "Socket max packet size too small, Change to socket maximum SocketMax={0}, SvrMax={1}, err = {2:X8}", iOptValue, (UINT)Const::PACKET_SIZE_MAX, GetLastWSAHRESULT() );
			//Const::PACKET_SIZE_MAX = iOptValue;
		}

		bOptValue = TRUE;
		if( setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&bOptValue, sizeof(bOptValue)) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_REUSEADDR = {0}, err = {1:X8}", bOptValue, GetLastWSAHRESULT() );
			netErr( E_UNEXPECTED );
		}

		iOptValue = FALSE;
		if (setsockopt(socket, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option IPV6_V6ONLY = {0}, err = {1:X8}", iOptValue, GetLastWSAHRESULT());
			netErr(E_UNEXPECTED);
		}


		bindAddr = GetSocketAddr();
		bindAddr.sin6_family = AF_INET6;
		bindAddr.sin6_addr = in6addr_any;
		if (bind(socket, (sockaddr*)&bindAddr, sizeof(bindAddr)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Socket bind failed, UDP err={0:X8}", GetLastWSAHRESULT() );
			netErr( E_UNEXPECTED );
		}

		SetSocket( socket );
		socket = INVALID_SOCKET;

		netChk(NetSystem::RegisterSocket(SockType::DataGram, this));

		// Prepare connection
		netChk( GetConnectionManager().InitManager( Const::SVR_PUBLIC_CONNECTION_POOLCACHE ) );


		// Ready recv
		if (NetSystem::IsProactorSystem())
		{
			if (m_pRecvBuffers) delete[] m_pRecvBuffers;
			netMem(m_pRecvBuffers = new IOBUFFER_READ[Const::SVR_NUM_RECV_THREAD]);

			for (INT uiRecv = 0; uiRecv < Const::SVR_NUM_RECV_THREAD; uiRecv++)
			{
				netChk(PendingRecv(&m_pRecvBuffers[uiRecv]));
			}
		}

		EnqueueNetEvent( netEvent ); 


	Proc_End:

		if( FAILED(hr) )
			HostClose();

		if( socket != INVALID_SOCKET )
			NetSystem::CloseSocket( socket );

		netTrace( TRC_NET, "HostOpen {0}, hr={1:X8}", GetLocalAddress(), hr );

		return hr;
	}

	// Close host and close all connections
	HRESULT ServerUDPBase::HostClose()
	{
		HRESULT hr = S_OK;
		INet::Event netEvent(INet::Event::EVT_NET_CLOSED);

		netChk(Server::HostClose() );

		if( GetSocket() != INVALID_SOCKET )
		{
			NetSystem::CloseSocket( GetSocket() );
			SetSocket( INVALID_SOCKET );
		}

		EnqueueNetEvent(netEvent);

	Proc_End:

		netTrace( TRC_NET, "HostClose {0}, hr={1:X8}", GetLocalAddress(), hr );

		return hr;
	}


	//// Send message to connection with network device
	//HRESULT ServerUDPBase::SendMsg( IConnection *pConnection, Message::MessageData *pMsg )
	//{
	//	HRESULT hr = S_OK, hrErr = S_OK;

	//	Message::MessageID msgID = pMsg->GetMessageHeader()->msgID;
	//	UINT uiMsgLen = pMsg->GetMessageHeader()->Length;
	//	ConnectionUDPBase *pUDPCon = (ConnectionUDPBase*)pConnection;
	//	IOBUFFER_WRITE *pOverlapped = nullptr;

	//	Assert(pMsg->GetDataLength() == 0 || pMsg->GetMessageHeader()->Crc32 != 0 || pMsg->GetMessageHeader()->msgID.IDs.Policy == POLICY_NONE);


	//	netChk( Net::NetSystem::AllocBuffer(pOverlapped) );
	//	pOverlapped->SetupSendUDP(pUDPCon->GetSocket(), pUDPCon->GetRemoteSockAddr(), pMsg );

	//	hrErr = NetSystem::SendTo(pUDPCon->GetSocket(), pOverlapped);
	//	switch (hrErr)
	//	{
	//	case E_NET_TRY_AGAIN:
	//		break;
	//	case S_OK:
	//	case E_NET_IO_PENDING:
	//	case E_NET_WOULDBLOCK:
	//		break;
	//	case E_NET_CONNABORTED:
	//	case E_NET_CONNRESET:
	//	case E_NET_NETRESET:
	//	case E_NET_NOTCONN:
	//	case E_NET_NOTSOCK:
	//	case E_NET_SHUTDOWN:
	//		// Send fail by connection close
	//		// Need to disconnect
	//		pUDPCon->Disconnect();
	//		hr = E_NET_CONNECTION_CLOSED;
	//		goto Proc_End;
	//		break;
	//	default:
	//		netErr(E_NET_IO_SEND_FAIL);
	//		break;
	//	};

	//Proc_End:

	//	if( FAILED(hr) )
	//	{
	//		if( pOverlapped )
	//		{
	//			Util::SafeRelease( pOverlapped->pMsgs );
	//			Net::NetSystem::FreeBuffer(pOverlapped);
	//		}
	//		else
	//		{
	//			Util::SafeRelease( pMsg );
	//		}

	//		if( hr != E_NET_IO_SEND_FAIL )
	//		{
	//			netTrace( Trace::TRC_ERROR, "UDP Send Failed, ip:{0}, err:{1:X8}, hr:{2:X8}", pUDPCon->GetConnectionInfo(), hrErr, hr );
	//		}
	//		else
	//		{
	//			netTrace(Net::TRC_SENDRAW, "UDP Send Failed, ip:{0}, err:{1:X8}, hr:{2:X8}", pUDPCon->GetConnectionInfo(), hrErr, hr);
	//			return S_OK;
	//		}
	//	}
	//	else
	//	{
	//		if( msgID.IDs.Type == Message::MSGTYPE_NETCONTROL )
	//		{
	//			netTrace( TRC_NETCTRL, "UDP SendCtrl ip:{0}, msg:{1}, Len:{2}", pUDPCon->GetConnectionInfo(), msgID, uiMsgLen );
	//		}
	//		else
	//		{
	//			netTrace( TRC_SENDRAW, "UDP Send ip:{0}, msg:{1}, Len:{2}", pUDPCon->GetConnectionInfo(), msgID, uiMsgLen );
	//		}
	//	}

	//	return hr;
	//}

	//// Send message to connection with network device to dst addr
	//HRESULT ServerUDPBase::SendMsg( IConnection *pConnection, const sockaddr_in6& dstAddr, Message::MessageData *pMsg )
	//{
	//	HRESULT hr = S_OK, hrErr = S_OK;

	//	Message::MessageID msgID = pMsg->GetMessageHeader()->msgID;
	//	UINT uiMsgLen = pMsg->GetMessageHeader()->Length;

	//	ConnectionUDPBase *pUDPCon = static_cast<ConnectionUDPBase*>(pConnection);
	//	NetAddress dstAddress;
	//	IOBUFFER_WRITE *pOverlapped = nullptr;

	//	SOCKET socketToSend = INVALID_SOCKET;
	//	if( pConnection != nullptr && pUDPCon != nullptr )
	//	{
	//		socketToSend = pUDPCon->GetSocket();
	//		dstAddress = pUDPCon->GetConnectionInfo().Remote;
	//	}
	//	else
	//	{
	//		socketToSend = GetSocket();
	//		SockAddr2Addr( dstAddr, dstAddress);
	//	}

	//	
	//	// If the address isn't mapped yet. ignore it
	//	if( (pUDPCon != nullptr && pUDPCon->GetRemoteSockAddr().sin6_port == 0)
	//		|| dstAddr.sin6_port == 0)
	//	{
	//		netTrace( Net::TRC_NET, "Remote address isn't mapped yet, ignoring send CID:{0}", pConnection->GetCID() );
	//		goto Proc_End;
	//	}


	//	netChk( NetSystem::AllocBuffer(pOverlapped) );

	//	pOverlapped->SetupSendUDP(socketToSend, dstAddr, pMsg );
	//	pMsg = nullptr;


	//	hrErr = NetSystem::SendTo(socketToSend, pOverlapped);
	//	switch (hrErr)
	//	{
	//	case S_OK:
	//	case E_NET_IO_PENDING:
	//	case E_NET_TRY_AGAIN:
	//	case E_NET_WOULDBLOCK:
	//		break;
	//	case E_NET_CONNABORTED:
	//	case E_NET_CONNRESET:
	//	case E_NET_NETRESET:
	//	case E_NET_NOTCONN:
	//	case E_NET_NOTSOCK:
	//	case E_NET_SHUTDOWN:
	//		// Send fail by connection close
	//		// Need to disconnect
	//		pUDPCon->Disconnect();
	//		hr = E_NET_CONNECTION_CLOSED;
	//		goto Proc_End;
	//		break;
	//	default:
	//		netErr(E_NET_IO_SEND_FAIL);
	//		break;
	//	};


	//Proc_End:

	//	Util::SafeRelease( pMsg );

	//	if( FAILED(hr) )
	//	{
	//		if( pOverlapped )
	//		{
	//			Util::SafeRelease( pOverlapped->pMsgs );
	//			NetSystem::FreeBuffer(pOverlapped);
	//		}

	//		if( hr != E_NET_IO_SEND_FAIL )
	//		{
	//			netTrace( Trace::TRC_ERROR, "UDP Send Failed, ip:{0}, err:{1:X8}, hr:{2:X8}", dstAddress, hrErr, hr );
	//		}
	//		else
	//			return S_OK;
	//	}
	//	else
	//	{
	//		if( msgID.IDs.Type == Message::MSGTYPE_NETCONTROL )
	//		{
	//			netTrace( TRC_NETCTRL, "UDP SendCtrl ip:{0}, msg:{1}, Len:{2}", dstAddress, msgID, uiMsgLen );
	//		}
	//		else
	//		{
	//			netTrace( TRC_SENDRAW, "UDP Send ip:{0}, msg:{1}, Len:{2}", dstAddress, msgID, uiMsgLen );
	//		}
	//	}

	//	return hr;
	//}


	//HRESULT ServerUDPBase::SendMsg( IConnection *pConnection, UINT uiBuffSize, BYTE* pBuff )
	//{
	//	HRESULT hr = S_OK, hrErr = S_OK;
	//	ConnectionUDP *pUDPCon = (ConnectionUDP*)pConnection;
	//	IOBUFFER_WRITE *pOverlapped = nullptr;

	//	// If the address isn't mapped yet. ignore it
	//	if (pUDPCon->GetRemoteSockAddr().sin6_port == 0)
	//	{
	//		netTrace( Net::TRC_NET, "Remote address isn't mapped yet, ignoring send CID:{0}", pConnection->GetCID() );
	//		goto Proc_End;
	//	}

	//	netChk( Net::NetSystem::AllocBuffer(pOverlapped) );

	//	pOverlapped->SetupSendUDP(pUDPCon->GetSocket(), pUDPCon->GetRemoteSockAddr(), uiBuffSize, pBuff );
	//	pBuff = nullptr;


	//	hrErr = NetSystem::SendTo(pUDPCon->GetSocket(), pOverlapped);
	//	switch (hrErr)
	//	{
	//	case S_OK:
	//	case E_NET_IO_PENDING:
	//	case E_NET_TRY_AGAIN:
	//	case E_NET_WOULDBLOCK:
	//		break;
	//	case E_NET_CONNABORTED:
	//	case E_NET_CONNRESET:
	//	case E_NET_NETRESET:
	//	case E_NET_NOTCONN:
	//	case E_NET_NOTSOCK:
	//	case E_NET_SHUTDOWN:
	//		// Send fail by connection close
	//		// Need to disconnect
	//		pUDPCon->Disconnect();
	//		hr = E_NET_CONNECTION_CLOSED;
	//		goto Proc_End;
	//		break;
	//	default:
	//		netErr(E_NET_IO_SEND_FAIL);
	//		break;
	//	};

	//Proc_End:

	//	if( pBuff )
	//	{
	//		NetSystem::FreeGatheringBuffer(pBuff);
	//	}

	//	if( FAILED(hr) )
	//	{
	//		if( pOverlapped )
	//		{
	//			if( pOverlapped->pSendBuff )
	//				NetSystem::FreeGatheringBuffer(pOverlapped->pSendBuff);
	//			pOverlapped->pSendBuff = nullptr;
	//			Net::NetSystem::FreeBuffer(pOverlapped);
	//		}

	//		if( hr != E_NET_IO_SEND_FAIL )
	//		{
	//			netTrace( TRC_NETCTRL, "UDP Send Failed, ip:{0}, err:{1:X8}, hr:{2:X8}", pUDPCon->GetConnectionInfo(), hrErr, hr );
	//		}
	//		else
	//			return S_OK;
	//	}
	//	else
	//	{
	//		netTrace( TRC_SENDRAW, "UDP Send ip:{0}, Len:{1}", pUDPCon->GetConnectionInfo(), uiBuffSize );
	//	}

	//	return hr;
	//}


	// Pending recv New one
	HRESULT ServerUDPBase::PendingRecv( IOBUFFER_READ *pOver )
	{
		HRESULT hr = S_OK, hrErr;

		if (!NetSystem::IsProactorSystem())
			return S_OK;


		while (1)
		{
			hrErr = Recv(pOver);
			switch (hrErr)
			{
			case S_OK:
			case E_NET_IO_PENDING:
			case E_NET_TRY_AGAIN:
			case E_NET_WOULDBLOCK:
				goto Proc_End;// success
				break;
			case E_NET_NETUNREACH:
			case E_NET_CONNABORTED:
			case E_NET_CONNRESET:
			case E_NET_NETRESET:
				// some remove has problem with continue connection
				netTrace(TRC_NETCTRL, "UDP Remote has connection error err={0:X8}, {1}", hrErr, pOver->NetAddr.From);
				//break;
			default:
				// Unknown error
				netTrace(TRC_RECVRAW, "UDP Read Pending failed err={0:X8}", hrErr);
				//netErr( HRESULT_FROM_WIN32(iErr2) );
				break;
			};
		}

	Proc_End:



		return hr;
	}

	

	////////////////////////////////////////////////////////////////////////////////
	//
	//	UDP Server class
	//

	ServerUDP::ServerUDP(ServerID InServerID, NetClass localClass)
		:ServerUDPBase(InServerID, localClass)
		,m_ConnectionManager(Const::SVR_PUBLIC_CONNECTION_BUCKET_SIZE)
	{
		m_ConnectionManager.SetNetOwner( this );
	}

	ServerUDP::~ServerUDP()
	{
	}

	// called when reciving message
	HRESULT ServerUDP::OnIORecvCompleted( HRESULT hrRes, IOBUFFER_READ* &pIOBuffer )
	{
		HRESULT hr = S_OK;
		SharedPointerT<Connection> pConnection;
		IConnection::ConnectionInformation connectionInfo;
		bool bReleaseOnFail = false;
		sockaddr_in6 from;
		if (pIOBuffer != nullptr) from = pIOBuffer->NetAddr.From;
		else memset(&from, 0, sizeof(from));


		if( FAILED( hrRes ) )
		{
			switch( hrRes )
			{
			case E_NET_CONNECTION_CLOSED:
			case E_NET_IO_ABORTED:
				if (SUCCEEDED(GetConnectionManager().GetConnectionByAddr(from, pConnection)))
				{
					netTrace( TRC_RECV, "UDP bad connection state IP:{0}", from);
				}
				hr = hrRes;
				break;
			default:
				netTrace( Trace::TRC_ERROR, "UDP Recv Msg Failed, SvrUDP, IP:{0}, hr={1:X8}", from, hrRes );
				break;
			};
		}
		else if(pIOBuffer != nullptr)
		{
			if(pIOBuffer->TransferredSize < sizeof(Message::MessageHeader) )// invalid packet size
				goto Proc_End;

			if( GetIsEnableAccept()
				&& FAILED(GetConnectionManager().GetConnectionByAddr(pIOBuffer->NetAddr.From, pConnection)))
			{
				MsgNetCtrlConnect *pNetCtrl = (MsgNetCtrlConnect*)pIOBuffer->buffer;
				if (pNetCtrl->Length == sizeof(MsgNetCtrlConnect) && pNetCtrl->msgID.IDSeq.MsgID == PACKET_NETCTRL_CONNECT.IDSeq.MsgID && pNetCtrl->rtnMsgID.ID == BR_PROTOCOL_VERSION)
				{
					netChk( GetConnectionManager().PendingNewConnection(pIOBuffer->NetAddr.From, pNetCtrl) );
				}
				else if( pNetCtrl->msgID.ID == PACKET_NETCTRL_ACK )
				{
					// ignore this packet
				}
				else
				{
					// send disconnect
					netTrace( Trace::TRC_WARN, "HackWarn : Not allowered connection try from {0}", pIOBuffer->NetAddr.From );
				}

			}
			else
			{
				//pConnection = (*iterCon).pConnection;
			}

			if( pConnection != nullptr )
			{
				netChk( pConnection->OnRecv(pIOBuffer->TransferredSize, (BYTE*)pIOBuffer->buffer) );
			}
		}

	Proc_End:

		if( pConnection != nullptr )
		{
			if( bReleaseOnFail )
			{
				if (pConnection->GetConnectionState() != IConnection::STATE_DISCONNECTED)
					pConnection->CloseConnection();
				ReleaseConnection( (Connection*)pConnection );
			}
		}


		if (NetSystem::IsProactorSystem())
		{
			if (hrRes != E_NET_IO_ABORTED && pIOBuffer != nullptr)
				PendingRecv(pIOBuffer);
		}
		else
		{
			Util::SafeDelete(pIOBuffer);
		}

		return hr;
	}
	
	
	


} // namespace Net
} // namespace BR

