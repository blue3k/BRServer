////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 Blue3k
// 
// Author : KyungKun Ko
//
// Description : Zone master server implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "Memory/MemoryPool.h"

#include "SvrTrace.h"
#include "ServerEntity/ServerEntity.h"
#include "Transaction/ServerTransactionGeneric.h"
#include "ServerEntity/ServerEntityManager.h"

#include "ServerEntity/SvrEntityServerEntity.h"
#include "ServiceEntity/ClusterManagerServiceEntity.h"

#include "Protocol/Message/ServerMsgClass.h"
#include "Protocol/Policy/ServerNetPolicy.h"
#include "Protocol/Message/ClusterServerMsgClass.h"
#include "Protocol/Policy/ClusterServerNetPolicy.h"


SF_MEMORYPOOL_IMPLEMENT(SF::Svr::GenericServerStartedTrans);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::EntityServerStartedTrans);


namespace SF {
namespace Svr {



	EntityServerStartedTrans::EntityServerStartedTrans(IHeap& memMgr, MessageDataPtr &pIMsg )
		:ServerStartedTrans(memMgr, pIMsg)
	{
		BR_TRANS_MESSAGE( Message::ClusterServer::GetClusterMemberListRes,	{ return OnGetClusterMemberList(pRes); });
	}

	EntityServerStartedTrans::~EntityServerStartedTrans()
	{
	}

	Result EntityServerStartedTrans::OnGetClusterMemberList(Svr::TransactionResult* pRes)
	{
		Result hr = ResultCode::SUCCESS;
		Message::ClusterServer::GetClusterMemberListRes msgRes;
		Svr::ServerEntity *pServerEntity = nullptr;
		ServerEntityManager *pServerEntityManager = GetServerComponent<Svr::ServerEntityManager>();
		ClusterManagerServiceEntity *pClusterManagerEntity = GetServerComponent<Svr::ClusterManagerServiceEntity>();

		svrChk(pRes->GetResult());

		svrChk( msgRes.ParseMessage( *((Svr::MessageResult*)pRes)->GetMessage() ) );


		// Update service information with master's one
		for( uint iMember = 0; iMember < msgRes.GetMemberList().size(); iMember++ )
		{
			const ServiceInformation *pCurrentService = &msgRes.GetMemberList()[iMember];
			Svr::ServerServiceInformation *pService = nullptr;
			EntityServerEntity *pNewServerEntity = nullptr;

			// We don't need other watcher services
			if (pClusterManagerEntity->GetClusterMembership() >= ClusterMembership::StatusWatcher
				&& pCurrentService->Membership >= ClusterMembership::StatusWatcher)
				continue;

			// This server doesn't registered
			svrChk( pServerEntityManager->GetOrRegisterServer<EntityServerEntity>( pCurrentService->UID.GetServerID(), NetClass::Entity, pCurrentService->ServerAddress, pNewServerEntity ) );
			pServerEntity = pNewServerEntity;

			svrChk( pClusterManagerEntity->NewServerService( pCurrentService->UID, pServerEntity, pCurrentService->Membership, pCurrentService->Status, pService ) );
		}

	Proc_End:

		return ResultCode::SUCCESS;
	}

		// Start Transaction
	Result EntityServerStartedTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ServerServiceInformation *pService = nullptr;
		const ServiceInformation &serviceInfo = GetClusterManagerServiceInformation();
		Svr::ClusterManagerServiceEntity *pClusterManagerEntity = nullptr;

		svrChk( super::StartTransaction() );


		svrChkPtr( pClusterManagerEntity = GetServerComponent<Svr::ClusterManagerServiceEntity>() );

		// We are going to add them manually, this is special case for cluster manager service
		// Add to cluster service
		svrChk( pClusterManagerEntity->NewServerService( serviceInfo.UID, GetMyOwner(), serviceInfo.Membership, serviceInfo.Status, pService ) );

		// If we got connected to the master, request member list so that we can get dynamically added cluster manager informations
		if( serviceInfo.Membership == ClusterMembership::Master )
		{
			svrChk(Policy::NetPolicyClusterServer(GetMyOwner()->GetConnection()).GetClusterMemberListCmd( RouteContext(GetOwnerEntityUID(),serviceInfo.UID), GetTransID(), 0, pService->GetClusterID() ) );
		}
		else
			CloseTransaction(hr);


	Proc_End:

		if( !(hr) )
			CloseTransaction(hr);

		return hr;
	}

	

}	// namespace Svr
}	// namespace SF 

