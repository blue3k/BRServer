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

#include "Common/HRESCommon.h"
#include "Common/HRESGame.h"
#include "Common/HRESLogin.h"
#include "Common/MemoryPool.h"
#include "Common/BRBaseTypes.h"

#include "GameServerClass.h"
#include "ServerSystem/BRServerUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/ServiceEntity/GameClusterServiceEntity.h"

#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceIPolicy.h"

#include "GamePlayerEntityTransFriend.h"
#include "GameInstance/GamePlayerEntity.h"

#include "ServerSystem/BrServer.h"

#include "GameInstance/GameEntityManager.h"

#include "GameSystem/UserFriendSystem.h"
#include "GameSystem/UserGamePlayerInfoSystem.h"


#include "DB/AccountDB.h"
#include "DB/AccountQuery.h"
#include "DB/GameConspiracyDB.h"
#include "DB/GameConspiracyQuery.h"




BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransInviteFriend);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransFriendAccept);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransFriendAcceptedS2S);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransRemoveFriend);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransFriendRemovedS2S);
BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransGetFriendList);

BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransGiveStamina);


	
namespace BR {
namespace GameServer {



	/////////////////////////////////////////////////////////////////////////////
	//
	//	Friend transaction
	//

	
	PlayerTransInviteFriend::PlayerTransInviteFriend( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE(DB::QueryGetPlayerShardIDCmd, { return OnGetPlayerShardID(pRes); });
		BR_TRANS_MESSAGE(DB::QueryNotification_AddCmd, { return OnNotifyAdded(pRes); });
	}

	HRESULT PlayerTransInviteFriend::OnGetPlayerShardID(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_OK;
		Svr::ServerEntity *pServerEntity = nullptr;
		EntityUID playerUID;
		Policy::IPolicyGameServer* pTargetPolicy = nullptr;

		svrChkClose( pRes->GetHRESULT() );
		auto *pMsgRes = (DB::QueryGetPlayerShardIDCmd*)pRes;

		if(pMsgRes->Result == 0)
		{
			svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->Notification_Add(GetTransID(), pMsgRes->ShardID, GetFriendID(), true, NotificationType::FriendRequest, GetMyOwner()->GetPlayerID(), GetMyOwner()->GetFacebookUID(), GetMyOwner()->GetNickName(), m_TimeStamp));
		}
		else
		{
			svrErrClose(E_INVALID_ACCOUNTID);
		}


	Proc_End:

		if (FAILED(hr))
			CloseTransaction(hr);

		return S_OK;
		
	}

	HRESULT PlayerTransInviteFriend::OnNotifyAdded(  Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;
		Svr::ServerEntity *pServerEntity = nullptr;
		EntityUID playerUID;
		Policy::IPolicyGameServer* pTargetPolicy = nullptr;

		svrChkClose( pRes->GetHRESULT() );
		DB::QueryNotification_AddCmd *pMsgRes = (DB::QueryNotification_AddCmd*)pRes;

		if( SUCCEEDED( Svr::GetServerComponent<Svr::GameClusterServiceEntity>()->FindPlayer( GetFriendID(), playerUID )) )
		{
			svrChk( Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity( playerUID.SvrID, pServerEntity ) );
			pTargetPolicy = pServerEntity->GetPolicy<Policy::IPolicyGameServer>();
			svrChkPtr(pTargetPolicy);
			pTargetPolicy->NotifyC2SEvt( RouteContext(GetOwnerEntityUID(),playerUID), 
				GetFriendID(), pMsgRes->NotificationID, NotificationType::FriendRequest, GetMyOwner()->GetPlayerID(), GetMyOwner()->GetFacebookUID(), GetMyOwner()->GetNickName(), m_TimeStamp.time_since_epoch().count() );
		}

	Proc_End:

		CloseTransaction(hr);

		return S_OK;
		
	}
	// Start Transaction
	HRESULT PlayerTransInviteFriend::StartTransaction()
	{
		HRESULT hr = S_OK;

		m_TimeStamp = Util::Time.GetTimeUTCSec();

		svrChk( __super::StartTransaction() );

		if (!GetMyOwner()->GetComponent<UserFriendSystem>()->CanAddFriend())
			svrErrClose(E_MAX_FRIEND);

		svrChk( Svr::GetServerComponent<DB::AccountDB>()->GetPlayerShardID( GetTransID(), GetFriendID() ) );

	Proc_End:

		if( FAILED(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}



	PlayerTransFriendAccept::PlayerTransFriendAccept( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE(DB::QueryAddFriendCmd, { return OnFriendAdded(pRes); });
		BR_TRANS_MESSAGE(DB::QueryGetFriendSlotStatusCmd, { return OnFriendSlotStatus(pRes); });
		//BR_TRANS_MESSAGE(DB::QueryGetNickNameCmd, { return OnGetNickName(pRes); });
		BR_TRANS_MESSAGE(DB::QueryGetFriendQuickInfoWithNickCmd, { return OnFriendQuickInfo(pRes); });
		BR_TRANS_MESSAGE(DB::QueryGetPlayerShardIDCmd, { return OnGetPlayerShardID(pRes); });
	}

	HRESULT PlayerTransFriendAccept::OnGetPlayerShardID(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_OK;

		svrChk(pRes->GetHRESULT());

		auto *pDBRes = (DB::QueryGetPlayerShardIDCmd*)pRes;
		if (pDBRes->Result < 0)
			svrErr(E_INVALID_PLAYERID);

		m_NewFriend.ShardID = pDBRes->ShardID;

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->GetFriendSlotStatus(GetTransID(), m_NewFriend.ShardID, GetInviterID()));

	Proc_End:

		if (FAILED(hr))
			CloseTransaction(hr);

		return hr;
	}

	HRESULT PlayerTransFriendAccept::OnFriendSlotStatus(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_OK;

		svrChk(pRes->GetHRESULT());

		auto *pDBRes = (DB::QueryGetFriendSlotStatusCmd*)pRes;
		if (pDBRes->Result < 0)
			svrErr(E_INVALID_PLAYERID);

		svrChkPtr(GetMyServer()->GetPresetGameConfig());

		auto maxFriend = pDBRes->AddedFriendSlot + GetMyServer()->GetPresetGameConfig()->DefaultFriend;

		if (pDBRes->NumFriends >= maxFriend)
			svrErrClose(E_TARGET_MAX_FRIEND);


		{
			auto myShardID = GetMyOwner()->GetShardID();

			// add db 
			m_WaitingResultCount = 0;
			svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->AddFriend(GetTransID(), myShardID, GetMyOwner()->GetAccountID(), GetInviterID(), m_NewFriend.ShardID, GetInviterFacebookUID())); m_WaitingResultCount++;
			svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->AddFriend(GetTransID(), m_NewFriend.ShardID, GetInviterID(), GetMyOwner()->GetAccountID(), myShardID, GetMyOwner()->GetFacebookUID())); m_WaitingResultCount++;
		}

	Proc_End:

		if (FAILED(hr))
			CloseTransaction(hr);

		return hr;
	}

	HRESULT PlayerTransFriendAccept::OnFriendAdded(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_OK;
		UserFriendSystem *pFriendSystem = nullptr;

		svrChk(pRes->GetHRESULT());


		auto *pDBRes = (DB::QueryAddFriendCmd*)pRes;
		if( pDBRes->Result >= 0 && GetInviterID() == pDBRes->FriendUID )
		{
			m_NewFriend.PlayerID = pDBRes->FriendUID;
			m_NewFriend.FacebookUID = pDBRes->FriendFacebookUID;
		}

		if (m_WaitingResultCount == 1) // the last add result
		{
			pFriendSystem = GetMyOwner()->GetComponent<UserFriendSystem>();
			if (pFriendSystem != nullptr)
			{
				hr = pFriendSystem->AddFriend(m_NewFriend);
				if (FAILED(hr))
				{
					if (hr != E_MAX_FRIEND) // silence max friend error
						svrErr(hr);
				}

			}
		}


	Proc_End:

		m_WaitingResultCount --;
		if( m_WaitingResultCount <= 0 )
		{
			SendNotifyToInviter();


			if (m_NewFriend.PlayerID != 0)
			{
				m_WaitingResultCount = 0;

				if (SUCCEEDED(Svr::GetServerComponent<DB::GameConspiracyDB>()->GetFriendQuickInfoWithNickCmd(GetTransID(), m_NewFriend.ShardID, m_NewFriend.PlayerID)))
					m_WaitingResultCount++;
			}
			else
			{
				CloseTransaction(hr);
			}
		}

		return S_OK; 
	}

	HRESULT PlayerTransFriendAccept::OnFriendQuickInfo(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_OK;
		ServerFriendInformation *pFriend = nullptr;

		svrChk(pRes->GetHRESULT());

		auto *pDBRes = (DB::QueryGetFriendQuickInfoWithNickCmd*)pRes;
		if (pDBRes->Result < 0 )
			svrErr(E_INVALID_PLAYERID);

		svrChkPtr(pFriend = GetMyOwner()->GetComponent<UserFriendSystem>()->GetFriend(pDBRes->PlayerID));
		StrUtil::StringCpy(pFriend->NickName, pDBRes->GameNick);
		pFriend->Level = pDBRes->Level;
		pFriend->WeeklyWin = pDBRes->WeeklyWin;
		pFriend->WeeklyLose = pDBRes->WeeklyLose;
		pFriend->IsPlayingGame = pDBRes->PlayerState != 0 ? 1 : 0;
		pFriend->LastActiveTime = pDBRes->LatestActiveTime;

		m_NewFriend = *pFriend;

	Proc_End:

		m_WaitingResultCount--;
		if (m_WaitingResultCount <= 0)
			CloseTransaction(hr);

		return S_OK;
	}
/*
	HRESULT PlayerTransFriendAccept::OnGetNickName( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;
		ServerFriendInformation *pFriend = nullptr;

		svrChk(pRes->GetHRESULT());

		auto *pDBRes = (DB::QueryGetNickNamesCmd*)pRes;
		if( pDBRes->Result < 0 || pDBRes->m_RowsetResult.size() == 0 )
			svrErrClose(E_INVALID_PLAYERID);

		{
			auto& result = pDBRes->m_RowsetResult.begin();
			svrChk(StrUtil::StringCpy(m_NewFriend.NickName, result->NickName));

			svrChkPtr(pFriend = GetMyOwner()->GetComponent<UserFriendSystem>()->GetFriend(result->UserID));
			svrChk(StrUtil::StringCpy(pFriend->NickName, result->NickName));
		}


	Proc_End:

		m_WaitingResultCount--;
		if (m_WaitingResultCount <= 0)
			CloseTransaction(hr);

		return S_OK; 
	}
*/
	HRESULT PlayerTransFriendAccept::SendNotifyToInviter()
	{
		HRESULT hr = S_OK;
		Svr::ServerEntity *pServerEntity = nullptr;
		EntityUID playerUID;
		Policy::IPolicyGameServer* pTargetPolicy = nullptr;

		if (FAILED(Svr::GetServerComponent<Svr::GameClusterServiceEntity>()->FindPlayer(GetInviterID(), playerUID)))
		{
			goto Proc_End;
		}

		svrChk( Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity( playerUID.SvrID, pServerEntity ) );

		svrChkPtr( pTargetPolicy = pServerEntity->GetPolicy<Policy::IPolicyGameServer>() );

		svrChk( pTargetPolicy->FriendAcceptedC2SEvt( RouteContext(GetOwnerEntityUID(),playerUID), 
			GetInviterID(), GetMyOwner()->GetFriendInformation() ) );

	Proc_End:

		return hr;
	}

	// Start Transaction
	HRESULT PlayerTransFriendAccept::StartTransaction()
	{
		HRESULT hr = S_OK;

		m_WaitingResultCount = 0;
		memset( &m_NewFriend, 0, sizeof(m_NewFriend) );

		svrChk( __super::StartTransaction() );

		if (!GetMyOwner()->GetComponent<UserFriendSystem>()->CanAddFriend())
			svrErrClose(E_MAX_FRIEND);

		// TODO: we need to check inviter's maximum friend slot
		svrChk(Svr::GetServerComponent<DB::AccountDB>()->GetPlayerShardID(GetTransID(), GetInviterID()));


	Proc_End:

		if( FAILED(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}


	// Start Transaction
	HRESULT PlayerTransFriendAcceptedS2S::StartTransaction()
	{
		HRESULT hr = S_OK;
		Policy::ISvrPolicyGame *pPolicy = nullptr;

		svrChk( __super::StartTransaction() );

		if( GetMyOwner()->GetPlayerID() != GetDestPlayerID() )
		{
			svrErr( E_INVALID_PLAYERID );
		}

		hr = GetMyOwner()->GetComponent<UserFriendSystem>()->AddFriend( GetAccepter() );
		if (hr == E_MAX_FRIEND)
		{
			svrErrClose(hr);
		}
		svrChk(hr);

		svrChkPtr(pPolicy = GetPolicy<Policy::ISvrPolicyGame>());
		svrChk(pPolicy->FriendRequestAcceptedS2CEvt(GetAccepter()));

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}



	PlayerTransRemoveFriend::PlayerTransRemoveFriend( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE( DB::QueryRemoveFriendCmd, { return OnRemoved(pRes); });
	}

	HRESULT PlayerTransRemoveFriend::OnRemoved( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;

		svrChk(pRes->GetHRESULT());

		DB::QueryRemoveFriendCmd *pDBRes = (DB::QueryRemoveFriendCmd*)pRes;

		svrChk( GetMyOwner()->GetComponent<UserFriendSystem>()->RemoveFriend( pDBRes->FriendUID ) );

	Proc_End:

		m_WaitingResultCount--;
		if( m_WaitingResultCount <= 0 )
			CloseTransaction(hr);

		return S_OK; 
	}

	// Start Transaction
	HRESULT PlayerTransRemoveFriend::StartTransaction()
	{
		HRESULT hr = S_OK;
		Policy::IPolicyGameServer *pPolicy = nullptr;
		Svr::ServerEntity *pServerEntity = nullptr;
		EntityUID playerUID;
		ServerFriendInformation* pFriendInfo = nullptr;

		m_WaitingResultCount = 0;

		svrChk( __super::StartTransaction() );

		pFriendInfo = GetMyOwner()->GetComponent<UserFriendSystem>()->GetFriend(GetFriendID());
		if (pFriendInfo == nullptr)
			svrErrClose(E_INVALID_PLAYERID);

		// Find player and notify to remove
		if( SUCCEEDED(Svr::GetServerComponent<Svr::GameClusterServiceEntity>()->FindPlayer( GetFriendID(), playerUID )) )
		{
			svrChk( Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity( playerUID.SvrID, pServerEntity ) );

			pPolicy = pServerEntity->GetPolicy<Policy::IPolicyGameServer>();

			svrChk( pPolicy->FriendRemovedC2SEvt( RouteContext(GetOwnerEntityUID(),playerUID), 
				GetFriendID(), GetMyOwner()->GetPlayerID() ) );
		}

		// Reset 
		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->RemoveFriend(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetAccountID(), GetFriendID())); m_WaitingResultCount++;
		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->RemoveFriend(GetTransID(), pFriendInfo->ShardID, GetFriendID(), GetMyOwner()->GetAccountID())); m_WaitingResultCount++;

	Proc_End:

		if( FAILED(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}
	


	// Start Transaction
	HRESULT PlayerTransFriendRemovedS2S::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( __super::StartTransaction() );

		if( GetMyOwner()->GetPlayerID() != GetDestPlayerID() )
		{
			svrErr( E_INVALID_PLAYERID );
		}

		svrChk( GetMyOwner()->GetComponent<UserFriendSystem>()->RemoveFriend( GetRemoverID() ) );
		auto pPolicy = GetPolicy();
		svrChkPtr(pPolicy);
		svrChk(pPolicy->FriendRemovedS2CEvt(GetRemoverID()));

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}



	PlayerTransGetFriendList::PlayerTransGetFriendList( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		BR_TRANS_MESSAGE(DB::QueryGetFriendListCmd, { return OnGetList(pRes); });
		//BR_TRANS_MESSAGE(DB::QueryGetFriendQuickInfoCmd, { return OnGetQuickInfo(pRes); });
		BR_TRANS_MESSAGE(DB::QueryGetFriendQuickInfoWithNickCmd, { return OnGetQuickInfoWithNick(pRes); });
		//BR_TRANS_MESSAGE(DB::QueryGetNickNameCmd, { return OnGetNickNames(pRes); });
	}

	HRESULT PlayerTransGetFriendList::OnGetList( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;
		UserFriendSystem *pFriendSystem = nullptr;

		m_WaitingCount = 0;

		svrChk(pRes->GetHRESULT());

		svrChkPtr( pFriendSystem = GetMyOwner()->GetComponent<UserFriendSystem>() );

		pFriendSystem->ClearFriendList();

		DB::QueryGetFriendListCmd *pDBRes = (DB::QueryGetFriendListCmd*)pRes;
		m_Friends.Reserve( pDBRes->m_RowsetResult.size() );
		std::for_each( pDBRes->m_RowsetResult.begin(), pDBRes->m_RowsetResult.end(), [&]( DB::QueryGetFriendListSet &set )
		{
			ServerFriendInformation info(set.FriendUID, set.FriendShardID, set.FriendFacebookUID, "", 1, 0, 0, FALSE, 0, set.FriendStaminaTime);

			HRESULT hRes = pFriendSystem->AddFriend(info);
			if (hRes == E_MAX_FRIEND)
			{
				svrTrace(Trace::TRC_WARN, "Failed to add friend. Max friends, PlayerID:%0% to friend system", set.FriendUID);
				return;
			}
			else if (FAILED(hr))
			{
				svrTrace(Trace::TRC_ERROR, "Failed to add friend PlayerID:%0% to friend system", set.FriendUID);
				return;
			}

			if (SUCCEEDED(Svr::GetServerComponent<DB::GameConspiracyDB>()->GetFriendQuickInfoWithNickCmd(GetTransID(), info.ShardID, info.PlayerID)))
				m_WaitingCount++;
		});

		m_TotalNumberOfFriends = pFriendSystem->GetNumberOfFriends();


	Proc_End:

		if( FAILED(hr) || m_WaitingCount <= 0 )
			CloseTransaction(hr);

		return S_OK; 
	}
/*
	HRESULT PlayerTransGetFriendList::OnGetQuickInfo( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;
		FriendInformation *pFriend = nullptr;
		DB::QueryGetFriendQuickInfoCmd *pDBRes = nullptr;

		svrChk(pRes->GetHRESULT());

		pDBRes = (DB::QueryGetFriendQuickInfoCmd*)pRes;

		if( pDBRes->Result == 0 )
		{
			svrChkPtr(pFriend = GetMyOwner()->GetComponent<UserFriendSystem>()->GetFriend(pDBRes->PlayerID));
			pFriend->Level = pDBRes->Level;
			pFriend->WeeklyWin = pDBRes->WeeklyWin;
			pFriend->WeeklyLose = pDBRes->WeeklyLose;
			pFriend->IsPlayingGame = pDBRes->PlayerState != 0 ? 1 : 0;
			pFriend->LastActiveTime = pDBRes->LatestActiveTime;
		}
		else
		{
			svrErr(E_INVALID_PLAYERID);
		}

	Proc_End:

		if( FAILED(hr) )
		{
			svrTrace( Trace::TRC_ERROR, "Failed to get friend level PlayerID:%0%, hr=%1%", pDBRes ? pDBRes->UserID : 0, ArgHex32(hr) );
		}

		m_WaitingCount--;
		if( m_WaitingCount <= 0 )
			CloseTransaction(hr);

		return S_OK; 
	}
*/

	HRESULT PlayerTransGetFriendList::OnGetQuickInfoWithNick(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_OK;
		FriendInformation *pFriend = nullptr;
		DB::QueryGetFriendQuickInfoWithNickCmd *pDBRes = nullptr;

		svrChk(pRes->GetHRESULT());

		pDBRes = (DB::QueryGetFriendQuickInfoWithNickCmd*)pRes;

		if( pDBRes->Result == 0 )
		{
			svrChkPtr( pFriend = GetMyOwner()->GetComponent<UserFriendSystem>()->GetFriend(pDBRes->PlayerID) );
			StrUtil::StringCpy(pFriend->NickName, pDBRes->GameNick);
			pFriend->Level = pDBRes->Level;
			pFriend->WeeklyWin = pDBRes->WeeklyWin;
			pFriend->WeeklyLose = pDBRes->WeeklyLose;
			pFriend->IsPlayingGame = pDBRes->PlayerState != 0 ? 1 : 0;
			pFriend->LastActiveTime = pDBRes->LatestActiveTime;
		}
		else
		{
			svrErr(E_INVALID_PLAYERID);
		}

	Proc_End:

		if( FAILED(hr) )
		{
			svrTrace(Trace::TRC_ERROR, "Failed to get friend level PlayerID:%0%, hr={1:X8}", pDBRes ? pDBRes->PlayerID : 0, hr);
		}

		m_WaitingCount--;
		if( m_WaitingCount <= 0 )
			CloseTransaction(hr);

		return S_OK; 
	}


	// Start Transaction
	HRESULT PlayerTransGetFriendList::StartTransaction()
	{
		HRESULT hr = S_OK;

		m_MaxFriendSlot = 0;
		m_TotalNumberOfFriends = 0;

		svrChk( __super::StartTransaction() );

		m_WaitingCount = 0;

		auto friendSystem = GetMyOwner()->GetComponent<UserFriendSystem>();
		m_TotalNumberOfFriends = friendSystem->GetNumberOfFriends();
		if (m_TotalNumberOfFriends == 0)
		{
			svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->GetFriendList(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetAccountID()));
		}
		else
		{
			//if (Util::TimeSince(friendSystem->GetLatestStatusSync()) > UserFriendSystem::STATUS_SYNC_TIME)
			{
				friendSystem->ForeachFriends(0, m_TotalNumberOfFriends, [&](const ServerFriendInformation &friendInfo)
				{
					if (SUCCEEDED(Svr::GetServerComponent<DB::GameConspiracyDB>()->GetFriendQuickInfoWithNickCmd(GetTransID(), friendInfo.ShardID, friendInfo.PlayerID)))
						m_WaitingCount++;
					return S_OK;
				});

				friendSystem->SetLatestStatusSync(Util::Time.GetTimeMs());
			}

			if (m_WaitingCount == 0)
				CloseTransaction(hr);
		}

	Proc_End:

		if( FAILED(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}

	HRESULT PlayerTransGetFriendList::CloseTransaction( HRESULT hr )
	{
		if( IsClosed() )
			return S_OK;

		auto friendSystem = GetMyOwner()->GetComponent<UserFriendSystem>();

		m_MaxFriendSlot = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GetFriendSlot();

		m_Friends.Clear();
		auto maxRequest = Util::Min(GetCount(), (ushort)UserFriendSystem::MAX_FRIEND_REQUEST);
		friendSystem->ForeachFriends(GetStartIndex(), maxRequest, [&](const FriendInformation& friendInfo)
		{
			m_Friends.push_back(friendInfo);
			return S_OK;
		});

		return __super::CloseTransaction(hr);
	}




	PlayerTransGiveStamina::PlayerTransGiveStamina( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		SetExclusive(true);
		BR_TRANS_MESSAGE( DB::QueryUpdateTickStatusCmd, { return OnSavedToDB(pRes); });
		BR_TRANS_MESSAGE( DB::QueryUpdateFriendStaminaTimeCmd, { return OnUpdateTime(pRes); });
		BR_TRANS_MESSAGE( DB::QueryNotification_AddCmd, { return OnNotifyAdded(pRes); } );
	}


	HRESULT PlayerTransGiveStamina::OnSavedToDB( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;
		DB::QueryUpdateTickStatusCmd *pDBRes = (DB::QueryUpdateTickStatusCmd*)pRes;


	Proc_End:


		m_WaitingQueries--;

		// if failed to write to DB, roleback the changes
		if(FAILED(hr))
		{
			svrTrace( Trace::TRC_ERROR, "Failed to save give stamina result PlayerID:%0%, Dest:%1%, hr:{2:X8}", GetMyOwner()->GetPlayerID(), GetTargetPlayer(), hr );
			CloseTransaction(hr);
		}

		if (m_WaitingQueries <= 0)
			CloseTransaction(hr);

		return S_OK; 
	}
	
	HRESULT PlayerTransGiveStamina::OnUpdateTime( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;
		DB::QueryUpdateFriendStaminaTimeCmd *pDBRes = (DB::QueryUpdateFriendStaminaTimeCmd*)pRes;
		svrChk(pRes->GetHRESULT());


	Proc_End:

		// if failed to write to DB, roleback the changes
		if(FAILED(hr))
		{
			svrTrace( Trace::TRC_ERROR, "Failed to save give-stamina timestamp PlayerID:%0%, Dest:%1%", GetMyOwner()->GetPlayerID(), GetTargetPlayer() );
		}

		m_WaitingQueries--;
		if( m_WaitingQueries <= 0 )
		{
			CloseTransaction( hr );
		}

		return S_OK; 
	}

	HRESULT PlayerTransGiveStamina::OnNotifyAdded(  Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;
		Svr::ServerEntity *pServerEntity = nullptr;
		EntityUID playerUID;
		Policy::IPolicyGameServer* pTargetPolicy = nullptr;

		svrChkClose( pRes->GetHRESULT() );
		DB::QueryNotification_AddCmd *pMsgRes = (DB::QueryNotification_AddCmd*)pRes;

		if( SUCCEEDED( Svr::GetServerComponent<Svr::GameClusterServiceEntity>()->FindPlayer( GetTargetPlayer(), playerUID )) )
		{
			svrChk( Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity( playerUID.SvrID, pServerEntity ) );
			pTargetPolicy = pServerEntity->GetPolicy<Policy::IPolicyGameServer>();
			svrChkPtr(pTargetPolicy);
			pTargetPolicy->NotifyC2SEvt( RouteContext(GetOwnerEntityUID(),playerUID), 
				GetTargetPlayer(), pMsgRes->NotificationID, NotificationType::GiftStamina, GetMyOwner()->GetPlayerID(), GetMyOwner()->GetFacebookUID(), GetMyOwner()->GetNickName(), m_TimeStamp.time_since_epoch().count());
		}

	Proc_End:

		m_WaitingQueries--;
		if( m_WaitingQueries <= 0 )
		{
			CloseTransaction( hr );
		}

		return S_OK;
		
	}


	// Start Transaction
	HRESULT PlayerTransGiveStamina::StartTransaction()
	{
		HRESULT hr = S_OK;
		UserGamePlayerInfoSystem *pPlayerInfoSystem = nullptr;
		Svr::ServerEntity *pServerEntity = nullptr;
		EntityUID playerUID;
		Policy::IPolicyGameServer* pTargetPolicy = nullptr;

		m_WaitingQueries = 0;
		m_TimeStamp = Util::Time.GetTimeUTCSec();

		svrChk( __super::StartTransaction() );

		svrChkPtr( pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>() );

		//GetMyOwner()->UpdateGamePlayer();

		//if( pPlayerInfoSystem->GetStamina() <= 0 )
		//	svrErrClose(E_GAME_NOTENOUGH_RESOURCE);


		auto pFriend = GetMyOwner()->GetComponent<UserFriendSystem>()->GetFriend(GetTargetPlayer());
		if( pFriend == nullptr )
			svrErrClose(E_PLAYER_NOT_FOUND);

		//svrChk( pPlayerInfoSystem->GainStamina( -1 ) );

		//GetMyOwner()->AddGameTransactionLog(TransLogCategory::Give, -1, 0, GetTargetPlayer());


		//// Save my status
		//if (SUCCEEDED(GetMyOwner()->UpdateDBSync(GetTransID())))
		//	m_WaitingQueries++;


		//Update give stamina time
		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->UpdateFriendStaminaTime(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(), GetTargetPlayer(), m_TimeStamp));
		pFriend->LastStaminaSent = m_TimeStamp.time_since_epoch().count();
		m_WaitingQueries++;


		// Add stamina notification
		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->Notification_Add(GetTransID(), pFriend->ShardID, pFriend->PlayerID, true, NotificationType::GiftStamina, GetMyOwner()->GetPlayerID(), GetMyOwner()->GetFacebookUID(), GetMyOwner()->GetNickName(), m_TimeStamp));
		m_WaitingQueries++;



	Proc_End:

		if( FAILED(hr) || m_WaitingQueries == 0 )
		{
			CloseTransaction( hr );
		}

		return hr;
	}



	//PlayerTransGiveStaminaS2S::PlayerTransGiveStaminaS2S( Message::MessageData* &pIMsg )
	//	:UserTransactionS2SEvt(pIMsg)
	//{
	//	SetExclusive(true);
	//	BR_TRANS_MESSAGE( DB::QueryUpdateTickStatusCmd, { return OnSavedToDB(pRes); });
	//}


	//HRESULT PlayerTransGiveStaminaS2S::OnSavedToDB( Svr::TransactionResult* &pRes )
	//{
	//	HRESULT hr = S_OK;
	//	DB::QueryUpdateTickStatusCmd *pDBRes = (DB::QueryUpdateTickStatusCmd*)pRes;
	//	svrChk(pRes->GetHRESULT());


	//Proc_End:

	//	if( FAILED(hr) )
	//	{
	//		svrTrace( Trace::TRC_ERROR, "Failed to save received stamina result PlayerID:%0%, Sender:%1%", GetDestPlayerID(), GetSenderID() );
	//	}

	//	CloseTransaction(hr);

	//	return S_OK; 
	//}


	//// Start Transaction
	//HRESULT PlayerTransGiveStaminaS2S::StartTransaction()
	//{
	//	HRESULT hr = S_OK;
	//	UserGamePlayerInfoSystem *pPlayerInfoSystem = nullptr;

	//	svrChk( __super::StartTransaction() );

	//	if( GetMyOwner()->GetPlayerID() != GetDestPlayerID() )
	//		svrErrClose(E_INVALID_PLAYERID);

	//	svrChkPtr( pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>() );

	//	svrChk( pPlayerInfoSystem->GainStamina( 1 ) );

	//	svrChk(GetMyOwner()->UpdateDBSync(GetTransID()));

	//Proc_End:

	//	if( FAILED(hr) )
	//	{
	//		CloseTransaction( hr );
	//	}

	//	return hr;
	//}


	


};// namespace GameServer 
};// namespace BR 

