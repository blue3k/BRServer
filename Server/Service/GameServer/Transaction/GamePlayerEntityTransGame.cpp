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

#include "Common/HRESCommon.h"
#include "Common/HRESGame.h"
#include "Common/HRESLogin.h"
#include "Common/MemoryPool.h"
#include "Common/BRBaseTypes.h"
#include "Common/ToStringGame.h"

#include "GameServerClass.h"
#include "ServerSystem/BRServerUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/ServerEntityManager.h"

#include "ServerSystem/ServerService/PartyMatchingQueueService.h"
#include "ServerSystem/ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServerSystem/ServiceEntity/GameClusterServiceEntity.h"
#include "ServerSystem/ServiceEntity/MatchingServiceUtil.h"
#include "ServerSystem/ServerService/GamePartyManagerService.h"
#include "ServerSystem/ServiceEntity/GamePartyManagerServiceEntity.h"

#include "Protocol/Message/PartyMatchingQueueMsgClass.h"

#include "Protocol/Message/GamePartyMsgClass.h"
#include "Protocol/Policy/GamePartyIPolicy.h"
#include "Protocol/Message/GamePartyManagerMsgClass.h"
#include "Protocol/Policy/GamePartyManagerIPolicy.h"

#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceIPolicy.h"

#include "GamePlayerEntityTrans.h"
#include "GamePlayerEntityTransClose.h"
#include "GamePlayerEntityTransGame.h"
#include "GameInstance/GamePlayerEntity.h"

#include "ServerSystem/BrServer.h"

#include "GameInstance/GameEntityManager.h"

#include "GameSystem/UserFriendSystem.h"
#include "GameSystem/UserGamePlayerInfoSystem.h"

#include "DB/AccountDB.h"
#include "DB/AccountQuery.h"
#include "DB/GameConspiracyDB.h"
#include "DB/GameConspiracyQuery.h"

#include "Table/Conspiracy/OrganicTbl.h"





BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransJoinGame);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransJoinedS2SEvt);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransLeaveGame);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransLeftS2SEvt);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransPlayerKickedS2SEvt);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransAssignRole);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransRoleAssignedS2SEvt);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransChatMessage);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransChatMessageFromOtherEntity);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransVoteGameAdvance);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransGameAdvanceVotedS2SEvt);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransVote);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransVoteEndS2SEvt);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransPlayerRevealedS2SEvt);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransAdvanceGame);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransGameAdvancedS2SEvt);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransGameEndedS2SEvt);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransPlayerKilledS2SEvt);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransPlayerVotedS2SEvt);

BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransRequestGameMatch);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransCancelGameMatch);

BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransPlayerMatchingCanceledS2CEvt);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransPlayerMatchingItemDequeuedS2CEvt);

BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransPlayAgain);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransPlayAgainS2SEvt);

BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransGameRevealPlayer);

BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransGamePlayerRevive);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransGamePlayerRevivedS2SEvt);

BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransGamePlayerResetRank);


namespace BR {
namespace GameServer {



	/////////////////////////////////////////////////////////////////////////////
	//
	//	Game command transaction
	//


	PlayerTransJoinGame::PlayerTransJoinGame( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		m_Role = PlayerRole::None;
		m_Dead = true;
		m_GameInsID = 0;
		m_Day = 0;

		BR_TRANS_MESSAGE(Message::GameInstance::JoinGameRes, { return OnJoinGameRes(pRes); });
		BR_TRANS_MESSAGE(Message::GameParty::LeavePartyRes, { return OnLeavePartyRes(pRes); });
	}

	HRESULT PlayerTransJoinGame::OnJoinGameRes( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;
		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameInstance::JoinGameRes joinRes;

		svrChkClose(pRes->GetHRESULT());

		svrChk( joinRes.ParseIMsg( pMsgRes->GetMessage() ) );

		GetMyOwner()->AddGameTransactionLog(TransLogCategory::Game, 1, 0, joinRes.GetRouteContext().GetFrom().UID);

		GetMyOwner()->SetGameInsUID( joinRes.GetRouteContext().GetFrom());

		m_GameInsID = joinRes.GetRouteContext().GetFrom();
		m_TimeStamp = joinRes.GetTimeStamp();
		m_GameState = joinRes.GetGameState();
		m_Day = joinRes.GetDay();
		m_Role = joinRes.GetRole();
		m_Dead = joinRes.GetDead() != 0;

		m_ChatHistoryData.SetLinkedBuffer(joinRes.GetChatHistoryData());
		m_GameLogData.SetLinkedBuffer(joinRes.GetGameLogData());

		m_PlayerIndex = joinRes.GetPlayerIndex();
		m_PlayerCharacter = joinRes.GetPlayerCharacter();
		m_MaxPlayer = joinRes.GetMaxPlayer();

		// We don't need to do it here
		if( joinRes.GetIsNewJoin() && GetMyServer()->GetPresetGameConfig() != nullptr )
		{
			//svrErr(E_INVALID_STATE);
			GetMyOwner()->UpdateGamePlayer();
			GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GainStamina( -GetMyServer()->GetPresetGameConfig()->StaminaForGame );
		}

		// Leave party when the player joined a party
		if (GetMyOwner()->GetPartyUID() != 0)
		{
			Policy::IPolicyGameParty *pPolicy = nullptr;
			Svr::ServerEntity *pServerEntity = nullptr;

			svrChk(Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity(GetMyOwner()->GetPartyUID().GetServerID(), pServerEntity));
			svrChkPtr(pPolicy = pServerEntity->GetPolicy<Policy::IPolicyGameParty>());

			svrChk(pPolicy->LeavePartyCmd(GetTransID(), RouteContext(GetOwnerEntityUID(), GetMyOwner()->GetPartyUID()), GetMyOwner()->GetPlayerID()));
		}
		else
		{
			CloseTransaction(hr);
		}

	Proc_End:

		if (FAILED(hr))
			CloseTransaction( hr );

		return S_OK;
	}

	HRESULT PlayerTransJoinGame::OnLeavePartyRes(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_OK;
		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameParty::LeavePartyRes leavePartyRes;

		svrChkClose(pRes->GetHRESULT());

		svrChk(leavePartyRes.ParseIMsg(pMsgRes->GetMessage()));

		GetMyOwner()->SetPartyUID(0);

	Proc_End:

		CloseTransaction(hr);

		return S_OK;
	}

	// Start Transaction
	HRESULT PlayerTransJoinGame::StartTransaction()
	{
		HRESULT hr = S_OK;
		GameInsUID insUID;
		Policy::IPolicyGameInstance* pPolicy = nullptr;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErr(E_SVR_ALREADY_INQUEUE);

		m_GameInsID = 0;
		m_TimeStamp = 0;
		m_GameState = GameStateID::None;
		m_Role = PlayerRole::None;
		m_Dead = 0;
		m_MaxPlayer = 0;
		m_PlayerIndex = 0;
		m_PlayerCharacter = 0;


		if( GetMyOwner()->GetAuthTicket() != GetTicket() )
			svrErr(E_INVALID_TICKET);

		if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
			svrErr(E_INVALID_PLAYERID);

		if( GetMyOwner()->GetGameInsUID() != 0 && GetMyOwner()->GetGameInsUID() != GetInsUID() )
			svrErr(E_GAME_ALREADY_IN_GAME);

		if( GetInsUID() == 0 )
			svrErr(E_INVALID_INSTANCEID);

		// We can't check stamina here because it could be a rejoin case
		//svrChkPtr(GetMyServer()->GetPresetGameConfig());
		//if( GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GetStamina() < GetMyServer()->GetPresetGameConfig()->StaminaForGame )
		//	svrErrClose(E_GAME_LOW_STAMINA);

		insUID = GetInsUID();

		svrChkPtr( pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(insUID.GetServerID()) );

		svrChk( pPolicy->JoinGameCmd( GetTransID(), RouteContext(GetOwnerEntityUID(),insUID),
			GetMyOwner()->GetPlayerInformation(), GetMyOwner()->GetAuthTicket(), PlayerRole::None ) );

	Proc_End:

		if( FAILED(hr) )
			CloseTransaction( hr );

		return hr;
	}

	// Start Transaction
	HRESULT PlayerTransJoinedS2SEvt::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
			svrErrClose(E_INVALID_INSTANCEID);

		svrChkPtr(GetPolicy());
		svrChk( GetPolicy()->PlayerJoinedS2CEvt( GetRouteContext().GetFrom(), GetJoinedPlayer(), GetJoinedPlayerRole(), GetJoinedPlayerDead(), GetJoinedPlayerIndex(), GetJoinedPlayerCharacter() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}


	PlayerTransLeaveGame::PlayerTransLeaveGame( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameInstance::LeaveGameRes, { return OnLeaveGameRes(pRes); } );
	}

	HRESULT PlayerTransLeaveGame::OnLeaveGameRes( Svr::TransactionResult* & pRes )
	{
		HRESULT hr = S_OK;
		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameInstance::LeaveGameRes leaveRes;

		GetMyOwner()->AddGameTransactionLog(TransLogCategory::Game, -1, 0, GetMyOwner()->GetGameInsUID().UID);

		if (pRes->GetHRESULT() == E_SVR_INVALID_ENTITYUID)
		{

			GetMyOwner()->SetGameInsUID(0);
			GetMyOwner()->UpdateDBSync(0);
		}
		else
		{
			svrChkClose(pRes->GetHRESULT());

			svrChk(leaveRes.ParseIMsg(pMsgRes->GetMessage()));

			GetMyOwner()->SetGameInsUID(0);
			GetMyOwner()->UpdateDBSync(0);
		}

	Proc_End:

		CloseTransaction( hr );

		return S_OK; 
	}

	// Start Transaction
	HRESULT PlayerTransLeaveGame::StartTransaction()
	{
		HRESULT hr = S_OK;
		GameInsUID insUID;
		Policy::IPolicyGameInstance *pPolicy = nullptr;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetAuthTicket() != GetTicket() )
			svrErr(E_INVALID_TICKET);

		if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
			svrErr(E_INVALID_PLAYERID);

		insUID = GetMyOwner()->GetGameInsUID();
		if( insUID.UID == 0 )
			svrErrClose(E_INVALID_INSTANCEID);

		svrChkPtr( pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(insUID.GetServerID()) );

		svrChk( pPolicy->LeaveGameCmd( GetTransID(), RouteContext(GetOwnerEntityUID(),insUID), GetMyOwner()->GetPlayerID() ) );

	Proc_End:

		if( FAILED(hr) )
			CloseTransaction( hr );

		return hr;
	}
	
	// Start Transaction
	HRESULT PlayerTransLeftS2SEvt::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
			svrErrClose(E_INVALID_INSTANCEID);

		svrChkPtr(GetPolicy());
		svrChk( GetPolicy()->PlayerLeftS2CEvt( GetRouteContext().GetFrom(), GetLeftPlayerID() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}



	PlayerTransKickPlayer::PlayerTransKickPlayer( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameInstance::KickPlayerRes, { return OnKickPlayerRes(pRes); } );
	}

	HRESULT PlayerTransKickPlayer::OnKickPlayerRes( Svr::TransactionResult* & pRes )
	{
		HRESULT hr = S_OK;
		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameInstance::KickPlayerRes msgRes;

		svrChkClose(pRes->GetHRESULT());

		svrChk( msgRes.ParseIMsg( pMsgRes->GetMessage() ) );

	Proc_End:

		CloseTransaction( hr );

		return S_OK; 
	}

	// Start Transaction
	HRESULT PlayerTransKickPlayer::StartTransaction()
	{
		HRESULT hr = S_OK;
		GameInsUID insUID;
		Policy::IPolicyGameInstance *pPolicy = nullptr;

		svrChk( super::StartTransaction() );

		//if( GetMyOwner()->GetAuthTicket() != GetTicket() )
		//	svrErr(E_INVALID_TICKET);

		if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
			svrErr(E_INVALID_PLAYERID);

		insUID = GetMyOwner()->GetGameInsUID();
		if( insUID.UID == 0 )
			svrErrClose(E_INVALID_INSTANCEID);

		svrChkPtr( pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(insUID.GetServerID()) );

		svrChk( pPolicy->KickPlayerCmd( GetTransID(), 
			RouteContext(GetOwnerEntityUID(),insUID.GetServerID()),
			GetMyOwner()->GetPlayerID(), GetPlayerToKick() ) );

	Proc_End:

		if( FAILED(hr) )
			CloseTransaction( hr );

		return hr;
	}
	
	// Start Transaction
	HRESULT PlayerTransPlayerKickedS2SEvt::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
			svrErrClose(E_INVALID_INSTANCEID);

		if( GetKickedPlayerID() == GetMyOwner()->GetPlayerID() )
		{
			GetMyOwner()->SetGameInsUID(0);
		}

		svrChkPtr(GetPolicy());
		svrChk( GetPolicy()->PlayerKickedS2CEvt( GetRouteContext().GetFrom(), GetKickedPlayerID() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}



	PlayerTransAssignRole::PlayerTransAssignRole( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameInstance::AssignRoleRes, { return OnAssignRoleRes(pRes); } );
	}

	HRESULT PlayerTransAssignRole::OnAssignRoleRes( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameInstance::AssignRoleRes res;

		svrChkClose(pRes->GetHRESULT());
		svrChk( res.ParseIMsg( pMsgRes->GetMessage() ) );

	Proc_End:

		CloseTransaction(hr);

		return S_OK; 
	}

	// Start Transaction
	HRESULT PlayerTransAssignRole::StartTransaction()
	{
		HRESULT hr = S_OK;
		GameInsUID insUID;
		Policy::IPolicyGameInstance *pPolicy = nullptr;

		svrChk( super::StartTransaction() );

		//if( GetMyOwner()->GetAuthTicket() != GetTicket() )
		//	svrErr(E_INVALID_TICKET);

		//if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
		//	svrErr(E_INVALID_PLAYERID);

		insUID = GetMyOwner()->GetGameInsUID();
		if( insUID.UID == 0 )
			svrErrClose(E_INVALID_INSTANCEID);

		svrChkPtr( pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(insUID.GetServerID()) );

		svrChk( pPolicy->AssignRoleCmd( GetTransID(), RouteContext(GetOwnerEntityUID(),insUID), 
			GetMyOwner()->GetPlayerID() ) );

	Proc_End:

		if( FAILED(hr) )
			CloseTransaction( hr );

		return hr;
	}

	
	// Start Transaction
	HRESULT PlayerTransRoleAssignedS2SEvt::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
			svrErrClose(E_INVALID_INSTANCEID);

		//GetMyOwner()->SendPushNotify(BRPUSHMSG_SYNC);

		svrChkPtr(GetPolicy());
		svrChk( GetPolicy()->RoleAssignedS2CEvt( GetRouteContext().GetFrom(), GetMyOwner()->GetPlayerID(), GetRole() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}


	
	// Start Transaction
	HRESULT PlayerTransChatMessage::StartTransaction()
	{
		HRESULT hr = S_OK;
		GameInsUID insUID;
		Policy::IPolicyGameInstance *pPolicy = nullptr;

		svrChk( super::StartTransaction() );

		//if( GetMyOwner()->GetAuthTicket() != GetTicket() )
		//	svrErr(E_INVALID_TICKET);

		//if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
		//	svrErr(E_INVALID_PLAYERID);
		
		insUID = GetMyOwner()->GetGameInsUID();
		if( insUID.UID == 0 )
			svrErrClose(E_INVALID_INSTANCEID);

		svrChkPtr( pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(insUID.GetServerID()) );

		svrChk( pPolicy->ChatMessageC2SEvt( RouteContext(GetOwnerEntityUID(),insUID), 
			GetMyOwner()->GetPlayerID(), GetRole(), GetChatMessage() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}


	// Start Transaction
	HRESULT PlayerTransChatMessageFromOtherEntity::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( super::StartTransaction() );

		svrChk( GetMyOwner()->GetISvrGamePolicy()->ChatMessageS2CEvt( GetSenderID(), GetRole(), GetSenderName(), GetChatMessage() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;

	}
	


	PlayerTransVoteGameAdvance::PlayerTransVoteGameAdvance( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameInstance::VoteGameAdvanceRes, { return OnVoteRes(pRes); } );
	}

	HRESULT PlayerTransVoteGameAdvance::OnVoteRes( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameInstance::VoteGameAdvanceRes res;

		svrChkClose(pRes->GetHRESULT());
		//svrChk( res.ParseIMsg( pMsgRes->GetMessage() ) );

	Proc_End:

		CloseTransaction(hr);

		return S_OK; 
	}
	
	// Start Transaction
	HRESULT PlayerTransVoteGameAdvance::StartTransaction()
	{
		HRESULT hr = S_OK;
		GameInsUID insUID;
		Policy::IPolicyGameInstance *pPolicy = nullptr;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetAuthTicket() != GetTicket() )
			svrErr(E_INVALID_TICKET);

		if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
			svrErr(E_INVALID_PLAYERID);

		insUID = GetMyOwner()->GetGameInsUID();
		if( insUID.UID == 0 )
			svrErrClose(E_INVALID_INSTANCEID);

		svrChkPtr( pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(insUID.GetServerID()) );

		svrChk( pPolicy->VoteGameAdvanceCmd( GetTransID(), RouteContext(GetOwnerEntityUID(),insUID), GetMyOwner()->GetPlayerID() ) );
	
	Proc_End:

		if( FAILED(hr) )
			CloseTransaction( hr );

		return hr;
	}
	
	// Start Transaction
	HRESULT PlayerTransGameAdvanceVotedS2SEvt::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( super::StartTransaction() );
		
		if( GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
			svrErrClose(E_INVALID_INSTANCEID);

		svrChkPtr(GetPolicy());
		svrChk( GetPolicy()->GameAdvanceVotedS2CEvt( GetRouteContext().GetFrom(), GetVoter() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}

	
	PlayerTransVote::PlayerTransVote( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameInstance::VoteRes, { return OnVoteRes(pRes); } );
	}

	HRESULT PlayerTransVote::OnVoteRes( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameInstance::VoteRes res;

		svrChkClose(pRes->GetHRESULT());
		//svrChk( res.ParseIMsg( pMsgRes->GetMessage() ) );

	Proc_End:

		CloseTransaction(hr);

		return S_OK; 
	}


	// Start Transaction
	HRESULT PlayerTransVote::StartTransaction()
	{
		HRESULT hr = S_OK;
		GameInsUID insUID;
		Policy::IPolicyGameInstance *pPolicy = nullptr;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetAuthTicket() != GetTicket() )
			svrErr(E_INVALID_TICKET);

		if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
			svrErr(E_INVALID_PLAYERID);

		insUID = GetMyOwner()->GetGameInsUID();
		if( insUID.UID == 0 )
			svrErrClose(E_INVALID_INSTANCEID);

		svrChkPtr( pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(insUID.GetServerID()) );

		svrChk( pPolicy->VoteCmd( GetTransID(), RouteContext(GetOwnerEntityUID(),insUID), 
			GetMyOwner()->GetPlayerID(), GetVoteTarget(), GetActionSerial() ) );
	
	Proc_End:

		if( FAILED(hr) )
			CloseTransaction( hr );

		return hr;
	}
	
	// Start Transaction
	HRESULT PlayerTransVoteEndS2SEvt::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
			svrErrClose(E_INVALID_INSTANCEID);

		svrChkPtr(GetPolicy());
		svrChk( GetPolicy()->VoteEndS2CEvt( GetRouteContext().GetFrom(), GetVoted() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}


	// Start Transaction
	HRESULT PlayerTransPlayerRevealedS2SEvt::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( super::StartTransaction() );
		
		if( GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
			svrErrClose(E_INVALID_INSTANCEID);

		svrChkPtr(GetPolicy());
		svrChk( GetPolicy()->PlayerRevealedS2CEvt( GetRouteContext().GetFrom(), GetRevealedPlayerID(), GetRole(), GetReason() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}



	PlayerTransAdvanceGame::PlayerTransAdvanceGame( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameInstance::AdvanceGameRes, { return OnAdvanceGameRes(pRes); } );
	}

	HRESULT PlayerTransAdvanceGame::OnAdvanceGameRes( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		//Message::GameInstance::AdvanceGameRes res;

		svrChkPtr(pRes);
		svrChkClose(pRes->GetHRESULT());
		//svrChk( res.ParseIMsg( pMsgRes->GetMessage() ) );

	Proc_End:

		CloseTransaction(hr);

		return S_OK; 
	}

	// Start Transaction
	HRESULT PlayerTransAdvanceGame::StartTransaction()
	{
		HRESULT hr = S_OK;
		GameInsUID insUID;
		Policy::IPolicyGameInstance *pPolicy = nullptr;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetAuthTicket() != GetTicket() )
			svrErr(E_INVALID_TICKET);

		if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
			svrErr(E_INVALID_PLAYERID);

		insUID = GetMyOwner()->GetGameInsUID();
		if( insUID.UID == 0 )
			svrErrClose(E_INVALID_INSTANCEID);

		svrChkPtr( pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(insUID.GetServerID()) );

		svrChk( pPolicy->AdvanceGameCmd( GetTransID(), RouteContext(GetOwnerEntityUID(),insUID), 
			GetMyOwner()->GetPlayerID() ) );
	
	Proc_End:

		if( FAILED(hr) )
			CloseTransaction( hr );

		return hr;
	}


	// Start Transaction
	HRESULT PlayerTransGameAdvancedS2SEvt::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( super::StartTransaction() );
		
		if( GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
			svrErrClose(E_INVALID_INSTANCEID);

		GetMyOwner()->SendPushNotify(BRPUSHMSG_SYNC);

		svrChkPtr(GetPolicy());
		svrChk( GetPolicy()->GameAdvancedS2CEvt( GetRouteContext().GetFrom(), GetTimeStamp(), GetGameState(), GetDay() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}


	PlayerTransGameEndedS2SEvt::PlayerTransGameEndedS2SEvt( Message::MessageData* &pIMsg )
		:UserTransactionS2SEvt( pIMsg )
	{
		//BR_TRANS_MESSAGE( DB::QuerySetPlayerInfoCmd, { return OnUpdateDBRes(pRes); } );
		BR_TRANS_MESSAGE( DB::QueryUpdateGameEndCmd, { return OnUpdateDBRes(pRes); } );
	}

	HRESULT PlayerTransGameEndedS2SEvt::OnUpdateDBRes(  Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;

		svrChk( pRes->GetHRESULT() );
		//DB::QuerySetPlayerInfoCmd *pMsgRes = (DB::QuerySetPlayerInfoCmd*)pRes;


	Proc_End:

		CloseTransaction(hr);

		return S_OK;
	}

	// Start Transaction
	HRESULT PlayerTransGameEndedS2SEvt::StartTransaction()
	{
		HRESULT hr = S_OK;
		UserGamePlayerInfoSystem *pPlayerInfoSystem = nullptr;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
			svrErrClose(E_INVALID_INSTANCEID);

		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		GetMyOwner()->AddGameTransactionLogT(TransLogCategory::GameResult, GetIsWon(), GetGainedExp(), GetGainedGameMoney(), "Role:%0%, exp:%1%, money:%2%", GetPlayedRole(), pPlayerInfoSystem->GetExp(), pPlayerInfoSystem->GetGameMoney());

		GetMyOwner()->SendPushNotify(BRPUSHMSG_SYNC);

		pPlayerInfoSystem->GainExp(GetGainedExp());
		pPlayerInfoSystem->GainGameMoney(GetGainedGameMoney());
		pPlayerInfoSystem->AchivedWin( GetPlayedRole(), GetIsWon() != 0 );

		// Save player record
		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->UpdateGameEndCmd(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(),
			pPlayerInfoSystem->GetLevel(), 
			pPlayerInfoSystem->GetExp(), pPlayerInfoSystem->GetGameMoney(), 
			pPlayerInfoSystem->GetTotalPlayed(), 
			pPlayerInfoSystem->GetWinPlaySCitizen(), pPlayerInfoSystem->GetWinPlaySMonster(), pPlayerInfoSystem->GetWinPlaySSeer(),
			pPlayerInfoSystem->GetLosePlaySCitizen(), pPlayerInfoSystem->GetLosePlaySMonster(), pPlayerInfoSystem->GetLosePlaySSeer(),
			pPlayerInfoSystem->GetWinPlayNCitizen(), pPlayerInfoSystem->GetWinPlayNMonster(), pPlayerInfoSystem->GetWinPlayNSeer(),
			pPlayerInfoSystem->GetLosePlayNCitizen(), pPlayerInfoSystem->GetLosePlayNMonster(), pPlayerInfoSystem->GetLosePlayNSeer(),
			GetMyOwner()->GetLatestActiveTime() ) );

		// Ignore sending errors, anyway we got the result
		if( GetPolicy() != nullptr )
			GetPolicy()->GameEndedS2CEvt( GetRouteContext().GetFrom(), GetWinner(), GetGainedExp(), GetGainedGameMoney() );

	Proc_End:

		if(FAILED(hr) )
			CloseTransaction( hr );

		return hr;
	}

	// Start Transaction
	HRESULT PlayerTransPlayerKilledS2SEvt::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( super::StartTransaction() );
		
		if( GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
			svrErrClose(E_INVALID_INSTANCEID);

		svrChkPtr(GetPolicy());
		svrChk( GetPolicy()->PlayerKilledS2CEvt( GetRouteContext().GetFrom(), GetKilledPlayer(), GetReason() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}

	
	// Start Transaction
	HRESULT PlayerTransPlayerVotedS2SEvt::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
			svrErrClose(E_INVALID_INSTANCEID);

		svrChkPtr(GetPolicy());
		svrChk( GetPolicy()->VotedS2CEvt( GetRouteContext().GetFrom(), GetVoter(), GetVotedTarget() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}



	PlayerTransRequestGameMatch::PlayerTransRequestGameMatch( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameParty::StartGameMatchRes, { return OnRequestPartyMatchingRes(pRes); } );
		BR_TRANS_MESSAGE( Message::PartyMatchingQueue::RegisterPlayerMatchingRes, { return OnRequestPlayerMatchingRes(pRes); } );
	}

	HRESULT PlayerTransRequestGameMatch::OnRequestPartyMatchingRes( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameParty::StartGameMatchRes res;

		svrChkClose(pRes->GetHRESULT());
		svrChk( res.ParseIMsg( pMsgRes->GetMessage() ) );

	Proc_End:

		CloseTransaction(hr);

		return S_OK; 
	}

	HRESULT PlayerTransRequestGameMatch::OnRequestPlayerMatchingRes( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::PartyMatchingQueue::RegisterPlayerMatchingRes res;

		svrChkClose(pRes->GetHRESULT());
		svrChk( res.ParseIMsg( pMsgRes->GetMessage() ) );

		GetMyOwner()->SetMatchingTicket( res.GetMatchingTicket() );

		GetPolicy()->GameMatchingStartedS2CEvt();

	Proc_End:

		CloseTransaction(hr);

		return S_OK; 
	}


	// Start Transaction
	HRESULT PlayerTransRequestGameMatch::StartTransaction()
	{
		HRESULT hr = S_OK;
		Svr::ServerServiceInformation *pService = nullptr;
		conspiracy::OrganicTbl::OrganicItem *pCostItem = nullptr;

		m_TotalGem = 0;
		m_TotalGameMoney = 0;

		svrChk( super::StartTransaction() );

		//if( GetMyOwner()->GetAuthTicket() != GetTicket() )
		//	svrErr(E_INVALID_TICKET);

		//if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
		//	svrErr(E_INVALID_PLAYERID);

		if( GetMyOwner()->GetGameInsUID() != 0 )
			svrErrClose(E_GAME_ALREADY_IN_GAME);

		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrClose(E_SVR_ALREADY_INQUEUE);

		svrChkPtr(GetMyServer()->GetPresetGameConfig());
		// Update player state
		auto pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>();
		GetMyOwner()->UpdateGamePlayer();

		auto currentStamina = pPlayerInfoSystem->GetStamina();
		auto StaminaForGame = GetMyServer()->GetPresetGameConfig()->StaminaForGame;
		if( currentStamina < StaminaForGame )
			svrErrClose(E_GAME_LOW_STAMINA);

		if (GetRequestRole() != PlayerRole::None)
		{
			svrChkCloseErr(E_GAME_INVALID_COSTID, conspiracy::OrganicTbl::FindItem((int)conspiracy::OrganicTbl::EItemEffect::Enum::RoleChoice, pCostItem));
			svrChkCloseErr(E_GAME_NOTENOUGH_RESOURCE, pPlayerInfoSystem->CheckCost(pCostItem));
		}

		m_TotalGem = pPlayerInfoSystem->GetGem();
		m_TotalGameMoney = pPlayerInfoSystem->GetGameMoney();


		if( GetMyOwner()->GetPartyUID().UID == 0 )
		{
			// Player isn't in a party, just do it alone
			auto componentID = Svr::MatchingUtil::GetQueueComponentID(GetNumPlayer(), 1, GetRequestRole());
			auto watcherService = Svr::GetServerComponent<Svr::MatchingQueueWatcherServiceEntity>(componentID);
			svrChkPtr(watcherService);
			svrChk(watcherService->GetService(pService));

			svrChk( pService->GetService<Svr::PartyMatchingQueueService>()->RegisterPlayerMatchingCmd( GetTransID(), 0, GetMyOwner()->GetPlayerID() ) );
		}
		else
		{
			// Let the party handle this
			Policy::IPolicyGameParty *pPolicy = nullptr;
			svrChkPtr( pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameParty>( GetMyOwner()->GetPartyUID().GetServerID()) );
			svrChk( pPolicy->StartGameMatchCmd( GetTransID(), RouteContext(GetOwnerEntityUID(),GetMyOwner()->GetPartyUID()), GetMyOwner()->GetPlayerID(), GetNumPlayer() ) );
		}
	
	Proc_End:

		if( FAILED(hr) )
			CloseTransaction( hr );

		return hr;
	}
	


	PlayerTransCancelGameMatch::PlayerTransCancelGameMatch( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameParty::CancelGameMatchRes, { return OnCancelPartyMatchingRes(pRes); } );
		BR_TRANS_MESSAGE( Message::PartyMatchingQueue::UnregisterMatchingRes, { return OnCancelPlayerMatchingRes(pRes); } );
	}

	HRESULT PlayerTransCancelGameMatch::OnCancelPartyMatchingRes( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameParty::CancelGameMatchRes res;

		svrChkClose(pRes->GetHRESULT());
		svrChk( res.ParseIMsg( pMsgRes->GetMessage() ) );

		// This means it just pended for canceling. yo have to wait canceled event from matching queue
		//GetMyOwner()->SetMatchingTicket(0);

		//GetPolicy()->GameMatchingCanceledS2CEvt();

	Proc_End:

		CloseTransaction(hr);

		return S_OK; 
	}
	
	HRESULT PlayerTransCancelGameMatch::OnCancelPlayerMatchingRes( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::PartyMatchingQueue::UnregisterMatchingRes res;

		switch( pRes->GetHRESULT() )
		{
		case E_SVR_RESERVED_QUEUEITEM:
			svrErrClose(E_SVR_RESERVED_QUEUEITEM);
			break;
		case E_SVR_QUEUEITEM_CANCELED:
		case E_SVR_INVALID_QUEUEITEM:
			GetMyOwner()->SetMatchingTicket( 0 );
			break;
		default:
			svrChk(pRes->GetHRESULT());
			svrChk( res.ParseIMsg( pMsgRes->GetMessage() ) );
			GetMyOwner()->SetMatchingTicket( 0 );
			break;
		};


	Proc_End:

		CloseTransaction(hr);

		return S_OK; 
	}


	// Start Transaction
	HRESULT PlayerTransCancelGameMatch::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetMatchingTicket() == 0 )
		{
			svrErrClose(E_SVR_INVALID_QUEUEITEM);
		}

		if( GetMyOwner()->GetPartyUID().UID == 0 )
		{
			Policy::IPolicyPartyMatchingQueue *pPolicy = nullptr;
			// Player isn't in a party, just do it alone
			svrChkPtr( pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyPartyMatchingQueue>( GetMyOwner()->GetMatchingTicket().QueueUID.GetServerID()) );

			svrChk( pPolicy->UnregisterMatchingCmd( GetTransID(), RouteContext(GetOwnerEntityUID(),GetMyOwner()->GetMatchingTicket().QueueUID), 0, GetMyOwner()->GetMatchingTicket() ) );
		}
		else
		{
			// Let the party handle this
			Policy::IPolicyGameParty *pPolicy = nullptr;
			svrChkPtr( pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameParty>( GetMyOwner()->GetPartyUID().GetServerID()) );
			svrChk( pPolicy->CancelGameMatchCmd( GetTransID(), RouteContext(GetOwnerEntityUID(),GetMyOwner()->GetPartyUID()), GetMyOwner()->GetPlayerID() ) );
		}
	
	Proc_End:

		if( FAILED(hr) )
			CloseTransaction( hr );

		return hr;
	}
	



	//////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Matching queue
	//

	// Start Transaction
	HRESULT PlayerTransPlayerMatchingCanceledS2CEvt::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetMatchingTicket() != GetMatchingTicket() )
		{
			svrErr(E_SVR_INVALID_QUEUEITEM);
		}

		GetMyOwner()->SetMatchingTicket(0);

		svrChkPtr(GetPolicy());
		svrChk( GetPolicy()->GameMatchingCanceledS2CEvt() );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	
	// Start Transaction
	HRESULT PlayerTransPlayerMatchingItemDequeuedS2CEvt::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( super::StartTransaction() );
		
		if( GetMyOwner()->GetMatchingTicket() != GetMatchingTicket() )
		{
			svrErrClose(E_SVR_INVALID_QUEUEITEM);
		}

		GetMyOwner()->SetMatchingTicket(0);

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}




	PlayerTransPlayAgain::PlayerTransPlayAgain(Message::MessageData* &pIMsg)
		:MessageTransaction(pIMsg)
	{
		BR_TRANS_MESSAGE(Message::GameInstance::GamePlayAgainRes, { return OnPlayAgainRes(pRes); });
		BR_TRANS_MESSAGE(Message::GamePartyManager::CreatePartyRes, { return OnCreatePartyRes(pRes); });
	}

	HRESULT PlayerTransPlayAgain::OnPlayAgainRes(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_OK;
		conspiracy::OrganicTbl::OrganicItem* pCostItem = nullptr;
		UserGamePlayerInfoSystem* pPlayerInfoSystem = nullptr;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameInstance::GamePlayAgainRes res;

		svrChkPtr(pRes);
		svrChkClose(pRes->GetHRESULT());
		svrChk( res.ParseIMsg( pMsgRes->GetMessage() ) );

		// This means it's not full game
		//if (m_RetryCount <= 1 
		//	&& pRes->GetHRESULT() == E_GAME_INAVLID_PLAYER_COUNT // if some members are missing
		//	&& GetMyOwner()->GetPartyUID() == 0)
		//{
		//	svrChk(RequestCreateParty()));
		//}
		//else
		if (res.GetReplayMemberCount() > GameConst::MAX_GAMEPLAYER)
			svrErr(E_GAME_INAVLID_PLAYER_COUNT);


		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		svrChkCloseErr(E_GAME_INVALID_COSTID, conspiracy::OrganicTbl::FindItem((int)conspiracy::OrganicTbl::EItemEffect::Enum::ReStart, pCostItem));
		svrChkCloseErr(E_GAME_NOTENOUGH_RESOURCE, pPlayerInfoSystem->ApplyCost(pCostItem, TransLogCategory::Buy, "PlayAgain"));

		m_TotalGem = pPlayerInfoSystem->GetGem();
		m_TotalGameMoney = pPlayerInfoSystem->GetGameMoney();


	Proc_End:

		CloseTransaction(hr);

		return S_OK;
	}

	HRESULT PlayerTransPlayAgain::OnCreatePartyRes(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_OK;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GamePartyManager::CreatePartyRes res;

		svrChkPtr(pRes);
		svrChkClose(pRes->GetHRESULT());
		svrChk( res.ParseIMsg( pMsgRes->GetMessage() ) );

		GetMyOwner()->SetPartyUID(res.GetRouteContext().GetFrom());

		svrChk(RequestPlayAgain());

	Proc_End:

		if (FAILED(hr))
			CloseTransaction(hr);

		return S_OK;
	}


	HRESULT PlayerTransPlayAgain::RequestPlayAgain()
	{
		HRESULT hr = S_OK;
		Policy::IPolicyGameInstance *pPolicy = nullptr;
		GameInsUID insUID = GetMyOwner()->GetGameInsUID();

		if (insUID == 0)
		{
			svrErr(E_SVR_INVALID_ENTITYUID);
		}

		svrChkPtr(pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(insUID.GetServerID()));
		svrChk(pPolicy->GamePlayAgainCmd(GetTransID(), RouteContext(GetOwnerEntityUID(), insUID), GetMyOwner()->GetPlayerID(), GetMyOwner()->GetPartyUID()));

	Proc_End:

		return hr;
	}

	HRESULT PlayerTransPlayAgain::RequestCreateParty()
	{
		HRESULT hr = S_OK;
		Svr::ServerServiceInformation *pService = nullptr;

		svrChk(Svr::GetServerComponent<Svr::GamePartyManagerServiceEntity>()->GetService(pService));

		svrChk(pService->GetService<Svr::GamePartyManagerService>()->CreatePartyCmd(GetTransID(), 0, GetMyOwner()->GetPlayerInformation()));

	Proc_End:

		return hr;
	}

	// Start Transaction
	HRESULT PlayerTransPlayAgain::StartTransaction()
	{
		HRESULT hr = S_OK;
		conspiracy::OrganicTbl::OrganicItem* pCostItem = nullptr;
		UserGamePlayerInfoSystem* pPlayerInfoSystem = nullptr;

		m_TotalGem = 0;
		m_TotalGameMoney = 0;

		svrChk(super::StartTransaction());

		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		svrChkCloseErr(E_GAME_INVALID_COSTID, conspiracy::OrganicTbl::FindItem((int)conspiracy::OrganicTbl::EItemEffect::Enum::ReStart, pCostItem));
		svrChkCloseErr(E_GAME_NOTENOUGH_RESOURCE, pPlayerInfoSystem->CheckCost(pCostItem));


		//m_RetryCount = 0;
		if (GetMyOwner()->GetPartyUID() == 0)
		{
			svrChk(RequestCreateParty());
		}
		else
		{
			svrChk(RequestPlayAgain());
		}

	Proc_End:

		if (FAILED(hr))
			CloseTransaction(hr);

		return hr;
	}



	PlayerTransPlayAgainS2SEvt::PlayerTransPlayAgainS2SEvt(Message::MessageData* &pIMsg)
		:UserTransactionS2SEvt(pIMsg)
	{
		BR_TRANS_MESSAGE(Message::GameParty::JoinPartyRes, { return OnJoinPartyRes(pRes); });
	}

	HRESULT PlayerTransPlayAgainS2SEvt::OnJoinPartyRes(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_OK;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameParty::JoinPartyRes res;

		svrChk(pRes->GetHRESULT());
		svrChk(res.ParseIMsg(pMsgRes->GetMessage()));

		GetMyOwner()->SetPartyUID(GetPartyUID());

		m_LeadPlayer = GetLeadPlayer();
		m_PartyUID = GetPartyUID();

		auto pPolicy = GetPolicy();
		if (pPolicy != nullptr)
			pPolicy->GamePlayAgainS2CEvt(GetPartyUID(), GetLeadPlayer());

	Proc_End:

		CloseTransaction(hr);

		return S_OK;
	}

	// Start Transaction
	HRESULT PlayerTransPlayAgainS2SEvt::StartTransaction()
	{
		HRESULT hr = S_OK;
		Policy::IPolicyGameParty *pPolicy = nullptr;
		Svr::ServerEntity *pServerEntity = nullptr;

		m_PartyUID = 0;
		m_LeadPlayer = 0;

		svrChk(super::StartTransaction());

		if (GetTargetPlayer() != GetMyOwner()->GetPlayerID())
			svrErr(E_INVALID_PLAYERID);

		GetMyOwner()->SetGameInsUID(0);

		if (GetMyOwner()->GetPartyUID() != 0)
		{
			if (GetMyOwner()->GetPartyUID() != GetPartyUID())
				svrErr(E_GAME_ALREADY_IN_PARTY);

			m_LeadPlayer = GetLeadPlayer();
			m_PartyUID = GetPartyUID();

			// I'm in the correct party
			GetPolicy()->GamePlayAgainS2CEvt(GetPartyUID(), GetLeadPlayer());

			CloseTransaction(hr);
		}
		else
		{
			svrChk(Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity(GetPartyUID().GetServerID(), pServerEntity));
			svrChkPtr(pPolicy = pServerEntity->GetPolicy<Policy::IPolicyGameParty>());
			svrChk(pPolicy->JoinPartyCmd(GetTransID(), RouteContext(GetOwnerEntityUID(), GetPartyUID()), GetLeadPlayer(), GetMyOwner()->GetPlayerInformation()));
		}

	Proc_End:

		if (FAILED(hr))
			CloseTransaction(hr);

		return hr;
	}




	PlayerTransGameRevealPlayer::PlayerTransGameRevealPlayer(Message::MessageData* &pIMsg)
		:MessageTransaction(pIMsg)
	{
		BR_TRANS_MESSAGE(DB::QueryUpdateTickStatusCmd, { return OnUpdatePlayerRes(pRes); });
		BR_TRANS_MESSAGE(Message::GameInstance::GameRevealPlayerRes, { return OnGameRevealPlayerRes(pRes); });
	}

	HRESULT PlayerTransGameRevealPlayer::OnUpdatePlayerRes(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_OK;
		Policy::IPolicyGameInstance *pPolicy = nullptr;
		GameInsUID insUID = GetMyOwner()->GetGameInsUID();

		auto *pDBRes = (DB::QueryUpdateTickStatusCmd*)pRes;

		svrChkPtr(pRes);
		svrChkClose(pRes->GetHRESULT());

		if (insUID == 0)
		{
			svrErr(E_SVR_INVALID_ENTITYUID);
		}

		svrChkPtr(pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(insUID.GetServerID()));
		svrChk(pPolicy->GameRevealPlayerCmd(GetTransID(), RouteContext(GetOwnerEntityUID(), insUID), GetMyOwner()->GetPlayerID(), GetTargetPlayerID()));

	Proc_End:

		if (FAILED(hr))
			CloseTransaction(hr);

		return S_OK;
	}

	HRESULT PlayerTransGameRevealPlayer::OnGameRevealPlayerRes(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_OK;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameInstance::GameRevealPlayerRes res;
		UserGamePlayerInfoSystem* pPlayerInfoSystem = nullptr;

		svrChkPtr(pRes);
		svrChkClose(pRes->GetHRESULT());
		svrChk(res.ParseIMsg(pMsgRes->GetMessage()));

		for (UINT iPlayer = 0; iPlayer < res.GetRevealedPlayerID().GetSize(); iPlayer++)
		{
			m_RevealedPlayerID.push_back(res.GetRevealedPlayerID()[iPlayer]);
		}
		for (UINT iPlayer = 0; iPlayer < res.GetRevealedRole().GetSize(); iPlayer++)
		{
			m_RevealedPlayerRole.push_back(res.GetRevealedRole()[iPlayer]);
		}

		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		m_TotalGem = pPlayerInfoSystem->GetGem();
		m_TotalGameMoney = pPlayerInfoSystem->GetGameMoney();

	Proc_End:

		CloseTransaction(hr);

		return S_OK;
	}


	// Start Transaction
	HRESULT PlayerTransGameRevealPlayer::StartTransaction()
	{
		HRESULT hr = S_OK;
		conspiracy::OrganicTbl::OrganicItem *pCostItem = nullptr;
		UserGamePlayerInfoSystem* pPlayerInfoSystem = nullptr;

		m_TotalGem = 0;
		m_TotalGameMoney = 0;

		svrChk(super::StartTransaction());

		if (GetMyOwner()->GetGameInsUID() == 0)
		{
			svrErrClose(E_INVALID_INSTANCEID);
		}

		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		svrChkCloseErr(E_GAME_INVALID_COSTID, conspiracy::OrganicTbl::FindItem((int)conspiracy::OrganicTbl::EItemEffect::Enum::Check, pCostItem));
		svrChkCloseErr(E_GAME_NOTENOUGH_RESOURCE, pPlayerInfoSystem->ApplyCost(pCostItem, TransLogCategory::Buy, "RevealPlayer"));


		svrChk(GetMyOwner()->UpdateDBSync(GetTransID()));

	Proc_End:

		if (FAILED(hr))
			CloseTransaction(hr);

		return hr;
	}



	PlayerTransGamePlayerRevive::PlayerTransGamePlayerRevive(Message::MessageData* &pIMsg)
		:MessageTransaction(pIMsg)
	{
		BR_TRANS_MESSAGE(DB::QueryUpdateTickStatusCmd, { return OnUpdatePlayerRes(pRes); });
		BR_TRANS_MESSAGE(Message::GameInstance::GamePlayerReviveRes, { return OnGamePlayerReviveRes(pRes); });
	}

	HRESULT PlayerTransGamePlayerRevive::OnUpdatePlayerRes(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_OK;
		Policy::IPolicyGameInstance *pPolicy = nullptr;
		GameInsUID insUID = GetMyOwner()->GetGameInsUID();

		auto *pDBRes = (DB::QueryUpdateTickStatusCmd*)pRes;

		svrChkPtr(pRes);
		svrChkClose(pRes->GetHRESULT());

		if (insUID == 0)
		{
			svrErr(E_SVR_INVALID_ENTITYUID);
		}

		svrChkPtr(pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(insUID.GetServerID()));
		svrChk(pPolicy->GamePlayerReviveCmd(GetTransID(), RouteContext(GetOwnerEntityUID(), insUID), GetMyOwner()->GetPlayerID()));

	Proc_End:

		if (FAILED(hr))
			CloseTransaction(hr);

		return S_OK;
	}

	HRESULT PlayerTransGamePlayerRevive::OnGamePlayerReviveRes(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_OK;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameInstance::GameRevealPlayerRes res;

		svrChkPtr(pRes);
		svrChkClose(pRes->GetHRESULT());
		svrChk(res.ParseIMsg(pMsgRes->GetMessage()));


	Proc_End:

		CloseTransaction(hr);

		return S_OK;
	}


	// Start Transaction
	HRESULT PlayerTransGamePlayerRevive::StartTransaction()
	{
		HRESULT hr = S_OK;
		conspiracy::OrganicTbl::OrganicItem *pCostItem = nullptr;
		UserGamePlayerInfoSystem* pPlayerInfoSystem = nullptr;

		m_TotalGem = 0;
		m_TotalGameMoney = 0;

		svrChk(super::StartTransaction());

		if (GetMyOwner()->GetGameInsUID() == 0)
		{
			svrErrClose(E_INVALID_INSTANCEID);
		}

		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		svrChkCloseErr(E_GAME_INVALID_COSTID, conspiracy::OrganicTbl::FindItem((int)conspiracy::OrganicTbl::EItemEffect::Enum::Revival, pCostItem));
		svrChkCloseErr(E_GAME_NOTENOUGH_RESOURCE, pPlayerInfoSystem->ApplyCost(pCostItem, TransLogCategory::Buy, "Revive"));

		m_TotalGem = pPlayerInfoSystem->GetGem();
		m_TotalGameMoney = pPlayerInfoSystem->GetGameMoney();

		svrChk(GetMyOwner()->UpdateDBSync(GetTransID()));

	Proc_End:

		if (FAILED(hr))
			CloseTransaction(hr);

		return hr;
	}




	// Start Transaction
	HRESULT PlayerTransGamePlayerRevivedS2SEvt::StartTransaction()
	{
		HRESULT hr = S_OK;
		Policy::IPolicyGameParty *pPolicy = nullptr;
		Svr::ServerEntity *pServerEntity = nullptr;

		svrChk(super::StartTransaction());

		if (GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
			svrErrClose(E_INVALID_INSTANCEID);

		GetPolicy()->GamePlayerRevivedS2CEvt(GetRevivedPlayerID());


	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


	// Start Transaction
	HRESULT PlayerTransGamePlayerResetRank::StartTransaction()
	{
		HRESULT hr = S_OK;
		UserGamePlayerInfoSystem* pPlayerInfoSystem = nullptr;
		conspiracy::OrganicTbl::OrganicItem *pCostItem = nullptr;

		m_TotalGem = 0;
		m_TotalGameMoney = 0;

		svrChk(super::StartTransaction());

		if (GetMyOwner()->GetGameInsUID() == 0)
		{
			svrErrClose(E_INVALID_INSTANCEID);
		}

		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());


		svrChkCloseErr(E_GAME_INVALID_COSTID, conspiracy::OrganicTbl::FindItem((int)conspiracy::OrganicTbl::EItemEffect::Enum::ResetRankNormal, pCostItem));
		svrChkCloseErr(E_GAME_NOTENOUGH_RESOURCE, pPlayerInfoSystem->ApplyCost(pCostItem, TransLogCategory::Buy, "ResetRank"));

		svrChkClose(pPlayerInfoSystem->ResetRankNormal(pCostItem));

		m_TotalGem = pPlayerInfoSystem->GetGem();
		m_TotalGameMoney = pPlayerInfoSystem->GetGameMoney();

		svrChk(GetMyOwner()->UpdateDBSync(0));

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


};// namespace GameServer 
};// namespace BR 

