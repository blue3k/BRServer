////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"

#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "ResultCode/SFResultCodeLogin.h"
#include "MemoryManager/SFMemoryPool.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"

#include "Net/SFNetServerUDP.h"

#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "ServerEntity/ServerEntityManager.h"

#include "Transaction/ExternalTransaction.h"
#include "Transaction/ExternalTransactionManager.h"

#include "Protocol/Policy/LoginServerNetPolicy.h"
#include "Protocol/Message/LoginServerMsgClass.h"

#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"
#include "Protocol/Message/GamePartyMsgClass.h"
#include "Protocol/Policy/GamePartyNetPolicy.h"
#include "Protocol/Message/PartyMatchingQueueMsgClass.h"
#include "Protocol/Policy/PartyMatchingQueueNetPolicy.h"

#include "GamePlayerEntityTrans.h"
#include "ServiceEntity/Game/GamePlayerEntity.h"

#include "Server/BrServer.h"


#include "ServiceEntity/Game/Subsystem/UserFriendSystem.h"
//#include "GameSystem/UserGamePlayerInfoSystem.h"
#include "ServiceEntity/Game/Subsystem/UserNotificationSystem.h"


#include "DB/AccountDB.h"
#include "DB/AccountQuery.h"
#include "Game/GameDB.h"
#include "Game/GameQuery.h"
#include "DB/RankingDB.h"
#include "DB/RankingDBQuery.h"
#include "DB/LoginSessionDB.h"
#include "DB/LoginSessionQuery.h"


#define 	SHA256_DIGEST_LENGTH   32




	
namespace SF {
namespace Svr {


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Game command transaction
	//


	PlayerTransJoinGameServer::PlayerTransJoinGameServer(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction(heap, Forward<MessageDataPtr>(pIMsg) )
	{
		SetExclusive(true);

		AddSubAction<DB::QueryConnectedToGameServerCmd>(&PlayerTransJoinGameServer::ConnectedToGameServerDB, &PlayerTransJoinGameServer::OnConnectedToGameServerDB);
		AddSubAction<DB::QueryGetPlayerInfoCmd>(&PlayerTransJoinGameServer::RequestPlayerInfoFromDB, &PlayerTransJoinGameServer::OnGetPlayerGameDataRes);
		AddSubAction<DB::QueryCreatePlayerInfoCmd>(&PlayerTransJoinGameServer::RequestPlayerInfoCreateDB, &PlayerTransJoinGameServer::OnCreatePlayerGameDataRes);
		AddSubAction<Message::GameParty::JoinPartyRes>(&PlayerTransJoinGameServer::RequestJoinPartyIfExist, &PlayerTransJoinGameServer::OnJoinPartyRes);
		AddSubAction(&PlayerTransJoinGameServer::FinalizeSuccess);
	}

	Result PlayerTransJoinGameServer::ConnectedToGameServerDB()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr) CloseTransaction(hr);
			});

		svrCheck(Svr::GetServerComponent<DB::LoginSessionDB>()->ConnectedToGameServer(GetTransID(), GetAccID(), GetTicket(), GetLoginEntityUID(), GetOwnerEntityUID()));

		return hr;
	}

	Result PlayerTransJoinGameServer::OnConnectedToGameServerDB(Svr::TransactionResult* pRes)
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr) CloseTransaction(hr);
			});
		auto* pDBRes = pRes->GetResultData<DB::QueryConnectedToGameServerCmd>();

		svrCheck(pRes->GetResult());

		// succeeded to login
		if (pDBRes->Result == 0)
		{
		}
		else
		{
			CloseTransaction(ResultCode::LOGIN_INVALID_SIGNATURE);
		}

		return hr;
	}

	//Result PlayerTransJoinGameServer::OnGameServerJoined( Svr::TransactionResult* pRes )
	//{
	//	ScopeContext hr;
	//	Message::LoginServer::PlayerJoinedToGameServerRes msgRes;

	//	svrCheckClose(pRes->GetResult());
	//	svrCheck( msgRes.ParseMessage(pRes->GetResultData<Message::MessageData>()) );

	//	return hr; 
	//}

	Result PlayerTransJoinGameServer::OnJoinPartyRes( Svr::TransactionResult* pRes )
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					GetMyOwner()->SetPartyUID(0);
			});
		Message::GameParty::JoinPartyRes msgRes;

		hr = pRes->GetResult();
		if (!(hr))
		{
			m_PartyLeaderID = 0;
			return hr;
		}

		svrCheck( msgRes.ParseMessage( pRes->GetResultData<Message::MessageData>() ) );

		m_PartyLeaderID = msgRes.GetPartyLeaderID();


		return ResultCode::SUCCESS;
	}

	Result PlayerTransJoinGameServer::SetPlayerGameData(const VariableTable& playerDataDB)
	{
		ScopeContext hr;
		auto& playerData = GetMyOwner()->GetPlayerData();

		// TODO: Need to change initial nick name setup process
		GetMyOwner()->SetNickName(playerDataDB.GetValue<String>("GameNick"_crc32c));

		auto latestTick = playerDataDB.GetValue<int64_t>("LatestTickTime"_crc32c);

		GetMyOwner()->SetLatestActiveTime(Util::Time.GetTimeUTCSec());

		// If first login, the value will be zero
		if (latestTick == 0)
		{
			svrTrace(Warning, "Invalid player tick time for PlayerID:{0}. Reset to current time", GetMyOwner()->GetPlayerID());
			GetMyOwner()->SetLatestUpdateTime(Util::Time.GetTimeUTCSec());
		}
		else
		{
			svrTrace(SVR_TRANSACTION, "Latest tick time PlayerID:{0}. {1}", GetMyOwner()->GetPlayerID(), (uint64_t)latestTick);

			GetMyOwner()->SetLatestUpdateTime(UTCTimeStampSec(DurationSec(latestTick)));
		}

		GetMyOwner()->AddGameTransactionLog(TransLogCategory::Account, 1, 0, 0, "Login");

		// copy to return information
		StrUtil::StringCopy(m_PlayerNick, GetMyOwner()->GetNickName());
		m_MatchingTicket = GetMyOwner()->GetMatchingTicket();
		m_GameUID = GetMyOwner()->GetGameInsUID();

		m_HaveValidPlayerData = true;

		return hr;
	}

	Result PlayerTransJoinGameServer::OnCreatePlayerGameDataRes(Svr::TransactionResult* pRes)
	{
		ScopeContext hr;
		auto pDBRes = pRes->GetResultData<DB::QueryCreatePlayerInfoCmd>();

		svrCheck(pRes->GetResult());

		// succeeded to query
		if( pDBRes->Result < 0 )
		{
			svrError(ResultCode::SVR_INVALID_PLAYER_GAMEDB);
		}

		svrCheckCondition( pDBRes->RowsetResults.size() >= 1 );
		{
			auto& playerInfoData = *pDBRes->RowsetResults.begin();
			auto& playerData = GetMyOwner()->GetPlayerData();

			// New player data. Reset stat to default
			if (pDBRes->Result == 0)
			{
				// TODO: FIXME
				//playerData.SetupDefaultStat();

				svrTrace(SVR_DBGTRANS, "Player data created PlayerID:{0}", GetMyOwner()->GetPlayerID());
			}

			svrCheck(SetPlayerGameData(playerInfoData));
		}

		return hr; 
	}

	Result PlayerTransJoinGameServer::OnGetPlayerGameDataRes(Svr::TransactionResult* pRes)
	{
		ScopeContext hr;
		auto pDBRes = (DB::QueryGetPlayerInfoCmd*)pRes;

		svrCheck(pRes->GetResult());

		// succeeded to query
		if (pDBRes->Result < 0)
		{
			// Need new player data. next action will push it
			return hr;
		}

		svrCheckCondition(pDBRes->RowsetResults.size() >= 1);
		auto& playerInfoData = *pDBRes->RowsetResults.begin();
		svrCheck(SetPlayerGameData(playerInfoData));

		return hr;
	}

	//Result PlayerTransJoinGameServer::NotifyLoginServer()
	//{
	//	ScopeContext hr;

	//	EntityUID loginEntityUID(GetLoginEntityUID());


	//	auto loginEndpoint = Service::MessageEndpointManager->GetEndpoint(loginEntityUID);

	//	// TODO: We need to distinguish whether character data is updated or not
	//	svrCheck(NetPolicyLoginServer(loginEndpoint).PlayerJoinedToGameServerCmd(
	//		RouteContext(GetOwnerEntityUID(), loginEntityUID), GetTransID(),
	//		GetAccID(), GetTicket()));

	//	return hr;
	//}

	Result PlayerTransJoinGameServer::RequestPlayerInfoFromDB()
	{
		ScopeContext hr;

		// succeeded to create
		svrCheck(Service::PlayerManager->CreatePlayer(GetAccID(), GetOwnerEntityUID()));

		svrCheck(Svr::GetServerComponent<DB::GameDB>()->GetPlayerInfoCmd(GetTransID(), GetMyOwner()->GetShardID(), GetAccID()));

		return hr;
	}

	Result PlayerTransJoinGameServer::RequestPlayerInfoCreateDB()
	{
		ScopeContext hr;

		if (m_HaveValidPlayerData)
			return ResultCode::SUCCESS_FALSE;

		svrCheck(Svr::GetServerComponent<DB::GameDB>()->CreatePlayerInfoCmd(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID()));

		return hr;
	}

	Result PlayerTransJoinGameServer::RequestJoinPartyIfExist()
	{
		ScopeContext hr;

		if (GetMyOwner()->GetPartyUID().UID != 0)
		{
			PartyUID partyUID = GetMyOwner()->GetPartyUID();

			auto partyEndpoint = Service::MessageEndpointManager->GetEndpoint(partyUID);

			svrCheck(NetPolicyGameParty(partyEndpoint).JoinPartyCmd(
				RouteContext(GetOwnerEntityUID(), partyUID), GetTransID(),
				0, GetMyOwner()->GetPlayerInformation()));
		}
		else
		{
			return ResultCode::SUCCESS_FALSE;
		}

		return hr;
	}


	Result PlayerTransJoinGameServer::FinalizeSuccess()
	{
		ScopeContext hr;

		GetMyOwner()->UpdateDBSync();
		CloseTransaction(hr);

		return hr;
	}

	// Start Transaction
	Result PlayerTransJoinGameServer::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});


		m_GameUID = 0;
		m_PartyLeaderID = 0;
		m_PlayerNick[0] = 0;
		m_MatchingTicket = 0;

		if (GetAccID() == 0)
		{
			svrErrorClose(ResultCode::INVALID_ACCOUNTID);
		}

		if (GetTicket() == 0)
		{
			svrErrorClose(ResultCode::INVALID_TICKET);
		}

		GetMyOwner()->SetAccountID(GetAccID());
		GetMyOwner()->SetAuthTicket(GetTicket());

		EntityUID currentPlayerUID;
		if (Service::PlayerManager->FindPlayer(GetAccID(), currentPlayerUID))
		{
			svrCheck(ResultCode::GAME_ALREADY_IN_GAME);
			return hr;
		}

		svrCheck(super::StartTransaction());

		return hr;
	}



	// Start Transaction
	Result PlayerTransGetUserGamePlayerInfo::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		m_Result.Clear();

		//auto userGamePlayerInfo = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>();

		svrCheck( super::StartTransaction() );

		if( GetMyOwner()->GetAccountID() == 0 )
		{
			svrErrorClose(ResultCode::INVALID_TICKET);
		}

		// TODO: FIXME
		//m_Result.Level = userGamePlayerInfo->GetLevel();
		//m_Result.Exp = userGamePlayerInfo->GetExp();
		//m_Result.GameMoney = userGamePlayerInfo->GetGameMoney();
		//m_Result.Gem = userGamePlayerInfo->GetGem();
		//m_Result.Stamina = userGamePlayerInfo->GetStamina();
		//m_Result.LastUpdateTime = GetMyOwner()->GetLatestUpdateTime().time_since_epoch().count();

		//m_Result.TotalPlayed = userGamePlayerInfo->GetTotalPlayed();

		return hr;
	}



	PlayerTransGetGamePlayerInfo::PlayerTransGetGamePlayerInfo(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction( heap, std::forward<MessageDataPtr>(pIMsg) )
	{
		SetExclusive(true);

		AddSubAction<DB::QueryGetPlayerShardIDCmd>(&PlayerTransGetGamePlayerInfo::RequestPlayerShardID, &PlayerTransGetGamePlayerInfo::OnGetPlayerShardID);
		AddSubAction<DB::QueryGetPlayerInfoCmd>(&PlayerTransGetGamePlayerInfo::RequestGamePlayerInfo, &PlayerTransGetGamePlayerInfo::OnGetGamePlayerInfo);
	}

	Result PlayerTransGetGamePlayerInfo::RequestPlayerShardID()
	{
		ScopeContext hr;

		svrCheck(Svr::GetServerComponent<DB::AccountDB>()->GetPlayerShardID(GetTransID(), GetPlayerID()));

		return hr;
	}

	Result PlayerTransGetGamePlayerInfo::OnGetPlayerShardID(Svr::TransactionResult* pRes)
	{
		ScopeContext hr;
		auto* pDBRes = pRes->GetResultData<DB::QueryGetPlayerShardIDCmd>();

		svrCheck(pRes->GetResult());

		// succeeded to query
		if (pDBRes->Result < 0)
		{
			svrError(ResultCode::GAME_INVALID_PLAYER);
		}

		m_ShardId = pDBRes->ShardID;

		return hr;
	}

	Result PlayerTransGetGamePlayerInfo::RequestGamePlayerInfo()
	{
		ScopeContext hr;

		svrCheck(Svr::GetServerComponent<DB::GameDB>()->GetPlayerInfoCmd(GetTransID(), m_ShardId, GetPlayerID()));

		return hr;
	}

	Result PlayerTransGetGamePlayerInfo::OnGetGamePlayerInfo( Svr::TransactionResult* pRes )
	{
		ScopeContext hr;
		auto pDBRes = (DB::QueryGetPlayerInfoCmd*)pRes;

		svrCheck(pRes->GetResult());

		// succeeded to query
		if( pDBRes->Result < 0 )
		{
			svrError(ResultCode::GAME_INVALID_PLAYER);
		}

		svrCheckCondition( pDBRes->RowsetResults.size() >= 1 );
		{
			auto& playerInfoData = *pDBRes->RowsetResults.begin();

			// TODO: FIXME
			//m_Result.Level = playerInfoData.Level;
			//m_Result.TotalPlayed = playerInfoData.TotalPlayed;

			//m_Result.WinPlaySC = playerInfoData.WinPlaySC;
			//m_Result.WinPlaySM = playerInfoData.WinPlaySM;
			//m_Result.WinPlaySM = playerInfoData.WinPlaySM;
			//m_Result.LosePlaySC = playerInfoData.LosePlaySC;
			//m_Result.LosePlaySM = playerInfoData.LosePlaySM;
			//m_Result.LosePlaySM = playerInfoData.LosePlaySM;

			//m_Result.WinPlayNC = playerInfoData.WinPlayNC;
			//m_Result.WinPlayNM = playerInfoData.WinPlayNM;
			//m_Result.WinPlayNM = playerInfoData.WinPlayNM;
			//m_Result.LosePlayNC = playerInfoData.LosePlayNC;
			//m_Result.LosePlayNM = playerInfoData.LosePlayNM;
			//m_Result.LosePlayNM = playerInfoData.LosePlayNM;

			//m_Result.WeeklyWin = playerInfoData.WeeklyPlayWin;
			//m_Result.WeeklyLose = playerInfoData.WeeklyPlayLose;
		}


		return ResultCode::SUCCESS; 
	}

	
	
	// Start Transaction
	Result PlayerTransGetGamePlayerInfo::StartTransaction()
	{
		ScopeContext hr;

		m_Result.Clear();

		if( GetMyOwner()->GetAccountID() == 0 )
		{
			svrErrorClose(ResultCode::INVALID_TICKET);
		}

		svrCheck(super::StartTransaction());

		return hr;
	}



	PlayerTransGetComplitionState::PlayerTransGetComplitionState(IHeap& heap, MessageDataPtr &pIMsg)
		: MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg))
	{
		RegisterMessageHandler<DB::QueryGetComplitionStateCmd>(&PlayerTransGetComplitionState::OnGetComplitionState);
	}

	Result PlayerTransGetComplitionState::OnGetComplitionState(Svr::TransactionResult* pRes)
	{
		ScopeContext hr;

		auto* pDBRes = (DB::QueryGetComplitionStateCmd*)pRes;

		svrCheck(pRes->GetResult());

		// succeeded to query
		if (pDBRes->Result < 0)
		{
			svrError(ResultCode::GAME_INVALID_PLAYER);
		}

		svrCheckCloseErr(ResultCode::UNEXPECTED, StrUtil::StringCopy(m_ComplitionState, pDBRes->ComplitionState));

		return hr;
	}


	// Start Transaction
	Result PlayerTransGetComplitionState::StartTransaction()
	{
		ScopeContext hr;

		memset(&m_ComplitionState, 0, sizeof(m_ComplitionState));

		svrCheck(super::StartTransaction());

		if (GetMyOwner()->GetAccountID() == 0)
		{
			svrErrorClose(ResultCode::INVALID_TICKET);
		}

		svrCheck(Svr::GetServerComponent<DB::GameDB>()->GetComplitionState(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID()));

		return hr;
	}




	PlayerTransSetComplitionState::PlayerTransSetComplitionState(IHeap& heap, MessageDataPtr &pIMsg)
		: MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg))
	{
		RegisterMessageHandler<DB::QuerySetComplitionStateCmd>(&PlayerTransSetComplitionState::OnSetComplitionState);
	}

	Result PlayerTransSetComplitionState::OnSetComplitionState(Svr::TransactionResult* pRes)
	{
		ScopeContext hr;

		auto* pDBRes = (DB::QuerySetComplitionStateCmd*)pRes;

		svrCheck(pRes->GetResult());

		// succeeded to query
		if (pDBRes->Result < 0)
		{
			svrError(ResultCode::GAME_INVALID_PLAYER);
		}

		return hr;
	}

	// Start Transaction
	Result PlayerTransSetComplitionState::StartTransaction()
	{
		ScopeContext hr;

		svrCheck(super::StartTransaction());

		if (GetMyOwner()->GetAccountID() == 0)
		{
			svrErrorClose(ResultCode::INVALID_TICKET);
		}

		svrCheck(Svr::GetServerComponent<DB::GameDB>()->SetComplitionState(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(), GetComplitionState()));

		return hr;
	}


	
	/////////////////////////////////////////////////////////////////////////////
	//
	//	GCM register/unregister transaction
	//


	PlayerTransRegisterGCM::PlayerTransRegisterGCM(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg) )
	{
		RegisterMessageHandler<DB::QueryUpdateGCMKeysCmd>(&PlayerTransRegisterGCM::OnUpdated);
	}

	Result PlayerTransRegisterGCM::OnUpdated( Svr::TransactionResult* pRes )
	{
		ScopeContext hr;

		svrCheck(pRes->GetResult());

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransRegisterGCM::StartTransaction()
	{
		ScopeContext hr;

		svrCheck( super::StartTransaction() );

		GetMyOwner()->SetGCMKeys( GetGCMRegisteredID() );

		svrCheck( Svr::GetServerComponent<DB::AccountDB>()->UpdateGCMKeys( GetTransID(), GetMyOwner()->GetAccountID(), GetMyOwner()->GetGCMKeys() ) );

		return hr;
	}
	


	PlayerTransUnregisterGCM::PlayerTransUnregisterGCM(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction( heap, std::forward<MessageDataPtr>(pIMsg) )
	{
		RegisterMessageHandler<DB::QueryUpdateGCMKeysCmd>(&PlayerTransUnregisterGCM::OnUpdated);
	}

	Result PlayerTransUnregisterGCM::OnUpdated( Svr::TransactionResult* pRes )
	{
		ScopeContext hr;

		svrCheck(pRes->GetResult());

		return hr; 
	}

	// Start Transaction
	Result PlayerTransUnregisterGCM::StartTransaction()
	{
		ScopeContext hr;

		svrCheck( super::StartTransaction() );

		GetMyOwner()->SetGCMKeys( "" );

		svrCheck( Svr::GetServerComponent<DB::AccountDB>()->UpdateGCMKeys( GetTransID(), GetMyOwner()->GetAccountID(), GetMyOwner()->GetGCMKeys() ) );

		return hr;
	}
	
	
	
	/////////////////////////////////////////////////////////////////////////////
	//
	//	Notifications transaction
	//
	
	PlayerTransGetNotificationList::PlayerTransGetNotificationList(IHeap& heap, MessageDataPtr &pIMsg)
		: MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg) )
	{
		RegisterMessageHandler<DB::QueryNotification_GetListCmd>(&PlayerTransGetNotificationList::OnGetList);
	}
	
	Result PlayerTransGetNotificationList::OnGetList(Svr::TransactionResult* pRes)
	{
		ScopeContext hr;
		DB::RowsetList::iterator itNotification;
		UserNotificationSystem* pNotifySystem = GetMyOwner()->GetComponent<UserNotificationSystem>();
		auto*pDBRes = pRes->GetResultData<DB::QueryNotification_GetListCmd>();

		svrCheck(pRes->GetResult());

		pNotifySystem->ClearNotificationList();

		itNotification = pDBRes->RowsetResults.begin();
		for( ; itNotification != pDBRes->RowsetResults.end(); ++itNotification )
		{
			auto NotificationID = itNotification->GetValue<uint32_t>("NotificationID"_crc);
			auto MessageID = (NotificationType)itNotification->GetValue<uint16_t>("MessageID"_crc);
			auto MessageParam0 = itNotification->GetValue<uint64_t>("MessageParam0"_crc);
			auto MessageParam1 = itNotification->GetValue<uint64_t>("MessageParam1"_crc);
			auto MessageText = itNotification->GetValue<String>("MessageText"_crc);
			auto IsRead = itNotification->GetValue<uint8_t>("IsRead"_crc);
			auto TimeStamp = itNotification->GetValue<uint64_t>("TimeStamp"_crc);

			svrCheck( pNotifySystem->AddNotification(NotificationID, MessageID, MessageParam0, MessageParam1, MessageText, IsRead, TimeStamp) );

			NetSvrPolicyGame(GetRemoteEndpoint()).NotifyS2CEvt(NotificationID, (int32_t)MessageID, MessageParam0, MessageParam1, MessageText, IsRead, TimeStamp );
		}

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransGetNotificationList::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!(hr))
					CloseTransaction(hr);
			});

		svrCheck( super::StartTransaction() );

		svrCheck(Svr::GetServerComponent<DB::GameDB>()->Notification_GetList(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetAccountID()));

		return hr;
	}
	
	
	PlayerTransDeleteNotification::PlayerTransDeleteNotification(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg))
	{
		BR_TRANS_MESSAGE( DB::QueryNotification_RemoveCmd, { return OnDeletedNotification(pRes); });
	}

	Result PlayerTransDeleteNotification::OnDeletedNotification( Svr::TransactionResult* &pRes )
	{
		ScopeContext hr;

		svrCheck(pRes->GetResult());

		svrCheck( GetMyOwner()->GetComponent<UserNotificationSystem>()->RemoveNotification(GetNotificationID()) );

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransDeleteNotification::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});

		svrCheck(super::StartTransaction());

		svrCheck(Svr::GetServerComponent<DB::GameDB>()->Notification_Remove(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(), GetNotificationID()));

		return hr;
	}


	PlayerTransSetNotificationRead::PlayerTransSetNotificationRead(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction( heap, std::forward<MessageDataPtr>(pIMsg))
	{
		RegisterMessageHandler<DB::QueryNotification_SetReadCmd>(&PlayerTransSetNotificationRead::OnSetRead);
	}

	Result PlayerTransSetNotificationRead::OnSetRead( Svr::TransactionResult* pRes )
	{
		ScopeContext hr;

		UserNotificationSystem::Notification *pNotify = nullptr;

		svrCheck(pRes->GetResult());


		svrCheckPtr( pNotify = GetMyOwner()->GetComponent<UserNotificationSystem>()->GetNotification(GetNotificationID()) );
		pNotify->IsRead = TRUE;

		switch( pNotify->MessageID )
		{
		case NotificationType::GiftStamina:
			// TODO: FIXME
			//GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GainStamina(1);
			//svrCheck(GetMyOwner()->UpdateDBSync(GetTransID()));
			break;
		default:
			break;
		}

		return hr; 
	}


	// Start Transaction
	Result PlayerTransSetNotificationRead::StartTransaction()
	{
		ScopeContext hr;

		svrCheck( super::StartTransaction() );

		if( GetMyOwner()->GetComponent<UserNotificationSystem>()->GetNotification(GetNotificationID()) == nullptr )
			svrErrorClose(ResultCode::INVALID_NOTIFICATIONID);

		svrCheck(Svr::GetServerComponent<DB::GameDB>()->Notification_SetRead(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(), GetNotificationID()));

		return hr;
	}


	PlayerTransAcceptNotification::PlayerTransAcceptNotification(IHeap& heap, MessageDataPtr &pIMsg)
		: MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg))
	{
		RegisterMessageHandler<DB::QueryNotification_RemoveCmd>(&PlayerTransAcceptNotification::OnDeletedNotification);
	}

	Result PlayerTransAcceptNotification::OnDeletedNotification(Svr::TransactionResult* pRes)
	{
		ScopeContext hr;

		auto notification = GetMyOwner()->GetComponent<UserNotificationSystem>()->GetNotification(GetNotificationID());

		svrCheck(pRes->GetResult());

		if (notification!= nullptr)
		{
			Result hrRes = ResultCode::SUCCESS;
			// accept notification
			// Friend will be processed separately
			// for now, only stamina is processed here
			switch (notification->MessageID)
			{
			case NotificationType::GiftStamina:
				// TODO: FIXME
				//hrRes = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GainStamina(1);
				//if (!(hrRes))
				//{
				//	svrTrace(Warning, "Stamina gain is failed, PlayerID:{0}, hr:{1:X8}", GetMyOwner()->GetPlayerID(), hrRes);
				//}
				break;
			default:
				break;
			}
		}
		svrCheck(GetMyOwner()->GetComponent<UserNotificationSystem>()->RemoveNotification(GetNotificationID()));

		return ResultCode::SUCCESS;
	}

	// Start Transaction
	Result PlayerTransAcceptNotification::StartTransaction()
	{
		ScopeContext hr;

		svrCheck(super::StartTransaction());

		svrCheck(Svr::GetServerComponent<DB::GameDB>()->Notification_Remove(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(), GetNotificationID()));

		return hr;
	}

	
	// Start Transaction
	Result PlayerTransNotifyS2S::StartTransaction()
	{
		ScopeContext hr;

		svrCheck( super::StartTransaction() );

		if( GetMyOwner()->GetPlayerID() != GetDestPlayerID() )
			svrError(ResultCode::GAME_INVALID_PLAYER);

		svrCheck( GetMyOwner()->GetComponent<UserNotificationSystem>()->AddNotification(GetNotificationID(), (NotificationType)GetMessageID(), GetMessageParam0(), GetMessageParam1(), GetMessageText(), 0, GetTimeStamp() ) );
		svrCheck(NetSvrPolicyGame(GetRemoteEndpoint()).NotifyS2CEvt(GetNotificationID(), (uint)GetMessageID(), GetMessageParam0(), GetMessageParam1(), GetMessageText(), 0, GetTimeStamp()));

		return hr;
	}


	/////////////////////////////////////////////////////////////////////////////
	//
	//	NickName transaction
	//


	PlayerTransSetNickName::PlayerTransSetNickName(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg))
	{
		RegisterMessageHandler<DB::QuerySetNickNameCmd>(&PlayerTransSetNickName::OnNickChanged);
	}

	Result PlayerTransSetNickName::OnNickChanged(Svr::TransactionResult* pRes)
	{
		ScopeContext hr;

		auto pDBRes = pRes->GetResultData<DB::QuerySetNickNameCmd>();
		//UserGamePlayerInfoSystem* pPlayerInfoSystem = nullptr;
		//conspiracy::OrganicTbl::OrganicItem *pCostItem = nullptr;

		svrCheck(pRes->GetResult());

		if (pDBRes->Result < 0)
			svrErrorClose(ResultCode::INVALID_PLAYERID);

		GetMyOwner()->AddGameTransactionLogT(TransLogCategory::Account, 0, 0, 0, "From {0} to {1}", GetMyOwner()->GetNickName(), GetNickName());

		svrCheck( GetMyOwner()->SetNickName( GetNickName() ) );

		//svrCheckPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		if (GetIsCostFree() == 0)
		{
			//svrChkCloseErr(ResultCode::GAME_INVALID_COSTID, conspiracy::OrganicTbl::FindItem((int)conspiracy::OrganicTbl::EItemEffect::Enum::NickName, pCostItem));
			//svrChkCloseErr(ResultCode::GAME_NOTENOUGH_RESOURCE, pPlayerInfoSystem->ApplyCost(pCostItem, TransLogCategory::Buy, "SetNickName"));
		}

		m_TotalGem = 0;
		m_TotalGameMoney = 0;

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransSetNickName::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});

		//conspiracy::OrganicTbl::OrganicItem *pCostItem = nullptr;
		//UserGamePlayerInfoSystem* pPlayerInfoSystem = nullptr;

		m_TotalGem = 0;
		m_TotalGameMoney = 0;

		svrCheck( super::StartTransaction() );

		//svrCheckPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		//if (GetIsCostFree() == 0)
		//{
		//	svrChkCloseErr(ResultCode::GAME_INVALID_COSTID, conspiracy::OrganicTbl::FindItem((int)conspiracy::OrganicTbl::EItemEffect::Enum::NickName, pCostItem));
		//	svrChkCloseErr(ResultCode::GAME_NOTENOUGH_RESOURCE, pPlayerInfoSystem->CheckCost(pCostItem));
		//}

		svrCheck(Svr::GetServerComponent<DB::GameDB>()->SetNickName(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(), GetNickName()));

		return hr;
	}
	
	

	/////////////////////////////////////////////////////////////////////////////
	//
	//	Find player transaction
	//

	PlayerTransFindPlayerByEMail::PlayerTransFindPlayerByEMail(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg))
	{
		RegisterMessageHandler<DB::QueryFindPlayerByEMailCmd>(&PlayerTransFindPlayerByEMail::OnFindPlayer);
		RegisterMessageHandler<DB::QueryGetNickNameCmd>(&PlayerTransFindPlayerByEMail::OnGetNickName);
	}

	Result PlayerTransFindPlayerByEMail::OnFindPlayer( Svr::TransactionResult* pRes )
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});

		DB::QueryFindPlayerByEMailCmd *pDBRes = (DB::QueryFindPlayerByEMailCmd*)pRes;

		svrCheck(pRes->GetResult());

		if( pDBRes->UserID == 0 )
			svrErrorClose(ResultCode::SVR_PLAYER_NOT_FOUND);

		m_Player.PlayerID = pDBRes->UserID;
		m_Player.FBUID = pDBRes->FacebookUID;
		m_PlayerShardID = pDBRes->ShardID;

		svrCheck(Svr::GetServerComponent<DB::GameDB>()->GetNickName(GetTransID(), m_PlayerShardID, m_Player.PlayerID));

		return ResultCode::SUCCESS; 
	}

	Result PlayerTransFindPlayerByEMail::OnGetNickName(Svr::TransactionResult* pRes)
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		auto *pDBRes = (DB::QueryGetNickNameCmd*)pRes;

		svrCheck(pRes->GetResult());

		if (pDBRes->Result < 0)
			svrErrorClose(ResultCode::SVR_PLAYER_NOT_FOUND);

		StrUtil::StringCopy( m_Player.NickName, pDBRes->NickName );

		return ResultCode::SUCCESS;
	}

	// Start Transaction
	Result PlayerTransFindPlayerByEMail::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});

		char email[GameConst::MAX_EMAIL];

		svrCheck( super::StartTransaction() );

		svrCheck( StrUtil::StringLower( email, GetPlayerEMail() ) );

		svrCheck( Svr::GetServerComponent<DB::AccountDB>()->FindPlayerByEMail( GetTransID(), GetPlayerEMail() ) );

		return hr;
	}
	

	PlayerTransFindPlayerByPlayerID::PlayerTransFindPlayerByPlayerID(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg))
	{
		BR_TRANS_MESSAGE(DB::QueryFindPlayerByPlayerIDCmd, { return OnFindPlayer(pRes); });
		BR_TRANS_MESSAGE(DB::QueryGetNickNameCmd, { return OnGetNickName(pRes); });
	}

	Result PlayerTransFindPlayerByPlayerID::OnFindPlayer(Svr::TransactionResult* &pRes)
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});

		auto pDBRes = (DB::QueryFindPlayerByPlayerIDCmd*)pRes;
		svrCheck(pRes->GetResult());

		if (pDBRes->PlayerID == 0)
			svrErrorClose(ResultCode::SVR_PLAYER_NOT_FOUND);

		if (pDBRes->Result != 0)
			svrErrorClose(ResultCode::SVR_PLAYER_NOT_FOUND);

		m_Player.PlayerID = pDBRes->PlayerID;
		m_Player.FBUID = pDBRes->FacebookUID;
		m_PlayerShardID = pDBRes->ShardID;

		svrCheck(Svr::GetServerComponent<DB::GameDB>()->GetNickName(GetTransID(), m_PlayerShardID, m_Player.PlayerID));

		return ResultCode::SUCCESS;
	}

	Result PlayerTransFindPlayerByPlayerID::OnGetNickName(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		auto *pDBRes = (DB::QueryGetNickNameCmd*)pRes;

		svrCheck(pRes->GetResult());

		if (pDBRes->Result < 0)
			svrErrorClose(ResultCode::SVR_PLAYER_NOT_FOUND);

		StrUtil::StringCopy( m_Player.NickName, pDBRes->NickName );

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}

	// Start Transaction
	Result PlayerTransFindPlayerByPlayerID::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});


		svrCheck( super::StartTransaction() );

		svrCheck( Svr::GetServerComponent<DB::AccountDB>()->FindPlayerByPlayerID( GetTransID(), GetPlayerID() ) );


		return hr;
	}
	
	
	PlayerTransRequestPlayerStatusUpdate::PlayerTransRequestPlayerStatusUpdate(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg))
	{
		BR_TRANS_MESSAGE(DB::QueryGetPlayerShardIDCmd, { return OnPlayerShardIDRes(pRes); });
		BR_TRANS_MESSAGE(DB::QueryGetPlayerStatusCmd, { return OnPlayerStatusUpdateRes(pRes); });
	}

	Result PlayerTransRequestPlayerStatusUpdate::OnPlayerShardIDRes(Svr::TransactionResult* &pRes)
	{
		ScopeContext hr([this](Result hr)
			{
				if (m_PlayerStatusQueryCount == 0)
					CloseTransaction(hr);
			});

		//NetSvrPolicyGame *pPolicy = nullptr;
		auto *pDBRes = (DB::QueryGetPlayerShardIDCmd*)pRes;

		m_PlayerStatusQueryCount--;

		svrCheck(pRes->GetResult());

		svrCheck(Svr::GetServerComponent<DB::GameDB>()->GetPlayerStatusCmd(GetTransID(), pDBRes->ShardID, pDBRes->UserID));
		m_PlayerStatusQueryCount++;

		return ResultCode::SUCCESS;
	}

	Result PlayerTransRequestPlayerStatusUpdate::OnPlayerStatusUpdateRes( Svr::TransactionResult* &pRes )
	{
		ScopeContext hr([this](Result hr)
			{
				if (m_PlayerStatusQueryCount == 0)
					CloseTransaction(hr);
			});
		NetSvrPolicyGame *pPolicy = nullptr;
		DB::QueryGetPlayerStatusCmd *pDBRes = (DB::QueryGetPlayerStatusCmd*)pRes;

		m_PlayerStatusQueryCount--;

		svrCheck(pRes->GetResult());

		svrCheck(NetSvrPolicyGame(GetRemoteEndpoint()).NotifyPlayerStatusUpdatedS2CEvt(pDBRes->PlayerID, pDBRes->LatestActiveTime, pDBRes->PlayerState != 0 ? 1 : 0));

		return ResultCode::SUCCESS;
	}

	// Start Transaction
	Result PlayerTransRequestPlayerStatusUpdate::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				if (m_PlayerStatusQueryCount == 0)
					CloseTransaction(hr);
			});
		EntityUID playerUID;
		NetSvrPolicyGame pPolicy(GetRemoteEndpoint());
		auto& targetPlayerID = GetTargetPlayerID();
		uint uiRequestMax = Util::Min((uint)targetPlayerID.size(), (uint)20);

		m_PlayerStatusQueryCount = 0;

		svrCheck( super::StartTransaction() );
		

		for( uint iPlayer = 0; iPlayer < uiRequestMax; iPlayer++ )
		{
			if( ( Service::PlayerManager->FindPlayer( targetPlayerID[iPlayer], playerUID )) )
			{
				// Now we know he is online we can check with DB
				auto pFriend = GetMyOwner()->GetComponent<UserFriendSystem>()->GetFriend(targetPlayerID[iPlayer]);
				if (pFriend != nullptr)
				{
					svrCheck(Svr::GetServerComponent<DB::GameDB>()->GetPlayerStatusCmd(GetTransID(), pFriend->ShardID, targetPlayerID[iPlayer]));
				}
				else
				{
					svrCheck(Svr::GetServerComponent<DB::AccountDB>()->GetPlayerShardID(GetTransID(), targetPlayerID[iPlayer]));
				}
				m_PlayerStatusQueryCount++;
			}
			else
			{
				svrCheck(pPolicy.NotifyPlayerStatusUpdatedS2CEvt(targetPlayerID[iPlayer], 0, 0));
			}
		}


		return hr;
	}
	
	
	// Start Transaction
	Result PlayerTransRequestPlayerStatusUpdateC2S::StartTransaction()
	{
		ScopeContext hr;
		Svr::ServerEntity *pServerEntity = nullptr;
		EntityUID playerUID;
		bool bInGame;


		svrCheck( super::StartTransaction() );

		if( GetMyOwner()->GetPlayerID() != GetDestPlayerID() )
			svrError(ResultCode::GAME_INVALID_PLAYER);

		playerUID = GetRouteContext().GetFrom();

		auto playerEndpoint = Service::MessageEndpointManager->GetEndpoint(playerUID);
		//svrCheck( Service::ServerEntityManager->GetServerEntity( playerUID.GetServerID(), pServerEntity ) );

		bInGame = GetMyOwner()->GetGameInsUID().UID != 0 || GetMyOwner()->GetPartyUID().UID != 0;

		svrCheck(NetPolicyGameServer(playerEndpoint).NotifyPlayerStatusUpdatedC2SEvt(
			RouteContext(GetOwnerEntityUID(),playerUID), 
			GetDestPlayerID(), GetMyOwner()->GetLatestActiveTime().time_since_epoch().count(), bInGame ? 1 : 0 ) );

		return hr;
	}
	
	// Start Transaction
	Result PlayerTransNotifyPlayerStatusUpdatedS2S::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		svrCheck( super::StartTransaction() );

		svrCheck(NetSvrPolicyGame(GetRemoteEndpoint()).NotifyPlayerStatusUpdatedS2CEvt( GetDestPlayerID(), GetLatestActiveTime(), GetIsInGame() ) );

		return hr;
	}




	PlayerTransGetRankingList::PlayerTransGetRankingList(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg))
		, m_RankingList(heap)
	{
		BR_TRANS_MESSAGE( DB::QueryGetTotalRankingCmd, { return OnGetRankingListRes(pRes); });
	}

	Result PlayerTransGetRankingList::OnGetRankingListRes( Svr::TransactionResult* &pRes )
	{
		ScopeContext hr;
		auto*pDBRes = pRes->GetResultData<DB::QueryGetTotalRankingCmd>();

		svrCheck(pRes->GetResult());

		m_RankingList.reserve( pDBRes->RowsetResults.size() );
		for (auto itRowSet = pDBRes->RowsetResults.begin(); itRowSet != pDBRes->RowsetResults.end(); ++itRowSet)
		{
			auto RankingID = itRowSet->GetValue<int32_t>("RankingID"_crc);
			auto Ranking = itRowSet->GetValue<int32_t>("Ranking"_crc);
			auto WinRate = itRowSet->GetValue<float>("WinRate"_crc);
			auto Win = itRowSet->GetValue<int32_t>("Win"_crc);
			auto Lose = itRowSet->GetValue<int32_t>("Lose"_crc);
			auto PlayerID = itRowSet->GetValue<int64_t>("PlayerID"_crc);
			auto FBUID = itRowSet->GetValue<int64_t>("FBUID"_crc);
			auto NickName = itRowSet->GetValue<String>("NickName"_crc);
			auto Level = itRowSet->GetValue<int32_t>("Level"_crc);

			TotalRankingPlayerInformation info( RankingID, Ranking, PlayerID, FBUID, NickName, Level, Win, Lose );

			m_RankingList.push_back(info);
		};

		return hr; 
	}

	// Start Transaction
	Result PlayerTransGetRankingList::StartTransaction()
	{
		ScopeContext hr;

		svrCheck( super::StartTransaction() );

		if (GetBaseRanking() <= 0 || GetBaseRanking() > GameConst::MAX_RANKING_QUERY_COUNT)
			svrErrorClose(ResultCode::INVALID_RANKING_RANGE);

		if (GetCount() <= 0 || GetCount() > GameConst::MAX_RANKING_QUERY_COUNT)
			svrErrorClose(ResultCode::INVALID_RANKING_RANGE);
		
		svrCheck( Svr::GetServerComponent<DB::RankingDB>()->GetRankingListCmd( GetTransID(), GetBaseRanking(), GetCount() ) );

		return hr;
	}



	PlayerTransBuyShopItemPrepare::PlayerTransBuyShopItemPrepare(IHeap& heap, MessageDataPtr &pIMsg)
		: MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg))
		, m_Signagure(heap)
	{
		RegisterMessageHandler<DB::QueryCheckPurchaseIDCmd>(&PlayerTransBuyShopItemPrepare::OnPurchaseIDChecked);
		m_Signagure.push_back('\0');
	}

	Result PlayerTransBuyShopItemPrepare::OnPurchaseIDChecked(Svr::TransactionResult* pRes)
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});
		auto *pDBRes = (DB::QueryCheckPurchaseIDCmd*)pRes;

		svrCheck(pRes->GetResult());

		if (pDBRes->Result < 0)
		{
			m_RetryCount++;
			if (m_RetryCount > MAX_RETRY)
			{
				svrErrorClose(ResultCode::FAIL);
			}
			else
			{
				svrCheck(GenerateSigunatureAndCheck());
			}
		}
		else
		{
			CloseTransaction(hr);
		}

		return ResultCode::SUCCESS;
	}

	Result PlayerTransBuyShopItemPrepare::GenerateSigunatureAndCheck()
	{
		ScopeContext hr([this](Result hr)
			{
				m_Signagure.push_back('\0');
			});

		StaticArray<uint8_t, 1024> dataBuffer(GetHeap());
		StaticArray<uint8_t, 128> hash(GetHeap());
		//AuthTicket authTicket = GetMyOwner()->GetAuthTicket();
		PlayerID playerID = GetMyOwner()->GetPlayerID();
		auto time = Util::Time.GetTimeUTCSec();
		auto time2 = Util::Time.GetTimeMs();

		m_Signagure.Clear();

		// Generate signature
		svrCheck(dataBuffer.push_back(sizeof(playerID), (const uint8_t*)&playerID));
		svrCheck(dataBuffer.push_back(sizeof(time), (const uint8_t*)&time));
		svrCheck(dataBuffer.push_back(sizeof(time2), (const uint8_t*)&time2));

		svrCheck(Util::SHA256Hash(dataBuffer.size(), dataBuffer.data(), hash));
		svrCheck(Util::Base64URLEncode(hash.size(), hash.data(), m_Signagure));

		svrCheck(Svr::GetServerComponent<DB::GameDB>()->CheckPurchaseID(GetTransID(), GetMyOwner()->GetShardID(), hash));


		return hr;
	}

	// Start Transaction
	Result PlayerTransBuyShopItemPrepare::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
				{
					CloseTransaction(hr);
				}
			});


		svrCheck(super::StartTransaction());

		m_RetryCount = 0;

		svrCheck(GenerateSigunatureAndCheck());

		return hr;
	}


	constexpr size_t PlayerTransBuyShopItem::MEMENTO_SIZE;

	PlayerTransBuyShopItem::PlayerTransBuyShopItem(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg) )
		, m_SavedData(heap)
	{
		SetExclusive(true);

		BR_TRANS_MESSAGE(Svr::ExternalTransactionGoogleAndroidReceiptCheck, { return OnPurchaseCheckedAndroid(pRes); });
		BR_TRANS_MESSAGE(Svr::ExternalTransactionIOSRecepitCheck, { return OnPurchaseCheckedIOS(pRes); });
		BR_TRANS_MESSAGE(DB::QuerySavePurchaseInfoToDBCmd, { return OnSavedToDB(pRes); });
	}


	Result PlayerTransBuyShopItem::OnPurchaseCheckedAndroid(Svr::TransactionResult* &pRes)
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
				{
					CloseTransaction(hr);
				}
			});

		auto *pCheckRes = (Svr::ExternalTransactionGoogleAndroidReceiptCheck*)pRes;
		StaticArray<uint8_t, 512> purchaseID(GetHeap());

		svrCheckClose(pRes->GetResult());

		if (pCheckRes->GetDeveloperPayload().length() == 0)
			svrErrorClose(ResultCode::SVR_INVALID_PURCHASE_INFO);

		svrCheckCloseErr(ResultCode::SVR_INVALID_PURCHASE_INFO, Util::Base64URLDecode(pCheckRes->GetDeveloperPayload().length(), (const uint8_t*)pCheckRes->GetDeveloperPayload().c_str(), purchaseID));

		if (purchaseID.size() != SHA256_DIGEST_LENGTH)
			svrErrorClose(ResultCode::SVR_INVALID_PURCHASE_INFO);

		auto& playerData = GetMyOwner()->GetPlayerData();

		// TODO: FIXME
		//svrCheck(pPlayerInfoSystem->SaveStatToMemento(m_SavedData));

		//svrChkClose(pPlayerInfoSystem->ApplyItem(m_pShopItem));

		//svrCheck(pPlayerInfoSystem->SavePurchaseInfoToDB(GetTransID(), purchaseID, GetPlatform(), GetPurchaseTransactionID()));

		return ResultCode::SUCCESS;
	}

	Result PlayerTransBuyShopItem::OnPurchaseCheckedIOS(Svr::TransactionResult* &pRes)
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
				{
					CloseTransaction(hr);
				}
			});
		auto *pCheckRes = (Svr::ExternalTransactionIOSRecepitCheck*)pRes;

		svrCheckClose(pRes->GetResult());

		if (pCheckRes->GetPurchaseTransactionID().size() == 0
			|| !StrUtil::StringCompair((const char*)pCheckRes->GetPurchaseTransactionID().data(), (INT)pCheckRes->GetPurchaseTransactionID().size(), GetPurchaseTransactionID(), -1))
			svrErrorClose(ResultCode::SVR_INVALID_PURCHASE_INFO);


		auto& playerData = GetMyOwner()->GetPlayerData();

		// TODO:FIXME
		//svrCheck(pPlayerInfoSystem->SaveStatToMemento(m_SavedData));

		//svrCheckClose(pPlayerInfoSystem->ApplyItem(m_pShopItem));

		//svrCheck(pPlayerInfoSystem->SavePurchaseInfoToDB(GetTransID(), pCheckRes->GetPurchaseTransactionID(), GetPlatform(), GetPurchaseTransactionID()));

		return hr;
	}

	Result PlayerTransBuyShopItem::OnSavedToDB( Svr::TransactionResult* &pRes )
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
				{
					m_SavedData.RestoreAllData();
				}
				CloseTransaction(hr);
			});
		auto *pDBRes = (DB::QuerySavePurchaseInfoToDBCmd*)pRes;

		svrCheck(pRes->GetResult());

		if (pDBRes->Result < 0)
		{
			svrErrorClose(ResultCode::SVR_INVALID_PURCHASE_DUPLICATED);
		}


		return hr; 
	}

	// Start Transaction
	Result PlayerTransBuyShopItem::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
				{
					CloseTransaction(hr);
				}
			});
		Svr::ExternalTransactionManager* pExtMgr = nullptr;

		m_pShopItem = nullptr;

		svrCheck( super::StartTransaction() );

		if (GetPurchaseTransactionID() == nullptr || GetPurchaseTransactionID()[0] == '\0')
		{
			svrErrorClose(ResultCode::SVR_INVALID_PURCHASE_INFO);
		}

		//if( !( conspiracy::ShopTbl::FindItem( GetShopItemID(), m_pShopItem ) ) )
		//{
		//	svrErrorClose(ResultCode::GAME_INVALID_SHOPITEMID);
		//}

		svrCheckPtr(pExtMgr = Svr::GetServerComponent<Svr::ExternalTransactionManager>());

		// TODO:FIXME
		//if (strlen(m_pShopItem->AndroidItemID) == 0 || strlen(m_pShopItem->iOSItemID) == 0)
		//{
		//	StaticArray<uint8_t,1024> pruchaseID(GetHeap());

		//	svrCheck( Util::Base64URLDecode(strlen(GetPurchaseTransactionID()), (uint8_t*)GetPurchaseTransactionID(), pruchaseID) );

			//svrCheckPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

			//svrCheck(pPlayerInfoSystem->SaveStatToMemento(m_SavedData));

			//svrCheckClose(pPlayerInfoSystem->ApplyItem(m_pShopItem));

			//svrCheck(pPlayerInfoSystem->SavePurchaseInfoToDB(GetTransID(), pruchaseID, GetPlatform(), GetPurchaseTransactionID()));
		//}
		//else
		//{
		//	if (StrUtil::StringCompairIgnoreCase("android", -1, GetPlatform(), -1)
		//		|| StrUtil::StringCompairIgnoreCase("windows", -1, GetPlatform(), -1))
		//	{
		//		// The last byte must be null, null terminate
		//		svrCheck(pExtMgr->AndroidCheckReceipt(GetTransID(), GetPackageName(), m_pShopItem->AndroidItemID, GetPurchaseTransactionID()));
		//	}
		//	else
		//	{
		//		if (GetPurchaseToken().size() == 0)
		//		{
		//			svrErrorClose(ResultCode::SVR_INVALID_PURCHASE_INFO);
		//		}

		//		svrCheck(pExtMgr->IOSCheckReceipt(GetTransID(), GetPackageName(), m_pShopItem->iOSItemID, GetPurchaseTransactionID(), GetPurchaseToken()));
		//	}
		//}


		return hr;
	}
	
	


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Debug
	//


	// Start Transaction
	Result PlayerTransSetConfigPreset::StartTransaction()
	{
		ScopeContext hr;
		NetPolicyGameInstance *pPolicy = nullptr;
		GameInsUID insUID;

		svrCheck( super::StartTransaction() );

		// TODO: FIXME
		//svrCheck( GetMyServer()->UpdateGameConfig(GetPresetID()) );

		svrCheck( GetMyOwner()->UpdateGameConfig() );

		insUID = GetMyOwner()->GetGameInsUID();
		if( insUID.UID != 0 )
		{
			auto gameEndpoint = Service::MessageEndpointManager->GetEndpoint(insUID);
			svrCheck(NetPolicyGameInstance(gameEndpoint).SetConfigPresetC2SEvt( RouteContext(GetOwnerEntityUID(),insUID), GetPresetID() ) );
		}

		return hr;
	}





} // namespace GameServer 
} // namespace SF 

