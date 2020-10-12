////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2020 The Braves
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
#include "ServerEntity/ServerEntityManager.h"

#include "Protocol/ServerService/PartyMatchingQueueService.h"
#include "ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServiceEntity/Game/PlayerManagerServiceEntity.h"
#include "ServiceEntity/MatchingServiceUtil.h"
#include "Protocol/ServerService/GamePartyManagerService.h"
#include "ServiceEntity/Party/GamePartyManagerServiceEntity.h"

#include "Protocol/Message/PartyMatchingQueueMsgClass.h"
#include "Protocol/Message/GamePartyMsgClass.h"
#include "Protocol/Policy/GamePartyNetPolicy.h"
#include "Protocol/Message/GamePartyManagerMsgClass.h"
#include "Protocol/Policy/GamePartyManagerNetPolicy.h"

#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"

#include "GamePlayerEntityTrans.h"
#include "GamePlayerEntityTransClose.h"
#include "GamePlayerEntityTransGame.h"

#include "Server/BrServer.h"

#include "DB/Game/GameDB.h"
#include "DB/Game/GameQuery.h"


namespace SF {
	namespace Svr {


		/////////////////////////////////////////////////////////////////////////////
		//
		//	Game command transaction
		//


		PlayerTransJoinGameInstance::PlayerTransJoinGameInstance(IHeap& heap, MessageDataPtr& pIMsg)
			:MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg))
		{
			m_GameInsID = 0;

			RegisterMessageHandler<Message::GameInstance::JoinGameRes>(&PlayerTransJoinGameInstance::OnJoinGameInstanceRes);
			//RegisterMessageHandler<Message::GameParty::LeavePartyRes>(&PlayerTransJoinGameInstance::OnLeavePartyRes);
		}

		Result PlayerTransJoinGameInstance::OnJoinGameInstanceRes(Svr::TransactionResult* pRes)
		{
			Result hr = ResultCode::SUCCESS;
			Svr::MessageResult* pMsgRes = (Svr::MessageResult*)pRes;
			Message::GameInstance::JoinGameRes joinRes;

			svrChkClose(pRes->GetResult());

			svrChk(joinRes.ParseMessage(*pMsgRes->GetMessage()));

			GetMyOwner()->AddGameTransactionLog(TransLogCategory::Game, 1, 0, joinRes.GetRouteContext().GetFrom().UID);

			GetMyOwner()->SetGameInsUID(joinRes.GetRouteContext().GetFrom());

			m_GameInsID = joinRes.GetRouteContext().GetFrom();

			// We don't need to do it here
			if (joinRes.GetIsNewJoin())
			{
				GetMyOwner()->UpdateGamePlayer();
			}

			// Leave party when the player joined a party
			if (GetMyOwner()->GetPartyUID().UID != 0)
			{
				Svr::ServerEntity* pServerEntity = nullptr;

				svrChk(Service::ServerEntityManager->GetServerEntity(GetMyOwner()->GetPartyUID().GetServerID(), pServerEntity));

				svrChk(Policy::NetPolicyGameParty(pServerEntity->GetConnection()).LeavePartyCmd(RouteContext(GetOwnerEntityUID(), GetMyOwner()->GetPartyUID()), GetTransID(), GetMyOwner()->GetPlayerID()));
			}
			else
			{
				CloseTransaction(hr);
			}

		Proc_End:

			if (!(hr))
				CloseTransaction(hr);

			return ResultCode::SUCCESS;
		}

		// Start Transaction
		Result PlayerTransJoinGameInstance::StartTransaction()
		{
			FunctionContext hr([this](Result hr)
				{
					if (!hr)
						CloseTransaction(hr);
				});
			GameInsUID insUID;

			svrCheck(super::StartTransaction());

			if (GetMyOwner()->GetMatchingTicket() != 0)
				svrError(ResultCode::SVR_ALREADY_INQUEUE);

			m_GameInsID = 0;

			if (GetMyOwner()->GetAuthTicket() != GetTicket())
				svrError(ResultCode::INVALID_TICKET);

			if (GetMyOwner()->GetPlayerID() != GetPlayerID())
				svrError(ResultCode::INVALID_PLAYERID);

			if (GetMyOwner()->GetGameInsUID().UID != 0 && GetMyOwner()->GetGameInsUID().UID != GetInsUID())
				svrError(ResultCode::GAME_ALREADY_IN_GAME);

			if (GetInsUID() == 0)
				svrError(ResultCode::INVALID_INSTANCEID);

			insUID = GetInsUID();

			svrCheck(Policy::NetPolicyGameInstance(Service::ServerEntityManager->GetServerConnection(insUID.GetServerID())).JoinGameInstanceCmd(
				RouteContext(GetOwnerEntityUID(), insUID), GetTransID(),
				GetMyOwner()->GetPlayerInformation()));

			return hr;
		}

		Result PlayerTransJoinGameInstance::CloseTransaction(Result hrRes)
		{
			Result result = super::CloseTransaction(hrRes);
			svrTrace(SVR_INFO, "PlayerTransJoinGameInstanceRes PlayerID:{0} : hr:{1}", GetMyOwner()->GetPlayerID(), hrRes);
			return result;
		}


		// Start Transaction
		Result PlayerTransJoinedS2SEvt::StartTransaction()
		{
			FunctionContext hr([this](Result hr)
				{
					CloseTransaction(hr);
				});

			svrCheck(super::StartTransaction());

			if (GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
				svrErrorClose(ResultCode::INVALID_INSTANCEID);

			svrCheck(Policy::NetSvrPolicyGame(GetConnection()).PlayerJoinedS2CEvt(GetRouteContext().GetFrom(), GetJoinedPlayer()));

			return hr;
		}


		PlayerTransLeaveGameInstance::PlayerTransLeaveGameInstance(IHeap& heap, MessageDataPtr& pIMsg)
			:MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg))
		{
			RegisterMessageHandler<Message::GameInstance::LeaveGameInstanceRes>(&PlayerTransLeaveGameInstance::OnLeaveGameInstanceRes);
		}

		Result PlayerTransLeaveGameInstance::OnLeaveGameInstanceRes(Svr::TransactionResult* pRes)
		{
			Result hr = ResultCode::SUCCESS;
			Svr::MessageResult* pMsgRes = (Svr::MessageResult*)pRes;
			Message::GameInstance::LeaveGameInstanceRes leaveRes;

			GetMyOwner()->AddGameTransactionLog(TransLogCategory::Game, -1, 0, GetMyOwner()->GetGameInsUID().UID);

			if (pRes->GetResult() == Result(ResultCode::SVR_INVALID_ENTITYUID))
			{

				GetMyOwner()->SetGameInsUID(0);
				GetMyOwner()->UpdateDBSync();
			}
			else
			{
				svrChkClose(pRes->GetResult());

				svrChk(leaveRes.ParseMessage(*pMsgRes->GetMessage()));

				GetMyOwner()->SetGameInsUID(0);
				GetMyOwner()->UpdateDBSync();
			}

		Proc_End:

			CloseTransaction(hr);

			return ResultCode::SUCCESS;
		}

		Result PlayerTransLeaveGameInstance::CloseTransaction(Result hrRes)
		{
			Result result = super::CloseTransaction(hrRes);
			svrTrace(SVR_INFO, "LeaveGameRes PlayerID:{0} : hr:{1}", GetMyOwner()->GetPlayerID(), hrRes);
			return result;
		}

		// Start Transaction
		Result PlayerTransLeaveGameInstance::StartTransaction()
		{
			Result hr = ResultCode::SUCCESS;
			GameInsUID insUID;
			Policy::NetPolicyGameInstance* pPolicy = nullptr;

			svrChk(super::StartTransaction());

			if (GetMyOwner()->GetAuthTicket() != GetTicket())
				svrErr(ResultCode::INVALID_TICKET);

			if (GetMyOwner()->GetPlayerID() != GetPlayerID())
				svrErr(ResultCode::INVALID_PLAYERID);

			insUID = GetMyOwner()->GetGameInsUID();
			if (insUID.UID == 0)
				svrErrClose(ResultCode::INVALID_INSTANCEID);

			svrChk(Policy::NetPolicyGameInstance(Service::ServerEntityManager->GetServerConnection(insUID.GetServerID())).LeaveGameInstanceCmd(
				RouteContext(GetOwnerEntityUID(), insUID), GetTransID(), GetMyOwner()->GetPlayerID()));

		Proc_End:

			if (!(hr))
				CloseTransaction(hr);

			return hr;
		}


		// Start Transaction
		Result PlayerTransPlayerMovementC2SEvt::StartTransaction()
		{
			FunctionContext hr([this](Result hr)
				{
					CloseTransaction(hr);
				});

			svrCheck(super::StartTransaction());

			if (GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
				svrErrorClose(ResultCode::INVALID_INSTANCEID);

			svrCheck(Policy::NetSvrPolicyGame(GetConnection()).PlayerMovementS2CEvt(GetRouteContext().GetFrom(), GetPlayerID(), GetAttributes()));

			return hr;
		}


		// Start Transaction
		Result PlayerTransLeftS2SEvt::StartTransaction()
		{
			FunctionContext hr([this](Result hr)
				{
					CloseTransaction(hr);
				});

			svrCheck(super::StartTransaction());

			if (GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
				svrErrorClose(ResultCode::INVALID_INSTANCEID);

			svrCheck(Policy::NetSvrPolicyGame(GetConnection()).PlayerLeftS2CEvt(GetRouteContext().GetFrom(), GetLeftPlayerID()));

			return hr;
		}




	}// namespace Svr 
}// namespace SF 

