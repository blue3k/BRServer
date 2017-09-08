////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Remote Entity inline implementation
//	
//
////////////////////////////////////////////////////////////////////////////////

namespace SF {
	namespace Svr {

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Remote Entity class
		//





		// return about initial connection or not
		bool ServerEntity::IsInitialConnection()
		{
			return m_bIsInitialConnect;
		}

		// Get Connection
		inline SharedPointerT<SF::Net::Connection> ServerEntity::GetConnection()
		{
			SharedPointerT<SF::Net::Connection> pConn;
			pConn = m_pConnLocal;
			if (pConn == nullptr || pConn->GetConnectionState() == Net::ConnectionState::DISCONNECTED)
				pConn = m_pConnRemote;

			return std::forward<SharedPointerT<SF::Net::Connection>>(pConn);
		}




		// Get/Set Class Name
		ServerEntity::ServerEntityClass ServerEntity::GetRemoteClass() const
		{
			auto pConn = *m_pConnLocal;
			if (pConn)
				return pConn->GetRemoteInfo().PeerClass;

			pConn = *m_pConnRemote;
			if (pConn)
				return pConn->GetRemoteInfo().PeerClass;

			return NetClass::Unknown;
		}

		// Get public net address
		inline const NetAddress& ServerEntity::GetPublicNetAddress() const
		{
			return m_NetPublic;
		}

		inline void ServerEntity::SetPublicNetAddress(const NetAddress& netAddr)
		{
			m_NetPublic = netAddr;
		}

		inline const NetAddress& ServerEntity::GetPrivateNetAddress() const
		{
			return m_NetPrivate;
		}

		inline void ServerEntity::SetPrivateNetAddress(const NetAddress& netAddr)
		{
			m_NetPrivate = netAddr;
		}


		// Get ServerID
		inline ServerID ServerEntity::GetServerID() const
		{
			return m_ServerID;
		}

		inline void ServerEntity::SetServerID(ServerID svrID)
		{
			m_ServerID = svrID;
		}



	}
}
