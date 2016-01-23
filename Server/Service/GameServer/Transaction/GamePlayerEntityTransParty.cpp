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

#include "Common/ResultCode/BRResultCodeCommon.h"
#include "Common/ResultCode/BRResultCodeGame.h"
#include "Common/ResultCode/BRResultCodeLogin.h"
#include "Common/MemoryPool.h"
#include "Common/BrBaseTypes.h"

#include "GameServerClass.h"
#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/ServerEntityManager.h"

#include "ServerSystem/ServiceEntity/GamePartyManagerServiceEntity.h"
#include "ServerSystem/ServerService/GamePartyManagerService.h"
#include "ServerSystem/ServiceEntity/GameClusterServiceEntity.h"

#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceIPolicy.h"

#include "Protocol/Message/GamePartyManagerMsgClass.h"
#include "Protocol/Policy/GamePartyManagerIPolicy.h"

#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Message/GamePartyMsgClass.h"
#include "Protocol/Policy/GamePartyIPolicy.h"

#include "GamePlayerEntityTransParty.h"
#include "GameInstance/GamePlayerEntity.h"

#include "GameSystem/UserFriendSystem.h"
#include "GameSystem/UserGamePlayerInfoSystem.h"

#include "DB/AccountDB.h"
#include "DB/AccountQuery.h"
#include "DB/GameConspiracyDB.h"
#include "DB/GameConspiracyQuery.h"


BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransGameMatchedS2SEvt);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransPartyQueuedGameMatchingS2CEvt);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransPartyCanceledGameMatchingS2CEvt);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransPartyMatchingItemDequeuedS2CEvt);

BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransCreateParty);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransJoinParty);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransPartyPlayerJoinedS2SEvt);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransPartyLeaderChangedS2SEvt);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransLeaveParty);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransPartyPlayerLeftS2SEvt);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransPartyKickPlayer);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransPartyPlayerKickedS2SEvt);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransPartyInvite);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransNotifyPartyInviteS2SEvt);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransPartyChatMessage);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransPartyChatMessageS2CEvt);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransPartyQuickChatMessage);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransPartyQuickChatMessageS2CEvt);




namespace BR {
namespace GameServer {


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Party
	//

	PlayerTransGameMatchedS2SEvt::PlayerTransGameMatchedS2SEvt(Message::MessageData* &pIMsg)
	  :UserTransactionS2SEvt( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameInstance::JoinGameRes, { return OnJoinGameRes(pRes); } );
		BR_TRANS_MESSAGE( DB::QueryUpdateJoinGameCmd, { return OnUpdateDBRes(pRes); } );
		BR_TRANS_MESSAGE(Message::GameParty::LeavePartyRes, { return OnLeavePartyRes(pRes); });
	}

	HRESULT PlayerTransGameMatchedS2SEvt::OnJoinGameRes( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_SYSTEM_OK;
		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameInstance::JoinGameRes joinRes;
		UserGamePlayerInfoSystem* pPlayerInfoSystem = nullptr;

		if( FAILED(pRes->GetHRESULT()) )
		{
			GetMyOwner()->GetISvrGamePolicy()->GameMatchFailedS2CEvt(pRes->GetHRESULT());
			goto Proc_End;
		}

		svrChk( joinRes.ParseIMsg( pMsgRes->GetMessage() ) );

		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		GetMyOwner()->SetGameInsUID( joinRes.GetRouteContext().GetFrom());


		// Consume cost
		if (GetRequestedRole() != PlayerRole::None)
		{
			conspiracy::OrganicTbl::OrganicItem *pCostItem = nullptr;
			auto pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>();
			GetMyOwner()->UpdateGamePlayer();

			// Apply regardless of its error
			if (SUCCEEDED(conspiracy::OrganicTbl::FindItem((int)conspiracy::OrganicTbl::EItemEffect::Enum::RoleChoice, pCostItem)))
			{
				pPlayerInfoSystem->ApplyCost(pCostItem, TransLogCategory::Buy, "RoleSelection");
			}
		}

		m_WaitingQueires = 0;
		if( joinRes.GetIsNewJoin() && GetMyServer()->GetPresetGameConfig() != nullptr )
		{
			GetMyOwner()->UpdateGamePlayer();
			GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GainStamina( -GetMyServer()->GetPresetGameConfig()->StaminaForGame );

			if (SUCCEEDED(Svr::GetServerComponent<DB::GameConspiracyDB>()->UpdateJoinGameCmd(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(),
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
			Policy::IPolicyGameParty *pPolicy = nullptr;
			Svr::ServerEntity *pServerEntity = nullptr;

			svrChk(Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity(GetMyOwner()->GetPartyUID().GetServerID(), pServerEntity));
			svrChkPtr(pPolicy = pServerEntity->GetPolicy<Policy::IPolicyGameParty>());

			svrChk(pPolicy->LeavePartyCmd(GetTransID(), RouteContext(GetOwnerEntityUID(), GetMyOwner()->GetPartyUID()), GetMyOwner()->GetPlayerID()));

			m_WaitingQueires++;
		}

		if (m_WaitingQueires == 0)
		{
			CloseTransaction( hr );
		}

	Proc_End:

		if( FAILED(hr) )
			CloseTransaction( hr );

		return S_SYSTEM_OK;
	}

	HRESULT PlayerTransGameMatchedS2SEvt::OnLeavePartyRes(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_SYSTEM_OK;
		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameParty::LeavePartyRes leavePartyRes;

		svrChkClose(pRes->GetHRESULT());

		svrChk(leavePartyRes.ParseIMsg(pMsgRes->GetMessage()));

		GetMyOwner()->SetPartyUID(0);

	Proc_End:

		m_WaitingQueires--;
		if (m_WaitingQueires <= 0)
		{
			CloseTransaction(hr);
		}


		return S_SYSTEM_OK;
	}

	HRESULT PlayerTransGameMatchedS2SEvt::OnUpdateDBRes(  Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk( pRes->GetHRESULT() );
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
	HRESULT PlayerTransGameMatchedS2SEvt::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;
		GameInsUID insUID;
		Policy::IPolicyGameInstance* pPolicy = nullptr;

		svrChk( super::StartTransaction() );

		if( GetDestPlayerID() != GetMyOwner()->GetPlayerID() )
			svrErr(E_INVALID_PLAYERID);

		if( GetMyOwner()->GetGameInsUID() != 0 )
			svrErr(E_GAME_ALREADY_IN_GAME);

		if( GetGameInsUID().UID == 0 )
			svrErr(E_INVALID_INSTANCEID);

		// clear matching ticket
		GetMyOwner()->SetMatchingTicket(0);

		insUID = GetGameInsUID();
		svrChkPtr( pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(insUID.GetServerID()) );

		svrChk( pPolicy->JoinGameCmd( GetTransID(), RouteContext(GetOwnerEntityUID(),insUID),
			GetMyOwner()->GetPlayerInformation(), GetMyOwner()->GetAuthTicket(), GetRequestedRole()));

	Proc_End:

		if( FAILED(hr) )
			CloseTransaction( hr );

		return S_SYSTEM_OK;
	}

	// Start Transaction
	HRESULT PlayerTransPartyQueuedGameMatchingS2CEvt::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;
		//Policy::IPolicyGameParty *pPolicy = nullptr;
		//Svr::ServerEntity *pServerEntity = nullptr;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErr(E_SVR_ALREADY_INQUEUE);

		GetMyOwner()->SetMatchingTicket( GetMatchingQueueTicket() );

		GetPolicy()->GameMatchingStartedS2CEvt();

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}

	// Start Transaction
	HRESULT PlayerTransPartyMatchingItemDequeuedS2CEvt::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;
		//Policy::IPolicyGameParty *pPolicy = nullptr;
		//Svr::ServerEntity *pServerEntity = nullptr;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetMatchingTicket() != GetMatchingTicket() )
			svrErr(E_SVR_INVALID_QUEUEITEM);

		GetMyOwner()->SetMatchingTicket( 0 );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}

	// Start Transaction
	HRESULT PlayerTransPartyCanceledGameMatchingS2CEvt::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;
		//Policy::IPolicyGameParty *pPolicy = nullptr;
		//Svr::ServerEntity *pServerEntity = nullptr;

		svrChk( super::StartTransaction() );

		if( GetMatchingQueueTicket() != GetMyOwner()->GetMatchingTicket() )
			svrErr(E_SVR_INVALID_QUEUEITEM);

		GetMyOwner()->SetMatchingTicket( 0 );

		GetPolicy()->GameMatchingCanceledS2CEvt();

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}



	PlayerTransCreateParty::PlayerTransCreateParty( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GamePartyManager::CreatePartyRes, { return OnCreatePartyRes(pRes); } );
	}

	HRESULT PlayerTransCreateParty::OnCreatePartyRes( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_SYSTEM_OK;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GamePartyManager::CreatePartyRes res;

		svrChk(pRes->GetHRESULT());
		svrChk( res.ParseIMsg( pMsgRes->GetMessage() ) );

		GetMyOwner()->SetPartyUID( res.GetRouteContext().GetFrom());
		m_PartyUID = res.GetRouteContext().GetFrom();

	Proc_End:

		CloseTransaction(hr);

		return S_SYSTEM_OK; 
	}

	// Start Transaction
	HRESULT PlayerTransCreateParty::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;
		Svr::ServerServiceInformation *pService = nullptr;

		svrChk( super::StartTransaction() );

		// Can't do any party operations while you are in matching queue
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrClose(E_SVR_ALREADY_INQUEUE);

		if( GetMyOwner()->GetGameInsUID() != 0 )
			svrErrClose(E_GAME_ALREADY_IN_GAME);

		if( GetMyOwner()->GetPartyUID() != 0 )
			svrErrClose(E_GAME_ALREADY_IN_PARTY);


		GetMyOwner()->UpdateGamePlayer();
		if (GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GetStamina() < GetMyServer()->GetPresetGameConfig()->StaminaForGame)
		{
			svrErrClose(E_GAME_LOW_STAMINA);
		}

		svrChk( Svr::GetServerComponent<Svr::GamePartyManagerServiceEntity>()->GetService( pService ) );

		svrChk( pService->GetService<Svr::GamePartyManagerService>()->CreatePartyCmd(  GetTransID(), 0, GetMyOwner()->GetPlayerInformation() ) );

	Proc_End:

		if( FAILED(hr) )
			CloseTransaction( hr );

		return hr;
	}
	

	

	PlayerTransJoinParty::PlayerTransJoinParty( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameParty::JoinPartyRes, { return OnJoinPartyRes(pRes); } );
	}

	HRESULT PlayerTransJoinParty::OnJoinPartyRes( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_SYSTEM_OK;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameParty::JoinPartyRes res;

		svrChk(pRes->GetHRESULT());
		svrChk( res.ParseIMsg( pMsgRes->GetMessage() ) );

		GetMyOwner()->SetPartyUID( GetPartyUID() );

		m_LeaderID = res.GetPartyLeaderID();

		Assert(m_ChatHistoryData.GetAllocatedSize() >= res.GetChatHistoryData().GetSize());
		m_ChatHistoryData.SetSize(std::min(m_ChatHistoryData.GetAllocatedSize(), res.GetChatHistoryData().GetSize()));
		memcpy(m_ChatHistoryData.data(), res.GetChatHistoryData().data(), sizeof(BYTE)*m_ChatHistoryData.GetSize());

		GetMyOwner()->UpdateDBSync(0);

	Proc_End:

		CloseTransaction(hr);

		return S_SYSTEM_OK; 
	}

	// Start Transaction
	HRESULT PlayerTransJoinParty::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;
		Policy::IPolicyGameParty *pPolicy = nullptr;
		Svr::ServerEntity *pServerEntity = nullptr;
		auto config = GetMyServer()->GetPresetGameConfig();

		m_LeaderID = 0;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetGameInsUID() != 0 )
			svrErr(E_GAME_ALREADY_IN_GAME);

		// Can't do any party operations while you are in matching queue
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrClose(E_SVR_ALREADY_INQUEUE);

		if( GetMyOwner()->GetPartyUID() != 0 )
			svrErr(E_GAME_ALREADY_IN_PARTY);

		// Need stamina to join a party
		if (config != nullptr)
		{
			//svrErr(E_INVALID_STATE);
			GetMyOwner()->UpdateGamePlayer();
			if (GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GetStamina() < config->StaminaForGame)
				svrErrClose(E_GAME_NOTENOUGH_RESOURCE);
		}

		svrChk( Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity( GetPartyUID().GetServerID(), pServerEntity ) );

		svrChkPtr( pPolicy = pServerEntity->GetPolicy<Policy::IPolicyGameParty>() );

		svrChk( pPolicy->JoinPartyCmd( GetTransID(), 
			RouteContext(GetOwnerEntityUID(),GetPartyUID()), GetInviterID(), GetMyOwner()->GetPlayerInformation() ) );

	Proc_End:

		if( FAILED(hr) )
			CloseTransaction( hr );

		return hr;
	}
	

	// Start Transaction
	HRESULT PlayerTransPartyPlayerJoinedS2SEvt::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetPartyUID() != GetRouteContext().GetFrom())
			svrErrClose(E_GAME_INVALID_PARTYID);

		svrChk( GetPolicy()->PartyPlayerJoinedS2CEvt( GetRouteContext().GetFrom(), GetJoinedPlayer() ) );
	
	Proc_End:

		CloseTransaction( hr );

		return hr;
	}


	// Start Transaction
	HRESULT PlayerTransPartyLeaderChangedS2SEvt::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetPartyUID() != GetRouteContext().GetFrom())
			svrErrClose(E_GAME_INVALID_PARTYID);

		svrChk( GetPolicy()->PartyLeaderChangedS2CEvt( GetRouteContext().GetFrom(), GetNewLeaderID() ) );
	
	Proc_End:

		CloseTransaction( hr );

		return hr;
	}



	PlayerTransLeaveParty::PlayerTransLeaveParty( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameParty::LeavePartyRes, { return OnLeavePartyRes(pRes); } );
	}

	HRESULT PlayerTransLeaveParty::OnLeavePartyRes( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_SYSTEM_OK;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameParty::LeavePartyRes res;

		if (pRes->GetHRESULT() == E_SVR_INVALID_ENTITYUID)
		{
			GetMyOwner()->SetPartyUID(0);
			GetMyOwner()->UpdateDBSync(0);
		}
		else
		{
			svrChk(pRes->GetHRESULT());
			svrChk(res.ParseIMsg(pMsgRes->GetMessage()));

			GetMyOwner()->SetPartyUID(0);
			GetMyOwner()->UpdateDBSync(0);
		}

	Proc_End:

		CloseTransaction(hr);

		return S_SYSTEM_OK; 
	}

	// Start Transaction
	HRESULT PlayerTransLeaveParty::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;
		Policy::IPolicyGameParty *pPolicy = nullptr;
		Svr::ServerEntity *pServerEntity = nullptr;

		svrChk( super::StartTransaction() );

		// Can't do any party operations while you are in matching queue
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrClose(E_SVR_ALREADY_INQUEUE);

		if( GetMyOwner()->GetPartyUID() == PartyUID(0) )
			svrErr(E_GAME_INVALID_PARTYID);

		svrChk( Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity( GetPartyUID().GetServerID(), pServerEntity ) );
		svrChkPtr( pPolicy = pServerEntity->GetPolicy<Policy::IPolicyGameParty>() );

		svrChk( pPolicy->LeavePartyCmd( GetTransID(), RouteContext(GetOwnerEntityUID(),GetMyOwner()->GetPartyUID()), 
			GetMyOwner()->GetPlayerID() ) );
	
	Proc_End:

		if( FAILED(hr) )
			CloseTransaction( hr );

		return hr;
	}
	
	
	
	// Start Transaction
	HRESULT PlayerTransPartyPlayerLeftS2SEvt::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetPartyUID() != GetRouteContext().GetFrom())
			svrErrClose(E_GAME_INVALID_PARTYID);

		svrChk( GetPolicy()->PartyPlayerLeftS2CEvt( GetRouteContext().GetFrom(), GetLeftPlayerID() ) );
	
	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	


	
	

	PlayerTransPartyKickPlayer::PlayerTransPartyKickPlayer( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameParty::KickPlayerRes, { return OnPlayerKickRes(pRes); } );
	}

	HRESULT PlayerTransPartyKickPlayer::OnPlayerKickRes( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_SYSTEM_OK;

		//Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		//Message::GameParty::KickPlayerRes res;

		svrChkClose(pRes->GetHRESULT());
		//svrChk( res.ParseIMsg( pMsgRes->GetMessage() ) );

	Proc_End:

		CloseTransaction(hr);

		return S_SYSTEM_OK; 
	}

	// Start Transaction
	HRESULT PlayerTransPartyKickPlayer::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;
		Policy::IPolicyGameParty *pPolicy = nullptr;
		Svr::ServerEntity *pServerEntity = nullptr;

		svrChk( super::StartTransaction() );

		// Can't do any party operations while you are in matching queue
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrClose(E_SVR_ALREADY_INQUEUE);

		if( GetMyOwner()->GetPartyUID() != GetPartyUID() )
			svrErr(E_GAME_INVALID_PARTYID);

		svrChk( Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity( GetPartyUID().GetServerID(), pServerEntity ) );
		svrChkPtr( pPolicy = pServerEntity->GetPolicy<Policy::IPolicyGameParty>() );

		svrChk( pPolicy->KickPlayerCmd( GetTransID(), RouteContext(GetOwnerEntityUID(),GetPartyUID()), 
			GetMyOwner()->GetPlayerID(), GetPlayerToKick() ) );
	
	Proc_End:

		if( FAILED(hr) )
			CloseTransaction( hr );

		return hr;
	}
	
	
	
	// Start Transaction
	HRESULT PlayerTransPartyPlayerKickedS2SEvt::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetPartyUID() != GetRouteContext().GetFrom())
			svrErrClose(E_GAME_INVALID_PARTYID);

		svrChk( GetPolicy()->PartyPlayerKickedS2CEvt( GetRouteContext().GetFrom(), GetKickedPlayerID() ) );

		if( GetKickedPlayerID() == GetMyOwner()->GetPlayerID() )
		{
			GetMyOwner()->SetPartyUID(0);
			GetMyOwner()->UpdateDBSync(0);
		}
	
	Proc_End:

		CloseTransaction( hr );

		return S_SYSTEM_OK;
	}
	

	
	PlayerTransPartyInvite::PlayerTransPartyInvite( Message::MessageData* &pIMsg ):MessageTransaction(pIMsg)
	{
		//BR_TRANS_MESSAGE( DB::QueryNotification_AddCmd, { return OnNotifyAdded(pRes); } );
	}

	//HRESULT PlayerTransPartyInvite::OnNotifyAdded(  Svr::TransactionResult* &pRes )
	//{
	//	HRESULT hr = S_SYSTEM_OK;
	//	Svr::ServerEntity *pServerEntity = nullptr;
	//	EntityUID playerUID;
	//	Policy::IPolicyGameServer* pTargetPolicy = nullptr;

	//	svrChk( pRes->GetHRESULT() );
	//	DB::QueryNotification_AddCmd *pMsgRes = (DB::QueryNotification_AddCmd*)pRes;


	//	// Find player and send notify
	//	if( SUCCEEDED(Svr::GetServerComponent<Svr::GameClusterServiceEntity>()->FindPlayer( GetInviteTargetID(), playerUID )) )
	//	{
	//		svrChk( Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity( playerUID.SvrID, pServerEntity ) );
	//		pTargetPolicy = pServerEntity->GetPolicy<Policy::IPolicyGameServer>();

	//		svrChk( pTargetPolicy->NotifyC2SEvt( RouteContext(GetOwnerEntityUID(),playerUID), 
	//			GetInviteTargetID(), pMsgRes->NotificationID, NotificationType::PartyRequest, GetMyOwner()->GetPlayerID(), GetMyOwner()->GetPartyUID(), GetMyOwner()->GetNickName() ) );
	//	}

	//Proc_End:

	//	CloseTransaction(hr);

	//	return hr;
	//	
	//}

	// Start Transaction
	HRESULT PlayerTransPartyInvite::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;
		Svr::ServerEntity *pServerEntity = nullptr;
		EntityUID playerUID;
		Policy::IPolicyGameServer* pTargetPolicy = nullptr;

		svrChk( super::StartTransaction() );

		// Can't do any party operations while you are in matching queue
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrClose(E_SVR_ALREADY_INQUEUE);

		if( GetMyOwner()->GetPartyUID() == PartyUID(0) )
			svrErr(E_GAME_INVALID_PARTYID);

		//svrChk( Svr::GetServerComponent<DB::AccountDB>()->Notification_Add( GetTransID(), GetInviteTargetID(), true, NotificationType::PartyRequest, GetMyOwner()->GetPlayerID(), GetMyOwner()->GetPartyUID(), GetMyOwner()->GetNickName() ) );
		
		// Find player and send notify
		if( SUCCEEDED(Svr::GetServerComponent<Svr::GameClusterServiceEntity>()->FindPlayer( GetInviteTargetID(), playerUID )) )
		{
			svrChk( Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity( playerUID.GetServerID(), pServerEntity ) );
			svrChkPtr(pTargetPolicy = pServerEntity->GetPolicy<Policy::IPolicyGameServer>());

			svrChk( pTargetPolicy->NotifyPartyInviteC2SEvt( RouteContext(GetOwnerEntityUID(),playerUID), 
				GetInviteTargetID(), GetMyOwner()->GetPlayerID(), GetMyOwner()->GetNickName(), GetMyOwner()->GetPartyUID() ) );

			CloseTransaction( hr );
		}
		else
		{
			// Not online
			svrErrClose(E_GAME_INVALID_PLAYER_STATE);
		}

	Proc_End:

		if( FAILED(hr) )
			CloseTransaction( hr );

		return hr;
	}



	// Start Transaction
	HRESULT PlayerTransNotifyPartyInviteS2SEvt::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetPartyUID() != 0 )
			svrErrClose(E_GAME_INVALID_PARTYID);

		GetPolicy()->PartyInviteRequestedS2CEvt( GetInviterID(), GetInviterName(), GetPartyUID() );
	
	Proc_End:

		CloseTransaction( hr );

		return hr;
	}



	// Start Transaction
	HRESULT PlayerTransPartyChatMessage::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;
		Svr::ServerEntity *pServerEntity = nullptr;
		Policy::IPolicyGameParty *pPolicy = nullptr;

		svrChk( super::StartTransaction() );

		//if( GetMyOwner()->GetAuthTicket() != GetTicket() )
		//	svrErr(E_INVALID_TICKET);

		//if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
		//	svrErr(E_INVALID_PLAYERID);
		
		if( GetMyOwner()->GetPartyUID() == 0 )
			svrErr(E_GAME_INVALID_PARTYID);

		svrChk( Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity( GetMyOwner()->GetPartyUID().GetServerID(), pServerEntity ) );
		svrChkPtr( pPolicy = pServerEntity->GetPolicy<Policy::IPolicyGameParty>() );

		svrChk( pPolicy->ChatMessageC2SEvt( RouteContext(GetOwnerEntityUID(),GetMyOwner()->GetPartyUID()), 
			GetMyOwner()->GetPlayerID(), GetChatMessage() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;

	}


	// Start Transaction
	HRESULT PlayerTransPartyChatMessageS2CEvt::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk( super::StartTransaction() );

		svrChk( GetPolicy()->PartyChatMessageS2CEvt( GetSenderID(), GetSenderName(), GetChatMessage() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;

	}



	// Start Transaction
	HRESULT PlayerTransPartyQuickChatMessage::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;
		Svr::ServerEntity *pServerEntity = nullptr;
		Policy::IPolicyGameParty *pPolicy = nullptr;

		svrChk( super::StartTransaction() );

		//if( GetMyOwner()->GetAuthTicket() != GetTicket() )
		//	svrErr(E_INVALID_TICKET);

		//if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
		//	svrErr(E_INVALID_PLAYERID);
		
		if( GetMyOwner()->GetPartyUID() == 0 )
			svrErr(E_GAME_INVALID_PARTYID);

		svrChk( Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity( GetMyOwner()->GetPartyUID().GetServerID(), pServerEntity ) );
		svrChkPtr( pPolicy = pServerEntity->GetPolicy<Policy::IPolicyGameParty>() );

		svrChk( pPolicy->QuickChatMessageC2SEvt( RouteContext(GetOwnerEntityUID(),GetMyOwner()->GetPartyUID()), 
			GetMyOwner()->GetPlayerID(), GetQuickChatID() ) );
	
	Proc_End:

		CloseTransaction( hr );

		return hr;
	}


	// Start Transaction
	HRESULT PlayerTransPartyQuickChatMessageS2CEvt::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk( super::StartTransaction() );

		svrChk( GetPolicy()->PartyQuickChatMessageS2CEvt( GetSenderID(), GetQuickChatID() ) );

	Proc_End:

		CloseTransaction( hr );

		return S_SYSTEM_OK;
	}


};// namespace GameServer 
};// namespace BR 

