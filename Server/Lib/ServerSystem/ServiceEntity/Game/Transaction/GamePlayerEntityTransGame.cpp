////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
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

#include "Protocol/PartyMatchingQueueService.h"
#include "ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServiceEntity/MatchingServiceUtil.h"
#include "Protocol/GameInstanceManagerService.h"

#include "Protocol/PartyMatchingQueueMsgClass.h"
#include "Protocol/GamePartyMsgClass.h"
#include "Protocol/GamePartyNetPolicy.h"
#include "Protocol/GamePartyManagerMsgClass.h"
#include "Protocol/GamePartyManagerNetPolicy.h"

#include "Protocol/GameInstanceManagerMsgClass.h"
#include "Protocol/GameInstanceManagerNetPolicy.h"
#include "Protocol/GameInstanceMsgClass.h"
#include "Protocol/GameInstanceNetPolicy.h"

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

		PlayerTransSearchGameInstance::PlayerTransSearchGameInstance(IHeap& heap, const MessageDataPtr& pIMsg)
			: MessageTransaction(heap, pIMsg)
			, m_GameInstances(GetHeap())
		{
		}

		// Start Transaction
		Result PlayerTransSearchGameInstance::StartTransaction()
		{
			ScopeContext hr([this](Result hr)
				{
					CloseTransaction(hr);
				});

			svrCheck(super::StartTransaction());

			if (GetMyOwner()->GetMatchingTicket() != 0)
				svrError(ResultCode::SVR_ALREADY_INQUEUE);

			VariableTable attributes;
			attributes.SetValue<String>("Custom.Type", "Static");

			// TODO: Need to use searchable nosql DB like mongo DB
			DynamicArray<SharedPointerT<EntityInformation>> services(GetHeap());
			svrCheck(Service::ServiceDirectory->FindObjects(Service::ServerConfig->GameClusterID, ClusterID::GameInstance, attributes, services));

			for (auto itZoneService : services)
			{
				VariableTable zoneInfo(GetHeap());
				auto zoneTableID = itZoneService->GetCustomAttributes().GetValue<uint32_t>("ZoneTableID");
				auto instanceType = itZoneService->GetCustomAttributes().GetValue<StringCrc32>("Type");
				auto numPlayers = itZoneService->GetCustomAttributes().GetValue<int32_t>("NumPlayers");

				zoneInfo.SetValue("InstanceUID", itZoneService->GetEntityUID().UID);
				zoneInfo.SetValue("ZoneTableID", zoneTableID);
				zoneInfo.SetValue("Type", instanceType);
				zoneInfo.SetValue("NumPlayers", numPlayers);

				m_GameInstances.push_back(Forward<VariableTable>(zoneInfo));
			}

			return hr;
		}


		PlayerTransJoinGameInstance::PlayerTransJoinGameInstance(IHeap& heap, const MessageDataPtr& pIMsg)
			:MessageTransaction(heap, pIMsg)
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
			Message::GameInstance::JoinGameInstanceRes joinRes;

			svrCheckClose(pRes->GetResult());

			svrCheck(joinRes.ParseMessage(*pMsgRes->GetMessage()));

			GetMyOwner()->SetGameInsUID(joinRes.GetRouteContext().GetFrom());
			m_GameInsAddress4 = joinRes.GetGameInsSvr4();
			m_GameInsAddress6 = joinRes.GetGameInsSvr6();
			m_GameInsID = joinRes.GetRouteContext().GetFrom();

			// We don't need to do it here
			//if (joinRes.GetIsNewJoin())
			{
				GetMyOwner()->UpdateGamePlayer();
			}

			CloseTransaction(hr);

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

			if (GetMyOwner()->GetCharacterID() == 0)
				svrError(ResultCode::CHARACTER_NOT_SELECTED);

			if (GetMyOwner()->GetGameInsUID().UID != 0 && GetMyOwner()->GetGameInsUID().UID != GetInsUID())
				svrError(ResultCode::GAME_ALREADY_IN_GAME);

			if (GetInsUID() == 0)
				svrError(ResultCode::INVALID_INSTANCEID);

			insUID = GetInsUID();

			serverEndpoint = Service::MessageEndpointManager->GetEndpoint(insUID);

			svrCheck(NetPolicyGameInstance(serverEndpoint).JoinGameInstanceCmd(
				RouteContext(GetOwnerEntityUID(), insUID), GetTransID(),
				GetMyOwner()->GetPlayerInformation(), 
				GetMyOwner()->GetCharacterVisualData(), 
				GetMyOwner()->GetCharacterData()));

			return hr;
		}

		Result PlayerTransJoinGameInstance::CloseTransaction(Result hrRes)
		{
			Result result = super::CloseTransaction(hrRes);
			svrTrace(SVR_INFO, "PlayerTransJoinGameInstanceRes PlayerID:{0} : hr:{1}", GetMyOwner()->GetPlayerID(), hrRes);
			return result;
		}


		PlayerTransLeaveGameInstance::PlayerTransLeaveGameInstance(IHeap& heap, const MessageDataPtr& pIMsg)
			:MessageTransaction(heap, pIMsg)
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

