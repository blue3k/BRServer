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
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BRServerUtil.h"
#include "ServerSystem/EventTask.h"
#include "ServerSystem/ExternalTask.h"
#include "Net/NetServerUDP.h"
#include "Common/TimeUtil.h"
#include "Common/BRBaseTypes.h"
#include "Common/HRESCommon.h"
#include "Common/HRESGame.h"


#include "GameSvrConst.h"
#include "GameServerClass.h"


#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Policy/GameIPolicy.h"
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


BR_MEMORYPOOL_IMPLEMENT(BR::GameServer::GamePlayerEntity);




namespace BR {
namespace GameServer {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	GamePlayerEntity::GamePlayerEntity()
		: m_ISvrGamePolicy(nullptr)
		, m_Allocator(STDAllocator::GetInstance())
		, m_PlayerState(PlayerState_None)
		, m_GameInsUID(0)
		, m_LatestUpdateTime(0)
		, m_LatestDBSyncTime(0)
		, m_LatestActiveTime(0)
	{
		memset( m_UserName, 0, sizeof(m_UserName) );
		memset( m_GCMKeys, 0, sizeof(m_GCMKeys) );
	}

	GamePlayerEntity::~GamePlayerEntity()
	{
	}

	// Initialize entity to proceed new connection
	HRESULT GamePlayerEntity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_OK;
		auto pGameConfig = GetMyServer()->GetPresetGameConfig();
		auto PlayerAutoLogout = pGameConfig->PlayerAutoLogout;

		svrChk( __super::InitializeEntity( newEntityID ) );

		m_GameInsUID = 0;
		m_PartyUID = 0;
		m_ShardID = 0;
		m_MatchingTicket = 0;

		memset( &m_PlayerInformation, 0, sizeof(m_PlayerInformation) );

		m_UserName[0] = '\0';
		m_GCMKeys[0] = '\0';
		
		SetLatestActiveTime(Util::Time.GetTimeUTCSec32());
		m_LatestUpdateTime = 0;
		//m_LatestUpdateTime = Util::Time.GetTimeUTCSec();

		m_TimeToKill.SetTimer(PlayerAutoLogout * 1000);

		svrChk( AddComponent<UserFriendSystem>(this) );
		svrChk( AddComponent<UserGamePlayerInfoSystem>(this) );
		svrChk( AddComponent<UserNotifySystem>(this) );

		svrChk( InitializeComponents() );

	Proc_End:


		return hr;
	}


	// Set connection for pilot
	HRESULT GamePlayerEntity::SetConnection(SharedPointerT<Net::Connection>&& pCon)
	{
		HRESULT hr = S_OK;

		if (pCon == GetConnection())
			return hr;

		if( GetConnection() != nullptr )
		{
			ReleaseConnection();
		}

		svrChk(__super::SetConnection(std::forward<SharedPointerT<Net::Connection>>(pCon)));

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

		__super::ReleaseConnection();
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity process
	//

	void GamePlayerEntity::SetLatestActiveTime(ULONG latestActiveTime)
	{
		auto pGameConfig = GetMyServer()->GetPresetGameConfig();
		if (pGameConfig != nullptr)
		{
			auto PlayerAutoLogout = pGameConfig->PlayerAutoLogout;
			m_TimeToKill.SetTimer(PlayerAutoLogout * 1000);
		}

		m_LatestActiveTime = latestActiveTime;
	}

	HRESULT GamePlayerEntity::OnNewUserTranscation()
	{
		SetLatestActiveTime(Util::Time.GetTimeUTCSec32());

		if (m_LatestDBSyncTime == 0 || Util::TimeSinceUTC(m_LatestDBSyncTime) > GameConst::PLAYER_UPDATE_STATUS_TIME)
		{
			UpdateDBSync(0);
		}

		return S_OK;
	}

	HRESULT GamePlayerEntity::UpdateDBSync(TransactionID transID)
	{
		HRESULT hr = S_OK;

		m_LatestDBSyncTime = Util::Time.GetTimeUTCSec32();
		auto pPlayerInfoSystem = GetComponent<UserGamePlayerInfoSystem>();

		svrChk(UpdateGamePlayer());

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->UpdateTickStatusCmd(transID, GetShardID(), GetPlayerID(),
			pPlayerInfoSystem->GetGem(), pPlayerInfoSystem->GetStamina(), GetIsInGame() ? 1 : 0,
			GetLatestActiveTime(),
			GetLatestUpdateTime()));

	Proc_End:

		return hr;
	}

	// register message handlers
	HRESULT GamePlayerEntity::RegisterMessageHandlers()
	{
		
		BR_ENTITY_MESSAGE(Message::Game::HeartBitC2SEvt)				{ pNewTrans = nullptr; return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::JoinGameServerCmd)				{ pNewTrans = new PlayerTransJoinGameServer(pMsgData); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::Game::GetUserGamePlayerInfoCmd)		{ pNewTrans = new PlayerTransGetUserGamePlayerInfo(pMsgData); return OnNewUserTranscation(); } );
		BR_ENTITY_MESSAGE(Message::Game::GetGamePlayerInfoCmd)			{ pNewTrans = new PlayerTransGetGamePlayerInfo(pMsgData); return OnNewUserTranscation(); } );

		BR_ENTITY_MESSAGE(Message::Game::RegisterGCMCmd)				{ pNewTrans = new PlayerTransRegisterGCM(pMsgData); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::Game::UnregisterGCMCmd)				{ pNewTrans = new PlayerTransUnregisterGCM(pMsgData); return S_OK; } );

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

		BR_ENTITY_MESSAGE(Message::Game::RequestPlayerStatusUpdateCmd)	{ pNewTrans = new PlayerTransRequestPlayerStatusUpdate(pMsgData); return OnNewUserTranscation(); } );

		BR_ENTITY_MESSAGE(Message::Game::GetRankingListCmd)				{ pNewTrans = new PlayerTransGetRankingList(pMsgData); return OnNewUserTranscation(); } );

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

		return S_OK;
	}

	// clear transaction
	HRESULT GamePlayerEntity::ClearEntity()
	{
		HRESULT hr = S_OK;

		ReleaseConnection();

		svrChk( __super::ClearEntity() );

	Proc_End:

		return hr;
	}

	// Run the task
	HRESULT GamePlayerEntity::TickUpdate(Svr::TimerAction *pAction)
	{
		HRESULT hr = S_OK;
		Svr::Transaction *trans = nullptr;

		hr = __super::TickUpdate(pAction);
		if (hr == S_FALSE)
			return hr;

		//if (m_LatestDBSyncTime == 0 || Util::TimeSinceUTC(m_LatestDBSyncTime) > GameConst::PLAYER_UPDATE_STATUS_TIME)
		//{
		//	UpdateDBSync(0);
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
	HRESULT GamePlayerEntity::UpdateGameConfig()
	{
		HRESULT hr = S_OK;

		svrChk( GetComponent<UserGamePlayerInfoSystem>()->UpdateStatMaximum() );

	Proc_End:

		return hr;
	}


	// Update Game Player 
	HRESULT GamePlayerEntity::UpdateGamePlayer()
	{
		HRESULT hr = S_OK;

		svrChkPtr(GetMyServer()->GetPresetGameConfig());

		// m_LatestUpdateTime will be initialized when character data is loaded
		if (m_LatestUpdateTime == 0)
		{
			return E_FAIL;
		}

		ULONGLONG tickTime = GetMyServer()->GetPresetGameConfig()->StaminaRecoveryTime;
		ULONGLONG curUTCSec = Util::Time.GetTimeUTCSec();
		ULONGLONG timeDiff = curUTCSec - m_LatestUpdateTime;
		ULONGLONG numberOfTicks = timeDiff / tickTime;
		ULONGLONG remainTime = timeDiff % tickTime;

		svrTrace(Svr::TRC_TRANSACTION, "Check GamePlayer Update Tick PID:%0% last:%1%, curTime:%2%, numTick:%3%, remain:%4%", GetPlayerID(), m_LatestUpdateTime, curUTCSec, numberOfTicks, remainTime);

		if( numberOfTicks > 0 )
		{
			svrTrace(Svr::TRC_TRANSACTION, "GamePlayer Update PID:%0%, numTick:%1%, Sta:%2%", GetPlayerID(), numberOfTicks, GetComponent<UserGamePlayerInfoSystem>()->GetStamina());

			if (numberOfTicks > std::numeric_limits<INT16>::max())
			{
				svrTrace( Trace::TRC_ERROR, "Invalid tick count, applying maximum int16" );
				numberOfTicks = std::numeric_limits<INT16>::max();
			}

			GetComponent<UserGamePlayerInfoSystem>()->GainStamina( (INT)(numberOfTicks * 1) );

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
		m_PlayerInformation.LastActiveTime = GetLatestActiveTime();
		m_PlayerInformation.IsPlayingGame = GetIsInGame();

		return m_PlayerInformation;
	}

	void GamePlayerEntity::AddGameTransactionLog(TransLogCategory LogCategory, INT consume, INT gain, UINT64 totalValue, const char* logMessage)
	{
		Svr::GetServerComponent<DB::GameTransactionDB>()->AddGameLog(GetShardID(), GetPlayerID(), Util::Time.GetTimeUTCSec32(), LogCategory, consume, gain, totalValue, logMessage);
	}

	void GamePlayerEntity::AddGameTransactionLog(TransLogCategory LogCategory, INT consume, INT gain, UINT64 totalValue)
	{
		AddGameTransactionLog(LogCategory, consume, gain, totalValue, "");
	}

	// Send push notify
	HRESULT GamePlayerEntity::SendPushNotify( const char* strMessage, UINT64 param )
	{
		HRESULT hr = S_OK;

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
}; // namespace BR


