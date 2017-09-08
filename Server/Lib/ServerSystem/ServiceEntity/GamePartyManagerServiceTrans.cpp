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
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "Memory/MemoryPool.h"
#include "Types/BrSvrTypes.h"

#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "Server/BrServer.h"
#include "ServerEntity/ServerEntityManager.h"
#include "Entity/EntityManager.h"
#include "ServiceEntity/GamePartyManagerServiceTrans.h"
#include "ServiceEntity/GamePartyManagerServiceEntity.h"




SF_MEMORYPOOL_IMPLEMENT(SF::Svr::PartyManagerTransCreateParty);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::PartyManagerTransPartyDeleted);


namespace SF {
namespace Svr {




	// Start Transaction
	Result PartyManagerTransCreateParty::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		svrChk( GetMyOwner()->CreateGameParty( GetCreator(), GetRouteContext().GetFrom(), GetServerEntity<ServerEntity>(), m_PartyUID ) );

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}



	// Start Transaction
	Result PartyManagerTransPartyDeleted::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::StartTransaction());

		svrChk(GetMyOwner()->FreeGameParty(GetRouteContext().GetFrom()));

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}



};// namespace Svr 
};// namespace SF 

