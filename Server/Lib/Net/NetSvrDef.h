////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Net server base type definitions. 
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Net/NetDef.h"
#include "Net/ConnectionManager.h"
#include "Common/ClassUtil.h"


namespace BR {
namespace Net {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Server network base interface
	//

	class ServerNet : public INet
	{
	private:
		
		// Socket handler
		SOCKET	m_socket;

		// local Address
		NetAddress m_LocalAddr;

		NetClass m_LocalClass;

		// Socket local address
		sockaddr_in6 m_LocalSockAddr;

		// Event queue
		PageQueue<INet::Event>	m_NetEventQueue;

		// Server ID
		BRCLASS_ATTRIBUTE_READONLY(ServerID,ServerID);

		// Enable new connection?
		BRCLASS_ATTRIBUTE_READONLY(bool,IsEnableAccept);

	protected:

		// Set local address
		inline void SetLocalAddress( const NetAddress& localAddr );

		// Set socket
		inline void SetSocket( SOCKET sock );

	public:

		ServerNet( ServerID InServerID, NetClass localClass );
		virtual ~ServerNet();

		// change status of new connection allowance 
		virtual void SetIsEnableAccept(bool bIsEnable);

		// Get Socket address
		inline const sockaddr_in6& GetSocketAddr();

		// Get local address
		inline const NetAddress& GetLocalAddress();

		inline NetClass GetLocalClass()							{ return m_LocalClass; }


		// Get Socket
		inline SOCKET GetSocket();


		// Add network event to queue
		virtual HRESULT EnqueueNetEvent( INet::Event& evt );

		// Query Network event
		virtual HRESULT DequeueNetEvent( Event& curEvent );

		virtual HRESULT Connect(IConnection* pIConn, UINT remoteID, NetClass netClass, const char *strDstIP, USHORT usDstPort) { return E_NOTIMPL; }

		// Close all connection
		virtual HRESULT CloseAllConnection() = 0;
	};


#include "NetSvrDef.inl"

} // namespace Net
} // namespace BR



