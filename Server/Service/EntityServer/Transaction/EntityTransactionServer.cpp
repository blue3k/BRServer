////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : KyungKun Ko
//
// Description : Entity server implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "EntityServer.h"
#include "SvrTrace.h"
#include "EntityServerClass.h"
#include "Memory/MemoryPool.h"

#include "Protocol/Message/EntityServerMsgClass.h"
#include "Protocol/Policy/EntityServerNetPolicy.h"
#include "Server/BrServer.h"
#include "ServerEntity/ServerEntity.h"
#include "ServerEntity/ServerEntityManager.h"
#include "Entity/EntityManager.h"
#include "ServiceEntity/ClusterManagerServiceEntity.h"

//#include "ServerEntity/GameServerEntity.h"

#include "Protocol/Policy/EntityServerNetPolicy.h"
#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GameMasterServerNetPolicy.h"

#include "EntityTransactionServer.h"
#include "EntityTransactionUser.h"



//SF_MEMORYPOOL_IMPLEMENT(BR::EntityServer::TransServerStarted);
SF_MEMORYPOOL_IMPLEMENT(BR::EntityServer::TransGenericServerStarted);

namespace SF {
namespace EntityServer {



	//Result TransServerStarted::StartTransaction()
	//{
	//	Result hr = ResultCode::SUCCESS;
	//	const ServiceInformation &serviceInfo = GetClusterManagerServiceInformation();
	//	Svr::ServerServiceInformation *pService = nullptr;

	//	svrChk(super::StartTransaction());

	//	// We are going to add them manually, this is special case for cluster manager service
	//	// Add to cluster service
	//	svrAssert(serviceInfo.Membership > ClusterMembership::Slave);

	//	svrChk(GetMyServer()->GetComponent<Svr::ClusterManagerServiceEntity>()->NewServerService(serviceInfo.UID, GetMyOwner(), serviceInfo.Membership, serviceInfo.Status, pService));

	//Proc_End:

	//	CloseTransaction(hr);

	//	return hr;
	//}



};// namespace EntityServer 
};// namespace SF 

