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

#include "SFTypedefs.h"
#include "Thread/Thread.h"
#include "Net/NetDef.h"
#include "Net/NetSvrDef.h"
#include "Net/ConnectionUDP.h"
#include "Net/ConnectionMUDP.h"
#include "Net/NetServer.h"
#include "Net/ConnectionManager.h"
#include "Common/SpinSharedBuffer.h"
#include "Container/PageQueue.h"




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
		virtual CounterType GetActiveConnectionCount() override;

		// Close all connection
		virtual Result CloseAllConnection() override;


		/////////////////////////////////////////////////////////////////////////////////////////////
		// callback
		// called when New connection TCP accepted


		// called when network message is received
		virtual Result Recv(IOBUFFER_READ* pIOBuffer) override;

		virtual Result OnSendReady() override;
		// called when send completed
		virtual Result SendBuffer(IOBUFFER_WRITE *pSendBuffer) override;
		virtual Result OnIOSendCompleted( Result hrRes, IOBUFFER_WRITE *pIOBuffer ) override;


		// Open host and start listen
		virtual Result HostOpen( NetClass netCls, const char *strLocalIP, USHORT usLocalPort ) override;

		// Close host and close all connections
		virtual Result HostClose() override;



		//// Send message to connection with network device
		//virtual Result SendMsg( IConnection *pConnection, Message::MessageData *pMsg );

		//// Send message to connection with network device to dst addr
		//virtual Result SendMsg( IConnection *pConnection, const sockaddr_storage& dstAddr, Message::MessageData *pMsg );

		//// Send array of message buffer to connection with network device
		//virtual Result SendMsg( IConnection *pConnection, UINT uiBuffSize, uint8_t* pBuff );

		// Pending recv New one
		Result PendingRecv( IOBUFFER_READ *pOverlaped );

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
		virtual Result OnIORecvCompleted( Result hrRes, IOBUFFER_READ* &pIOBuffer );
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

		Result SendRaw(const sockaddr_storage& dstAddress, Message::MessageData* &pMsg);
		Result SendNetCtrl( const sockaddr_storage& dstAddress, UINT uiCtrlCode, UINT uiSequence, Message::MessageID msgID, uint64_t UID );
		
		// Maximum connection
		inline ConnectionManager& GetConnectionManager();

		// Register PeerID to map

		Result OnNoConnectionPacket(const struct sockaddr_storage& from, const uint8_t* pData);

		// called when network message is received
		virtual Result OnIORecvCompleted( Result hrRes, IOBUFFER_READ* &pIOBuffer );
	};








#include "NetServerUDP.inl"

} // namespace Net
} // namespace BR



