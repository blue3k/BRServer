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
#include "Common/MemoryPool.h"
#include "Common/BrSvrTypes.h"

#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServiceEntity/GamePartyManagerServiceTrans.h"
#include "ServerSystem/ServiceEntity/GamePartyManagerServiceEntity.h"




BR_MEMORYPOOL_IMPLEMENT(Svr::PartyManagerTransCreateParty);
BR_MEMORYPOOL_IMPLEMENT(Svr::PartyManagerTransPartyDeleted);


namespace BR {
namespace Svr {




	// Start Transaction
	HRESULT PartyManagerTransCreateParty::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk( super::StartTransaction() );

		svrChk( GetMyOwner()->CreateGameParty( GetCreator(), GetRouteContext().GetFrom(), GetServerEntity<ServerEntity>(), m_PartyUID ) );

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}



	// Start Transaction
	HRESULT PartyManagerTransPartyDeleted::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk(super::StartTransaction());

		svrChk(GetMyOwner()->FreeGameParty(GetRouteContext().GetFrom()));

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}



};// namespace Svr 
};// namespace BR 

