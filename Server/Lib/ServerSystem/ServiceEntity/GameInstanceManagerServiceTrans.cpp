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

		svrChk( super::StartTransaction() );

		svrChk( GetMyOwner()->CreateGameInstance( m_GameInsUID, GetNumberOfBotPlayer(), GetMaxPlayer() ) );

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


	// Start Transaction
	HRESULT GameInstanceTransGameDeleted::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk(super::StartTransaction());

		svrChk(GetMyOwner()->FreeGameInstance(GetRouteContext().GetFrom()));

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


};// namespace Svr 
};// namespace BR 

