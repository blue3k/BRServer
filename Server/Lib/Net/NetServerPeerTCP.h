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

#include "Common/Typedefs.h"
#include "Common/Thread.h"
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
		inline virtual HRESULT CloseAllConnection() override;


		/////////////////////////////////////////////////////////////////////////////////////////////
		//  Net callback

		// handle Socket accept
		virtual HRESULT OnNewSocket(SOCKET acceptedSocket, const sockaddr_storage& remoteSockAddr, const IConnection::ConnectionInformation& connectionInfo, IConnection* &pConnOut ) override;


		// Release instance
		virtual void Release() override;

		// check about initialize
		virtual bool IsReady() override;


		// Open host and start listen
		virtual HRESULT HostOpen( NetClass netCls, const char *strLocalIP, USHORT usLocalPort ) override;

		// Close host and close all connections
		//virtual HRESULT HostClose();

		// Make a connection to another server
		virtual HRESULT Connect(IConnection* pIConn, UINT remoteID, NetClass netClass, const NetAddress& destAddress) override;

		// Connect to other peer
		HRESULT RegisterServerConnection(ServerID serverID, NetClass netClass, const NetAddress& netAddress, Net::IConnection* &pConnection);

	};


#include "NetServerPeerTCP.inl"


} // namespace BR
} // namespace Net



