////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game player Entity
//
////////////////////////////////////////////////////////////////////////////////



#include "GameServerPCH.h"
#include "SvrConst.h"
#include "SvrTrace.h"
#include "Server/BrServerUtil.h"
#include "Task/ServerTaskEvent.h"
#include "Transaction/ExternalTransactionManager.h"
#include "Net/SFNetServerUDP.h"
#include "Util/SFTimeUtil.h"
#include "Types/SFEngineTypedefs.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"


#include "GameSvrConst.h"
#include "GameServerClass.h"


#include "ServiceEntity/Game/GameServiceTrans.h"
#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GameNetPolicy.h"
#include "Protocol/Message/GameMsgClass.h"

#include "Transaction/GamePlayerEntityTrans.h"
#include "Transaction/GamePlayerEntityTransGame.h"
#include "Transaction/GamePlayerEntityTransParty.h"
#include "Transaction/GamePlayerEntityTransFriend.h"
#include "Transaction/GamePlayerEntityTransClose.h"

#include "GamePlayerEntity.h"
#include "GameSystem/UserFriendSystem.h"
#include "GameSystem/UserNotifySystem.h"
#include "GameSystem/UserGamePlayerInfoSystem.h"

#include "DB/GameConspiracyDB.h"
#include "DB/GameTransactionDB.h"






namespace SF {
namespace GameServer {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	GamePlayerEntity::GamePlayerEntity()
		: super()
		, m_ComponentCarrier(GetHeap())
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Game instance server to player

		RegisterMessageHandler<PlayerTransJoinedS2SEvt>();
		RegisterMessageHandler<PlayerTransLeftS2SEvt>();

		RegisterMessageHandler<PlayerTransPlayerKickedS2SEvt>();
		RegisterMessageHandler<PlayerTransRoleAssignedS2SEvt>();
		RegisterMessageHandler<PlayerTransGameAdvanceVotedS2SEvt>();
		RegisterMessageHandler<PlayerTransVoteEndS2SEvt>();
		RegisterMessageHandler<PlayerTransPlayerRevealedS2SEvt>();
		RegisterMessageHandler<PlayerTransGameAdvancedS2SEvt>();
		RegisterMessageHandler<PlayerTransGameEndedS2SEvt>();
		RegisterMessageHandler<PlayerTransPlayerKilledS2SEvt>();
		RegisterMessageHandler<PlayerTransPlayerVotedS2SEvt>();
		RegisterMessageHandler<PlayerTransPlayAgainS2SEvt>();
		RegisterMessageHandler<PlayerTransGamePlayerRevivedS2SEvt>();

		//BR_ENTITY_MESSAGE(Message::GameInstance::PlayerJoinedS2CEvt)								{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransJoinedS2SEvt(GetHeap(),  pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::GameInstance::PlayerLeftS2CEvt)									{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransLeftS2SEvt(GetHeap(),  pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::GameInstance::PlayerKickedS2CEvt)								{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransPlayerKickedS2SEvt(GetHeap(),  pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::GameInstance::RoleAssignedS2CEvt)								{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransRoleAssignedS2SEvt(GetHeap(),  pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::GameInstance::GameAdvanceVotedS2CEvt)							{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransGameAdvanceVotedS2SEvt(GetHeap(),  pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::GameInstance::VoteEndS2CEvt)										{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransVoteEndS2SEvt(GetHeap(),  pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::GameInstance::PlayerRevealedS2CEvt)								{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransPlayerRevealedS2SEvt(GetHeap(),  pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::GameInstance::GameAdvancedS2CEvt)								{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransGameAdvancedS2SEvt(GetHeap(),  pMsgData)); return ResultCode::SUCCESS; });
		//BR_ENTITY_MESSAGE(Message::GameInstance::GameEndedS2CEvt)									{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransGameEndedS2SEvt(GetHeap(),  pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::GameInstance::PlayerKilledS2CEvt)								{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransPlayerKilledS2SEvt(GetHeap(),  pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::GameInstance::VotedS2CEvt)										{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransPlayerVotedS2SEvt(GetHeap(),  pMsgData)); 	return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::GameInstance::GamePlayAgainS2CEvt)								{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransPlayAgainS2SEvt(GetHeap(),  pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::GameInstance::GamePlayerRevivedS2CEvt)							{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransGamePlayerRevivedS2SEvt(GetHeap(),  pMsgData)); return ResultCode::SUCCESS; } );

		//////////////////////////////////////////////////////////////////////////////////////////////////
		// PartyServer to player 

		//RegisterMessageHandler<PlayerTransPartyQueuedGameMatchingS2CEvt>();
		//RegisterMessageHandler<PlayerTransPartyCanceledGameMatchingS2CEvt>();
		//RegisterMessageHandler<PlayerTransPartyMatchingItemDequeuedS2CEvt>();

		//BR_ENTITY_MESSAGE(Message::GameParty::PlayerJoinedS2CEvt)									{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransPartyPlayerJoinedS2SEvt(GetHeap(),  pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::GameParty::PartyLeaderChangedS2CEvt)								{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransPartyLeaderChangedS2SEvt(GetHeap(),  pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::GameParty::PlayerLeftS2CEvt)										{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransPartyPlayerLeftS2SEvt(GetHeap(),  pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::GameParty::PlayerKickedS2CEvt)									{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransPartyPlayerKickedS2SEvt(GetHeap(),  pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::GameParty::ChatMessageS2CEvt)									{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransPartyChatMessageS2CEvt(GetHeap(),  pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::GameParty::QuickChatMessageS2CEvt)								{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransPartyQuickChatMessageS2CEvt(GetHeap(),  pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::GameParty::QueuedGameMatchingS2CEvt)								{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransPartyQueuedGameMatchingS2CEvt(GetHeap(),  pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::GameParty::CanceledGameMatchingS2CEvt)							{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransPartyCanceledGameMatchingS2CEvt(GetHeap(),  pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::GameParty::MatchingItemDequeuedS2CEvt)							{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransPartyMatchingItemDequeuedS2CEvt(GetHeap(),  pMsgData)); return ResultCode::SUCCESS; } );


		////////////////////////////////////////////////////////////////////////////////////////////////////
		// GameMatching to player

		//BR_ENTITY_MESSAGE(Message::PartyMatching::PlayerGameMatchedS2CEvt)							{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransGameMatchedS2SEvt(GetHeap(),  pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::PlayerMatchingCanceledS2CEvt)				{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransPlayerMatchingCanceledS2CEvt(GetHeap(),  pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt)			{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransPlayerMatchingItemDequeuedS2CEvt(GetHeap(),  pMsgData)); return ResultCode::SUCCESS; } );
	}

	GamePlayerEntity::~GamePlayerEntity()
	{
	}



	// Initialize entity to proceed new connection
	Result GamePlayerEntity::InitializeEntity(EntityID newEntityID)
	{
		Result hr = ResultCode::SUCCESS;
		auto pGameConfig = GetMyServer()->GetPresetGameConfig();
		auto PlayerAutoLogout = pGameConfig->PlayerAutoLogout;

		svrChk(super::InitializeEntity(newEntityID));

		GetTimeToKill().SetTimer(DurationMS(PlayerAutoLogout * 1000));

		svrChk(GetComponentCarrier().AddComponent<UserFriendSystem>(this));
		svrChk(GetComponentCarrier().AddComponent<UserGamePlayerInfoSystem>(this));
		svrChk(GetComponentCarrier().AddComponent<UserNotifySystem>(this));

		svrChk(GetComponentCarrier().InitializeComponents());

	Proc_End:


		return hr;
	}


	void GamePlayerEntity::SetLatestActiveTime(UTCTimeStampSec latestActiveTime)
	{
		auto pGameConfig = GetMyServer()->GetPresetGameConfig();
		if (pGameConfig != nullptr)
		{
			auto PlayerAutoLogout = pGameConfig->PlayerAutoLogout;
			GetTimeToKill().SetTimer(DurationMS(PlayerAutoLogout * 1000));
		}

		super::SetLatestActiveTime(latestActiveTime);
	}

	Result GamePlayerEntity::UpdateDBSync(TransactionID transID)
	{
		Result hr = ResultCode::SUCCESS;

		if (!(UpdateGamePlayer()))
			return hr;

		SetLatestDBSyncTime(Util::Time.GetTimeUTCSec());
		auto pPlayerInfoSystem = GetComponent<UserGamePlayerInfoSystem>();

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->UpdateTickStatusCmd(transID, GetShardID(), GetPlayerID(),
			pPlayerInfoSystem->GetGem(), pPlayerInfoSystem->GetStamina(), GetIsInGame() ? 1 : 0,
			GetLatestActiveTime(),
			GetLatestUpdateTime()));

	Proc_End:

		return hr;
	}

	// Update game configuration
	Result GamePlayerEntity::UpdateGameConfig()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(GetComponent<UserGamePlayerInfoSystem>()->UpdateStatMaximum());

	Proc_End:

		return hr;
	}


	// Update Game Player 
	Result GamePlayerEntity::UpdateGamePlayer()
	{
		Result hr = ResultCode::SUCCESS;

		// m_LatestUpdateTime will be initialized when character data is loaded
		if (GetLatestUpdateTime() == UTCTimeStampSec::min())
		{
			return ResultCode::FAIL;
		}

		auto playerInfoSystem = GetComponent<UserGamePlayerInfoSystem>();

		auto tickTime = GetMyServer()->GetPresetGameConfig()->StaminaRecoveryTime;
		auto curUTCSec = Util::Time.GetTimeUTCSec();
		DurationSec timeDiff = curUTCSec - GetLatestUpdateTime();
		auto numberOfTicks = timeDiff.count() / tickTime;
		DurationSec remainTime = timeDiff % tickTime;

		svrChkPtr(GetMyServer()->GetPresetGameConfig());

		svrTrace(SVR_DBGTRANS, "Check GamePlayer Update Tick PID:{0} last:{1}, curTime:{2}, numTick:{3}, remain:{4}", GetPlayerID(), GetLatestUpdateTime(), curUTCSec, numberOfTicks, remainTime);

		if (numberOfTicks > 0)
		{
			svrTrace(SVR_DBGTRANS, "GamePlayer Update PID:{0}, numTick:{1}, Sta:{2}", GetPlayerID(), numberOfTicks, playerInfoSystem->GetStamina());

			if (numberOfTicks > std::numeric_limits<uint16_t>::max())
			{
				svrTrace(Error, "Invalid tick count, applying maximum int16");
				numberOfTicks = std::numeric_limits<uint16_t>::max();
			}

			if (playerInfoSystem->GetStamina() < playerInfoSystem->GetMaxAutoRefillStamina())
			{
				// limit stamina gain to MaxAutoRefillStamina
				auto maxCanGain = std::min((INT)(numberOfTicks * 1), (playerInfoSystem->GetMaxAutoRefillStamina() - playerInfoSystem->GetStamina()));
				playerInfoSystem->GainStamina(maxCanGain);
			}

			curUTCSec -= remainTime; // Subtract the remain from current time so that it can add up the the next tick calculation
			SetLatestUpdateTime(curUTCSec);
		}


	Proc_End:

		return hr;
	}


	const PlayerInformation& GamePlayerEntity::GetPlayerInformation() const
	{
		const_cast<GamePlayerEntity*>(this)->m_PlayerInformation.Level = GetComponent<UserGamePlayerInfoSystem>()->GetLevel();
		const_cast<GamePlayerEntity*>(this)->m_PlayerInformation.WeeklyWin = GetComponent<UserGamePlayerInfoSystem>()->GetWeeklyWin();
		const_cast<GamePlayerEntity*>(this)->m_PlayerInformation.WeeklyLose = GetComponent<UserGamePlayerInfoSystem>()->GetWeeklyLose();

		return m_PlayerInformation;
	}


	const ServerFriendInformation& GamePlayerEntity::GetFriendInformation() const
	{
		m_PlayerInformation.ShardID = GetShardID();
		m_PlayerInformation.Level = GetComponent<UserGamePlayerInfoSystem>()->GetLevel();
		m_PlayerInformation.WeeklyWin = GetComponent<UserGamePlayerInfoSystem>()->GetWeeklyWin();
		m_PlayerInformation.WeeklyLose = GetComponent<UserGamePlayerInfoSystem>()->GetWeeklyLose();
		m_PlayerInformation.LastActiveTime = GetLatestActiveTime().time_since_epoch().count();
		m_PlayerInformation.IsPlayingGame = GetIsInGame();

		return m_PlayerInformation;
	}

	void GamePlayerEntity::PendingCloseTransaction(const char* reason)
	{
		Result hr;
		TransactionPtr trans;

		unused(reason);

		if (m_ClosingPended) return;
		m_ClosingPended = true;

		svrMem(trans = new(GetHeap()) PlayerTransCloseInstance(GetHeap()));
		svrChk(trans->InitializeTransaction(this));
		PendingTransaction(GetTaskWorker()->GetThreadID(), trans);

	Proc_End:

		return;
	}

	// register message handlers
	Result GamePlayerEntity::RegisterMessageHandlers()
	{
		FunctionContext hr;

		svrCheck(super::RegisterMessageHandlers());

		//BR_ENTITY_MESSAGE(Message::Game::HeartBitC2SEvt) { pNewTrans = nullptr; return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::JoinGameServerCmd) { pNewTrans = new(GetHeap()) PlayerTransJoinGameServer(GetHeap(), pMsgData); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::Game::GetUserGamePlayerInfoCmd) { pNewTrans = new(GetHeap()) PlayerTransGetUserGamePlayerInfo(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::GetGamePlayerInfoCmd) { pNewTrans = new(GetHeap()) PlayerTransGetGamePlayerInfo(GetHeap(), pMsgData); return OnNewUserTranscation(); } );

		//BR_ENTITY_MESSAGE(Message::Game::GetComplitionStateCmd) { pNewTrans = new(GetHeap()) PlayerTransGetComplitionState(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::SetComplitionStateCmd) { pNewTrans = new(GetHeap()) PlayerTransSetComplitionState(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::RegisterGCMCmd) { pNewTrans = new(GetHeap()) PlayerTransRegisterGCM(GetHeap(), pMsgData); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::Game::UnregisterGCMCmd) { pNewTrans = new(GetHeap()) PlayerTransUnregisterGCM(GetHeap(), pMsgData); return ResultCode::SUCCESS; } );

		//BR_ENTITY_MESSAGE(Message::Game::InviteFriendCmd) { pNewTrans = new(GetHeap()) PlayerTransInviteFriend(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::AcceptFriendRequestCmd) { pNewTrans = new(GetHeap()) PlayerTransFriendAccept(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::RemoveFriendCmd) { pNewTrans = new(GetHeap()) PlayerTransRemoveFriend(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::GetFriendListCmd) { pNewTrans = new(GetHeap()) PlayerTransGetFriendList(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::GiveStaminaCmd) { pNewTrans = new(GetHeap()) PlayerTransGiveStamina(GetHeap(), pMsgData); return OnNewUserTranscation(); } );

		//BR_ENTITY_MESSAGE(Message::Game::GetNotificationListCmd) { pNewTrans = new(GetHeap()) PlayerTransGetNotificationList(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::DeleteNotificationCmd) { pNewTrans = new(GetHeap()) PlayerTransDeleteNotification(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::AcceptNotificationCmd) { pNewTrans = new(GetHeap()) PlayerTransAcceptNotification(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::SetNotificationReadCmd) { pNewTrans = new(GetHeap()) PlayerTransSetNotificationRead(GetHeap(), pMsgData); return OnNewUserTranscation(); } );

		//BR_ENTITY_MESSAGE(Message::Game::SetNickNameCmd) { pNewTrans = new(GetHeap()) PlayerTransSetNickName(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::FindPlayerByEMailCmd) { pNewTrans = new(GetHeap()) PlayerTransFindPlayerByEMail(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::FindPlayerByPlayerIDCmd) { pNewTrans = new(GetHeap()) PlayerTransFindPlayerByPlayerID(GetHeap(), pMsgData); return OnNewUserTranscation(); } );

		//BR_ENTITY_MESSAGE(Message::Game::RequestPlayerStatusUpdateCmd) { pNewTrans = new(GetHeap()) PlayerTransRequestPlayerStatusUpdate(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::GetRankingListCmd) { pNewTrans = new(GetHeap()) PlayerTransGetRankingList(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::BuyShopItemPrepareCmd) { pNewTrans = new(GetHeap()) PlayerTransBuyShopItemPrepare(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::BuyShopItemCmd) { pNewTrans = new(GetHeap()) PlayerTransBuyShopItem(GetHeap(), pMsgData); return OnNewUserTranscation(); } );

		BR_ENTITY_MESSAGE(Message::Game::JoinGameCmd) { pNewTrans = new(GetHeap()) PlayerTransJoinGame(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::KickPlayerCmd) { pNewTrans = new(GetHeap()) PlayerTransKickPlayer(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::LeaveGameCmd) { pNewTrans = new(GetHeap()) PlayerTransLeaveGame(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::AssignRoleCmd) { pNewTrans = new(GetHeap()) PlayerTransAssignRole(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::ChatMessageCmd) { pNewTrans = new(GetHeap()) PlayerTransChatMessage(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::VoteGameAdvanceCmd) { pNewTrans = new(GetHeap()) PlayerTransVoteGameAdvance(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::VoteCmd) { pNewTrans = new(GetHeap()) PlayerTransVote(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::AdvanceGameCmd) { pNewTrans = new(GetHeap()) PlayerTransAdvanceGame(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::GamePlayAgainCmd) { pNewTrans = new(GetHeap()) PlayerTransPlayAgain(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::GameRevealPlayerCmd) { pNewTrans = new(GetHeap()) PlayerTransGameRevealPlayer(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::GamePlayerReviveCmd) { pNewTrans = new(GetHeap()) PlayerTransGamePlayerRevive(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::GamePlayerResetRankCmd) { pNewTrans = new(GetHeap()) PlayerTransGamePlayerResetRank(GetHeap(), pMsgData); return OnNewUserTranscation(); } );

		//BR_ENTITY_MESSAGE(Message::Game::CreatePartyCmd) { pNewTrans = new(GetHeap()) PlayerTransCreateParty(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::JoinPartyCmd) { pNewTrans = new(GetHeap()) PlayerTransJoinParty(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::LeavePartyCmd) { pNewTrans = new(GetHeap()) PlayerTransLeaveParty(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::PartyKickPlayerCmd) { pNewTrans = new(GetHeap()) PlayerTransPartyKickPlayer(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::PartyInviteCmd) { pNewTrans = new(GetHeap()) PlayerTransPartyInvite(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::PartyChatMessageCmd) { pNewTrans = new(GetHeap()) PlayerTransPartyChatMessage(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::PartyQuickChatMessageCmd) { pNewTrans = new(GetHeap()) PlayerTransPartyQuickChatMessage(GetHeap(), pMsgData); return OnNewUserTranscation(); } );

		//BR_ENTITY_MESSAGE(Message::Game::RequestGameMatchCmd) { pNewTrans = new(GetHeap()) PlayerTransRequestGameMatch(GetHeap(), pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::CancelGameMatchCmd) { pNewTrans = new(GetHeap()) PlayerTransCancelGameMatch(GetHeap(), pMsgData); return OnNewUserTranscation(); } );

		BR_ENTITY_MESSAGE(Message::Game::SetPresetGameConfigIDCmd) { pNewTrans = new(GetHeap()) PlayerTransSetConfigPreset(GetHeap(), pMsgData); return OnNewUserTranscation(); } );

		return ResultCode::SUCCESS;
	}


}; // namespace GameServer
}; // namespace SF


