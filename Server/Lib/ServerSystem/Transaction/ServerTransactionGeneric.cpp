////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2011 Bluenauts Corporation.
// 
// Author : KyungKun Ko
// Date : 01/27/2011
//
// Description : Zone master server implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "Common/MemoryPool.h"

#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/Transaction/ServerTransactionGeneric.h"
#include "ServerSystem/ServerEntityManager.h"

#include "ServerSystem/ServerEntity/EntityServerEntity.h"
#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"

#include "Protocol/Message/ServerMsgClass.h"
#include "Protocol/Policy/ServerNetPolicy.h"
#include "Protocol/Message/ClusterServerMsgClass.h"
#include "Protocol/Policy/ClusterServerNetPolicy.h"


BR_MEMORYPOOL_IMPLEMENT(Svr::GenericServerStartedTrans);
BR_MEMORYPOOL_IMPLEMENT(Svr::EntityServerStartedTrans);


namespace BR {
namespace Svr {


	
	EntityServerStartedTrans::EntityServerStartedTrans( Message::MessageData* &pIMsg )
		:ServerStartedTrans(pIMsg)
	{
		BR_TRANS_MESSAGE( Message::ClusterServer::GetClusterMemberListRes,	{ return OnGetClusterMemberList(pRes); });
	}

	EntityServerStartedTrans::~EntityServerStartedTrans()
	{
	}
	
	HRESULT EntityServerStartedTrans::OnGetClusterMemberList(Svr::TransactionResult* pRes)
	{
		HRESULT hr = S_OK;
		Message::ClusterServer::GetClusterMemberListRes msgRes;
		Svr::ServerEntity *pServerEntity = nullptr;
		const ServiceInformation *currentMaster = nullptr, *currentSlave = nullptr, *myInformation = nullptr;
		ServerEntityManager *pServerEntityManager = BrServer::GetInstance()->GetComponent<Svr::ServerEntityManager>();
		ClusterManagerServiceEntity *pClusterManagerEntity = BrServer::GetInstance()->GetComponent<Svr::ClusterManagerServiceEntity>();

		svrChk(pRes->GetHRESULT());

		svrChk( msgRes.ParseIMsg( ((Svr::MessageResult*)pRes)->GetMessage() ) );


		// Update service information with master's one
		for( UINT iMember = 0; iMember < msgRes.GetMemberList().GetSize(); iMember++ )
		{
			const ServiceInformation *pCurrentService = &msgRes.GetMemberList()[iMember];
			Svr::ServerServiceInformation *pService = nullptr;
			EntityServerEntity *pNewServerEntity = nullptr;

			// We don't need other watcher services
			if (pClusterManagerEntity->GetClusterMembership() >= ClusterMembership::StatusWatcher
				&& pCurrentService->Membership >= ClusterMembership::StatusWatcher)
				continue;

			// This server doesn't registered
			svrChk( pServerEntityManager->GetOrRegisterServer<EntityServerEntity>( pCurrentService->UID.GetServerID(), NetClass::Entity, pCurrentService->ServerAddress.strAddr, pCurrentService->ServerAddress.usPort, pNewServerEntity ) );
			pServerEntity = pNewServerEntity;

			svrChk( pClusterManagerEntity->NewServerService( pCurrentService->UID, pServerEntity, pCurrentService->Membership, pCurrentService->Status, pService ) );
		}

	Proc_End:

		return S_OK;
	}

		// Start Transaction
	HRESULT EntityServerStartedTrans::StartTransaction()
	{
		HRESULT hr = S_OK;
		Svr::ServerServiceInformation *pService = nullptr;
		const BR::ServiceInformation &serviceInfo = GetClusterManagerServiceInformation();
		Svr::ClusterManagerServiceEntity *pClusterManagerEntity = nullptr;

		svrChk( __super::StartTransaction() );


		svrChkPtr( pClusterManagerEntity = GetServerComponent<Svr::ClusterManagerServiceEntity>() );

		// We are going to add them manually, this is special case for cluster manager service
		// Add to cluster service
		svrChk( pClusterManagerEntity->NewServerService( serviceInfo.UID, GetMyOwner(), serviceInfo.Membership, serviceInfo.Status, pService ) );

		// If we got connected to the master, request member list so that we can get dynamically added cluster manager informations
		if( serviceInfo.Membership == ClusterMembership::Master )
		{
			svrChk( GetMyOwner()->GetPolicy<Policy::IPolicyClusterServer>()->GetClusterMemberListCmd( GetTransID(), RouteContext(GetOwnerEntityUID(),serviceInfo.UID), 0, pService->GetClusterID() ) );
		}
		else
			CloseTransaction(hr);


	Proc_End:

		if( FAILED(hr) )
			CloseTransaction(hr);

		return hr;
	}

	

}	// namespace Svr
}	// namespace BR 

