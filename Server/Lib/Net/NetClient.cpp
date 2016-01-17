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
			SyncCounter g_lCIDGen(1);

			// system opened count
			SyncCounter g_lSysOpenCount(0);

			// system opened count
			ConnectionTaskManager *g_ConnectionTaskManager = nullptr;



			CounterType GenNewCID()
			{
				CounterType lCID = g_lCIDGen.fetch_add(1, std::memory_order_relaxed) + 1;
				if (lCID == 0)
					lCID = g_lCIDGen.fetch_add(1, std::memory_order_relaxed) + 1;
				return lCID;
			}


			// Open network system
			HRESULT OpenSystem(UINT uiOverBufferCount)
			{
				HRESULT hr = S_OK;

				netChk(NetSystem::OpenSystem(uiOverBufferCount, Const::CLI_NUM_RECV_THREAD, Const::PACKET_GATHER_SIZE_MAX));

				if (g_lSysOpenCount.fetch_add(1, std::memory_order_relaxed) == 0)
				{
					netTrace(TRC_NET, "Opening client net system");

					g_ConnectionTaskManager = new ConnectionTaskManager;

					netChk(g_ConnectionTaskManager->InitializeManager(Const::CLI_CONNECTION_TASK_THREAD));
				}


			Proc_End:

				return hr;
			}


			// Close network system
			void CloseSystem()
			{
				HRESULT hr = S_OK;
				CounterType lCount = 0;

				if (g_lSysOpenCount > 0)
				{
					lCount = g_lSysOpenCount.fetch_sub(1, std::memory_order_relaxed) - 1;

					if (lCount <= 0)
					{
						netTrace(TRC_NET, "Closing client net system");

						if (g_ConnectionTaskManager)
							netChk(g_ConnectionTaskManager->TerminateManager());

						Util::SafeDelete(g_ConnectionTaskManager);

						g_lSysOpenCount = 0;
					}

					NetSystem::CloseSystem();
				}

			Proc_End:
				;
			}


			// Add connection update task
			HRESULT AddConnectionTask(ConnectionManager *pConMgr, Connection * pConnection)
			{
				HRESULT hr = S_OK;
				ConnectionTask *pTask = nullptr;

				if (g_ConnectionTaskManager == nullptr)
					return E_NET_NOTINITIALIZED;

				netChkPtr(pConMgr);
				netChkPtr(pConnection);

				netChk(g_ConnectionTaskManager->CreateTask(pTask));

				pTask->SetConnection(pConMgr, pConnection);

				pConnection->SetUData((uintptr_t)pTask);

				netChk(g_ConnectionTaskManager->AddTickTask(pTask));

			Proc_End:

				return hr;
			}

			// Remove Connection update task
			HRESULT RemoveConnectionTask(Connection * pConnection)
			{
				HRESULT hr = S_OK;
				ConnectionTask *pTask = nullptr;

				if (g_ConnectionTaskManager == nullptr)
					return E_NET_NOTINITIALIZED;

				pTask = (ConnectionTask*)pConnection->GetUData();

				if (pTask)
					netChk(g_ConnectionTaskManager->RemoveTickTask(pTask));
				pTask = nullptr;

			Proc_End:

				Util::SafeDelete(pTask);

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
			ConnectionManager::Operation oper;
			ConnectionTask *pTask = nullptr;

			super::m_bNoNewConnection = true;

			Thread::Stop(true);


			// Clear pending operation queue
			while (super::m_PendingOperations.Dequeue(oper) == S_OK)
			{
				switch (oper.OpCode)
				{
				case super::Operation::OP_WAITING_CONNECTION:
				case super::Operation::OP_PENDING_CONNECTION:
				case super::Operation::OP_PENDING_INITCONNECTION:
				case super::Operation::OP_RELEASE_CONNECTION:
					if (oper.pConn != nullptr)
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
						ConnectionManagerT<ConnectionType>::FreeConnection((Connection*)pConn);
					}
					break;
				case super::Operation::OP_PENDING_ADDR:
				default:
					break;
				};
			}

			// clear connection queue
			super::m_ManagedConnections.ForeachOrder(0, (UINT)super::m_ManagedConnections.GetItemCount(), [&](const uintptr_t& key, SharedPointerT<Connection> pConn)->bool
			{
				pConn->CloseConnection();
				pTask = (ConnectionTask*)pConn->GetUData();
				if (pTask)
				{
					pTask->SetConnection(nullptr, nullptr);
					CliSystem::RemoveConnectionTask((Connection*)pConn);
				}
				super::FreeConnection((Connection*)pConn);
				return true;
			});

			super::m_ManagedConnections.ClearMap();


		//Proc_End:


			return hr;
		}



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Client base class
	//


	Client::Client(UINT uiConMapBucketSize)
		:m_NetEventQueue(32)
	{
		//CliSystem::OpenSystem( SvrConst::Net::CLI_OVERBUFFER_COUNT );
	}

	Client::~Client()
	{
		//System::CloseSystem();
	}

	// Called when connection result 
	void Client::OnConnectionResult(Connection *pConnection, HRESULT hrConnect)
	{
	}

	HRESULT Client::ConnectCli(const NetAddress& destAddress, IConnection* &pNewConnection)
	{
		HRESULT hr = S_OK;

		pNewConnection = GetConnectionManager().NewConnection();
		netChkPtr(pNewConnection);

		netChk(Connect(pNewConnection, 0, NetClass::Client, destAddress));

	Proc_End:

		return hr;
	}

	// Release instance
	void Client::Release()
	{
		delete this;
	}


	// take over connection management
	HRESULT Client::TakeOverConnection(IConnection* pIConnection)
	{
		HRESULT hr = S_OK;
		Connection* pConn = (Connection*)pIConnection;

		netChkPtr(pIConnection);
		netErr(E_NOTIMPL);
		netChk(GetConnectionManager().PendingManagedConnectionTakenOver(pConn));

	Proc_End:

		if (FAILED(hr))
		{
			netTrace(Trace::TRC_ERROR, "ReleaseConnection Failed hr={0:X8}", hr);
		}

		return hr;
	}

	// Release Connection
	HRESULT Client::ReleaseConnection(IConnection* pIConnection)
	{
		HRESULT hr = S_OK;
		ConnectionTCP *pConTCP = (ConnectionTCP*)pIConnection;

		if (pIConnection == nullptr)
		{
			netErr(E_UNEXPECTED);
		}

		if (pIConnection->GetConnectionState() != Net::IConnection::STATE_DISCONNECTED)
		{
			netChk(pIConnection->CloseConnection());
		}

		netChk(CliSystem::RemoveConnectionTask(pConTCP));

		// Release will be done after task released
		//netChk( GetConnectionManager().PendingReleaseConnection( (Connection*)pIConnection ) );


	Proc_End:

		return hr;
	}

	HRESULT Client::GetConnection(uintptr_t uiCID, SharedPointerT<IConnection> &pIConnection)
	{
		SharedPointerT<Connection> pConnection;

		HRESULT hr = GetConnectionManager().GetConnectionByCID(uiCID, pConnection);
		if (SUCCEEDED(hr))
		{
			pIConnection = pConnection;
		}

		return hr;
	}


	// Query Network event
	HRESULT Client::DequeueNetEvent(Event& evt)
	{
		return m_NetEventQueue.Dequeue(evt);
	}




	////////////////////////////////////////////////////////////////////////////////
	//
	//	TCP Client class
	//

	ClientTCP::ClientTCP()
		:Client(Const::CLI_CONMAP_BUCKET_SIZE)
	{
		CliSystem::OpenSystem(Const::CLI_OVERBUFFER_COUNT);

		m_ConnectionManager.SetNetOwner(this);
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
	HRESULT ClientTCP::Connect(IConnection* pIConn, UINT remoteID, NetClass netClass, const NetAddress& destAddress)
	{
		HRESULT hr = S_OK;
		SOCKET socket = INVALID_SOCKET;
		int iOptValue;
		sockaddr_storage sockAddr, sockAddrDest;
		ConnectionTCPClient *pConn = nullptr;
		Net::IConnection::ConnectionInformation connectionInfo;
		sockaddr_storage bindAddr;
		NetAddress localAddress;

		if(destAddress.SocketFamily == SockFamily::IPV4)
			Net::GetLocalAddressIPv4(localAddress);
		else
			Net::GetLocalAddressIPv6(localAddress);

		Addr2SockAddr(localAddress, sockAddr);


		socket = NetSystem::Socket(localAddress.SocketFamily, SockType::Stream);
		if (socket == INVALID_SOCKET)
		{
			netTrace(Trace::TRC_ERROR, "Failed to Open Client Socket {0:X8}", GetLastWSAHRESULT());
			netErr(E_UNEXPECTED);
		}


		iOptValue = Const::CLI_RECV_BUFFER_SIZE;
		if (setsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_RCVBUF={0}, err={1:X8}", iOptValue, GetLastWSAHRESULT());
			netErr(E_UNEXPECTED);
		}

		iOptValue = Const::CLI_SEND_BUFFER_SIZE;
		if (setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_SNDBUF={0}, err={1:X8}", iOptValue, GetLastWSAHRESULT());
			netErr(E_UNEXPECTED);
		}

		if (sockAddr.ss_family == AF_INET6)
		{
			iOptValue = FALSE;
			if (setsockopt(socket, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
			{
				netTrace(Trace::TRC_ERROR, "Failed to change socket option IPV6_V6ONLY={0}, err={1:X8}", iOptValue, GetLastWSAHRESULT());
				netErr(E_UNEXPECTED);
			}
		}

#if WINDOWS
		{
			u_long iMode = true;// non block
			iMode = true;
			if (ioctlsocket(socket, FIONBIO, &iMode) == SOCKET_ERROR)
			{
				netTrace(Trace::TRC_ERROR, "Failed to change socket IO Mode to {0},  err={0:X8}", iMode, GetLastWSAHRESULT());
				netErr(E_UNEXPECTED);
			}
		}
#endif

		bindAddr = sockAddr;
		if (bind(socket, (sockaddr*)&bindAddr, sizeof(bindAddr)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Socket bind failed, TCP {0:X8}", GetLastWSAHRESULT() );
			netErr( E_UNEXPECTED );
		}


		netMem( pConn = dynamic_cast<ConnectionTCPClient*>(pIConn) );


		netChk(Addr2SockAddr(destAddress, sockAddrDest));

		memset( &connectionInfo, 0, sizeof(connectionInfo) );
		SockAddr2Addr( sockAddr, connectionInfo.Local );
		connectionInfo.LocalClass = netClass;
		connectionInfo.LocalID = 1;

		connectionInfo.Remote = destAddress;
		connectionInfo.RemoteID = remoteID;

		netChk( pConn->InitConnection( socket, connectionInfo ) );
		netTrace(TRC_CONNECTION, "Initialize connection CID:{0}, Addr:{1}:{2}", pConn->GetCID(), pConn->GetConnectionInfo().Remote.strAddr, pConn->GetConnectionInfo().Remote.usPort);
		socket = INVALID_SOCKET;


		netChk(NetSystem::RegisterSocket(SockType::Stream, pConn));

		if (connect(pConn->GetIOSocket(), (sockaddr*)&sockAddrDest, sizeof(sockAddrDest)) != SOCKET_ERROR
			|| GetLastWSAHRESULT() != E_NET_WOULDBLOCK)
		{
			netTrace(Trace::TRC_ERROR, "connect failed, TCP {0:X8}", GetLastWSAHRESULT());
			netErr(E_UNEXPECTED);
		}

		netChk( GetConnectionManager().PendingWaitConnection( pConn ) );

		netChk( CliSystem::AddConnectionTask( &GetConnectionManager(), pConn ) );


		// Wait connection 
		//new BR::FunctorThread([&, ID](Thread* pThread)
		//{
		pConn->WaitConnect();
		//});

		pConn = nullptr;


	Proc_End:

		if (pConn)
		{
			auto connTem = SharedPointerT<Connection>(pConn);
			pIConn = nullptr;
		}
		//if( pConn )
		//	pConn->Release();

		if( socket != INVALID_SOCKET )
			NetSystem::CloseSocket( socket );


		return hr;
	}
	


	// Called when connection state changed
	HRESULT ClientTCP::OnConnectionStateChange( IConnection *pConnection )
	{
		HRESULT hr = S_OK;

		switch (pConnection->GetConnectionState())
		{
		case IConnection::STATE_CONNECTING:
			break;
		case IConnection::STATE_CONNECTED:
			break;
		case IConnection::STATE_DISCONNECTED:
			break;
		case IConnection::STATE_DISCONNECTING:
			break;
		default:
			break;
		};

	//Proc_End:

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
	}


	// check about initialize
	bool ClientUDP::IsReady()
	{
		return true;
	}


	// Connect to server
	HRESULT ClientUDP::Connect(IConnection* pIConn, UINT remoteID, NetClass netClass, const NetAddress& destAddress)
	{
		HRESULT hr = S_OK;
		SOCKET socket = INVALID_SOCKET;
		int iOptValue;
		sockaddr_storage sockAddr;
		sockaddr_storage sockAddrDest;
		ConnectionUDPClient *pConn = nullptr;
		IConnection::ConnectionInformation connectionInfo;
		sockaddr_storage bindAddr;

		NetAddress localAddress;

		if (destAddress.SocketFamily == SockFamily::IPV4)
			Net::GetLocalAddressIPv4(localAddress);
		else
			Net::GetLocalAddressIPv6(localAddress);

		Addr2SockAddr(localAddress, sockAddr);


		socket = NetSystem::Socket(localAddress.SocketFamily, SockType::DataGram);
		if( socket == INVALID_SOCKET )
		{
			netTrace(Trace::TRC_ERROR, "Failed to Open Client Socket {0:X8}", GetLastWSAHRESULT());
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


		if (sockAddr.ss_family == AF_INET6)
		{
			iOptValue = FALSE;
			if (setsockopt(socket, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
			{
				netTrace(Trace::TRC_ERROR, "Failed to change socket option IPV6_V6ONLY = {0}, err = {1:X8}", iOptValue, GetLastWSAHRESULT());
				netErr(E_UNEXPECTED);
			}
		}


		bindAddr = sockAddr;
		if (bind(socket, (sockaddr*)&bindAddr, sizeof(bindAddr)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Socket bind failed, UDP err={0:X8}", GetLastWSAHRESULT() );
			netErr( E_UNEXPECTED );
		}

		netChk(Addr2SockAddr(destAddress, sockAddrDest));


		memset( &connectionInfo, 0, sizeof(connectionInfo) );
		SockAddr2Addr( sockAddr, connectionInfo.Local );
		connectionInfo.LocalClass = netClass;
		connectionInfo.LocalID = 1;

		connectionInfo.Remote = destAddress;
		connectionInfo.RemoteID = remoteID;

		netMem( pConn = (ConnectionUDPClient*)pIConn);
		netChk( pConn->InitConnection( socket, connectionInfo ) );
		netTrace(TRC_CONNECTION, "Initialize connection CID:{0}, Addr:{1}:{2}", pConn->GetCID(), pConn->GetConnectionInfo().Remote.strAddr, pConn->GetConnectionInfo().Remote.usPort);
		socket = INVALID_SOCKET;

		netChk(NetSystem::RegisterSocket(SockType::DataGram, pConn));

		netChk( pConn->PendingRecv() );

		netChk( GetConnectionManager().PendingWaitConnection( pConn ) );

		netChk( CliSystem::AddConnectionTask( &GetConnectionManager(), pConn ) );


		pConn = nullptr;

	Proc_End:


		if (pConn)
		{
			auto connTem = SharedPointerT<Connection>(pConn);
		}

		if( socket != INVALID_SOCKET )
			NetSystem::CloseSocket( socket );

		return hr;
	}

	





	////////////////////////////////////////////////////////////////////////////////
	//
	//	UDP Client class
	//
	ClientMUDP::ClientMUDP()
		:Client(Const::CLI_CONMAP_BUCKET_SIZE)
	{
		CliSystem::OpenSystem(Const::CLI_OVERBUFFER_COUNT);
		m_ConnectionManager.SetNetOwner(this);
	}


	ClientMUDP::~ClientMUDP()
	{
		m_ConnectionManager.TerminateManager();

		//ClientIO::g_ClientNetThread.Release();
		CliSystem::CloseSystem();
	}


	// check about initialize
	bool ClientMUDP::IsReady()
	{
		return true;
	}


	// Connect to server
	HRESULT ClientMUDP::Connect(IConnection* pIConn, UINT remoteID, NetClass netClass, const NetAddress& destAddress)
	{
		HRESULT hr = S_OK;
		SOCKET socket = INVALID_SOCKET;
		int iOptValue;
		sockaddr_storage sockAddr;
		sockaddr_storage sockAddrDest;
		ConnectionMUDPClient *pConn = nullptr;
		IConnection::ConnectionInformation connectionInfo;
		sockaddr_storage bindAddr;
		NetAddress localAddress;

		if (destAddress.SocketFamily == SockFamily::IPV4)
			Net::GetLocalAddressIPv4(localAddress);
		else
			Net::GetLocalAddressIPv6(localAddress);

		netChk(Addr2SockAddr(localAddress, sockAddr));

		socket = NetSystem::Socket(localAddress.SocketFamily, SockType::DataGram);
		if (socket == INVALID_SOCKET)
		{
			netTrace(Trace::TRC_ERROR, "Failed to Open Client Socket {0:X8}", GetLastWSAHRESULT());
			netErr(E_UNEXPECTED);
		}


		iOptValue = Const::CLI_RECV_BUFFER_SIZE;
		if (setsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_RCVBUF = {0}, err = {1:X8}", iOptValue, GetLastWSAHRESULT());
			netErr(E_UNEXPECTED);
		}

		iOptValue = Const::CLI_SEND_BUFFER_SIZE;
		if (setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_SNDBUF = {0}, err = {1:X8}", iOptValue, GetLastWSAHRESULT());
			netErr(E_UNEXPECTED);
		}


		if (localAddress.SocketFamily == SockFamily::IPV6)
		{
			iOptValue = FALSE;
			if (setsockopt(socket, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
			{
				netTrace(Trace::TRC_ERROR, "Failed to change socket option IPV6_V6ONLY = {0}, err = {1:X8}", iOptValue, GetLastWSAHRESULT());
				netErr(E_UNEXPECTED);
			}
		}


		bindAddr = sockAddr;
		if (bind(socket, (sockaddr*)&bindAddr, sizeof(bindAddr)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Socket bind failed, UDP err={0:X8}", GetLastWSAHRESULT());
			netErr(E_UNEXPECTED);
		}


		netChk(Addr2SockAddr(destAddress, sockAddrDest));


		memset(&connectionInfo, 0, sizeof(connectionInfo));
		SockAddr2Addr(sockAddr, connectionInfo.Local);
		connectionInfo.LocalClass = netClass;
		connectionInfo.LocalID = 1;

		connectionInfo.Remote = destAddress;
		connectionInfo.RemoteID = remoteID;

		netMem(pConn = dynamic_cast<ConnectionMUDPClient*>(pIConn));

		netChk(pConn->InitConnection(socket, connectionInfo));
		netTrace(TRC_CONNECTION, "Initialize connection CID:{0}, Addr:{1}:{2}", pConn->GetCID(), pConn->GetConnectionInfo().Remote.strAddr, pConn->GetConnectionInfo().Remote.usPort);
		socket = INVALID_SOCKET;

		netChk(NetSystem::RegisterSocket(SockType::DataGram, pConn));

		netChk(pConn->PendingRecv());

		netChk(GetConnectionManager().PendingWaitConnection(pConn));

		netChk(CliSystem::AddConnectionTask(&GetConnectionManager(), pConn));


		pConn = nullptr;

	Proc_End:


		if (pConn)
		{
			auto connTem = SharedPointerT<Connection>(pConn);
		}

		if (socket != INVALID_SOCKET)
			NetSystem::CloseSocket(socket);

		return hr;
	}





} // namespace Net
} // namespace BR

