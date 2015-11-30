////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Net Server 
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/Thread.h"
#include "Net/NetDef.h"
#include "Net/NetSvrDef.h"
#include "Net/ConnectionUDP.h"
#include "Net/ConnectionMUDP.h"
#include "Net/NetServer.h"
#include "Net/ConnectionManager.h"
#include "Common/SpinSharedBuffer.h"
#include "Common/PageQueue.h"




namespace BR {
namespace Net {



	////////////////////////////////////////////////////////////////////////////////
	//
	//	UDP Server class
	//

	class ServerUDPBase : public Server, public INetIOCallBack
	{
	public:

		// recv buffer
		IOBUFFER_READ *m_pRecvBuffers;

	public:
		ServerUDPBase(ServerID InServerID, NetClass localClass);
		virtual ~ServerUDPBase();


		// Get Active(non-free) connection
		virtual CounterType GetActiveConnectionCount();

		// Close all connection
		virtual HRESULT CloseAllConnection();


		/////////////////////////////////////////////////////////////////////////////////////////////
		// callback
		// called when New connection TCP accepted

		// called when network message is received
		virtual HRESULT Recv(IOBUFFER_READ* pIOBuffer) override;
		//virtual HRESULT OnIORecvCompleted( HRESULT hrRes, IOBUFFER_READ *pIOBuffer ) = 0;

		// called when send completed
		virtual HRESULT OnIOSendCompleted( HRESULT hrRes, IOBUFFER_WRITE *pIOBuffer );


		// Open host and start listen
		virtual HRESULT HostOpen( NetClass netCls, const char *strLocalIP, USHORT usLocalPort );

		// Close host and close all connections
		virtual HRESULT HostClose();



		// Send message to connection with network device
		virtual HRESULT SendMsg( IConnection *pConnection, Message::MessageData *pMsg );

		// Send message to connection with network device to dst addr
		virtual HRESULT SendMsg( IConnection *pConnection, const sockaddr_in6& dstAddr, Message::MessageData *pMsg );

		// Send array of message buffer to connection with network device
		virtual HRESULT SendMsg( IConnection *pConnection, UINT uiBuffSize, BYTE* pBuff );

		// Pending recv New one
		HRESULT PendingRecv( IOBUFFER_READ *pOverlaped );

	};
	
	


	////////////////////////////////////////////////////////////////////////////////
	//
	//	UDP Server class
	//

	class ServerUDP : public ServerUDPBase
	{
	public:
		typedef ConnectionManagerT<ConnectionUDPServer> ConnectionManagerType;

	private:
		// Connection manager
		ConnectionManagerType	m_ConnectionManager;

	public:
		ServerUDP(ServerID InServerID, NetClass localClass);
		virtual ~ServerUDP();
		
		// Maximum connection
		inline ConnectionManager& GetConnectionManager();


		// called when network message is received
		virtual HRESULT OnIORecvCompleted( HRESULT hrRes, IOBUFFER_READ *pIOBuffer );
	};




	////////////////////////////////////////////////////////////////////////////////
	//
	//	Mobile UDP Server class
	//

	class ServerMUDP : public ServerUDPBase
	{
	public:
		typedef ConnectionManagerT<ConnectionMUDPServer> ConnectionManagerType;

	private:
		// Connection manager
		ConnectionManagerType	m_ConnectionManager;

	public:
		ServerMUDP(ServerID InServerID, NetClass localClass);
		virtual ~ServerMUDP();

		HRESULT SendNetCtrl( const sockaddr_in6& dstAddress, UINT uiCtrlCode, UINT uiSequence, Message::MessageID msgID, UINT64 UID );
		
		// Maximum connection
		inline ConnectionManager& GetConnectionManager();

		// Register PeerID to map

		HRESULT OnNoConnectionPacket(const struct sockaddr_in6& from, const BYTE* pData);

		// called when network message is received
		virtual HRESULT OnIORecvCompleted( HRESULT hrRes, IOBUFFER_READ *pIOBuffer );
	};








#include "NetServerUDP.inl"

} // namespace Net
} // namespace BR



