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
#include "MemoryManager/SFMemoryPool.h"
#include "Types/SFEngineTypedefs.h"

#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "ServerEntity/ServerEntityManager.h"

#include "Protocol/ServerService/PartyMatchingQueueService.h"
#include "ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServiceEntity/MatchingServiceUtil.h"
#include "Protocol/ServerService/GameInstanceManagerService.h"

#include "Protocol/Message/PartyMatchingQueueMsgClass.h"
#include "Protocol/Message/GamePartyMsgClass.h"
#include "Protocol/Policy/GamePartyNetPolicy.h"
#include "Protocol/Message/GamePartyManagerMsgClass.h"
#include "Protocol/Policy/GamePartyManagerNetPolicy.h"

#include "Protocol/Message/GameInstanceManagerMsgClass.h"
#include "Protocol/Policy/GameInstanceManagerNetPolicy.h"
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

		PlayerTransSearchGameInstance::PlayerTransSearchGameInstance(IHeap& heap, MessageDataPtr& pIMsg)
			: MessageTransaction(heap, Forward<MessageDataPtr>(pIMsg))
			, m_GameInstances(GetHeap())
		{
			//RegisterMessageHandler<Message::GameInstanceManager::SearchGameInstanceRes>(&PlayerTransSearchGameInstance::OnSearchGameInstanceRes);
		}

		//Result PlayerTransSearchGameInstance::OnSearchGameInstanceRes(Svr::TransactionResult* pRes)
		//{
		//	ScopeContext hr([this](Result hr)
		//		{
		//			CloseTransaction(hr);
		//		});
		//	Svr::MessageResult* pMsgRes = (Svr::MessageResult*)pRes;
		//	Message::GameInstanceManager::SearchGameInstanceRes msgRes;

		//	svrCheckClose(pRes->GetResult());

		//	svrCheck(msgRes.ParseMessage(*pMsgRes->GetMessage()));

		//	GetMyOwner()->SetGameInsUID(msgRes.GetRouteContext().GetFrom());

		//	m_GameInstances = msgRes.GetGameInstances();
		//	// TODO: query game info for client

		//	return ResultCode::SUCCESS;
		//}

		// Start Transaction
		Result PlayerTransSearchGameInstance::StartTransaction()
		{
			ScopeContext hr([this](Result hr)
				{
					CloseTransaction(hr);
				});

			ServerServiceInformation* pService = nullptr;

			svrCheck(super::StartTransaction());

			if (GetMyOwner()->GetMatchingTicket() != 0)
				svrError(ResultCode::SVR_ALREADY_INQUEUE);

			if (GetMyOwner()->GetPlayerID() != GetPlayerID())
				svrError(ResultCode::INVALID_PLAYERID);

			// TODO: Need to use searchable nosql DB like mongo DB
			DynamicArray<ServerServiceInformation*> services(GetHeap());
			svrCheck(Service::ServiceDirectory->GetServiceList(Service::ServerConfig->GameClusterID, ClusterID::GameInstance, services));

			for (auto itZoneService : services)
			{
				auto zoneTableID = itZoneService->GetCustomAttributes().GetValue<uint32_t>("ZoneTableID");
				auto instanceType = itZoneService->GetCustomAttributes().GetValue<StringCrc32>("InstanceType");
				if (instanceType != "Static"_crc)
				{
					continue;
				}

				 m_GameInstances.push_back(GameInstanceInfo(itZoneService->GetEntityUID(), instanceType, zoneTableID));
			}

			return hr;
		}



		PlayerTransJoinGameInstance::PlayerTransJoinGameInstance(IHeap& heap, MessageDataPtr& pIMsg)
			:MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg))
		{
			m_GameInsID = 0;

			RegisterMessageHandler<Message::GameInstance::JoinGameInstanceRes>(&PlayerTransJoinGameInstance::OnJoinGameInstanceRes);
		}

		Result PlayerTransJoinGameInstance::OnJoinGameInstanceRes(Svr::TransactionResult* pRes)
		{
			ScopeContext hr([this](Result hr)
				{
					if (!hr)
						CloseTransaction(hr);
				});
			Svr::MessageResult* pMsgRes = (Svr::MessageResult*)pRes;
			Message::GameInstance::JoinGameRes joinRes;

			svrCheckClose(pRes->GetResult());

			svrCheck(joinRes.ParseMessage(*pMsgRes->GetMessage()));

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
				auto serverEndpoint = Service::MessageEndpointManager->GetEndpoint(GetMyOwner()->GetPartyUID());

				svrCheck(NetPolicyGameParty(serverEndpoint).LeavePartyCmd(RouteContext(GetOwnerEntityUID(), GetMyOwner()->GetPartyUID()), GetTransID(), GetMyOwner()->GetPlayerID()));
			}
			else
			{
				CloseTransaction(hr);
			}

			return ResultCode::SUCCESS;
		}

		// Start Transaction
		Result PlayerTransJoinGameInstance::StartTransaction()
		{
			ScopeContext hr([this](Result hr)
				{
					if (!hr)
						CloseTransaction(hr);
				});
			GameInsUID insUID;
			MessageEndpoint* serverEndpoint{};

			svrCheck(super::StartTransaction());

			if (GetMyOwner()->GetMatchingTicket() != 0)
				svrError(ResultCode::SVR_ALREADY_INQUEUE);

			m_GameInsID = 0;

			if (GetMyOwner()->GetPlayerID() != GetPlayerID())
				svrError(ResultCode::INVALID_PLAYERID);

			if (GetMyOwner()->GetGameInsUID().UID != 0 && GetMyOwner()->GetGameInsUID().UID != GetInsUID())
				svrError(ResultCode::GAME_ALREADY_IN_GAME);

			if (GetInsUID() == 0)
				svrError(ResultCode::INVALID_INSTANCEID);

			insUID = GetInsUID();

			serverEndpoint = Service::MessageEndpointManager->GetEndpoint(insUID);

			svrCheck(NetPolicyGameInstance(serverEndpoint).JoinGameInstanceCmd(
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

			if (pRes->GetResult() == ResultCode::SVR_INVALID_ENTITYUID)
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
			NetPolicyGameInstance* pPolicy = nullptr;
			MessageEndpoint* serverEndpoint{};

			svrChk(super::StartTransaction());

			if (GetMyOwner()->GetPlayerID() != GetPlayerID())
				svrErr(ResultCode::INVALID_PLAYERID);

			insUID = GetMyOwner()->GetGameInsUID();
			if (insUID.UID == 0)
				svrErrClose(ResultCode::INVALID_INSTANCEID);

			serverEndpoint = Service::MessageEndpointManager->GetEndpoint(insUID);

			svrChk(NetPolicyGameInstance(serverEndpoint).LeaveGameInstanceCmd(
				RouteContext(GetOwnerEntityUID(), insUID), GetTransID(), GetMyOwner()->GetPlayerID()));

		Proc_End:

			if (!(hr))
				CloseTransaction(hr);

			return hr;
		}

	}// namespace Svr 
}// namespace SF 

