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
#include "Net/NetCtrl.h"
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
		ClientConnectionManagerT() : ConnectionManagerT<ConnectionType>(5) {}

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
		PageQueue<INet::Event>	m_NetEventQueue;


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
		virtual HRESULT GetConnection( uintptr_t uiCID, SharedPointerT<Connection> &pIConnection );

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
		virtual HRESULT GetConnection(uintptr_t uiCID, SharedPointerT<Connection> &pIConnection);

	};



#include "NetClient.inl"

} // namespace Net
} // namespace BR



