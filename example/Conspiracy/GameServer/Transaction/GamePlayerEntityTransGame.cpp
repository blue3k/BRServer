////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "GameServerPCH.h"
#include "GameServer.h"
#include "GameServerClass.h"

#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "ResultCode/SFResultCodeLogin.h"
#include "MemoryManager/SFMemoryPool.h"
#include "Types/SFEngineTypedefs.h"


#include "GameServerClass.h"
#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "ServerEntity/ServerEntityManager.h"

#include "Protocol/ServerService/PartyMatchingQueueService.h"
#include "ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServiceEntity/Game/PlayerDirectoryManager.h"
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
#include "GameInstance/GamePlayerEntity.h"

#include "Server/BrServer.h"


#include "GameSystem/UserFriendSystem.h"
#include "GameSystem/UserGamePlayerInfoSystem.h"

#include "DB/AccountDB.h"
#include "DB/AccountQuery.h"
#include "GameConspiracyDB.h"
#include "GameConspiracyQuery.h"

#include "conspiracy/OrganicTbl.h"







namespace SF {
namespace GameServer {



	/////////////////////////////////////////////////////////////////////////////
	//
	//	Game command transaction
	//


	PlayerTransJoinGame::PlayerTransJoinGame( IHeap& heap, MessageDataPtr &pIMsg )
		:MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg))
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

		svrChk( joinRes.ParseMessage( *pMsgRes->GetMessage() ) );

		GetMyOwner()->AddGameTransactionLog(TransLogCategory::Game, 1, 0, joinRes.GetRouteContext().GetFrom().UID);

		GetMyOwner()->SetGameInsUID( joinRes.GetRouteContext().GetFrom());

		m_GameInsID = joinRes.GetRouteContext().GetFrom();
		m_TimeStamp = joinRes.GetTimeStamp();
		m_GameState = (GameStateID)joinRes.GetGameState();
		m_Day = joinRes.GetDay();
		m_Role = (PlayerRole)joinRes.GetRole();
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
		if (GetMyOwner()->GetPartyUID().UID != 0)
		{
			//Svr::ServerEntity *pServerEntity = nullptr;

			//svrChk(Service::ServerEntityManager->GetServerEntity(GetMyOwner()->GetPartyUID().GetServerID(), pServerEntity));
			auto serverEndpoint = Service::MessageEndpointManager->GetEndpoint(GetMyOwner()->GetPartyUID());

			svrChk(NetPolicyGameParty(serverEndpoint).LeavePartyCmd(RouteContext(GetOwnerEntityUID(), GetMyOwner()->GetPartyUID()), GetTransID(), GetMyOwner()->GetPlayerID()));
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

		svrChk(leavePartyRes.ParseMessage(*pMsgRes->GetMessage()));

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
		MessageEndpoint* serverEndpoint{};

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
			svrErr(ResultCode::INVALID_TICKET);

		if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
			svrErr(ResultCode::INVALID_PLAYERID);

		if( GetMyOwner()->GetGameInsUID().UID != 0 && GetMyOwner()->GetGameInsUID().UID != GetInsUID() )
			svrErr(ResultCode::GAME_ALREADY_IN_GAME);

		if( GetInsUID() == 0 )
			svrErr(ResultCode::INVALID_INSTANCEID);

		// We can't check stamina here because it could be a rejoin case
		//svrChkPtr(GetMyServer()->GetPresetGameConfig());
		//if( GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GetStamina() < GetMyServer()->GetPresetGameConfig()->StaminaForGame )
		//	svrErrClose(ResultCode::GAME_LOW_STAMINA);

		insUID = GetInsUID();

		serverEndpoint = Service::MessageEndpointManager->GetEndpoint(insUID);

		svrChk(NetPolicyGameInstance(serverEndpoint).JoinGameCmd(
			RouteContext(GetOwnerEntityUID(),insUID), GetTransID(),
			GetMyOwner()->GetPlayerInformation(), (uint8_t)PlayerRole::None ) );

	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}

	Result PlayerTransJoinGame::CloseTransaction(Result hrRes)
	{
		Result result = super::CloseTransaction(hrRes);
		svrTrace(SVR_INFO, "JoinGameRes PlayerID:{0} : hr:{1}", GetMyOwner()->GetPlayerID(), hrRes);
		return result;
	}


	PlayerTransLeaveGame::PlayerTransLeaveGame(IHeap& heap, MessageDataPtr &pIMsg )
		:MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg))
	{
		BR_TRANS_MESSAGE( Message::GameInstance::LeaveGameInstanceRes, { return OnLeaveGameRes(pRes); } );
	}

	Result PlayerTransLeaveGame::OnLeaveGameRes( Svr::TransactionResult* & pRes )
	{
		Result hr = ResultCode::SUCCESS;
		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
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

		CloseTransaction( hr );

		return ResultCode::SUCCESS; 
	}

	Result PlayerTransLeaveGame::CloseTransaction(Result hrRes)
	{
		Result result = super::CloseTransaction(hrRes);
		svrTrace(SVR_INFO, "LeaveGameRes PlayerID:{0} : hr:{1}", GetMyOwner()->GetPlayerID(), hrRes);
		return result;
	}

	// Start Transaction
	Result PlayerTransLeaveGame::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		GameInsUID insUID;
		NetPolicyGameInstance *pPolicy = nullptr;
		MessageEndpoint* serverEndpoint{};

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetAuthTicket() != GetTicket() )
			svrErr(ResultCode::INVALID_TICKET);

		if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
			svrErr(ResultCode::INVALID_PLAYERID);

		insUID = GetMyOwner()->GetGameInsUID();
		if( insUID.UID == 0 )
			svrErrClose(ResultCode::INVALID_INSTANCEID);

		serverEndpoint = Service::MessageEndpointManager->GetEndpoint(insUID);

		svrChk(NetPolicyGameInstance(serverEndpoint).LeaveGameInstanceCmd(
			RouteContext(GetOwnerEntityUID(),insUID), GetTransID(), GetMyOwner()->GetPlayerID() ) );

	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}
	

	PlayerTransKickPlayer::PlayerTransKickPlayer(IHeap& heap, MessageDataPtr &pIMsg )
		:MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg))
	{
		BR_TRANS_MESSAGE( Message::GameInstance::KickPlayerRes, { return OnKickPlayerRes(pRes); } );
	}

	Result PlayerTransKickPlayer::OnKickPlayerRes( Svr::TransactionResult* & pRes )
	{
		Result hr = ResultCode::SUCCESS;
		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameInstance::KickPlayerRes msgRes;

		svrChkClose(pRes->GetResult());

		svrChk( msgRes.ParseMessage(* pMsgRes->GetMessage() ) );

	Proc_End:

		CloseTransaction( hr );

		return ResultCode::SUCCESS; 
	}

	Result PlayerTransKickPlayer::CloseTransaction(Result hrRes)
	{
		Result result = super::CloseTransaction(hrRes);
		svrTrace(SVR_INFO, "KickPlayerRes PlayerID:{0}, KickPlayerID:{1}, hr:{2}", GetMyOwner()->GetPlayerID(), GetPlayerID(), hrRes);
		return result;
	}

	// Start Transaction
	Result PlayerTransKickPlayer::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		GameInsUID insUID;
		NetPolicyGameInstance *pPolicy = nullptr;
		MessageEndpoint* serverEndpoint{};

		svrChk( super::StartTransaction() );

		//if( GetMyOwner()->GetAuthTicket() != GetTicket() )
		//	svrErr(ResultCode::INVALID_TICKET);

		if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
			svrErr(ResultCode::INVALID_PLAYERID);

		insUID = GetMyOwner()->GetGameInsUID();
		if( insUID.UID == 0 )
			svrErrClose(ResultCode::INVALID_INSTANCEID);

		serverEndpoint = Service::MessageEndpointManager->GetEndpoint(insUID);

		svrChk(NetPolicyGameInstance(serverEndpoint).KickPlayerCmd(
			RouteContext(GetOwnerEntityUID(), insUID.GetServerID()), GetTransID(),
			GetMyOwner()->GetPlayerID(), GetPlayerToKick() ) );

	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}
	//
	//// Start Transaction
	//Result PlayerTransPlayerKickedS2SEvt::StartTransaction()
	//{
	//	Result hr = ResultCode::SUCCESS;

	//	svrChk( super::StartTransaction() );

	//	if( GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
	//		svrErrClose(ResultCode::INVALID_INSTANCEID);

	//	if( GetKickedPlayerID() == GetMyOwner()->GetPlayerID() )
	//	{
	//		GetMyOwner()->SetGameInsUID(0);
	//	}

	//	svrChk( NetSvrPolicyGame(GetRemoteEndpoint()).PlayerKickedS2CEvt( GetRouteContext().GetFrom(), GetKickedPlayerID() ) );

	//Proc_End:

	//	CloseTransaction( hr );

	//	return hr;
	//}



	PlayerTransAssignRole::PlayerTransAssignRole(IHeap& heap, MessageDataPtr &pIMsg )
		:MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg))
	{
		BR_TRANS_MESSAGE( Message::GameInstance::AssignRoleRes, { return OnAssignRoleRes(pRes); } );
	}

	Result PlayerTransAssignRole::OnAssignRoleRes( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameInstance::AssignRoleRes res;

		svrChkClose(pRes->GetResult());
		svrChk( res.ParseMessage(* pMsgRes->GetMessage() ) );

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransAssignRole::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		GameInsUID insUID;
		NetPolicyGameInstance *pPolicy = nullptr;
		MessageEndpoint* serverEndpoint{};

		svrChk( super::StartTransaction() );

		//if( GetMyOwner()->GetAuthTicket() != GetTicket() )
		//	svrErr(ResultCode::INVALID_TICKET);

		//if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
		//	svrErr(ResultCode::INVALID_PLAYERID);

		insUID = GetMyOwner()->GetGameInsUID();
		if( insUID.UID == 0 )
			svrErrClose(ResultCode::INVALID_INSTANCEID);

		serverEndpoint = Service::MessageEndpointManager->GetEndpoint(insUID);

		svrChk(NetPolicyGameInstance(serverEndpoint).AssignRoleCmd( 
			RouteContext(GetOwnerEntityUID(),insUID), GetTransID(),
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
			svrErrClose(ResultCode::INVALID_INSTANCEID);

		//GetMyOwner()->SendPushNotify(BRPUSHMSG_SYNC);

		svrChk( NetSvrPolicyGame(GetRemoteEndpoint()).RoleAssignedS2CEvt( GetRouteContext().GetFrom(), GetMyOwner()->GetPlayerID(), GetRole() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}



	// Start Transaction
	Result PlayerTransChatMessage::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		GameInsUID insUID;
		NetPolicyGameInstance *pPolicy = nullptr;
		MessageEndpoint* serverEndpoint{};

		svrChk( super::StartTransaction() );

		//if( GetMyOwner()->GetAuthTicket() != GetTicket() )
		//	svrErr(ResultCode::INVALID_TICKET);

		//if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
		//	svrErr(ResultCode::INVALID_PLAYERID);
		
		insUID = GetMyOwner()->GetGameInsUID();
		if( insUID.UID == 0 )
			svrErrClose(ResultCode::INVALID_INSTANCEID);

		serverEndpoint = Service::MessageEndpointManager->GetEndpoint(insUID);

		svrChk(NetPolicyGameInstance(serverEndpoint).ChatMessageC2SEvt( 
			RouteContext(GetOwnerEntityUID(),insUID),
			GetMyOwner()->GetPlayerID(), GetRole(), GetChatMessage() ) );

	Proc_End:


		svrTrace(SVR_INFO, "ChatMessage PlayerID:{0}, hr:{1}", GetMyOwner()->GetPlayerID(), hr);

		CloseTransaction( hr );

		return hr;
	}


	// Start Transaction
	Result PlayerTransChatMessageFromOtherEntity::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		svrChk( NetSvrPolicyGame(GetRemoteEndpoint()).ChatMessageS2CEvt( GetSenderID(), GetRole(), GetSenderName(), GetChatMessage() ) );

	Proc_End:

		svrTrace(SVR_INFO, "ChatMessage received PlayerID:{0}, fromPlayer:{1}", GetMyOwner()->GetPlayerID(), GetSenderName());

		CloseTransaction( hr );

		return hr;

	}
	


	PlayerTransVoteGameAdvance::PlayerTransVoteGameAdvance(IHeap& heap, MessageDataPtr &pIMsg )
		:MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg))
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
		NetPolicyGameInstance *pPolicy = nullptr;
		MessageEndpoint* serverEndpoint{};

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetAuthTicket() != GetTicket() )
			svrErr(ResultCode::INVALID_TICKET);

		if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
			svrErr(ResultCode::INVALID_PLAYERID);

		insUID = GetMyOwner()->GetGameInsUID();
		if( insUID.UID == 0 )
			svrErrClose(ResultCode::INVALID_INSTANCEID);

		serverEndpoint = Service::MessageEndpointManager->GetEndpoint(insUID);

		svrChk(NetPolicyGameInstance(serverEndpoint).VoteGameAdvanceCmd( 
			RouteContext(GetOwnerEntityUID(),insUID), GetTransID(), GetMyOwner()->GetPlayerID() ));
	
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
			svrErrClose(ResultCode::INVALID_INSTANCEID);

		svrChk( NetSvrPolicyGame(GetRemoteEndpoint()).GameAdvanceVotedS2CEvt( GetRouteContext().GetFrom(), GetVoter() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}

	
	PlayerTransVote::PlayerTransVote(IHeap& heap, MessageDataPtr &pIMsg )
		:MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg))
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
		NetPolicyGameInstance *pPolicy = nullptr;
		MessageEndpoint* serverEndpoint{};

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetAuthTicket() != GetTicket() )
			svrErr(ResultCode::INVALID_TICKET);

		if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
			svrErr(ResultCode::INVALID_PLAYERID);

		insUID = GetMyOwner()->GetGameInsUID();
		if( insUID.UID == 0 )
			svrErrClose(ResultCode::INVALID_INSTANCEID);

		serverEndpoint = Service::MessageEndpointManager->GetEndpoint(insUID);

		svrChk(NetPolicyGameInstance(serverEndpoint).VoteCmd( 
			RouteContext(GetOwnerEntityUID(),insUID), GetTransID(),
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
			svrErrClose(ResultCode::INVALID_INSTANCEID);

		svrChk( NetSvrPolicyGame(GetRemoteEndpoint()).VoteEndS2CEvt( GetRouteContext().GetFrom(), GetVoted() ) );

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
			svrErrClose(ResultCode::INVALID_INSTANCEID);

		svrChk( NetSvrPolicyGame(GetRemoteEndpoint()).PlayerRevealedS2CEvt( GetRouteContext().GetFrom(), GetRevealedPlayerID(), GetRole(), GetReason() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}



	PlayerTransAdvanceGame::PlayerTransAdvanceGame(IHeap& heap, MessageDataPtr &pIMsg )
		:MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg))
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
		NetPolicyGameInstance *pPolicy = nullptr;
		MessageEndpoint* serverEndpoint{};

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetAuthTicket() != GetTicket() )
			svrErr(ResultCode::INVALID_TICKET);

		if( GetMyOwner()->GetPlayerID() != GetPlayerID() )
			svrErr(ResultCode::INVALID_PLAYERID);

		insUID = GetMyOwner()->GetGameInsUID();
		if( insUID.UID == 0 )
			svrErrClose(ResultCode::INVALID_INSTANCEID);

		serverEndpoint = Service::MessageEndpointManager->GetEndpoint(insUID);

		svrChk(NetPolicyGameInstance(serverEndpoint).AdvanceGameCmd( 
			RouteContext(GetOwnerEntityUID(),insUID), GetTransID(),
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
			svrErrClose(ResultCode::INVALID_INSTANCEID);

		GetMyOwner()->SendPushNotify(BRPUSHMSG_SYNC);

		svrChk( NetSvrPolicyGame(GetRemoteEndpoint()).GameAdvancedS2CEvt( GetRouteContext().GetFrom(), GetTimeStamp(), GetGameState(), GetDay() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}


	PlayerTransGameEndedS2SEvt::PlayerTransGameEndedS2SEvt(IHeap& heap, MessageDataPtr &pIMsg )
		:UserTransactionS2SEvt(heap, pIMsg )
	{
		//BR_TRANS_MESSAGE( DB::QuerySetPlayerInfoCmd, { return OnUpdateDBRes(pRes); } );
		BR_TRANS_MESSAGE( conspiracy::QueryUpdateGameEndCmd, { return OnUpdateDBRes(pRes); } );
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
			svrErrClose(ResultCode::INVALID_INSTANCEID);

		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		GetMyOwner()->AddGameTransactionLogT(TransLogCategory::GameResult, GetIsWon(), GetGainedExp(), GetGainedGameMoney(), "Role:{0}, exp:{1}, money:{2}", GetPlayedRole(), pPlayerInfoSystem->GetExp(), pPlayerInfoSystem->GetGameMoney());

		GetMyOwner()->SendPushNotify(BRPUSHMSG_SYNC);

		pPlayerInfoSystem->GainExp(GetGainedExp());
		pPlayerInfoSystem->GainGameMoney(GetGainedGameMoney());
		pPlayerInfoSystem->AchivedWin( (PlayerRole)GetPlayedRole(), GetIsWon() != 0 );

		// Save player record
		svrChk(Svr::GetServerComponent<conspiracy::GameConspiracyDB>()->UpdateGameEndCmd(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(),
			pPlayerInfoSystem->GetLevel(), 
			pPlayerInfoSystem->GetExp(), pPlayerInfoSystem->GetGameMoney(), 
			pPlayerInfoSystem->GetTotalPlayed(), 
			pPlayerInfoSystem->GetWinPlaySCitizen(), pPlayerInfoSystem->GetWinPlaySMonster(), pPlayerInfoSystem->GetWinPlaySSeer(),
			pPlayerInfoSystem->GetLosePlaySCitizen(), pPlayerInfoSystem->GetLosePlaySMonster(), pPlayerInfoSystem->GetLosePlaySSeer(),
			pPlayerInfoSystem->GetWinPlayNCitizen(), pPlayerInfoSystem->GetWinPlayNMonster(), pPlayerInfoSystem->GetWinPlayNSeer(),
			pPlayerInfoSystem->GetLosePlayNCitizen(), pPlayerInfoSystem->GetLosePlayNMonster(), pPlayerInfoSystem->GetLosePlayNSeer(),
			GetMyOwner()->GetLatestActiveTime() ) );

		// Ignore sending errors, anyway we got the result
		NetSvrPolicyGame(GetRemoteEndpoint()).GameEndedS2CEvt( GetRouteContext().GetFrom(), GetWinner(), GetGainedExp(), GetGainedGameMoney() );

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
			svrErrClose(ResultCode::INVALID_INSTANCEID);

		svrChk( NetSvrPolicyGame(GetRemoteEndpoint()).PlayerKilledS2CEvt( GetRouteContext().GetFrom(), GetKilledPlayer(), GetReason() ) );

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
			svrErrClose(ResultCode::INVALID_INSTANCEID);

		svrChk( NetSvrPolicyGame(GetRemoteEndpoint()).VotedS2CEvt( GetRouteContext().GetFrom(), GetVoter(), GetVotedTarget() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}






	PlayerTransPlayAgain::PlayerTransPlayAgain(IHeap& heap, MessageDataPtr &pIMsg)
		:MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg))
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
		svrChk( res.ParseMessage(* pMsgRes->GetMessage() ) );

		// This means it's not full game
		//if (m_RetryCount <= 1 
		//	&& pRes->GetResult() == Result(ResultCode::GAME_INVALID_PLAYER_COUNT) // if some members are missing
		//	&& GetMyOwner()->GetPartyUID() == 0)
		//{
		//	svrChk(RequestCreateParty()));
		//}
		//else
		if (res.GetReplayMemberCount() > GameLogItem::LEGACY_MAX_GAMEPLAYER)
			svrErr(ResultCode::GAME_INVALID_PLAYER_COUNT);


		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		svrChkCloseErr(ResultCode::GAME_INVALID_COSTID, conspiracy::OrganicTbl::FindItem((int)conspiracy::OrganicTbl::EItemEffect::Enum::ReStart, pCostItem));
		svrChkCloseErr(ResultCode::GAME_NOTENOUGH_RESOURCE, pPlayerInfoSystem->ApplyCost(pCostItem, TransLogCategory::Buy, "PlayAgain"));

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
		svrChk( res.ParseMessage(* pMsgRes->GetMessage() ) );

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
		GameInsUID insUID = GetMyOwner()->GetGameInsUID();
		MessageEndpoint* serverEndpoint{};

		if (insUID.UID == 0)
		{
			svrErr(ResultCode::SVR_INVALID_ENTITYUID);
		}

		serverEndpoint = Service::MessageEndpointManager->GetEndpoint(insUID);

		svrChk(NetPolicyGameInstance(serverEndpoint).GamePlayAgainCmd(
			RouteContext(GetOwnerEntityUID(), insUID), GetTransID(), GetMyOwner()->GetPlayerID(), GetMyOwner()->GetPartyUID()));

	Proc_End:

		return hr;
	}

	Result PlayerTransPlayAgain::RequestCreateParty()
	{
		Result hr = ResultCode::SUCCESS;
		SharedPointerT<ServerServiceInformation> pService;

		svrChk(Service::ServiceDirectory->GetRandomService(Service::ServerConfig->GameClusterID, ClusterID::GamePartyManager, pService));

		svrChk(pService->GetService<GamePartyManagerService>()->CreatePartyCmd(GetTransID(), 0, (uint32_t)Service::ServerConfig->GameClusterID, GetMyOwner()->GetPlayerInformation()));

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

		svrChkCloseErr(ResultCode::GAME_INVALID_COSTID, conspiracy::OrganicTbl::FindItem((int)conspiracy::OrganicTbl::EItemEffect::Enum::ReStart, pCostItem));
		svrChkCloseErr(ResultCode::GAME_NOTENOUGH_RESOURCE, pPlayerInfoSystem->CheckCost(pCostItem));


		//m_RetryCount = 0;
		if (GetMyOwner()->GetPartyUID().UID == 0)
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



	PlayerTransPlayAgainS2SEvt::PlayerTransPlayAgainS2SEvt(IHeap& heap, MessageDataPtr &pIMsg)
		: UserTransactionS2SEvt(heap, pIMsg)
	{
		BR_TRANS_MESSAGE(Message::GameParty::JoinPartyRes, { return OnJoinPartyRes(pRes); });
	}

	Result PlayerTransPlayAgainS2SEvt::OnJoinPartyRes(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameParty::JoinPartyRes res;

		svrChk(pRes->GetResult());
		svrChk(res.ParseMessage(*pMsgRes->GetMessage()));

		GetMyOwner()->SetPartyUID(GetPartyUID());

		m_LeadPlayer = GetLeadPlayer();
		m_PartyUID = GetPartyUID();

		{
			NetSvrPolicyGame(GetRemoteEndpoint()).GamePlayAgainS2CEvt(GetPartyUID(), GetLeadPlayer());
		}

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}

	// Start Transaction
	Result PlayerTransPlayAgainS2SEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ServerEntity *pServerEntity = nullptr;

		m_PartyUID = 0;
		m_LeadPlayer = 0;

		svrChk(super::StartTransaction());

		if (GetTargetPlayer() != GetMyOwner()->GetPlayerID())
			svrErr(ResultCode::INVALID_PLAYERID);

		GetMyOwner()->SetGameInsUID(0);

		if (GetMyOwner()->GetPartyUID().UID != 0)
		{
			if (GetMyOwner()->GetPartyUID().UID != GetPartyUID())
				svrErr(ResultCode::GAME_ALREADY_IN_PARTY);

			m_LeadPlayer = GetLeadPlayer();
			m_PartyUID = GetPartyUID();

			// I'm in the correct party
			NetSvrPolicyGame(GetRemoteEndpoint()).GamePlayAgainS2CEvt(GetPartyUID(), GetLeadPlayer());

			CloseTransaction(hr);
		}
		else
		{
			auto serverEndpoint = Service::MessageEndpointManager->GetEndpoint(GetPartyUID());

			//svrChk(Service::ServerEntityManager->GetServerEntity(EntityUID(GetPartyUID()).GetServerID(), pServerEntity));

			svrChk(NetPolicyGameParty(serverEndpoint).JoinPartyCmd(
				RouteContext(GetOwnerEntityUID(), GetPartyUID()), GetTransID(), GetLeadPlayer(), GetMyOwner()->GetPlayerInformation()));
		}

	Proc_End:

		if (!(hr))
			CloseTransaction(hr);

		return hr;
	}




	PlayerTransGameRevealPlayer::PlayerTransGameRevealPlayer(IHeap& heap, MessageDataPtr &pIMsg)
		: MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg))
		, m_RevealedPlayerID(heap)
		, m_RevealedPlayerRole(heap)
	{
		BR_TRANS_MESSAGE(conspiracy::QueryUpdateTickStatusCmd, { return OnUpdatePlayerRes(pRes); });
		BR_TRANS_MESSAGE(Message::GameInstance::GameRevealPlayerRes, { return OnGameRevealPlayerRes(pRes); });
	}

	Result PlayerTransGameRevealPlayer::OnUpdatePlayerRes(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		MessageEndpoint* serverEndpoint{};
		GameInsUID insUID = GetMyOwner()->GetGameInsUID();

		//auto *pDBRes = (DB::QueryUpdateTickStatusCmd*)pRes;

		svrChkPtr(pRes);
		svrChkClose(pRes->GetResult());

		if (insUID.UID == 0)
		{
			svrErr(ResultCode::SVR_INVALID_ENTITYUID);
		}

		serverEndpoint = Service::MessageEndpointManager->GetEndpoint(insUID);

		svrChk(NetPolicyGameInstance(serverEndpoint).GameRevealPlayerCmd(
			RouteContext(GetOwnerEntityUID(), insUID), GetTransID(), GetMyOwner()->GetPlayerID(), GetTargetPlayerID()));

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
		svrChk(res.ParseMessage(*pMsgRes->GetMessage()));

		for (uint iPlayer = 0; iPlayer < res.GetRevealedPlayerID().size(); iPlayer++)
		{
			m_RevealedPlayerID.push_back(res.GetRevealedPlayerID()[iPlayer]);
		}
		for (uint iPlayer = 0; iPlayer < res.GetRevealedRole().size(); iPlayer++)
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

		if (GetMyOwner()->GetGameInsUID().UID == 0)
		{
			svrErrClose(ResultCode::INVALID_INSTANCEID);
		}

		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		svrChkCloseErr(ResultCode::GAME_INVALID_COSTID, conspiracy::OrganicTbl::FindItem((int)conspiracy::OrganicTbl::EItemEffect::Enum::Check, pCostItem));
		svrChkCloseErr(ResultCode::GAME_NOTENOUGH_RESOURCE, pPlayerInfoSystem->ApplyCost(pCostItem, TransLogCategory::Buy, "RevealPlayer"));


		svrChk(GetMyOwner()->UpdateDBSync(GetTransID()));

	Proc_End:

		if (!(hr))
			CloseTransaction(hr);

		return hr;
	}



	PlayerTransGamePlayerRevive::PlayerTransGamePlayerRevive(IHeap& heap, MessageDataPtr &pIMsg)
		:MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg))
	{
		BR_TRANS_MESSAGE(conspiracy::QueryUpdateTickStatusCmd, { return OnUpdatePlayerRes(pRes); });
		BR_TRANS_MESSAGE(Message::GameInstance::GamePlayerReviveRes, { return OnGamePlayerReviveRes(pRes); });
	}

	Result PlayerTransGamePlayerRevive::OnUpdatePlayerRes(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		MessageEndpoint* serverEndpoint{};

		GameInsUID insUID = GetMyOwner()->GetGameInsUID();

		//auto *pDBRes = (DB::QueryUpdateTickStatusCmd*)pRes;

		svrChkPtr(pRes);
		svrChkClose(pRes->GetResult());

		if (insUID.UID == 0)
		{
			svrErr(ResultCode::SVR_INVALID_ENTITYUID);
		}

		serverEndpoint = Service::MessageEndpointManager->GetEndpoint(insUID);

		svrChk(NetPolicyGameInstance(serverEndpoint).GamePlayerReviveCmd(
			RouteContext(GetOwnerEntityUID(), insUID), GetTransID(), GetMyOwner()->GetPlayerID()));

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
		svrChk(res.ParseMessage(*pMsgRes->GetMessage()));


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

		if (GetMyOwner()->GetGameInsUID().UID == 0)
		{
			svrErrClose(ResultCode::INVALID_INSTANCEID);
		}

		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		svrChkCloseErr(ResultCode::GAME_INVALID_COSTID, conspiracy::OrganicTbl::FindItem((int)conspiracy::OrganicTbl::EItemEffect::Enum::Revival, pCostItem));
		svrChkCloseErr(ResultCode::GAME_NOTENOUGH_RESOURCE, pPlayerInfoSystem->ApplyCost(pCostItem, TransLogCategory::Buy, "Revive"));

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
		//NetPolicyGameParty *pPolicy = nullptr;
		//Svr::ServerEntity *pServerEntity = nullptr;

		svrChk(super::StartTransaction());

		if (GetMyOwner()->GetGameInsUID() != GetRouteContext().GetFrom())
			svrErrClose(ResultCode::INVALID_INSTANCEID);

		NetSvrPolicyGame(GetRemoteEndpoint()).GamePlayerRevivedS2CEvt(GetRevivedPlayerID());


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

		if (GetMyOwner()->GetGameInsUID().UID != 0)
		{
			svrErrClose(ResultCode::SVR_INVALID_PLAYER_STATE);
		}

		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());


		svrChkCloseErr(ResultCode::GAME_INVALID_COSTID, conspiracy::OrganicTbl::FindItem((int)conspiracy::OrganicTbl::EItemEffect::Enum::ResetRankNormal, pCostItem));
		svrChkCloseErr(ResultCode::GAME_NOTENOUGH_RESOURCE, pPlayerInfoSystem->ApplyCost(pCostItem, TransLogCategory::Buy, "ResetRank"));

		svrChkClose(pPlayerInfoSystem->ResetRankNormal(pCostItem));

		m_TotalGem = pPlayerInfoSystem->GetGem();
		m_TotalGameMoney = pPlayerInfoSystem->GetGameMoney();

		svrChk(GetMyOwner()->UpdateDBSync());

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


}// namespace GameServer 
}// namespace SF 

