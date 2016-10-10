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
		sockaddr_storage m_LocalSockAddr;

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
		inline const sockaddr_storage& GetSocketAddr();

		int GetSocketAddrSize() { return m_LocalSockAddr.ss_family == AF_INET6 ? (int)sizeof(sockaddr_in6) : (int)sizeof(sockaddr_in); }

		// Get local address
		inline const NetAddress& GetLocalAddress();

		inline NetClass GetLocalClass()							{ return m_LocalClass; }


		// Get Socket
		const SOCKET& GetSocket();


		// Add network event to queue
		virtual Result EnqueueNetEvent( INet::Event& evt );

		// Query Network event
		virtual Result DequeueNetEvent( Event& curEvent );

		virtual Result Connect(IConnection* pIConn, UINT remoteID, NetClass netClass, const NetAddress& destAddress) { return ResultCode::NOT_IMPLEMENTED; }

		// Close all connection
		virtual Result CloseAllConnection() = 0;
	};


#include "NetSvrDef.inl"

} // namespace Net
} // namespace BR



