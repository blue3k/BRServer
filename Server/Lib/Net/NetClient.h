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


#pragma once

#include "Net/NetDef.h"
#include "Net/NetMessage.h"
#include "Net/NetConnectionTask.h"
#include "Net/ConnectionManager.h"
#include "Net/ConnectionUDP.h"
#include "Net/ConnectionTCP.h"


namespace BR {
namespace Net {

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ConnectionManagerT
	//

	template< class ConnectionType >
	class ClientConnectionManagerT : public ConnectionManagerT<ConnectionType>
	{
	private:

	public:
		ClientConnectionManagerT() : ConnectionManagerT(5) {}

		// Release all connection and terminate manager
		virtual HRESULT TerminateManager();
	};
	

	

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Client base class
	//

	class Client : public IClient
	{
	public :

	private:

		// Event queue
		BR::PageQueue<INet::Event>	m_NetEventQueue;


	protected:

		// Add network event to queue
		inline HRESULT EnqueueNetEvent( INet::Event& evt );

	public:
		Client( UINT uiConMapBucketSize );
		virtual ~Client();

		// Query Network event
		virtual HRESULT DequeueNetEvent( Event& curEvent );


		// Called when connection result 
		virtual void OnConnectionResult( Connection *pConnection, HRESULT hrConnect );

		// Release instance
		virtual void Release();
	};




	////////////////////////////////////////////////////////////////////////////////
	//
	//	TCP Client class
	//

	class ClientTCP : public Client
	{
	private:

		ClientConnectionManagerT<ConnectionTCPClient>	m_ConnectionManager;

	public:
		ClientTCP();
		virtual ~ClientTCP();

		// Get connection manager
		inline ConnectionManager& GetConnectionManager();

		// check about initialize
		virtual bool IsReady();

		// Connect to server
		virtual HRESULT Connect( const char *strServerIP, USHORT usServerPort, IConnection* &pNewConnection );

		// Release Connection
		virtual HRESULT ReleaseConnection( IConnection* pIConnection ) override;

		// take over connection management
		virtual HRESULT TakeOverConnection(IConnection* pIConnection) override;

		// Get connection from connection ID
		virtual HRESULT GetConnection( UINT_PTR uiCID, SharedPointerT<Connection> &pIConnection );

		// Send message to connection with network device
		virtual HRESULT SendMsg( IConnection *pConnection, Message::MessageData *pMsg );

		// Send message to connection with network device to dst addr
		virtual HRESULT SendMsg( IConnection *pConnection, const sockaddr_in6& dstAddr, Message::MessageData *pMsg );

		// Send array of message buffer to connection with network device
		virtual HRESULT SendMsg( IConnection *pConnection, UINT uiBuffSize, BYTE* pBuff );

		// Called when connection state changed
		HRESULT OnConnectionStateChange( IConnection *pConnection );
	};


	////////////////////////////////////////////////////////////////////////////////
	//
	//	UDP Client class
	//

	class ClientUDP : public Client
	{
	private:

		ClientConnectionManagerT<ConnectionUDPClient>	m_ConnectionManager;

	public:
		ClientUDP();
		virtual ~ClientUDP();
		
		// Get connection manager
		inline ConnectionManager& GetConnectionManager();

		// check about initialize
		virtual bool IsReady();

		// Connect to server
		virtual HRESULT Connect( const char *strServerIP, USHORT usServerPort, IConnection* &pNewConnection );
		

		// Release Connection
		virtual HRESULT ReleaseConnection( IConnection* pIConnection );
		
		// Get connection from connection ID
		virtual HRESULT GetConnection(UINT_PTR uiCID, SharedPointerT<Connection> &pIConnection);

		// Send message to connection with network device
		virtual HRESULT SendMsg( IConnection *pConnection, Message::MessageData *pMsg );

		// Send message to connection with network device to dst addr
		virtual HRESULT SendMsg( IConnection *pConnection, const sockaddr_in6& dstAddr, Message::MessageData *pMsg );

		// Send array of message buffer to connection with network device
		virtual HRESULT SendMsg( IConnection *pConnection, UINT uiBuffSize, BYTE* pBuff );

		// Pending recv New one
		//HRESULT PendingRecv( ConnectionUDPClient *pConnection );
	};



#include "NetClient.inl"

} // namespace Net
} // namespace BR



