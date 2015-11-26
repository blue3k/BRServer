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
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "Common/HRESCommon.h"
#include "ServerSystem/SvrConstDefault.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/Transaction.h"
//#include "ServerSystem/PlugIn.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/BrServerUtil.h"
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

	HRESULT ServerEntityManager::GetOrRegisterServer( ServerID serverID, NetClass netClass, const char *strIP, USHORT usPort, ServerEntity* &pServerEntity )
	{
		HRESULT hr = S_OK;
		ServerEntity *pNewServerEntity = nullptr;
		Net::IConnection *pConnection = nullptr;

		MutexScopeLock localLock(m_ServerTableLock);

		ServerEntity* pSvrEntity = nullptr;
		if (SUCCEEDED(GetServerEntity(serverID, pSvrEntity)))
		{
			AssertRel(netClass == pSvrEntity->GetRemoteClass());
			pServerEntity = pSvrEntity;
			return hr;
		}

		svrChkPtr( strIP );

		Assert(netClass != NetClass::Unknown);

		svrTrace( Svr::TRC_ENTITY, "Registering Server %0% SvrID:%1% %2%:%3%", netClass, serverID, strIP, usPort );

		svrChk( BrServer::GetInstance()->CreateServerEntity(netClass, pNewServerEntity) );

		svrChk( BrServer::GetInstance()->GetNetPrivate()->RegisterServerConnection( serverID, netClass, strIP, usPort, pConnection ) );

		pNewServerEntity->SetPrivateNetAddress(NetAddress(strIP, usPort));
		pNewServerEntity->SetServerID( serverID );
		pNewServerEntity->SetLocalConnection(pConnection);

		pServerEntity = pNewServerEntity;

		svrChk(AddServerEntity(netClass, pNewServerEntity));

	Proc_End:

		return hr;
	}



	// Get remote entity
	HRESULT ServerEntityManager::GetServerEntity( ServerID svrID, ServerEntity* &pServerEntity )
	{
		HRESULT hr = S_OK;

		hr = m_ServerIDMap.Find(svrID, pServerEntity);

	Proc_End:

		Assert(FAILED(hr) || pServerEntity != nullptr);
		return hr;
	}



	HRESULT ServerEntityManager::GetEntityManagerServerEntity( ServerEntity* &pServerEntity )
	{
		HRESULT hr = S_OK;

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

	Proc_End:

		return pServerEntity != nullptr ? hr : E_FAIL;
	}

	HRESULT ServerEntityManager::UpdateEntityManagerServerEntity( ServerEntity* pServerEntity )
	{
		HRESULT hr = S_OK;
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
			return S_FALSE;

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
	HRESULT ServerEntityManager::AddServerEntity(NetClass netClass, ServerEntity* pServerEntity)
	{
		HRESULT hr = S_OK;
		auto& entityTable = BrServer::GetInstance()->GetEntityTable();
		svrChkPtr( pServerEntity );

		if (netClass < NetClass::Unknown
			|| netClass >= BR::NetClass::Max)
		{
			return E_INVALIDARG;
		}

		svrChk( pServerEntity->InitializeEntity(entityTable.GenEntityID(EntityFaculty::Server) ) );

		// Add to task list
		svrChk( AddTickTask( pServerEntity ) );

		svrChk(entityTable.Insert(pServerEntity->GetEntityID(), pServerEntity));

		svrChk(m_ServerIDMap.Insert(pServerEntity->GetServerID(), pServerEntity));

		pServerEntity = nullptr;

	Proc_End:

		if( pServerEntity )
		{
			RemoveTickTask( pServerEntity );
		}

		return hr;
	}

	HRESULT ServerEntityManager::AddOrGetServerEntity(ServerID serverID, NetClass netClass, ServerEntity* &pServerEntity)
	{
		HRESULT hr = S_OK;

		MutexScopeLock localLock(m_ServerTableLock);

		if (netClass < NetClass::Unknown
			|| netClass >= BR::NetClass::Max)
		{
			return E_INVALIDARG;
		}

		ServerEntity* pOldEntity = nullptr;
		if (SUCCEEDED(GetServerEntity(serverID, pOldEntity)))
		{
			AssertRel(netClass == pOldEntity->GetRemoteClass());
			if (pServerEntity != nullptr && pOldEntity != pServerEntity)
			{
				svrTrace(Svr::TRC_ENTITY, "Adding Duplicated Server %0% SvrID:%1%", netClass, serverID);
				AssertRel(false);
				return E_UNEXPECTED;
			}

			pServerEntity = pOldEntity;
			return hr;
		}

		Assert(netClass != NetClass::Unknown);

		svrTrace(Svr::TRC_ENTITY, "Adding Server %0% SvrID:%1%", netClass, serverID);

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



