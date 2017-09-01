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


#include "stdafx.h"
#include "String/StrUtil.h"
#include "Util/TimeUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/Thread.h"
#include "ResultCode/SFResultCodeCommon.h"
#include "ServerSystem/SvrConstDefault.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/Transaction.h"
//#include "ServerSystem/PlugIn.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/EntityTable.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/SvrConst.h"
#include "Net/NetServerPeer.h"
#include "ServerSystem/ServiceEntity/ClusteredServiceEntity.h"



namespace BR {
namespace Svr
{


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Plugin entity class
	//


	ServerEntityManager::ServerEntityManager()
		:IServerComponent(ServerComponentID_ServerEntityManager)
	{
	}

	ServerEntityManager::~ServerEntityManager()
	{
	}

	Result ServerEntityManager::GetOrRegisterServer( ServerID serverID, NetClass netClass, const NetAddress& netAddress, ServerEntity* &pServerEntity )
	{
		Result hr = ResultCode::SUCCESS;
		ServerEntity *pNewServerEntity = nullptr;
		Net::ConnectionPtr pConnection = nullptr;

		MutexScopeLock localLock(m_ServerTableLock);

		ServerEntity* pSvrEntity = nullptr;
		if ((GetServerEntity(serverID, pSvrEntity)))
		{
			AssertRel(netClass == pSvrEntity->GetRemoteClass());
			pServerEntity = pSvrEntity;
			return hr;
		}

		Assert(netClass != NetClass::Unknown);

		svrTrace( Svr::TRC_ENTITY, "Registering Server {0} SvrID:{1} {2}", netClass, serverID, netAddress);

		svrChk( BrServer::GetInstance()->CreateServerEntity(netClass, pNewServerEntity) );

		pNewServerEntity->SetPrivateNetAddress(netAddress);
		pNewServerEntity->SetServerID( serverID );

		pServerEntity = pNewServerEntity;

		svrChk(AddServerEntity(netClass, pNewServerEntity));

		svrChk(BrServer::GetInstance()->GetNetPrivate()->RegisterServerConnection(serverID, netClass, netAddress, pConnection));

		pNewServerEntity->SetLocalConnection((Net::Connection*)pConnection);

		svrTrace(Svr::TRC_ENTITY, "Registered Server {0} SvrID:{1} {2}, taskGrp:{3}", netClass, serverID, netAddress, pNewServerEntity->GetTaskGroupID());


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

	Result ServerEntityManager::UpdateEntityManagerServerEntity( ServerEntity* pServerEntity )
	{
		Result hr = ResultCode::SUCCESS;
		ServerUpTimeList::Node *pPrevNode = nullptr;
		ServerUpTimeListNodeItem *pPrevListNode = nullptr;
		ServerUpTimeListNodeItem *pNewListNode = nullptr;
		ServerUpTimeList::iterator itEntity;

		svrChkPtr( pServerEntity );

		// Find in the list and remove if it exists
		for( itEntity = m_EntityManagerServerUpTimeMap.begin(); itEntity.IsValid(); ++itEntity )
		{
			ServerUpTimeListNodeItem *pNode = (ServerUpTimeListNodeItem*)&*itEntity;
			if( pNode->pServerEntity == pServerEntity )
			{
				svrChk( m_EntityManagerServerUpTimeMap.Remove( itEntity ) );
				break;
			}
		}

		svrChk( m_EntityManagerServerUpTimeMap.FindPrevNode( pServerEntity->GetServerUpTime().time_since_epoch().count(), pPrevNode ) );

		pPrevListNode = (ServerUpTimeListNodeItem*)pPrevNode;
		if( pPrevNode->Key != 0 && pPrevListNode->pServerEntity == pServerEntity )
			return ResultCode::SUCCESS_FALSE;

		svrMem( pNewListNode = new ServerUpTimeListNodeItem );
		memset( pNewListNode, 0, sizeof(ServerUpTimeListNodeItem) );

		pNewListNode->pServerEntity = pServerEntity;

		//pNewListNode
		svrChk( m_EntityManagerServerUpTimeMap.Insert( pPrevNode, pServerEntity->GetServerUpTime().time_since_epoch().count(), pNewListNode ) );

		pNewListNode = nullptr;

	Proc_End:

		Util::SafeDelete( pNewListNode );

		return hr;
	}

	// Add new connection
	Result ServerEntityManager::AddServerEntity(NetClass netClass, ServerEntity* pServerEntity)
	{
		Result hr = ResultCode::SUCCESS, res;
		auto& entityTable = GetEntityTable();
		svrChkPtr( pServerEntity );

		if (netClass < NetClass::Unknown
			|| netClass >= BR::NetClass::Max)
		{
			return ResultCode::INVALID_ARG;
		}

		svrChk( pServerEntity->InitializeEntity(entityTable.GenEntityID(EntityFaculty::Server) ) );

		// Add to task list
		svrChk( AddTickTask( pServerEntity ) );

		svrChk(entityTable.Insert(pServerEntity));

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
			|| netClass >= BR::NetClass::Max)
		{
			return ResultCode::INVALID_ARG;
		}

		ServerEntity* pOldEntity = nullptr;
		if ((GetServerEntity(serverID, pOldEntity)))
		{
			AssertRel(netClass == pOldEntity->GetRemoteClass());
			if (pServerEntity != nullptr && pOldEntity != pServerEntity)
			{
				svrTrace(Svr::TRC_ENTITY, "Adding Duplicated Server {0} SvrID:{1}", netClass, serverID);
				AssertRel(false);
				return ResultCode::UNEXPECTED;
			}

			pServerEntity = pOldEntity;
			return hr;
		}

		Assert(netClass != NetClass::Unknown);

		svrTrace(Svr::TRC_ENTITY, "Adding Server {0} SvrID:{1}", netClass, serverID);

		if (pServerEntity == nullptr)
		{
			svrChk(BrServer::GetInstance()->CreateServerEntity(netClass, pServerEntity));
			pServerEntity->SetServerID(serverID);
		}

		svrChk(AddServerEntity(netClass, pServerEntity));


	Proc_End:


		return hr;
	}




}; // namespace Svr
}; // namespace BR



