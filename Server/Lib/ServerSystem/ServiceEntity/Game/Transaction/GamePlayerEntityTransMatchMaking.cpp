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
#include "ResultCode/SFResultCodeLogin.h"
#include "Memory/SFMemoryPool.h"
#include "Types/SFEngineTypedefs.h"

#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "Server/BrServer.h"
#include "ServerEntity/ServerEntityManager.h"

#include "ServiceEntity/Party/GamePartyManagerServiceEntity.h"
#include "Protocol/ServerService/GamePartyManagerService.h"
#include "ServiceEntity/Game/PlayerManagerServiceEntity.h"

#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"

#include "Protocol/Message/GamePartyManagerMsgClass.h"
#include "Protocol/Policy/GamePartyManagerNetPolicy.h"

#include "Protocol/Message/GamePartyMsgClass.h"
#include "Protocol/Policy/GamePartyNetPolicy.h"

#include "Protocol/ServerService/PartyMatchingQueueService.h"

#include "GamePlayerEntityTransMatchMaking.h"
#include "ServiceEntity/Game/GamePlayerEntity.h"

#include "ServiceEntity/Game/Subsystem/UserFriendSystem.h"
//#include "GameSystem/UserGamePlayerInfoSystem.h"
#include "ServiceEntity/MatchingServiceUtil.h"






namespace SF {
namespace Svr {


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Match making
	//


	PlayerTransRequestGameMatch::PlayerTransRequestGameMatch(IHeap& heap, MessageDataPtr& pIMsg)
		: MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg))
	{
		BR_TRANS_MESSAGE(Message::GameParty::StartGameMatchRes, { return OnRequestPartyMatchingRes(pRes); });
		BR_TRANS_MESSAGE(Message::PartyMatchingQueue::RegisterPlayerMatchingRes, { return OnRequestPlayerMatchingRes(pRes); });
	}

	Result PlayerTransRequestGameMatch::OnRequestPartyMatchingRes(Svr::TransactionResult*& pRes)
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult* pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameParty::StartGameMatchRes res;

		svrChkClose(pRes->GetResult());
		svrChk(res.ParseMessage(*pMsgRes->GetMessage()));

	Proc_End:

		svrTrace(SVR_INFO, "PartyMatch requested PlayerID:{0}, hr:{1}", GetMyOwner()->GetPlayerID(), hr);

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}

	Result PlayerTransRequestGameMatch::OnRequestPlayerMatchingRes(Svr::TransactionResult*& pRes)
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult* pMsgRes = (Svr::MessageResult*)pRes;
		Message::PartyMatchingQueue::RegisterPlayerMatchingRes res;

		svrChkClose(pRes->GetResult());
		svrChk(res.ParseMessage(*pMsgRes->GetMessage()));

		GetMyOwner()->SetMatchingTicket(res.GetMatchingTicket());

		Policy::NetSvrPolicyGame(GetConnection()).GameMatchingStartedS2CEvt();

	Proc_End:

		svrTrace(SVR_INFO, "PlayerMatch requested PlayerID:{0}, hr:{1}", GetMyOwner()->GetPlayerID(), hr);

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}


	// Start Transaction
	Result PlayerTransRequestGameMatch::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ServerServiceInformation* pService = nullptr;

		m_TotalGem = 0;
		m_TotalGameMoney = 0;

		svrChk(super::StartTransaction());

		if (GetMyOwner()->GetGameInsUID().UID != 0)
			svrErrClose(ResultCode::GAME_ALREADY_IN_GAME);

		if (GetMyOwner()->GetMatchingTicket() != 0)
		{
			if (Util::TimeSince(GetMyOwner()->GetMatchingStartTime()) < DurationMS(GameConst::MATCHING_TIMEOUT))
				svrErrClose(ResultCode::SVR_ALREADY_INQUEUE);

			// clear timed out tickets
			GetMyOwner()->SetMatchingTicket(0);
		}

		if (GetMyOwner()->GetPartyUID().UID == 0)
		{
			// Player isn't in a party, just do it alone
			Svr::ServerServiceInformation* pWatcherService = nullptr;
			auto queueClusterID = Svr::MatchingUtil::GetQueueClusterID(GetNumPlayer(), 1, (PlayerRole)GetRequestRole());

			svrChk(Service::ClusterManager->GetRandomService(Svr::GetServerGameID(), queueClusterID, pService));
			svrChk(pService->GetService<Svr::PartyMatchingQueueService>()->RegisterPlayerMatchingCmd(GetTransID(), 0, GetMyOwner()->GetPlayerID()));
		}
		else
		{
			// Let the party handle this
			svrChk(Policy::NetPolicyGameParty(Service::ServerEntityManager->GetServerConnection(GetMyOwner()->GetPartyUID().GetServerID())).StartGameMatchCmd(
				RouteContext(GetOwnerEntityUID(), GetMyOwner()->GetPartyUID()), GetTransID(), GetMyOwner()->GetPlayerID(), GetNumPlayer()));
		}

	Proc_End:

		if (!(hr))
			CloseTransaction(hr);

		return hr;
	}



	PlayerTransCancelGameMatch::PlayerTransCancelGameMatch(IHeap& heap, MessageDataPtr& pIMsg)
		: MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg))
	{
		BR_TRANS_MESSAGE(Message::GameParty::CancelGameMatchRes, { return OnCancelPartyMatchingRes(pRes); });
		BR_TRANS_MESSAGE(Message::PartyMatchingQueue::UnregisterMatchingRes, { return OnCancelPlayerMatchingRes(pRes); });
	}

	Result PlayerTransCancelGameMatch::OnCancelPartyMatchingRes(Svr::TransactionResult*& pRes)
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult* pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameParty::CancelGameMatchRes res;

		svrChkClose(pRes->GetResult());
		svrChk(res.ParseMessage(*pMsgRes->GetMessage()));

		// This means it just pended for canceling. yo have to wait canceled event from matching queue
		//GetMyOwner()->SetMatchingTicket(0);

		//Policy::NetSvrPolicyGame(GetConnection()).GameMatchingCanceledS2CEvt();

	Proc_End:

		svrTrace(SVR_INFO, "PartyMatch canceled PlayerID:{0}, hr:{1}", GetMyOwner()->GetPlayerID(), hr);

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}

	Result PlayerTransCancelGameMatch::OnCancelPlayerMatchingRes(Svr::TransactionResult*& pRes)
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult* pMsgRes = (Svr::MessageResult*)pRes;
		Message::PartyMatchingQueue::UnregisterMatchingRes res;

		switch ((int32_t)pRes->GetResult())
		{
		case (int32_t)ResultCode::SVR_RESERVED_QUEUEITEM:
			svrErrClose(ResultCode::SVR_RESERVED_QUEUEITEM);
			break;
		case (int32_t)ResultCode::SVR_QUEUEITEM_CANCELED:
		case (int32_t)ResultCode::SVR_INVALID_QUEUEITEM:
			GetMyOwner()->SetMatchingTicket(0);
			break;
		default:
			svrChk(pRes->GetResult());
			svrChk(res.ParseMessage(*pMsgRes->GetMessage()));
			GetMyOwner()->SetMatchingTicket(0);
			break;
		};


	Proc_End:

		svrTrace(SVR_INFO, "PlayerMatch canceled PlayerID:{0}, hr:{1}", GetMyOwner()->GetPlayerID(), hr);

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}


	// Start Transaction
	Result PlayerTransCancelGameMatch::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::StartTransaction());

		if (GetMyOwner()->GetMatchingTicket() == 0)
		{
			svrErrClose(ResultCode::SVR_INVALID_QUEUEITEM);
		}

		if (GetMyOwner()->GetPartyUID().UID == 0)
		{
			// Player isn't in a party, just do it alone
			svrChk(Policy::NetPolicyPartyMatchingQueue(Service::ServerEntityManager->GetServerConnection(GetMyOwner()->GetMatchingTicket().QueueUID.GetServerID())).UnregisterMatchingCmd(
				RouteContext(GetOwnerEntityUID(), GetMyOwner()->GetMatchingTicket().QueueUID), GetTransID(), 0, GetMyOwner()->GetMatchingTicket()));
		}
		else
		{
			// Let the party handle this
			svrChk(Policy::NetPolicyGameParty(Service::ServerEntityManager->GetServerConnection(GetMyOwner()->GetPartyUID().GetServerID())).CancelGameMatchCmd(
				RouteContext(GetOwnerEntityUID(), GetMyOwner()->GetPartyUID()), GetTransID(), GetMyOwner()->GetPlayerID()));
		}

	Proc_End:

		if (!(hr))
			CloseTransaction(hr);

		return hr;
	}




	//////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Matching queue
	//

	// Start Transaction
	Result PlayerTransPlayerMatchingCanceledS2CEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::StartTransaction());

		if (GetMyOwner()->GetMatchingTicket() != GetMatchingTicket())
		{
			svrErr(ResultCode::SVR_INVALID_QUEUEITEM);
		}

		GetMyOwner()->SetMatchingTicket(0);

		svrChk(Policy::NetSvrPolicyGame(GetConnection()).GameMatchingCanceledS2CEvt());

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}

	// Start Transaction
	Result PlayerTransPlayerMatchingItemDequeuedS2CEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::StartTransaction());

		if (GetMyOwner()->GetMatchingTicket() != GetMatchingTicket())
		{
			svrErrClose(ResultCode::SVR_INVALID_QUEUEITEM);
		}

		GetMyOwner()->SetMatchingTicket(0);

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


}// namespace Svr
}// namespace SF 

