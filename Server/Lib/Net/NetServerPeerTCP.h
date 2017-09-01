////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Net Server Peer implementation 
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Thread/Thread.h"
#include "Common/Synchronization.h"
#include "Net/NetDef.h"
#include "Net/NetSvrDef.h"
#include "Net/ConnectionUDP.h"
#include "Net/NetSystem.h"
#include "Net/NetServer.h"
#include "Net/NetServerPeer.h"



namespace BR {
namespace Svr {
namespace Config {
	class GenericServer;
};
};
};


namespace BR {
namespace Net {
	

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Peer Server network class
	//

	class ServerPeerTCP : public ServerTCP
	{
	public:
		typedef ConnectionManagerT<ConnectionPeerTCP> ConnectionManagerType;

	private:

		ConnectionManagerT<ConnectionPeerTCP>	m_ConnectionManager;


	public:
		ServerPeerTCP(ServerID InServerID, NetClass localClass);
		virtual ~ServerPeerTCP();


		// Maximum connection
		inline ConnectionManager& GetConnectionManager() override;

		// Get Active(non-free) connection
		inline virtual CounterType GetActiveConnectionCount() override;

		// Close all connection
		inline virtual Result CloseAllConnection() override;


		/////////////////////////////////////////////////////////////////////////////////////////////
		//  Net callback

		// handle Socket accept
		virtual Result OnAcceptedSocket(SOCKET acceptedSocket, const sockaddr_storage& remoteSockAddr, const IConnection::ConnectionInformation& connectionInfo, ConnectionPtr &pConnOut ) override;


		// Release instance
		virtual void Release() override;

		// check about initialize
		virtual bool IsReady() override;


		// Open host and start listen
		virtual Result HostOpen( NetClass netCls, const char *strLocalIP, USHORT usLocalPort ) override;

		// Close host and close all connections
		//virtual Result HostClose();

		// Make a connection to another server
		virtual Result Connect(IConnection* pIConn, UINT remoteID, NetClass netClass, const NetAddress& destAddress) override;

		// Connect to other peer
		Result RegisterServerConnection(ServerID serverID, NetClass netClass, const NetAddress& netAddress, Net::ConnectionPtr &pConnection);

	};


#include "NetServerPeerTCP.inl"


} // namespace BR
} // namespace Net



