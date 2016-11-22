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
#include "Net/Connection.h"
#include "Net/NetSystem.h"
#include "Net/ConnectionManager.h"
#include "Common/SpinSharedBuffer.h"
#include "Common/PageQueue.h"
#include "Net/ConnectionTCP.h"




namespace BR {
namespace Net {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Server base class
	//

	class Server : public ServerNet
	{
	private:


	protected:

	public:
		Server(ServerID InServerID, NetClass localClass);
		virtual ~Server();

		
		// check about initialize
		virtual bool IsReady();
	
		// Release instance
		virtual void Release();


		// Maximum connection
		virtual ConnectionManager& GetConnectionManager() = 0;

		virtual CounterType GetActiveConnectionCount() = 0;

		// Get connection from connection ID
		virtual Result GetConnection( uintptr_t uiCID, SharedPointerT<Connection> &pConn );



		// Open host and start listen
		virtual Result HostOpen( NetClass netCls, const char *strLocalIP, USHORT usLocalPort );

		// Close host and close all connections
		virtual Result HostClose();


		// Release Connection, Make connection to send free state
		virtual Result ReleaseConnection( IConnection* pIConnection ) override;

		// take over connection management
		virtual Result TakeOverConnection(IConnection* pIConnection) override;

		// Called when connection state changed
		virtual Result OnConnectionStateChange( IConnection *pConnection );

	};







	////////////////////////////////////////////////////////////////////////////////
	//
	//	TCP Server class
	//

	class ServerTCP : public Server, public INetIOCallBack
	{
	public:

	private:

		SyncCounter m_PendingAccept;

	protected:

		virtual Result SetupSocketOption(SOCKET socket);


	public:
		ServerTCP(ServerID InServerID, NetClass localClass);
		virtual ~ServerTCP();

		/////////////////////////////////////////////////////////////////////////////////////////////
		// callback


		virtual Result Accept(IOBUFFER_ACCEPT* &pAcceptInfo) override;
		// called when New connection TCP accepted
		virtual Result OnIOAccept( Result hrRes, IOBUFFER_ACCEPT *pAcceptInfo ) override;

		// called when network message is received
		virtual Result Recv(IOBUFFER_READ* pIOBuffer) override;
		virtual Result OnIORecvCompleted( Result hrRes, IOBUFFER_READ* &pIOBuffer ) override;

		virtual Result OnSendReady() override;
		// called when send completed
		virtual Result SendBuffer(IOBUFFER_WRITE *pSendBuffer) override;
		virtual Result OnIOSendCompleted( Result hrRes, IOBUFFER_WRITE *pIOBuffer ) override;

		// Pending Accept New one
		Result PendingAccept();

		// handle Socket accept
		virtual Result OnAcceptedSocket(SOCKET acceptedSocket, const sockaddr_storage& remoteSockAddr, const IConnection::ConnectionInformation& connectionInfo, IConnection* &pConnOut);

		// Get pending accept count
		inline CounterType GetPendingAccept();


		// Enable/Disable new connection acception
		virtual void SetIsEnableAccept( bool bIsEnable ) override;


		// Open host and start listen
		virtual Result HostOpen( NetClass netCls, const char *strLocalIP, USHORT usLocalPort ) override;

		// Close host and close all connections
		virtual Result HostClose() override;


		// Release Connection, Make connection to send free state
		virtual Result ReleaseConnection( IConnection* pIConnection ) override;


		//// Send message to connection with network device
		//virtual Result SendMsg( IConnection *pConnection, Message::MessageData *pMsg ) override;

		//// Send message to connection with network device to dst addr
		//virtual Result SendMsg( IConnection *pConnection, const sockaddr_in6& dstAddr, Message::MessageData *pMsg ) override;

		//// Send array of message buffer to connection with network device
		//virtual Result SendMsg( IConnection *pConnection, UINT uiBuffSize, BYTE* pBuff ) override;
	};



	class ServerPublicTCP : public ServerTCP
	{
	public:
		typedef ConnectionManagerT<ConnectionTCPServer> ConnectionManagerType;

	private:

		ConnectionManagerT<ConnectionTCPServer>	m_ConnectionManager;

	public:

		ServerPublicTCP(ServerID InServerID, NetClass localClass);
		virtual ~ServerPublicTCP();


		// Maximum connection
		inline virtual ConnectionManager& GetConnectionManager() override;

		// Get Active(non-free) connection
		inline virtual CounterType GetActiveConnectionCount() override;

		// Close all connection
		inline virtual Result CloseAllConnection() override;
	};


#include "NetServer.inl"

} // namespace Net
} // namespace BR



