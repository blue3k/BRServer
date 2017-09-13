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
		inline const SharedPointerAtomicT<SF::Net::Connection>& ServerEntity::GetConnection()
		{
			// I can't use SharedPointerT for return.
			// Even though I use cache variable, if GetConnection is called over different thread, the SharedPointer can be screwred.
			auto pConn = *m_pConnLocal;
			if (pConn == nullptr || pConn->GetConnectionState() == Net::ConnectionState::DISCONNECTED)
				return m_pConnRemote;

			return m_pConnLocal;
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
