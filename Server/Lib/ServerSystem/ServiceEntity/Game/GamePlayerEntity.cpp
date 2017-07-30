////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Blue3k
// 
// Author : KyungKun Ko
//
// Description : Game player Entity
//	
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServerUtil.h"
#include "Common/Task/EventTask.h"
#include "ServerSystem/ExternalTransactionManager.h"
#include "Net/NetServerUDP.h"
#include "Common/TimeUtil.h"
#include "Common/BrBaseTypes.h"
#include "Common/ResultCode/BRResultCodeCommon.h"
#include "Common/ResultCode/BRResultCodeGame.h"

#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Policy/GameIPolicy.h"
#include "Protocol/Message/GameMsgClass.h"

#include "ServerSystem/ServiceEntity/Game/GamePlayerEntity.h"

#include "DB/GameConspiracyDB.h"
#include "DB/GameTransactionDB.h"


BR_MEMORYPOOL_IMPLEMENT(BR::Svr::GamePlayerEntity);




namespace BR {
namespace Svr {


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
	{
	}

	GamePlayerEntity::~GamePlayerEntity()
	{
	}

	// Initialize entity to proceed new connection
	Result GamePlayerEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::SimpleUserEntity::InitializeEntity( newEntityID ) );

		m_GameInsUID = 0;
		m_ShardID = 0;

		memset( &m_PlayerInformation, 0, sizeof(m_PlayerInformation) );

		
		SetLatestActiveTime(Util::Time.GetTimeUTCSec());
		m_LatestUpdateTime = TimeStampSec(DurationSec(0));

		SetPlayerAutoLogout(DurationMS(15*60*1000));

		//m_TimeToKill.SetTimer(DurationMS(PlayerAutoLogout * 1000));

		//svrChk( AddComponent<UserFriendSystem>(this) );
		//svrChk( AddComponent<UserGamePlayerInfoSystem>(this) );
		//svrChk( AddComponent<UserNotifySystem>(this) );

		//svrChk( InitializeComponents() );

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
			svrChkPtr(m_ISvrGamePolicy = GetConnection()->GetPolicy<Policy::ISvrPolicyGame>());
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

	void GamePlayerEntity::SetLatestActiveTime(TimeStampSec latestActiveTime)
	{
		m_TimeToKill.SetTimer(GetPlayerAutoLogout());

		m_LatestActiveTime = latestActiveTime;
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
			return ResultCode::SUCCESS_FALSE;

		m_LatestDBSyncTime = Util::Time.GetTimeUTCSec();
		//auto pPlayerInfoSystem = GetComponent<UserGamePlayerInfoSystem>();

		//svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->UpdateTickStatusCmd(transID, GetShardID(), GetPlayerID(),
		//	pPlayerInfoSystem->GetGem(), pPlayerInfoSystem->GetStamina(), GetIsInGame() ? 1 : 0,
		//	GetLatestActiveTime(),
		//	GetLatestUpdateTime()));

	//Proc_End:

		return hr;
	}

	// register message handlers
	Result GamePlayerEntity::RegisterMessageHandlers()
	{
		
		BR_ENTITY_MESSAGE(Message::Game::HeartBitC2SEvt)				{ pNewTrans = nullptr; return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::JoinGameServerCmd)				{ pNewTrans = new PlayerTransJoinGameServer(pMsgData); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::Game::GetUserGamePlayerInfoCmd)		{ pNewTrans = new PlayerTransGetUserGamePlayerInfo(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::GetGamePlayerInfoCmd)			{ pNewTrans = new PlayerTransGetGamePlayerInfo(pMsgData); return OnNewUserTranscation(); } );

		//BR_ENTITY_MESSAGE(Message::Game::GetComplitionStateCmd)			{ pNewTrans = new PlayerTransGetComplitionState(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::SetComplitionStateCmd)			{ pNewTrans = new PlayerTransSetComplitionState(pMsgData); return OnNewUserTranscation(); } );

		//BR_ENTITY_MESSAGE(Message::Game::RegisterGCMCmd)				{ pNewTrans = new PlayerTransRegisterGCM(pMsgData); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::Game::UnregisterGCMCmd)				{ pNewTrans = new PlayerTransUnregisterGCM(pMsgData); return ResultCode::SUCCESS; } );

		//BR_ENTITY_MESSAGE(Message::Game::InviteFriendCmd)				{ pNewTrans = new PlayerTransInviteFriend(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::AcceptFriendRequestCmd)		{ pNewTrans = new PlayerTransFriendAccept(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::RemoveFriendCmd)				{ pNewTrans = new PlayerTransRemoveFriend(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::GetFriendListCmd)				{ pNewTrans = new PlayerTransGetFriendList(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::GiveStaminaCmd)				{ pNewTrans = new PlayerTransGiveStamina(pMsgData); return OnNewUserTranscation(); } );

		//BR_ENTITY_MESSAGE(Message::Game::GetNotificationListCmd)		{ pNewTrans = new PlayerTransGetNotificationList(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::DeleteNotificationCmd)			{ pNewTrans = new PlayerTransDeleteNotification(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::AcceptNotificationCmd)			{ pNewTrans = new PlayerTransAcceptNotification(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::SetNotificationReadCmd)		{ pNewTrans = new PlayerTransSetNotificationRead(pMsgData); return OnNewUserTranscation(); } );

		//BR_ENTITY_MESSAGE(Message::Game::SetNickNameCmd)				{ pNewTrans = new PlayerTransSetNickName(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::FindPlayerByEMailCmd)			{ pNewTrans = new PlayerTransFindPlayerByEMail(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::FindPlayerByPlayerIDCmd)		{ pNewTrans = new PlayerTransFindPlayerByPlayerID(pMsgData); return OnNewUserTranscation(); } );

		//BR_ENTITY_MESSAGE(Message::Game::RequestPlayerStatusUpdateCmd)	{ pNewTrans = new PlayerTransRequestPlayerStatusUpdate(pMsgData); return OnNewUserTranscation(); } );

		//BR_ENTITY_MESSAGE(Message::Game::GetRankingListCmd)				{ pNewTrans = new PlayerTransGetRankingList(pMsgData); return OnNewUserTranscation(); } );

		//BR_ENTITY_MESSAGE(Message::Game::BuyShopItemPrepareCmd)			{ pNewTrans = new PlayerTransBuyShopItemPrepare(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::BuyShopItemCmd)				{ pNewTrans = new PlayerTransBuyShopItem(pMsgData); return OnNewUserTranscation(); } );

		//BR_ENTITY_MESSAGE(Message::Game::JoinGameCmd)					{ pNewTrans = new PlayerTransJoinGame(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::KickPlayerCmd)					{ pNewTrans = new PlayerTransKickPlayer(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::LeaveGameCmd)					{ pNewTrans = new PlayerTransLeaveGame(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::AssignRoleCmd)					{ pNewTrans = new PlayerTransAssignRole(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::ChatMessageCmd)				{ pNewTrans = new PlayerTransChatMessage(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::VoteGameAdvanceCmd)			{ pNewTrans = new PlayerTransVoteGameAdvance(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::VoteCmd)						{ pNewTrans = new PlayerTransVote(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::AdvanceGameCmd)				{ pNewTrans = new PlayerTransAdvanceGame(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::GamePlayAgainCmd)				{ pNewTrans = new PlayerTransPlayAgain(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::GameRevealPlayerCmd)			{ pNewTrans = new PlayerTransGameRevealPlayer(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::GamePlayerReviveCmd)			{ pNewTrans = new PlayerTransGamePlayerRevive(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::GamePlayerResetRankCmd)		{ pNewTrans = new PlayerTransGamePlayerResetRank(pMsgData); return OnNewUserTranscation(); } );

		//BR_ENTITY_MESSAGE(Message::Game::CreatePartyCmd)				{ pNewTrans = new PlayerTransCreateParty(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::JoinPartyCmd)					{ pNewTrans = new PlayerTransJoinParty(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::LeavePartyCmd)					{ pNewTrans = new PlayerTransLeaveParty(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::PartyKickPlayerCmd)			{ pNewTrans = new PlayerTransPartyKickPlayer(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::PartyInviteCmd)				{ pNewTrans = new PlayerTransPartyInvite(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::PartyChatMessageCmd)			{ pNewTrans = new PlayerTransPartyChatMessage(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::PartyQuickChatMessageCmd)		{ pNewTrans = new PlayerTransPartyQuickChatMessage(pMsgData); return OnNewUserTranscation(); } );

		//BR_ENTITY_MESSAGE(Message::Game::RequestGameMatchCmd)			{ pNewTrans = new PlayerTransRequestGameMatch(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::CancelGameMatchCmd)			{ pNewTrans = new PlayerTransCancelGameMatch(pMsgData); return OnNewUserTranscation(); } );

		//BR_ENTITY_MESSAGE(Message::Game::SetPresetGameConfigIDCmd)		{ pNewTrans = new PlayerTransSetConfigPreset(pMsgData); return OnNewUserTranscation(); } );
		//BR_ENTITY_MESSAGE(Message::Game::GainGameResourceCmd)			{ pNewTrans = new PlayerTransGainGameResource(pMsgData); return OnNewUserTranscation(); } );

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
		Transaction *trans = nullptr;

		hr = Svr::SimpleUserEntity::TickUpdate(pAction);
		if (hr == Result(ResultCode::SUCCESS_FALSE))
			return hr;


		if (m_TimeToKill.CheckTimer() 
			&& GetEntityState() == EntityState::WORKING)
		{
			svrMem(trans = CreateCloseTransaction());
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



	// Update Game Player 
	Result GamePlayerEntity::UpdateGamePlayer()
	{
		Result hr = ResultCode::SUCCESS;

		// m_LatestUpdateTime will be initialized when character data is loaded
		if (m_LatestUpdateTime == TimeStampSec::min())
		{
			return ResultCode::SUCCESS_FALSE;
		}


	//Proc_End:

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
		//const_cast<GamePlayerEntity*>(this)->m_PlayerInformation.Level = GetComponent<UserGamePlayerInfoSystem>()->GetLevel();
		//const_cast<GamePlayerEntity*>(this)->m_PlayerInformation.WeeklyWin = GetComponent<UserGamePlayerInfoSystem>()->GetWeeklyWin();
		//const_cast<GamePlayerEntity*>(this)->m_PlayerInformation.WeeklyLose = GetComponent<UserGamePlayerInfoSystem>()->GetWeeklyLose();

		return m_PlayerInformation;
	}


	//const ServerFriendInformation& GamePlayerEntity::GetFriendInformation() const
	//{
	//	m_PlayerInformation.ShardID = GetShardID();
	//	m_PlayerInformation.Level = GetComponent<UserGamePlayerInfoSystem>()->GetLevel();
	//	m_PlayerInformation.WeeklyWin = GetComponent<UserGamePlayerInfoSystem>()->GetWeeklyWin();
	//	m_PlayerInformation.WeeklyLose = GetComponent<UserGamePlayerInfoSystem>()->GetWeeklyLose();
	//	m_PlayerInformation.LastActiveTime = GetLatestActiveTime().time_since_epoch().count();
	//	m_PlayerInformation.IsPlayingGame = GetIsInGame();

	//	return m_PlayerInformation;
	//}

	void GamePlayerEntity::AddGameTransactionLog(TransLogCategory LogCategory, INT consume, INT gain, UINT64 totalValue, const char* logMessage)
	{
		Svr::GetServerComponent<DB::GameTransactionDB>()->AddGameLog(GetShardID(), GetPlayerID(), Util::Time.GetTimeUTCSec(), LogCategory, consume, gain, totalValue, logMessage);
	}

	void GamePlayerEntity::AddGameTransactionLog(TransLogCategory LogCategory, INT consume, INT gain, UINT64 totalValue)
	{
		AddGameTransactionLog(LogCategory, consume, gain, totalValue, "");
	}

	//// Send push notify
	//Result GamePlayerEntity::SendPushNotify( const char* strMessage, UINT64 param )
	//{
	//	Result hr = ResultCode::SUCCESS;

	//	// Send GCM push notify
	//	if( GetGCMKeys() != nullptr && GetGCMKeys()[0] != '\0' )
	//	{
	//		Svr::ExternalTransactionManager *pExternal = Svr::GetServerComponent<Svr::ExternalTransactionManager>();
	//		svrChkPtr(pExternal);
	//		svrChk( pExternal->SendGCMNotify( 0, GetGCMKeys(), strMessage, param ) );
	//	}
	//Proc_End:

	//	return hr;
	//}


}; // namespace GameServer
}; // namespace BR


