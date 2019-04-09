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
#include "Memory/SFMemoryPool.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"

#include "Net/SFNetServerUDP.h"

#include "GameServerClass.h"
#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "ServerEntity/ServerEntityManager.h"

#include "ServiceEntity/Game/PlayerManagerServiceEntity.h"
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
#include "GameInstance/GamePlayerEntity.h"

#include "Server/BrServer.h"


#include "GameSystem/UserFriendSystem.h"
#include "GameSystem/UserGamePlayerInfoSystem.h"
#include "GameSystem/UserNotifySystem.h"


#include "DB/AccountDB.h"
#include "DB/AccountQuery.h"
#include "DB/GameConspiracyDB.h"
#include "DB/GameConspiracyQuery.h"
#include "DB/RankingDB.h"
#include "DB/RankingDBQuery.h"

#include "Table/conspiracy/GameConfigTbl.h"
#include "Table/conspiracy/OrganicTbl.h"

//#include "openssl/sha.h"
#define 	SHA256_DIGEST_LENGTH   32



SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransJoinGameServer);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransGetUserGamePlayerInfo);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransGetGamePlayerInfo);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransGetComplitionState);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransSetComplitionState);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransRegisterGCM);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransUnregisterGCM);

SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransGetNotificationList);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransDeleteNotification);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransAcceptNotification);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransNotifyS2S);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransSetNotificationRead);


SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransSetNickName);

SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransFindPlayerByEMail);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransFindPlayerByPlayerID);

SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransRequestPlayerStatusUpdate);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransRequestPlayerStatusUpdateS2S);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransNotifyPlayerStatusUpdatedS2S);

SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransGetRankingList);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransBuyShopItemPrepare);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransBuyShopItem);

SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransSetConfigPreset);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransGainGameResource);

	
namespace SF {
namespace GameServer {


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Game command transaction
	//


	PlayerTransJoinGameServer::PlayerTransJoinGameServer(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction(heap, pIMsg )
	{
		SetExclusive(true);

		BR_TRANS_MESSAGE(DB::QueryCreatePlayerInfoCmd, { return OnCreatePlayerGameDataRes(pRes); });
		BR_TRANS_MESSAGE(DB::QueryGetPlayerInfoCmd, { return OnGetPlayerGameDataRes(pRes); });
		BR_TRANS_MESSAGE( Message::LoginServer::PlayerJoinedToGameServerRes, { return OnGameServerJoined(pRes); });
		BR_TRANS_MESSAGE( Message::GameParty::JoinPartyRes, { return OnJoinPartyRes(pRes); });
	}

	Result PlayerTransJoinGameServer::OnGameServerJoined( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		Message::LoginServer::PlayerJoinedToGameServerRes msgRes;

		// TODO: We need to close this entity on error
		svrChkClose(pRes->GetResult());
		svrChk( msgRes.ParseMessage( *((Svr::MessageResult*)pRes)->GetMessage() ) );

		// succeeded to create
		svrChk( RegisterToPlayerManager() );

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->GetPlayerInfoCmd(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID()));

	Proc_End:

		if( !(hr) )
			CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	Result PlayerTransJoinGameServer::OnJoinPartyRes( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		Message::GameParty::JoinPartyRes msgRes;

		hr = pRes->GetResult();
		if (!(hr))
		{
			m_PartyLeaderID = 0;
			goto Proc_End;
		}

		svrChk( msgRes.ParseMessage( *((Svr::MessageResult*)pRes)->GetMessage() ) );

		m_PartyLeaderID = msgRes.GetPartyLeaderID();


	Proc_End:

		if( !(hr) )
			GetMyOwner()->SetPartyUID(0);

		GetMyOwner()->UpdateDBSync();

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}

	Result PlayerTransJoinGameServer::SetPlayerGameData(const DB::QueryGetPlayerInfoData &playerData)
	{
		Result hr = ResultCode::SUCCESS;
		auto playerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>();

		// TODO: Need to change initial nick name setup process
		GetMyOwner()->SetNickName(playerData.GameNick);

		playerInfoSystem->SetGrade(playerData.Grade);
		playerInfoSystem->SetLevel(playerData.Level);
		playerInfoSystem->SetExp(playerData.Exp);
		playerInfoSystem->SetGameMoney(playerData.GameMoney);
		playerInfoSystem->SetGem(playerData.Gem);
		playerInfoSystem->SetStamina(playerData.Stamina);
		playerInfoSystem->SetAddedFriendSlot(playerData.AddedFriendSlot);
		playerInfoSystem->SetTotalPlayed(playerData.TotalPlayed);

		playerInfoSystem->SetWinPlaySCitizen(playerData.WinPlaySC);
		playerInfoSystem->SetWinPlaySMonster(playerData.WinPlaySM);
		playerInfoSystem->SetWinPlaySSeer(playerData.WinPlaySS);
		playerInfoSystem->SetLosePlaySCitizen(playerData.LosePlaySC);
		playerInfoSystem->SetLosePlaySMonster(playerData.LosePlaySM);
		playerInfoSystem->SetLosePlaySSeer(playerData.LosePlaySS);

		playerInfoSystem->SetWinPlayNCitizen(playerData.WinPlayNC);
		playerInfoSystem->SetWinPlayNMonster(playerData.WinPlayNM);
		playerInfoSystem->SetWinPlayNSeer(playerData.WinPlayNS);
		playerInfoSystem->SetLosePlayNCitizen(playerData.LosePlayNC);
		playerInfoSystem->SetLosePlayNMonster(playerData.LosePlayNM);
		playerInfoSystem->SetLosePlayNSeer(playerData.LosePlayNS);

		playerInfoSystem->SetWeeklyWin(playerData.WeeklyPlayWin);
		playerInfoSystem->SetWeeklyLose(playerData.WeeklyPlayLose);

		playerInfoSystem->UpdateStatMaximum();
		playerInfoSystem->UpdateStatByLevel();


		svrTrace(SVR_DBGTRANS, "SetPlayerGameData PlayerID:{0}. Grade:{1}, lvl:{2}, Exp:{3}, GameMoney:{4}, Gem:{5}, Sta:{6}, updateTick:{7}", GetMyOwner()->GetPlayerID(),
			playerData.Grade, playerData.Level, playerData.Exp, playerData.GameMoney, playerData.Gem, playerData.Stamina,
			(uint64_t)playerData.LatestTickTime);

		auto latestTick = playerData.LatestTickTime;

		GetMyOwner()->SetLatestActiveTime(Util::Time.GetTimeUTCSec());

		// If first login, the value will be zero
		if (latestTick == 0)
		{
			svrTrace(Warning, "Invalid player tick time for PlayerID:{0}. Reset to current time", GetMyOwner()->GetPlayerID());
			GetMyOwner()->SetLatestUpdateTime(Util::Time.GetTimeUTCSec());
		}
		else
		{
			svrTrace(SVR_TRANSACTION, "Latest tick time PlayerID:{0}. {1}", GetMyOwner()->GetPlayerID(), (uint64_t)playerData.LatestTickTime);

			GetMyOwner()->SetLatestUpdateTime(TimeStampSec(DurationSec(playerData.LatestTickTime)));
		}

		GetMyOwner()->AddGameTransactionLog(TransLogCategory::Account, 1, 0, 0, "Login");
		

		// copy to return information
		StrUtil::StringCopy(m_PlayerNick, GetMyOwner()->GetNickName());
		m_MatchingTicket = GetMyOwner()->GetMatchingTicket();
		m_GameUID = GetMyOwner()->GetGameInsUID();


		if (GetMyOwner()->GetPartyUID().UID != 0)
		{
			PartyUID partyUID = GetMyOwner()->GetPartyUID();

			svrChk(Policy::NetPolicyGameParty(Service::ServerEntityManager->GetServerConnection(partyUID.GetServerID())).JoinPartyCmd(
				RouteContext(GetOwnerEntityUID(), partyUID), GetTransID(), 
				0, GetMyOwner()->GetPlayerInformation()));
		}
		else
		{
			GetMyOwner()->UpdateDBSync();
			CloseTransaction(hr);
		}

	Proc_End:

		return ResultCode::SUCCESS;
	}

	Result PlayerTransJoinGameServer::OnCreatePlayerGameDataRes(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		auto pDBRes = (DB::QueryCreatePlayerInfoCmd*)pRes;

		svrChk(pRes->GetResult());

		// succeeded to query
		if( pDBRes->Result < 0 )
		{
			svrErr(ResultCode::SVR_INVALID_PLAYER_GAMEDB);
		}

		svrAssert( pDBRes->m_RowsetResult.size() >= 1 );
		{
			auto& playerInfoData = *pDBRes->m_RowsetResult.begin();
			auto playerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>();

			// New player data. Reset stat to default
			if (pDBRes->Result == 0)
			{
				playerInfoSystem->SetupDefaultStat();
				playerInfoData.LatestTickTime = Util::Time.GetTimeUTCSec().time_since_epoch().count();

				svrTrace(SVR_DBGTRANS, "Player data created PlayerID:{0}", GetMyOwner()->GetPlayerID());
			}

			svrChk(SetPlayerGameData(playerInfoData));
		}

	Proc_End:

		if( !(hr) )
			CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	Result PlayerTransJoinGameServer::OnGetPlayerGameDataRes(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		auto pDBRes = (DB::QueryGetPlayerInfoCmd*)pRes;

		svrChk(pRes->GetResult());

		// succeeded to query
		if (pDBRes->Result < 0)
		{
			conspiracy::GameConfigTbl::GameConfigItem *pConfig = GetMyServer()->GetPresetGameConfig();
			svrChkPtr(pConfig);

			svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->CreatePlayerInfoCmd(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(), pConfig->DefaultStamina));
			goto Proc_End;
		}

		svrAssert(pDBRes->m_RowsetResult.size() >= 1);
		{
			auto& playerInfoData = *pDBRes->m_RowsetResult.begin();
			svrChk(SetPlayerGameData(playerInfoData));
		}

	Proc_End:

		if (!(hr))
			CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}

	Result PlayerTransJoinGameServer::RegisterToPlayerManager()
	{
		Result hr = ResultCode::SUCCESS;
		EntityUID playerUID;

		svrChk(Service::PlayerManager->CreatePlayer( GetMyOwner()->GetPlayerID(), GetOwnerEntityUID() ) );

	Proc_End:

		if( !(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}

	// Start Transaction
	Result PlayerTransJoinGameServer::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		EntityUID loginEntityUID(GetLoginEntityUID());

		m_GameUID = 0;
		m_PartyLeaderID = 0;
		m_PlayerNick[0] = 0;
		m_MatchingTicket = 0;

		svrChk( super::StartTransaction() );

		if( GetAccID() == 0 || GetMyOwner()->GetAccountID() == 0
			|| GetMyOwner()->GetAccountID() != GetAccID() )
		{
			svrErr(ResultCode::INVALID_ACCOUNTID);
		}

		if( GetTicket() != GetMyOwner()->GetAuthTicket() )
		{
			svrErrClose(ResultCode::INVALID_TICKET);
		}

		// TODO: We need to distinguish whether character data is updated or not
		svrChk(Policy::NetPolicyLoginServer(Service::ServerEntityManager->GetServerConnection(loginEntityUID.GetServerID())).PlayerJoinedToGameServerCmd(
			RouteContext(GetOwnerEntityUID(),loginEntityUID), GetTransID(),
			GetAccID(), GetTicket() ) );

	Proc_End:

		if( !(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}


	// Start Transaction
	Result PlayerTransGetUserGamePlayerInfo::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		memset(&m_Result, 0, sizeof(m_Result));

		auto userGamePlayerInfo = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>();

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetAccountID() == 0 )
		{
			svrErrClose(ResultCode::INVALID_TICKET);
		}


		//m_Grade = userGamePlayerInfo->GetGrade();
		m_Result.Level = userGamePlayerInfo->GetLevel();
		m_Result.Exp = userGamePlayerInfo->GetExp();
		m_Result.GameMoney = userGamePlayerInfo->GetGameMoney();
		m_Result.Gem = userGamePlayerInfo->GetGem();
		m_Result.Stamina = userGamePlayerInfo->GetStamina();
		m_Result.LastUpdateTime = GetMyOwner()->GetLatestUpdateTime().time_since_epoch().count();

		m_Result.TotalPlayed = userGamePlayerInfo->GetTotalPlayed();

		m_Result.WinPlaySC = userGamePlayerInfo->GetWinPlaySCitizen();
		m_Result.WinPlaySM = userGamePlayerInfo->GetWinPlaySMonster();
		m_Result.WinPlaySS = userGamePlayerInfo->GetWinPlaySSeer();
		m_Result.LosePlaySC = userGamePlayerInfo->GetLosePlaySCitizen();
		m_Result.LosePlaySM = userGamePlayerInfo->GetLosePlaySMonster();
		m_Result.LosePlaySS = userGamePlayerInfo->GetLosePlaySSeer();

		m_Result.WinPlayNC = userGamePlayerInfo->GetWinPlayNCitizen();
		m_Result.WinPlayNM = userGamePlayerInfo->GetWinPlayNMonster();
		m_Result.WinPlayNS = userGamePlayerInfo->GetWinPlayNSeer();
		m_Result.LosePlayNC = userGamePlayerInfo->GetLosePlayNCitizen();
		m_Result.LosePlayNM = userGamePlayerInfo->GetLosePlayNMonster();
		m_Result.LosePlayNS = userGamePlayerInfo->GetLosePlayNSeer();

		m_Result.WeeklyWin = userGamePlayerInfo->GetWeeklyWin();
		m_Result.WeeklyLose = userGamePlayerInfo->GetWeeklyLose();

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	
	

	PlayerTransGetGamePlayerInfo::PlayerTransGetGamePlayerInfo(IHeap& heap, MessageDataPtr &pIMsg )
		:MessageTransaction( heap, pIMsg )
	{
		SetExclusive(true);
		BR_TRANS_MESSAGE(DB::QueryGetPlayerShardIDCmd, { return OnGetPlayerShardID(pRes); });
		BR_TRANS_MESSAGE(DB::QueryGetPlayerInfoCmd, { return OnGetGamePlayerInfo(pRes); });
	}

	Result PlayerTransGetGamePlayerInfo::OnGetPlayerShardID(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		auto* pDBRes = (DB::QueryGetPlayerShardIDCmd*)pRes;

		svrChk(pRes->GetResult());

		// succeeded to query
		if (pDBRes->Result < 0)
		{
			svrErr(ResultCode::GAME_INVALID_PLAYER);
		}

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->GetPlayerInfoCmd(GetTransID(), pDBRes->ShardID, GetPlayerID()));


	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}


	Result PlayerTransGetGamePlayerInfo::OnGetGamePlayerInfo( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		auto pDBRes = (DB::QueryGetPlayerInfoCmd*)pRes;

		svrChk(pRes->GetResult());

		// succeeded to query
		if( pDBRes->Result < 0 )
		{
			svrErr(ResultCode::GAME_INVALID_PLAYER);
		}

		svrAssert( pDBRes->m_RowsetResult.size() >= 1 );
		{
			auto& playerInfoData = *pDBRes->m_RowsetResult.begin();

			m_Result.Level = playerInfoData.Level;
			m_Result.TotalPlayed = playerInfoData.TotalPlayed;

			m_Result.WinPlaySC = playerInfoData.WinPlaySC;
			m_Result.WinPlaySM = playerInfoData.WinPlaySM;
			m_Result.WinPlaySM = playerInfoData.WinPlaySM;
			m_Result.LosePlaySC = playerInfoData.LosePlaySC;
			m_Result.LosePlaySM = playerInfoData.LosePlaySM;
			m_Result.LosePlaySM = playerInfoData.LosePlaySM;

			m_Result.WinPlayNC = playerInfoData.WinPlayNC;
			m_Result.WinPlayNM = playerInfoData.WinPlayNM;
			m_Result.WinPlayNM = playerInfoData.WinPlayNM;
			m_Result.LosePlayNC = playerInfoData.LosePlayNC;
			m_Result.LosePlayNM = playerInfoData.LosePlayNM;
			m_Result.LosePlayNM = playerInfoData.LosePlayNM;

			m_Result.WeeklyWin = playerInfoData.WeeklyPlayWin;
			m_Result.WeeklyLose = playerInfoData.WeeklyPlayLose;
		}


	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	
	
	// Start Transaction
	Result PlayerTransGetGamePlayerInfo::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		memset(&m_Result, 0, sizeof(m_Result));

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetAccountID() == 0 )
		{
			svrErrClose(ResultCode::INVALID_TICKET);
		}

		svrChk( Svr::GetServerComponent<DB::AccountDB>()->GetPlayerShardID( GetTransID(), GetPlayerID() ) );


	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}



	PlayerTransGetComplitionState::PlayerTransGetComplitionState(IHeap& heap, MessageDataPtr &pIMsg)
		: MessageTransaction(heap, pIMsg)
	{
		BR_TRANS_MESSAGE(DB::QueryGetComplitionStateCmd, { return OnGetComplitionState(pRes); });
	}

	Result PlayerTransGetComplitionState::OnGetComplitionState(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;

		auto* pDBRes = (DB::QueryGetComplitionStateCmd*)pRes;

		svrChk(pRes->GetResult());

		// succeeded to query
		if (pDBRes->Result < 0)
		{
			svrErr(ResultCode::GAME_INVALID_PLAYER);
		}

		svrChkCloseErr(ResultCode::UNEXPECTED, StrUtil::StringCopy(m_ComplitionState, pDBRes->ComplitionState));

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


	// Start Transaction
	Result PlayerTransGetComplitionState::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		memset(&m_ComplitionState, 0, sizeof(m_ComplitionState));

		svrChk(super::StartTransaction());

		if (GetMyOwner()->GetAccountID() == 0)
		{
			svrErrClose(ResultCode::INVALID_TICKET);
		}

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->GetComplitionState(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID()));


	Proc_End:

		if (!(hr))
			CloseTransaction(hr);

		return hr;
	}




	PlayerTransSetComplitionState::PlayerTransSetComplitionState(IHeap& heap, MessageDataPtr &pIMsg)
		: MessageTransaction(heap, pIMsg)
	{
		BR_TRANS_MESSAGE(DB::QuerySetComplitionStateCmd, { return OnSetComplitionState(pRes); });
	}

	Result PlayerTransSetComplitionState::OnSetComplitionState(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;

		auto* pDBRes = (DB::QuerySetComplitionStateCmd*)pRes;

		svrChk(pRes->GetResult());

		// succeeded to query
		if (pDBRes->Result < 0)
		{
			svrErr(ResultCode::GAME_INVALID_PLAYER);
		}

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}

	// Start Transaction
	Result PlayerTransSetComplitionState::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::StartTransaction());

		if (GetMyOwner()->GetAccountID() == 0)
		{
			svrErrClose(ResultCode::INVALID_TICKET);
		}

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->SetComplitionState(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(), GetComplitionState()));


	Proc_End:

		if (!(hr))
			CloseTransaction(hr);

		return hr;
	}


	
	/////////////////////////////////////////////////////////////////////////////
	//
	//	GCM register/unregister transaction
	//


	PlayerTransRegisterGCM::PlayerTransRegisterGCM(IHeap& heap, MessageDataPtr &pIMsg )
		:MessageTransaction(heap, pIMsg )
	{
		BR_TRANS_MESSAGE( DB::QueryUpdateGCMKeysCmd, { return OnUpdated(pRes); });
	}

	Result PlayerTransRegisterGCM::OnUpdated( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(pRes->GetResult());

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransRegisterGCM::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		GetMyOwner()->SetGCMKeys( GetGCMRegisteredID() );

		svrChk( Svr::GetServerComponent<DB::AccountDB>()->UpdateGCMKeys( GetTransID(), GetMyOwner()->GetAccountID(), GetMyOwner()->GetGCMKeys() ) );

	Proc_End:

		if( !(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}
	


	PlayerTransUnregisterGCM::PlayerTransUnregisterGCM(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction( heap, pIMsg )
	{
		BR_TRANS_MESSAGE( DB::QueryUpdateGCMKeysCmd, { return OnUpdated(pRes); });
	}

	Result PlayerTransUnregisterGCM::OnUpdated( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(pRes->GetResult());

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransUnregisterGCM::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		GetMyOwner()->SetGCMKeys( "" );

		svrChk( Svr::GetServerComponent<DB::AccountDB>()->UpdateGCMKeys( GetTransID(), GetMyOwner()->GetAccountID(), GetMyOwner()->GetGCMKeys() ) );

	Proc_End:

		if( !(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}
	
	
	
	/////////////////////////////////////////////////////////////////////////////
	//
	//	Notifications transaction
	//
	
	PlayerTransGetNotificationList::PlayerTransGetNotificationList(IHeap& heap, MessageDataPtr &pIMsg )
		:MessageTransaction(heap, pIMsg )
	{
		BR_TRANS_MESSAGE( DB::QueryNotification_GetListCmd, { return OnGetList(pRes); });
	}
	
	Result PlayerTransGetNotificationList::OnGetList( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		DB::QueryNotification_GetListCmd::RowsetList::iterator itNotification;
		UserNotifySystem* pNotifySystem = GetMyOwner()->GetComponent<UserNotifySystem>();
		DB::QueryNotification_GetListCmd *pDBRes = (DB::QueryNotification_GetListCmd*)pRes;

		svrChk(pRes->GetResult());

		pNotifySystem->ClearNotificationList();

		itNotification = pDBRes->m_RowsetResult.begin();
		for( ; itNotification != pDBRes->m_RowsetResult.end(); ++itNotification )
		{
			svrChk( pNotifySystem->AddNotification(itNotification->NotificationID, (NotificationType)itNotification->MessageID, itNotification->MessageParam0, itNotification->MessageParam1, itNotification->MessageText, itNotification->IsRead, itNotification->TimeStamp) );
			Policy::NetSvrPolicyGame(GetConnection()).NotifyS2CEvt(itNotification->NotificationID, itNotification->MessageID, itNotification->MessageParam0, itNotification->MessageParam1, itNotification->MessageText, itNotification->IsRead, itNotification->TimeStamp );
		}


	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransGetNotificationList::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->Notification_GetList(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetAccountID()));

	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}
	
	
	PlayerTransDeleteNotification::PlayerTransDeleteNotification(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction(heap, pIMsg )
	{
		BR_TRANS_MESSAGE( DB::QueryNotification_RemoveCmd, { return OnDeletedNotification(pRes); });
	}

	Result PlayerTransDeleteNotification::OnDeletedNotification( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		//DB::QueryNotification_RemoveCmd *pDBRes = (DB::QueryNotification_RemoveCmd*)pRes;

		svrChk(pRes->GetResult());

		svrChk( GetMyOwner()->GetComponent<UserNotifySystem>()->RemoveNotification(GetNotificationID()) );

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransDeleteNotification::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->Notification_Remove(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(), GetNotificationID()));

	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}


	PlayerTransSetNotificationRead::PlayerTransSetNotificationRead(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction( heap, pIMsg )
	{
		BR_TRANS_MESSAGE( DB::QueryNotification_SetReadCmd, { return OnSetRead(pRes); });
		BR_TRANS_MESSAGE( DB::QueryUpdateTickStatusCmd, { return OnUpdateStatus(pRes); });
	}

	Result PlayerTransSetNotificationRead::OnSetRead( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		UserNotifySystem::Notification *pNotify = nullptr;
		//DB::QueryNotification_SetReadCmd *pDBRes = (DB::QueryNotification_SetReadCmd*)pRes;

		svrChk(pRes->GetResult());


		svrChkPtr( pNotify = GetMyOwner()->GetComponent<UserNotifySystem>()->GetNotification(GetNotificationID()) );
		pNotify->IsRead = TRUE;

		switch( pNotify->MessageID )
		{
		case NotificationType::GiftStamina:
			GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GainStamina(1);
			svrChk(GetMyOwner()->UpdateDBSync(GetTransID()));
			goto Proc_End;
			break;
		default:
			break;
		}

		CloseTransaction(hr);

	Proc_End:

		if( !(hr) )
			CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	Result PlayerTransSetNotificationRead::OnUpdateStatus( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		//UserNotifySystem::Notification *pNotify = nullptr;
		//DB::QueryUpdateTickStatusCmd *pDBRes = (DB::QueryUpdateTickStatusCmd*)pRes;

		svrChk(pRes->GetResult());



	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}


	// Start Transaction
	Result PlayerTransSetNotificationRead::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetComponent<UserNotifySystem>()->GetNotification(GetNotificationID()) == nullptr )
			svrErrClose(ResultCode::INVALID_NOTIFICATIONID);

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->Notification_SetRead(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(), GetNotificationID()));

	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}


	PlayerTransAcceptNotification::PlayerTransAcceptNotification(IHeap& heap, MessageDataPtr &pIMsg)
		: MessageTransaction(heap, pIMsg)
	{
		BR_TRANS_MESSAGE(DB::QueryNotification_RemoveCmd, { return OnDeletedNotification(pRes); });
	}

	Result PlayerTransAcceptNotification::OnDeletedNotification(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		//DB::QueryNotification_RemoveCmd *pDBRes = (DB::QueryNotification_RemoveCmd*)pRes;
		auto notification = GetMyOwner()->GetComponent<UserNotifySystem>()->GetNotification(GetNotificationID());

		svrChk(pRes->GetResult());

		if (notification!= nullptr)
		{
			Result hrRes = ResultCode::SUCCESS;
			// accept notification
			// Friend will be processed separately
			// for now, only stamina is processed here
			switch (notification->MessageID)
			{
			case NotificationType::GiftStamina:
				hrRes = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GainStamina(1);
				if (!(hrRes))
				{
					svrTrace(Warning, "Stamina gain is failed, PlayerID:{0}, hr:{1:X8}", GetMyOwner()->GetPlayerID(), hrRes);
				}
				break;
			default:
				break;
			}
		}
		svrChk(GetMyOwner()->GetComponent<UserNotifySystem>()->RemoveNotification(GetNotificationID()));

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}

	// Start Transaction
	Result PlayerTransAcceptNotification::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::StartTransaction());

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->Notification_Remove(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(), GetNotificationID()));

	Proc_End:

		if (!(hr))
			CloseTransaction(hr);

		return hr;
	}

	
	// Start Transaction
	Result PlayerTransNotifyS2S::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Policy::NetSvrPolicyGame *pPolicy = nullptr;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetPlayerID() != GetDestPlayerID() )
			svrErr(ResultCode::GAME_INVALID_PLAYER);

		svrChk( GetMyOwner()->GetComponent<UserNotifySystem>()->AddNotification(GetNotificationID(), (NotificationType)GetMessageID(), GetMessageParam0(), GetMessageParam1(), GetMessageText(), 0, GetTimeStamp() ) );
		svrChk(Policy::NetSvrPolicyGame(GetConnection()).NotifyS2CEvt(GetNotificationID(), (uint)GetMessageID(), GetMessageParam0(), GetMessageParam1(), GetMessageText(), 0, GetTimeStamp()));

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}


	/////////////////////////////////////////////////////////////////////////////
	//
	//	NickName transaction
	//


	PlayerTransSetNickName::PlayerTransSetNickName(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction(heap, pIMsg )
	{
		BR_TRANS_MESSAGE( DB::QuerySetNickNameCmd, { return OnNickChanged(pRes); });
	}

	Result PlayerTransSetNickName::OnNickChanged( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		auto pDBRes = (DB::QuerySetNickNameCmd*)pRes;
		UserGamePlayerInfoSystem* pPlayerInfoSystem = nullptr;
		conspiracy::OrganicTbl::OrganicItem *pCostItem = nullptr;

		svrChk(pRes->GetResult());

		if (pDBRes->Result < 0)
			svrErrClose(ResultCode::INVALID_PLAYERID);

		GetMyOwner()->AddGameTransactionLogT(TransLogCategory::Account, 0, 0, 0, "From {0} to {1}", GetMyOwner()->GetNickName(), GetNickName());

		svrChk( GetMyOwner()->SetNickName( GetNickName() ) );

		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		if (GetIsCostFree() == 0)
		{
			svrChkCloseErr(ResultCode::GAME_INVALID_COSTID, conspiracy::OrganicTbl::FindItem((int)conspiracy::OrganicTbl::EItemEffect::Enum::NickName, pCostItem));
			svrChkCloseErr(ResultCode::GAME_NOTENOUGH_RESOURCE, pPlayerInfoSystem->ApplyCost(pCostItem, TransLogCategory::Buy, "SetNickName"));
		}

		m_TotalGem = pPlayerInfoSystem->GetGem();
		m_TotalGameMoney = pPlayerInfoSystem->GetGameMoney();


	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransSetNickName::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		conspiracy::OrganicTbl::OrganicItem *pCostItem = nullptr;
		UserGamePlayerInfoSystem* pPlayerInfoSystem = nullptr;

		m_TotalGem = 0;
		m_TotalGameMoney = 0;

		svrChk( super::StartTransaction() );

		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		if (GetIsCostFree() == 0)
		{
			svrChkCloseErr(ResultCode::GAME_INVALID_COSTID, conspiracy::OrganicTbl::FindItem((int)conspiracy::OrganicTbl::EItemEffect::Enum::NickName, pCostItem));
			svrChkCloseErr(ResultCode::GAME_NOTENOUGH_RESOURCE, pPlayerInfoSystem->CheckCost(pCostItem));
		}

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->SetNickName(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(), GetNickName()));

	Proc_End:

		if( !(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}
	
	

	/////////////////////////////////////////////////////////////////////////////
	//
	//	Find player transaction
	//

	PlayerTransFindPlayerByEMail::PlayerTransFindPlayerByEMail(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction(heap, pIMsg )
	{
		BR_TRANS_MESSAGE(DB::QueryFindPlayerByEMailCmd, { return OnFindPlayer(pRes); });
		BR_TRANS_MESSAGE(DB::QueryGetNickNameCmd, { return OnGetNickName(pRes); });
	}

	Result PlayerTransFindPlayerByEMail::OnFindPlayer( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		DB::QueryFindPlayerByEMailCmd *pDBRes = (DB::QueryFindPlayerByEMailCmd*)pRes;

		svrChk(pRes->GetResult());

		if( pDBRes->UserID == 0 )
			svrErrClose(ResultCode::SVR_PLAYER_NOT_FOUND);

		m_Player.PlayerID = pDBRes->UserID;
		m_Player.FBUID = pDBRes->FacebookUID;
		m_PlayerShardID = pDBRes->ShardID;

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->GetNickName(GetTransID(), m_PlayerShardID, m_Player.PlayerID));

	Proc_End:

		if (!(hr))
			CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	Result PlayerTransFindPlayerByEMail::OnGetNickName(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		auto *pDBRes = (DB::QueryGetNickNameCmd*)pRes;

		svrChk(pRes->GetResult());

		if (pDBRes->Result < 0)
			svrErrClose(ResultCode::SVR_PLAYER_NOT_FOUND);

		StrUtil::StringCopy( m_Player.NickName, pDBRes->NickName );

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}

	// Start Transaction
	Result PlayerTransFindPlayerByEMail::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		char email[GameConst::MAX_EMAIL];

		svrChk( super::StartTransaction() );

		svrChk( StrUtil::StringLower( email, GetPlayerEMail() ) );

		svrChk( Svr::GetServerComponent<DB::AccountDB>()->FindPlayerByEMail( GetTransID(), GetPlayerEMail() ) );

	Proc_End:

		if( !(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}
	

	PlayerTransFindPlayerByPlayerID::PlayerTransFindPlayerByPlayerID(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction( heap, pIMsg )
	{
		BR_TRANS_MESSAGE(DB::QueryFindPlayerByPlayerIDCmd, { return OnFindPlayer(pRes); });
		BR_TRANS_MESSAGE(DB::QueryGetNickNameCmd, { return OnGetNickName(pRes); });
	}

	Result PlayerTransFindPlayerByPlayerID::OnFindPlayer(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		auto pDBRes = (DB::QueryFindPlayerByPlayerIDCmd*)pRes;
		svrChk(pRes->GetResult());

		if (pDBRes->PlayerID == 0)
			svrErrClose(ResultCode::SVR_PLAYER_NOT_FOUND);

		if (pDBRes->Result != 0)
			svrErrClose(ResultCode::SVR_PLAYER_NOT_FOUND);

		m_Player.PlayerID = pDBRes->PlayerID;
		m_Player.FBUID = pDBRes->FacebookUID;
		m_PlayerShardID = pDBRes->ShardID;

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->GetNickName(GetTransID(), m_PlayerShardID, m_Player.PlayerID));

	Proc_End:

		if (!(hr))
			CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}

	Result PlayerTransFindPlayerByPlayerID::OnGetNickName(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		auto *pDBRes = (DB::QueryGetNickNameCmd*)pRes;

		svrChk(pRes->GetResult());

		if (pDBRes->Result < 0)
			svrErrClose(ResultCode::SVR_PLAYER_NOT_FOUND);

		StrUtil::StringCopy( m_Player.NickName, pDBRes->NickName );

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}

	// Start Transaction
	Result PlayerTransFindPlayerByPlayerID::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		svrChk( Svr::GetServerComponent<DB::AccountDB>()->FindPlayerByPlayerID( GetTransID(), GetPlayerID() ) );

	Proc_End:

		if( !(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}
	
	
	PlayerTransRequestPlayerStatusUpdate::PlayerTransRequestPlayerStatusUpdate(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction(heap, pIMsg )
	{
		BR_TRANS_MESSAGE(DB::QueryGetPlayerShardIDCmd, { return OnPlayerShardIDRes(pRes); });
		BR_TRANS_MESSAGE(DB::QueryGetPlayerStatusCmd, { return OnPlayerStatusUpdateRes(pRes); });
	}

	Result PlayerTransRequestPlayerStatusUpdate::OnPlayerShardIDRes(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		//Policy::NetSvrPolicyGame *pPolicy = nullptr;
		auto *pDBRes = (DB::QueryGetPlayerShardIDCmd*)pRes;

		m_PlayerStatusQueryCount--;

		svrChk(pRes->GetResult());

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->GetPlayerStatusCmd(GetTransID(), pDBRes->ShardID, pDBRes->UserID));
		m_PlayerStatusQueryCount++;

	Proc_End:

		if (m_PlayerStatusQueryCount == 0)
			CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}

	Result PlayerTransRequestPlayerStatusUpdate::OnPlayerStatusUpdateRes( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		Policy::NetSvrPolicyGame *pPolicy = nullptr;
		DB::QueryGetPlayerStatusCmd *pDBRes = (DB::QueryGetPlayerStatusCmd*)pRes;

		m_PlayerStatusQueryCount--;

		svrChk(pRes->GetResult());

		svrChk(Policy::NetSvrPolicyGame(GetConnection()).NotifyPlayerStatusUpdatedS2CEvt(pDBRes->PlayerID, pDBRes->LatestActiveTime, pDBRes->PlayerState != 0 ? 1 : 0));

	Proc_End:

		if( m_PlayerStatusQueryCount == 0 )
			CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransRequestPlayerStatusUpdate::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		EntityUID playerUID;
		Policy::NetSvrPolicyGame pPolicy(GetConnection());
		auto& targetPlayerID = GetTargetPlayerID();
		uint uiRequestMax = Util::Min((uint)targetPlayerID.size(), (uint)20);

		m_PlayerStatusQueryCount = 0;

		svrChk( super::StartTransaction() );
		

		for( uint iPlayer = 0; iPlayer < uiRequestMax; iPlayer++ )
		{
			if( ( Service::PlayerManager->FindPlayer( targetPlayerID[iPlayer], playerUID )) )
			{
				// Now we know he is online we can check with DB
				auto pFriend = GetMyOwner()->GetComponent<UserFriendSystem>()->GetFriend(targetPlayerID[iPlayer]);
				if (pFriend != nullptr)
				{
					svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->GetPlayerStatusCmd(GetTransID(), pFriend->ShardID, targetPlayerID[iPlayer]));
				}
				else
				{
					svrChk(Svr::GetServerComponent<DB::AccountDB>()->GetPlayerShardID(GetTransID(), targetPlayerID[iPlayer]));
				}
				m_PlayerStatusQueryCount++;
			}
			else
			{
				svrChk(pPolicy.NotifyPlayerStatusUpdatedS2CEvt(targetPlayerID[iPlayer], 0, 0));
			}
		}


	Proc_End:

		if( m_PlayerStatusQueryCount == 0 )
			CloseTransaction( hr );

		return hr;
	}
	
	
	// Start Transaction
	Result PlayerTransRequestPlayerStatusUpdateS2S::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ServerEntity *pServerEntity = nullptr;
		EntityUID playerUID;
		bool bInGame;


		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetPlayerID() != GetDestPlayerID() )
			svrErr(ResultCode::GAME_INVALID_PLAYER);

		playerUID = GetRouteContext().GetFrom();

		svrChk( Service::ServerEntityManager->GetServerEntity( playerUID.GetServerID(), pServerEntity ) );

		bInGame = GetMyOwner()->GetGameInsUID().UID != 0 || GetMyOwner()->GetPartyUID().UID != 0;

		svrChk(Policy::NetPolicyGameServer(pServerEntity->GetConnection()).NotifyPlayerStatusUpdatedC2SEvt( 
			RouteContext(GetOwnerEntityUID(),playerUID), 
			GetDestPlayerID(), GetMyOwner()->GetLatestActiveTime().time_since_epoch().count(), bInGame ? 1 : 0 ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	
	// Start Transaction
	Result PlayerTransNotifyPlayerStatusUpdatedS2S::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		svrChk(Policy::NetSvrPolicyGame(GetConnection()).NotifyPlayerStatusUpdatedS2CEvt( GetDestPlayerID(), GetLatestActiveTime(), GetIsInGame() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}




	PlayerTransGetRankingList::PlayerTransGetRankingList(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction(heap, pIMsg )
		, m_RankingList(heap)
	{
		BR_TRANS_MESSAGE( DB::QueryGetTotalRankingCmd, { return OnGetRankingListRes(pRes); });
	}

	Result PlayerTransGetRankingList::OnGetRankingListRes( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		DB::QueryGetTotalRankingCmd *pDBRes = (DB::QueryGetTotalRankingCmd*)pRes;

		svrChk(pRes->GetResult());

		m_RankingList.reserve( pDBRes->m_RowsetResult.size() );
		std::for_each( pDBRes->m_RowsetResult.begin(), pDBRes->m_RowsetResult.end(), [&]( DB::QueryGetTotalRankingSet &set )
		{
			TotalRankingPlayerInformation info( set.RankingID, set.Ranking, set.PlayerID, set.FBUID, set.NickName, set.Level, set.Win, set.Lose );

			m_RankingList.push_back(info);
		});

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransGetRankingList::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if (GetBaseRanking() <= 0 || GetBaseRanking() > GameConst::MAX_RANKING_QUERY_COUNT)
			svrErrClose(ResultCode::INVALID_RANKING_RANGE);

		if (GetCount() <= 0 || GetCount() > GameConst::MAX_RANKING_QUERY_COUNT)
			svrErrClose(ResultCode::INVALID_RANKING_RANGE);
		
		svrChk( Svr::GetServerComponent<DB::RankingDB>()->GetRankingListCmd( GetTransID(), GetBaseRanking(), GetCount() ) );

	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}



	PlayerTransBuyShopItemPrepare::PlayerTransBuyShopItemPrepare(IHeap& heap, MessageDataPtr &pIMsg)
		: MessageTransaction(heap, pIMsg)
		, m_Signagure(heap)
	{
		BR_TRANS_MESSAGE(DB::QueryCheckPurchaseIDCmd, { return OnPurchaseIDChecked(pRes); });
		m_Signagure.push_back('\0');
	}

	Result PlayerTransBuyShopItemPrepare::OnPurchaseIDChecked(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		auto *pDBRes = (DB::QueryCheckPurchaseIDCmd*)pRes;

		svrChk(pRes->GetResult());

		if (pDBRes->Result < 0)
		{
			m_RetryCount++;
			if (m_RetryCount > MAX_RETRY)
			{
				svrErrClose(ResultCode::FAIL);
			}
			else
			{
				svrChk(GenerateSigunatureAndCheck());
			}
		}
		else
		{
			CloseTransaction(hr);
		}


	Proc_End:

		if (!(hr))
			CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}

	Result PlayerTransBuyShopItemPrepare::GenerateSigunatureAndCheck()
	{
		Result hr = ResultCode::SUCCESS;
		StaticArray<uint8_t, 1024> dataBuffer(GetHeap());
		StaticArray<uint8_t, 128> hash(GetHeap());
		//AuthTicket authTicket = GetMyOwner()->GetAuthTicket();
		PlayerID playerID = GetMyOwner()->GetPlayerID();
		auto time = Util::Time.GetTimeUTCSec();
		auto time2 = Util::Time.GetTimeMs();

		m_Signagure.Clear();

		// Generate signature
		svrChk(dataBuffer.push_back(sizeof(playerID), (const uint8_t*)&playerID));
		svrChk(dataBuffer.push_back(sizeof(time), (const uint8_t*)&time));
		svrChk(dataBuffer.push_back(sizeof(time2), (const uint8_t*)&time2));

		svrChk(Util::SHA256Hash(dataBuffer.size(), dataBuffer.data(), hash));
		svrChk(Util::Base64URLEncode(hash.size(), hash.data(), m_Signagure));

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->CheckPurchaseID(GetTransID(), GetMyOwner()->GetShardID(), hash));

	Proc_End:

		m_Signagure.push_back('\0');

		return hr;
	}

	// Start Transaction
	Result PlayerTransBuyShopItemPrepare::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;


		svrChk(super::StartTransaction());

		m_RetryCount = 0;

		svrChk(GenerateSigunatureAndCheck());

	Proc_End:

		if (!(hr))
			CloseTransaction(hr);

		return hr;
	}



	PlayerTransBuyShopItem::PlayerTransBuyShopItem(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction(heap, pIMsg )
		, m_SavedData(heap)
	{
		SetExclusive(true);
		BR_TRANS_MESSAGE(Svr::ExternalTransactionGoogleAndroidReceiptCheck, { return OnPurchaseCheckedAndroid(pRes); });
		BR_TRANS_MESSAGE(Svr::ExternalTransactionIOSRecepitCheck, { return OnPurchaseCheckedIOS(pRes); });
		BR_TRANS_MESSAGE(DB::QuerySavePurchaseInfoToDBCmd, { return OnSavedToDB(pRes); });
	}


	Result PlayerTransBuyShopItem::OnPurchaseCheckedAndroid(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		auto *pCheckRes = (Svr::ExternalTransactionGoogleAndroidReceiptCheck*)pRes;
		UserGamePlayerInfoSystem *pPlayerInfoSystem = nullptr;
		StaticArray<uint8_t, 512> purchaseID(GetHeap());

		svrChkClose(pRes->GetResult());

		if (pCheckRes->GetDeveloperPayload().length() == 0)
			svrErrClose(ResultCode::SVR_INVALID_PURCHASE_INFO);

		svrChkCloseErr(ResultCode::SVR_INVALID_PURCHASE_INFO, Util::Base64URLDecode(pCheckRes->GetDeveloperPayload().length(), (const uint8_t*)pCheckRes->GetDeveloperPayload().c_str(), purchaseID));

		if (purchaseID.size() != SHA256_DIGEST_LENGTH)
			svrErrClose(ResultCode::SVR_INVALID_PURCHASE_INFO);

		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		svrChk(pPlayerInfoSystem->SaveStatToMemento(m_SavedData));

		svrChkClose(pPlayerInfoSystem->ApplyItem(m_pShopItem));

		svrChk(pPlayerInfoSystem->SavePurchaseInfoToDB(GetTransID(), purchaseID, GetPlatform(), GetPurchaseTransactionID()));

	Proc_End:

		if (!(hr))
		{
			CloseTransaction(hr);
		}

		return ResultCode::SUCCESS;
	}

	Result PlayerTransBuyShopItem::OnPurchaseCheckedIOS(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		auto *pCheckRes = (Svr::ExternalTransactionIOSRecepitCheck*)pRes;
		UserGamePlayerInfoSystem *pPlayerInfoSystem = nullptr;

		svrChkClose(pRes->GetResult());

		if (pCheckRes->GetPurchaseTransactionID().size() == 0
			|| !StrUtil::StringCompair((const char*)pCheckRes->GetPurchaseTransactionID().data(), (INT)pCheckRes->GetPurchaseTransactionID().size(), GetPurchaseTransactionID(), -1))
			svrErrClose(ResultCode::SVR_INVALID_PURCHASE_INFO);


		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		svrChk(pPlayerInfoSystem->SaveStatToMemento(m_SavedData));

		svrChkClose(pPlayerInfoSystem->ApplyItem(m_pShopItem));

		svrChk(pPlayerInfoSystem->SavePurchaseInfoToDB(GetTransID(), pCheckRes->GetPurchaseTransactionID(), GetPlatform(), GetPurchaseTransactionID()));

	Proc_End:

		if (!(hr))
		{
			CloseTransaction(hr);
		}

		return ResultCode::SUCCESS;
	}

	Result PlayerTransBuyShopItem::OnSavedToDB( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		auto *pDBRes = (DB::QuerySavePurchaseInfoToDBCmd*)pRes;

		svrChk(pRes->GetResult());

		if (pDBRes->Result < 0)
		{
			svrErrClose(ResultCode::SVR_INVALID_PURCHASE_DUPLICATED);
		}


	Proc_End:

		// if failed to write to DB, roleback the changes
		if(!(hr))
		{
			m_SavedData.RestoreAllData();
		}

		CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransBuyShopItem::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ExternalTransactionManager* pExtMgr = nullptr;
		UserGamePlayerInfoSystem *pPlayerInfoSystem = nullptr;

		m_pShopItem = nullptr;

		svrChk( super::StartTransaction() );

		if (GetPurchaseTransactionID() == nullptr || GetPurchaseTransactionID()[0] == '\0')
		{
			svrErrClose(ResultCode::SVR_INVALID_PURCHASE_INFO);
		}

		if( !( conspiracy::ShopTbl::FindItem( GetShopItemID(), m_pShopItem ) ) )
		{
			svrErrClose(ResultCode::GAME_INVALID_SHOPITEMID);
		}

		svrChkPtr(pExtMgr = Svr::GetServerComponent<Svr::ExternalTransactionManager>());

		if (strlen(m_pShopItem->AndroidItemID) == 0 || strlen(m_pShopItem->iOSItemID) == 0)
		{
			StaticArray<uint8_t,1024> pruchaseID(GetHeap());

			svrChk( Util::Base64URLDecode(strlen(GetPurchaseTransactionID()), (uint8_t*)GetPurchaseTransactionID(), pruchaseID) );
			//svrChk( pruchaseID.AddItems(strlen(GetPurchaseTransactionID()), (uint8_t*)GetPurchaseTransactionID()) );

			svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

			svrChk(pPlayerInfoSystem->SaveStatToMemento(m_SavedData));

			svrChkClose(pPlayerInfoSystem->ApplyItem(m_pShopItem));

			svrChk(pPlayerInfoSystem->SavePurchaseInfoToDB(GetTransID(), pruchaseID, GetPlatform(), GetPurchaseTransactionID()));
		}
		else
		{
			if (StrUtil::StringCompairIgnoreCase("android", -1, GetPlatform(), -1)
				|| StrUtil::StringCompairIgnoreCase("windows", -1, GetPlatform(), -1))
			{
				// The last byte must be null, null terminate
				svrChk(pExtMgr->AndroidCheckReceipt(GetTransID(), GetPackageName(), m_pShopItem->AndroidItemID, GetPurchaseTransactionID()));
			}
			else
			{
				if (GetPurchaseToken().size() == 0)
				{
					svrErrClose(ResultCode::SVR_INVALID_PURCHASE_INFO);
				}

				svrChk(pExtMgr->IOSCheckReceipt(GetTransID(), GetPackageName(), m_pShopItem->iOSItemID, GetPurchaseTransactionID(), GetPurchaseToken()));
			}
		}


	Proc_End:

		if( !(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}
	
	


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Debug
	//


	// Start Transaction
	Result PlayerTransSetConfigPreset::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Policy::NetPolicyGameInstance *pPolicy = nullptr;
		GameInsUID insUID;

		svrChk( super::StartTransaction() );

		svrChk( GetMyServer()->UpdateGameConfig(GetPresetID()) );

		svrChk( GetMyOwner()->UpdateGameConfig() );

		insUID = GetMyOwner()->GetGameInsUID();
		if( insUID.UID != 0 )
		{
			svrChk(Policy::NetPolicyGameInstance(Service::ServerEntityManager->GetServerConnection(insUID.GetServerID())).SetConfigPresetC2SEvt( RouteContext(GetOwnerEntityUID(),insUID), GetPresetID() ) );
		}

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}



	PlayerTransGainGameResource::PlayerTransGainGameResource(IHeap& heap, MessageDataPtr &pIMsg )
		:MessageTransaction( heap, pIMsg )
	{
		BR_TRANS_MESSAGE( DB::QuerySetPlayerInfoCmd, { return OnSetPlayerInfoRes(pRes); } );
	}

	Result PlayerTransGainGameResource::OnSetPlayerInfoRes(  Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( pRes->GetResult() );
		//DB::QuerySetPlayerInfoCmd *pMsgRes = (DB::QuerySetPlayerInfoCmd*)pRes;


	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}

	// Start Transaction
	Result PlayerTransGainGameResource::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		UserGamePlayerInfoSystem *pPlayerInfoSystem = nullptr;
		DebugGameResource res = (DebugGameResource)GetResource();

		svrChk( super::StartTransaction() );

		if( GetResource() < 0 || GetResource() >= (int)DebugGameResource::Max )
			svrErrClose(ResultCode::INVALID_ARG);

		GetMyOwner()->AddGameTransactionLog(TransLogCategory::DbgGain, GetValue(), 0, GetResource());

		switch( res )
		{
		case DebugGameResource::Gem:
			svrChk( GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GainGem( GetValue() ) );
			break;
		case DebugGameResource::GameMoney:
			svrChk( GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GainGameMoney( GetValue() ) );
			break;
		case DebugGameResource::Stamina:
			svrChk( GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GainStamina( GetValue() ) );
			break;
		default:
			svrErrClose(ResultCode::INVALID_ARG);
			break;
		}

		svrChkPtr( pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>() );

		// Save player data
		svrChk(pPlayerInfoSystem->SavePlayerInfoToDB(GetTransID()));


	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}
	



};// namespace GameServer 
};// namespace SF 

