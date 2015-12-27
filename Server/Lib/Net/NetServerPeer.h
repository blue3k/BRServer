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

	class ServerPeer : public ServerNet, public INetIOCallBack
	{
	public:
		typedef ConnectionManagerT<ConnectionUDPServerPeer> ConnectionManagerType;

	private:
		// Connection manager
		ConnectionManagerType	m_ConnectionManager;

		// Connection ID gen
		LONG			m_CIDGen;

		// Buffer for recv
		IOBUFFER_READ*	m_pRecvBuffers;

		SyncCounter				m_PendingRecvCnt;

		HRESULT ServerHostOpen(NetClass netCls, const char *strLocalIP, USHORT usLocalPort);

	public:
		ServerPeer(ServerID InServerID, NetClass localClass);
		virtual ~ServerPeer();


		// Get connection manager
		inline ConnectionManager& GetConnectionManager();

		// Get Active(non-free) connection
		virtual CounterType GetActiveConnectionCount();

		// Close all connection
		virtual HRESULT CloseAllConnection();



		/////////////////////////////////////////////////////////////////////////////////////////////
		//  Net callback

		virtual SOCKET GetIOSocket() override { return GetSocket(); }

		// called when reciving TCP message
		virtual HRESULT OnIORecvCompleted( HRESULT hrRes, IOBUFFER_READ* &pIOBuffer ) override;

		virtual HRESULT OnSendReady() override;

		// called when send completed
		virtual HRESULT OnIOSendCompleted( HRESULT hrRes, IOBUFFER_WRITE *pIOBuffer ) override;


		// Release instance
		virtual void Release();

		// check about initialize
		virtual bool IsReady();

		// Get connection from connection ID
		virtual HRESULT GetConnection(UINT uiCID, SharedPointerT<Connection> &pConnection);


		//// Send message to connection with network device
		//virtual HRESULT SendMsg( IConnection *pConnection, Message::MessageData *pMsg );

		//// Send message to connection with network device to dst addr
		//virtual HRESULT SendMsg( IConnection *pConnection, const sockaddr_in6& dstAddr, Message::MessageData *pMsg );

		//// Send array of message buffer to connection with network device
		//virtual HRESULT SendMsg( IConnection *pConnection, UINT uiBuffSize, BYTE* pBuff );

		// Pending recv New one
		HRESULT PendingRecv( IOBUFFER_READ *pOver );


		// Open host and start listen
		virtual HRESULT HostOpen( NetClass netCls, const char *strLocalIP, USHORT usLocalPort );

		// Close host and close all connections
		virtual HRESULT HostClose();


		// Connect to other peer
		virtual HRESULT RegisterServerConnection( ServerID serverID, NetClass netClass, const NetAddress& destAddress, Net::IConnection* &pConnection );

	};


#include "NetServerPeer.inl"


} // namespace BR
} // namespace Net



