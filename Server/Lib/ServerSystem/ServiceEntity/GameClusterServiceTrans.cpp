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
#include "Common/HRESCommon.h"
#include "Common/HRESGame.h"
#include "Common/HRESLogin.h"
#include "Common/MemoryPool.h"
#include "Common/BRSvrTypes.h"

#include "ServerSystem/BRServerUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServiceEntity/GameClusterServiceTrans.h"
#include "ServerSystem/ServiceEntity/GameClusterServiceEntity.h"




BR_MEMORYPOOL_IMPLEMENT(Svr::GameServerTransGamePlayerEntityCreatedS2CEvt);
BR_MEMORYPOOL_IMPLEMENT(Svr::GameServerTransGamePlayerEntityDeletedS2CEvt);


namespace BR {
namespace Svr {

	
	// Start Transaction
	HRESULT GameServerTransGamePlayerEntityCreatedS2CEvt::StartTransaction()
	{
		HRESULT hr = S_OK;
		ServerEntity *pServerEntity = nullptr;

		svrChk( __super::StartTransaction() );

		svrChk( GetServerComponent<ServerEntityManager>()->GetServerEntity( GetPlayerUID().GetServerID(), pServerEntity ) );

		svrChk( GetMyOwner()->CreatePlayer( GetPlayerID(), GetPlayerUID(), pServerEntity ) );

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}




	// Start Transaction
	HRESULT GameServerTransGamePlayerEntityDeletedS2CEvt::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( __super::StartTransaction() );

		svrChk( GetMyOwner()->DeletePlayer( GetPlayerID(), GetPlayerUID() ) );

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}



};// namespace Svr 
};// namespace BR 

