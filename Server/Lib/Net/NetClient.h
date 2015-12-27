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
#include "Net/ConnectionMUDP.h"
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
		typedef ConnectionManagerT<ConnectionType> super;

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

		virtual ConnectionManager& GetConnectionManager() = 0;

	public:
		Client( UINT uiConMapBucketSize );
		virtual ~Client();

		// Get connection by connection ID
		virtual HRESULT GetConnection(uintptr_t uiCID, SharedPointerT<IConnection> &pIConnection) override;

		// Query Network event
		virtual HRESULT DequeueNetEvent( Event& curEvent );


		// Called when connection result 
		virtual void OnConnectionResult( Connection *pConnection, HRESULT hrConnect );

		virtual HRESULT ConnectCli(const NetAddress& destAddress, IConnection* &pNewConnection) override;

		// take over connection management
		virtual HRESULT TakeOverConnection(IConnection* pIConnection) override;

		// Release Connection
		virtual HRESULT ReleaseConnection(IConnection* pIConnection) override;

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
		virtual HRESULT Connect(IConnection* pIConn, UINT remoteID, NetClass netClass, const NetAddress& destAddress) override;

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
		virtual HRESULT Connect(IConnection* pIConn, UINT remoteID, NetClass netClass, const NetAddress& destAddress) override;

	};



	////////////////////////////////////////////////////////////////////////////////
	//
	//	MUDP Client class
	//

	class ClientMUDP : public Client
	{
	private:

		ClientConnectionManagerT<ConnectionMUDPClient>	m_ConnectionManager;

	public:
		ClientMUDP();
		virtual ~ClientMUDP();

		// Get connection manager
		ConnectionManager& GetConnectionManager() { return m_ConnectionManager; }

		// check about initialize
		virtual bool IsReady() override;

		// Connect to server
		virtual HRESULT Connect(IConnection* pIConn, UINT remoteID, NetClass netClass, const NetAddress& destAddress) override;

	};



#include "NetClient.inl"

} // namespace Net
} // namespace BR



