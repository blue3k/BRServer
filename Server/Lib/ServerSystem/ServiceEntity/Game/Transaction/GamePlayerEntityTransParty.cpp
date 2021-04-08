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
#include "MemoryManager/SFMemoryPool.h"
#include "Types/SFEngineTypedefs.h"

#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "Server/BrServer.h"
#include "ServerEntity/ServerEntityManager.h"

#include "ServiceEntity/Party/GamePartyManagerServiceEntity.h"
#include "Protocol/GamePartyManagerService.h"

#include "Protocol/GameInstanceMsgClass.h"
#include "Protocol/GameInstanceNetPolicy.h"

#include "Protocol/GamePartyManagerMsgClass.h"
#include "Protocol/GamePartyManagerNetPolicy.h"

#include "Protocol/GamePartyMsgClass.h"
#include "Protocol/GamePartyNetPolicy.h"

#include "GamePlayerEntityTransParty.h"
#include "ServiceEntity/Game/GamePlayerEntity.h"

#include "ServiceEntity/Game/Subsystem/UserFriendSystem.h"
//#include "GameSystem/UserGamePlayerInfoSystem.h"

#include "DB/AccountDB.h"
#include "DB/AccountQuery.h"
#include "DB/Game/GameDB.h"
#include "DB/Game/GameQuery.h"





namespace SF {
namespace Svr {


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Party
	//

	PlayerTransGameMatchedS2SEvt::PlayerTransGameMatchedS2SEvt(IHeap& heap, const MessageDataPtr &pIMsg)
	  : UserTransactionS2SEvt( heap, pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameInstance::JoinGameInstanceRes, { return OnJoinGameRes(pRes); } );
		// TODO: FIXME
		//BR_TRANS_MESSAGE( DB::QueryUpdateJoinGameCmd, { return OnUpdateDBRes(pRes); } );
		BR_TRANS_MESSAGE(Message::GameParty::LeavePartyRes, { return OnLeavePartyRes(pRes); });
	}

	Result PlayerTransGameMatchedS2SEvt::OnJoinGameRes( Svr::TransactionResult* &pRes )
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});
		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameInstance::JoinGameInstanceRes joinRes;

		if( !(pRes->GetResult()) )
		{
			NetSvrPolicyGame(GetMyOwner()->GetConnection()->GetMessageEndpoint()).GameMatchFailedS2CEvt(pRes->GetResult());
			goto Proc_End;
		}

		svrCheck( joinRes.ParseMessage( *pMsgRes->GetMessage() ) );

		//svrCheckPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		GetMyOwner()->SetGameInsUID( joinRes.GetRouteContext().GetFrom());


		// Consume cost
		m_WaitingQueires = 0;


		// FIXME: 
		NetSvrPolicyGame(GetMyOwner()->GetConnection()->GetMessageEndpoint()).GameMatchedS2CEvt(GetGameInsUID(), joinRes.GetTimeStamp(), 
			0, 0, 100,
			0, {}, 0, 0,
			{}, {},
			0, 0, 0
			/*pPlayerInfoSystem->GetStamina(), pPlayerInfoSystem->GetGem(), pPlayerInfoSystem->GetGameMoney()*/);


		if (GetMyOwner()->GetPartyUID().UID != 0)
		{
			auto serverEndpoint = Service::MessageEndpointManager->GetEndpoint(GetMyOwner()->GetPartyUID());

			svrCheck(NetPolicyGameParty(serverEndpoint).LeavePartyCmd(RouteContext(GetOwnerEntityUID(), GetMyOwner()->GetPartyUID()), GetTransID(), GetMyOwner()->GetPlayerID()) );

			m_WaitingQueires++;
		}

		if (m_WaitingQueires == 0)
		{
			CloseTransaction( hr );
		}

	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return ResultCode::SUCCESS;
	}

	Result PlayerTransGameMatchedS2SEvt::OnLeavePartyRes(Svr::TransactionResult* &pRes)
	{
		ScopeContext hr([this](Result hr)
			{
				m_WaitingQueires--;
				if (m_WaitingQueires <= 0)
				{
					CloseTransaction(hr);
				}
			});
		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameParty::LeavePartyRes leavePartyRes;

		svrCheckClose(pRes->GetResult());

		svrCheck(leavePartyRes.ParseMessage(*pMsgRes->GetMessage()));

		GetMyOwner()->SetPartyUID(0);

		return ResultCode::SUCCESS;
	}

	Result PlayerTransGameMatchedS2SEvt::OnUpdateDBRes(  Svr::TransactionResult* &pRes )
	{
		ScopeContext hr([this](Result hr)
			{
				m_WaitingQueires--;
				if (m_WaitingQueires <= 0)
				{
					CloseTransaction(hr);
				}
			});

		svrCheck( pRes->GetResult() );
		//DB::QueryUpdateJoinGameCmd *pMsgRes = (DB::QueryUpdateJoinGameCmd*)pRes;

		return hr;
	}

	// Start Transaction
	Result PlayerTransGameMatchedS2SEvt::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});

		GameInsUID insUID;

		svrCheck( super::StartTransaction() );

		if( GetDestPlayerID() != GetMyOwner()->GetPlayerID() )
			svrError(ResultCode::INVALID_PLAYERID);

		if( GetMyOwner()->GetGameInsUID().UID != 0 )
			svrError(ResultCode::GAME_ALREADY_IN_GAME);

		if( GetGameInsUID() == 0 )
			svrError(ResultCode::INVALID_INSTANCEID);

		// clear matching ticket
		GetMyOwner()->SetMatchingTicket(0);

		insUID = GetGameInsUID();

		auto serverEndpoint = Service::MessageEndpointManager->GetEndpoint(insUID);
		svrCheck(NetPolicyGameInstance(serverEndpoint).JoinGameInstanceCmd( RouteContext(GetOwnerEntityUID(),insUID), GetTransID(),
			GetMyOwner()->GetPlayerInformation(), GetMyOwner()->GetCharacterVisualData(), GetMyOwner()->GetCharacterData()));

		return ResultCode::SUCCESS;
	}

	// Start Transaction
	Result PlayerTransPartyQueuedGameMatchingS2CEvt::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});
		//NetPolicyGameParty *pPolicy = nullptr;
		//Svr::ServerEntity *pServerEntity = nullptr;

		svrCheck( super::StartTransaction() );

		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrError(ResultCode::SVR_ALREADY_INQUEUE);

		GetMyOwner()->SetMatchingTicket( GetMatchingQueueTicket() );

		NetSvrPolicyGame(GetMyOwner()->GetConnection()->GetMessageEndpoint()).GameMatchingStartedS2CEvt();

		return hr;
	}

	// Start Transaction
	Result PlayerTransPartyMatchingItemDequeuedS2CEvt::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});
		//NetPolicyGameParty *pPolicy = nullptr;
		//Svr::ServerEntity *pServerEntity = nullptr;

		svrCheck( super::StartTransaction() );

		if( GetMyOwner()->GetMatchingTicket() != GetMatchingTicket() )
			svrError(ResultCode::SVR_INVALID_QUEUEITEM);

		GetMyOwner()->SetMatchingTicket( 0 );

		return hr;
	}

	// Start Transaction
	Result PlayerTransPartyCanceledGameMatchingS2CEvt::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});
		//NetPolicyGameParty *pPolicy = nullptr;
		//Svr::ServerEntity *pServerEntity = nullptr;

		svrCheck( super::StartTransaction() );

		if( GetMatchingQueueTicket() != GetMyOwner()->GetMatchingTicket() )
			svrError(ResultCode::SVR_INVALID_QUEUEITEM);

		GetMyOwner()->SetMatchingTicket( 0 );

		NetSvrPolicyGame(GetMyOwner()->GetConnection()->GetMessageEndpoint()).GameMatchingCanceledS2CEvt();

		return hr;
	}



	PlayerTransCreateParty::PlayerTransCreateParty(IHeap& heap, const MessageDataPtr &pIMsg )
		:MessageTransaction(heap, pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GamePartyManager::CreatePartyRes, { return OnCreatePartyRes(pRes); } );
	}

	Result PlayerTransCreateParty::OnCreatePartyRes( Svr::TransactionResult* &pRes )
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GamePartyManager::CreatePartyRes res;

		svrCheck(pRes->GetResult());
		svrCheck( res.ParseMessage( *pMsgRes->GetMessage() ) );

		GetMyOwner()->SetPartyUID( res.GetRouteContext().GetFrom());
		m_PartyUID = res.GetRouteContext().GetFrom();

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransCreateParty::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});
		SharedPointerT<ServerServiceInformation> pService;

		svrCheck( super::StartTransaction() );

		// Can't do any party operations while you are in matching queue
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrorClose(ResultCode::SVR_ALREADY_INQUEUE);

		if( GetMyOwner()->GetGameInsUID().UID != 0 )
			svrErrorClose(ResultCode::GAME_ALREADY_IN_GAME);

		if( GetMyOwner()->GetPartyUID().UID != 0 )
			svrErrorClose(ResultCode::GAME_ALREADY_IN_PARTY);


		GetMyOwner()->UpdateGamePlayer();
		//if (GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GetStamina() < GetMyServer()->GetPresetGameConfig()->StaminaForGame)
		//{
		//	svrErrorClose(ResultCode::GAME_LOW_STAMINA);
		//}

		svrCheck( Service::ServiceDirectory->GetRandomService(Service::ServerConfig->GameClusterID, ClusterID::GamePartyManager, pService ) );

		svrCheck( pService->GetService<GamePartyManagerService>()->CreatePartyCmd(GetTransID(), 0, (uint32_t)Service::ServerConfig->GameClusterID, GetMyOwner()->GetPlayerInformation() ) );

		return hr;
	}


	PlayerTransJoinParty::PlayerTransJoinParty(IHeap& heap, const MessageDataPtr &pIMsg )
		: MessageTransaction( heap, pIMsg )
		, m_ChatHistoryData(heap)
	{
		BR_TRANS_MESSAGE( Message::GameParty::JoinPartyRes, { return OnJoinPartyRes(pRes); } );
	}

	Result PlayerTransJoinParty::OnJoinPartyRes( Svr::TransactionResult* &pRes )
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameParty::JoinPartyRes res;

		svrCheck(pRes->GetResult());
		svrCheck( res.ParseMessage( *pMsgRes->GetMessage() ) );

		GetMyOwner()->SetPartyUID( GetPartyUID() );

		m_LeaderID = res.GetPartyLeaderID();

		//assert(m_ChatHistoryData.GetAllocatedSize() >= res.GetChatHistoryData().size());
		m_ChatHistoryData.resize(std::min(m_ChatHistoryData.GetAllocatedSize(), res.GetChatHistoryData().size()));
		memcpy(m_ChatHistoryData.data(), res.GetChatHistoryData().data(), sizeof(uint8_t)*m_ChatHistoryData.size());

		GetMyOwner()->UpdateDBSync();

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransJoinParty::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});

		Svr::ServerEntity *pServerEntity = nullptr;
		//auto config = GetMyServer()->GetPresetGameConfig();

		m_LeaderID = 0;

		svrCheck( super::StartTransaction() );

		if( GetMyOwner()->GetGameInsUID().UID != 0 )
			svrError(ResultCode::GAME_ALREADY_IN_GAME);

		// Can't do any party operations while you are in matching queue
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrorClose(ResultCode::SVR_ALREADY_INQUEUE);

		if( GetMyOwner()->GetPartyUID().UID != 0 )
			svrError(ResultCode::GAME_ALREADY_IN_PARTY);

		// Need stamina to join a party
		//if (config != nullptr)
		//{
		//	//svrError(ResultCode::INVALID_STATE);
		//	GetMyOwner()->UpdateGamePlayer();
		//	if (GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GetStamina() < config->StaminaForGame)
		//		svrErrorClose(ResultCode::GAME_NOTENOUGH_RESOURCE);
		//}


		auto serverEndpoint = Service::MessageEndpointManager->GetEndpoint(GetPartyUID());
		//svrCheck( Service::ServerEntityManager->GetServerEntity( EntityUID(GetPartyUID()).GetServerID(), pServerEntity ) );

		svrCheck(NetPolicyGameParty(serverEndpoint).JoinPartyCmd(RouteContext(GetOwnerEntityUID(), GetPartyUID()), GetTransID(),
			GetInviterID(), GetMyOwner()->GetPlayerInformation() ) );

		return hr;
	}
	

	// Start Transaction
	Result PlayerTransPartyPlayerJoinedS2SEvt::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		svrCheck( super::StartTransaction() );

		if( GetMyOwner()->GetPartyUID() != GetRouteContext().GetFrom())
			svrErrorClose(ResultCode::GAME_INVALID_PARTYID);

		svrCheck(NetSvrPolicyGame(GetMyOwner()->GetConnection()->GetMessageEndpoint()).PartyPlayerJoinedS2CEvt( GetRouteContext().GetFrom(), GetJoinedPlayer() ) );
	
		return hr;
	}


	// Start Transaction
	Result PlayerTransPartyLeaderChangedS2SEvt::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		svrCheck( super::StartTransaction() );

		if( GetMyOwner()->GetPartyUID() != GetRouteContext().GetFrom())
			svrErrorClose(ResultCode::GAME_INVALID_PARTYID);

		svrCheck(NetSvrPolicyGame(GetMyOwner()->GetConnection()->GetMessageEndpoint()).PartyLeaderChangedS2CEvt( GetRouteContext().GetFrom(), GetNewLeaderID() ) );

		return hr;
	}



	PlayerTransLeaveParty::PlayerTransLeaveParty(IHeap& heap, const MessageDataPtr &pIMsg )
		: MessageTransaction( heap, pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameParty::LeavePartyRes, { return OnLeavePartyRes(pRes); } );
	}

	Result PlayerTransLeaveParty::OnLeavePartyRes( Svr::TransactionResult* &pRes )
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameParty::LeavePartyRes res;

		if (pRes->GetResult() == Result(ResultCode::SVR_INVALID_ENTITYUID))
		{
			GetMyOwner()->SetPartyUID(0);
			GetMyOwner()->UpdateDBSync();
		}
		else
		{
			svrCheck(pRes->GetResult());
			svrCheck(res.ParseMessage(*pMsgRes->GetMessage()));

			GetMyOwner()->SetPartyUID(0);
			GetMyOwner()->UpdateDBSync();
		}

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransLeaveParty::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});
		Svr::ServerEntity *pServerEntity = nullptr;

		svrCheck( super::StartTransaction() );

		// Can't do any party operations while you are in matching queue
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrorClose(ResultCode::SVR_ALREADY_INQUEUE);

		if( GetMyOwner()->GetPartyUID() == PartyUID(0) )
			svrError(ResultCode::GAME_INVALID_PARTYID);

		auto serverEndpoint = Service::MessageEndpointManager->GetEndpoint(GetPartyUID());
		//svrCheck( Service::ServerEntityManager->GetServerEntity( EntityUID(GetPartyUID()).GetServerID(), pServerEntity ) );

		svrCheck(NetPolicyGameParty(serverEndpoint).LeavePartyCmd( RouteContext(GetOwnerEntityUID(),GetMyOwner()->GetPartyUID()), GetTransID(),
			GetMyOwner()->GetPlayerID() ) );
	
		return hr;
	}
	
	
	
	// Start Transaction
	Result PlayerTransPartyPlayerLeftS2SEvt::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		svrCheck( super::StartTransaction() );

		if( GetMyOwner()->GetPartyUID() != GetRouteContext().GetFrom())
			svrErrorClose(ResultCode::GAME_INVALID_PARTYID);

		svrCheck(NetSvrPolicyGame(GetMyOwner()->GetConnection()->GetMessageEndpoint()).PartyPlayerLeftS2CEvt( GetRouteContext().GetFrom(), GetLeftPlayerID() ) );

		return hr;
	}
	


	
	

	PlayerTransPartyKickPlayer::PlayerTransPartyKickPlayer(IHeap& heap, const MessageDataPtr &pIMsg )
		: MessageTransaction( heap, pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameParty::KickPlayerRes, { return OnPlayerKickRes(pRes); } );
	}

	Result PlayerTransPartyKickPlayer::OnPlayerKickRes( Svr::TransactionResult* &pRes )
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		//Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		//Message::GameParty::KickPlayerRes res;

		svrCheckClose(pRes->GetResult());
		//svrCheck( res.ParseMessage( pMsgRes->GetMessage() ) );

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransPartyKickPlayer::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});
		Svr::ServerEntity *pServerEntity = nullptr;

		svrCheck( super::StartTransaction() );

		// Can't do any party operations while you are in matching queue
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrorClose(ResultCode::SVR_ALREADY_INQUEUE);

		if( GetMyOwner()->GetPartyUID().UID != GetPartyUID() )
			svrError(ResultCode::GAME_INVALID_PARTYID);

		auto serverEndpoint = Service::MessageEndpointManager->GetEndpoint(GetPartyUID());
		//svrCheck( Service::ServerEntityManager->GetServerEntity( EntityUID(GetPartyUID()).GetServerID(), pServerEntity ) );

		svrCheck(NetPolicyGameParty(serverEndpoint).KickPlayerCmd( RouteContext(GetOwnerEntityUID(),GetPartyUID()), GetTransID(),
			GetMyOwner()->GetPlayerID(), GetPlayerToKick() ) );

		return hr;
	}
	
	
	
	// Start Transaction
	Result PlayerTransPartyPlayerKickedS2SEvt::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		svrCheck( super::StartTransaction() );

		if( GetMyOwner()->GetPartyUID() != GetRouteContext().GetFrom())
			svrErrorClose(ResultCode::GAME_INVALID_PARTYID);

		svrCheck( NetSvrPolicyGame(GetMyOwner()->GetConnection()->GetMessageEndpoint()).PartyPlayerKickedS2CEvt( GetRouteContext().GetFrom(), GetKickedPlayerID() ) );

		if( GetKickedPlayerID() == GetMyOwner()->GetPlayerID() )
		{
			GetMyOwner()->SetPartyUID(0);
			GetMyOwner()->UpdateDBSync();
		}

		return ResultCode::SUCCESS;
	}
	

	
	PlayerTransPartyInvite::PlayerTransPartyInvite(IHeap& heap, const MessageDataPtr &pIMsg )
		: MessageTransaction(heap, pIMsg)
	{
		//BR_TRANS_MESSAGE( DB::QueryNotification_AddCmd, { return OnNotifyAdded(pRes); } );
	}


	// Start Transaction
	Result PlayerTransPartyInvite::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});
		Svr::ServerEntity *pServerEntity = nullptr;
		EntityUID playerUID;

		svrCheck( super::StartTransaction() );

		// Can't do any party operations while you are in matching queue
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrorClose(ResultCode::SVR_ALREADY_INQUEUE);

		if( GetMyOwner()->GetPartyUID() == PartyUID(0) )
			svrError(ResultCode::GAME_INVALID_PARTYID);

		//svrCheck( Svr::GetServerComponent<DB::AccountDB>()->Notification_Add( GetTransID(), GetInviteTargetID(), true, NotificationType::PartyRequest, GetMyOwner()->GetPlayerID(), GetMyOwner()->GetPartyUID(), GetMyOwner()->GetNickName() ) );

		// Find player and send notify
		if( Service::PlayerManager->FindPlayer( GetInviteTargetID(), playerUID ) )
		{
			auto serverEndpoint = Service::MessageEndpointManager->GetEndpoint(playerUID);
			//svrCheck( Service::ServerEntityManager->GetServerEntity( playerUID.GetServerID(), pServerEntity ) );

			svrCheck(NetPolicyGameServer(serverEndpoint).NotifyPartyInviteC2SEvt( RouteContext(GetOwnerEntityUID(),playerUID),
				GetInviteTargetID(), GetMyOwner()->GetPlayerID(), GetMyOwner()->GetNickName(), GetMyOwner()->GetPartyUID() ) );

			CloseTransaction( hr );
		}
		else
		{
			// Not online
			svrErrorClose(ResultCode::GAME_INVALID_PLAYER_STATE);
		}

		return hr;
	}



	// Start Transaction
	Result PlayerTransNotifyPartyInviteS2SEvt::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		svrCheck( super::StartTransaction() );

		if( GetMyOwner()->GetPartyUID().UID != 0 )
			svrErrorClose(ResultCode::GAME_INVALID_PARTYID);

		NetSvrPolicyGame(GetMyOwner()->GetConnection()->GetMessageEndpoint()).PartyInviteRequestedS2CEvt( GetInviterID(), GetInviterName(), GetPartyUID() );

		return hr;
	}



	// Start Transaction
	Result PlayerTransPartyChatMessage::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				svrTrace(SVR_INFO, "PartyChat PlayerID:{0}, hr:{1}", GetMyOwner()->GetPlayerID(), hr);

				CloseTransaction(hr);
			});
		Svr::ServerEntity *pServerEntity = nullptr;

		svrCheck( super::StartTransaction() );

		//if( GetMyOwner()->GetAuthTicket() != GetTicket() )
		//	svrError(ResultCode::E_INVALID_TICKET);

		//if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
		//	svrError(ResultCode::INVALID_PLAYERID);
		
		if( GetMyOwner()->GetPartyUID().UID == 0 )
			svrError(ResultCode::GAME_INVALID_PARTYID);


		auto serverEndpoint = Service::MessageEndpointManager->GetEndpoint(GetMyOwner()->GetPartyUID());
		//svrCheck( Service::ServerEntityManager->GetServerEntity( GetMyOwner()->GetPartyUID().GetServerID(), pServerEntity ) );

		svrCheck(NetPolicyGameParty(serverEndpoint).ChatMessageC2SEvt( RouteContext(GetOwnerEntityUID(),GetMyOwner()->GetPartyUID()),
			GetMyOwner()->GetPlayerID(), GetChatMessage() ) );

		return hr;

	}


	// Start Transaction
	Result PlayerTransPartyChatMessageS2CEvt::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				svrTrace(SVR_INFO, "PartyChat Received PlayerID:{0}, FromPlayerID:{1} hr:{2}", GetMyOwner()->GetPlayerID(), GetSenderID(), hr);

				CloseTransaction(hr);
			});

		svrCheck( super::StartTransaction() );

		svrCheck( NetSvrPolicyGame(GetMyOwner()->GetConnection()->GetMessageEndpoint()).PartyChatMessageS2CEvt( GetSenderID(), GetSenderName(), GetChatMessage() ) );

		return hr;

	}



	// Start Transaction
	Result PlayerTransPartyQuickChatMessage::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});
		Svr::ServerEntity *pServerEntity = nullptr;

		svrCheck( super::StartTransaction() );

		//if( GetMyOwner()->GetAuthTicket() != GetTicket() )
		//	svrError(ResultCode::E_INVALID_TICKET);

		//if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
		//	svrError(ResultCode::INVALID_PLAYERID);
		
		if( GetMyOwner()->GetPartyUID().UID == 0 )
			svrError(ResultCode::GAME_INVALID_PARTYID);

		auto serverEndpoint = Service::MessageEndpointManager->GetEndpoint(GetMyOwner()->GetPartyUID());
		//svrCheck( Service::ServerEntityManager->GetServerEntity( GetMyOwner()->GetPartyUID().GetServerID(), pServerEntity ) );

		svrCheck(NetPolicyGameParty(serverEndpoint).QuickChatMessageC2SEvt( RouteContext(GetOwnerEntityUID(),GetMyOwner()->GetPartyUID()),
			GetMyOwner()->GetPlayerID(), GetQuickChatID() ) );
	
		return hr;
	}


	// Start Transaction
	Result PlayerTransPartyQuickChatMessageS2CEvt::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		svrCheck( super::StartTransaction() );

		svrCheck( NetSvrPolicyGame(GetMyOwner()->GetConnection()->GetMessageEndpoint()).PartyQuickChatMessageS2CEvt( GetSenderID(), GetQuickChatID() ) );

		return ResultCode::SUCCESS;
	}


}// namespace GameServer 
}// namespace SF 

