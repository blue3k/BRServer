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
	
	HRESULT ServerTCP::SetupSocketOption(SOCKET socket)
	{
		HRESULT hr = S_SYSTEM_OK;
		socklen_t iOptValue = 0;

		iOptValue = Net::Const::SVR_RECV_BUFFER_SIZE;
		if (setsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_RCVBUF = {0}, err = {1:X8}", iOptValue, GetLastWSAHRESULT());
			netErr(E_SYSTEM_UNEXPECTED);
		}

		iOptValue = Net::Const::SVR_SEND_BUFFER_SIZE;
		if (setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_SNDBUF = {0}, err = {1:X8}", iOptValue, GetLastWSAHRESULT());
			netErr(E_SYSTEM_UNEXPECTED);
		}

	Proc_End:

		return hr;
	}

	HRESULT ServerTCP::Accept(IOBUFFER_ACCEPT* &pAcceptInfo)
	{
		HRESULT hr = S_SYSTEM_OK, hrErr = S_SYSTEM_OK;
		SOCKET sockAccept = INVALID_SOCKET;

		pAcceptInfo = new IOBUFFER_ACCEPT;
		netMem(pAcceptInfo);

		memset(pAcceptInfo, 0, sizeof(IOBUFFER_ACCEPT));

		sockAccept = NetSystem::Socket(GetLocalAddress().SocketFamily, SockType::Stream);
		if (sockAccept == INVALID_SOCKET)
		{
			netTrace(Trace::TRC_ERROR, "Failed to Open Accept Socket {0:X8}", GetLastWSAHRESULT());
			netErr(E_SYSTEM_UNEXPECTED);
		}

		pAcceptInfo->SetupAccept(sockAccept);

		netChk(SetupSocketOption(pAcceptInfo->sockAccept));

		hrErr = NetSystem::Accept(GetSocket(), pAcceptInfo);
		switch (hrErr)
		{
		case S_SYSTEM_OK:
		case E_NET_WOULDBLOCK:
		case E_NET_IO_PENDING:
			// successed
			break;
		case E_NET_TRY_AGAIN:
			netTrace(TRC_NET, "TCP accept busy, try again {0} accepts are queued", m_PendingAccept.load(std::memory_order_relaxed));
		default:
			hr = hrErr;
			break;
		}

	Proc_End:

		if (FAILED(hr))
		{
			Util::SafeDelete(pAcceptInfo);
		}

		return hr;
	}

	// On New connection accepted
	HRESULT ServerTCP::OnIOAccept( HRESULT hrRes, IOBUFFER_ACCEPT *pOverAccept )
	{
		HRESULT hr = S_SYSTEM_OK;
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

		if( SUCCEEDED( hrRes ) )
		{
			netChk(NetSystem::HandleAcceptedSocket(sockSvr, pOverAccept, remoteAddr));

			connectionInfo.Local = GetLocalAddress();
			connectionInfo.LocalClass = GetLocalClass();
			connectionInfo.LocalID = GetServerID();
			SockAddr2Addr(remoteAddr, connectionInfo.Remote );

			IConnection *pConnOut = nullptr;
			svrChk(OnNewSocket(sockAccept, remoteAddr, connectionInfo, pConnOut));

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
	HRESULT ServerTCP::OnNewSocket( SOCKET acceptedSocket, const sockaddr_storage& remoteSockAddr, const IConnection::ConnectionInformation& connectionInfo, IConnection* &pConnOut )
	{
		HRESULT hr = S_SYSTEM_OK;
		ConnectionTCP *pConnection = nullptr;
		Connection *pConn = nullptr;
		uintptr_t cid = 0;

		unused(remoteSockAddr);
		
		// Create New connection for accept
		pConnection = (ConnectionTCP*)GetConnectionManager().NewConnection();
		if( pConnection == nullptr )// Maybe maxconnection ?
		{
			netTrace( Trace::TRC_ERROR, "Failed to allocated new connection now active:{0}", GetConnectionManager().GetNumActiveConnection() );
			netErr( E_SYSTEM_FAIL );
		}

		Assert(pConnection->GetWriteQueue() != nullptr);

		pConn = pConnection;
		pConnOut = pConnection;
		cid = pConnection->GetCID();

		pConnection->SetConnectingTimeOut( Const::CONNECTION_TIMEOUT );

		// Initialize connection
		netChk( pConnection->InitConnection( acceptedSocket, connectionInfo ) );
		netTrace(TRC_CONNECTION, "Connection Accepted CID:{0}, Addr:{1}:{2}", pConn->GetCID(), pConn->GetConnectionInfo().Remote.strAddr, pConn->GetConnectionInfo().Remote.usPort);


		netChk(NetSystem::RegisterSocket(SockType::Stream, pConnection));

		pConn = nullptr;

			
	Proc_End:

		if( FAILED(hr) )
		{
			if( pConn != nullptr )
			{
				GetConnectionManager().PendingReleaseConnection(pConn);
			}

			netTrace( Trace::TRC_ERROR, "Tcp Accept failed  hr = {0:X8}", hr );
		}
		else
		{
			if( pConn )
				netTrace( TRC_NET, "Net Con Accept Svr:{0}, CID:{1}, From {2}", GetLocalAddress().usPort, cid, connectionInfo.Remote );
		}

		return hr;

	}

	HRESULT ServerTCP::Recv(IOBUFFER_READ* pIOBuffer)
	{
		return E_SYSTEM_NOTIMPL;
	}

	// called when reciving message
	HRESULT ServerTCP::OnIORecvCompleted( HRESULT hrRes, IOBUFFER_READ* &pIOBuffer )
	{
		Assert(false);
		return E_SYSTEM_NOTIMPL;
	}

	HRESULT ServerTCP::OnSendReady()
	{
		return ProcessSendQueue();
	}

	HRESULT ServerTCP::SendBuffer(IOBUFFER_WRITE *pIOBuffer)
	{
		return E_SYSTEM_NOTIMPL;
	}

	// called when send completed
	HRESULT ServerTCP::OnIOSendCompleted( HRESULT hrRes, IOBUFFER_WRITE *pIOBuffer )
	{
		Util::SafeDeleteArray( pIOBuffer->pSendBuff );
		Util::SafeRelease( pIOBuffer->pMsgs );
		NetSystem::FreeBuffer( pIOBuffer );
		return S_SYSTEM_OK;
	}


	// Pending Accept New one
	HRESULT ServerTCP::PendingAccept()
	{
		HRESULT hr = S_SYSTEM_OK;
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
	HRESULT ServerTCP::HostOpen( NetClass netCls, const char *strLocalIP, USHORT usLocalPort )
	{
		HRESULT hr = S_SYSTEM_OK;
		SOCKET socket = INVALID_SOCKET;
		INT32 iOptValue;
		int bOptValue;
		sockaddr_storage bindAddr;
		INet::Event netEvent(INet::Event::EVT_NET_INITIALIZED);

		if( GetSocket() != INVALID_SOCKET )// already initialized?
			return S_SYSTEM_OK;

		netChk(Server::HostOpen( netCls, strLocalIP, usLocalPort ) );


		netTrace(Trace::TRC_TRACE, "Open Server TCP Host {0}:{1}", strLocalIP, usLocalPort );

		socket = NetSystem::Socket(GetLocalAddress().SocketFamily, SockType::Stream);
		if( socket == INVALID_SOCKET )
		{
			netTrace(Trace::TRC_ERROR, "Failed to Open Server Socket {0:X8}", GetLastWSAHRESULT());
			netErr( E_SYSTEM_UNEXPECTED );
		}


		iOptValue = Const::SVR_RECV_BUFFER_SIZE;
		if( setsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_RCVBUF = {0}, err = {1:X8}", iOptValue, GetLastWSAHRESULT() );
			netErr( E_SYSTEM_UNEXPECTED );
		}

		iOptValue = Const::SVR_SEND_BUFFER_SIZE;
		if( setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_SNDBUF = {0}, err = {1:X8}", iOptValue, GetLastWSAHRESULT() );
			netErr( E_SYSTEM_UNEXPECTED );
		}

		bOptValue = TRUE;
		if( setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&bOptValue, sizeof(bOptValue)) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_REUSEADDR = {0}, err = {1:X8}", bOptValue, GetLastWSAHRESULT() );
			netErr( E_SYSTEM_UNEXPECTED );
		}

		if (GetSocketAddr().ss_family== AF_INET6)
		{
			iOptValue = FALSE;
			if (setsockopt(socket, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
			{
				netTrace(Trace::TRC_ERROR, "Failed to change socket option IPV6_V6ONLY = {0}, err = {1:X8}", iOptValue, GetLastWSAHRESULT());
				netErr( E_SYSTEM_UNEXPECTED );
			}
		}

		GetAnyBindAddr(GetSocketAddr(), bindAddr);
		if (bind(socket, (sockaddr*)&bindAddr, GetSocketAddrSize()) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Socket bind failed, TCP {0:X8}", GetLastWSAHRESULT() );
			netErr( E_SYSTEM_UNEXPECTED );
		}

		if( listen(socket, SOMAXCONN) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to listen socket {0:X8}", GetLastWSAHRESULT() );
			netErr( E_SYSTEM_UNEXPECTED );
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
	HRESULT ServerTCP::HostClose()
	{
		HRESULT hr = S_SYSTEM_OK;
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
	HRESULT ServerTCP::ReleaseConnection( IConnection* pIConnection )
	{
		HRESULT hr = S_SYSTEM_OK;
		
		netChk(Server::ReleaseConnection( pIConnection ) );

		netChk( PendingAccept() );

	Proc_End:


		return hr;
	}


	//// Send message to connection with network device
	//HRESULT ServerTCP::SendMsg( IConnection *pConnection, Message::MessageData *pMsg )
	//{
	//	HRESULT hr = S_SYSTEM_OK, hrErr = S_SYSTEM_OK;

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
	//	case S_SYSTEM_OK:
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
	//		pTCPCon->Disconnect();
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
	//			netTrace(Trace::TRC_ERROR, "TCP Send Failed, CID:{3}, ip:{0}, err:{1:X8}, hr:{2:X8}", pTCPCon->GetConnectionInfo().Remote, hrErr, hr, pTCPCon->GetCID());
	//		}
	//		else
	//			return S_SYSTEM_OK;
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

	//HRESULT ServerTCP::SendMsg( IConnection *pConnection, UINT uiBuffSize, BYTE* pBuff )
	//{
	//	HRESULT hr = S_SYSTEM_OK, hrErr = S_SYSTEM_OK;

	//	ConnectionTCP *pTCPCon = (ConnectionTCP*)pConnection;

	//	IOBUFFER_WRITE *pOverlapped = nullptr;
	//	netChk( Net::NetSystem::AllocBuffer(pOverlapped) );

	//	pOverlapped->SetupSendTCP( uiBuffSize, pBuff );

	//	hrErr = NetSystem::Send(pTCPCon->GetSocket(), pOverlapped);
	//	switch (hrErr)
	//	{
	//	case S_SYSTEM_OK:
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
	//		pTCPCon->Disconnect();
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
	//			Net::NetSystem::FreeBuffer(pOverlapped);
	//		}

	//		if( hr != E_NET_IO_SEND_FAIL )
	//		{
	//			netTrace( Trace::TRC_ERROR, "TCP Send Failed, ip:{0}, err:{1:X8}, hr:{2:X8}", pTCPCon->GetConnectionInfo().Remote, hrErr, hr );
	//		}
	//		else
	//			return S_SYSTEM_OK;
	//	}
	//	else
	//	{
	//		netTrace(TRC_TCPSENDRAW, "TCP Send ip:{0}, Len:{1}", pTCPCon->GetConnectionInfo().Remote, uiBuffSize);
	//	}


	//	return hr;
	//}


	//// Send message to connection with network device to dst addr
	//HRESULT ServerTCP::SendMsg( IConnection *pConnection, const sockaddr_in6& dstAddr, Message::MessageData *pMsg )
	//{
	//	return E_SYSTEM_NOTIMPL;
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

