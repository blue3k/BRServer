////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Remote Entity Manager implementation
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"
#include "String/SFStrUtil.h"
#include "Util/SFTimeUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/SFThread.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "SvrConst.h"
#include "ServerEntity/ServerEntityManager.h"
#include "Transaction/Transaction.h"
//#include "ServerSystem/PlugIn.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Entity/EntityTable.h"
#include "SvrTrace.h"
#include "SvrConst.h"
#include "Net/SFNetServerPeerTCP.h"
#include "ServiceEntity/ClusteredServiceEntity.h"
#include "Service/ServerService.h"



namespace SF {
namespace Svr
{


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ServerEntityManager class
	//

	constexpr FixedString ServerEntityManager::TypeName;

	ServerEntityManager::ServerEntityManager(uint threadCount)
		: m_Heap("ServerEntityManager", GetSystemHeap())
		, m_ThreadCount(threadCount)
		, m_ServerIDMap(GetHeap())
	{
		Service::ServerEntityManager = this;
	}

	ServerEntityManager::~ServerEntityManager()
	{
	}

	void ServerEntityManager::Dispose()
	{
		Service::ServerEntityManager = nullptr;

		Clear();
	}

	void ServerEntityManager::Clear()
	{
		m_ServerIDMap.for_each([this](ServerID, ServerEntity* pSvrEntity)
		{
			Service::EntityManager->RemoveEntity(pSvrEntity);
			return true;
		});
		Service::EntityManager->FlushDeletedEntity();
		m_ServerIDMap.Clear();
	}

	Result ServerEntityManager::GetOrRegisterServer( ServerID serverID, NetClass netClass, const NetAddress& netAddress, ServerEntity* &pServerEntity )
	{
		Result hr = ResultCode::SUCCESS;
		ServerEntity *pNewServerEntity = nullptr;
		Net::ConnectionPtr pConnection;

		MutexScopeLock localLock(m_ServerTableLock);

		ServerEntity* pSvrEntity = nullptr;
		if ((GetServerEntity(serverID, pSvrEntity)))
		{
			pServerEntity = pSvrEntity;
			return hr;
		}

		Assert(netClass != NetClass::Unknown);

		svrTrace( SVR_ENTITY, "Registering Server {0} SvrID:{1} {2}", netClass, serverID, netAddress);

		svrChk( BrServer::GetInstance()->CreateServerEntity(netClass, pNewServerEntity) );

		pNewServerEntity->SetPrivateNetAddress(netAddress);
		pNewServerEntity->SetServerID( serverID );

		pServerEntity = pNewServerEntity;

		svrChk(AddServerEntity(netClass, pNewServerEntity));

		svrChk(BrServer::GetInstance()->GetNetPrivate()->RegisterServerConnection(serverID, netClass, netAddress, pConnection));

		pNewServerEntity->SetLocalConnection((Net::Connection*)pConnection);

		svrTrace(SVR_ENTITY, "Registered Server {0} SvrID:{1} {2}, taskGrp:{3}", netClass, serverID, netAddress, pNewServerEntity->GetTaskGroupID());


	Proc_End:

		return hr;
	}



	// Get remote entity
	Result ServerEntityManager::GetServerEntity( ServerID svrID, ServerEntity* &pServerEntity )
	{
		Result hr = ResultCode::SUCCESS;

		hr = m_ServerIDMap.Find(svrID, pServerEntity);

	//Proc_End:

		Assert(!(hr) || pServerEntity != nullptr);
		return hr;
	}



	Result ServerEntityManager::GetEntityManagerServerEntity( ServerEntity* &pServerEntity )
	{
		Result hr = ResultCode::SUCCESS;

		// Get first non-zero one, This should be the biggest one
		ServerUpTimeList::iterator itEntity = m_EntityManagerServerUpTimeMap.begin();
		for( ; itEntity.IsValid(); ++itEntity )
		{
			ServerUpTimeListNodeItem *pNode = (ServerUpTimeListNodeItem*)&*itEntity;
			if( pNode->pServerEntity != nullptr )
			{
				pServerEntity = pNode->pServerEntity;
				break;
			}
		}

	//Proc_End:

		return pServerEntity != nullptr ? hr : ResultCode::FAIL;
	}

	// Add new connection
	Result ServerEntityManager::AddServerEntity(NetClass netClass, ServerEntity* pServerEntity)
	{
		Result hr = ResultCode::SUCCESS, res;
		svrChkPtr( pServerEntity );

		if (netClass < NetClass::Unknown
			|| netClass >= NetClass::Max)
		{
			return ResultCode::INVALID_ARG;
		}

		svrChk( pServerEntity->InitializeEntity(Service::EntityTable->GenEntityID(EntityFaculty::Server) ) );

		// Add to task list
		svrChk( AddTickTask( pServerEntity ) );

		svrChk(Service::EntityTable->insert(pServerEntity));

		svrChk(m_ServerIDMap.Insert(pServerEntity->GetServerID(), pServerEntity));

		pServerEntity = nullptr;

	Proc_End:

		if( pServerEntity )
		{
			RemoveTickTask( pServerEntity );
		}

		return hr;
	}

	Result ServerEntityManager::AddOrGetServerEntity(ServerID serverID, NetClass netClass, ServerEntity* &pServerEntity)
	{
		Result hr = ResultCode::SUCCESS;

		MutexScopeLock localLock(m_ServerTableLock);

		if (netClass < NetClass::Unknown
			|| netClass >= NetClass::Max)
		{
			return ResultCode::INVALID_ARG;
		}

		ServerEntity* pOldEntity = nullptr;
		if ((GetServerEntity(serverID, pOldEntity)))
		{
			if (pServerEntity != nullptr && pOldEntity != pServerEntity)
			{
				svrTrace(SVR_ENTITY, "Adding Duplicated Server {0} SvrID:{1}", netClass, serverID);
				AssertRel(false);
				return ResultCode::UNEXPECTED;
			}

			pServerEntity = pOldEntity;
			return hr;
		}

		Assert(netClass != NetClass::Unknown);

		svrTrace(SVR_ENTITY, "Adding Server {0} SvrID:{1}", netClass, serverID);

		if (pServerEntity == nullptr)
		{
			svrChk(BrServer::GetInstance()->CreateServerEntity(netClass, pServerEntity));
			pServerEntity->SetServerID(serverID);
		}

		svrChk(AddServerEntity(netClass, pServerEntity));


	Proc_End:


		return hr;
	}

	const SharedPointerAtomicT<Net::Connection>& ServerEntityManager::GetServerConnection(ServerID svrID)
	{
		const static SharedPointerAtomicT<Net::Connection> Dummy;
		ServerEntity* pServerEntity = nullptr;

		if (!GetServerEntity(svrID, pServerEntity))
			return Dummy;

		return pServerEntity->GetConnection();
	}


}; // namespace Svr
}; // namespace SF



