////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Blue3k
// 
// Author : KyungKun Ko
//
// Description : Game player Entity
//	
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
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
#include "Server/BrServer.h"

#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GameNetPolicy.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Message/GameServerMsgClass.h"

#include "ServiceEntity/Game/GamePlayerEntity.h"
#include "ServiceEntity/Game/GameServiceTrans.h"
#include "ServiceEntity/Game/Subsystem/UserFriendSystem.h"
#include "ServiceEntity/Game/Subsystem/UserNotificationSystem.h"

#include "DB/GameTransactionDB.h"

#include "Transaction/GamePlayerEntityTrans.h"
#include "Transaction/GamePlayerEntityTransParty.h"
#include "Transaction/GamePlayerEntityTransFriend.h"
#include "Transaction/GamePlayerEntityTransClose.h"
#include "Transaction/GamePlayerEntityTransChatChannel.h"
#include "Transaction/GamePlayerEntityTransMatchMaking.h"





namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	GamePlayerEntity::GamePlayerEntity()
		: m_PlayerState(PlayerState_None)
		, m_GameInsUID(0)
		, m_PlayerData(GetHeap())
		, m_CharacterData(GetHeap())
	{
		memset(m_UserName, 0, sizeof(m_UserName));
		memset(m_GCMKeys, 0, sizeof(m_GCMKeys));

	}

	GamePlayerEntity::~GamePlayerEntity()
	{
	}

	// Initialize entity to proceed new connection
	Result GamePlayerEntity::InitializeEntity(EntityID newEntityID)
	{
		ScopeContext hr;

		svrCheck(Svr::SimpleUserEntity::InitializeEntity(newEntityID));

		m_GameInsUID = 0;
		m_PartyUID = 0;
		m_ShardID = 0;
		m_MatchingTicket = 0;
		m_MatchingStartTime = TimeStampMS(DurationMS(0));

		memset(&m_PlayerInformation, 0, sizeof(m_PlayerInformation));

		m_UserName[0] = '\0';
		m_GCMKeys[0] = '\0';

		SetLatestActiveTime(Util::Time.GetTimeUTCSec());
		m_LatestUpdateTime = {};

		uint32_t PlayerAutoLogout = 2 * 60; // TODO: move to constant
		GetTimeToKill().SetTimer(DurationMS(PlayerAutoLogout * 1000));

		svrCheck(GetComponentManager().AddComponent<UserFriendSystem>(this));
		//svrCheck(GetComponentManager().AddComponent<UserGamePlayerInfoSystem>(this));
		svrCheck(GetComponentManager().AddComponent<UserNotificationSystem>(this));

		svrCheck(GetComponentManager().InitializeComponents());


		return hr;
	}

	Result GamePlayerEntity::RegisterMessageHandlers()
	{
		ScopeContext hr;

		svrCheck(super::RegisterMessageHandlers());

		// RegisterPlayerToJoinGameServerCmd can send to player entity when previously logged in
		RegisterMessageHandler<GameServerTransRegisterPlayerToJoinGameServer<GamePlayerEntity>>();
		RegisterMessageHandler<PlayerTransRegisterPlayerToJoinGameServerOnPlayerEntity>();

		// Use chat channel
		//RegisterMessageHandler<PlayerTransChatMessageFromOtherEntity>();
		//BR_ENTITY_MESSAGE(Message::GameServer::ChatMessageC2SEvt)									{ svrMemReturn(pNewTrans = new(GetHeap()) PlayerTransChatMessageFromOtherEntity(GetHeap(),  pMsgData)); return ResultCode::SUCCESS; } );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Game Player

		RegisterMessageHandler<PlayerTransJoinGameServer>();
		RegisterMessageHandler<PlayerTransGetUserGamePlayerInfo>();
		RegisterMessageHandler<PlayerTransGetGamePlayerInfo>();

		RegisterMessageHandler<PlayerTransGetComplitionState>();
		RegisterMessageHandler<PlayerTransSetComplitionState>();
		RegisterMessageHandler<PlayerTransRegisterGCM>();
		RegisterMessageHandler<PlayerTransUnregisterGCM>();


		RegisterMessageHandler<PlayerTransSetNickName>();
		RegisterMessageHandler<PlayerTransFindPlayerByEMail>();
		RegisterMessageHandler<PlayerTransFindPlayerByPlayerID>();

		// Presence
		RegisterMessageHandler<PlayerTransRequestPlayerStatusUpdate>();
		RegisterMessageHandler<PlayerTransRequestPlayerStatusUpdateC2S>();
		RegisterMessageHandler<PlayerTransNotifyPlayerStatusUpdatedS2S>();


		// shop
		RegisterMessageHandler<PlayerTransBuyShopItemPrepare>();
		RegisterMessageHandler<PlayerTransBuyShopItem>();

		// Chat channel
		RegisterMessageHandler<PlayerTransCreateOrJoinChatChannel>();
		RegisterMessageHandler<PlayerTransJoinChatChannel>();
		RegisterMessageHandler<PlayerTransChatChannelPlayerJoinedS2SEvt>();
		RegisterMessageHandler<PlayerTransChatChannelLeaderChangedS2SEvt>();
		RegisterMessageHandler<PlayerTransLeaveChatChannel>();
		RegisterMessageHandler<PlayerTransChatChannelPlayerLeftS2SEvt>();
		RegisterMessageHandler<PlayerTransChatChannelKickPlayer>();
		RegisterMessageHandler<PlayerTransChatChannelPlayerKickedS2SEvt>();
		RegisterMessageHandler<PlayerTransChatChannelChatMessage>();
		RegisterMessageHandler<PlayerTransChatChannelChatMessageS2CEvt>();

		// Friends
		RegisterMessageHandler<PlayerTransInviteFriend>();
		RegisterMessageHandler<PlayerTransFriendAccept>();
		RegisterMessageHandler<PlayerTransRemoveFriend>();
		RegisterMessageHandler<PlayerTransGetFriendList>();
		RegisterMessageHandler<PlayerTransFriendAcceptedS2S>();
		RegisterMessageHandler<PlayerTransFriendRemovedS2S>();

		// notification
		RegisterMessageHandler<PlayerTransGetNotificationList>();
		RegisterMessageHandler<PlayerTransDeleteNotification>();
		RegisterMessageHandler<PlayerTransAcceptNotification>();
		RegisterMessageHandler<PlayerTransSetNotificationRead>();
		RegisterMessageHandler<PlayerTransNotifyS2S>();


		// party
		RegisterMessageHandler<PlayerTransCreateParty>();
		RegisterMessageHandler<PlayerTransJoinParty>();
		RegisterMessageHandler<PlayerTransLeaveParty>();
		RegisterMessageHandler<PlayerTransPartyKickPlayer>();
		RegisterMessageHandler<PlayerTransPartyInvite>();
		RegisterMessageHandler<PlayerTransPartyChatMessage>();
		RegisterMessageHandler<PlayerTransPartyQuickChatMessage>();
		RegisterMessageHandler<PlayerTransNotifyPartyInviteS2SEvt>();

		RegisterMessageHandler<PlayerTransPartyPlayerJoinedS2SEvt>();
		RegisterMessageHandler<PlayerTransPartyLeaderChangedS2SEvt>();
		RegisterMessageHandler<PlayerTransPartyPlayerLeftS2SEvt>();
		RegisterMessageHandler<PlayerTransPartyPlayerKickedS2SEvt>();
		RegisterMessageHandler<PlayerTransPartyChatMessageS2CEvt>();
		RegisterMessageHandler<PlayerTransPartyQuickChatMessageS2CEvt>();


		// Match making
		RegisterMessageHandler<PlayerTransRequestGameMatch>();
		RegisterMessageHandler<PlayerTransCancelGameMatch>();
		RegisterMessageHandler<PlayerTransPlayerMatchingCanceledS2CEvt>();
		RegisterMessageHandler<PlayerTransPlayerMatchingItemDequeuedS2CEvt>();
		RegisterMessageHandler<PlayerTransPartyQueuedGameMatchingS2CEvt>();
		RegisterMessageHandler<PlayerTransPartyCanceledGameMatchingS2CEvt>();
		RegisterMessageHandler<PlayerTransPartyMatchingItemDequeuedS2CEvt>();
		RegisterMessageHandler<PlayerTransGameMatchedS2SEvt>();

		// Ranking
		RegisterMessageHandler<PlayerTransGetRankingList>();

		return hr;
	}

	// Set connection for pilot
	Result GamePlayerEntity::SetConnection(SharedPointerT<Net::Connection>&& pCon)
	{
		ScopeContext hr;

		if (*pCon == *GetConnection())
			return hr;

		if (GetConnection() != nullptr)
		{
			ReleaseConnection("Replacing game player connection");
		}

		Service::ConnectionManager->AddConnection(pCon);

		svrCheck(Svr::SimpleUserEntity::SetConnection(std::forward<SharedPointerT<Net::Connection>>(pCon)));

		return hr;
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

	Result GamePlayerEntity::OnJoinGameServerInitialize(AuthTicket authTicket, FacebookUID fbUID)
	{
		Net::ConnectionPtr pConnection;
		ScopeContext hr([this, &pConnection](Result hr)
			{
				if (pConnection != nullptr && pConnection->GetCID() != 0)
				{
					Service::ConnectionManager->FreeCID((uint)pConnection->GetCID());
					pConnection->SetCID(0);
				}
			});
		Net::PeerInfo local, remote;

		svrCheckPtr(m_ServerNet);

		ReleaseConnection("JoinGameServer releasing old connection");

		SetAuthTicket(authTicket);
		SetFacebookUID(fbUID);

		svrCheckPtr(pConnection = new(GetSystemHeap()) Net::ConnectionMUDPServer(GetSystemHeap(), m_ServerNet->GetSocketIO()));

		local.SetInfo(GetServerNet()->GetNetClass(), m_ServerNet->GetLocalAddress(), BrServer::GetInstance()->GetServerUID());
		remote.SetInfo(NetClass::Client, authTicket);

		pConnection->SetCID(Service::ConnectionManager->NewCID());
		pConnection->SetRemoteID(authTicket);

		svrCheck(pConnection->InitConnection(local, remote));

		svrTrace(SVR_INFO, "Initialize connection CID:{0}, Addr:{1}", pConnection->GetCID(), pConnection->GetRemoteInfo().PeerAddress);

		// All players are already logged in, we don't need address map
		// All player connection in game service will be routed with peer id
		pConnection->SetUseAddressMap(false);

		// replace previous connection first
		svrCheck(SetConnection(std::forward<Net::ConnectionPtr>(pConnection)));

		SetLatestActiveTime(Util::Time.GetTimeUTCSec());

		AddGameTransactionLog(TransLogCategory::Account, 2, 0, 0, "Entity Initialize");

		return hr;
	}

	void GamePlayerEntity::Heartbeat()
	{
		OnNewUserTranscation();
	}

	Result GamePlayerEntity::OnNewUserTranscation()
	{
		// m_LatestUpdateTime is used as a valid character data signal
		if (m_LatestUpdateTime == UTCTimeStampSec::min())
			return ResultCode::SUCCESS_FALSE;

		super::Heartbeat();

		SetLatestActiveTime(Util::Time.GetTimeUTCSec());

		if (m_LatestDBSyncTime == UTCTimeStampSec::min() || Util::TimeSinceUTC(m_LatestDBSyncTime) > DurationSec(GameConst::PLAYER_UPDATE_STATUS_TIME))
		{
			UpdateDBSync();
		}

		return ResultCode::SUCCESS;
	}

	// clear transaction
	Result GamePlayerEntity::ClearEntity()
	{
		ScopeContext hr;

		ReleaseConnection("Clearing game player entity");

		svrCheck(Svr::SimpleUserEntity::ClearEntity());

		GetComponentManager().TerminateComponents();

		return hr;
	}

	// Run the task
	Result GamePlayerEntity::TickUpdate(TimerAction *pAction)
	{
		ScopeContext hr;

		hr = super::TickUpdate(pAction);
		if (hr == Result(ResultCode::SUCCESS_FALSE))
			return hr;

		//if (m_LatestDBSyncTime == 0 || Util::TimeSinceUTC(m_LatestDBSyncTime) > GameConst::PLAYER_UPDATE_STATUS_TIME)
		//{
		//	UpdateDBSync();
		//}

		return hr;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	
	//

	bool GamePlayerEntity::GetIsInGame() const
	{
		return GetGameInsUID().UID != 0 || GetPartyUID().UID != 0;
	}

	void GamePlayerEntity::SetGameInsUID(const GameInsUID& gameInsID)
	{
		m_GameInsUID = gameInsID;

		m_PlayerInformation.IsPlayingGame = m_GameInsUID.UID != 0 ? TRUE : FALSE;
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
	Result GamePlayerEntity::SendPushNotify(const char* strMessage, uint64_t param)
	{
		ScopeContext hr;

		// Send GCM push notify
		if (GetGCMKeys() != nullptr && GetGCMKeys()[0] != '\0')
		{
			Svr::ExternalTransactionManager *pExternal = Svr::GetServerComponent<Svr::ExternalTransactionManager>();
			svrCheckPtr(pExternal);
			svrCheck(pExternal->SendGCMNotify(0, GetGCMKeys(), strMessage, param));
		}

		return hr;
	}


} // namespace Svr
} // namespace SF


