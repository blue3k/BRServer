////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves.
// 
// Author : KyungKun Ko
//
// Description : Net Client
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Common/HRESNet.h"
#include "Net/NetConst.h"
#include "Net/NetSystem.h"
#include "Common/Thread.h"
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Net/Connection.h"
#include "Net/NetTrace.h"
#include "Net/NetClient.h"
#include "Protocol/ProtocolVer.h"
#include "Net/NetConnectionTask.h"



namespace BR {
namespace Net {


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Client network io process
	//

	namespace CliSystem
	{
		//////////////////////////////////////////////////////////////////////
		// client connection ID gen
		BR::SyncCounter g_lCIDGen = 1;

		// system opened count
		BR::SyncCounter g_lSysOpenCount;
		
		// system opened count
		ConnectionTaskManager *g_ConnectionTaskManager = nullptr;



		CounterType GenNewCID()
		{
			CounterType lCID = g_lCIDGen.fetch_add(1, std::memory_order_relaxed) + 1;
			if( lCID == 0 )
				lCID = g_lCIDGen.fetch_add(1, std::memory_order_relaxed) + 1;
			return lCID;
		}


		// Open network system
		HRESULT OpenSystem( UINT uiOverBufferCount )
		{
			HRESULT hr = S_OK;


			netChk( WSASystem::OpenSystem( uiOverBufferCount ) );


			if (g_lSysOpenCount.fetch_add(1, std::memory_order_relaxed) == 0)
			{
				netTrace( TRC_NET, "Opening client net system" );
				netChk( IOCPSystem::GetSystem().InitIOCP( Const::CLI_NUM_RECV_THREAD ) );

				g_ConnectionTaskManager = new ConnectionTaskManager;

				netChk( g_ConnectionTaskManager->InitializeManager( Const::CLI_CONNECTION_TASK_THREAD ) );
			}


		Proc_End:

			return hr;
		}


		// Close network system
		void CloseSystem()
		{
			HRESULT hr = S_OK;
			int iErr = 0;
			CounterType lCount = 0;

			if( g_lSysOpenCount > 0 )
			{
				lCount = g_lSysOpenCount.fetch_sub(1, std::memory_order_relaxed) - 1;
			}

			if( lCount <= 0 )
			{
				netTrace( TRC_NET, "Closing client net system" );

				netChk( IOCPSystem::GetSystem().CloseIOCP() );

				if( g_ConnectionTaskManager )
					netChk( g_ConnectionTaskManager->TerminateManager() );

				Util::SafeDelete( g_ConnectionTaskManager );

				g_lSysOpenCount = 0;
			}

			WSASystem::CloseSystem();

		Proc_End:
			;
		}


		// Add connection update task
		HRESULT AddConnectionTask( ConnectionManager *pConMgr, Connection * pConnection )
		{
			HRESULT hr = S_OK;
			ConnectionTask *pTask = nullptr;

			if( g_ConnectionTaskManager == nullptr )
				return E_NET_NOTINITIALIZED;

			netChkPtr( pConMgr );
			netChkPtr( pConnection );

			netChk( g_ConnectionTaskManager->CreateTask( pTask ) );

			pTask->SetConnection( pConMgr, pConnection );

			pConnection->SetUData( (UINT_PTR)pTask );

			netChk( g_ConnectionTaskManager->AddTickTask( pTask ) );

	Proc_End:

			return hr;
		}

		// Remove Connection update task
		HRESULT RemoveConnectionTask( Connection * pConnection )
		{
			HRESULT hr = S_OK;
			ConnectionTask *pTask = nullptr;

			if( g_ConnectionTaskManager == nullptr )
				return E_NET_NOTINITIALIZED;

			pTask = (ConnectionTask*)pConnection->GetUData();

			if( pTask )
				netChk( g_ConnectionTaskManager->RemoveTickTask( pTask ) );
			pTask = nullptr;

	Proc_End:

			Util::SafeDelete( pTask );

			return hr;
		}



	}; // namespace ClientIO


	

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Client base class
	//
	
	// Release all connection and terminate manager
	template< class ConnectionType >
	HRESULT ClientConnectionManagerT<ConnectionType>::TerminateManager()
	{
		HRESULT hr = S_OK;
		Operation oper;
		ConnectionTask *pTask = nullptr;

		m_bNoNewConnection = true;

		Stop(true);


		// Clear pending operation queue
		while( m_PendingOperations.Dequeue( oper ) == S_OK )
		{
			switch( oper.OpCode )
			{
			case Operation::OP_WAITING_CONNECTION:
			case Operation::OP_PENDING_CONNECTION:
			case Operation::OP_PENDING_INITCONNECTION:
			case Operation::OP_RELEASE_CONNECTION:
				if( oper.pConn != nullptr )
				{
					SharedPointerT<Connection>& pConn = oper.pConn;
					//oper.pConn.GetSharedPointer(pConn);
					if (pConn == nullptr)
						break;

					pConn->CloseConnection();
					pTask = (ConnectionTask*)pConn->GetUData();
					if (pTask)
					{
						pTask->SetConnection(nullptr, nullptr);
						CliSystem::RemoveConnectionTask((Connection*)pConn);
					}
					FreeConnection((Connection*)pConn);
				}
				break;
			case Operation::OP_PENDING_ADDR:
			default:
				break;
			};
		}

		// clear connection queue
		m_ManagedConnections.ForeachOrder(0, (UINT)m_ManagedConnections.GetItemCount(), [&](const UINT_PTR& key, SharedPointerT<Connection> pConn)->bool
		{
			pConn->CloseConnection();
			pTask = (ConnectionTask*)pConn->GetUData();
			if( pTask )
			{
				pTask->SetConnection( nullptr, nullptr );
				CliSystem::RemoveConnectionTask((Connection*)pConn);
			}
			FreeConnection((Connection*)pConn);
			return true;
		});

		m_ManagedConnections.ClearMap();


	Proc_End:


		return hr;
	}



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Client base class
	//


	Client::Client( UINT uiConMapBucketSize )
		:m_NetEventQueue(32)
	{
		//CliSystem::OpenSystem( SvrConst::Net::CLI_OVERBUFFER_COUNT );
	}

	Client::~Client()
	{
		//System::CloseSystem();
	}

	// Called when connection result 
	void Client::OnConnectionResult( Connection *pConnection, HRESULT hrConnect )
	{
	}

	// Release instance
	void Client::Release()
	{
		delete this;
	}


	// Query Network event
	HRESULT Client::DequeueNetEvent( Event& evt )
	{
		return m_NetEventQueue.Dequeue( evt );
	}




	////////////////////////////////////////////////////////////////////////////////
	//
	//	TCP Client class
	//

	ClientTCP::ClientTCP()
		:Client(Const::CLI_CONMAP_BUCKET_SIZE)
	{
		CliSystem::OpenSystem( Const::CLI_OVERBUFFER_COUNT );

		m_ConnectionManager.SetNetOwner( this );
		//ClientIO::g_ClientNetThread.AddRef();
	}

	ClientTCP::~ClientTCP()
	{
		m_ConnectionManager.TerminateManager();

		//ClientIO::g_ClientNetThread.Release();
		CliSystem::CloseSystem();
	}
	
	// check about initialize
	bool ClientTCP::IsReady()
	{
		return true;
	}


	// Connect to server
	HRESULT ClientTCP::Connect( const char *strServerIP, USHORT usServerPort, IConnection* &pNewConnection )
	{
		HRESULT hr = S_OK;
		SOCKET socket = INVALID_SOCKET;
		int iOptValue;
		sockaddr_in6 sockAddr, sockAddrDest;
		u_long iMode = true;// non block
		ConnectionTCPClient *pConn = nullptr;
		BR::Net::IConnection::ConnectionInformation connectionInfo;
		//hostent* localHost;
		//char* localIP;
		OVERLAPPED_BUFFER_READ *pIOBuffer= NULL;
		sockaddr_in6 bindAddr;
		NetAddress localAddress;

		Net::GetLocalAddressIPv4(localAddress);
		Addr2SockAddr(localAddress, sockAddr);

		// Get the local host information
		//localHost = gethostbyname("");
		//localHost = getaddrinfo();
		//localIP = inet_ntoa(*(struct in_addr *)*localHost->h_addr_list);

		//Addr2SockAddr(localAddress, sockAddr);// to make port generation

		socket = WSASocket(sockAddr.sin6_family, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
		if( socket == INVALID_SOCKET )
		{
			netTrace(Trace::TRC_ERROR, "Failed to Open Client Socket %0%", WSAGetLastError());
			netErr( E_UNEXPECTED );
		}


		iOptValue = Const::CLI_RECV_BUFFER_SIZE;
		if( setsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_RCVBUF = %0%, err = %1%", iOptValue, WSAGetLastError() );
			netErr( E_UNEXPECTED );
		}

		iOptValue = Const::CLI_SEND_BUFFER_SIZE;
		if( setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_SNDBUF = %0%, err = %1%", iOptValue, WSAGetLastError() );
			netErr( E_UNEXPECTED );
		}

		if (sockAddr.sin6_family == AF_INET6)
		{
			iOptValue = FALSE;
			if (setsockopt(socket, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
			{
				netTrace(Trace::TRC_ERROR, "Failed to change socket option IPV6_V6ONLY = %0%, err = %1%", iOptValue, WSAGetLastError());
				netErr(E_UNEXPECTED);
			}
		}

		iMode = true;
		if( ioctlsocket(socket, FIONBIO, &iMode) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket IO Mode to %d,  err = %0%", iMode, WSAGetLastError() );
			netErr( E_UNEXPECTED );
		}


		bindAddr = sockAddr;
		//bindAddr.sin6_addr = in6addr_any;
		if (bind(socket, (SOCKADDR*)&bindAddr, sizeof(bindAddr)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Socket bind failed, TCP %0%", WSAGetLastError() );
			netErr( E_UNEXPECTED );
		}


		netMem( pConn = dynamic_cast<ConnectionTCPClient*>(GetConnectionManager().NewConnection()) );
		//pConn->SetupNet( this, CliSystem::GenNewCID() );

		if( !CreateIoCompletionPort((HANDLE)socket, IOCPSystem::GetSystem().GetIOCP(), (ULONG_PTR)(IOCPSystem::IOCallBack*)pConn, 0) )
		{
			netTrace(Trace::TRC_ERROR, "CreateIoCompletionPort Failed TCP, hr = %0%", ArgHex32(GetLastHRESULT()) );
			netErr( E_UNEXPECTED );
		}


		SetSockAddr( sockAddrDest, strServerIP, usServerPort );


		if( connect(socket, (SOCKADDR*)&sockAddrDest, sizeof(SOCKADDR_IN)) != SOCKET_ERROR
			|| WSAGetLastError() != WSAEWOULDBLOCK )
		{
			netTrace(Trace::TRC_ERROR, "connect failed, TCP %0%", WSAGetLastError() );
			netErr( E_UNEXPECTED );
		}

		memset( &connectionInfo, 0, sizeof(connectionInfo) );
		SockAddr2Addr( sockAddr, connectionInfo.Local );

		netChk( StrUtil::StringCpy( connectionInfo.Remote.strAddr, strServerIP ) );
		connectionInfo.Remote.usPort = usServerPort;

		netChk( pConn->InitConnection( socket, connectionInfo ) );
		netTrace(TRC_CONNECTION, "Initialize connection CID:%0%, Addr:%1%:%2%", pConn->GetCID(), pConn->GetConnectionInfo().Remote.strAddr, pConn->GetConnectionInfo().Remote.usPort);
		socket = INVALID_SOCKET;

		netChk( GetConnectionManager().PendingWaitConnection( pConn ) );

		netChk( CliSystem::AddConnectionTask( &GetConnectionManager(), pConn ) );


		// Wait connection 
		//new BR::FunctorThread([&, ID](Thread* pThread)
		//{
		pConn->WaitConnect();
		//});

		pNewConnection = pConn;
		pConn = nullptr;


	Proc_End:

		if (pConn)
		{
			auto connTem = SharedPointerT<Connection>(pConn);
		}
		//if( pConn )
		//	pConn->Release();

		if( socket != INVALID_SOCKET )
			closesocket( socket );


		return hr;
	}
	
	// Release Connection
	HRESULT ClientTCP::ReleaseConnection( IConnection* pIConnection )
	{
		HRESULT hr = S_OK;
		ConnectionTCP *pConTCP = (ConnectionTCP*)pIConnection;

		if( pIConnection == NULL )
		{
			netErr( E_UNEXPECTED );
		}

		if (pIConnection->GetConnectionState() != BR::Net::IConnection::STATE_DISCONNECTED)
		{
			netChk( pIConnection->CloseConnection() );
		}

		netChk( CliSystem::RemoveConnectionTask( pConTCP ) );

		// Release will be done after task released
		//netChk( GetConnectionManager().PendingReleaseConnection( (Connection*)pIConnection ) );


	Proc_End:

		return hr;
	}

	// take over connection management
	HRESULT ClientTCP::TakeOverConnection(IConnection* pIConnection)
	{
		HRESULT hr = S_OK;
		Connection* pConn = (Connection*)pIConnection;

		netChkPtr(pIConnection);
		netErr(E_NOTIMPL);
		//netChk(GetConnectionManager().PendingManagedConnectionTakenOver(pConn));

	Proc_End:

		if (FAILED(hr))
		{
			netTrace(Trace::TRC_ERROR, "ReleaseConnection Failed hr=%0%", ArgHex32(hr));
		}

		return hr;
	}

	// Get connection from connection ID
	HRESULT ClientTCP::GetConnection(UINT_PTR uiCID, SharedPointerT<Connection> &pIConnection)
	{
		return m_ConnectionManager.GetConnectionByCID(uiCID, pIConnection);
	}

	// Send message to connection with network device
	HRESULT ClientTCP::SendMsg( IConnection *pConnection, Message::MessageData *pMsg )
	{
		HRESULT hr = S_OK;
		//DWORD dwSend = 0;
		int iWSAErr = 0;

		Message::MessageID msgID = pMsg->GetMessageHeader()->msgID;
		UINT uiMsgLen = pMsg->GetMessageHeader()->Length;

		ConnectionTCP *pTCPCon = (ConnectionTCP*)pConnection;

		OVERLAPPED_BUFFER_WRITE *pOverlapped = NULL;
		netChk( WSASystem::AllocBuffer(pOverlapped) );

		pOverlapped->SetupSendTCP( pMsg );

		iWSAErr = WSASend( pTCPCon->GetSocket(), &pOverlapped->wsaBuff, 1, nullptr, 0, pOverlapped, NULL );
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
					hr = E_NET_CONNECTION_CLOSED;
					goto Proc_End;
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
				WSASystem::FreeBuffer(pOverlapped);
			}
			else
			{
				Util::SafeRelease( pMsg );
			}

			if( hr != E_NET_IO_SEND_FAIL )
			{
				netTrace( TRC_TCPSENDRAW, "TCP Send Failed, ip:%0%, err:%1%, hr:%2%", pTCPCon->GetConnectionInfo().Remote, iWSAErr, ArgHex32(hr) );
			}
			else
				return S_OK;
		}
		else
		{
			if( msgID.IDs.Type == Message::MSGTYPE_NETCONTROL )
			{
				netTrace(TRC_TCPNETCTRL, "TCP Ctrl CID %3%, ip:%0%, msg:%1%, Len:%2%", pTCPCon->GetConnectionInfo().Remote, msgID, uiMsgLen, pTCPCon->GetCID());
			}
			else
			{
				netTrace(TRC_TCPSENDRAW, "TCP Send CID %3%, ip:%0%, msg:%1%, Len:%2%", pTCPCon->GetConnectionInfo().Remote, msgID, uiMsgLen, pTCPCon->GetCID());
			}
		}


		return hr;
	}

	// Send message to connection with network device to dst addr
	HRESULT ClientTCP::SendMsg( IConnection *pConnection, const sockaddr_in6& dstAddr, Message::MessageData *pMsg )
	{
		return E_NOTIMPL;
	}


	// Send message to connection with network device
	HRESULT ClientTCP::SendMsg( IConnection *pConnection, UINT uiBuffSize, BYTE* pBuff )
	{
		HRESULT hr = S_OK;
		int iWSAErr = 0;

		ConnectionTCP *pTCPCon = (ConnectionTCP*)pConnection;


		OVERLAPPED_BUFFER_WRITE *pOverlapped = NULL;
		netChk( WSASystem::AllocBuffer(pOverlapped) );


		pOverlapped->SetupSendTCP( uiBuffSize, pBuff );


		if( WSASend( pTCPCon->GetSocket(), &pOverlapped->wsaBuff, 1, nullptr, 0,
			pOverlapped, NULL ) == SOCKET_ERROR )
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
//				netErr( E_NET_IO_SEND_FAIL );
			}
		}


		pOverlapped = nullptr;



	Proc_End:

		if( FAILED(hr) )
		{
			Util::SafeDelete( pBuff );
			if( pOverlapped )
			{
				pOverlapped->pSendBuff = nullptr;
				WSASystem::FreeBuffer(pOverlapped);
			}

			if( hr != E_NET_IO_SEND_FAIL )
			{
				netTrace( TRC_TCPSENDRAW, "TCP Send Failed, ip:%0%, err:%1%, hr:%2%", pTCPCon->GetConnectionInfo().Remote, iWSAErr, ArgHex32(hr) );
			}
			else
				return S_OK;
		}
		else
		{
			netTrace( TRC_TCPSENDRAW, "TCP Send ip:%0%, size:%1%", pTCPCon->GetConnectionInfo().Remote, uiBuffSize );
		}


		return hr;
	}


	// Called when connection state changed
	HRESULT ClientTCP::OnConnectionStateChange( IConnection *pConnection )
	{
		HRESULT hr = S_OK;
		ConnectionTCP *pTCPCon = (ConnectionTCP*)pConnection;

		//netChk( __super::OnConnectionStateChange( pConnection ) );

		switch (pConnection->GetConnectionState())
		{
		case IConnection::STATE_CONNECTING:
			break;
		case IConnection::STATE_CONNECTED:
			if( pTCPCon->GetRecvBuffer()->Operation == OVERLAPPED_BUFFER::OP_NONE )
				pTCPCon->GetRecvBuffer()->Operation = OVERLAPPED_BUFFER::OP_TCPREADPENDING;
			//netChk( PendingRecv( (ConnectionTCP*)pConnection ) );
			break;
		case IConnection::STATE_DISCONNECTED:
			break;
		case IConnection::STATE_DISCONNECTING:
			break;
		};

	Proc_End:

		return hr;
	}



	////////////////////////////////////////////////////////////////////////////////
	//
	//	UDP Client class
	//
	ClientUDP::ClientUDP()
		:Client(Const::CLI_CONMAP_BUCKET_SIZE)
	{
		CliSystem::OpenSystem( Const::CLI_OVERBUFFER_COUNT );
		m_ConnectionManager.SetNetOwner( this );
	}


	ClientUDP::~ClientUDP()
	{
		m_ConnectionManager.TerminateManager();

		//ClientIO::g_ClientNetThread.Release();
		CliSystem::CloseSystem();

		//OverBufferMap::iterator iterBuffer = m_ioBuffers.begin();
		//for( ; iterBuffer != m_ioBuffers.end(); iterBuffer++ )
		//	delete iterBuffer->second;
		//m_ioBuffers.clear();
	}


	// check about initialize
	bool ClientUDP::IsReady()
	{
		return true;
	}


	// Connect to server
	HRESULT ClientUDP::Connect( const char *strServerIP, USHORT usServerPort, IConnection* &pNewConnection )
	{
		HRESULT hr = S_OK;
		SOCKET socket = INVALID_SOCKET;
		int iOptValue;
		sockaddr_in6 sockAddr;
		sockaddr_in6 sockAddrDest;
		u_long iMode = true;// non block
		ConnectionUDPClient *pConn = NULL;
		//hostent* localHost;
		//char* localIP;
		IConnection::ConnectionInformation connectionInfo;
		IConnection *pTemConn = NULL;
		OVERLAPPED_BUFFER_READ *pIOBuffer= NULL;
		sockaddr_in6 bindAddr;

		NetAddress localAddress;

		Net::GetLocalAddressIPv4(localAddress);
		Addr2SockAddr(localAddress, sockAddr);

		// Get the local host information
		//localHost = gethostbyname("");
		//localIP = inet_ntoa(*(struct in_addr *)*localHost->h_addr_list);

		// Set system allocate port
		//SetSockAddr(sockAddr, localIP, 0);


		socket = WSASocket(sockAddr.sin6_family, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
		if( socket == INVALID_SOCKET )
		{
			netTrace(Trace::TRC_ERROR, "Failed to Open Client Socket %0%", WSAGetLastError());
			netErr( E_UNEXPECTED );
		}


		iOptValue = Const::CLI_RECV_BUFFER_SIZE;
		if( setsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_RCVBUF = %0%, err = %1%", iOptValue, WSAGetLastError() );
			netErr( E_UNEXPECTED );
		}

		iOptValue = Const::CLI_SEND_BUFFER_SIZE;
		if( setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_SNDBUF = %0%, err = %1%", iOptValue, WSAGetLastError() );
			netErr( E_UNEXPECTED );
		}


		if (sockAddr.sin6_family == AF_INET6)
		{
			iOptValue = FALSE;
			if (setsockopt(socket, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
			{
				netTrace(Trace::TRC_ERROR, "Failed to change socket option IPV6_V6ONLY = %0%, err = %1%", iOptValue, WSAGetLastError());
				netErr(E_UNEXPECTED);
			}
		}


		bindAddr = sockAddr;
		//bindAddr.sin6_addr = in6addr_any;
		if (bind(socket, (SOCKADDR*)&bindAddr, sizeof(bindAddr)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Socket bind failed, UDP err=%0%", WSAGetLastError() );
			netErr( E_UNEXPECTED );
		}


		SetSockAddr( sockAddrDest, strServerIP, usServerPort );


		memset( &connectionInfo, 0, sizeof(connectionInfo) );
		SockAddr2Addr( sockAddr, connectionInfo.Local );

		netChk( StrUtil::StringCpy( connectionInfo.Remote.strAddr, strServerIP ) );
		connectionInfo.Remote.usPort = usServerPort;

		netMem( pConn = (ConnectionUDPClient*)m_ConnectionManager.NewConnection() );
		//pConn->SetupNet( this, CliSystem::GenNewCID() );
		netChk( pConn->InitConnection( socket, connectionInfo ) );
		netTrace(TRC_CONNECTION, "Initialize connection CID:%0%, Addr:%1%:%2%", pConn->GetCID(), pConn->GetConnectionInfo().Remote.strAddr, pConn->GetConnectionInfo().Remote.usPort);
		socket = INVALID_SOCKET;


		if( !CreateIoCompletionPort((HANDLE)pConn->GetSocket(), IOCPSystem::GetSystem().GetIOCP(), (ULONG_PTR)(IOCPSystem::IOCallBack*)pConn, 0) )
		{
			netTrace(Trace::TRC_ERROR, "CreateIoCompletionPort Failed TCP, hr = %0%", ArgHex32(GetLastHRESULT()) );
			netErr( E_UNEXPECTED );
		}

		pTemConn = pConn;
		//netChk( __super::Connect( strServerIP, usServerPort, pTemConn ) );

		netChk( pConn->PendingRecv() );

		netChk( GetConnectionManager().PendingWaitConnection( pConn ) );

		netChk( CliSystem::AddConnectionTask( &GetConnectionManager(), pConn ) );


		pNewConnection = pConn;
		pConn = NULL;

	Proc_End:


		if (pConn)
		{
			auto connTem = SharedPointerT<Connection>(pConn);
		}

		if( socket != INVALID_SOCKET )
			closesocket( socket );

		return hr;
	}

	

	// Release Connection
	HRESULT ClientUDP::ReleaseConnection( IConnection* pIConnection )
	{
		HRESULT hr = S_OK;
		ConnectionUDPClient *pConnection = (ConnectionUDPClient*)pIConnection;

		if( pIConnection == NULL )
		{
			netErr( E_UNEXPECTED );
		}

		if (pIConnection->GetConnectionState() != IConnection::STATE_DISCONNECTED)
		{
			netChk( pIConnection->CloseConnection() );
		}

		// close udp socket
		pConnection->CloseSocket();

		netChk( CliSystem::RemoveConnectionTask( pConnection ) );

		// Release will be done after task released
		//netChk( GetConnectionManager().PendingReleaseConnection( pConnection ) );


	Proc_End:

		return hr;
	}


	// Get connection from connection ID
	HRESULT ClientUDP::GetConnection( UINT_PTR uiCID, SharedPointerT<Connection> &pIConnection )
	{
		return m_ConnectionManager.GetConnectionByCID(uiCID, pIConnection);
	}


	// Send message to connection with network device
	HRESULT ClientUDP::SendMsg( IConnection *pConnection, Message::MessageData *pMsg )
	{
		ConnectionUDP *pUDPCon = (ConnectionUDP*)pConnection;
		return SendMsg( pConnection, pUDPCon->GetRemoteSockAddr(), pMsg );
	}

	// Send message to connection with network device to dst addr
	HRESULT ClientUDP::SendMsg( IConnection *pConnection, const sockaddr_in6& dstAddress, Message::MessageData *pMsg )
	{
		HRESULT hr = S_OK;
		//DWORD dwSend = 0;
		int iWSAErr = 0;

		Message::MessageID msgID = pMsg->GetMessageHeader()->msgID;
		UINT uiMsgLen = pMsg->GetMessageHeader()->Length;

		ConnectionUDP *pUDPCon = (ConnectionUDP*)pConnection;

		OVERLAPPED_BUFFER_WRITE *pOverlapped = NULL;
		netChk( WSASystem::AllocBuffer(pOverlapped) );

		pOverlapped->SetupSendUDP( pMsg );


		if( WSASendTo( pUDPCon->GetSocket(), &pOverlapped->wsaBuff, 1, nullptr, 0,
			(sockaddr*)&dstAddress, sizeof(sockaddr_in6), 
			pOverlapped, NULL ) == SOCKET_ERROR )
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
					pUDPCon->Disconnect();
					netErrSilent( E_NET_CONNECTION_CLOSED );
					break;
				default:
					netErr( E_NET_IO_SEND_FAIL );
					break;
				};
			}
		}

	Proc_End:

		if( FAILED(hr) )
		{
			if( pOverlapped )
			{
				Util::SafeRelease( pOverlapped->pMsgs );
				WSASystem::FreeBuffer(pOverlapped);
			}
			else
			{
				Util::SafeRelease( pMsg );
			}

			if( hr != E_NET_IO_SEND_FAIL )
			{
				netTrace( Trace::TRC_ERROR, "UDP Send Failed, ip:%0%, err:%1%, hr:%2%", pUDPCon->GetConnectionInfo().Remote, iWSAErr, ArgHex32(hr) );
			}
			else
				return S_OK;
		}
		else
		{
			if( msgID.IDs.Type == Message::MSGTYPE_NETCONTROL )
			{
				netTrace( TRC_NETCTRL, "UDP Ctrl ip:%0%, msg:%1%, Len:%2%", pUDPCon->GetConnectionInfo().Remote, msgID, uiMsgLen );
			}
			else
			{
				netTrace( TRC_SENDRAW, "UDP Send ip:%0%, msg:%1%, Len:%2%", pUDPCon->GetConnectionInfo().Remote, msgID, uiMsgLen );
			}
		}

		return hr;
	}


	// Send array of message buffer to connection with network device
	HRESULT ClientUDP::SendMsg( IConnection *pConnection, UINT uiBuffSize, BYTE* pBuff )
	{
		HRESULT hr = S_OK;
		//DWORD dwSend = 0;
		int iWSAErr = 0;


		ConnectionUDP *pUDPCon = (ConnectionUDP*)pConnection;

		//return S_OK;

		OVERLAPPED_BUFFER_WRITE *pOverlapped = NULL;
		netChk( WSASystem::AllocBuffer(pOverlapped) );

		pOverlapped->SetupSendUDP( uiBuffSize, pBuff );
		//pOverlapped->pConnection = pUDPCon;

		if( WSASendTo( pUDPCon->GetSocket(), &pOverlapped->wsaBuff, 1, nullptr, 0,
			(sockaddr*)&pUDPCon->GetRemoteSockAddr(), sizeof(sockaddr_in6), 
			pOverlapped, NULL ) == SOCKET_ERROR )
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
					pUDPCon->Disconnect();
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
			//// Send done just free all
			//Net::System::g_pOverlappedBuffer->FreeBuffer( pOverlapped );
			//pMsg->Release();
		}

	Proc_End:

		if( FAILED(hr) )
		{
			Util::SafeDelete( pBuff );
			if( pOverlapped )
			{
				//Util::SafeRelease( pOverlapped->pMsgs );
				pOverlapped->pSendBuff = nullptr;
				WSASystem::FreeBuffer(pOverlapped);
			}
			else
			{
				//Util::SafeRelease( pMsg );
			}

			if( hr != E_NET_IO_SEND_FAIL )
			{
				netTrace( Trace::TRC_ERROR, "UDP Send Failed, ip:%0%, err:%1%, hr:%2%", pUDPCon->GetConnectionInfo().Remote, iWSAErr, ArgHex32(hr) );
			}
			else
				return S_OK;
		}
		else
		{
			netTrace( TRC_SENDRAW, "UDP Send ip:%0%, Len:%1%", pUDPCon->GetConnectionInfo().Remote, uiBuffSize );
		}

		return hr;

	}

} // namespace Net
} // namespace BR
