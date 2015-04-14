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
#include "ServerSystem/ServiceEntity/GamePartyManagerServiceTrans.h"
#include "ServerSystem/ServiceEntity/GamePartyManagerServiceEntity.h"




BR_MEMORYPOOL_IMPLEMENT(Svr::PartyManagerTransCreateParty);
BR_MEMORYPOOL_IMPLEMENT(Svr::PartyManagerTransPartyDeleted);


namespace BR {
namespace Svr {




	// Start Transaction
	HRESULT PartyManagerTransCreateParty::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( __super::StartTransaction() );

		svrChk( GetMyOwner()->CreateGameParty( GetCreator(), GetRouteContext().From, GetServerEntity<ServerEntity>(), m_PartyUID ) );

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}



	// Start Transaction
	HRESULT PartyManagerTransPartyDeleted::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk(__super::StartTransaction());

		svrChk(GetMyOwner()->FreeGameParty(GetRouteContext().From));

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}



};// namespace Svr 
};// namespace BR 

