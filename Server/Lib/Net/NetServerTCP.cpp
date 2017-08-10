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
#include "Common/ResultCode/BRResultCodeNet.h"
#include "Net/NetConst.h"
#include "Net/NetSystem.h"
#include "Common/Thread.h"
#include "Common/StrUtil.h"
#include "Net/Connection.h"
#include "Net/NetTrace.h"
#include "Net/NetDef.h"
#include "Net/NetServer.h"
#include "Protocol/ProtocolVer.h"



namespace BR {
namespace Net {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	TCP Server class
	//

	ServerTCP::ServerTCP(ServerID InServerID, NetClass localClass)
		: Server(InServerID, localClass)
		, INetIOCallBack(GetSocket())
		, m_PendingAccept(0)
	{
		// ServerTCP listen only for accept
		//SetWriteQueue(new WriteBufferQueue);
	}

	ServerTCP::~ServerTCP()
	{
		//if (GetWriteQueue()) delete GetWriteQueue();
	}
	
	Result ServerTCP::SetupSocketOption(SOCKET socket)
	{
		Result hr = ResultCode::SUCCESS;
		socklen_t iOptValue = 0;

		iOptValue = Net::Const::SVR_RECV_BUFFER_SIZE;
		if (setsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_RCVBUF = {0}, err = {1:X8}", iOptValue, GetLastWSAResult());
			netErr(ResultCode::UNEXPECTED);
		}

		iOptValue = Net::Const::SVR_SEND_BUFFER_SIZE;
		if (setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_SNDBUF = {0}, err = {1:X8}", iOptValue, GetLastWSAResult());
			netErr(ResultCode::UNEXPECTED);
		}

	Proc_End:

		return hr;
	}

	Result ServerTCP::Accept(IOBUFFER_ACCEPT* &pAcceptInfo)
	{
		Result hr = ResultCode::SUCCESS, hrErr = ResultCode::SUCCESS;
		SOCKET sockAccept = INVALID_SOCKET;

		pAcceptInfo = new IOBUFFER_ACCEPT;
		netMem(pAcceptInfo);

		memset(pAcceptInfo, 0, sizeof(IOBUFFER_ACCEPT));

		sockAccept = NetSystem::Socket(GetLocalAddress().SocketFamily, SockType::Stream);
		if (sockAccept == INVALID_SOCKET)
		{
			netTrace(Trace::TRC_ERROR, "Failed to Open Accept Socket {0:X8}", GetLastWSAResult());
			netErr(ResultCode::UNEXPECTED);
		}

		pAcceptInfo->SetupAccept(sockAccept);

		netChk(SetupSocketOption(pAcceptInfo->sockAccept));

		hrErr = NetSystem::Accept(GetSocket(), pAcceptInfo);
		switch ((uint32_t)hrErr)
		{
		case ResultCode::SUCCESS:
		case ResultCode::E_NET_WOULDBLOCK:
		case ResultCode::E_NET_IO_PENDING:
			// successed
			break;
		case ResultCode::E_NET_TRY_AGAIN:
			netTrace(TRC_NET, "TCP accept busy, try again {0} accepts are queued", m_PendingAccept.load(std::memory_order_relaxed));
		default:
			hr = hrErr;
			break;
		}

	Proc_End:

		if (!(hr))
		{
			Util::SafeDelete(pAcceptInfo);
		}

		return hr;
	}

	// On New connection accepted
	Result ServerTCP::OnIOAccept( Result hrRes, IOBUFFER_ACCEPT *pOverAccept )
	{
		Result hr = ResultCode::SUCCESS;
		sockaddr_storage remoteAddr;
		SOCKET sockSvr = GetSocket();
		SOCKET sockAccept = pOverAccept->sockAccept;
		Net::IConnection::ConnectionInformation connectionInfo;

		netChkPtr( pOverAccept );

		if(NetSystem::IsProactorSystem())
			m_PendingAccept.fetch_sub(1, std::memory_order_relaxed);

		memset( &connectionInfo, 0, sizeof(connectionInfo) );

		// skip if not accept mode
		if( !GetIsEnableAccept() )
		{
			goto Proc_End;
		}

		if( ( hrRes ) )
		{
			netChk(NetSystem::HandleAcceptedSocket(sockSvr, pOverAccept, remoteAddr));

			connectionInfo.Local = GetLocalAddress();
			connectionInfo.LocalClass = GetLocalClass();
			connectionInfo.LocalID = GetServerID();
			SockAddr2Addr(remoteAddr, connectionInfo.Remote );

			ConnectionPtr pConnOut;
			netChk(OnAcceptedSocket(sockAccept, remoteAddr, connectionInfo, pConnOut));

			sockAccept = INVALID_SOCKET;
		}
		else
		{
			hr = hrRes;
		}


	Proc_End:

		Util::SafeDelete(pOverAccept);

		if( sockAccept != INVALID_SOCKET )
		{
			NetSystem::CloseSocket( sockAccept );
		}

		PendingAccept();

		return hr;
	}

	// handle Socket accept
	Result ServerTCP::OnAcceptedSocket( SOCKET acceptedSocket, const sockaddr_storage& remoteSockAddr, const IConnection::ConnectionInformation& connectionInfo, ConnectionPtr &pConnOut )
	{
		Result hr = ResultCode::SUCCESS;
		ConnectionTCP *pConnection = nullptr;
		ConnectionPtr pConn;
		uintptr_t cid = 0;

		unused(remoteSockAddr);
		
		// Create New connection for accept
		pConnection = (ConnectionTCP*)GetConnectionManager().NewConnection();
		if( pConnection == nullptr )// Maybe maxconnection ?
		{
			netTrace( Trace::TRC_ERROR, "Failed to allocated new connection now active:{0}", GetConnectionManager().GetNumActiveConnection() );
			netErr( ResultCode::FAIL );
		}


		pConn = pConnection;
		pConnOut = pConnection;
		cid = pConnection->GetCID();

		pConnection->SetConnectingTimeOut( Const::CONNECTION_TIMEOUT );

		// Initialize connection
		netChk( pConnection->InitConnection( acceptedSocket, connectionInfo ) );
		netTrace(TRC_CONNECTION, "Connection Accepted CID:{0}, Addr:{1}, sock:{2}", pConn->GetCID(), pConn->GetConnectionInfo().Remote, acceptedSocket);


		netChk(NetSystem::RegisterSocket(SockType::Stream, pConnection->GetIOCallback()));

		pConn = nullptr;

			
	Proc_End:

		if( !(hr) )
		{
			if( pConn != nullptr )
			{
				GetConnectionManager().PendingReleaseConnection(pConn);
			}

			netTrace( Trace::TRC_ERROR, "Tcp Accept failed  hr = {0:X8}", hr );
		}
		else
		{
			if( pConn != nullptr )
				netTrace( TRC_NET, "Net Con Accept Svr:{0}, CID:{1}, From {2}", GetLocalAddress().usPort, cid, connectionInfo.Remote );
		}

		return hr;

	}

	Result ServerTCP::Recv(IOBUFFER_READ* pIOBuffer)
	{
		return ResultCode::NOT_IMPLEMENTED;
	}

	// called when reciving message
	Result ServerTCP::OnIORecvCompleted( Result hrRes, IOBUFFER_READ* &pIOBuffer )
	{
		Assert(false);
		return ResultCode::NOT_IMPLEMENTED;
	}

	Result ServerTCP::OnSendReady()
	{
		return ProcessSendQueue();
	}

	Result ServerTCP::SendBuffer(IOBUFFER_WRITE *pIOBuffer)
	{
		return ResultCode::NOT_IMPLEMENTED;
	}

	// called when send completed
	Result ServerTCP::OnIOSendCompleted( Result hrRes, IOBUFFER_WRITE *pIOBuffer )
	{
		Util::SafeDeleteArray( pIOBuffer->pSendBuff );
		Util::SafeRelease( pIOBuffer->pMsgs );
		NetSystem::FreeBuffer( pIOBuffer );
		return ResultCode::SUCCESS;
	}


	// Pending Accept New one
	Result ServerTCP::PendingAccept()
	{
		Result hr = ResultCode::SUCCESS;
		UINT iMaxAccept = Util::Max((UINT)Const::TCP_ACCEPT_PENDING_MAX, (UINT)1);

		// skip if not accept mode
		if(!NetSystem::IsProactorSystem() || !GetIsEnableAccept())
			goto Proc_End;

		// if server not initialized
		if( GetSocket() == INVALID_SOCKET )
			goto Proc_End;

		// used buffer count == pended accept count
		while(m_PendingAccept.load(std::memory_order_acquire) < iMaxAccept)
		{
			IOBUFFER_ACCEPT* pAcceptInfo = nullptr;

			netTrace(TRC_CONNECTION, "Pending accept : {0}, {1}", m_PendingAccept.load(std::memory_order_relaxed), GetConnectionManager().GetNumActiveConnection());

			netChk(Accept(pAcceptInfo));

			m_PendingAccept.fetch_add(1, std::memory_order_relaxed);

		}// while

	Proc_End:


		return hr;
	}


	// Enable/Disable new connection acception
	void ServerTCP::SetIsEnableAccept( bool bIsEnable )
	{
		Server::SetIsEnableAccept( bIsEnable );

		if( IsReady() )
			PendingAccept();
	}

	// Open host and start listen
	Result ServerTCP::HostOpen( NetClass netCls, const char *strLocalIP, USHORT usLocalPort )
	{
		Result hr = ResultCode::SUCCESS;
		SOCKET socket = INVALID_SOCKET;
		INT32 iOptValue;
		int bOptValue;
		sockaddr_storage bindAddr;
		INet::Event netEvent(INet::Event::EVT_NET_INITIALIZED);

		if( GetSocket() != INVALID_SOCKET )// already initialized?
			return ResultCode::SUCCESS;

		netChk(Server::HostOpen( netCls, strLocalIP, usLocalPort ) );


		socket = NetSystem::Socket(GetLocalAddress().SocketFamily, SockType::Stream);
		if( socket == INVALID_SOCKET )
		{
			netTrace(Trace::TRC_ERROR, "Failed to Open Server Socket {0:X8}", GetLastWSAResult());
			netErr( ResultCode::UNEXPECTED );
		}

		netTrace(Trace::TRC_TRACE, "Open Server TCP Host {0}:{1}, sock:{2}", strLocalIP, usLocalPort, socket);

		iOptValue = Const::SVR_RECV_BUFFER_SIZE;
		if( setsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_RCVBUF = {0}, err = {1:X8}", iOptValue, GetLastWSAResult() );
			netErr( ResultCode::UNEXPECTED );
		}

		iOptValue = Const::SVR_SEND_BUFFER_SIZE;
		if( setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_SNDBUF = {0}, err = {1:X8}", iOptValue, GetLastWSAResult() );
			netErr( ResultCode::UNEXPECTED );
		}

		bOptValue = TRUE;
		if( setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&bOptValue, sizeof(bOptValue)) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_REUSEADDR = {0}, err = {1:X8}", bOptValue, GetLastWSAResult() );
			netErr( ResultCode::UNEXPECTED );
		}

		if (GetSocketAddr().ss_family== AF_INET6)
		{
			iOptValue = FALSE;
			if (setsockopt(socket, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
			{
				netTrace(Trace::TRC_ERROR, "Failed to change socket option IPV6_V6ONLY = {0}, err = {1:X8}", iOptValue, GetLastWSAResult());
				netErr( ResultCode::UNEXPECTED );
			}
		}

		GetAnyBindAddr(GetSocketAddr(), bindAddr);
		if (bind(socket, (sockaddr*)&bindAddr, GetSocketAddrSize()) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Socket bind failed, TCP {0:X8}", GetLastWSAResult() );
			netErr( ResultCode::UNEXPECTED );
		}

		if( listen(socket, SOMAXCONN) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to listen socket {0:X8}", GetLastWSAResult() );
			netErr( ResultCode::UNEXPECTED );
		}

		SetSocket( socket );
		socket = INVALID_SOCKET;

		GetIOFlagsEditable().IsListenSocket = 1;
		netChk(NetSystem::RegisterSocket(SockType::Stream, this));

		// Prepare connection
		netChk( GetConnectionManager().InitManager( Const::SVR_PUBLIC_CONNECTION_POOLCACHE ) );



		EnqueueNetEvent( netEvent ); 


		netChk( PendingAccept() );

	Proc_End:

		if( socket != INVALID_SOCKET )
			NetSystem::CloseSocket( socket );

		netTrace( TRC_NET, "HostOpen {0}, hr={1:X8}", GetLocalAddress(), hr );

		return hr;
	}

	// Close host and close all connections
	Result ServerTCP::HostClose()
	{
		Result hr = ResultCode::SUCCESS;
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


	// Release Connection, Make connection to send free state
	Result ServerTCP::ReleaseConnection( IConnection* pIConnection )
	{
		Result hr = ResultCode::SUCCESS;
		
		netChk(Server::ReleaseConnection( pIConnection ) );

		netChk( PendingAccept() );

	Proc_End:


		return hr;
	}


	//// Send message to connection with network device
	//Result ServerTCP::SendMsg( IConnection *pConnection, Message::MessageData *pMsg )
	//{
	//	Result hr = ResultCode::SUCCESS, hrErr = ResultCode::SUCCESS;

	//	Message::MessageID msgID = pMsg->GetMessageHeader()->msgID;
	//	UINT uiMsgLen = pMsg->GetMessageHeader()->Length;

	//	ConnectionTCP *pTCPCon = (ConnectionTCP*)pConnection;

	//	Assert(pMsg->GetRefCount() > 0);

	//	IOBUFFER_WRITE *pOverlapped = nullptr;
	//	netChk( Net::NetSystem::AllocBuffer(pOverlapped) );
	//	pOverlapped->SetupSendTCP( pMsg );


	//	hrErr = NetSystem::Send(pTCPCon->GetSocket(), pOverlapped);
	//	switch (hrErr)
	//	{
	//	case ResultCode::SUCCESS:
	//	case ResultCode::E_NET_IO_PENDING:
	//	case ResultCode::E_NET_TRY_AGAIN:
	//	case ResultCode::E_NET_WOULDBLOCK:
	//		break;
	//	case ResultCode::E_NET_CONNABORTED:
	//	case ResultCode::E_NET_CONNRESET:
	//	case ResultCode::E_NET_NETRESET:
	//	case ResultCode::E_NET_NOTCONN:
	//	case ResultCode::E_NET_NOTSOCK:
	//	case ResultCode::E_NET_SHUTDOWN:
	//		// Send fail by connection close
	//		// Need to disconnect
	//		pTCPCon->Disconnect();
	//		hr = ResultCode::E_NET_CONNECTION_CLOSED;
	//		goto Proc_End;
	//		break;
	//	default:
	//		netErr(ResultCode::E_NET_IO_SEND_FAIL);
	//		break;
	//	};

	//Proc_End:

	//	if( !(hr) )
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

	//		if( hr != ResultCode::E_NET_IO_SEND_FAIL )
	//		{
	//			netTrace(Trace::TRC_ERROR, "TCP Send Failed, CID:{3}, ip:{0}, err:{1:X8}, hr:{2:X8}", pTCPCon->GetConnectionInfo().Remote, hrErr, hr, pTCPCon->GetCID());
	//		}
	//		else
	//			return ResultCode::SUCCESS;
	//	}
	//	else
	//	{
	//		if( msgID.IDs.Type == Message::MSGTYPE_NETCONTROL )
	//		{
	//			netTrace(TRC_TCPNETCTRL, "TCP Ctrl CID:{3}, ip:{0}, msg:{1}, Len:{2}", pTCPCon->GetConnectionInfo().Remote, msgID, uiMsgLen, pTCPCon->GetCID());
	//		}
	//		else
	//		{
	//			netTrace(TRC_TCPSENDRAW, "TCP Send CID:{3}, ip:{0}, msg:{1}, Len:{2}", pTCPCon->GetConnectionInfo().Remote, msgID, uiMsgLen, pTCPCon->GetCID());
	//		}
	//	}


	//	return hr;
	//}

	//Result ServerTCP::SendMsg( IConnection *pConnection, UINT uiBuffSize, BYTE* pBuff )
	//{
	//	Result hr = ResultCode::SUCCESS, hrErr = ResultCode::SUCCESS;

	//	ConnectionTCP *pTCPCon = (ConnectionTCP*)pConnection;

	//	IOBUFFER_WRITE *pOverlapped = nullptr;
	//	netChk( Net::NetSystem::AllocBuffer(pOverlapped) );

	//	pOverlapped->SetupSendTCP( uiBuffSize, pBuff );

	//	hrErr = NetSystem::Send(pTCPCon->GetSocket(), pOverlapped);
	//	switch (hrErr)
	//	{
	//	case ResultCode::SUCCESS:
	//	case ResultCode::E_NET_IO_PENDING:
	//	case ResultCode::E_NET_TRY_AGAIN:
	//	case ResultCode::E_NET_WOULDBLOCK:
	//		break;
	//	case ResultCode::E_NET_CONNABORTED:
	//	case ResultCode::E_NET_CONNRESET:
	//	case ResultCode::E_NET_NETRESET:
	//	case ResultCode::E_NET_NOTCONN:
	//	case ResultCode::E_NET_NOTSOCK:
	//	case ResultCode::E_NET_SHUTDOWN:
	//		// Send fail by connection close
	//		// Need to disconnect
	//		pTCPCon->Disconnect();
	//		hr = ResultCode::E_NET_CONNECTION_CLOSED;
	//		goto Proc_End;
	//		break;
	//	default:
	//		netErr(ResultCode::E_NET_IO_SEND_FAIL);
	//		break;
	//	};

	//Proc_End:

	//	if( !(hr) )
	//	{
	//		if( pOverlapped )
	//		{
	//			Net::NetSystem::FreeBuffer(pOverlapped);
	//		}

	//		if( hr != ResultCode::E_NET_IO_SEND_FAIL )
	//		{
	//			netTrace( Trace::TRC_ERROR, "TCP Send Failed, ip:{0}, err:{1:X8}, hr:{2:X8}", pTCPCon->GetConnectionInfo().Remote, hrErr, hr );
	//		}
	//		else
	//			return ResultCode::SUCCESS;
	//	}
	//	else
	//	{
	//		netTrace(TRC_TCPSENDRAW, "TCP Send ip:{0}, Len:{1}", pTCPCon->GetConnectionInfo().Remote, uiBuffSize);
	//	}


	//	return hr;
	//}


	//// Send message to connection with network device to dst addr
	//Result ServerTCP::SendMsg( IConnection *pConnection, const sockaddr_in6& dstAddr, Message::MessageData *pMsg )
	//{
	//	return ResultCode::NOT_IMPLEMENTED;
	//}




	ServerPublicTCP::ServerPublicTCP(ServerID InServerID, NetClass localClass)
		:ServerTCP(InServerID, localClass),
		m_ConnectionManager(Const::SVR_PUBLIC_CONNECTION_BUCKET_SIZE)
	{
		GetConnectionManager().SetNetOwner( this );
	}

	ServerPublicTCP::~ServerPublicTCP()
	{
		//Util::SafeDelete( m_pAcceptBuffer );
	}
	


} // namespace Net
} // namespace BR

