////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Network Server Peer implementation 
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Common/HRESNet.h"
#include "Common/Thread.h"
#include "Common/StrUtil.h"
#include "Net/Connection.h"
#include "Net/NetTrace.h"
#include "Net/NetDef.h"
#include "Net/NetSystem.h"
#include "Net/NetServer.h"
#include "Net/NetServerPeer.h"
#include "Net/NetConst.h"
#include "Protocol/ProtocolVer.h"
#include "Net/NetCtrl.h"


namespace BR {
namespace Net {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Peer network class
	//

	ServerPeer::ServerPeer( ServerID InServerID, NetClass localClass )
		: ServerNet( InServerID, localClass )
		, m_ConnectionManager(Net::Const::SVR_PRIVATE_CONNECTION_BUCKET_SIZE)
		, m_CIDGen(0)
		, m_pRecvBuffers(nullptr)
		, m_PendingRecvCnt(0)
	{
		m_ConnectionManager.SetNetOwner( this );

		if (!NetSystem::IsProactorSystem())
			SetWriteQueue(new WriteBufferQueue);
	}

	ServerPeer::~ServerPeer()
	{
		HostClose();

		Util::SafeDeleteArray( m_pRecvBuffers );
		if (GetWriteQueue()) delete GetWriteQueue();
	}


	
	// Close all connection
	HRESULT ServerPeer::CloseAllConnection()
	{
		return m_ConnectionManager.PendingCloseAllConnection();
	}


	// Get Active(non-free) connection
	CounterType ServerPeer::GetActiveConnectionCount()
	{
		return m_ConnectionManager.GetNumActiveConnection();
	}


	// called when reciving message
	HRESULT ServerPeer::OnIORecvCompleted( HRESULT hrRes, IOBUFFER_READ *pIOBuffer )
	{
		HRESULT hr = S_OK;
		SharedPointerT<Connection> pConnection;

		sockaddr_in6 from;
		if (pIOBuffer != nullptr) from = pIOBuffer->NetAddr.From;
		else memset(&from, 0, sizeof(from));



		if( FAILED( hrRes ) )
		{
			switch( hrRes )
			{
			case E_NET_CONNECTION_CLOSED:
			case E_NET_IO_ABORTED:
				if (SUCCEEDED(m_ConnectionManager.GetConnectionByAddr(from, pConnection)))
				{
					// Release connection table
					if (pConnection->GetConnectionState() != IConnection::STATE_DISCONNECTED)
						pConnection->CloseConnection();
				}
				hr = hrRes;
				break;
			default:
				//netTrace( TRC_RECV, "UDP Recv Msg Failed, SvrUDP, IP:{0}, hr={1:X8}", addr, hrRes );
				break;
			};
		}


		if(pIOBuffer == nullptr || pIOBuffer->TransferredSize == 0)
			goto Proc_End;

		Assert(pIOBuffer->bIsPending == true);
		pIOBuffer->bIsPending = false;
		m_PendingRecvCnt.fetch_sub(1, std::memory_order_relaxed);


		if (FAILED(m_ConnectionManager.GetConnectionByAddr(from, pConnection))) // not mapped yet. We need to make a new connection
		{
			// check control packet
			MsgNetCtrlConnect *pNetCtrl = (MsgNetCtrlConnect*)pIOBuffer->buffer;
			if (pNetCtrl->Length == sizeof(MsgNetCtrlConnect) && pNetCtrl->msgID.IDSeq.MsgID == PACKET_NETCTRL_CONNECT.IDSeq.MsgID && pNetCtrl->rtnMsgID.ID == BR_PROTOCOL_VERSION)
			{
				if( GetIsEnableAccept() )
				{
					// Peer network only allow registered connection
					netChk( m_ConnectionManager.PendingNewConnection(from, pNetCtrl) );
				}
			}
			else
			{
				netTrace( Trace::TRC_WARN, "HackWarn : Invalid packet From {0}", from);
				netErr( E_UNEXPECTED );
			}

		}
		else
		{
			if(pIOBuffer->TransferredSize >= sizeof(Message::MessageHeader) )// invalid packet size
			{
				if (pConnection->GetConnectionState() == IConnection::STATE_DISCONNECTED)
				{
					MsgNetCtrlConnect *pNetCtrl = (MsgNetCtrlConnect*)pIOBuffer->buffer;
					if( pNetCtrl->Length == sizeof(MsgNetCtrlConnect) 
						&& pNetCtrl->msgID.IDSeq.MsgID == PACKET_NETCTRL_CONNECT.IDSeq.MsgID
						&& pNetCtrl->rtnMsgID.ID == BR_PROTOCOL_VERSION )
					{
						// Peer network only allow registered connection
						netChk( m_ConnectionManager.PendingInitConnection((Connection*)pConnection) );
					}
					else
					{
						netTrace( Trace::TRC_WARN, "Unexpected packet From {0}", from);
						netErr( E_UNEXPECTED );
					}
				}
				else
				{
					netChk( pConnection->OnRecv(pIOBuffer->TransferredSize, (BYTE*)pIOBuffer->buffer) );
				}
			}
		}


	Proc_End:

		// Release connection table
		//iterCon = nullptr;

		if( hrRes != E_NET_IO_ABORTED )
			PendingRecv( pIOBuffer );
		else
		{
			netTrace( Trace::TRC_ERROR, "NoPending {0}", from);
		}

		return hr;
	}


	// called when send completed
	HRESULT ServerPeer::OnIOSendCompleted( HRESULT hrRes, IOBUFFER_WRITE *pIOBuffer )
	{
		NetSystem::FreeGatheringBuffer( pIOBuffer->pSendBuff );
		Util::SafeRelease( pIOBuffer->pMsgs );
		NetSystem::FreeBuffer( pIOBuffer );
		return S_OK;
	}


	// Release instance
	void ServerPeer::Release()
	{
		delete this;
	}

	// check about initialize
	bool ServerPeer::IsReady()
	{
		return GetSocket() != INVALID_SOCKET;
	}

	// Get connection from connection ID
	HRESULT ServerPeer::GetConnection(UINT uiCID, SharedPointerT<Connection> &pIConnection)
	{
		return m_ConnectionManager.GetConnectionByCID(uiCID, pIConnection);
	}



	// Send message to connection with network device
	HRESULT ServerPeer::SendMsg( IConnection *pConnection, Message::MessageData *pMsg )
	{
		HRESULT hr = S_OK, hrErr = S_OK;

		Message::MessageID msgID = pMsg->GetMessageHeader()->msgID;
		UINT uiMsgLen = pMsg->GetMessageHeader()->Length;
		ConnectionUDP *pUDPCon = (ConnectionUDP*)pConnection;
		IOBUFFER_WRITE *pOverlapped = nullptr;

		Assert(uiMsgLen == 0 || pMsg->GetMessageHeader()->Crc32 != 0);


		netChk( Net::NetSystem::AllocBuffer(pOverlapped) );
		pOverlapped->SetupSendUDP(pUDPCon->GetSocket(), pUDPCon->GetRemoteSockAddr(), pMsg );
		hrErr = NetSystem::SendTo(pUDPCon->GetSocket(), pOverlapped);
		switch (hrErr)
		{
		case S_OK:
		case E_NET_IO_PENDING:
		case E_NET_TRY_AGAIN:
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
			pUDPCon->Disconnect();
			hr = E_NET_CONNECTION_CLOSED;
			goto Proc_End;
			break;
		default:
			netErr(E_NET_IO_SEND_FAIL);
			break;
		};

	Proc_End:

		if( FAILED(hr) )
		{
			if( pOverlapped )
			{
				Util::SafeRelease( pOverlapped->pMsgs );
				Net::NetSystem::FreeBuffer(pOverlapped);
			}
			else
			{
				Util::SafeRelease( pMsg );
			}

			if( hr != E_NET_IO_SEND_FAIL )
			{
				netTrace( Trace::TRC_ERROR, "UDP Send Failed, ip:{0}, err:{1:X8}, hr:{2:X8}", pUDPCon->GetConnectionInfo().Remote, hrErr, hr );
			}
			else
				return S_OK;
		}
		else
		{
			if( msgID.IDs.Type == Message::MSGTYPE_NETCONTROL )
			{
				netTrace( TRC_NETCTRL, "UDP SendCtrl ip:{0}, msg:{1}, Len:{2}", pUDPCon->GetConnectionInfo().Remote, msgID, uiMsgLen );
			}
			else
			{
				netTrace( TRC_SENDRAW, "UDP Send ip:{0}, msg:{1}, Len:{2}", pUDPCon->GetConnectionInfo().Remote, msgID, uiMsgLen );
			}
		}

		return hr;
	}

	// Send message to connection with network device to dst addr
	HRESULT ServerPeer::SendMsg( IConnection *pConnection, const sockaddr_in6& dstAddr, Message::MessageData *pMsg )
	{
		return E_NOTIMPL;
	}

	HRESULT ServerPeer::SendMsg( IConnection *pConnection, UINT uiBuffSize, BYTE* pBuff )
	{
		HRESULT hr = S_OK, hrErr = S_OK;

		ConnectionUDP *pUDPCon = (ConnectionUDP*)pConnection;

		IOBUFFER_WRITE *pOverlapped = nullptr;
		netChk( Net::NetSystem::AllocBuffer(pOverlapped) );

		pOverlapped->SetupSendUDP(pUDPCon->GetSocket(), pUDPCon->GetRemoteSockAddr(), uiBuffSize, pBuff );


		hrErr = NetSystem::SendTo(pUDPCon->GetSocket(), pOverlapped);
		switch (hrErr)
		{
		case S_OK:
		case E_NET_IO_PENDING:
		case E_NET_TRY_AGAIN:
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
			pUDPCon->Disconnect();
			hr = E_NET_CONNECTION_CLOSED;
			goto Proc_End;
			break;
		default:
			netErr(E_NET_IO_SEND_FAIL);
			break;
		};

		//if( WSASendTo( pUDPCon->GetSocket(), &pOverlapped->wsaBuff, 1, nullptr, 0,
		//	(sockaddr*)&pUDPCon->GetRemoteSockAddr(), sizeof(sockaddr_in6), 
		//	pOverlapped, nullptr ) == SOCKET_ERROR )
		//{
		//	iWSAErr = WSAGetLastError();
		//	if( iWSAErr != WSA_IO_PENDING )
		//	{
		//		switch( iWSAErr )
		//		{
		//		case WSAECONNABORTED:
		//		case WSAECONNRESET:
		//		case WSAENETRESET:
		//		case WSAENOTCONN:
		//		case WSAENOTSOCK:
		//		case WSAESHUTDOWN:
		//			// Send fail by connection close
		//			// Need to disconnect
		//			pUDPCon->Disconnect();
		//			netErrSilent( E_NET_CONNECTION_CLOSED );
		//			break;
		//		default:
		//			netErr( E_NET_IO_SEND_FAIL );
		//			break;
		//		};
		//	}
		//}
		//else
		//{
		//	// Send done just free all
		//}

	Proc_End:

		if( FAILED(hr) )
		{
			Util::SafeDelete( pBuff );
			if( pOverlapped )
			{
				pOverlapped->pSendBuff = nullptr;
				Net::NetSystem::FreeBuffer(pOverlapped);
			}

			if( hr != E_NET_IO_SEND_FAIL )
			{
				netTrace( Trace::TRC_ERROR, "UDP Send Failed, ip:{0}, err:{1:X8}, hr:{2:X8}", pUDPCon->GetConnectionInfo().Remote, hrErr, hr );
			}
			else
				return S_OK;
		}
		else
		{
			netTrace( TRC_SENDRAW, "UDP Send ip:{0}, Len:{1}", pUDPCon->GetConnectionInfo().Remote, uiBuffSize );
		}

		return hr;
	}

	// Pending recv New one
	HRESULT ServerPeer::PendingRecv( IOBUFFER_READ *pOver )
	{
		HRESULT hr = S_OK, hrErr = S_OK;
		//int iErr = 0;//, iSockLen = sizeof(sockaddr_in6);

		if (!NetSystem::IsProactorSystem())
			return S_OK;

		pOver->SetupRecvUDP(0);

		Assert(pOver->bIsPending == false);
		pOver->bIsPending = true;
		m_PendingRecvCnt.fetch_add(1, std::memory_order_relaxed);



		while (1)
		{
			hrErr = NetSystem::RecvFrom(GetSocket(), pOver);
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

		//if( WSARecvFrom( GetSocket(), &pOver->wsaBuff, 1, NULL, &pOver->dwFlags, (sockaddr*)&pOver->From, &pOver->iSockLen, pOver, nullptr ) == SOCKET_ERROR )
		//{
		//	iErr = WSAGetLastError();
		//	switch( iErr )
		//	{
		//	case WSA_IO_PENDING:
		//		//goto Proc_End;// success
		//		
		//		break;
		//	case WSAENETUNREACH:
		//	case WSAECONNABORTED:
		//	case WSAECONNRESET:
		//	case WSAENETRESET:
		//		// some remove has problem with continue connection
		//		netTrace( TRC_NETCTRL, "UDP Remote has connection error err={0}, {1}", iErr, pOver->From );
		//		//goto Proc_End;
		//	default:
		//		// Unknown error
		//		netTrace( Trace::TRC_ERROR, "UDP Read Pending failed err={0}", iErr );
		//		netErr( HRESULT_FROM_WIN32(iErr) );
		//		break;
		//	};
		//}
		//else
		//{
		//	netTrace( TRC_NETCTRL, "UDP Error Directive receive err={0}, {1}", iErr, pOver->From );
		//}



	Proc_End:


		return hr;
	}



	// Open host and start listen
	HRESULT ServerPeer::HostOpen( NetClass netCls, const char *strLocalIP, USHORT usLocalPort )
	{
		HRESULT hr = S_OK;
		SOCKET socket = INVALID_SOCKET;
		NetAddress localAddr;
		INT iOptValue;
		sockaddr_in6 bindAddr;
		INet::Event netEvent(INet::Event::EVT_NET_INITIALIZED);

		netTrace(Trace::TRC_TRACE, "Opening Server Peer, {0}:{1}", strLocalIP, usLocalPort);

		netChk( NetSystem::OpenSystem( Const::SVR_OVERBUFFER_COUNT, Const::SVR_NUM_RECV_THREAD, Const::PACKET_GATHER_SIZE_MAX) );

		if( GetSocket() != INVALID_SOCKET )
			return S_OK;

		// set local address
		svrChk( StrUtil::StringCpy( localAddr.strAddr, strLocalIP ) );
		localAddr.usPort = usLocalPort;
		SetLocalAddress( localAddr );

		netTrace(Trace::TRC_TRACE, "Open Server Peer Host {0}:{1}", strLocalIP, usLocalPort );

		//socket = WSASocket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP, nullptr, 0, WSA_FLAG_OVERLAPPED);
		socket = NetSystem::Socket(SockFamily::IPV6, SockType::DataGram);
		if( socket == INVALID_SOCKET )
		{
			netTrace(Trace::TRC_ERROR, "Failed to Open Server Socket {0:X8}", GetLastWSAHRESULT());
			netErr( E_UNEXPECTED );
		}

		iOptValue = Const::CLI_RECV_BUFFER_SIZE;
		if( setsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_RCVBUF = {0}, err = {1:X8}", iOptValue, GetLastWSAHRESULT() );
			netErr( E_UNEXPECTED );
		}

		iOptValue = Const::CLI_SEND_BUFFER_SIZE;
		if( setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_SNDBUF = {0}, err = {1:X8}", iOptValue, GetLastWSAHRESULT() );
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

		netChk(NetSystem::RegisterSocket(SockType::DataGram, this));

		m_NetClass = netCls;

		netChk( m_ConnectionManager.InitManager( Const::SVR_PRIVATE_CONNECTION_POOLCACHE ) );


		if (NetSystem::IsProactorSystem())
		{
			// Pending recv
			Util::SafeDeleteArray(m_pRecvBuffers);
			netMem(m_pRecvBuffers = new IOBUFFER_READ[Const::SVR_NUM_RECV_THREAD]);
			memset(m_pRecvBuffers, 0, sizeof(IOBUFFER_READ)*Const::SVR_NUM_RECV_THREAD);

			m_PendingRecvCnt = 0;

			for (INT iRecv = 0; iRecv < Const::SVR_NUM_RECV_THREAD; iRecv++)
				netChk(PendingRecv(&m_pRecvBuffers[iRecv]));
		}


		EnqueueNetEvent(netEvent);


	Proc_End:

		if( FAILED(hr) )
			HostClose();

		netTrace( TRC_NET, "HostOpen {0}, hr={1:X8}", GetLocalAddress(), hr );

		return hr;
	}

	// Close host and close all connections
	HRESULT ServerPeer::HostClose()
	{
		HRESULT hr = S_OK;
		INet::Event myEvent(INet::Event::EVT_NET_CLOSED);

		m_ConnectionManager.Stop( true );


		if( GetSocket() != INVALID_SOCKET )
		{
			NetSystem::CloseSocket( GetSocket() );
			SetSocket( INVALID_SOCKET );

			NetSystem::CloseSystem();
		}

		EnqueueNetEvent(myEvent);


	//Proc_End:

		netTrace( TRC_NET, "HostClose {0}, hr={1:X8}", GetLocalAddress(), hr );

		return hr;
	}



	// Connect to other peer
	HRESULT ServerPeer::RegisterServerConnection( ServerID serverID, NetClass netClass, const char *strDstIP, USHORT usDstPort, Net::IConnection* &pConnection )
	{
		HRESULT hr = S_OK;
		Net::IConnection::ConnectionInformation connectionInfo;
		ConnectionUDPServerPeer *pConn = nullptr;
		uintptr_t CID = 0;


		netMem( pConn = dynamic_cast<ConnectionUDPServerPeer*>(m_ConnectionManager.NewConnection()) );

		memset( &connectionInfo, 0, sizeof(connectionInfo) );

		connectionInfo.SetLocalInfo( GetNetClass(), GetLocalAddress(), GetServerID() );

		netChk( StrUtil::StringCpy( connectionInfo.Remote.strAddr, strDstIP ) );
		connectionInfo.Remote.usPort = usDstPort;
		connectionInfo.RemoteClass = netClass;
		connectionInfo.RemoteID = serverID;

		pConn->SetHeartbitTry( Const::SVR_HEARTBIT_TIME_PEER );
		pConn->SetConnectingTimeOut( LONG_MAX ); // infinite retry

		netChk( pConn->SetMessageWindowSize( 256, 256 ) );
		netChk( pConn->InitConnection( GetSocket(), connectionInfo ) );
		netTrace(TRC_CONNECTION, "Initialize connection CID:{0}, Addr:{1}:{2}", pConn->GetCID(), pConn->GetConnectionInfo().Remote.strAddr, pConn->GetConnectionInfo().Remote.usPort);

		pConn->SetWriteQueueUDP(GetWriteQueue());

		// Server entity will update this connection, thus just adding address map is enough
		netChk( m_ConnectionManager.PendingConnection( pConn ) );

		CID = pConn->GetCID();

		pConnection = pConn;
		pConn = nullptr;

	Proc_End:

		if( pConn ) delete pConn;

		if( SUCCEEDED(hr) )
		{
			netTrace(TRC_NET, "ServerPeer Allowing Server:%3%:%4%, {0}:{1}, CID:{2}", strDstIP, usDstPort, CID, netClass, serverID);
		}

		return hr;
	}



} // namespace Net	
} // namespace BR

