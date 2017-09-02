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
#include "String/ToStringGame.h"

#include "GameServerClass.h"
#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "ServerEntity/ServerEntityManager.h"

#include "ServerSystem/ServerService/PartyMatchingQueueService.h"
#include "ServerSystem/ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServerSystem/ServiceEntity/Game/GameClusterServiceEntity.h"
#include "ServerSystem/ServiceEntity/MatchingServiceUtil.h"
#include "ServerSystem/ServerService/GamePartyManagerService.h"
#include "ServerSystem/ServiceEntity/GamePartyManagerServiceEntity.h"

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
#include "GameInstance/GamePlayerEntity.h"

#include "Server/BrServer.h"

#include "GameInstance/GameEntityManager.h"

#include "GameSystem/UserFriendSystem.h"
#include "GameSystem/UserGamePlayerInfoSystem.h"

#include "DB/AccountDB.h"
#include "DB/AccountQuery.h"
#include "DB/GameConspiracyDB.h"
#include "DB/GameConspiracyQuery.h"

#include "Table/conspiracy/OrganicTbl.h"





SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransJoinGame);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransJoinedS2SEvt);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransLeaveGame);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransLeftS2SEvt);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransPlayerKickedS2SEvt);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransAssignRole);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransRoleAssignedS2SEvt);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransChatMessage);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransChatMessageFromOtherEntity);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransVoteGameAdvance);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransGameAdvanceVotedS2SEvt);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransVote);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransVoteEndS2SEvt);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransPlayerRevealedS2SEvt);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransAdvanceGame);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransGameAdvancedS2SEvt);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransGameEndedS2SEvt);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransPlayerKilledS2SEvt);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransPlayerVotedS2SEvt);

SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransRequestGameMatch);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransCancelGameMatch);

SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransPlayerMatchingCanceledS2CEvt);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransPlayerMatchingItemDequeuedS2CEvt);

SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransPlayAgain);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransPlayAgainS2SEvt);

SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransGameRevealPlayer);

SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransGamePlayerRevive);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransGamePlayerRevivedS2SEvt);

SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransGamePlayerResetRank);


namespace SF {
namespace GameServer {



	/////////////////////////////////////////////////////////////////////////////
	//
	//	Game command transaction
	//


	PlayerTransJoinGame::PlayerTransJoinGame( MessageDataPtr &pIMsg )
		:MessageTransaction( pIMsg )
	{
		m_Role = PlayerRole::None;
		m_Dead = true;
		m_GameInsID = 0;
		m_Day = 0;

		BR_TRANS_MESSAGE(Message::GameInstance::JoinGameRes, { return OnJoinGameRes(pRes); });
		BR_TRANS_MESSAGE(Message::GameParty::LeavePartyRes, { return OnLeavePartyRes(pRes); });
	}

	Result PlayerTransJoinGame::OnJoinGameRes( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameInstance::JoinGameRes joinRes;

		svrChkClose(pRes->GetResult());

		svrChk( joinRes.ParseMessage( pMsgRes->GetMessage() ) );

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
			//svrErr(ResultCode::INVALID_STATE);
			GetMyOwner()->UpdateGamePlayer();
			GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GainStamina( -GetMyServer()->GetPresetGameConfig()->StaminaForGame );
		}

		// Leave party when the player joined a party
		if (GetMyOwner()->GetPartyUID() != 0)
		{
			Policy::IPolicyGameParty *pPolicy = nullptr;
			Svr::ServerEntity *pServerEntity = nullptr;

			svrChk(Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity(GetMyOwner()->GetPartyUID().GetServerID(), pServerEntity));
			svrChkPtr(pPolicy = pServerEntity->GetInterface<Policy::IPolicyGameParty>());

			svrChk(pPolicy->LeavePartyCmd(RouteContext(GetOwnerEntityUID(), GetMyOwner()->GetPartyUID()), GetTransID(), GetMyOwner()->GetPlayerID()));
		}
		else
		{
			CloseTransaction(hr);
		}

	Proc_End:

		if (!(hr))
			CloseTransaction( hr );

		return ResultCode::SUCCESS;
	}

	Result PlayerTransJoinGame::OnLeavePartyRes(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameParty::LeavePartyRes leavePartyRes;

		svrChkClose(pRes->GetResult());

		svrChk(leavePartyRes.ParseMessage(pMsgRes->GetMessage()));

		GetMyOwner()->SetPartyUID(0);

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}

	// Start Transaction
	Result PlayerTransJoinGame::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		GameInsUID insUID;
		Policy::IPolicyGameInstance* pPolicy = nullptr;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErr(ResultCode::SVR_ALREADY_INQUEUE);

		m_GameInsID = 0;
		m_TimeStamp = 0;
		m_GameState = GameStateID::None;
		m_Role = PlayerRole::None;
		m_Dead = 0;
		m_MaxPlayer = 0;
		m_PlayerIndex = 0;
		m_PlayerCharacter = 0;


		if( GetMyOwner()->GetAuthTicket() != GetTicket() )
			svrErr(ResultCode::E_INVALID_TICKET);

		if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
			svrErr(ResultCode::E_INVALID_PLAYERID);

		if( GetMyOwner()->GetGameInsUID() != 0 && GetMyOwner()->GetGameInsUID() != GetInsUID() )
			svrErr(ResultCode::E_GAME_ALREADY_IN_GAME);

		if( GetInsUID() == 0 )
			svrErr(ResultCode::E_INVALID_INSTANCEID);

		// We can't check stamina here because it could be a rejoin case
		//svrChkPtr(GetMyServer()->GetPresetGameConfig());
		//if( GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GetStamina() < GetMyServer()->GetPresetGameConfig()->StaminaForGame )
		//	svrErrClose(ResultCode::E_GAME_LOW_STAMINA);

		insUID = GetInsUID();

		svrChkPtr( pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(insUID.GetServerID()) );

		svrChk( pPolicy->JoinGameCmd( RouteContext(GetOwnerEntityUID(),insUID), GetTransID(),
			GetMyOwner()->GetPlayerInformation(), GetMyOwner()->GetAuthTicket(), PlayerRole::None ) );

	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}

	Result PlayerTransJoinGame::CloseTransaction(Result hrRes)
	{
		Result result = super::CloseTransaction(hrRes);
		svrTrace(Svr::TRC_INFO, "JoinGameRes PlayerID:{0} : hr:{1}", GetMyOwner()->GetPlayerID(), hrRes);
		return result;
	}

	// Start Transaction
	Result PlayerTransJoinedS2SEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
			svrErrClose(ResultCode::E_INVALID_INSTANCEID);

		svrChkPtr(GetInterface());
		svrChk( GetInterface()->PlayerJoinedS2CEvt( GetRouteContext().GetFrom(), GetJoinedPlayer(), GetJoinedPlayerRole(), GetJoinedPlayerDead(), GetJoinedPlayerIndex(), GetJoinedPlayerCharacter() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}


	PlayerTransLeaveGame::PlayerTransLeaveGame( MessageDataPtr &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameInstance::LeaveGameRes, { return OnLeaveGameRes(pRes); } );
	}

	Result PlayerTransLeaveGame::OnLeaveGameRes( Svr::TransactionResult* & pRes )
	{
		Result hr = ResultCode::SUCCESS;
		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameInstance::LeaveGameRes leaveRes;

		GetMyOwner()->AddGameTransactionLog(TransLogCategory::Game, -1, 0, GetMyOwner()->GetGameInsUID().UID);

		if (pRes->GetResult() == Result(ResultCode::SVR_INVALID_ENTITYUID))
		{

			GetMyOwner()->SetGameInsUID(0);
			GetMyOwner()->UpdateDBSync();
		}
		else
		{
			svrChkClose(pRes->GetResult());

			svrChk(leaveRes.ParseMessage(pMsgRes->GetMessage()));

			GetMyOwner()->SetGameInsUID(0);
			GetMyOwner()->UpdateDBSync();
		}

	Proc_End:

		CloseTransaction( hr );

		return ResultCode::SUCCESS; 
	}

	Result PlayerTransLeaveGame::CloseTransaction(Result hrRes)
	{
		Result result = super::CloseTransaction(hrRes);
		svrTrace(Svr::TRC_INFO, "LeaveGameRes PlayerID:{0} : hr:{1}", GetMyOwner()->GetPlayerID(), hrRes);
		return result;
	}

	// Start Transaction
	Result PlayerTransLeaveGame::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		GameInsUID insUID;
		Policy::IPolicyGameInstance *pPolicy = nullptr;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetAuthTicket() != GetTicket() )
			svrErr(ResultCode::E_INVALID_TICKET);

		if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
			svrErr(ResultCode::E_INVALID_PLAYERID);

		insUID = GetMyOwner()->GetGameInsUID();
		if( insUID.UID == 0 )
			svrErrClose(ResultCode::E_INVALID_INSTANCEID);

		svrChkPtr( pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(insUID.GetServerID()) );

		svrChk( pPolicy->LeaveGameCmd( RouteContext(GetOwnerEntityUID(),insUID), GetTransID(), GetMyOwner()->GetPlayerID() ) );

	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}
	
	// Start Transaction
	Result PlayerTransLeftS2SEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
			svrErrClose(ResultCode::E_INVALID_INSTANCEID);

		svrChkPtr(GetInterface());
		svrChk( GetInterface()->PlayerLeftS2CEvt( GetRouteContext().GetFrom(), GetLeftPlayerID() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}



	PlayerTransKickPlayer::PlayerTransKickPlayer( MessageDataPtr &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameInstance::KickPlayerRes, { return OnKickPlayerRes(pRes); } );
	}

	Result PlayerTransKickPlayer::OnKickPlayerRes( Svr::TransactionResult* & pRes )
	{
		Result hr = ResultCode::SUCCESS;
		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameInstance::KickPlayerRes msgRes;

		svrChkClose(pRes->GetResult());

		svrChk( msgRes.ParseMessage( pMsgRes->GetMessage() ) );

	Proc_End:

		CloseTransaction( hr );

		return ResultCode::SUCCESS; 
	}

	Result PlayerTransKickPlayer::CloseTransaction(Result hrRes)
	{
		Result result = super::CloseTransaction(hrRes);
		svrTrace(Svr::TRC_INFO, "KickPlayerRes PlayerID:{0}, KickPlayerID:{1}, hr:{2}", GetMyOwner()->GetPlayerID(), GetPlayerID(), hrRes);
		return result;
	}

	// Start Transaction
	Result PlayerTransKickPlayer::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		GameInsUID insUID;
		Policy::IPolicyGameInstance *pPolicy = nullptr;

		svrChk( super::StartTransaction() );

		//if( GetMyOwner()->GetAuthTicket() != GetTicket() )
		//	svrErr(ResultCode::E_INVALID_TICKET);

		if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
			svrErr(ResultCode::E_INVALID_PLAYERID);

		insUID = GetMyOwner()->GetGameInsUID();
		if( insUID.UID == 0 )
			svrErrClose(ResultCode::E_INVALID_INSTANCEID);

		svrChkPtr( pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(insUID.GetServerID()) );

		svrChk( pPolicy->KickPlayerCmd(RouteContext(GetOwnerEntityUID(), insUID.GetServerID()), GetTransID(),
			GetMyOwner()->GetPlayerID(), GetPlayerToKick() ) );

	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}
	
	// Start Transaction
	Result PlayerTransPlayerKickedS2SEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
			svrErrClose(ResultCode::E_INVALID_INSTANCEID);

		if( GetKickedPlayerID() == GetMyOwner()->GetPlayerID() )
		{
			GetMyOwner()->SetGameInsUID(0);
		}

		svrChkPtr(GetInterface());
		svrChk( GetInterface()->PlayerKickedS2CEvt( GetRouteContext().GetFrom(), GetKickedPlayerID() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}



	PlayerTransAssignRole::PlayerTransAssignRole( MessageDataPtr &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameInstance::AssignRoleRes, { return OnAssignRoleRes(pRes); } );
	}

	Result PlayerTransAssignRole::OnAssignRoleRes( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameInstance::AssignRoleRes res;

		svrChkClose(pRes->GetResult());
		svrChk( res.ParseMessage( pMsgRes->GetMessage() ) );

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransAssignRole::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		GameInsUID insUID;
		Policy::IPolicyGameInstance *pPolicy = nullptr;

		svrChk( super::StartTransaction() );

		//if( GetMyOwner()->GetAuthTicket() != GetTicket() )
		//	svrErr(ResultCode::E_INVALID_TICKET);

		//if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
		//	svrErr(ResultCode::E_INVALID_PLAYERID);

		insUID = GetMyOwner()->GetGameInsUID();
		if( insUID.UID == 0 )
			svrErrClose(ResultCode::E_INVALID_INSTANCEID);

		svrChkPtr( pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(insUID.GetServerID()) );

		svrChk( pPolicy->AssignRoleCmd( RouteContext(GetOwnerEntityUID(),insUID), GetTransID(),
			GetMyOwner()->GetPlayerID() ) );

	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}

	
	// Start Transaction
	Result PlayerTransRoleAssignedS2SEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
			svrErrClose(ResultCode::E_INVALID_INSTANCEID);

		//GetMyOwner()->SendPushNotify(BRPUSHMSG_SYNC);

		svrChkPtr(GetInterface());
		svrChk( GetInterface()->RoleAssignedS2CEvt( GetRouteContext().GetFrom(), GetMyOwner()->GetPlayerID(), GetRole() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}



	// Start Transaction
	Result PlayerTransChatMessage::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		GameInsUID insUID;
		Policy::IPolicyGameInstance *pPolicy = nullptr;

		svrChk( super::StartTransaction() );

		//if( GetMyOwner()->GetAuthTicket() != GetTicket() )
		//	svrErr(ResultCode::E_INVALID_TICKET);

		//if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
		//	svrErr(ResultCode::E_INVALID_PLAYERID);
		
		insUID = GetMyOwner()->GetGameInsUID();
		if( insUID.UID == 0 )
			svrErrClose(ResultCode::E_INVALID_INSTANCEID);

		svrChkPtr( pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(insUID.GetServerID()) );

		svrChk( pPolicy->ChatMessageC2SEvt( RouteContext(GetOwnerEntityUID(),insUID), 
			GetMyOwner()->GetPlayerID(), GetRole(), GetChatMessage() ) );

	Proc_End:


		svrTrace(Svr::TRC_INFO, "ChatMessage PlayerID:{0}, hr:{1}", GetMyOwner()->GetPlayerID(), hr);

		CloseTransaction( hr );

		return hr;
	}


	// Start Transaction
	Result PlayerTransChatMessageFromOtherEntity::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		svrChk( GetMyOwner()->GetISvrGamePolicy()->ChatMessageS2CEvt( GetSenderID(), GetRole(), GetSenderName(), GetChatMessage() ) );

	Proc_End:

		svrTrace(Svr::TRC_INFO, "ChatMessage received PlayerID:{0}, fromPlayer:{1}", GetMyOwner()->GetPlayerID(), GetSenderName());

		CloseTransaction( hr );

		return hr;

	}
	


	PlayerTransVoteGameAdvance::PlayerTransVoteGameAdvance( MessageDataPtr &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameInstance::VoteGameAdvanceRes, { return OnVoteRes(pRes); } );
	}

	Result PlayerTransVoteGameAdvance::OnVoteRes( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		//Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		//Message::GameInstance::VoteGameAdvanceRes res;

		svrChkClose(pRes->GetResult());
		//svrChk( res.ParseMessage( pMsgRes->GetMessage() ) );

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}
	
	// Start Transaction
	Result PlayerTransVoteGameAdvance::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		GameInsUID insUID;
		Policy::IPolicyGameInstance *pPolicy = nullptr;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetAuthTicket() != GetTicket() )
			svrErr(ResultCode::E_INVALID_TICKET);

		if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
			svrErr(ResultCode::E_INVALID_PLAYERID);

		insUID = GetMyOwner()->GetGameInsUID();
		if( insUID.UID == 0 )
			svrErrClose(ResultCode::E_INVALID_INSTANCEID);

		svrChkPtr( pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(insUID.GetServerID()) );

		svrChk( pPolicy->VoteGameAdvanceCmd( RouteContext(GetOwnerEntityUID(),insUID), GetTransID(), GetMyOwner()->GetPlayerID() ));
	
	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}
	
	// Start Transaction
	Result PlayerTransGameAdvanceVotedS2SEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );
		
		if( GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
			svrErrClose(ResultCode::E_INVALID_INSTANCEID);

		svrChkPtr(GetInterface());
		svrChk( GetInterface()->GameAdvanceVotedS2CEvt( GetRouteContext().GetFrom(), GetVoter() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}

	
	PlayerTransVote::PlayerTransVote( MessageDataPtr &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameInstance::VoteRes, { return OnVoteRes(pRes); } );
	}

	Result PlayerTransVote::OnVoteRes( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;

		//Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		//Message::GameInstance::VoteRes res;

		svrChkClose(pRes->GetResult());
		//svrChk( res.ParseMessage( pMsgRes->GetMessage() ) );

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}


	// Start Transaction
	Result PlayerTransVote::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		GameInsUID insUID;
		Policy::IPolicyGameInstance *pPolicy = nullptr;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetAuthTicket() != GetTicket() )
			svrErr(ResultCode::E_INVALID_TICKET);

		if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
			svrErr(ResultCode::E_INVALID_PLAYERID);

		insUID = GetMyOwner()->GetGameInsUID();
		if( insUID.UID == 0 )
			svrErrClose(ResultCode::E_INVALID_INSTANCEID);

		svrChkPtr( pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(insUID.GetServerID()) );

		svrChk( pPolicy->VoteCmd( RouteContext(GetOwnerEntityUID(),insUID), GetTransID(),
			GetMyOwner()->GetPlayerID(), GetVoteTarget(), GetActionSerial() ) );
	
	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}
	
	// Start Transaction
	Result PlayerTransVoteEndS2SEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
			svrErrClose(ResultCode::E_INVALID_INSTANCEID);

		svrChkPtr(GetInterface());
		svrChk( GetInterface()->VoteEndS2CEvt( GetRouteContext().GetFrom(), GetVoted() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}


	// Start Transaction
	Result PlayerTransPlayerRevealedS2SEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );
		
		if( GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
			svrErrClose(ResultCode::E_INVALID_INSTANCEID);

		svrChkPtr(GetInterface());
		svrChk( GetInterface()->PlayerRevealedS2CEvt( GetRouteContext().GetFrom(), GetRevealedPlayerID(), GetRole(), GetReason() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}



	PlayerTransAdvanceGame::PlayerTransAdvanceGame( MessageDataPtr &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameInstance::AdvanceGameRes, { return OnAdvanceGameRes(pRes); } );
	}

	Result PlayerTransAdvanceGame::OnAdvanceGameRes( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;

		//Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		//Message::GameInstance::AdvanceGameRes res;

		svrChkPtr(pRes);
		svrChkClose(pRes->GetResult());
		//svrChk( res.ParseMessage( pMsgRes->GetMessage() ) );

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransAdvanceGame::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		GameInsUID insUID;
		Policy::IPolicyGameInstance *pPolicy = nullptr;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetAuthTicket() != GetTicket() )
			svrErr(ResultCode::E_INVALID_TICKET);

		if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
			svrErr(ResultCode::E_INVALID_PLAYERID);

		insUID = GetMyOwner()->GetGameInsUID();
		if( insUID.UID == 0 )
			svrErrClose(ResultCode::E_INVALID_INSTANCEID);

		svrChkPtr( pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(insUID.GetServerID()) );

		svrChk( pPolicy->AdvanceGameCmd( RouteContext(GetOwnerEntityUID(),insUID), GetTransID(),
			GetMyOwner()->GetPlayerID() ) );
	
	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}


	// Start Transaction
	Result PlayerTransGameAdvancedS2SEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );
		
		if( GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
			svrErrClose(ResultCode::E_INVALID_INSTANCEID);

		GetMyOwner()->SendPushNotify(BRPUSHMSG_SYNC);

		svrChkPtr(GetInterface());
		svrChk( GetInterface()->GameAdvancedS2CEvt( GetRouteContext().GetFrom(), GetTimeStamp(), GetGameState(), GetDay() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}


	PlayerTransGameEndedS2SEvt::PlayerTransGameEndedS2SEvt( MessageDataPtr &pIMsg )
		:UserTransactionS2SEvt( pIMsg )
	{
		//BR_TRANS_MESSAGE( DB::QuerySetPlayerInfoCmd, { return OnUpdateDBRes(pRes); } );
		BR_TRANS_MESSAGE( DB::QueryUpdateGameEndCmd, { return OnUpdateDBRes(pRes); } );
	}

	Result PlayerTransGameEndedS2SEvt::OnUpdateDBRes(  Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( pRes->GetResult() );
		//DB::QuerySetPlayerInfoCmd *pMsgRes = (DB::QuerySetPlayerInfoCmd*)pRes;


	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}

	// Start Transaction
	Result PlayerTransGameEndedS2SEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		UserGamePlayerInfoSystem *pPlayerInfoSystem = nullptr;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
			svrErrClose(ResultCode::E_INVALID_INSTANCEID);

		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		GetMyOwner()->AddGameTransactionLogT(TransLogCategory::GameResult, GetIsWon(), GetGainedExp(), GetGainedGameMoney(), "Role:{0}, exp:{1}, money:{2}", GetPlayedRole(), pPlayerInfoSystem->GetExp(), pPlayerInfoSystem->GetGameMoney());

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
		if( GetInterface() != nullptr )
			GetInterface()->GameEndedS2CEvt( GetRouteContext().GetFrom(), GetWinner(), GetGainedExp(), GetGainedGameMoney() );

	Proc_End:

		if(!(hr) )
			CloseTransaction( hr );

		return hr;
	}

	// Start Transaction
	Result PlayerTransPlayerKilledS2SEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );
		
		if( GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
			svrErrClose(ResultCode::E_INVALID_INSTANCEID);

		svrChkPtr(GetInterface());
		svrChk( GetInterface()->PlayerKilledS2CEvt( GetRouteContext().GetFrom(), GetKilledPlayer(), GetReason() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}

	
	// Start Transaction
	Result PlayerTransPlayerVotedS2SEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
			svrErrClose(ResultCode::E_INVALID_INSTANCEID);

		svrChkPtr(GetInterface());
		svrChk( GetInterface()->VotedS2CEvt( GetRouteContext().GetFrom(), GetVoter(), GetVotedTarget() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}



	PlayerTransRequestGameMatch::PlayerTransRequestGameMatch( MessageDataPtr &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameParty::StartGameMatchRes, { return OnRequestPartyMatchingRes(pRes); } );
		BR_TRANS_MESSAGE( Message::PartyMatchingQueue::RegisterPlayerMatchingRes, { return OnRequestPlayerMatchingRes(pRes); } );
	}

	Result PlayerTransRequestGameMatch::OnRequestPartyMatchingRes( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameParty::StartGameMatchRes res;

		svrChkClose(pRes->GetResult());
		svrChk( res.ParseMessage( pMsgRes->GetMessage() ) );

	Proc_End:

		svrTrace(Svr::TRC_INFO, "PartyMatch requested PlayerID:{0}, hr:{1}", GetMyOwner()->GetPlayerID(), hr);

		CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	Result PlayerTransRequestGameMatch::OnRequestPlayerMatchingRes( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::PartyMatchingQueue::RegisterPlayerMatchingRes res;

		svrChkClose(pRes->GetResult());
		svrChk( res.ParseMessage( pMsgRes->GetMessage() ) );

		GetMyOwner()->SetMatchingTicket( res.GetMatchingTicket() );

		GetInterface()->GameMatchingStartedS2CEvt();

	Proc_End:

		svrTrace(Svr::TRC_INFO, "PlayerMatch requested PlayerID:{0}, hr:{1}", GetMyOwner()->GetPlayerID(), hr);

		CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}


	// Start Transaction
	Result PlayerTransRequestGameMatch::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ServerServiceInformation *pService = nullptr;
		conspiracy::OrganicTbl::OrganicItem *pCostItem = nullptr;
		UserGamePlayerInfoSystem* pPlayerInfoSystem = nullptr;
		INT currentStamina = 0, StaminaForGame = 0;

		m_TotalGem = 0;
		m_TotalGameMoney = 0;

		svrChk( super::StartTransaction() );

		//if( GetMyOwner()->GetAuthTicket() != GetTicket() )
		//	svrErr(ResultCode::E_INVALID_TICKET);

		//if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
		//	svrErr(ResultCode::E_INVALID_PLAYERID);

		if( GetMyOwner()->GetGameInsUID() != 0 )
			svrErrClose(ResultCode::E_GAME_ALREADY_IN_GAME);

		if (GetMyOwner()->GetMatchingTicket() != 0)
		{
			if( Util::TimeSince(GetMyOwner()->GetMatchingStartTime()) < DurationMS(GameConst::MATCHING_TIMEOUT) )
				svrErrClose(ResultCode::SVR_ALREADY_INQUEUE);

			// clear timeouted tickets
			GetMyOwner()->SetMatchingTicket(0);
		}

		svrChkPtr(GetMyServer()->GetPresetGameConfig());
		// Update player state
		pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>();
		GetMyOwner()->UpdateGamePlayer();

		currentStamina = pPlayerInfoSystem->GetStamina();
		StaminaForGame = GetMyServer()->GetPresetGameConfig()->StaminaForGame;
		if( currentStamina < StaminaForGame )
			svrErrClose(ResultCode::E_GAME_LOW_STAMINA);

		if (GetRequestRole() != PlayerRole::None)
		{
			svrChkCloseErr(ResultCode::E_GAME_INVALID_COSTID, conspiracy::OrganicTbl::FindItem((int)conspiracy::OrganicTbl::EItemEffect::Enum::RoleChoice, pCostItem));
			svrChkCloseErr(ResultCode::E_GAME_NOTENOUGH_RESOURCE, pPlayerInfoSystem->CheckCost(pCostItem));
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

			svrChk( pService->GetService<Svr::PartyMatchingQueueService>()->RegisterPlayerMatchingCmd(GetTransID(), 0, GetMyOwner()->GetPlayerID() ) );
		}
		else
		{
			// Let the party handle this
			Policy::IPolicyGameParty *pPolicy = nullptr;
			svrChkPtr( pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameParty>( GetMyOwner()->GetPartyUID().GetServerID()) );
			svrChk( pPolicy->StartGameMatchCmd( RouteContext(GetOwnerEntityUID(),GetMyOwner()->GetPartyUID()), GetTransID(), GetMyOwner()->GetPlayerID(), GetNumPlayer() ) );
		}
	
	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}
	


	PlayerTransCancelGameMatch::PlayerTransCancelGameMatch( MessageDataPtr &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( Message::GameParty::CancelGameMatchRes, { return OnCancelPartyMatchingRes(pRes); } );
		BR_TRANS_MESSAGE( Message::PartyMatchingQueue::UnregisterMatchingRes, { return OnCancelPlayerMatchingRes(pRes); } );
	}

	Result PlayerTransCancelGameMatch::OnCancelPartyMatchingRes( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameParty::CancelGameMatchRes res;

		svrChkClose(pRes->GetResult());
		svrChk( res.ParseMessage( pMsgRes->GetMessage() ) );

		// This means it just pended for canceling. yo have to wait canceled event from matching queue
		//GetMyOwner()->SetMatchingTicket(0);

		//GetInterface()->GameMatchingCanceledS2CEvt();

	Proc_End:

		svrTrace(Svr::TRC_INFO, "PartyMatch canceled PlayerID:{0}, hr:{1}", GetMyOwner()->GetPlayerID(), hr);

		CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}
	
	Result PlayerTransCancelGameMatch::OnCancelPlayerMatchingRes( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::PartyMatchingQueue::UnregisterMatchingRes res;

		switch((int32_t)pRes->GetResult() )
		{
		case ResultCode::SVR_RESERVED_QUEUEITEM:
			svrErrClose(ResultCode::SVR_RESERVED_QUEUEITEM);
			break;
		case ResultCode::SVR_QUEUEITEM_CANCELED:
		case ResultCode::SVR_INVALID_QUEUEITEM:
			GetMyOwner()->SetMatchingTicket( 0 );
			break;
		default:
			svrChk(pRes->GetResult());
			svrChk( res.ParseMessage( pMsgRes->GetMessage() ) );
			GetMyOwner()->SetMatchingTicket( 0 );
			break;
		};


	Proc_End:

		svrTrace(Svr::TRC_INFO, "PlayerMatch canceled PlayerID:{0}, hr:{1}", GetMyOwner()->GetPlayerID(), hr);

		CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}


	// Start Transaction
	Result PlayerTransCancelGameMatch::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetMatchingTicket() == 0 )
		{
			svrErrClose(ResultCode::SVR_INVALID_QUEUEITEM);
		}

		if( GetMyOwner()->GetPartyUID().UID == 0 )
		{
			Policy::IPolicyPartyMatchingQueue *pPolicy = nullptr;
			// Player isn't in a party, just do it alone
			svrChkPtr( pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyPartyMatchingQueue>( GetMyOwner()->GetMatchingTicket().QueueUID.GetServerID()) );

			svrChk( pPolicy->UnregisterMatchingCmd( RouteContext(GetOwnerEntityUID(),GetMyOwner()->GetMatchingTicket().QueueUID), GetTransID(), 0, GetMyOwner()->GetMatchingTicket() ) );
		}
		else
		{
			// Let the party handle this
			Policy::IPolicyGameParty *pPolicy = nullptr;
			svrChkPtr( pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameParty>( GetMyOwner()->GetPartyUID().GetServerID()) );
			svrChk( pPolicy->CancelGameMatchCmd( RouteContext(GetOwnerEntityUID(),GetMyOwner()->GetPartyUID()), GetTransID(), GetMyOwner()->GetPlayerID() ) );
		}
	
	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

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

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetMatchingTicket() != GetMatchingTicket() )
		{
			svrErr(ResultCode::SVR_INVALID_QUEUEITEM);
		}

		GetMyOwner()->SetMatchingTicket(0);

		svrChkPtr(GetInterface());
		svrChk( GetInterface()->GameMatchingCanceledS2CEvt() );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	
	// Start Transaction
	Result PlayerTransPlayerMatchingItemDequeuedS2CEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );
		
		if( GetMyOwner()->GetMatchingTicket() != GetMatchingTicket() )
		{
			svrErrClose(ResultCode::SVR_INVALID_QUEUEITEM);
		}

		GetMyOwner()->SetMatchingTicket(0);

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}




	PlayerTransPlayAgain::PlayerTransPlayAgain(MessageDataPtr &pIMsg)
		:MessageTransaction(pIMsg)
	{
		BR_TRANS_MESSAGE(Message::GameInstance::GamePlayAgainRes, { return OnPlayAgainRes(pRes); });
		BR_TRANS_MESSAGE(Message::GamePartyManager::CreatePartyRes, { return OnCreatePartyRes(pRes); });
	}

	Result PlayerTransPlayAgain::OnPlayAgainRes(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		conspiracy::OrganicTbl::OrganicItem* pCostItem = nullptr;
		UserGamePlayerInfoSystem* pPlayerInfoSystem = nullptr;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameInstance::GamePlayAgainRes res;

		svrChkPtr(pRes);
		svrChkClose(pRes->GetResult());
		svrChk( res.ParseMessage( pMsgRes->GetMessage() ) );

		// This means it's not full game
		//if (m_RetryCount <= 1 
		//	&& pRes->GetResult() == Result(ResultCode::E_GAME_INVALID_PLAYER_COUNT) // if some members are missing
		//	&& GetMyOwner()->GetPartyUID() == 0)
		//{
		//	svrChk(RequestCreateParty()));
		//}
		//else
		if (res.GetReplayMemberCount() > GameConst::MAX_GAMEPLAYER)
			svrErr(ResultCode::E_GAME_INVALID_PLAYER_COUNT);


		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		svrChkCloseErr(ResultCode::E_GAME_INVALID_COSTID, conspiracy::OrganicTbl::FindItem((int)conspiracy::OrganicTbl::EItemEffect::Enum::ReStart, pCostItem));
		svrChkCloseErr(ResultCode::E_GAME_NOTENOUGH_RESOURCE, pPlayerInfoSystem->ApplyCost(pCostItem, TransLogCategory::Buy, "PlayAgain"));

		m_TotalGem = pPlayerInfoSystem->GetGem();
		m_TotalGameMoney = pPlayerInfoSystem->GetGameMoney();


	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}

	Result PlayerTransPlayAgain::OnCreatePartyRes(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GamePartyManager::CreatePartyRes res;

		svrChkPtr(pRes);
		svrChkClose(pRes->GetResult());
		svrChk( res.ParseMessage( pMsgRes->GetMessage() ) );

		GetMyOwner()->SetPartyUID(res.GetRouteContext().GetFrom());

		svrChk(RequestPlayAgain());

	Proc_End:

		if (!(hr))
			CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}


	Result PlayerTransPlayAgain::RequestPlayAgain()
	{
		Result hr = ResultCode::SUCCESS;
		Policy::IPolicyGameInstance *pPolicy = nullptr;
		GameInsUID insUID = GetMyOwner()->GetGameInsUID();

		if (insUID == 0)
		{
			svrErr(ResultCode::SVR_INVALID_ENTITYUID);
		}

		svrChkPtr(pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(insUID.GetServerID()));
		svrChk(pPolicy->GamePlayAgainCmd(RouteContext(GetOwnerEntityUID(), insUID), GetTransID(), GetMyOwner()->GetPlayerID(), GetMyOwner()->GetPartyUID()));

	Proc_End:

		return hr;
	}

	Result PlayerTransPlayAgain::RequestCreateParty()
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ServerServiceInformation *pService = nullptr;

		svrChk(Svr::GetServerComponent<Svr::GamePartyManagerServiceEntity>()->GetService(pService));

		svrChk(pService->GetService<Svr::GamePartyManagerService>()->CreatePartyCmd(GetTransID(), 0, GetMyOwner()->GetPlayerInformation()));

	Proc_End:

		return hr;
	}

	// Start Transaction
	Result PlayerTransPlayAgain::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		conspiracy::OrganicTbl::OrganicItem* pCostItem = nullptr;
		UserGamePlayerInfoSystem* pPlayerInfoSystem = nullptr;

		m_TotalGem = 0;
		m_TotalGameMoney = 0;

		svrChk(super::StartTransaction());

		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		svrChkCloseErr(ResultCode::E_GAME_INVALID_COSTID, conspiracy::OrganicTbl::FindItem((int)conspiracy::OrganicTbl::EItemEffect::Enum::ReStart, pCostItem));
		svrChkCloseErr(ResultCode::E_GAME_NOTENOUGH_RESOURCE, pPlayerInfoSystem->CheckCost(pCostItem));


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

		if (!(hr))
			CloseTransaction(hr);

		return hr;
	}



	PlayerTransPlayAgainS2SEvt::PlayerTransPlayAgainS2SEvt(MessageDataPtr &pIMsg)
		:UserTransactionS2SEvt(pIMsg)
	{
		BR_TRANS_MESSAGE(Message::GameParty::JoinPartyRes, { return OnJoinPartyRes(pRes); });
	}

	Result PlayerTransPlayAgainS2SEvt::OnJoinPartyRes(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameParty::JoinPartyRes res;

		svrChk(pRes->GetResult());
		svrChk(res.ParseMessage(pMsgRes->GetMessage()));

		GetMyOwner()->SetPartyUID(GetPartyUID());

		m_LeadPlayer = GetLeadPlayer();
		m_PartyUID = GetPartyUID();

		{
			auto pPolicy = GetInterface();
			if (pPolicy != nullptr)
				pPolicy->GamePlayAgainS2CEvt(GetPartyUID(), GetLeadPlayer());
		}

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}

	// Start Transaction
	Result PlayerTransPlayAgainS2SEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Policy::IPolicyGameParty *pPolicy = nullptr;
		Svr::ServerEntity *pServerEntity = nullptr;

		m_PartyUID = 0;
		m_LeadPlayer = 0;

		svrChk(super::StartTransaction());

		if (GetTargetPlayer() != GetMyOwner()->GetPlayerID())
			svrErr(ResultCode::E_INVALID_PLAYERID);

		GetMyOwner()->SetGameInsUID(0);

		if (GetMyOwner()->GetPartyUID() != 0)
		{
			if (GetMyOwner()->GetPartyUID() != GetPartyUID())
				svrErr(ResultCode::E_GAME_ALREADY_IN_PARTY);

			m_LeadPlayer = GetLeadPlayer();
			m_PartyUID = GetPartyUID();

			// I'm in the correct party
			GetInterface()->GamePlayAgainS2CEvt(GetPartyUID(), GetLeadPlayer());

			CloseTransaction(hr);
		}
		else
		{
			svrChk(Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity(GetPartyUID().GetServerID(), pServerEntity));
			svrChkPtr(pPolicy = pServerEntity->GetInterface<Policy::IPolicyGameParty>());
			svrChk(pPolicy->JoinPartyCmd(RouteContext(GetOwnerEntityUID(), GetPartyUID()), GetTransID(), GetLeadPlayer(), GetMyOwner()->GetPlayerInformation()));
		}

	Proc_End:

		if (!(hr))
			CloseTransaction(hr);

		return hr;
	}




	PlayerTransGameRevealPlayer::PlayerTransGameRevealPlayer(MessageDataPtr &pIMsg)
		:MessageTransaction(pIMsg)
	{
		BR_TRANS_MESSAGE(DB::QueryUpdateTickStatusCmd, { return OnUpdatePlayerRes(pRes); });
		BR_TRANS_MESSAGE(Message::GameInstance::GameRevealPlayerRes, { return OnGameRevealPlayerRes(pRes); });
	}

	Result PlayerTransGameRevealPlayer::OnUpdatePlayerRes(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		Policy::IPolicyGameInstance *pPolicy = nullptr;
		GameInsUID insUID = GetMyOwner()->GetGameInsUID();

		//auto *pDBRes = (DB::QueryUpdateTickStatusCmd*)pRes;

		svrChkPtr(pRes);
		svrChkClose(pRes->GetResult());

		if (insUID == 0)
		{
			svrErr(ResultCode::SVR_INVALID_ENTITYUID);
		}

		svrChkPtr(pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(insUID.GetServerID()));
		svrChk(pPolicy->GameRevealPlayerCmd(RouteContext(GetOwnerEntityUID(), insUID), GetTransID(), GetMyOwner()->GetPlayerID(), GetTargetPlayerID()));

	Proc_End:

		if (!(hr))
			CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}

	Result PlayerTransGameRevealPlayer::OnGameRevealPlayerRes(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameInstance::GameRevealPlayerRes res;
		UserGamePlayerInfoSystem* pPlayerInfoSystem = nullptr;

		svrChkPtr(pRes);
		svrChkClose(pRes->GetResult());
		svrChk(res.ParseMessage(pMsgRes->GetMessage()));

		for (uint iPlayer = 0; iPlayer < res.GetRevealedPlayerID().GetSize(); iPlayer++)
		{
			m_RevealedPlayerID.push_back(res.GetRevealedPlayerID()[iPlayer]);
		}
		for (uint iPlayer = 0; iPlayer < res.GetRevealedRole().GetSize(); iPlayer++)
		{
			m_RevealedPlayerRole.push_back(res.GetRevealedRole()[iPlayer]);
		}

		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		m_TotalGem = pPlayerInfoSystem->GetGem();
		m_TotalGameMoney = pPlayerInfoSystem->GetGameMoney();

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}


	// Start Transaction
	Result PlayerTransGameRevealPlayer::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		conspiracy::OrganicTbl::OrganicItem *pCostItem = nullptr;
		UserGamePlayerInfoSystem* pPlayerInfoSystem = nullptr;

		m_TotalGem = 0;
		m_TotalGameMoney = 0;

		svrChk(super::StartTransaction());

		if (GetMyOwner()->GetGameInsUID() == 0)
		{
			svrErrClose(ResultCode::E_INVALID_INSTANCEID);
		}

		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		svrChkCloseErr(ResultCode::E_GAME_INVALID_COSTID, conspiracy::OrganicTbl::FindItem((int)conspiracy::OrganicTbl::EItemEffect::Enum::Check, pCostItem));
		svrChkCloseErr(ResultCode::E_GAME_NOTENOUGH_RESOURCE, pPlayerInfoSystem->ApplyCost(pCostItem, TransLogCategory::Buy, "RevealPlayer"));


		svrChk(GetMyOwner()->UpdateDBSync(GetTransID()));

	Proc_End:

		if (!(hr))
			CloseTransaction(hr);

		return hr;
	}



	PlayerTransGamePlayerRevive::PlayerTransGamePlayerRevive(MessageDataPtr &pIMsg)
		:MessageTransaction(pIMsg)
	{
		BR_TRANS_MESSAGE(DB::QueryUpdateTickStatusCmd, { return OnUpdatePlayerRes(pRes); });
		BR_TRANS_MESSAGE(Message::GameInstance::GamePlayerReviveRes, { return OnGamePlayerReviveRes(pRes); });
	}

	Result PlayerTransGamePlayerRevive::OnUpdatePlayerRes(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		Policy::IPolicyGameInstance *pPolicy = nullptr;
		GameInsUID insUID = GetMyOwner()->GetGameInsUID();

		//auto *pDBRes = (DB::QueryUpdateTickStatusCmd*)pRes;

		svrChkPtr(pRes);
		svrChkClose(pRes->GetResult());

		if (insUID == 0)
		{
			svrErr(ResultCode::SVR_INVALID_ENTITYUID);
		}

		svrChkPtr(pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(insUID.GetServerID()));
		svrChk(pPolicy->GamePlayerReviveCmd(RouteContext(GetOwnerEntityUID(), insUID), GetTransID(), GetMyOwner()->GetPlayerID()));

	Proc_End:

		if (!(hr))
			CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}

	Result PlayerTransGamePlayerRevive::OnGamePlayerReviveRes(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameInstance::GameRevealPlayerRes res;

		svrChkPtr(pRes);
		svrChkClose(pRes->GetResult());
		svrChk(res.ParseMessage(pMsgRes->GetMessage()));


	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}


	// Start Transaction
	Result PlayerTransGamePlayerRevive::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		conspiracy::OrganicTbl::OrganicItem *pCostItem = nullptr;
		UserGamePlayerInfoSystem* pPlayerInfoSystem = nullptr;

		m_TotalGem = 0;
		m_TotalGameMoney = 0;

		svrChk(super::StartTransaction());

		if (GetMyOwner()->GetGameInsUID() == 0)
		{
			svrErrClose(ResultCode::E_INVALID_INSTANCEID);
		}

		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		svrChkCloseErr(ResultCode::E_GAME_INVALID_COSTID, conspiracy::OrganicTbl::FindItem((int)conspiracy::OrganicTbl::EItemEffect::Enum::Revival, pCostItem));
		svrChkCloseErr(ResultCode::E_GAME_NOTENOUGH_RESOURCE, pPlayerInfoSystem->ApplyCost(pCostItem, TransLogCategory::Buy, "Revive"));

		m_TotalGem = pPlayerInfoSystem->GetGem();
		m_TotalGameMoney = pPlayerInfoSystem->GetGameMoney();

		svrChk(GetMyOwner()->UpdateDBSync(GetTransID()));

	Proc_End:

		if (!(hr))
			CloseTransaction(hr);

		return hr;
	}




	// Start Transaction
	Result PlayerTransGamePlayerRevivedS2SEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		//Policy::IPolicyGameParty *pPolicy = nullptr;
		//Svr::ServerEntity *pServerEntity = nullptr;

		svrChk(super::StartTransaction());

		if (GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
			svrErrClose(ResultCode::E_INVALID_INSTANCEID);

		GetInterface()->GamePlayerRevivedS2CEvt(GetRevivedPlayerID());


	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


	// Start Transaction
	Result PlayerTransGamePlayerResetRank::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		UserGamePlayerInfoSystem* pPlayerInfoSystem = nullptr;
		conspiracy::OrganicTbl::OrganicItem *pCostItem = nullptr;

		m_TotalGem = 0;
		m_TotalGameMoney = 0;

		svrChk(super::StartTransaction());

		if (GetMyOwner()->GetGameInsUID() != 0)
		{
			svrErrClose(ResultCode::SVR_INVALID_PLAYER_STATE);
		}

		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());


		svrChkCloseErr(ResultCode::E_GAME_INVALID_COSTID, conspiracy::OrganicTbl::FindItem((int)conspiracy::OrganicTbl::EItemEffect::Enum::ResetRankNormal, pCostItem));
		svrChkCloseErr(ResultCode::E_GAME_NOTENOUGH_RESOURCE, pPlayerInfoSystem->ApplyCost(pCostItem, TransLogCategory::Buy, "ResetRank"));

		svrChkClose(pPlayerInfoSystem->ResetRankNormal(pCostItem));

		m_TotalGem = pPlayerInfoSystem->GetGem();
		m_TotalGameMoney = pPlayerInfoSystem->GetGameMoney();

		svrChk(GetMyOwner()->UpdateDBSync());

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


};// namespace GameServer 
};// namespace SF 

