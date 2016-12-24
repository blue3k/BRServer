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
		virtual Result TerminateManager();
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
		inline Result EnqueueNetEvent( INet::Event& evt ) override;

		virtual ConnectionManager& GetConnectionManager() = 0;

	public:
		Client( UINT uiConMapBucketSize );
		virtual ~Client();

		// Get connection by connection ID
		virtual Result GetConnection(uintptr_t uiCID, SharedPointerT<IConnection> &pIConnection) override;

		// Query Network event
		virtual Result DequeueNetEvent( Event& curEvent ) override;


		// Called when connection result 
		virtual void OnConnectionResult( Connection *pConnection, Result hrConnect );

		virtual Result ConnectCli(const NetAddress& destAddress, IConnection* &pNewConnection) override;

		// take over connection management
		virtual Result TakeOverConnection(IConnection* pIConnection) override;

		// Release Connection
		virtual Result ReleaseConnection(IConnection* pIConnection) override;

		// Release instance
		virtual void Release() override;
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
		inline ConnectionManager& GetConnectionManager() override;

		// check about initialize
		virtual bool IsReady() override;

		// Connect to server
		virtual Result Connect(IConnection* pIConn, UINT remoteID, NetClass netClass, const NetAddress& destAddress) override;

		// Called when connection state changed
		Result OnConnectionStateChange( IConnection *pConnection );
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
		inline ConnectionManager& GetConnectionManager() override;

		// check about initialize
		virtual bool IsReady() override;

		// Connect to server
		virtual Result Connect(IConnection* pIConn, UINT remoteID, NetClass netClass, const NetAddress& destAddress) override;

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
		ConnectionManager& GetConnectionManager() override { return m_ConnectionManager; }

		// check about initialize
		virtual bool IsReady() override;

		// Connect to server
		virtual Result Connect(IConnection* pIConn, UINT remoteID, NetClass netClass, const NetAddress& destAddress) override;

	};



#include "NetClient.inl"

} // namespace Net
} // namespace BR



