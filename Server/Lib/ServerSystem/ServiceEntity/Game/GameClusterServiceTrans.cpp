////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves
// 
// Author : KyungKun Ko
//
// Description : game cluster entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/ResultCode/BRResultCodeCommon.h"
#include "Common/ResultCode/BRResultCodeGame.h"
#include "Common/ResultCode/BRResultCodeLogin.h"
#include "Common/MemoryPool.h"
#include "Common/BrSvrTypes.h"

#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServiceEntity/Game/GameClusterServiceTrans.h"
#include "ServerSystem/ServiceEntity/Game/GameClusterServiceEntity.h"




BR_MEMORYPOOL_IMPLEMENT(Svr::GameServerTransGamePlayerEntityCreatedS2CEvt);
BR_MEMORYPOOL_IMPLEMENT(Svr::GameServerTransGamePlayerEntityDeletedS2CEvt);


namespace BR {
namespace Svr {

	
	// Start Transaction
	HRESULT GameServerTransGamePlayerEntityCreatedS2CEvt::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;
		ServerEntity *pServerEntity = nullptr;

		svrChk( super::StartTransaction() );

		svrChk( GetServerComponent<ServerEntityManager>()->GetServerEntity( GetPlayerUID().GetServerID(), pServerEntity ) );

		svrChk( GetMyOwner()->CreatePlayer( GetPlayerID(), GetPlayerUID(), pServerEntity ) );

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}




	// Start Transaction
	HRESULT GameServerTransGamePlayerEntityDeletedS2CEvt::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk( super::StartTransaction() );

		svrChk( GetMyOwner()->DeletePlayer( GetPlayerID(), GetPlayerUID() ) );

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}



};// namespace Svr 
};// namespace BR 

