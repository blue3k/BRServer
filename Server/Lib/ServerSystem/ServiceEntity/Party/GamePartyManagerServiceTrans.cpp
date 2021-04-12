////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "MemoryManager/SFMemoryPool.h"
#include "Types/BrSvrTypes.h"

#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "Server/BrServer.h"
#include "Entity/EntityManager.h"
#include "ServiceEntity/Party/GamePartyManagerServiceTrans.h"
#include "ServiceEntity/Party/GamePartyManagerServiceEntity.h"





namespace SF {
namespace Svr {




	// Start Transaction
	Result PartyManagerTransCreateParty::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		svrChk( GetMyOwner()->CreateGameParty( (GameID)GetGameID(), GetCreator(), GetRouteContext().GetFrom(), GetRemoteEndpoint(), m_PartyUID ) );

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

