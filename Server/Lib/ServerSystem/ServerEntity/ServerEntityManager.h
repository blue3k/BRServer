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


#include "SFTypedefs.h"
#include "Container/OrderedLinkedList.h"
#include "Container/HashTable2.h"
#include "Container/Indexing.h"
#include "Thread/SystemSynchronization.h"
#include "Net/NetDef.h"
#include "Net/NetServerPeerTCP.h"
#include "Task/ServerTaskManager.h"
#include "ServerEntity/ServerEntity.h"
#include "Component/ServerComponent.h"



namespace SF {
namespace Svr
{


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Remote entity class
	//		- Will use with server type
	//

	class ServerEntityManager : public TickTaskManager, public IServerComponent
	{
	public:
		enum { ComponentID = ServerComponentID_ServerEntityManager };

		typedef HashTable2<ServerID, ServerEntity* > ServerIDMap;

		typedef OrderedLinkedList<uint64_t> ServerUpTimeList;

		// List node item
		struct ServerUpTimeListNodeItem : public ServerUpTimeList::Node
		{
			ServerEntity *pServerEntity;
		};



	private:

		MemoryManager m_MemoryManager;

		// Server ID map for entity manager server
		ServerUpTimeList		m_EntityManagerServerUpTimeMap;

		// Server ID map
		ServerIDMap				m_ServerIDMap;

		CriticalSection			m_ServerTableLock;

	private:
		Result AddServerEntity(NetClass netClass, ServerEntity* pServerEntity);


	public:
		// Constructor/Destructor
		ServerEntityManager();
		virtual ~ServerEntityManager();

		IMemoryManager& GetMemoryManager() { return m_MemoryManager; }

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

			return pServerEntity->GetInterface<PolicyType>();
		}
	};


	
	// Register server
	template<class ServerEntityType>
	Result ServerEntityManager::GetOrRegisterServer(ServerID serverID, NetClass netClass, const NetAddress& netAddress, ServerEntityType* &pServerEntity)
	{
		Result hr = ResultCode::SUCCESS;
		ServerEntityType *pNewServerEntity = nullptr;
		Net::ConnectionPtr pConnection;

		MutexScopeLock localLock(m_ServerTableLock);

		ServerEntity* pSvrEntity = nullptr;
		if ((GetServerEntity(serverID, pSvrEntity)))
		{
			AssertRel(netClass == pSvrEntity->GetRemoteClass());
			pServerEntity = dynamic_cast<ServerEntityType*>(pSvrEntity);
			Assert(pServerEntity);
			return hr;
		}

		svrTrace( SVR_ENTITY, "Registering Server {0} SvrID:{1}, {2}", typeid(ServerEntityType).name(), serverID, netAddress);

		svrMem( pNewServerEntity = new(GetMemoryManager()) ServerEntityType );

		svrChk( BrServer::GetInstance()->GetNetPrivate()->RegisterServerConnection( serverID, netClass, netAddress, pConnection ) );

		pNewServerEntity->SetServerID( serverID );
		pNewServerEntity->SetLocalConnection((Net::Connection*)pConnection);

		pServerEntity = pNewServerEntity;

		svrChk(AddServerEntity(netClass, pNewServerEntity));

	Proc_End:

		return hr;
	}




}; // namespace Svr
}; // namespace SF





