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
#include "ServerSystem/SvrTrace.h"
#include "EntityServerClass.h"
#include "Memory/MemoryPool.h"

#include "Protocol/Message/EntityServerMsgClass.h"
#include "Protocol/Policy/EntityServerIPolicy.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"

//#include "ServerEntity/GameServerEntity.h"

#include "Protocol/Policy/EntityServerIPolicy.h"
#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Policy/GameMasterServerIPolicy.h"

#include "EntityTransactionServer.h"
#include "EntityTransactionUser.h"



//BR_MEMORYPOOL_IMPLEMENT(EntityServer::TransServerStarted);
BR_MEMORYPOOL_IMPLEMENT(EntityServer::TransGenericServerStarted);

namespace BR {
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
};// namespace BR 

