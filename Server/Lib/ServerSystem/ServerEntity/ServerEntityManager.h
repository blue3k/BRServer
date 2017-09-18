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
#include "Object/SharedObject.h"
#include "Container/OrderedLinkedList.h"
#include "Container/HashTable2.h"
#include "Container/Indexing.h"
#include "Thread/SystemSynchronization.h"
#include "Net/NetDef.h"
#include "Net/NetServerPeerTCP.h"
#include "Task/ServerTaskManager.h"
#include "ServerEntity/ServerEntity.h"
#include "Component/ServerComponent.h"
#include "Service/ServerEntityManagerService.h"


namespace SF {
namespace Svr
{


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Remote entity class
	//		- Will use with server type
	//

	class ServerEntityManager : public TickTaskManager, public ServerEntityManagerService
	{
	public:

		static constexpr FixedString TypeName = "ServerEntityManager";

		typedef HashTable2<ServerID, ServerEntity* > ServerIDMap;

		typedef OrderedLinkedList<uint64_t> ServerUpTimeList;

		// List node item
		struct ServerUpTimeListNodeItem : public ServerUpTimeList::Node
		{
			ServerEntity *pServerEntity;
		};



	private:

		MemoryManager m_MemoryManager;

		uint m_ThreadCount = 2;

		// Server ID map for entity manager server
		ServerUpTimeList		m_EntityManagerServerUpTimeMap;

		// Server ID map
		ServerIDMap				m_ServerIDMap;

		CriticalSection			m_ServerTableLock;

	private:
		Result AddServerEntity(NetClass netClass, ServerEntity* pServerEntity);


	public:
		// Constructor/Destructor
		ServerEntityManager(uint threadCount);
		virtual ~ServerEntityManager();

		virtual void Dispose() override;

		IMemoryManager& GetHeap() { return m_MemoryManager; }

		// Clear all server entity
		virtual void Clear() override;

		// Register server
		template<class ServerEntityType>
		Result GetOrRegisterServer( ServerID serverID, NetClass netClass, const NetAddress& netAddress, ServerEntityType* &pServerEntity );
		virtual Result GetOrRegisterServer(ServerID serverID, NetClass netClass, const NetAddress& netAddress, ServerEntity* &pServerEntity) override;

		// Get remote entity
		virtual Result GetServerEntity( ServerID svrID, ServerEntity* &pServerEntity ) override;

		// Get available firstEntity server entity
		virtual Result GetEntityManagerServerEntity( ServerEntity* &pServerEntity ) override;

		// Add new remote entity
		virtual Result UpdateEntityManagerServerEntity( ServerEntity* pServerEntity ) override;

		// Add new remote entity
		virtual Result AddOrGetServerEntity(ServerID serverID, NetClass netClass, ServerEntity* &pServerEntity) override;

		virtual const SharedPointerAtomicT<Net::Connection>& GetServerConnection(ServerID svrID) override;

		Result InitializeComponent() { return TickTaskManager::InitializeManager(m_ThreadCount); }
		// Terminate component
		void DeinitializeComponent() { TickTaskManager::TerminateManager(); }
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
			pServerEntity = dynamic_cast<ServerEntityType*>(pSvrEntity);
			Assert(pServerEntity);
			return hr;
		}

		svrTrace( SVR_ENTITY, "Registering Server {0} SvrID:{1}, {2}", typeid(ServerEntityType).name(), serverID, netAddress);

		svrMem( pNewServerEntity = new(GetHeap()) ServerEntityType );

		svrChk( BrServer::GetInstance()->GetNetPrivate()->RegisterServerConnection( serverID, netClass, netAddress, pConnection ) );

		pNewServerEntity->SetServerID( serverID );
		pNewServerEntity->SetLocalConnection((Net::Connection*)pConnection);

		pServerEntity = pNewServerEntity;
		pNewServerEntity = nullptr;

		svrChk(AddServerEntity(netClass, pServerEntity));

	Proc_End:

		IHeap::Delete(pNewServerEntity);

		if (!hr)
		{
			SharedReferenceDec dec(pServerEntity);
			pServerEntity = nullptr;
		}

		return hr;
	}




}; // namespace Svr
}; // namespace SF





