////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game player Entity
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "SvrConst.h"
#include "SvrTrace.h"
#include "Server/BrServerUtil.h"
#include "Task/ServerTaskEvent.h"
#include "ServerSystem/ExternalTransactionManager.h"
#include "Net/NetServerUDP.h"
#include "Util/TimeUtil.h"
#include "Types/SFEngineTypedefs.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"


#include "GameSvrConst.h"
#include "GameServerClass.h"


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
#include "Transaction/GameServerEntityTrans.h"

#include "DB/GameConspiracyDB.h"
#include "DB/GameTransactionDB.h"


SF_MEMORYPOOL_IMPLEMENT(BR::BR::GameServer::GamePlayerEntity);




namespace SF {
namespace GameServer {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	GamePlayerEntity::GamePlayerEntity()
		: m_ISvrGamePolicy(nullptr)
		, m_PlayerState(PlayerState_None)
		, m_GameInsUID(0)
		, m_LatestUpdateTime(TimeStampSec(DurationSec(0)))
		, m_LatestActiveTime(TimeStampSec(DurationSec(0)))
		, m_LatestDBSyncTime(TimeStampSec(DurationSec(0)))
		, m_Allocator(STDAllocator::GetInstance())
	{
		memset( m_UserName, 0, sizeof(m_UserName) );
		memset( m_GCMKeys, 0, sizeof(m_GCMKeys) );

		BR_ENTITY_MESSAGE(Message::GameServer::RegisterPlayerToJoinGameServerCmd) { svrMemReturn(pNewTrans = new GameServerTransRegisterPlayerToJoinGameServer<GamePlayerEntity>(pMsgData)); return ResultCode::SUCCESS; } );

		BR_ENTITY_MESSAGE(Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd) { svrMemReturn(pNewTrans = new PlayerTransRegisterPlayerToJoinGameServerOnPlayerEntity(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameServer::ChatMessageC2SEvt) { svrMemReturn(pNewTrans = new PlayerTransChatMessageFromOtherEntity(pMsgData)); return ResultCode::SUCCESS; } );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Game Player

		BR_ENTITY_MESSAGE(Message::GameServer::FriendAcceptedC2SEvt) { svrMemReturn(pNewTrans = new PlayerTransFriendAcceptedS2S(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameServer::FriendRemovedC2SEvt) { svrMemReturn(pNewTrans = new PlayerTransFriendRemovedS2S(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameServer::NotifyC2SEvt) { svrMemReturn(pNewTrans = new PlayerTransNotifyS2S(pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::GameServer::GiveStaminaC2SEvt)					{ svrMemReturn( pNewTrans = new PlayerTransGiveStaminaS2S(pMsgData) ); return ResultCode::SUCCESS; } );

		BR_ENTITY_MESSAGE(Message::GameServer::RequestPlayerStatusUpdateC2SEvt) { svrMemReturn(pNewTrans = new PlayerTransRequestPlayerStatusUpdateS2S(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameServer::NotifyPlayerStatusUpdatedC2SEvt) { svrMemReturn(pNewTrans = new PlayerTransNotifyPlayerStatusUpdatedS2S(pMsgData)); return ResultCode::SUCCESS; } );

		BR_ENTITY_MESSAGE(Message::GameServer::NotifyPartyInviteC2SEvt) { svrMemReturn(pNewTrans = new PlayerTransNotifyPartyInviteS2SEvt(pMsgData)); return ResultCode::SUCCESS; } );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Game instance server to player

		BR_ENTITY_MESSAGE(Message::GameInstance::PlayerJoinedS2CEvt) { svrMemReturn(pNewTrans = new PlayerTransJoinedS2SEvt(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::PlayerLeftS2CEvt) { svrMemReturn(pNewTrans = new PlayerTransLeftS2SEvt(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::PlayerKickedS2CEvt) { svrMemReturn(pNewTrans = new PlayerTransPlayerKickedS2SEvt(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::RoleAssignedS2CEvt) { svrMemReturn(pNewTrans = new PlayerTransRoleAssignedS2SEvt(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::GameAdvanceVotedS2CEvt) { svrMemReturn(pNewTrans = new PlayerTransGameAdvanceVotedS2SEvt(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::VoteEndS2CEvt) { svrMemReturn(pNewTrans = new PlayerTransVoteEndS2SEvt(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::PlayerRevealedS2CEvt) { svrMemReturn(pNewTrans = new PlayerTransPlayerRevealedS2SEvt(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::GameAdvancedS2CEvt) { svrMemReturn(pNewTrans = new PlayerTransGameAdvancedS2SEvt(pMsgData)); return ResultCode::SUCCESS; });
		BR_ENTITY_MESSAGE(Message::GameInstance::GameEndedS2CEvt) { svrMemReturn(pNewTrans = new PlayerTransGameEndedS2SEvt(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::PlayerKilledS2CEvt) { svrMemReturn(pNewTrans = new PlayerTransPlayerKilledS2SEvt(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::VotedS2CEvt) { svrMemReturn(pNewTrans = new PlayerTransPlayerVotedS2SEvt(pMsgData)); 	return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::GamePlayAgainS2CEvt) { svrMemReturn(pNewTrans = new PlayerTransPlayAgainS2SEvt(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::GamePlayerRevivedS2CEvt) { svrMemReturn(pNewTrans = new PlayerTransGamePlayerRevivedS2SEvt(pMsgData)); return ResultCode::SUCCESS; } );

		//////////////////////////////////////////////////////////////////////////////////////////////////
		// PartyServer to player 
		BR_ENTITY_MESSAGE(Message::GameParty::PlayerJoinedS2CEvt) { svrMemReturn(pNewTrans = new PlayerTransPartyPlayerJoinedS2SEvt(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameParty::PartyLeaderChangedS2CEvt) { svrMemReturn(pNewTrans = new PlayerTransPartyLeaderChangedS2SEvt(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameParty::PlayerLeftS2CEvt) { svrMemReturn(pNewTrans = new PlayerTransPartyPlayerLeftS2SEvt(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameParty::PlayerKickedS2CEvt) { svrMemReturn(pNewTrans = new PlayerTransPartyPlayerKickedS2SEvt(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameParty::ChatMessageS2CEvt) { svrMemReturn(pNewTrans = new PlayerTransPartyChatMessageS2CEvt(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameParty::QuickChatMessageS2CEvt) { svrMemReturn(pNewTrans = new PlayerTransPartyQuickChatMessageS2CEvt(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameParty::QueuedGameMatchingS2CEvt) { svrMemReturn(pNewTrans = new PlayerTransPartyQueuedGameMatchingS2CEvt(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameParty::CanceledGameMatchingS2CEvt) { svrMemReturn(pNewTrans = new PlayerTransPartyCanceledGameMatchingS2CEvt(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameParty::MatchingItemDequeuedS2CEvt) { svrMemReturn(pNewTrans = new PlayerTransPartyMatchingItemDequeuedS2CEvt(pMsgData)); return ResultCode::SUCCESS; } );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// GameMatching to player
		BR_ENTITY_MESSAGE(Message::PartyMatching::PlayerGameMatchedS2CEvt) { svrMemReturn(pNewTrans = new PlayerTransGameMatchedS2SEvt(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::PlayerMatchingCanceledS2CEvt) { svrMemReturn(pNewTrans = new PlayerTransPlayerMatchingCanceledS2CEvt(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt) { svrMemReturn(pNewTrans = new PlayerTransPlayerMatchingItemDequeuedS2CEvt(pMsgData)); return ResultCode::SUCCESS; } );
	}

	GamePlayerEntity::~GamePlayerEntity()
	{
	}

	// Initialize entity to proceed new connection
	Result GamePlayerEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;
		auto pGameConfig = GetMyServer()->GetPresetGameConfig();
		auto PlayerAutoLogout = pGameConfig->PlayerAutoLogout;

		svrChk(Svr::SimpleUserEntity::InitializeEntity( newEntityID ) );

		m_GameInsUID = 0;
		m_PartyUID = 0;
		m_ShardID = 0;
		m_MatchingTicket = 0;
		m_MatchingStartTime = TimeStampMS(DurationMS(0));

		memset( &m_PlayerInformation, 0, sizeof(m_PlayerInformation) );

		m_UserName[0] = '\0';
		m_GCMKeys[0] = '\0';
		
		SetLatestActiveTime(Util::Time.GetTimeUTCSec());
		m_LatestUpdateTime = TimeStampSec(DurationSec(0));

		m_TimeToKill.SetTimer(DurationMS(PlayerAutoLogout * 1000));

		svrChk( AddComponent<UserFriendSystem>(this) );
		svrChk( AddComponent<UserGamePlayerInfoSystem>(this) );
		svrChk( AddComponent<UserNotifySystem>(this) );

		svrChk( InitializeComponents() );

	Proc_End:


		return hr;
	}


	// Set connection for pilot
	Result GamePlayerEntity::SetConnection(SharedPointerT<Net::Connection>&& pCon)
	{
		Result hr = ResultCode::SUCCESS;

		if (pCon == GetConnection())
			return hr;

		if( GetConnection() != nullptr )
		{
			ReleaseConnection();
		}

		svrChk(Svr::SimpleUserEntity::SetConnection(std::forward<SharedPointerT<Net::Connection>>(pCon)));

		if (GetConnection() != nullptr)
		{
			svrChkPtr(m_ISvrGamePolicy = GetConnection()->GetInterface<Policy::NetSvrPolicyGame>());
		}

	Proc_End:

		return hr;
	}

	// Release connection if has
	void GamePlayerEntity::ReleaseConnection()
	{
		m_ISvrGamePolicy = nullptr;

		Svr::SimpleUserEntity::ReleaseConnection();
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity process
	//

	void GamePlayerEntity::SetMatchingTicket(MatchingQueueTicket ticket)
	{
		if (ticket != 0)
		{
			m_MatchingStartTime = Util::Time.GetTimeMs();
		}
		m_MatchingTicket = ticket;
	}

	void GamePlayerEntity::SetLatestActiveTime(TimeStampSec latestActiveTime)
	{
		auto pGameConfig = GetMyServer()->GetPresetGameConfig();
		if (pGameConfig != nullptr)
		{
			auto PlayerAutoLogout = pGameConfig->PlayerAutoLogout;
			m_TimeToKill.SetTimer(DurationMS(PlayerAutoLogout * 1000));
		}

		m_LatestActiveTime = latestActiveTime;
	}

	Result GamePlayerEntity::OnJoinGameServerInitialize(AuthTicket authTicket, FacebookUID fbUID)
	{
		Result hr = ResultCode::SUCCESS;
		Net::ConnectionPtr pConnection;
		Net::Connection::ConnectionInformation connectionInfo;

		svrChkPtr(GetMyServer()->GetNetPublic());

		ReleaseConnection();

		SetAuthTicket(authTicket);
		SetFacebookUID(fbUID);

		svrChkPtr(pConnection = GetMyServer()->GetNetPublic()->GetConnectionManager().NewConnection());

		memset(&connectionInfo, 0, sizeof(connectionInfo));
		connectionInfo.SetLocalInfo(GetMyServer()->GetNetClass(), GetMyServer()->GetNetPublic()->GetLocalAddress(), GetMyServer()->GetServerUID());
		connectionInfo.SetRemoteInfo(NetClass::Client, authTicket);

		svrChk(pConnection->InitConnection(GetMyServer()->GetNetPublic()->GetSocket(), connectionInfo));
		svrTrace(Trace::TRC_USER1, "Initialize connection CID:{0}, Addr:{1}", pConnection->GetCID(), pConnection->GetConnectionInfo().Remote);

		svrChk(GetMyServer()->GetNetPublic()->GetConnectionManager().PendingConnection(pConnection));

		svrChk(SetConnection(std::forward<Net::ConnectionPtr>(pConnection)));

		SetLatestActiveTime(Util::Time.GetTimeUTCSec());

		AddGameTransactionLog(TransLogCategory::Account, 2, 0, 0, "Entity Initialize");

	Proc_End:

		return hr;
	}

	Result GamePlayerEntity::OnNewUserTranscation()
	{
		// m_LatestUpdateTime is used as a valid character data signal
		if (m_LatestUpdateTime == TimeStampSec::min())
			return ResultCode::SUCCESS_FALSE;

		SetLatestActiveTime(Util::Time.GetTimeUTCSec());

		if (m_LatestDBSyncTime == TimeStampSec::min() || Util::TimeSinceUTC(m_LatestDBSyncTime) > DurationSec(GameConst::PLAYER_UPDATE_STATUS_TIME))
		{
			UpdateDBSync();
		}

		return ResultCode::SUCCESS;
	}

	Result GamePlayerEntity::UpdateDBSync(TransactionID transID)
	{
		Result hr = ResultCode::SUCCESS;

		if (!(UpdateGamePlayer()))
			return hr;

		m_LatestDBSyncTime = Util::Time.GetTimeUTCSec();
		auto pPlayerInfoSystem = GetComponent<UserGamePlayerInfoSystem>();

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->UpdateTickStatusCmd(transID, GetShardID(), GetPlayerID(),
			pPlayerInfoSystem->GetGem(), pPlayerInfoSystem->GetStamina(), GetIsInGame() ? 1 : 0,
			GetLatestActiveTime(),
			GetLatestUpdateTime()));

	Proc_End:

		return hr;
	}

	// register message handlers
	Result GamePlayerEntity::RegisterMessageHandlers()
	{
		
		BR_ENTITY_MESSAGE(Message::Game::HeartBitC2SEvt)				{ pNewTrans = nullptr; return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::JoinGameServerCmd)				{ pNewTrans = new PlayerTransJoinGameServer(pMsgData); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::Game::GetUserGamePlayerInfoCmd)		{ pNewTrans = new PlayerTransGetUserGamePlayerInfo(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::GetGamePlayerInfoCmd)			{ pNewTrans = new PlayerTransGetGamePlayerInfo(pMsgData); return OnNewUserTranscation(); } );

		BR_ENTITY_MESSAGE(Message::Game::GetComplitionStateCmd)			{ pNewTrans = new PlayerTransGetComplitionState(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::SetComplitionStateCmd)			{ pNewTrans = new PlayerTransSetComplitionState(pMsgData); return OnNewUserTranscation(); } );

		BR_ENTITY_MESSAGE(Message::Game::RegisterGCMCmd)				{ pNewTrans = new PlayerTransRegisterGCM(pMsgData); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::Game::UnregisterGCMCmd)				{ pNewTrans = new PlayerTransUnregisterGCM(pMsgData); return ResultCode::SUCCESS; } );

		BR_ENTITY_MESSAGE(Message::Game::InviteFriendCmd)				{ pNewTrans = new PlayerTransInviteFriend(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::AcceptFriendRequestCmd)		{ pNewTrans = new PlayerTransFriendAccept(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::RemoveFriendCmd)				{ pNewTrans = new PlayerTransRemoveFriend(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::GetFriendListCmd)				{ pNewTrans = new PlayerTransGetFriendList(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::GiveStaminaCmd)				{ pNewTrans = new PlayerTransGiveStamina(pMsgData); return OnNewUserTranscation(); } );

		BR_ENTITY_MESSAGE(Message::Game::GetNotificationListCmd)		{ pNewTrans = new PlayerTransGetNotificationList(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::DeleteNotificationCmd)			{ pNewTrans = new PlayerTransDeleteNotification(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::AcceptNotificationCmd)			{ pNewTrans = new PlayerTransAcceptNotification(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::SetNotificationReadCmd)		{ pNewTrans = new PlayerTransSetNotificationRead(pMsgData); return OnNewUserTranscation(); } );

		BR_ENTITY_MESSAGE(Message::Game::SetNickNameCmd)				{ pNewTrans = new PlayerTransSetNickName(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::FindPlayerByEMailCmd)			{ pNewTrans = new PlayerTransFindPlayerByEMail(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::FindPlayerByPlayerIDCmd)		{ pNewTrans = new PlayerTransFindPlayerByPlayerID(pMsgData); return OnNewUserTranscation(); } );

		BR_ENTITY_MESSAGE(Message::Game::RequestPlayerStatusUpdateCmd)	{ pNewTrans = new PlayerTransRequestPlayerStatusUpdate(pMsgData); return OnNewUserTranscation(); } );

		BR_ENTITY_MESSAGE(Message::Game::GetRankingListCmd)				{ pNewTrans = new PlayerTransGetRankingList(pMsgData); return OnNewUserTranscation(); } );

		BR_ENTITY_MESSAGE(Message::Game::BuyShopItemPrepareCmd)			{ pNewTrans = new PlayerTransBuyShopItemPrepare(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::BuyShopItemCmd)				{ pNewTrans = new PlayerTransBuyShopItem(pMsgData); return OnNewUserTranscation(); } );

		BR_ENTITY_MESSAGE(Message::Game::JoinGameCmd)					{ pNewTrans = new PlayerTransJoinGame(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::KickPlayerCmd)					{ pNewTrans = new PlayerTransKickPlayer(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::LeaveGameCmd)					{ pNewTrans = new PlayerTransLeaveGame(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::AssignRoleCmd)					{ pNewTrans = new PlayerTransAssignRole(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::ChatMessageCmd)				{ pNewTrans = new PlayerTransChatMessage(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::VoteGameAdvanceCmd)			{ pNewTrans = new PlayerTransVoteGameAdvance(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::VoteCmd)						{ pNewTrans = new PlayerTransVote(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::AdvanceGameCmd)				{ pNewTrans = new PlayerTransAdvanceGame(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::GamePlayAgainCmd)				{ pNewTrans = new PlayerTransPlayAgain(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::GameRevealPlayerCmd)			{ pNewTrans = new PlayerTransGameRevealPlayer(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::GamePlayerReviveCmd)			{ pNewTrans = new PlayerTransGamePlayerRevive(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::GamePlayerResetRankCmd)		{ pNewTrans = new PlayerTransGamePlayerResetRank(pMsgData); return OnNewUserTranscation(); } );

		BR_ENTITY_MESSAGE(Message::Game::CreatePartyCmd)				{ pNewTrans = new PlayerTransCreateParty(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::JoinPartyCmd)					{ pNewTrans = new PlayerTransJoinParty(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::LeavePartyCmd)					{ pNewTrans = new PlayerTransLeaveParty(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::PartyKickPlayerCmd)			{ pNewTrans = new PlayerTransPartyKickPlayer(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::PartyInviteCmd)				{ pNewTrans = new PlayerTransPartyInvite(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::PartyChatMessageCmd)			{ pNewTrans = new PlayerTransPartyChatMessage(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::PartyQuickChatMessageCmd)		{ pNewTrans = new PlayerTransPartyQuickChatMessage(pMsgData); return OnNewUserTranscation(); } );

		BR_ENTITY_MESSAGE(Message::Game::RequestGameMatchCmd)			{ pNewTrans = new PlayerTransRequestGameMatch(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::CancelGameMatchCmd)			{ pNewTrans = new PlayerTransCancelGameMatch(pMsgData); return OnNewUserTranscation(); } );

		BR_ENTITY_MESSAGE(Message::Game::SetPresetGameConfigIDCmd)		{ pNewTrans = new PlayerTransSetConfigPreset(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::GainGameResourceCmd)			{ pNewTrans = new PlayerTransGainGameResource(pMsgData); return OnNewUserTranscation(); } );

		return ResultCode::SUCCESS;
	}

	// clear transaction
	Result GamePlayerEntity::ClearEntity()
	{
		Result hr = ResultCode::SUCCESS;

		ReleaseConnection();

		svrChk(Svr::SimpleUserEntity::ClearEntity() );

	Proc_End:

		return hr;
	}

	// Run the task
	Result GamePlayerEntity::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;
		Svr::Transaction *trans = nullptr;

		hr = Svr::SimpleUserEntity::TickUpdate(pAction);
		if (hr == Result(ResultCode::SUCCESS_FALSE))
			return hr;

		//if (m_LatestDBSyncTime == 0 || Util::TimeSinceUTC(m_LatestDBSyncTime) > GameConst::PLAYER_UPDATE_STATUS_TIME)
		//{
		//	UpdateDBSync();
		//}


		if (m_TimeToKill.CheckTimer() 
			&& GetEntityState() == EntityState::WORKING)
		{
			svrMem(trans = new PlayerTransCloseInstance);
			svrChk(trans->InitializeTransaction(this));
			PendingTransaction(GetTaskWorker()->GetThreadID(), trans);
		}

	Proc_End:

		if (trans != nullptr)
		{
			SharedPointerT<Svr::Transaction>(trans);
		}

		return hr;
	}


	// Update game configuration
	Result GamePlayerEntity::UpdateGameConfig()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( GetComponent<UserGamePlayerInfoSystem>()->UpdateStatMaximum() );

	Proc_End:

		return hr;
	}


	// Update Game Player 
	Result GamePlayerEntity::UpdateGamePlayer()
	{
		Result hr = ResultCode::SUCCESS;

		// m_LatestUpdateTime will be initialized when character data is loaded
		if (m_LatestUpdateTime == TimeStampSec::min())
		{
			return ResultCode::FAIL;
		}

		auto playerInfoSystem = GetComponent<UserGamePlayerInfoSystem>();

		auto tickTime = GetMyServer()->GetPresetGameConfig()->StaminaRecoveryTime;
		TimeStampSec curUTCSec = Util::Time.GetTimeUTCSec();
		DurationSec timeDiff = curUTCSec - m_LatestUpdateTime;
		auto numberOfTicks = timeDiff.count() / tickTime;
		DurationSec remainTime = timeDiff % tickTime;

		svrChkPtr(GetMyServer()->GetPresetGameConfig());

		svrTrace(Svr::TRC_DBGTRANS, "Check GamePlayer Update Tick PID:{0} last:{1}, curTime:{2}, numTick:{3}, remain:{4}", GetPlayerID(), m_LatestUpdateTime, curUTCSec, numberOfTicks, remainTime);

		if( numberOfTicks > 0 )
		{
			svrTrace(Svr::TRC_DBGTRANS, "GamePlayer Update PID:{0}, numTick:{1}, Sta:{2}", GetPlayerID(), numberOfTicks, playerInfoSystem->GetStamina());

			if (numberOfTicks > std::numeric_limits<uint16_t>::max())
			{
				svrTrace( Trace::TRC_ERROR, "Invalid tick count, applying maximum int16" );
				numberOfTicks = std::numeric_limits<uint16_t>::max();
			}

			if (playerInfoSystem->GetStamina() < playerInfoSystem->GetMaxAutoRefillStamina())
			{
				// limit stamina gain to MaxAutoRefillStamina
				auto maxCanGain = std::min((INT)(numberOfTicks * 1), (playerInfoSystem->GetMaxAutoRefillStamina()- playerInfoSystem->GetStamina()));
				playerInfoSystem->GainStamina(maxCanGain);
			}

			curUTCSec -= remainTime; // Subtract the remain from current time so that it can add up the the next tick calculation
			SetLatestUpdateTime(curUTCSec);
		}


	Proc_End:

		return hr;
	}
	
	

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	
	//

	bool GamePlayerEntity::GetIsInGame() const
	{
		return GetGameInsUID() != 0 || GetPartyUID() != 0;
	}

	void GamePlayerEntity::SetGameInsUID( const GameInsUID& gameInsID )
	{
		m_GameInsUID = gameInsID;

		m_PlayerInformation.IsPlayingGame = m_GameInsUID != 0 ? TRUE : FALSE;
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

	void GamePlayerEntity::AddGameTransactionLog(TransLogCategory LogCategory, INT consume, INT gain, uint64_t totalValue, const char* logMessage)
	{
		Svr::GetServerComponent<DB::GameTransactionDB>()->AddGameLog(GetShardID(), GetPlayerID(), Util::Time.GetTimeUTCSec(), LogCategory, consume, gain, totalValue, logMessage);
	}

	void GamePlayerEntity::AddGameTransactionLog(TransLogCategory LogCategory, INT consume, INT gain, uint64_t totalValue)
	{
		AddGameTransactionLog(LogCategory, consume, gain, totalValue, "");
	}

	// Send push notify
	Result GamePlayerEntity::SendPushNotify( const char* strMessage, uint64_t param )
	{
		Result hr = ResultCode::SUCCESS;

		// Send GCM push notify
		if( GetGCMKeys() != nullptr && GetGCMKeys()[0] != '\0' )
		{
			Svr::ExternalTransactionManager *pExternal = Svr::GetServerComponent<Svr::ExternalTransactionManager>();
			svrChkPtr(pExternal);
			svrChk( pExternal->SendGCMNotify( 0, GetGCMKeys(), strMessage, param ) );
		}
	Proc_End:

		return hr;
	}


}; // namespace GameServer
}; // namespace SF


