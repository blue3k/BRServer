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
#include "Net/NetServerPeerTCP.h"
#include "Net/NetConst.h"
#include "Protocol/ProtocolVer.h"


namespace BR {
namespace Net {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Peer network class
	//

	ServerPeerTCP::ServerPeerTCP(ServerID InServerID, NetClass localClass)
		:ServerTCP( InServerID, localClass )
		,m_ConnectionManager(Const::SVR_PUBLIC_CONNECTION_BUCKET_SIZE)
	{
		m_ConnectionManager.SetNetOwner(this);
		m_ConnectionManager.SetUseAddressMap(false);
		m_ConnectionManager.SetPeerIDMap(false);
	}

	ServerPeerTCP::~ServerPeerTCP()
	{
	}


	// handle Socket accept
	HRESULT ServerPeerTCP::OnNewSocket(SOCKET acceptedSocket, const sockaddr_in6& remoteSockAddr, const IConnection::ConnectionInformation& connectionInfo, IConnection* &pConnOut)
	{
		HRESULT hr = S_OK;
		ConnectionTCP *pConnection = nullptr;
		SharedPointerT<Connection> pConn;
		uintptr_t cid = 0;
//		ConnectionManager::AddrIterator itCon;
		bool bNeedPending = false;

		pConnOut = nullptr;

		// Connect to IOCP
		if (SUCCEEDED(GetConnectionManager().GetConnectionByAddr(remoteSockAddr, pConn)))
		{
			//pConnection = (ConnectionTCP*)(*itCon).pConnection;
			pConnection = (ConnectionTCP*)(Connection*)pConn;
			pConnection->CloseConnection();
			pConn = SharedPointerT<Connection>();
			netTrace(TRC_INFO, "Swap incomming connection socket with old server connection CID:%0%", pConnection->GetCID());
		}
		else
		{
			// Create New connection for accept
			pConnection = (ConnectionTCP*)GetConnectionManager().NewConnection();
			if (pConnection == nullptr)// Maybe maxconnection ?
			{
				netTrace(Trace::TRC_ERROR, "Failed to allocate a new connection now active:%0%", GetConnectionManager().GetNumActiveConnection());
				netErr(E_FAIL);
			}

			pConn = SharedPointerT<Connection>(pConnection);
			bNeedPending = true;
		}

		netChk(NetSystem::RegisterSocket(acceptedSocket, pConnection, false));

		pConnOut = pConnection;
		cid = pConnection->GetCID();

		// Initialize connection
		netChk( pConnection->InitConnection( acceptedSocket, connectionInfo ) );
		netTrace(TRC_CONNECTION, "Initialize connection CID:%0%, Addr:%1%:%2%", pConnection->GetCID(), pConnection->GetConnectionInfo().Remote.strAddr, pConnection->GetConnectionInfo().Remote.usPort);

		if (bNeedPending)
		{
			netChk(GetConnectionManager().PendingWaitConnection(pConnection));
		}
		pConn = SharedPointerT<Connection>();


	Proc_End:


		if( FAILED(hr) )
		{
			if( pConn != nullptr )
			{
				//pConn->AddRef(); // add 1 before release
				GetConnectionManager().PendingReleaseConnection((Connection*)pConn);
			}

			netTrace( Trace::TRC_ERROR, "Tcp Accept failed  hr = {0:X8}", hr );
		}
		else
		{
			if( pConn != nullptr )
				netTrace( TRC_NET, "Net Con Accept Svr:%0%, CID:%1%, From %2%", GetLocalAddress().usPort, cid, connectionInfo.Remote );
		}

		return hr;

	}


	// Release instance
	void ServerPeerTCP::Release()
	{
		delete this;
	}

	// check about initialize
	bool ServerPeerTCP::IsReady()
	{
		return GetSocket() != INVALID_SOCKET;
	}
	



	// Open host and start listen
	HRESULT ServerPeerTCP::HostOpen( NetClass netCls, const char *strLocalIP, USHORT usLocalPort )
	{
		HRESULT hr = S_OK;

		netChk( ServerTCP::HostOpen( netCls, strLocalIP, usLocalPort ) );

	Proc_End:

		if( FAILED(hr) )
			HostClose();

		return hr;
	}


	// Make a connection to another server
	HRESULT ServerPeerTCP::Connect(IConnection* pIConn, UINT remoteID, NetClass netClass, const char *strDstIP, USHORT usDstPort)
	{
		HRESULT hr = S_OK;
		Net::IConnection::ConnectionInformation connectionInfo;
		ConnectionTCP *pConn = nullptr;
		SOCKET socket = INVALID_SOCKET;
		sockaddr_in6 remoteAddr, localsockAddr;
		INT32 iOptValue;

		netChkPtr(pIConn);
		pConn = dynamic_cast<ConnectionTCP*>(pIConn);
		netChkPtr(pIConn);

		memset(&connectionInfo, 0, sizeof(connectionInfo));

		connectionInfo.SetLocalInfo(GetNetClass(), GetLocalAddress(), GetServerID());

		netChk(StrUtil::StringCpy(connectionInfo.Remote.strAddr, strDstIP));
		connectionInfo.Remote.usPort = usDstPort;
		connectionInfo.RemoteClass = netClass;
		connectionInfo.RemoteID = remoteID;

		// Get the local host information
		SetSockAddr(localsockAddr, GetLocalAddress().strAddr, 0);// to make port generation

		//socket = WSASocket(localsockAddr.sin6_family, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
		socket = NetSystem::Socket(SockFamily::IPV6, SockType::Stream);
		if (socket == INVALID_SOCKET)
		{
			netTrace(Trace::TRC_ERROR, "Failed to Open a Socket {0:X8}", GetLastWSAHRESULT());
			netErr(E_UNEXPECTED);
		}

		netChk(SetupSocketOption(socket));

#if WINDOWS
		{
			u_long iMode = true;// non block
			iMode = true;
			if (ioctlsocket(socket, FIONBIO, &iMode) == SOCKET_ERROR)
			{
				netTrace(Trace::TRC_ERROR, "Failed to change socket IO Mode to {0},  err = {1:X8}", iMode, GetLastWSAHRESULT());
				netErr(E_UNEXPECTED);
			}
		}
#endif

		if (localsockAddr.sin6_family == AF_INET6)
		{
			iOptValue = FALSE;
			if (setsockopt(socket, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
			{
				netTrace(Trace::TRC_ERROR, "Failed to change socket option IPV6_V6ONLY = {0}, err = {1:X8}", iOptValue, GetLastWSAHRESULT());
				netErr(E_UNEXPECTED);
			}
		}

		SetSockAddr(remoteAddr, strDstIP, usDstPort);

		netChk(pConn->InitConnection(socket, connectionInfo));
		socket = INVALID_SOCKET;

		netChk(NetSystem::RegisterSocket(pConn->GetSocket(), pConn, false));

		if(SUCCEEDED(pConn->Connect()))
			pConn->WaitConnect();


	Proc_End:

		if (socket != INVALID_SOCKET)
			NetSystem::CloseSocket(socket);

		return hr;
	}


	// Connect to other peer
	HRESULT ServerPeerTCP::RegisterServerConnection( ServerID serverID, NetClass netClass, const char *strDstIP, USHORT usDstPort, Net::IConnection* &pConnection )
	{
		HRESULT hr = S_OK;
		ConnectionTCP *pConn = nullptr;
		uintptr_t CID = 0;

		netMem( pConn = dynamic_cast<ConnectionPeerTCP*>(GetConnectionManager().NewConnection()) );

		CID = pConn->GetCID();

		if(FAILED(Connect(pConn, serverID, netClass, strDstIP, usDstPort)))
		{
			netTrace(Trace::TRC_WARN, "Opening connection is failed, ServerID %0%", serverID);
		}

		pConnection = pConn;
		pConn = nullptr;

	Proc_End:

		if( SUCCEEDED(hr) )
		{
			netTrace(TRC_NET, "ServerPeer Allowing Server:%3%:%4%, %0%:%1%, CID:%2%", strDstIP, usDstPort, CID, netClass, serverID);
		}

		return hr;
	}



} // namespace Net	
} // namespace BR

