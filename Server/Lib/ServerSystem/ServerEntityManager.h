////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Remote Entity Manager definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Common/Typedefs.h"
#include "Common/OrderedLinkedList.h"
#include "Common/HashTable2.h"
#include "Common/Indexing.h"
#include "Common/SystemSynchronization.h"
#include "Net/NetDef.h"
#include "Net/NetServerPeer.h"
#include "Net/NetServerPeerTCP.h"
#include "Common/Task/TaskManager.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/ServerComponent.h"



namespace BR {
namespace Svr
{


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Remote entity class
	//		- Will use with server type
	//

	class ServerEntityManager : public TaskManager, public IServerComponent
	{
	public:
		enum { ComponentID = ServerComponentID_ServerEntityManager };

		typedef Hash::HashTable2<ServerID, ServerEntity* > ServerIDMap;

		typedef OrderedLinkedList<ULONGLONG> ServerUpTimeList;

		// List node item
		struct ServerUpTimeListNodeItem : public ServerUpTimeList::Node
		{
			ServerEntity *pServerEntity;
		};



	private:

		// Server ID map for entity manager server
		ServerUpTimeList		m_EntityManagerServerUpTimeMap;

		// Server ID map
		ServerIDMap				m_ServerIDMap;

		BR::CriticalSection			m_ServerTableLock;

	private:
		Result AddServerEntity(NetClass netClass, ServerEntity* pServerEntity);


	public:
		// Constructor/Destructor
		ServerEntityManager();
		virtual ~ServerEntityManager();

		// Register server
		template<class ServerEntityType>
		Result GetOrRegisterServer( ServerID serverID, NetClass netClass, const NetAddress& netAddress, ServerEntityType* &pServerEntity );
		Result GetOrRegisterServer(ServerID serverID, NetClass netClass, const NetAddress& netAddress, ServerEntity* &pServerEntity);

		// Get remote entity
		Result GetServerEntity( ServerID svrID, ServerEntity* &pServerEntity );

		// Get available firstEntity server entity
		Result GetEntityManagerServerEntity( ServerEntity* &pServerEntity );

		// Add new remote entity
		Result UpdateEntityManagerServerEntity( ServerEntity* pServerEntity );

		// Add new remote entity
		Result AddOrGetServerEntity(ServerID serverID, NetClass netClass, ServerEntity* &pServerEntity);

		// GetServerPolicy
		template< class PolicyType >
		PolicyType* GetServerPolicy( ServerID svrID )
		{
			ServerEntity* pServerEntity = nullptr;
			if( !(GetServerEntity(svrID, pServerEntity)) )
				return nullptr;

			return pServerEntity->GetPolicy<PolicyType>();
		}
	};


	
	// Register server
	template<class ServerEntityType>
	Result ServerEntityManager::GetOrRegisterServer(ServerID serverID, NetClass netClass, const NetAddress& netAddress, ServerEntityType* &pServerEntity)
	{
		Result hr = ResultCode::SUCCESS;
		ServerEntityType *pNewServerEntity = nullptr;
		Net::ConnectionPtr pConnection = nullptr;

		MutexScopeLock localLock(m_ServerTableLock);

		ServerEntity* pSvrEntity = nullptr;
		if ((GetServerEntity(serverID, pSvrEntity)))
		{
			AssertRel(netClass == pSvrEntity->GetRemoteClass());
			pServerEntity = dynamic_cast<ServerEntityType*>(pSvrEntity);
			Assert(pServerEntity);
			return hr;
		}

		svrTrace( Svr::TRC_ENTITY, "Registering Server {0} SvrID:{1}, {2}", typeid(ServerEntityType).name(), serverID, netAddress);

		svrMem( pNewServerEntity = new ServerEntityType );

		svrChk( BrServer::GetInstance()->GetNetPrivate()->RegisterServerConnection( serverID, netClass, netAddress, pConnection ) );

		pNewServerEntity->SetServerID( serverID );
		pNewServerEntity->SetLocalConnection((Net::Connection*)pConnection);

		pServerEntity = pNewServerEntity;

		svrChk(AddServerEntity(netClass, pNewServerEntity));

	Proc_End:

		return hr;
	}




}; // namespace Svr
}; // namespace BR





