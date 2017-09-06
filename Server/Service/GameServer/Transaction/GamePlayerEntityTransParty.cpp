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
#include "GameServer.h"
#include "GameServerClass.h"

#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "ResultCode/SFResultCodeLogin.h"
#include "Memory/MemoryPool.h"
#include "Types/SFEngineTypedefs.h"

#include "GameServerClass.h"
#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "Server/BrServer.h"
#include "ServerEntity/ServerEntityManager.h"

#include "ServerSystem/ServiceEntity/GamePartyManagerServiceEntity.h"
#include "ServerSystem/ServerService/GamePartyManagerService.h"
#include "ServerSystem/ServiceEntity/Game/GameClusterServiceEntity.h"

#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"

#include "Protocol/Message/GamePartyManagerMsgClass.h"
#include "Protocol/Policy/GamePartyManagerNetPolicy.h"

#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Message/GamePartyMsgClass.h"
#include "Protocol/Policy/GamePartyNetPolicy.h"

#include "GamePlayerEntityTransParty.h"
#include "GameInstance/GamePlayerEntity.h"

#include "GameSystem/UserFriendSystem.h"
#include "GameSystem/UserGamePlayerInfoSystem.h"

#include "DB/AccountDB.h"
#include "DB/AccountQuery.h"
#include "DB/GameConspiracyDB.h"
#include "DB/GameConspiracyQuery.h"


SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransGameMatchedS2SEvt);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransPartyQueuedGameMatchingS2CEvt);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransPartyCanceledGameMatchingS2CEvt);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransPartyMatchingItemDequeuedS2CEvt);

SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransCreateParty);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransJoinParty);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransPartyPlayerJoinedS2SEvt);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransPartyLeaderChangedS2SEvt);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransLeaveParty);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransPartyPlayerLeftS2SEvt);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransPartyKickPlayer);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransPartyPlayerKickedS2SEvt);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransPartyInvite);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransNotifyPartyInviteS2SEvt);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransPartyChatMessage);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransPartyChatMessageS2CEvt);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransPartyQuickChatMessage);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransPartyQuickChatMessageS2CEvt);




namespace SF {
namespace GameServer {


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Party
	//

	PlayerTransGameMatchedS2SEvt::PlayerTransGameMatchedS2SEvt(MessageDataPtr &pIMsg)
	  :UserTransactionS2SEvt( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameInstance::JoinGameRes, { return OnJoinGameRes(pRes); } );
		BR_TRANS_MESSAGE( DB::QueryUpdateJoinGameCmd, { return OnUpdateDBRes(pRes); } );
		BR_TRANS_MESSAGE(Message::GameParty::LeavePartyRes, { return OnLeavePartyRes(pRes); });
	}

	Result PlayerTransGameMatchedS2SEvt::OnJoinGameRes( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameInstance::JoinGameRes joinRes;
		UserGamePlayerInfoSystem* pPlayerInfoSystem = nullptr;

		if( !(pRes->GetResult()) )
		{
			GetMyOwner()->GetISvrGamePolicy()->GameMatchFailedS2CEvt(pRes->GetResult());
			goto Proc_End;
		}

		svrChk( joinRes.ParseMessage( pMsgRes->GetMessage() ) );

		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		GetMyOwner()->SetGameInsUID( joinRes.GetRouteContext().GetFrom());


		// Consume cost
		if (GetRequestedRole() != PlayerRole::None)
		{
			conspiracy::OrganicTbl::OrganicItem *pCostItem = nullptr;
			auto pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>();
			GetMyOwner()->UpdateGamePlayer();

			// Apply regardless of its error
			if ((conspiracy::OrganicTbl::FindItem((int)conspiracy::OrganicTbl::EItemEffect::Enum::RoleChoice, pCostItem)))
			{
				pPlayerInfoSystem->ApplyCost(pCostItem, TransLogCategory::Buy, "RoleSelection");
			}
		}

		m_WaitingQueires = 0;
		if( joinRes.GetIsNewJoin() && GetMyServer()->GetPresetGameConfig() != nullptr )
		{
			GetMyOwner()->UpdateGamePlayer();
			GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GainStamina( -GetMyServer()->GetPresetGameConfig()->StaminaForGame );

			if ((Svr::GetServerComponent<DB::GameConspiracyDB>()->UpdateJoinGameCmd(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(),
				pPlayerInfoSystem->GetGem(), pPlayerInfoSystem->GetStamina(),
				GetMyOwner()->GetIsInGame() != 0 ? 1 : 0,
				GetMyOwner()->GetLatestActiveTime(),
				GetMyOwner()->GetLatestUpdateTime())))
			{
				m_WaitingQueires++;
			}
		}


		GetMyOwner()->GetISvrGamePolicy()->GameMatchedS2CEvt(GetGameInsUID(), joinRes.GetTimeStamp(), joinRes.GetGameState(), joinRes.GetDay(), joinRes.GetMaxPlayer(),
			joinRes.GetPlayerIndex(), joinRes.GetPlayerCharacter(), joinRes.GetRole(), joinRes.GetDead(),
			joinRes.GetChatHistoryData(), joinRes.GetGameLogData(),
			pPlayerInfoSystem->GetStamina(), pPlayerInfoSystem->GetGem(), pPlayerInfoSystem->GetGameMoney());


		if (GetMyOwner()->GetPartyUID() != 0)
		{
			Policy::NetPolicyGameParty *pPolicy = nullptr;
			Svr::ServerEntity *pServerEntity = nullptr;

			svrChk(Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity(GetMyOwner()->GetPartyUID().GetServerID(), pServerEntity));
			svrChkPtr(pPolicy = pServerEntity->GetInterface<Policy::NetPolicyGameParty>());

			svrChk(pPolicy->LeavePartyCmd(RouteContext(GetOwnerEntityUID(), GetMyOwner()->GetPartyUID()), GetTransID(), GetMyOwner()->GetPlayerID()) );

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
		Result hr = ResultCode::SUCCESS;
		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameParty::LeavePartyRes leavePartyRes;

		svrChkClose(pRes->GetResult());

		svrChk(leavePartyRes.ParseMessage(pMsgRes->GetMessage()));

		GetMyOwner()->SetPartyUID(0);

	Proc_End:

		m_WaitingQueires--;
		if (m_WaitingQueires <= 0)
		{
			CloseTransaction(hr);
		}


		return ResultCode::SUCCESS;
	}

	Result PlayerTransGameMatchedS2SEvt::OnUpdateDBRes(  Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( pRes->GetResult() );
		//DB::QueryUpdateJoinGameCmd *pMsgRes = (DB::QueryUpdateJoinGameCmd*)pRes;


	Proc_End:

		m_WaitingQueires--;
		if (m_WaitingQueires <= 0)
		{
			CloseTransaction(hr);
		}

		return hr;
	}

	// Start Transaction
	Result PlayerTransGameMatchedS2SEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		GameInsUID insUID;
		Policy::NetPolicyGameInstance* pPolicy = nullptr;

		svrChk( super::StartTransaction() );

		if( GetDestPlayerID() != GetMyOwner()->GetPlayerID() )
			svrErr(ResultCode::INVALID_PLAYERID);

		if( GetMyOwner()->GetGameInsUID() != 0 )
			svrErr(ResultCode::E_GAME_ALREADY_IN_GAME);

		if( GetGameInsUID().UID == 0 )
			svrErr(ResultCode::E_INVALID_INSTANCEID);

		// clear matching ticket
		GetMyOwner()->SetMatchingTicket(0);

		insUID = GetGameInsUID();
		svrChkPtr( pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::NetPolicyGameInstance>(insUID.GetServerID()) );

		svrChk( pPolicy->JoinGameCmd( RouteContext(GetOwnerEntityUID(),insUID), GetTransID(),
			GetMyOwner()->GetPlayerInformation(), GetMyOwner()->GetAuthTicket(), GetRequestedRole()));

	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return ResultCode::SUCCESS;
	}

	// Start Transaction
	Result PlayerTransPartyQueuedGameMatchingS2CEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		//Policy::NetPolicyGameParty *pPolicy = nullptr;
		//Svr::ServerEntity *pServerEntity = nullptr;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErr(ResultCode::SVR_ALREADY_INQUEUE);

		GetMyOwner()->SetMatchingTicket( GetMatchingQueueTicket() );

		GetInterface()->GameMatchingStartedS2CEvt();

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}

	// Start Transaction
	Result PlayerTransPartyMatchingItemDequeuedS2CEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		//Policy::NetPolicyGameParty *pPolicy = nullptr;
		//Svr::ServerEntity *pServerEntity = nullptr;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetMatchingTicket() != GetMatchingTicket() )
			svrErr(ResultCode::SVR_INVALID_QUEUEITEM);

		GetMyOwner()->SetMatchingTicket( 0 );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}

	// Start Transaction
	Result PlayerTransPartyCanceledGameMatchingS2CEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		//Policy::NetPolicyGameParty *pPolicy = nullptr;
		//Svr::ServerEntity *pServerEntity = nullptr;

		svrChk( super::StartTransaction() );

		if( GetMatchingQueueTicket() != GetMyOwner()->GetMatchingTicket() )
			svrErr(ResultCode::SVR_INVALID_QUEUEITEM);

		GetMyOwner()->SetMatchingTicket( 0 );

		GetInterface()->GameMatchingCanceledS2CEvt();

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}



	PlayerTransCreateParty::PlayerTransCreateParty( MessageDataPtr &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GamePartyManager::CreatePartyRes, { return OnCreatePartyRes(pRes); } );
	}

	Result PlayerTransCreateParty::OnCreatePartyRes( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GamePartyManager::CreatePartyRes res;

		svrChk(pRes->GetResult());
		svrChk( res.ParseMessage( pMsgRes->GetMessage() ) );

		GetMyOwner()->SetPartyUID( res.GetRouteContext().GetFrom());
		m_PartyUID = res.GetRouteContext().GetFrom();

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransCreateParty::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ServerServiceInformation *pService = nullptr;

		svrChk( super::StartTransaction() );

		// Can't do any party operations while you are in matching queue
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrClose(ResultCode::SVR_ALREADY_INQUEUE);

		if( GetMyOwner()->GetGameInsUID() != 0 )
			svrErrClose(ResultCode::E_GAME_ALREADY_IN_GAME);

		if( GetMyOwner()->GetPartyUID() != 0 )
			svrErrClose(ResultCode::E_GAME_ALREADY_IN_PARTY);


		GetMyOwner()->UpdateGamePlayer();
		if (GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GetStamina() < GetMyServer()->GetPresetGameConfig()->StaminaForGame)
		{
			svrErrClose(ResultCode::E_GAME_LOW_STAMINA);
		}

		svrChk( Svr::GetServerComponent<Svr::GamePartyManagerServiceEntity>()->GetService( pService ) );

		svrChk( pService->GetService<Svr::GamePartyManagerService>()->CreatePartyCmd(GetTransID(), 0, GetMyOwner()->GetPlayerInformation() ) );

	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}
	

	

	PlayerTransJoinParty::PlayerTransJoinParty( MessageDataPtr &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameParty::JoinPartyRes, { return OnJoinPartyRes(pRes); } );
	}

	Result PlayerTransJoinParty::OnJoinPartyRes( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameParty::JoinPartyRes res;

		svrChk(pRes->GetResult());
		svrChk( res.ParseMessage( pMsgRes->GetMessage() ) );

		GetMyOwner()->SetPartyUID( GetPartyUID() );

		m_LeaderID = res.GetPartyLeaderID();

		Assert(m_ChatHistoryData.GetAllocatedSize() >= res.GetChatHistoryData().GetSize());
		m_ChatHistoryData.SetSize(std::min(m_ChatHistoryData.GetAllocatedSize(), res.GetChatHistoryData().GetSize()));
		memcpy(m_ChatHistoryData.data(), res.GetChatHistoryData().data(), sizeof(uint8_t)*m_ChatHistoryData.GetSize());

		GetMyOwner()->UpdateDBSync();

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransJoinParty::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Policy::NetPolicyGameParty *pPolicy = nullptr;
		Svr::ServerEntity *pServerEntity = nullptr;
		auto config = GetMyServer()->GetPresetGameConfig();

		m_LeaderID = 0;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetGameInsUID() != 0 )
			svrErr(ResultCode::E_GAME_ALREADY_IN_GAME);

		// Can't do any party operations while you are in matching queue
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrClose(ResultCode::SVR_ALREADY_INQUEUE);

		if( GetMyOwner()->GetPartyUID() != 0 )
			svrErr(ResultCode::E_GAME_ALREADY_IN_PARTY);

		// Need stamina to join a party
		if (config != nullptr)
		{
			//svrErr(ResultCode::INVALID_STATE);
			GetMyOwner()->UpdateGamePlayer();
			if (GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GetStamina() < config->StaminaForGame)
				svrErrClose(ResultCode::E_GAME_NOTENOUGH_RESOURCE);
		}

		svrChk( Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity( GetPartyUID().GetServerID(), pServerEntity ) );

		svrChkPtr( pPolicy = pServerEntity->GetInterface<Policy::NetPolicyGameParty>() );

		svrChk( pPolicy->JoinPartyCmd(RouteContext(GetOwnerEntityUID(), GetPartyUID()), GetTransID(),
			GetInviterID(), GetMyOwner()->GetPlayerInformation() ) );

	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}
	

	// Start Transaction
	Result PlayerTransPartyPlayerJoinedS2SEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetPartyUID() != GetRouteContext().GetFrom())
			svrErrClose(ResultCode::E_GAME_INVALID_PARTYID);

		svrChk( GetInterface()->PartyPlayerJoinedS2CEvt( GetRouteContext().GetFrom(), GetJoinedPlayer() ) );
	
	Proc_End:

		CloseTransaction( hr );

		return hr;
	}


	// Start Transaction
	Result PlayerTransPartyLeaderChangedS2SEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetPartyUID() != GetRouteContext().GetFrom())
			svrErrClose(ResultCode::E_GAME_INVALID_PARTYID);

		svrChk( GetInterface()->PartyLeaderChangedS2CEvt( GetRouteContext().GetFrom(), GetNewLeaderID() ) );
	
	Proc_End:

		CloseTransaction( hr );

		return hr;
	}



	PlayerTransLeaveParty::PlayerTransLeaveParty( MessageDataPtr &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameParty::LeavePartyRes, { return OnLeavePartyRes(pRes); } );
	}

	Result PlayerTransLeaveParty::OnLeavePartyRes( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameParty::LeavePartyRes res;

		if (pRes->GetResult() == Result(ResultCode::SVR_INVALID_ENTITYUID))
		{
			GetMyOwner()->SetPartyUID(0);
			GetMyOwner()->UpdateDBSync();
		}
		else
		{
			svrChk(pRes->GetResult());
			svrChk(res.ParseMessage(pMsgRes->GetMessage()));

			GetMyOwner()->SetPartyUID(0);
			GetMyOwner()->UpdateDBSync();
		}

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransLeaveParty::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Policy::NetPolicyGameParty *pPolicy = nullptr;
		Svr::ServerEntity *pServerEntity = nullptr;

		svrChk( super::StartTransaction() );

		// Can't do any party operations while you are in matching queue
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrClose(ResultCode::SVR_ALREADY_INQUEUE);

		if( GetMyOwner()->GetPartyUID() == PartyUID(0) )
			svrErr(ResultCode::E_GAME_INVALID_PARTYID);

		svrChk( Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity( GetPartyUID().GetServerID(), pServerEntity ) );
		svrChkPtr( pPolicy = pServerEntity->GetInterface<Policy::NetPolicyGameParty>() );

		svrChk( pPolicy->LeavePartyCmd( RouteContext(GetOwnerEntityUID(),GetMyOwner()->GetPartyUID()), GetTransID(),
			GetMyOwner()->GetPlayerID() ) );
	
	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}
	
	
	
	// Start Transaction
	Result PlayerTransPartyPlayerLeftS2SEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetPartyUID() != GetRouteContext().GetFrom())
			svrErrClose(ResultCode::E_GAME_INVALID_PARTYID);

		svrChkPtr(GetInterface());
		svrChk( GetInterface()->PartyPlayerLeftS2CEvt( GetRouteContext().GetFrom(), GetLeftPlayerID() ) );
	
	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	


	
	

	PlayerTransPartyKickPlayer::PlayerTransPartyKickPlayer( MessageDataPtr &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameParty::KickPlayerRes, { return OnPlayerKickRes(pRes); } );
	}

	Result PlayerTransPartyKickPlayer::OnPlayerKickRes( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;

		//Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		//Message::GameParty::KickPlayerRes res;

		svrChkClose(pRes->GetResult());
		//svrChk( res.ParseMessage( pMsgRes->GetMessage() ) );

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransPartyKickPlayer::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Policy::NetPolicyGameParty *pPolicy = nullptr;
		Svr::ServerEntity *pServerEntity = nullptr;

		svrChk( super::StartTransaction() );

		// Can't do any party operations while you are in matching queue
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrClose(ResultCode::SVR_ALREADY_INQUEUE);

		if( GetMyOwner()->GetPartyUID() != GetPartyUID() )
			svrErr(ResultCode::E_GAME_INVALID_PARTYID);

		svrChk( Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity( GetPartyUID().GetServerID(), pServerEntity ) );
		svrChkPtr( pPolicy = pServerEntity->GetInterface<Policy::NetPolicyGameParty>() );

		svrChk( pPolicy->KickPlayerCmd( RouteContext(GetOwnerEntityUID(),GetPartyUID()), GetTransID(),
			GetMyOwner()->GetPlayerID(), GetPlayerToKick() ) );
	
	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}
	
	
	
	// Start Transaction
	Result PlayerTransPartyPlayerKickedS2SEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetPartyUID() != GetRouteContext().GetFrom())
			svrErrClose(ResultCode::E_GAME_INVALID_PARTYID);

		svrChk( GetInterface()->PartyPlayerKickedS2CEvt( GetRouteContext().GetFrom(), GetKickedPlayerID() ) );

		if( GetKickedPlayerID() == GetMyOwner()->GetPlayerID() )
		{
			GetMyOwner()->SetPartyUID(0);
			GetMyOwner()->UpdateDBSync();
		}
	
	Proc_End:

		CloseTransaction( hr );

		return ResultCode::SUCCESS;
	}
	

	
	PlayerTransPartyInvite::PlayerTransPartyInvite( MessageDataPtr &pIMsg ):MessageTransaction(pIMsg)
	{
		//BR_TRANS_MESSAGE( DB::QueryNotification_AddCmd, { return OnNotifyAdded(pRes); } );
	}

	//Result PlayerTransPartyInvite::OnNotifyAdded(  Svr::TransactionResult* &pRes )
	//{
	//	Result hr = ResultCode::SUCCESS;
	//	Svr::ServerEntity *pServerEntity = nullptr;
	//	EntityUID playerUID;
	//	Policy::NetPolicyGameServer* pTargetPolicy = nullptr;

	//	svrChk( pRes->GetResult() );
	//	DB::QueryNotification_AddCmd *pMsgRes = (DB::QueryNotification_AddCmd*)pRes;


	//	// Find player and send notify
	//	if( (Svr::GetServerComponent<Svr::GameClusterServiceEntity>()->FindPlayer( GetInviteTargetID(), playerUID )) )
	//	{
	//		svrChk( Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity( playerUID.SvrID, pServerEntity ) );
	//		pTargetPolicy = pServerEntity->GetInterface<Policy::NetPolicyGameServer>();

	//		svrChk( pTargetPolicy->NotifyC2SEvt( RouteContext(GetOwnerEntityUID(),playerUID), 
	//			GetInviteTargetID(), pMsgRes->NotificationID, NotificationType::PartyRequest, GetMyOwner()->GetPlayerID(), GetMyOwner()->GetPartyUID(), GetMyOwner()->GetNickName() ) );
	//	}

	//Proc_End:

	//	CloseTransaction(hr);

	//	return hr;
	//	
	//}

	// Start Transaction
	Result PlayerTransPartyInvite::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ServerEntity *pServerEntity = nullptr;
		EntityUID playerUID;
		Policy::NetPolicyGameServer* pTargetPolicy = nullptr;

		svrChk( super::StartTransaction() );

		// Can't do any party operations while you are in matching queue
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrClose(ResultCode::SVR_ALREADY_INQUEUE);

		if( GetMyOwner()->GetPartyUID() == PartyUID(0) )
			svrErr(ResultCode::E_GAME_INVALID_PARTYID);

		//svrChk( Svr::GetServerComponent<DB::AccountDB>()->Notification_Add( GetTransID(), GetInviteTargetID(), true, NotificationType::PartyRequest, GetMyOwner()->GetPlayerID(), GetMyOwner()->GetPartyUID(), GetMyOwner()->GetNickName() ) );
		
		// Find player and send notify
		if( (Svr::GetServerComponent<Svr::GameClusterServiceEntity>()->FindPlayer( GetInviteTargetID(), playerUID )) )
		{
			svrChk( Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity( playerUID.GetServerID(), pServerEntity ) );
			svrChkPtr(pTargetPolicy = pServerEntity->GetInterface<Policy::NetPolicyGameServer>());

			svrChk( pTargetPolicy->NotifyPartyInviteC2SEvt( RouteContext(GetOwnerEntityUID(),playerUID), 
				GetInviteTargetID(), GetMyOwner()->GetPlayerID(), GetMyOwner()->GetNickName(), GetMyOwner()->GetPartyUID() ) );

			CloseTransaction( hr );
		}
		else
		{
			// Not online
			svrErrClose(ResultCode::E_GAME_INVALID_PLAYER_STATE);
		}

	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}



	// Start Transaction
	Result PlayerTransNotifyPartyInviteS2SEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetPartyUID() != 0 )
			svrErrClose(ResultCode::E_GAME_INVALID_PARTYID);

		GetInterface()->PartyInviteRequestedS2CEvt( GetInviterID(), GetInviterName(), GetPartyUID() );
	
	Proc_End:

		CloseTransaction( hr );

		return hr;
	}



	// Start Transaction
	Result PlayerTransPartyChatMessage::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ServerEntity *pServerEntity = nullptr;
		Policy::NetPolicyGameParty *pPolicy = nullptr;

		svrChk( super::StartTransaction() );

		//if( GetMyOwner()->GetAuthTicket() != GetTicket() )
		//	svrErr(ResultCode::E_INVALID_TICKET);

		//if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
		//	svrErr(ResultCode::INVALID_PLAYERID);
		
		if( GetMyOwner()->GetPartyUID() == 0 )
			svrErr(ResultCode::E_GAME_INVALID_PARTYID);

		svrChk( Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity( GetMyOwner()->GetPartyUID().GetServerID(), pServerEntity ) );
		svrChkPtr( pPolicy = pServerEntity->GetInterface<Policy::NetPolicyGameParty>() );

		svrChk( pPolicy->ChatMessageC2SEvt( RouteContext(GetOwnerEntityUID(),GetMyOwner()->GetPartyUID()), 
			GetMyOwner()->GetPlayerID(), GetChatMessage() ) );

	Proc_End:

		svrTrace(SVR_INFO, "PartyChat PlayerID:{0}, hr:{1}", GetMyOwner()->GetPlayerID(), hr);

		CloseTransaction( hr );

		return hr;

	}


	// Start Transaction
	Result PlayerTransPartyChatMessageS2CEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		svrChk( GetInterface()->PartyChatMessageS2CEvt( GetSenderID(), GetSenderName(), GetChatMessage() ) );

	Proc_End:

		svrTrace(SVR_INFO, "PartyChat Received PlayerID:{0}, FromPlayerID:{1} hr:{2}", GetMyOwner()->GetPlayerID(), GetSenderID(), hr);

		CloseTransaction( hr );

		return hr;

	}



	// Start Transaction
	Result PlayerTransPartyQuickChatMessage::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ServerEntity *pServerEntity = nullptr;
		Policy::NetPolicyGameParty *pPolicy = nullptr;

		svrChk( super::StartTransaction() );

		//if( GetMyOwner()->GetAuthTicket() != GetTicket() )
		//	svrErr(ResultCode::E_INVALID_TICKET);

		//if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
		//	svrErr(ResultCode::INVALID_PLAYERID);
		
		if( GetMyOwner()->GetPartyUID() == 0 )
			svrErr(ResultCode::E_GAME_INVALID_PARTYID);

		svrChk( Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity( GetMyOwner()->GetPartyUID().GetServerID(), pServerEntity ) );
		svrChkPtr( pPolicy = pServerEntity->GetInterface<Policy::NetPolicyGameParty>() );

		svrChk( pPolicy->QuickChatMessageC2SEvt( RouteContext(GetOwnerEntityUID(),GetMyOwner()->GetPartyUID()), 
			GetMyOwner()->GetPlayerID(), GetQuickChatID() ) );
	
	Proc_End:

		CloseTransaction( hr );

		return hr;
	}


	// Start Transaction
	Result PlayerTransPartyQuickChatMessageS2CEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		svrChk( GetInterface()->PartyQuickChatMessageS2CEvt( GetSenderID(), GetQuickChatID() ) );

	Proc_End:

		CloseTransaction( hr );

		return ResultCode::SUCCESS;
	}


};// namespace GameServer 
};// namespace SF 

