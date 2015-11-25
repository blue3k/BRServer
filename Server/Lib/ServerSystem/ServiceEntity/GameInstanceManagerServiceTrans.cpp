////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
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
#include "ServerSystem/ServiceEntity/GameInstanceManagerServiceTrans.h"
#include "ServerSystem/ServiceEntity/GameInstanceManagerServiceEntity.h"




BR_MEMORYPOOL_IMPLEMENT(Svr::GameInstanceTransCreateGame);
BR_MEMORYPOOL_IMPLEMENT(Svr::GameInstanceTransGameDeleted);


namespace BR {
namespace Svr {




	// Start Transaction
	HRESULT GameInstanceTransCreateGame::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( __super::StartTransaction() );

		svrChk( GetMyOwner()->CreateGameInstance( m_GameInsUID, GetNumberOfBotPlayer(), GetMaxPlayer() ) );

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


	// Start Transaction
	HRESULT GameInstanceTransGameDeleted::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk(__super::StartTransaction());

		svrChk(GetMyOwner()->FreeGameInstance(GetRouteContext().GetFrom()));

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


};// namespace Svr 
};// namespace BR 

