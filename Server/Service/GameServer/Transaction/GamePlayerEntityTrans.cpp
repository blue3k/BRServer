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

#include "Common/ResultCode/BRResultCodeCommon.h"
#include "Common/ResultCode/BRResultCodeGame.h"
#include "Common/ResultCode/BRResultCodeLogin.h"
#include "Common/MemoryPool.h"
#include "Common/BrBaseTypes.h"
#include "Common/GameConst.h"

#include "Net/NetServerUDP.h"

#include "GameServerClass.h"
#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServerSystem/ServiceEntity/Game/GameClusterServiceEntity.h"
#include "ServerSystem/ExternalTransaction.h"
#include "ServerSystem/ExternalTransactionManager.h"

#include "Protocol/Message/LoginServerMsgClass.h"
#include "ServerSystem/ServerService/LoginServerService.h"

#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceIPolicy.h"
#include "Protocol/Message/GamePartyMsgClass.h"
#include "Protocol/Policy/GamePartyIPolicy.h"
#include "Protocol/Message/PartyMatchingQueueMsgClass.h"
#include "Protocol/Policy/PartyMatchingQueueIPolicy.h"

#include "GamePlayerEntityTrans.h"
#include "GameInstance/GamePlayerEntity.h"

#include "ServerSystem/BrServer.h"

#include "GameInstance/GameEntityManager.h"

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
#include "openssl/sha.h"


BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransRegisterPlayerToJoinGameServerOnPlayerEntity);

BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransJoinGameServer);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransGetUserGamePlayerInfo);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransGetGamePlayerInfo);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransGetComplitionState);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransSetComplitionState);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransRegisterGCM);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransUnregisterGCM);

BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransGetNotificationList);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransDeleteNotification);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransAcceptNotification);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransNotifyS2S);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransSetNotificationRead);


BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransSetNickName);

BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransFindPlayerByEMail);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransFindPlayerByPlayerID);

BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransRequestPlayerStatusUpdate);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransRequestPlayerStatusUpdateS2S);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransNotifyPlayerStatusUpdatedS2S);

BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransGetRankingList);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransBuyShopItemPrepare);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransBuyShopItem);

BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransSetConfigPreset);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransGainGameResource);

	
namespace BR {
namespace GameServer {


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Game command transaction
	//

	// Start Transaction
	HRESULT PlayerTransRegisterPlayerToJoinGameServerOnPlayerEntity::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;
		auto pPlayerEntity = GetMyOwner();
		Net::Connection *pConnection = nullptr;
		Net::IConnection::ConnectionInformation connectionInfo;

		svrChk(super::StartTransaction());
		svrChkPtr(GetMyServer()->GetNetPublic());

		pPlayerEntity->ReleaseConnection();

		pPlayerEntity->SetAuthTicket(GetTicket());
		pPlayerEntity->SetFacebookUID(GetFBUserID());

		svrChkPtr(pConnection = GetMyServer()->GetNetPublic()->GetConnectionManager().NewConnection());

		memset(&connectionInfo, 0, sizeof(connectionInfo));
		connectionInfo.SetLocalInfo(GetMyServer()->GetNetClass(), GetMyServer()->GetNetPublic()->GetLocalAddress(), GetMyServer()->GetServerUID());
		connectionInfo.SetRemoteInfo(NetClass::Client, GetTicket());

		svrChk(pConnection->InitConnection(GetMyServer()->GetNetPublic()->GetSocket(), connectionInfo));
		svrTrace(Trace::TRC_USER1, "Initialize connection CID:{0}, Addr:{1}", pConnection->GetCID(), pConnection->GetConnectionInfo().Remote);

		svrChk(GetMyServer()->GetNetPublic()->GetConnectionManager().PendingConnection(pConnection));

		svrChk(pPlayerEntity->SetConnection(pConnection));

		GetMyOwner()->SetLatestActiveTime(Util::Time.GetTimeUTCSec());

		GetMyOwner()->AddGameTransactionLog(TransLogCategory::Account, 2, 0, 0, "Entity Initialize");


	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


	PlayerTransJoinGameServer::PlayerTransJoinGameServer( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		SetExclusive(true);

		BR_TRANS_MESSAGE(DB::QueryCreatePlayerInfoCmd, { return OnCreatePlayerGameDataRes(pRes); });
		BR_TRANS_MESSAGE(DB::QueryGetPlayerInfoCmd, { return OnGetPlayerGameDataRes(pRes); });
		BR_TRANS_MESSAGE( Message::LoginServer::PlayerJoinedToGameServerRes, { return OnGameServerJoined(pRes); });
		BR_TRANS_MESSAGE( Message::GameParty::JoinPartyRes, { return OnJoinPartyRes(pRes); });
	}

	HRESULT PlayerTransJoinGameServer::OnGameServerJoined( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_SYSTEM_OK;
		Message::LoginServer::PlayerJoinedToGameServerRes msgRes;

		// TODO: We need to close this entity on error
		svrChkClose(pRes->GetHRESULT());
		svrChk( msgRes.ParseIMsg( ((Svr::MessageResult*)pRes)->GetMessage() ) );

		// succeeded to create
		svrChk( RegisterToPlayerManager() );

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->GetPlayerInfoCmd(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID()));

	Proc_End:

		if( FAILED(hr) )
			CloseTransaction(hr);

		return S_SYSTEM_OK; 
	}

	HRESULT PlayerTransJoinGameServer::OnJoinPartyRes( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_SYSTEM_OK;
		Message::GameParty::JoinPartyRes msgRes;

		hr = pRes->GetHRESULT();
		if (FAILED(hr))
		{
			m_PartyLeaderID = 0;
			goto Proc_End;
		}

		svrChk( msgRes.ParseIMsg( ((Svr::MessageResult*)pRes)->GetMessage() ) );

		m_PartyLeaderID = msgRes.GetPartyLeaderID();


	Proc_End:

		if( FAILED(hr) )
			GetMyOwner()->SetPartyUID(0);

		GetMyOwner()->UpdateDBSync(0);

		CloseTransaction(hr);

		return S_SYSTEM_OK;
	}

	HRESULT PlayerTransJoinGameServer::SetPlayerGameData(const DB::QueryGetPlayerInfoData &playerData)
	{
		HRESULT hr = S_SYSTEM_OK;
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


		svrTrace(Svr::TRC_TRANSACTION, "SetPlayerGameData PlayerID:{0}. Grade:{1}, lvl:{2}, Exp:%3%, GameMoney:%4%, Gem:%5%, Sta:%6%, updateTick:%7%", GetMyOwner()->GetPlayerID(), 
			playerData.Grade, playerData.Level, playerData.Exp, playerData.GameMoney, playerData.Gem, playerData.Stamina,
			(UINT64)playerData.LatestTickTime);

		auto latestTick = playerData.LatestTickTime;

		GetMyOwner()->SetLatestActiveTime(Util::Time.GetTimeUTCSec());

		// If first login, the value will be zero
		if (latestTick == 0)
		{
			svrTrace(Trace::TRC_WARN, "Invalid player tick time for PlayerID:{0}. Reset to current time", GetMyOwner()->GetPlayerID());
			GetMyOwner()->SetLatestUpdateTime(Util::Time.GetTimeUTCSec());
		}
		else
		{
			svrTrace(Svr::TRC_TRANSACTION, "Latest tick time PlayerID:{0}. {1}", GetMyOwner()->GetPlayerID(), (UINT64)playerData.LatestTickTime);

			GetMyOwner()->SetLatestUpdateTime(TimeStampSec(DurationSec(playerData.LatestTickTime)));
		}

		GetMyOwner()->AddGameTransactionLog(TransLogCategory::Account, 1, 0, 0, "Login");
		

		// copy to return information
		StrUtil::StringCpy(m_PlayerNick, GetMyOwner()->GetNickName());
		m_MatchingTicket = GetMyOwner()->GetMatchingTicket();
		m_GameUID = GetMyOwner()->GetGameInsUID();


		if (GetMyOwner()->GetPartyUID() != 0)
		{
			PartyUID partyUID = GetMyOwner()->GetPartyUID();
			Policy::IPolicyGameParty* pPolicy = nullptr;

			svrChkPtr(pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameParty>(partyUID.GetServerID()));
			svrChk(pPolicy->JoinPartyCmd(RouteContext(GetOwnerEntityUID(), partyUID), GetTransID(), 0, GetMyOwner()->GetPlayerInformation()));
		}
		else
		{
			GetMyOwner()->UpdateDBSync(0);
			CloseTransaction(hr);
		}

	Proc_End:

		return S_SYSTEM_OK;
	}

	HRESULT PlayerTransJoinGameServer::OnCreatePlayerGameDataRes(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_SYSTEM_OK;
		auto pDBRes = (DB::QueryCreatePlayerInfoCmd*)pRes;

		svrChk(pRes->GetHRESULT());

		// succeeded to query
		if( pDBRes->Result < 0 )
		{
			svrErr(E_SVR_INVALID_PLAYER_GAMEDB);
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

				svrTrace(Svr::TRC_TRANSACTION, "Player data created PlayerID:{0}", GetMyOwner()->GetPlayerID());
			}

			svrChk(SetPlayerGameData(playerInfoData));
		}

	Proc_End:

		if( FAILED(hr) )
			CloseTransaction(hr);

		return S_SYSTEM_OK; 
	}

	HRESULT PlayerTransJoinGameServer::OnGetPlayerGameDataRes(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_SYSTEM_OK;
		auto pDBRes = (DB::QueryGetPlayerInfoCmd*)pRes;

		svrChk(pRes->GetHRESULT());

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

		if (FAILED(hr))
			CloseTransaction(hr);

		return S_SYSTEM_OK;
	}

	HRESULT PlayerTransJoinGameServer::RegisterToPlayerManager()
	{
		HRESULT hr = S_SYSTEM_OK;
		Svr::GameClusterServiceEntity *pGameService = nullptr;
		EntityUID playerUID;

		svrChkPtr( pGameService = Svr::GetServerComponent<Svr::GameClusterServiceEntity>() );
		if( SUCCEEDED(pGameService->FindPlayer( GetAccID(), playerUID)) )
		{
			// We don't have to kick because we use previous entity
			//Assert( playerUID == GetMyOwner()->GetEntityUID() );
			goto Proc_End;
		}
		else
		{
			svrChk( pGameService->CreatePlayer( GetMyOwner()->GetPlayerID(), GetOwnerEntityUID() ) );
		}

	Proc_End:

		if( FAILED(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}

	// Start Transaction
	HRESULT PlayerTransJoinGameServer::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;
		EntityUID loginEntityUID(GetLoginEntityUID());
		Policy::IPolicyLoginServer *pLoginPolicy = nullptr;

		m_GameUID = 0;
		m_PartyLeaderID = 0;
		m_PlayerNick[0] = 0;
		m_MatchingTicket = 0;

		svrChk( super::StartTransaction() );

		if( GetAccID() == 0 || GetMyOwner()->GetAccountID() == 0
			|| GetMyOwner()->GetAccountID() != GetAccID() )
		{
			svrErr(E_INVALID_ACCOUNTID);
		}

		if( GetTicket() != GetMyOwner()->GetAuthTicket() )
		{
			svrErrClose(E_INVALID_TICKET);
		}

		// TODO: We need to distinguish whether character data is updated or not
		svrChkPtr( pLoginPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyLoginServer>( loginEntityUID.GetServerID()) );
		svrChk( pLoginPolicy->PlayerJoinedToGameServerCmd( RouteContext(GetOwnerEntityUID(),loginEntityUID), GetTransID(),
			GetAccID(), GetTicket() ) );

	Proc_End:

		if( FAILED(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}


	// Start Transaction
	HRESULT PlayerTransGetUserGamePlayerInfo::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		memset(&m_Result, 0, sizeof(m_Result));

		auto userGamePlayerInfo = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>();

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetAccountID() == 0 )
		{
			svrErrClose(E_INVALID_TICKET);
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
	
	

	PlayerTransGetGamePlayerInfo::PlayerTransGetGamePlayerInfo( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		SetExclusive(true);
		BR_TRANS_MESSAGE(DB::QueryGetPlayerShardIDCmd, { return OnGetPlayerShardID(pRes); });
		BR_TRANS_MESSAGE(DB::QueryGetPlayerInfoCmd, { return OnGetGamePlayerInfo(pRes); });
	}

	HRESULT PlayerTransGetGamePlayerInfo::OnGetPlayerShardID(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_SYSTEM_OK;
		auto* pDBRes = (DB::QueryGetPlayerShardIDCmd*)pRes;

		svrChk(pRes->GetHRESULT());

		// succeeded to query
		if (pDBRes->Result < 0)
		{
			svrErr(E_GAME_INVALID_PLAYER);
		}

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->GetPlayerInfoCmd(GetTransID(), pDBRes->ShardID, GetPlayerID()));


	Proc_End:

		CloseTransaction(hr);

		return S_SYSTEM_OK;
	}


	HRESULT PlayerTransGetGamePlayerInfo::OnGetGamePlayerInfo( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_SYSTEM_OK;
		auto pDBRes = (DB::QueryGetPlayerInfoCmd*)pRes;

		svrChk(pRes->GetHRESULT());

		// succeeded to query
		if( pDBRes->Result < 0 )
		{
			svrErr(E_GAME_INVALID_PLAYER);
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

		return S_SYSTEM_OK; 
	}

	
	
	// Start Transaction
	HRESULT PlayerTransGetGamePlayerInfo::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		memset(&m_Result, 0, sizeof(m_Result));

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetAccountID() == 0 )
		{
			svrErrClose(E_INVALID_TICKET);
		}

		svrChk( Svr::GetServerComponent<DB::AccountDB>()->GetPlayerShardID( GetTransID(), GetPlayerID() ) );


	Proc_End:

		if( FAILED(hr) )
			CloseTransaction( hr );

		return hr;
	}



	PlayerTransGetComplitionState::PlayerTransGetComplitionState(Message::MessageData* &pIMsg)
		: MessageTransaction(pIMsg)
	{
		BR_TRANS_MESSAGE(DB::QueryGetComplitionStateCmd, { return OnGetComplitionState(pRes); });
	}

	HRESULT PlayerTransGetComplitionState::OnGetComplitionState(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_SYSTEM_OK;

		auto* pDBRes = (DB::QueryGetComplitionStateCmd*)pRes;

		svrChk(pRes->GetHRESULT());

		// succeeded to query
		if (pDBRes->Result < 0)
		{
			svrErr(E_GAME_INVALID_PLAYER);
		}

		svrChkCloseErr(E_SYSTEM_UNEXPECTED, StrUtil::StringCpy(m_ComplitionState, pDBRes->ComplitionState));

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


	// Start Transaction
	HRESULT PlayerTransGetComplitionState::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		memset(&m_ComplitionState, 0, sizeof(m_ComplitionState));

		svrChk(super::StartTransaction());

		if (GetMyOwner()->GetAccountID() == 0)
		{
			svrErrClose(E_INVALID_TICKET);
		}

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->GetComplitionState(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID()));


	Proc_End:

		if (FAILED(hr))
			CloseTransaction(hr);

		return hr;
	}




	PlayerTransSetComplitionState::PlayerTransSetComplitionState(Message::MessageData* &pIMsg)
		: MessageTransaction(pIMsg)
	{
		BR_TRANS_MESSAGE(DB::QuerySetComplitionStateCmd, { return OnSetComplitionState(pRes); });
	}

	HRESULT PlayerTransSetComplitionState::OnSetComplitionState(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_SYSTEM_OK;

		auto* pDBRes = (DB::QuerySetComplitionStateCmd*)pRes;

		svrChk(pRes->GetHRESULT());

		// succeeded to query
		if (pDBRes->Result < 0)
		{
			svrErr(E_GAME_INVALID_PLAYER);
		}

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}

	// Start Transaction
	HRESULT PlayerTransSetComplitionState::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk(super::StartTransaction());

		if (GetMyOwner()->GetAccountID() == 0)
		{
			svrErrClose(E_INVALID_TICKET);
		}

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->SetComplitionState(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(), GetComplitionState()));


	Proc_End:

		if (FAILED(hr))
			CloseTransaction(hr);

		return hr;
	}


	
	/////////////////////////////////////////////////////////////////////////////
	//
	//	GCM register/unregister transaction
	//


	PlayerTransRegisterGCM::PlayerTransRegisterGCM( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( DB::QueryUpdateGCMKeysCmd, { return OnUpdated(pRes); });
	}

	HRESULT PlayerTransRegisterGCM::OnUpdated( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk(pRes->GetHRESULT());

	Proc_End:

		CloseTransaction(hr);

		return S_SYSTEM_OK; 
	}

	// Start Transaction
	HRESULT PlayerTransRegisterGCM::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk( super::StartTransaction() );

		svrChk( GetMyOwner()->SetGCMKeys( GetGCMRegisteredID() ) );

		svrChk( Svr::GetServerComponent<DB::AccountDB>()->UpdateGCMKeys( GetTransID(), GetMyOwner()->GetAccountID(), GetMyOwner()->GetGCMKeys() ) );

	Proc_End:

		if( FAILED(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}
	


	PlayerTransUnregisterGCM::PlayerTransUnregisterGCM( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( DB::QueryUpdateGCMKeysCmd, { return OnUpdated(pRes); });
	}

	HRESULT PlayerTransUnregisterGCM::OnUpdated( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk(pRes->GetHRESULT());

	Proc_End:

		CloseTransaction(hr);

		return S_SYSTEM_OK; 
	}

	// Start Transaction
	HRESULT PlayerTransUnregisterGCM::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk( super::StartTransaction() );

		svrChk( GetMyOwner()->SetGCMKeys( "" ) );

		svrChk( Svr::GetServerComponent<DB::AccountDB>()->UpdateGCMKeys( GetTransID(), GetMyOwner()->GetAccountID(), GetMyOwner()->GetGCMKeys() ) );

	Proc_End:

		if( FAILED(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}
	
	
	
	/////////////////////////////////////////////////////////////////////////////
	//
	//	Notifications transaction
	//
	
	PlayerTransGetNotificationList::PlayerTransGetNotificationList(Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( DB::QueryNotification_GetListCmd, { return OnGetList(pRes); });
	}
	
	HRESULT PlayerTransGetNotificationList::OnGetList( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_SYSTEM_OK;
		DB::QueryNotification_GetListCmd::RowsetList::iterator itNotification;
		UserNotifySystem* pNotifySystem = GetMyOwner()->GetComponent<UserNotifySystem>();
		DB::QueryNotification_GetListCmd *pDBRes = (DB::QueryNotification_GetListCmd*)pRes;

		svrChk(pRes->GetHRESULT());

		pNotifySystem->ClearNotificationList();

		itNotification = pDBRes->m_RowsetResult.begin();
		for( ; itNotification != pDBRes->m_RowsetResult.end(); ++itNotification )
		{
			svrChk( pNotifySystem->AddNotification(itNotification->NotificationID, (NotificationType)itNotification->MessageID, itNotification->MessageParam0, itNotification->MessageParam1, itNotification->MessageText, itNotification->IsRead, itNotification->TimeStamp) );
			GetPolicy()->NotifyS2CEvt(itNotification->NotificationID, (NotificationType)itNotification->MessageID, itNotification->MessageParam0, itNotification->MessageParam1, itNotification->MessageText, itNotification->IsRead, itNotification->TimeStamp );
		}


	Proc_End:

		CloseTransaction(hr);

		return S_SYSTEM_OK; 
	}

	// Start Transaction
	HRESULT PlayerTransGetNotificationList::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk( super::StartTransaction() );

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->Notification_GetList(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetAccountID()));

	Proc_End:

		if( FAILED(hr) )
			CloseTransaction( hr );

		return hr;
	}
	
	
	PlayerTransDeleteNotification::PlayerTransDeleteNotification(Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( DB::QueryNotification_RemoveCmd, { return OnDeletedNotification(pRes); });
	}

	HRESULT PlayerTransDeleteNotification::OnDeletedNotification( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_SYSTEM_OK;
		//DB::QueryNotification_RemoveCmd *pDBRes = (DB::QueryNotification_RemoveCmd*)pRes;

		svrChk(pRes->GetHRESULT());

		svrChk( GetMyOwner()->GetComponent<UserNotifySystem>()->RemoveNotification(GetNotificationID()) );

	Proc_End:

		CloseTransaction(hr);

		return S_SYSTEM_OK; 
	}

	// Start Transaction
	HRESULT PlayerTransDeleteNotification::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk( super::StartTransaction() );

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->Notification_Remove(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(), GetNotificationID()));

	Proc_End:

		if( FAILED(hr) )
			CloseTransaction( hr );

		return hr;
	}


	PlayerTransSetNotificationRead::PlayerTransSetNotificationRead(Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( DB::QueryNotification_SetReadCmd, { return OnSetRead(pRes); });
		BR_TRANS_MESSAGE( DB::QueryUpdateTickStatusCmd, { return OnUpdateStatus(pRes); });
	}

	HRESULT PlayerTransSetNotificationRead::OnSetRead( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_SYSTEM_OK;
		UserNotifySystem::Notification *pNotify = nullptr;
		//DB::QueryNotification_SetReadCmd *pDBRes = (DB::QueryNotification_SetReadCmd*)pRes;

		svrChk(pRes->GetHRESULT());


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

		if( FAILED(hr) )
			CloseTransaction(hr);

		return S_SYSTEM_OK; 
	}

	HRESULT PlayerTransSetNotificationRead::OnUpdateStatus( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_SYSTEM_OK;
		//UserNotifySystem::Notification *pNotify = nullptr;
		//DB::QueryUpdateTickStatusCmd *pDBRes = (DB::QueryUpdateTickStatusCmd*)pRes;

		svrChk(pRes->GetHRESULT());



	Proc_End:

		CloseTransaction(hr);

		return S_SYSTEM_OK; 
	}


	// Start Transaction
	HRESULT PlayerTransSetNotificationRead::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetComponent<UserNotifySystem>()->GetNotification(GetNotificationID()) == nullptr )
			svrErrClose(E_INVALID_NOTIFICATIONID);

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->Notification_SetRead(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(), GetNotificationID()));

	Proc_End:

		if( FAILED(hr) )
			CloseTransaction( hr );

		return hr;
	}


	PlayerTransAcceptNotification::PlayerTransAcceptNotification(Message::MessageData* &pIMsg)
		:MessageTransaction(pIMsg)
	{
		BR_TRANS_MESSAGE(DB::QueryNotification_RemoveCmd, { return OnDeletedNotification(pRes); });
	}

	HRESULT PlayerTransAcceptNotification::OnDeletedNotification(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_SYSTEM_OK;
		//DB::QueryNotification_RemoveCmd *pDBRes = (DB::QueryNotification_RemoveCmd*)pRes;
		auto notification = GetMyOwner()->GetComponent<UserNotifySystem>()->GetNotification(GetNotificationID());

		svrChk(pRes->GetHRESULT());

		if (notification!= nullptr)
		{
			HRESULT hrRes = S_SYSTEM_OK;
			// accept notification
			// Friend will be processed separately
			// for now, only stamina is processed here
			switch (notification->MessageID)
			{
			case NotificationType::GiftStamina:
				hrRes = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GainStamina(1);
				if (FAILED(hrRes))
				{
					svrTrace(Trace::TRC_WARN, "Stamina gain is failed, PlayerID:{0}, hr:{1:X8}", GetMyOwner()->GetPlayerID(), hrRes);
				}
				break;
			default:
				break;
			}
		}
		svrChk(GetMyOwner()->GetComponent<UserNotifySystem>()->RemoveNotification(GetNotificationID()));

	Proc_End:

		CloseTransaction(hr);

		return S_SYSTEM_OK;
	}

	// Start Transaction
	HRESULT PlayerTransAcceptNotification::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk(super::StartTransaction());

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->Notification_Remove(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(), GetNotificationID()));

	Proc_End:

		if (FAILED(hr))
			CloseTransaction(hr);

		return hr;
	}

	
	// Start Transaction
	HRESULT PlayerTransNotifyS2S::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;
		Policy::ISvrPolicyGame *pPolicy = nullptr;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetPlayerID() != GetDestPlayerID() )
			svrErr(E_GAME_INVALID_PLAYER);

		svrChk( GetMyOwner()->GetComponent<UserNotifySystem>()->AddNotification(GetNotificationID(), (NotificationType)GetMessageID(), GetMessageParam0(), GetMessageParam1(), GetMessageText(), 0, GetTimeStamp() ) );
		svrChkPtr(pPolicy = GetPolicy<Policy::ISvrPolicyGame>());
		svrChk(pPolicy->NotifyS2CEvt(GetNotificationID(), GetMessageID(), GetMessageParam0(), GetMessageParam1(), GetMessageText(), 0, GetTimeStamp()));

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}


	/////////////////////////////////////////////////////////////////////////////
	//
	//	NickName transaction
	//


	PlayerTransSetNickName::PlayerTransSetNickName( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( DB::QuerySetNickNameCmd, { return OnNickChanged(pRes); });
	}

	HRESULT PlayerTransSetNickName::OnNickChanged( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_SYSTEM_OK;
		auto pDBRes = (DB::QuerySetNickNameCmd*)pRes;
		UserGamePlayerInfoSystem* pPlayerInfoSystem = nullptr;
		conspiracy::OrganicTbl::OrganicItem *pCostItem = nullptr;

		svrChk(pRes->GetHRESULT());

		if (pDBRes->Result < 0)
			svrErrClose(E_INVALID_PLAYERID);

		GetMyOwner()->AddGameTransactionLogT(TransLogCategory::Account, 0, 0, 0, "From {0} to {1}", GetMyOwner()->GetNickName(), GetNickName());

		svrChk( GetMyOwner()->SetNickName( GetNickName() ) );

		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		if (GetIsCostFree() == 0)
		{
			svrChkCloseErr(E_GAME_INVALID_COSTID, conspiracy::OrganicTbl::FindItem((int)conspiracy::OrganicTbl::EItemEffect::Enum::NickName, pCostItem));
			svrChkCloseErr(E_GAME_NOTENOUGH_RESOURCE, pPlayerInfoSystem->ApplyCost(pCostItem, TransLogCategory::Buy, "SetNickName"));
		}

		m_TotalGem = pPlayerInfoSystem->GetGem();
		m_TotalGameMoney = pPlayerInfoSystem->GetGameMoney();


	Proc_End:

		CloseTransaction(hr);

		return S_SYSTEM_OK; 
	}

	// Start Transaction
	HRESULT PlayerTransSetNickName::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;
		conspiracy::OrganicTbl::OrganicItem *pCostItem = nullptr;
		UserGamePlayerInfoSystem* pPlayerInfoSystem = nullptr;

		m_TotalGem = 0;
		m_TotalGameMoney = 0;

		svrChk( super::StartTransaction() );

		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		if (GetIsCostFree() == 0)
		{
			svrChkCloseErr(E_GAME_INVALID_COSTID, conspiracy::OrganicTbl::FindItem((int)conspiracy::OrganicTbl::EItemEffect::Enum::NickName, pCostItem));
			svrChkCloseErr(E_GAME_NOTENOUGH_RESOURCE, pPlayerInfoSystem->CheckCost(pCostItem));
		}

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->SetNickName(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(), GetNickName()));

	Proc_End:

		if( FAILED(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}
	
	

	/////////////////////////////////////////////////////////////////////////////
	//
	//	Find player transaction
	//

	PlayerTransFindPlayerByEMail::PlayerTransFindPlayerByEMail( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE(DB::QueryFindPlayerByEMailCmd, { return OnFindPlayer(pRes); });
		BR_TRANS_MESSAGE(DB::QueryGetNickNameCmd, { return OnGetNickName(pRes); });
	}

	HRESULT PlayerTransFindPlayerByEMail::OnFindPlayer( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_SYSTEM_OK;
		DB::QueryFindPlayerByEMailCmd *pDBRes = (DB::QueryFindPlayerByEMailCmd*)pRes;

		svrChk(pRes->GetHRESULT());

		if( pDBRes->UserID == 0 )
			svrErrClose(E_SVR_PLAYER_NOT_FOUND);

		m_Player.PlayerID = pDBRes->UserID;
		m_Player.FBUID = pDBRes->FacebookUID;
		m_PlayerShardID = pDBRes->ShardID;

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->GetNickName(GetTransID(), m_PlayerShardID, m_Player.PlayerID));

	Proc_End:

		if (FAILED(hr))
			CloseTransaction(hr);

		return S_SYSTEM_OK; 
	}

	HRESULT PlayerTransFindPlayerByEMail::OnGetNickName(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_SYSTEM_OK;
		auto *pDBRes = (DB::QueryGetNickNameCmd*)pRes;

		svrChk(pRes->GetHRESULT());

		if (pDBRes->Result < 0)
			svrErrClose(E_SVR_PLAYER_NOT_FOUND);

		StrUtil::StringCpy( m_Player.NickName, pDBRes->NickName );

	Proc_End:

		CloseTransaction(hr);

		return S_SYSTEM_OK;
	}

	// Start Transaction
	HRESULT PlayerTransFindPlayerByEMail::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;
		char email[GameConst::MAX_EMAIL];

		svrChk( super::StartTransaction() );

		svrChk( StrUtil::StringLwr( email, GetPlayerEMail() ) );

		svrChk( Svr::GetServerComponent<DB::AccountDB>()->FindPlayerByEMail( GetTransID(), GetPlayerEMail() ) );

	Proc_End:

		if( FAILED(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}
	

	PlayerTransFindPlayerByPlayerID::PlayerTransFindPlayerByPlayerID( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE(DB::QueryFindPlayerByPlayerIDCmd, { return OnFindPlayer(pRes); });
		BR_TRANS_MESSAGE(DB::QueryGetNickNameCmd, { return OnGetNickName(pRes); });
	}

	HRESULT PlayerTransFindPlayerByPlayerID::OnFindPlayer(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_SYSTEM_OK;
		auto pDBRes = (DB::QueryFindPlayerByPlayerIDCmd*)pRes;
		svrChk(pRes->GetHRESULT());

		if (pDBRes->PlayerID == 0)
			svrErrClose(E_SVR_PLAYER_NOT_FOUND);

		if (pDBRes->Result != 0)
			svrErrClose(E_SVR_PLAYER_NOT_FOUND);

		m_Player.PlayerID = pDBRes->PlayerID;
		m_Player.FBUID = pDBRes->FacebookUID;
		m_PlayerShardID = pDBRes->ShardID;

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->GetNickName(GetTransID(), m_PlayerShardID, m_Player.PlayerID));

	Proc_End:

		if (FAILED(hr))
			CloseTransaction(hr);

		return S_SYSTEM_OK;
	}

	HRESULT PlayerTransFindPlayerByPlayerID::OnGetNickName(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_SYSTEM_OK;
		auto *pDBRes = (DB::QueryGetNickNameCmd*)pRes;

		svrChk(pRes->GetHRESULT());

		if (pDBRes->Result < 0)
			svrErrClose(E_SVR_PLAYER_NOT_FOUND);

		StrUtil::StringCpy( m_Player.NickName, pDBRes->NickName );

	Proc_End:

		CloseTransaction(hr);

		return S_SYSTEM_OK;
	}

	// Start Transaction
	HRESULT PlayerTransFindPlayerByPlayerID::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk( super::StartTransaction() );

		svrChk( Svr::GetServerComponent<DB::AccountDB>()->FindPlayerByPlayerID( GetTransID(), GetPlayerID() ) );

	Proc_End:

		if( FAILED(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}
	
	
	PlayerTransRequestPlayerStatusUpdate::PlayerTransRequestPlayerStatusUpdate( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE(DB::QueryGetPlayerShardIDCmd, { return OnPlayerShardIDRes(pRes); });
		BR_TRANS_MESSAGE(DB::QueryGetPlayerStatusCmd, { return OnPlayerStatusUpdateRes(pRes); });
	}

	HRESULT PlayerTransRequestPlayerStatusUpdate::OnPlayerShardIDRes(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_SYSTEM_OK;
		//Policy::ISvrPolicyGame *pPolicy = nullptr;
		auto *pDBRes = (DB::QueryGetPlayerShardIDCmd*)pRes;

		m_PlayerStatusQueryCount--;

		svrChk(pRes->GetHRESULT());

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->GetPlayerStatusCmd(GetTransID(), pDBRes->ShardID, pDBRes->UserID));
		m_PlayerStatusQueryCount++;

	Proc_End:

		if (m_PlayerStatusQueryCount == 0)
			CloseTransaction(hr);

		return S_SYSTEM_OK;
	}

	HRESULT PlayerTransRequestPlayerStatusUpdate::OnPlayerStatusUpdateRes( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_SYSTEM_OK;
		Policy::ISvrPolicyGame *pPolicy = nullptr;
		DB::QueryGetPlayerStatusCmd *pDBRes = (DB::QueryGetPlayerStatusCmd*)pRes;

		m_PlayerStatusQueryCount--;

		svrChk(pRes->GetHRESULT());

		svrChkPtr(pPolicy = GetPolicy<Policy::ISvrPolicyGame>());
		svrChk(pPolicy->NotifyPlayerStatusUpdatedS2CEvt(pDBRes->PlayerID, pDBRes->LatestActiveTime, pDBRes->PlayerState != 0 ? 1 : 0));

	Proc_End:

		if( m_PlayerStatusQueryCount == 0 )
			CloseTransaction(hr);

		return S_SYSTEM_OK; 
	}

	// Start Transaction
	HRESULT PlayerTransRequestPlayerStatusUpdate::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;
		//Svr::ServerEntity *pServerEntity = nullptr;
		EntityUID playerUID;
		//Policy::IPolicyGameServer* pTargetPolicy = nullptr;
		Policy::ISvrPolicyGame *pPolicy = nullptr;
		auto& targetPlayerID = GetTargetPlayerID();
		UINT uiRequestMax = Util::Min((UINT)targetPlayerID.GetSize(), (UINT)20);

		m_PlayerStatusQueryCount = 0;

		svrChk( super::StartTransaction() );
		
		svrChkPtr(pPolicy = GetPolicy<Policy::ISvrPolicyGame>());


		for( UINT iPlayer = 0; iPlayer < uiRequestMax; iPlayer++ )
		{
			if( SUCCEEDED( Svr::GetServerComponent<Svr::GameClusterServiceEntity>()->FindPlayer( targetPlayerID[iPlayer], playerUID )) )
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
				svrChk(pPolicy->NotifyPlayerStatusUpdatedS2CEvt(targetPlayerID[iPlayer], 0, 0));
			}
		}


	Proc_End:

		if( m_PlayerStatusQueryCount == 0 )
			CloseTransaction( hr );

		return hr;
	}
	
	
	// Start Transaction
	HRESULT PlayerTransRequestPlayerStatusUpdateS2S::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;
		Svr::ServerEntity *pServerEntity = nullptr;
		EntityUID playerUID;
		Policy::IPolicyGameServer* pTargetPolicy = nullptr;
		bool bInGame;


		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetPlayerID() != GetDestPlayerID() )
			svrErr(E_GAME_INVALID_PLAYER);

		playerUID = GetRouteContext().GetFrom();

		svrChk( Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity( playerUID.GetServerID(), pServerEntity ) );
		svrChkPtr(pTargetPolicy = pServerEntity->GetPolicy<Policy::IPolicyGameServer>());

		bInGame = GetMyOwner()->GetGameInsUID() != 0 || GetMyOwner()->GetPartyUID() != 0;

		svrChk( pTargetPolicy->NotifyPlayerStatusUpdatedC2SEvt( RouteContext(GetOwnerEntityUID(),playerUID), GetDestPlayerID(), GetMyOwner()->GetLatestActiveTime().time_since_epoch().count(), bInGame ? 1 : 0 ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	
	// Start Transaction
	HRESULT PlayerTransNotifyPlayerStatusUpdatedS2S::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;
		Policy::ISvrPolicyGame *pPolicy = nullptr;

		svrChk( super::StartTransaction() );

		svrChkPtr(pPolicy = GetPolicy<Policy::ISvrPolicyGame>());
		svrChk( pPolicy->NotifyPlayerStatusUpdatedS2CEvt( GetDestPlayerID(), GetLatestActiveTime(), GetIsInGame() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}




	PlayerTransGetRankingList::PlayerTransGetRankingList( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( DB::QueryGetTotalRankingCmd, { return OnGetRankingListRes(pRes); });
	}

	HRESULT PlayerTransGetRankingList::OnGetRankingListRes( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_SYSTEM_OK;
		DB::QueryGetTotalRankingCmd *pDBRes = (DB::QueryGetTotalRankingCmd*)pRes;

		svrChk(pRes->GetHRESULT());

		m_RankingList.Reserve( pDBRes->m_RowsetResult.size() );
		std::for_each( pDBRes->m_RowsetResult.begin(), pDBRes->m_RowsetResult.end(), [&]( DB::QueryGetTotalRankingSet &set )
		{
			TotalRankingPlayerInformation info( set.RankingID, set.Ranking, set.PlayerID, set.FBUID, set.NickName, set.Level, set.Win, set.Lose );

			m_RankingList.push_back(info);
		});

	Proc_End:

		CloseTransaction(hr);

		return S_SYSTEM_OK; 
	}

	// Start Transaction
	HRESULT PlayerTransGetRankingList::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk( super::StartTransaction() );

		if (GetBaseRanking() <= 0 || GetBaseRanking() > GameConst::MAX_RANKING_QUERY_COUNT)
			svrErrClose(E_INVALID_RANKING_RANGE);

		if (GetCount() <= 0 || GetCount() > GameConst::MAX_RANKING_QUERY_COUNT)
			svrErrClose(E_INVALID_RANKING_RANGE);
		
		svrChk( Svr::GetServerComponent<DB::RankingDB>()->GetRankingListCmd( GetTransID(), GetBaseRanking(), GetCount() ) );

	Proc_End:

		if( FAILED(hr) )
			CloseTransaction( hr );

		return hr;
	}



	PlayerTransBuyShopItemPrepare::PlayerTransBuyShopItemPrepare(Message::MessageData* &pIMsg)
		: MessageTransaction(pIMsg)
	{
		BR_TRANS_MESSAGE(DB::QueryCheckPurchaseIDCmd, { return OnPurchaseIDChecked(pRes); });
		m_Signagure.push_back('\0');
	}

	HRESULT PlayerTransBuyShopItemPrepare::OnPurchaseIDChecked(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_SYSTEM_OK;
		auto *pDBRes = (DB::QueryCheckPurchaseIDCmd*)pRes;

		svrChk(pRes->GetHRESULT());

		if (pDBRes->Result < 0)
		{
			m_RetryCount++;
			if (m_RetryCount > MAX_RETRY)
			{
				svrErrClose(E_SYSTEM_FAIL);
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

		if (FAILED(hr))
			CloseTransaction(hr);

		return S_SYSTEM_OK;
	}

	HRESULT PlayerTransBuyShopItemPrepare::GenerateSigunatureAndCheck()
	{
		HRESULT hr = S_SYSTEM_OK;
		StaticArray<BYTE, 1024> dataBuffer;
		StaticArray<BYTE, 128> hash;
		//AuthTicket authTicket = GetMyOwner()->GetAuthTicket();
		PlayerID playerID = GetMyOwner()->GetPlayerID();
		auto time = Util::Time.GetTimeUTCSec();
		auto time2 = Util::Time.GetTimeMs();

		m_Signagure.Clear();

		// Generate sigunature
		svrChk(dataBuffer.AddItems(sizeof(playerID), (const BYTE*)&playerID));
		svrChk(dataBuffer.AddItems(sizeof(time), (const BYTE*)&time));
		svrChk(dataBuffer.AddItems(sizeof(time2), (const BYTE*)&time2));

		svrChk(Util::SHA256Hash(dataBuffer.GetSize(), dataBuffer.data(), hash));
		svrChk(Util::Base64URLEncode(hash.GetSize(), hash.data(), m_Signagure));

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->CheckPurchaseID(GetTransID(), GetMyOwner()->GetShardID(), hash));

	Proc_End:

		m_Signagure.push_back('\0');

		return hr;
	}

	// Start Transaction
	HRESULT PlayerTransBuyShopItemPrepare::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;


		svrChk(super::StartTransaction());

		m_RetryCount = 0;

		svrChk(GenerateSigunatureAndCheck());

	Proc_End:

		if (FAILED(hr))
			CloseTransaction(hr);

		return hr;
	}



	PlayerTransBuyShopItem::PlayerTransBuyShopItem( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		SetExclusive(true);
		BR_TRANS_MESSAGE(Svr::ExternalTransactionGoogleAndroidReceiptCheck, { return OnPurchaseCheckedAndroid(pRes); });
		BR_TRANS_MESSAGE(Svr::ExternalTransactionIOSRecepitCheck, { return OnPurchaseCheckedIOS(pRes); });
		BR_TRANS_MESSAGE(DB::QuerySavePurchaseInfoToDBCmd, { return OnSavedToDB(pRes); });
	}


	HRESULT PlayerTransBuyShopItem::OnPurchaseCheckedAndroid(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_SYSTEM_OK;
		auto *pCheckRes = (Svr::ExternalTransactionGoogleAndroidReceiptCheck*)pRes;
		UserGamePlayerInfoSystem *pPlayerInfoSystem = nullptr;
		StaticArray<BYTE, 512> purchaseID;

		svrChkClose(pRes->GetHRESULT());

		if (pCheckRes->GetDeveloperPayload().length() == 0)
			svrErrClose(E_SVR_INVALID_PURCHASE_INFO);

		svrChkCloseErr(E_SVR_INVALID_PURCHASE_INFO, Util::Base64URLDecode(pCheckRes->GetDeveloperPayload().length(), (const BYTE*)pCheckRes->GetDeveloperPayload().c_str(), purchaseID));

		if (purchaseID.GetSize() != SHA256_DIGEST_LENGTH)
			svrErrClose(E_SVR_INVALID_PURCHASE_INFO);

		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		svrChk(pPlayerInfoSystem->SaveStatToMemento(m_SavedData));

		svrChkClose(pPlayerInfoSystem->ApplyItem(m_pShopItem));

		svrChk(pPlayerInfoSystem->SavePurchaseInfoToDB(GetTransID(), purchaseID, GetPlatform(), GetPurchaseTransactionID()));

	Proc_End:

		if (FAILED(hr))
		{
			CloseTransaction(hr);
		}

		return S_SYSTEM_OK;
	}

	HRESULT PlayerTransBuyShopItem::OnPurchaseCheckedIOS(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_SYSTEM_OK;
		auto *pCheckRes = (Svr::ExternalTransactionIOSRecepitCheck*)pRes;
		UserGamePlayerInfoSystem *pPlayerInfoSystem = nullptr;

		svrChkClose(pRes->GetHRESULT());

		if (pCheckRes->GetPurchaseTransactionID().GetSize() == 0
			|| StrUtil::StringCmp((const char*)pCheckRes->GetPurchaseTransactionID().data(), (INT)pCheckRes->GetPurchaseTransactionID().GetSize(), GetPurchaseTransactionID(), -1) != 0)
			svrErrClose(E_SVR_INVALID_PURCHASE_INFO);


		svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

		svrChk(pPlayerInfoSystem->SaveStatToMemento(m_SavedData));

		svrChkClose(pPlayerInfoSystem->ApplyItem(m_pShopItem));

		svrChk(pPlayerInfoSystem->SavePurchaseInfoToDB(GetTransID(), pCheckRes->GetPurchaseTransactionID(), GetPlatform(), GetPurchaseTransactionID()));

	Proc_End:

		if (FAILED(hr))
		{
			CloseTransaction(hr);
		}

		return S_SYSTEM_OK;
	}

	HRESULT PlayerTransBuyShopItem::OnSavedToDB( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_SYSTEM_OK;
		auto *pDBRes = (DB::QuerySavePurchaseInfoToDBCmd*)pRes;

		svrChk(pRes->GetHRESULT());

		if (pDBRes->Result < 0)
		{
			svrErrClose(E_SVR_INVALID_PURCHASE_DUPLICATED);
		}


	Proc_End:

		// if failed to write to DB, roleback the changes
		if(FAILED(hr))
		{
			m_SavedData.RestoreAllData();
		}

		CloseTransaction(hr);

		return S_SYSTEM_OK; 
	}

	// Start Transaction
	HRESULT PlayerTransBuyShopItem::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;
		Svr::ExternalTransactionManager* pExtMgr = nullptr;
		UserGamePlayerInfoSystem *pPlayerInfoSystem = nullptr;

		m_pShopItem = nullptr;

		svrChk( super::StartTransaction() );

		if (GetPurchaseTransactionID() == nullptr || GetPurchaseTransactionID()[0] == '\0')
		{
			svrErrClose(E_SVR_INVALID_PURCHASE_INFO);
		}

		if( FAILED( conspiracy::ShopTbl::FindItem( GetShopItemID(), m_pShopItem ) ) )
		{
			svrErrClose(E_GAME_INVALID_SHOPITEMID);
		}

		svrChkPtr(pExtMgr = Svr::GetServerComponent<Svr::ExternalTransactionManager>());

		if (strlen(m_pShopItem->AndroidItemID) == 0 || strlen(m_pShopItem->iOSItemID) == 0)
		{
			StaticArray<BYTE,1024> pruchaseID;

			svrChk( Util::Base64URLDecode(strlen(GetPurchaseTransactionID()), (BYTE*)GetPurchaseTransactionID(), pruchaseID) );
			//svrChk( pruchaseID.AddItems(strlen(GetPurchaseTransactionID()), (BYTE*)GetPurchaseTransactionID()) );

			svrChkPtr(pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>());

			svrChk(pPlayerInfoSystem->SaveStatToMemento(m_SavedData));

			svrChkClose(pPlayerInfoSystem->ApplyItem(m_pShopItem));

			svrChk(pPlayerInfoSystem->SavePurchaseInfoToDB(GetTransID(), pruchaseID, GetPlatform(), GetPurchaseTransactionID()));
		}
		else
		{
			if (StrUtil::StringCmpLwr("android", -1, GetPlatform(), -1) == 0
				|| StrUtil::StringCmpLwr("windows", -1, GetPlatform(), -1) == 0)
			{
				// The last byte must be null, null terminate
				svrChk(pExtMgr->AndroidCheckReceipt(GetTransID(), GetPackageName(), m_pShopItem->AndroidItemID, GetPurchaseTransactionID()));
			}
			else
			{
				if (GetPurchaseToken().GetSize() == 0)
				{
					svrErrClose(E_SVR_INVALID_PURCHASE_INFO);
				}

				svrChk(pExtMgr->IOSCheckReceipt(GetTransID(), GetPackageName(), m_pShopItem->iOSItemID, GetPurchaseTransactionID(), GetPurchaseToken()));
			}
		}


	Proc_End:

		if( FAILED(hr) )
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
	HRESULT PlayerTransSetConfigPreset::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;
		Policy::IPolicyGameInstance *pPolicy = nullptr;
		GameInsUID insUID;

		svrChk( super::StartTransaction() );

		svrChk( GetMyServer()->UpdateGameConfig(GetPresetID()) );

		svrChk( GetMyOwner()->UpdateGameConfig() );

		insUID = GetMyOwner()->GetGameInsUID();
		if( insUID != 0 )
		{
			svrChkPtr( pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(insUID.GetServerID()) );

			svrChk( pPolicy->SetConfigPresetC2SEvt( RouteContext(GetOwnerEntityUID(),insUID), GetPresetID() ) );
		}

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}



	PlayerTransGainGameResource::PlayerTransGainGameResource( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( DB::QuerySetPlayerInfoCmd, { return OnSetPlayerInfoRes(pRes); } );
	}

	HRESULT PlayerTransGainGameResource::OnSetPlayerInfoRes(  Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk( pRes->GetHRESULT() );
		//DB::QuerySetPlayerInfoCmd *pMsgRes = (DB::QuerySetPlayerInfoCmd*)pRes;


	Proc_End:

		CloseTransaction(hr);

		return S_SYSTEM_OK;
	}

	// Start Transaction
	HRESULT PlayerTransGainGameResource::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;
		UserGamePlayerInfoSystem *pPlayerInfoSystem = nullptr;
		DebugGameResource res = (DebugGameResource)GetResource();

		svrChk( super::StartTransaction() );

		if( GetResource() < 0 || GetResource() >= (int)DebugGameResource::Max )
			svrErrClose(E_SYSTEM_INVALIDARG);

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
			svrErrClose(E_SYSTEM_INVALIDARG);
			break;
		}

		svrChkPtr( pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>() );

		// Save player data
		svrChk(pPlayerInfoSystem->SavePlayerInfoToDB(GetTransID()));


	Proc_End:

		if( FAILED(hr) )
			CloseTransaction( hr );

		return hr;
	}
	



};// namespace GameServer 
};// namespace BR 

