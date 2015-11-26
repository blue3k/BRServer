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



namespace BR {
namespace Net {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	TCP Server class
	//

	ServerTCP::ServerTCP(ServerID InServerID, NetClass localClass)
		:Server(InServerID, localClass),
		m_pAcceptBuffer(nullptr)
	{
	}

	ServerTCP::~ServerTCP()
	{
		Util::SafeDelete( m_pAcceptBuffer );
	}
	
	HRESULT ServerTCP::SetupSocketOption(SOCKET socket)
	{
		HRESULT hr = S_OK;
		int iOptValue = 0;

		iOptValue = BR::Net::Const::SVR_RECV_BUFFER_SIZE;
		if (setsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_RCVBUF = %0%, err = %1%", iOptValue, WSAGetLastError());
			netErr(E_UNEXPECTED);
		}

		iOptValue = BR::Net::Const::SVR_SEND_BUFFER_SIZE;
		if (setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_SNDBUF = %0%, err = %1%", iOptValue, WSAGetLastError());
			netErr(E_UNEXPECTED);
		}

	Proc_End:

		return hr;
	}

	// On New connection accepted
	HRESULT ServerTCP::OnIOAccept( HRESULT hrRes, OVERLAPPED_BUFFER_ACCEPT *pOverAccept )
	{
		HRESULT hr = S_OK;
		int iLenLocalAddr = 0, iLenRemoteAddr = 0;
		sockaddr_in6 *pLocalAddr = nullptr, *pRemoteAddr = nullptr;
		SOCKET sockSvr = GetSocket();
		SOCKET sockAccept = pOverAccept->sockAccept;
		BR::Net::IConnection::ConnectionInformation connectionInfo;
		UINT cid = 0;

		netChkPtr( pOverAccept );

		memset( &connectionInfo, 0, sizeof(connectionInfo) );

		// skip if not accept mode
		if( !GetIsEnableAccept() )
		{
			goto Proc_End;
		}


		if( SUCCEEDED( hrRes ) )
		{
			GetAcceptExSockaddrs( (void*)pOverAccept->pAcceptInfo,
								  0,
								  sizeof(sockaddr_in6)+16,
								  sizeof(sockaddr_in6)+16, 
								  (SOCKADDR**)&pLocalAddr, 
								  &iLenLocalAddr, 
								  (SOCKADDR**)&pRemoteAddr, 
								  &iLenRemoteAddr );


			connectionInfo.Local = GetLocalAddress();
			connectionInfo.LocalClass = GetLocalClass();
			connectionInfo.LocalID = GetServerID();
			SockAddr2Addr( *pRemoteAddr, connectionInfo.Remote );

			if( setsockopt(sockAccept, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char *)&sockSvr, sizeof(SOCKET)) == SOCKET_ERROR )
			{
				netTrace( TRC_CONNECTION, "Failed set socket option SO_UPDATE_ACCEPT_CONTEXT err:%0%", WSAGetLastError() );
				netErr( E_FAIL );
			}
			IConnection *pConnOut = nullptr;
			svrChk(OnNewSocket(sockAccept, *pRemoteAddr, connectionInfo, pConnOut));

			sockAccept = INVALID_SOCKET;
		}
		else
		{
			hr = hrRes;
		}


	Proc_End:


		if( m_pAcceptBuffer && pOverAccept )
			m_pAcceptBuffer->FreeBuffer( pOverAccept );

		if( sockAccept != INVALID_SOCKET )
		{
			closesocket( sockAccept );
		}

		PendingAccept();

		return hr;
	}

	// handle Socket accept
	HRESULT ServerTCP::OnNewSocket( SOCKET acceptedSocket, const sockaddr_in6& remoteSockAddr, const IConnection::ConnectionInformation& connectionInfo, IConnection* &pConnOut )
	{
		HRESULT hr = S_OK;
		ConnectionTCP *pConnection = nullptr;
		Connection *pConn = nullptr;
		uintptr_t cid = 0;

		remoteSockAddr;
		
		// Create New connection for accept
		pConnection = (ConnectionTCP*)GetConnectionManager().NewConnection();
		if( pConnection == nullptr )// Maybe maxconnection ?
		{
			netTrace( Trace::TRC_ERROR, "Failed to allocated new connection now active:%0%", GetConnectionManager().GetNumActiveConnection() );
			netErr( E_FAIL );
		}

		pConn = pConnection;
		pConnOut = pConnection;
		cid = pConnection->GetCID();

		// Connect to IOCP
		if( !CreateIoCompletionPort((HANDLE)acceptedSocket, IOCPSystem::GetSystem().GetIOCP(), (ULONG_PTR)(IOCPSystem::IOCallBack*)pConnection, 0) )
		{
			netTrace( TRC_CONNECTION, "Failed bind IOCP to TCP socket err:%0%", WSAGetLastError() );
			netErr( E_FAIL );
		}

		pConnection->SetConnectingTimeOut( Const::CONNECTION_TIMEOUT );

		// Initialize connection
		netChk( pConnection->InitConnection( acceptedSocket, connectionInfo ) );
		netTrace(TRC_CONNECTION, "Connection Accepted CID:%0%, Addr:%1%:%2%", pConn->GetCID(), pConn->GetConnectionInfo().Remote.strAddr, pConn->GetConnectionInfo().Remote.usPort);

		//netChk( GetConnectionManager().PendingWaitConnection( pConnection ) );
		pConn = nullptr;

		// When connection process is done it will be added
		//EnqueueNetEvent( INet::Event( INet::Event::EVT_NEW_CONNECTION ) ); 

			
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
				netTrace( TRC_NET, "Net Con Accept Svr:%0%, CID:%1%, From %2%", GetLocalAddress().usPort, cid, connectionInfo.Remote );
		}

		return hr;

	}

	// called when reciving message
	HRESULT ServerTCP::OnIORecvCompleted( HRESULT hrRes, OVERLAPPED_BUFFER_READ *pIOBuffer, DWORD dwTransferred )
	{
		return E_NOTIMPL;
	}

	// called when send completed
	HRESULT ServerTCP::OnIOSendCompleted( HRESULT hrRes, OVERLAPPED_BUFFER_WRITE *pIOBuffer, DWORD dwTransferred )
	{
		Util::SafeDeleteArray( pIOBuffer->pSendBuff );
		Util::SafeRelease( pIOBuffer->pMsgs );
		WSASystem::FreeBuffer( pIOBuffer );
		return S_OK;
	}


	// Pending Accept New one
	HRESULT ServerTCP::PendingAccept()
	{
		HRESULT hr = S_OK;
		OVERLAPPED_BUFFER_ACCEPT *pOverlapped = nullptr;
		DWORD dwNumBytes = 0;
		int iOptValue = 0;
		SOCKET sockAccept = 0;

		// skip if not accept mode
		if( !GetIsEnableAccept() )
			goto Proc_End;

		// if server not initialized
		if( GetSocket() == INVALID_SOCKET )
			goto Proc_End;

		while( m_pAcceptBuffer->GetUsedBufferCount() < m_pAcceptBuffer->GetBufferCount() )
		{
			if( sockAccept == INVALID_SOCKET )
			{
				netTrace(Trace::TRC_ERROR, "Failed to Open Accept Socket %0%", WSAGetLastError());
				netErr( E_UNEXPECTED );
			}

			netTrace( TRC_CONNECTION, "Pending Accept : %0%", GetConnectionManager().GetNumActiveConnection() );

			if( FAILED(m_pAcceptBuffer->TryAllocBuffer( Const::TCP_ACCEPT_TRYALLOC_MAX, pOverlapped )) )
			{
				// all accept operation pending done
				break;
			}

			memset( pOverlapped, 0, sizeof(OVERLAPPED_BUFFER_ACCEPT) );

			sockAccept = WSASocket(GetSocketAddr().sin6_family, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);

			pOverlapped->SetupAccept( sockAccept );

			netChk( SetupSocketOption(pOverlapped->sockAccept) );

			if( !AcceptEx( GetSocket(), pOverlapped->sockAccept, 
					pOverlapped->pAcceptInfo, 0, 
					sizeof(sockaddr_in6)+16, sizeof(sockaddr_in6)+16, 
					&dwNumBytes, pOverlapped ) )
			{
				int iErr = WSAGetLastError();
				switch( iErr )
				{
				case WSAENOTSOCK:// server closing or not initialized
					netTrace( Trace::TRC_ERROR, "TCP Abnormal accept, Not socked %0%", iErr );
					netErr( HRESULT_FROM_WIN32(iErr) );
					break;
				case ERROR_IO_PENDING:
					netTrace( TRC_NET, "TCP accept pending %0% queued", m_pAcceptBuffer->GetUsedBufferCount() );
					break;
				default:
					netTrace( Trace::TRC_ERROR, "TCP Abnormal accept, err:%0%", iErr );
					netErr( HRESULT_FROM_WIN32(iErr) );
					break;
				};
			}
			else
			{
				netTrace( Trace::TRC_ERROR, "TCP Abnormal accept" );
				netErr( E_UNEXPECTED );
			}


		}// while

	Proc_End:

		if( FAILED(hr) )
		{
			if( pOverlapped && m_pAcceptBuffer )
				m_pAcceptBuffer->FreeBuffer( pOverlapped );
		}

		return hr;
	}


	// Enable/Disable new connection acception
	void ServerTCP::SetIsEnableAccept( bool bIsEnable )
	{
		__super::SetIsEnableAccept( bIsEnable );

		if( IsReady() )
			PendingAccept();
	}

	// Open host and start listen
	HRESULT ServerTCP::HostOpen( NetClass netCls, const char *strLocalIP, USHORT usLocalPort )
	{
		HRESULT hr = S_OK;
		SOCKET socket = INVALID_SOCKET;
		INT32 iOptValue;
		BOOL bOptValue;
		sockaddr_in6 bindAddr;

		if( GetSocket() != INVALID_SOCKET )// already initialized?
			return S_OK;


		netChk( __super::HostOpen( netCls, strLocalIP, usLocalPort ) );


		netTrace(Trace::TRC_TRACE, "Open Server TCP Host %0%:%1%", strLocalIP, usLocalPort );

		socket = WSASocket(GetSocketAddr().sin6_family, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
		if( socket == INVALID_SOCKET )
		{
			netTrace(Trace::TRC_ERROR, "Failed to Open Server Socket %0%", WSAGetLastError());
			netErr( E_UNEXPECTED );
		}


		iOptValue = Const::SVR_RECV_BUFFER_SIZE;
		if( setsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_RCVBUF = %0%, err = %1%", iOptValue, WSAGetLastError() );
			netErr( E_UNEXPECTED );
		}

		iOptValue = Const::SVR_SEND_BUFFER_SIZE;
		if( setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_SNDBUF = %0%, err = %1%", iOptValue, WSAGetLastError() );
			netErr( E_UNEXPECTED );
		}

		bOptValue = TRUE;
		if( setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&bOptValue, sizeof(bOptValue)) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_REUSEADDR = %0%, err = %1%", bOptValue, WSAGetLastError() );
			netErr( E_UNEXPECTED );
		}

		bindAddr = GetSocketAddr();
		if (GetSocketAddr().sin6_family == AF_INET6)
		{
			iOptValue = FALSE;
			if (setsockopt(socket, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
			{
				netTrace(Trace::TRC_ERROR, "Failed to change socket option IPV6_V6ONLY = %0%, err = %1%", iOptValue, WSAGetLastError());
				netErr( E_UNEXPECTED );
			}

			bindAddr.sin6_family = AF_INET6;
			bindAddr.sin6_addr = in6addr_any;
		}
		if (bind(socket, (SOCKADDR*)&bindAddr, sizeof(bindAddr)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Socket bind failed, TCP %0%", WSAGetLastError() );
			netErr( E_UNEXPECTED );
		}

		if( listen(socket, SOMAXCONN) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to listen socket %0%", WSAGetLastError() );
			netErr( E_UNEXPECTED );
		}

		if( !CreateIoCompletionPort((HANDLE)socket, IOCPSystem::GetSystem().GetIOCP(), (ULONG_PTR)(IOCPSystem::IOCallBack*)this, 0) )
		{
			netTrace(Trace::TRC_ERROR, "CreateIoCompletionPort Failed TCP, hr = {0:X8}", GetLastHRESULT() );
			netErr( E_UNEXPECTED );
		}


		// allocate accept buffer
		Util::SafeDelete( m_pAcceptBuffer );


		int iMaxAccept = Const::TCP_ACCEPT_PENDING_MAX;
		iMaxAccept = Util::Max( iMaxAccept, 1 );
		netMem( m_pAcceptBuffer = new BR::SpinSharedBuffer<OVERLAPPED_BUFFER_ACCEPT>( iMaxAccept ) );


		SetSocket( socket );
		socket = INVALID_SOCKET;


		// Prepare connection
		netChk( GetConnectionManager().InitManager( Const::SVR_PUBLIC_CONNECTION_POOLCACHE ) );



		EnqueueNetEvent( INet::Event( INet::Event::EVT_NET_INITIALIZED ) ); 


		netChk( PendingAccept() );

	Proc_End:

		if( socket != INVALID_SOCKET )
			closesocket( socket );

		netTrace( TRC_NET, "HostOpen %0%, hr={1:X8}", GetLocalAddress(), hr );

		return hr;
	}

	// Close host and close all connections
	HRESULT ServerTCP::HostClose()
	{
		HRESULT hr = S_OK;

		netChk( __super::HostClose() );

		if( GetSocket() != INVALID_SOCKET )
		{
			closesocket( GetSocket() );
			SetSocket( INVALID_SOCKET );
		}

		netTrace( Trace::TRC_ERROR, "TCP Server closed completely" );

		EnqueueNetEvent( INet::Event( INet::Event::EVT_NET_CLOSED ) ); 

	Proc_End:

		netTrace( TRC_NET, "HostClose %0%, hr={1:X8}", GetLocalAddress(), hr );

		return hr;
	}


	// Release Connection, Make connection to send free state
	HRESULT ServerTCP::ReleaseConnection( IConnection* pIConnection )
	{
		HRESULT hr = S_OK;
		
		netChk( __super::ReleaseConnection( pIConnection ) );

		netChk( PendingAccept() );

	Proc_End:


		return hr;
	}


	// Send message to connection with network device
	HRESULT ServerTCP::SendMsg( IConnection *pConnection, Message::MessageData *pMsg )
	{
		HRESULT hr = S_OK;
		//DWORD dwSend = 0;
		int iWSAErr = 0;

		Message::MessageID msgID = pMsg->GetMessageHeader()->msgID;
		UINT uiMsgLen = pMsg->GetMessageHeader()->Length;

		ConnectionTCP *pTCPCon = (ConnectionTCP*)pConnection;

		OVERLAPPED_BUFFER_WRITE *pOverlapped = nullptr;
		netChk( BR::Net::WSASystem::AllocBuffer(pOverlapped) );

		Assert(pMsg->GetRefCount() > 0);
		pOverlapped->SetupSendTCP( pMsg );


		iWSAErr = WSASend( pTCPCon->GetSocket(), &pOverlapped->wsaBuff, 1, nullptr, 0, pOverlapped, nullptr );
		if( iWSAErr == SOCKET_ERROR )
		{
			iWSAErr = WSAGetLastError();
			if( iWSAErr != WSA_IO_PENDING )
			{
				switch( iWSAErr )
				{
				case WSAENOTCONN:
					// Send fail by connection close
					// Need to disconnect
					pTCPCon->Disconnect();
					netErrSilent(E_NET_NOT_CONNECTED);
					break;
				case WSAECONNABORTED:
				case WSAECONNRESET:
				case WSAENETRESET:
				case WSAENOTSOCK:
				case WSAESHUTDOWN:
					// Send fail by connection close
					// Need to disconnect
					pTCPCon->Disconnect();
					netErrSilent( E_NET_CONNECTION_CLOSED );
					break;
				default:
					netErr( E_NET_IO_SEND_FAIL );
					break;
				};
			}
		}
		else
		{
			// IOCP process will free this
		}

	Proc_End:

		if( FAILED(hr) )
		{
			if( pOverlapped )
			{
				Util::SafeRelease( pOverlapped->pMsgs );
				BR::Net::WSASystem::FreeBuffer(pOverlapped);
			}
			else
			{
				Util::SafeRelease( pMsg );
			}

			if( hr != E_NET_IO_SEND_FAIL )
			{
				netTrace(Trace::TRC_ERROR, "TCP Send Failed, CID:%3%, ip:%0%, err:%1%, hr:{2:X8}", pTCPCon->GetConnectionInfo().Remote, iWSAErr, hr, pTCPCon->GetCID());
			}
			else
				return S_OK;
		}
		else
		{
			if( msgID.IDs.Type == Message::MSGTYPE_NETCONTROL )
			{
				netTrace(TRC_TCPNETCTRL, "TCP Ctrl CID:%3%, ip:%0%, msg:%1%, Len:%2%", pTCPCon->GetConnectionInfo().Remote, msgID, uiMsgLen, pTCPCon->GetCID());
			}
			else
			{
				netTrace(TRC_TCPSENDRAW, "TCP Send CID:%3%, ip:%0%, msg:%1%, Len:%2%", pTCPCon->GetConnectionInfo().Remote, msgID, uiMsgLen, pTCPCon->GetCID());
			}
		}


		return hr;
	}

	HRESULT ServerTCP::SendMsg( IConnection *pConnection, UINT uiBuffSize, BYTE* pBuff )
	{
		HRESULT hr = S_OK;
		//DWORD dwSend = 0;
		int iWSAErr = 0;

		ConnectionTCP *pTCPCon = (ConnectionTCP*)pConnection;

		OVERLAPPED_BUFFER_WRITE *pOverlapped = nullptr;
		netChk( BR::Net::WSASystem::AllocBuffer(pOverlapped) );


		pOverlapped->SetupSendTCP( uiBuffSize, pBuff );

		iWSAErr = WSASend( pTCPCon->GetSocket(), &pOverlapped->wsaBuff, 1, nullptr, 0, pOverlapped, nullptr );
		if( iWSAErr == SOCKET_ERROR )
		{
			iWSAErr = WSAGetLastError();
			if( iWSAErr != WSA_IO_PENDING )
			{
				switch( iWSAErr )
				{
				case WSAECONNABORTED:
				case WSAECONNRESET:
				case WSAENETRESET:
				case WSAENOTCONN:
				case WSAENOTSOCK:
				case WSAESHUTDOWN:
					// Send fail by connection close
					// Need to disconnect
					pTCPCon->Disconnect();
					netErrSilent( E_NET_CONNECTION_CLOSED );
					break;
				default:
					netErr( E_NET_IO_SEND_FAIL );
					break;
				};
			}
		}
		else
		{
			// IOCP process will free this
		}

	Proc_End:

		if( FAILED(hr) )
		{
			if( pOverlapped )
			{
				BR::Net::WSASystem::FreeBuffer(pOverlapped);
			}

			if( hr != E_NET_IO_SEND_FAIL )
			{
				netTrace( Trace::TRC_ERROR, "TCP Send Failed, ip:%0%, err:%1%, hr:{2:X8}", pTCPCon->GetConnectionInfo().Remote, iWSAErr, hr );
			}
			else
				return S_OK;
		}
		else
		{
			netTrace(TRC_TCPSENDRAW, "TCP Send ip:%0%, Len:%1%", pTCPCon->GetConnectionInfo().Remote, uiBuffSize);
		}


		return hr;
	}


	// Send message to connection with network device to dst addr
	HRESULT ServerTCP::SendMsg( IConnection *pConnection, const sockaddr_in6& dstAddr, Message::MessageData *pMsg )
	{
		return E_NOTIMPL;
	}




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

