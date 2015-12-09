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
//#include "Common/HashTable.h"
//#include "Common/StaticHashTable.h"
#include "Common/OrderedLinkedList.h"
#include "Common/HashTable2.h"
#include "Common/Indexing.h"
#include "Common/SystemSynchronization.h"
#include "Net/NetDef.h"
#include "Net/NetServerPeer.h"
#include "Net/NetServerPeerTCP.h"
#include "ServerSystem/TaskManager.h"
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
		HRESULT AddServerEntity(NetClass netClass, ServerEntity* pServerEntity);


	public:
		// Constructor/Destructor
		ServerEntityManager();
		virtual ~ServerEntityManager();

		// Register server
		template<class ServerEntityType>
		HRESULT GetOrRegisterServer( ServerID serverID, NetClass netClass, const char *strIP, USHORT usPort, ServerEntityType* &pServerEntity );
		HRESULT GetOrRegisterServer(ServerID serverID, NetClass netClass, const char *strIP, USHORT usPort, ServerEntity* &pServerEntity);

		// Get remote entity
		HRESULT GetServerEntity( ServerID svrID, ServerEntity* &pServerEntity );

		// Get available firstEntity server entity
		HRESULT GetEntityManagerServerEntity( ServerEntity* &pServerEntity );

		// Add new remote entity
		HRESULT UpdateEntityManagerServerEntity( ServerEntity* pServerEntity );

		// Add new remote entity
		HRESULT AddOrGetServerEntity(ServerID serverID, NetClass netClass, ServerEntity* &pServerEntity);

		// GetServerPolicy
		template< class PolicyType >
		PolicyType* GetServerPolicy( ServerID svrID )
		{
			ServerEntity* pServerEntity = nullptr;
			if( FAILED(GetServerEntity(svrID, pServerEntity)) )
				return nullptr;

			return pServerEntity->GetPolicy<PolicyType>();
		}
	};


	
	// Register server
	template<class ServerEntityType>
	HRESULT ServerEntityManager::GetOrRegisterServer(ServerID serverID, NetClass netClass, const char *strIP, USHORT usPort, ServerEntityType* &pServerEntity)
	{
		HRESULT hr = S_OK;
		ServerEntityType *pNewServerEntity = nullptr;
		Net::IConnection *pConnection = nullptr;

		MutexScopeLock localLock(m_ServerTableLock);

		ServerEntity* pSvrEntity = nullptr;
		if (SUCCEEDED(GetServerEntity(serverID, pSvrEntity)))
		{
			AssertRel(netClass == pSvrEntity->GetRemoteClass());
			pServerEntity = dynamic_cast<ServerEntityType*>(pSvrEntity);
			Assert(pServerEntity);
			return hr;
		}

		svrChkPtr( strIP );

		svrTrace( Svr::TRC_ENTITY, "Registering Server {0} SvrID:{1}, {2}:{3}", typeid(ServerEntityType).name(), serverID, strIP, usPort );

		svrMem( pNewServerEntity = new ServerEntityType );

		svrChk( BrServer::GetInstance()->GetNetPrivate()->RegisterServerConnection( serverID, netClass, strIP, usPort, pConnection ) );

		pNewServerEntity->SetServerID( serverID );
		pNewServerEntity->SetLocalConnection(pConnection);

		pServerEntity = pNewServerEntity;

		svrChk(AddServerEntity(netClass, pNewServerEntity));

	Proc_End:

		return hr;
	}




}; // namespace Svr
}; // namespace BR





