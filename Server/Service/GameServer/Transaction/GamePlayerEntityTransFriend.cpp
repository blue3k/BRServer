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

#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "ResultCode/SFResultCodeLogin.h"
#include "Memory/MemoryPool.h"
#include "Types/SFEngineTypedefs.h"

#include "GameServerClass.h"
#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "ServerEntity/ServerEntityManager.h"
#include "ServiceEntity/Game/PlayerManagerServiceEntity.h"

#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"

#include "GamePlayerEntityTransFriend.h"
#include "GameInstance/GamePlayerEntity.h"

#include "Server/BrServer.h"


#include "GameSystem/UserFriendSystem.h"
#include "GameSystem/UserGamePlayerInfoSystem.h"


#include "DB/AccountDB.h"
#include "DB/AccountQuery.h"
#include "DB/GameConspiracyDB.h"
#include "DB/GameConspiracyQuery.h"




SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransInviteFriend);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransFriendAccept);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransFriendAcceptedS2S);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransRemoveFriend);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransFriendRemovedS2S);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransGetFriendList);

SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransGiveStamina);


	
namespace SF {
namespace GameServer {



	/////////////////////////////////////////////////////////////////////////////
	//
	//	Friend transaction
	//

	
	PlayerTransInviteFriend::PlayerTransInviteFriend(IHeap& heap, MessageDataPtr &pIMsg )
		:MessageTransaction( heap, pIMsg )
	{
		BR_TRANS_MESSAGE(DB::QueryGetPlayerShardIDCmd, { return OnGetPlayerShardID(pRes); });
		BR_TRANS_MESSAGE(DB::QueryNotification_AddCmd, { return OnNotifyAdded(pRes); });
	}

	Result PlayerTransInviteFriend::OnGetPlayerShardID(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		EntityUID playerUID;
		DB::QueryGetPlayerShardIDCmd* pMsgRes;

		svrChkClose( pRes->GetResult() );
		pMsgRes = (DB::QueryGetPlayerShardIDCmd*)pRes;

		if(pMsgRes->Result == 0)
		{
			svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->Notification_Add(GetTransID(), pMsgRes->ShardID, GetFriendID(), true, NotificationType::FriendRequest, GetMyOwner()->GetPlayerID(), GetMyOwner()->GetFacebookUID(), GetMyOwner()->GetNickName(), m_TimeStamp));
		}
		else
		{
			svrErrClose(ResultCode::INVALID_ACCOUNTID);
		}


	Proc_End:

		if (!(hr))
			CloseTransaction(hr);

		return ResultCode::SUCCESS;
		
	}

	Result PlayerTransInviteFriend::OnNotifyAdded(  Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ServerEntity *pServerEntity = nullptr;
		EntityUID playerUID;
		DB::QueryNotification_AddCmd *pMsgRes;

		svrChkClose( pRes->GetResult() );
		pMsgRes = (DB::QueryNotification_AddCmd*)pRes;

		if( ( Service::PlayerManager->FindPlayer( GetFriendID(), playerUID )) )
		{
			svrChk( Service::ServerEntityManager->GetServerEntity( playerUID.GetServerID(), pServerEntity ) );

			Policy::NetPolicyGameServer(pServerEntity->GetConnection()).NotifyC2SEvt( RouteContext(GetOwnerEntityUID(),playerUID),
				GetFriendID(), pMsgRes->NotificationID, NotificationType::FriendRequest, GetMyOwner()->GetPlayerID(), GetMyOwner()->GetFacebookUID(), GetMyOwner()->GetNickName(), m_TimeStamp.time_since_epoch().count() );
		}

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
		
	}
	// Start Transaction
	Result PlayerTransInviteFriend::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		m_TimeStamp = Util::Time.GetTimeUTCSec();

		svrChk( super::StartTransaction() );

		if (!GetMyOwner()->GetComponent<UserFriendSystem>()->CanAddFriend())
			svrErrClose(ResultCode::MAX_FRIEND);

		if (!GetMyOwner()->GetComponent<UserFriendSystem>()->IsFriend(GetFriendID()))
			svrErrClose(ResultCode::ALREADY_IN_FRIEND);

		svrChk( Svr::GetServerComponent<DB::AccountDB>()->GetPlayerShardID( GetTransID(), GetFriendID() ) );

	Proc_End:

		if( !(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}



	PlayerTransFriendAccept::PlayerTransFriendAccept( IHeap& heap, MessageDataPtr &pIMsg )
		:MessageTransaction(heap, pIMsg )
	{
		BR_TRANS_MESSAGE(DB::QueryAddFriendCmd, { return OnFriendAdded(pRes); });
		BR_TRANS_MESSAGE(DB::QueryGetFriendSlotStatusCmd, { return OnFriendSlotStatus(pRes); });
		//BR_TRANS_MESSAGE(DB::QueryGetNickNameCmd, { return OnGetNickName(pRes); });
		BR_TRANS_MESSAGE(DB::QueryGetFriendQuickInfoWithNickCmd, { return OnFriendQuickInfo(pRes); });
		BR_TRANS_MESSAGE(DB::QueryGetPlayerShardIDCmd, { return OnGetPlayerShardID(pRes); });
	}

	Result PlayerTransFriendAccept::OnGetPlayerShardID(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		DB::QueryGetPlayerShardIDCmd* pDBRes;

		svrChk(pRes->GetResult());

		pDBRes = (DB::QueryGetPlayerShardIDCmd*)pRes;
		if (pDBRes->Result < 0)
			svrErr(ResultCode::INVALID_PLAYERID);

		m_NewFriend.ShardID = pDBRes->ShardID;

		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->GetFriendSlotStatus(GetTransID(), m_NewFriend.ShardID, GetInviterID()));

	Proc_End:

		if (!(hr))
			CloseTransaction(hr);

		return hr;
	}

	Result PlayerTransFriendAccept::OnFriendSlotStatus(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		auto *pDBRes = (DB::QueryGetFriendSlotStatusCmd*)pRes;
		INT maxFriend;

		svrChk(pRes->GetResult());

		if (pDBRes->Result < 0)
			svrErr(ResultCode::INVALID_PLAYERID);

		svrChkPtr(GetMyServer()->GetPresetGameConfig());

		maxFriend = pDBRes->AddedFriendSlot + GetMyServer()->GetPresetGameConfig()->DefaultFriend;

		if (pDBRes->NumFriends >= maxFriend)
			svrErrClose(ResultCode::TARGET_MAX_FRIEND);


		{
			auto myShardID = GetMyOwner()->GetShardID();

			// add db 
			m_WaitingResultCount = 0;
			svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->AddFriend(GetTransID(), myShardID, GetMyOwner()->GetAccountID(), GetInviterID(), m_NewFriend.ShardID, GetInviterFacebookUID())); m_WaitingResultCount++;
			svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->AddFriend(GetTransID(), m_NewFriend.ShardID, GetInviterID(), GetMyOwner()->GetAccountID(), myShardID, GetMyOwner()->GetFacebookUID())); m_WaitingResultCount++;
		}

	Proc_End:

		if (!(hr))
			CloseTransaction(hr);

		return hr;
	}

	Result PlayerTransFriendAccept::OnFriendAdded(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		UserFriendSystem *pFriendSystem = nullptr;
		auto *pDBRes = (DB::QueryAddFriendCmd*)pRes;

		svrChk(pRes->GetResult());


		if( pDBRes->Result >= 0 && ((decltype(pDBRes->FriendUID))GetInviterID()) == pDBRes->FriendUID )
		{
			m_NewFriend.PlayerID = pDBRes->FriendUID;
			m_NewFriend.FBUID = pDBRes->FriendFacebookUID;
		}

		if (m_WaitingResultCount == 1) // the last add result
		{
			pFriendSystem = GetMyOwner()->GetComponent<UserFriendSystem>();
			if (pFriendSystem != nullptr)
			{
				hr = pFriendSystem->AddFriend(m_NewFriend);
				if (!(hr))
				{
					if (hr != Result(ResultCode::MAX_FRIEND)) // silence max friend error
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

				if ((Svr::GetServerComponent<DB::GameConspiracyDB>()->GetFriendQuickInfoWithNickCmd(GetTransID(), m_NewFriend.ShardID, m_NewFriend.PlayerID)))
					m_WaitingResultCount++;
			}
			else
			{
				CloseTransaction(hr);
			}
		}

		return ResultCode::SUCCESS; 
	}

	Result PlayerTransFriendAccept::OnFriendQuickInfo(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		ServerFriendInformation *pFriend = nullptr;
		auto *pDBRes = (DB::QueryGetFriendQuickInfoWithNickCmd*)pRes;

		svrChk(pRes->GetResult());

		if (pDBRes->Result < 0 )
			svrErr(ResultCode::INVALID_PLAYERID);

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

		return ResultCode::SUCCESS;
	}
/*
	Result PlayerTransFriendAccept::OnGetNickName( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		ServerFriendInformation *pFriend = nullptr;

		svrChk(pRes->GetResult());

		auto *pDBRes = (DB::QueryGetNickNamesCmd*)pRes;
		if( pDBRes->Result < 0 || pDBRes->m_RowsetResult.size() == 0 )
			svrErrClose(ResultCode::INVALID_PLAYERID);

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

		return ResultCode::SUCCESS; 
	}
*/
	Result PlayerTransFriendAccept::SendNotifyToInviter()
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ServerEntity *pServerEntity = nullptr;
		EntityUID playerUID;

		if (!(Service::PlayerManager->FindPlayer(GetInviterID(), playerUID)))
		{
			goto Proc_End;
		}

		svrChk( Service::ServerEntityManager->GetServerEntity( playerUID.GetServerID(), pServerEntity ) );

		svrChk(Policy::NetPolicyGameServer(pServerEntity->GetConnection()).FriendAcceptedC2SEvt( RouteContext(GetOwnerEntityUID(),playerUID),
			GetInviterID(), GetMyOwner()->GetFriendInformation() ) );

	Proc_End:

		return hr;
	}

	// Start Transaction
	Result PlayerTransFriendAccept::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		m_WaitingResultCount = 0;
		memset( &m_NewFriend, 0, sizeof(m_NewFriend) );

		svrChk( super::StartTransaction() );

		if (!GetMyOwner()->GetComponent<UserFriendSystem>()->CanAddFriend())
			svrErrClose(ResultCode::MAX_FRIEND);

		// TODO: we need to check inviter's maximum friend slot
		svrChk(Svr::GetServerComponent<DB::AccountDB>()->GetPlayerShardID(GetTransID(), GetInviterID()));


	Proc_End:

		if( !(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}


	// Start Transaction
	Result PlayerTransFriendAcceptedS2S::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetPlayerID() != GetDestPlayerID() )
		{
			svrErr( ResultCode::INVALID_PLAYERID );
		}

		hr = GetMyOwner()->GetComponent<UserFriendSystem>()->AddFriend( GetAccepter() );
		if (hr == Result(ResultCode::MAX_FRIEND))
		{
			svrErrClose(hr);
		}
		svrChk(hr);

		svrChk(Policy::NetSvrPolicyGame(GetConnection()).FriendRequestAcceptedS2CEvt(GetAccepter()));

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}



	PlayerTransRemoveFriend::PlayerTransRemoveFriend( IHeap& heap, MessageDataPtr &pIMsg )
		:MessageTransaction(heap, pIMsg )
	{
		BR_TRANS_MESSAGE( DB::QueryRemoveFriendCmd, { return OnRemoved(pRes); });
	}

	Result PlayerTransRemoveFriend::OnRemoved( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		DB::QueryRemoveFriendCmd *pDBRes = (DB::QueryRemoveFriendCmd*)pRes;

		svrChk(pRes->GetResult());


		svrChk( GetMyOwner()->GetComponent<UserFriendSystem>()->RemoveFriend( pDBRes->FriendUID ) );

	Proc_End:

		m_WaitingResultCount--;
		if( m_WaitingResultCount <= 0 )
			CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransRemoveFriend::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ServerEntity *pServerEntity = nullptr;
		EntityUID playerUID;
		ServerFriendInformation* pFriendInfo = nullptr;

		m_WaitingResultCount = 0;

		svrChk( super::StartTransaction() );

		pFriendInfo = GetMyOwner()->GetComponent<UserFriendSystem>()->GetFriend(GetFriendID());
		if (pFriendInfo == nullptr)
			svrErrClose(ResultCode::INVALID_PLAYERID);

		// Find player and notify to remove
		if( (Service::PlayerManager->FindPlayer( GetFriendID(), playerUID )) )
		{
			svrChk( Service::ServerEntityManager->GetServerEntity( playerUID.GetServerID(), pServerEntity ) );

			svrChk(Policy::NetPolicyGameServer(pServerEntity->GetConnection()).FriendRemovedC2SEvt( RouteContext(GetOwnerEntityUID(),playerUID),
				GetFriendID(), GetMyOwner()->GetPlayerID() ) );
		}

		// Reset 
		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->RemoveFriend(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetAccountID(), GetFriendID())); m_WaitingResultCount++;
		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->RemoveFriend(GetTransID(), pFriendInfo->ShardID, GetFriendID(), GetMyOwner()->GetAccountID())); m_WaitingResultCount++;

	Proc_End:

		if( !(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}
	


	// Start Transaction
	Result PlayerTransFriendRemovedS2S::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetPlayerID() != GetDestPlayerID() )
		{
			svrErr( ResultCode::INVALID_PLAYERID );
		}

		svrChk( GetMyOwner()->GetComponent<UserFriendSystem>()->RemoveFriend( GetRemoverID() ) );
		svrChk(Policy::NetSvrPolicyGame(GetConnection()).FriendRemovedS2CEvt(GetRemoverID()));

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}



	PlayerTransGetFriendList::PlayerTransGetFriendList( IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction(heap, pIMsg )
		, m_Friends(heap)
	{
		BR_TRANS_MESSAGE(DB::QueryGetFriendListCmd, { return OnGetList(pRes); });
		//BR_TRANS_MESSAGE(DB::QueryGetFriendQuickInfoCmd, { return OnGetQuickInfo(pRes); });
		BR_TRANS_MESSAGE(DB::QueryGetFriendQuickInfoWithNickCmd, { return OnGetQuickInfoWithNick(pRes); });
		//BR_TRANS_MESSAGE(DB::QueryGetNickNameCmd, { return OnGetNickNames(pRes); });
	}

	Result PlayerTransGetFriendList::OnGetList( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		UserFriendSystem *pFriendSystem = nullptr;
		DB::QueryGetFriendListCmd *pDBRes = (DB::QueryGetFriendListCmd*)pRes;

		m_WaitingCount = 0;

		svrChk(pRes->GetResult());

		svrChkPtr( pFriendSystem = GetMyOwner()->GetComponent<UserFriendSystem>() );

		pFriendSystem->ClearFriendList();

		m_Friends.Reserve( pDBRes->m_RowsetResult.size() );
		std::for_each( pDBRes->m_RowsetResult.begin(), pDBRes->m_RowsetResult.end(), [&]( DB::QueryGetFriendListSet &set )
		{
			ServerFriendInformation info(set.FriendUID, set.FriendShardID, set.FriendFacebookUID, "", 1, 0, 0, FALSE, 0, set.FriendStaminaTime);

			Result hRes = pFriendSystem->AddFriend(info);
			if (hRes == Result(ResultCode::MAX_FRIEND))
			{
				svrTrace(Warning, "Failed to add friend. Max friends, PlayerID:{0} to friend system", set.FriendUID);
				return;
			}
			else if (!(hr))
			{
				svrTrace(Error, "Failed to add friend PlayerID:{0} to friend system", set.FriendUID);
				return;
			}

			if ((Svr::GetServerComponent<DB::GameConspiracyDB>()->GetFriendQuickInfoWithNickCmd(GetTransID(), info.ShardID, info.PlayerID)))
				m_WaitingCount++;
		});

		m_TotalNumberOfFriends = (decltype(m_TotalNumberOfFriends))pFriendSystem->GetNumberOfFriends();


	Proc_End:

		if( !(hr) || m_WaitingCount <= 0 )
			CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}
/*
	Result PlayerTransGetFriendList::OnGetQuickInfo( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		FriendInformation *pFriend = nullptr;
		DB::QueryGetFriendQuickInfoCmd *pDBRes = nullptr;

		svrChk(pRes->GetResult());

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
			svrErr(ResultCode::INVALID_PLAYERID);
		}

	Proc_End:

		if( !(hr) )
		{
			svrTrace( Error, "Failed to get friend level PlayerID:{0}, hr={1}", pDBRes ? pDBRes->UserID : 0, ArgHex32(hr) );
		}

		m_WaitingCount--;
		if( m_WaitingCount <= 0 )
			CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}
*/

	Result PlayerTransGetFriendList::OnGetQuickInfoWithNick(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		FriendInformation *pFriend = nullptr;
		DB::QueryGetFriendQuickInfoWithNickCmd *pDBRes = nullptr;

		svrChk(pRes->GetResult());

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
			svrErr(ResultCode::INVALID_PLAYERID);
		}

	Proc_End:

		if( !(hr) )
		{
			svrTrace(Error, "Failed to get friend level PlayerID:{0}, hr={1:X8}", pDBRes ? pDBRes->PlayerID : 0, hr);
		}

		m_WaitingCount--;
		if( m_WaitingCount <= 0 )
			CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}


	// Start Transaction
	Result PlayerTransGetFriendList::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		UserFriendSystem* friendSystem = nullptr;
		m_MaxFriendSlot = 0;
		m_TotalNumberOfFriends = 0;

		svrChk( super::StartTransaction() );

		m_WaitingCount = 0;

		friendSystem = GetMyOwner()->GetComponent<UserFriendSystem>();
		m_TotalNumberOfFriends = (decltype(m_TotalNumberOfFriends))friendSystem->GetNumberOfFriends();
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
					if ((Svr::GetServerComponent<DB::GameConspiracyDB>()->GetFriendQuickInfoWithNickCmd(GetTransID(), friendInfo.ShardID, friendInfo.PlayerID)))
						m_WaitingCount++;
					return ResultCode::SUCCESS;
				});

				friendSystem->SetLatestStatusSync(Util::Time.GetTimeMs());
			}

			if (m_WaitingCount == 0)
				CloseTransaction(hr);
		}

	Proc_End:

		if( !(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}

	Result PlayerTransGetFriendList::CloseTransaction( Result hr )
	{
		if( IsClosed() )
			return ResultCode::SUCCESS;

		auto friendSystem = GetMyOwner()->GetComponent<UserFriendSystem>();

		m_MaxFriendSlot = (decltype(m_MaxFriendSlot))GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GetFriendSlot();

		m_Friends.Clear();
		auto maxRequest = Util::Min(GetCount(), (ushort)UserFriendSystem::MAX_FRIEND_REQUEST);
		friendSystem->ForeachFriends(GetStartIndex(), maxRequest, [&](const FriendInformation& friendInfo)
		{
			m_Friends.push_back(friendInfo);
			return ResultCode::SUCCESS;
		});

		return super::CloseTransaction(hr);
	}




	PlayerTransGiveStamina::PlayerTransGiveStamina(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction(heap, pIMsg )
	{
		SetExclusive(true);
		BR_TRANS_MESSAGE( DB::QueryUpdateTickStatusCmd, { return OnSavedToDB(pRes); });
		BR_TRANS_MESSAGE( DB::QueryUpdateFriendStaminaTimeCmd, { return OnUpdateTime(pRes); });
		BR_TRANS_MESSAGE( DB::QueryNotification_AddCmd, { return OnNotifyAdded(pRes); } );
	}


	Result PlayerTransGiveStamina::OnSavedToDB( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		//DB::QueryUpdateTickStatusCmd *pDBRes = (DB::QueryUpdateTickStatusCmd*)pRes;


	//Proc_End:


		m_WaitingQueries--;

		// if failed to write to DB, roleback the changes
		if(!(hr))
		{
			svrTrace( Error, "Failed to save give stamina result PlayerID:{0}, Dest:{1}, hr:{2:X8}", GetMyOwner()->GetPlayerID(), GetTargetPlayer(), hr );
			CloseTransaction(hr);
		}

		if (m_WaitingQueries <= 0)
			CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}
	
	Result PlayerTransGiveStamina::OnUpdateTime( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		//DB::QueryUpdateFriendStaminaTimeCmd *pDBRes = (DB::QueryUpdateFriendStaminaTimeCmd*)pRes;
		svrChk(pRes->GetResult());


	Proc_End:

		// if failed to write to DB, roleback the changes
		if(!(hr))
		{
			svrTrace( Error, "Failed to save give-stamina timestamp PlayerID:{0}, Dest:{1}", GetMyOwner()->GetPlayerID(), GetTargetPlayer() );
		}

		m_WaitingQueries--;
		if( m_WaitingQueries <= 0 )
		{
			CloseTransaction( hr );
		}

		return ResultCode::SUCCESS; 
	}

	Result PlayerTransGiveStamina::OnNotifyAdded(  Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ServerEntity *pServerEntity = nullptr;
		EntityUID playerUID;
		DB::QueryNotification_AddCmd *pMsgRes = (DB::QueryNotification_AddCmd*)pRes;

		svrChkClose( pRes->GetResult() );

		if( ( Service::PlayerManager->FindPlayer( GetTargetPlayer(), playerUID )) )
		{
			svrChk( Service::ServerEntityManager->GetServerEntity( playerUID.GetServerID(), pServerEntity ) );
			Policy::NetPolicyGameServer(pServerEntity->GetConnection()).NotifyC2SEvt( RouteContext(GetOwnerEntityUID(),playerUID),
				GetTargetPlayer(), pMsgRes->NotificationID, NotificationType::GiftStamina, GetMyOwner()->GetPlayerID(), GetMyOwner()->GetFacebookUID(), GetMyOwner()->GetNickName(), m_TimeStamp.time_since_epoch().count());
		}

	Proc_End:

		m_WaitingQueries--;
		if( m_WaitingQueries <= 0 )
		{
			CloseTransaction( hr );
		}

		return ResultCode::SUCCESS;
		
	}


	// Start Transaction
	Result PlayerTransGiveStamina::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		UserGamePlayerInfoSystem *pPlayerInfoSystem = nullptr;
		ServerFriendInformation* pFriend = nullptr;
		EntityUID playerUID;

		m_WaitingQueries = 0;
		m_TimeStamp = Util::Time.GetTimeUTCSec();

		svrChk( super::StartTransaction() );

		svrChkPtr( pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>() );

		//GetMyOwner()->UpdateGamePlayer();

		//if( pPlayerInfoSystem->GetStamina() <= 0 )
		//	svrErrClose(ResultCode::GAME_NOTENOUGH_RESOURCE);


		pFriend = GetMyOwner()->GetComponent<UserFriendSystem>()->GetFriend(GetTargetPlayer());
		if( pFriend == nullptr )
			svrErrClose(ResultCode::SVR_PLAYER_NOT_FOUND);

		//svrChk( pPlayerInfoSystem->GainStamina( -1 ) );

		//GetMyOwner()->AddGameTransactionLog(TransLogCategory::Give, -1, 0, GetTargetPlayer());


		//// Save my status
		//if ((GetMyOwner()->UpdateDBSync(GetTransID())))
		//	m_WaitingQueries++;


		//Update give stamina time
		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->UpdateFriendStaminaTime(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(), GetTargetPlayer(), m_TimeStamp));
		pFriend->LastStaminaSent = m_TimeStamp.time_since_epoch().count();
		m_WaitingQueries++;


		// Add stamina notification
		svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->Notification_Add(GetTransID(), pFriend->ShardID, pFriend->PlayerID, true, NotificationType::GiftStamina, GetMyOwner()->GetPlayerID(), GetMyOwner()->GetFacebookUID(), GetMyOwner()->GetNickName(), m_TimeStamp));
		m_WaitingQueries++;



	Proc_End:

		if( !(hr) || m_WaitingQueries == 0 )
		{
			CloseTransaction( hr );
		}

		return hr;
	}



	//PlayerTransGiveStaminaS2S::PlayerTransGiveStaminaS2S( IHeap& heap, MessageDataPtr &pIMsg )
	//	:UserTransactionS2SEvt(pIMsg)
	//{
	//	SetExclusive(true);
	//	BR_TRANS_MESSAGE( DB::QueryUpdateTickStatusCmd, { return OnSavedToDB(pRes); });
	//}


	//Result PlayerTransGiveStaminaS2S::OnSavedToDB( Svr::TransactionResult* &pRes )
	//{
	//	Result hr = ResultCode::SUCCESS;
	//	DB::QueryUpdateTickStatusCmd *pDBRes = (DB::QueryUpdateTickStatusCmd*)pRes;
	//	svrChk(pRes->GetResult());


	//Proc_End:

	//	if( !(hr) )
	//	{
	//		svrTrace( Error, "Failed to save received stamina result PlayerID:{0}, Sender:{1}", GetDestPlayerID(), GetSenderID() );
	//	}

	//	CloseTransaction(hr);

	//	return ResultCode::SUCCESS; 
	//}


	//// Start Transaction
	//Result PlayerTransGiveStaminaS2S::StartTransaction()
	//{
	//	Result hr = ResultCode::SUCCESS;
	//	UserGamePlayerInfoSystem *pPlayerInfoSystem = nullptr;

	//	svrChk( super::StartTransaction() );

	//	if( GetMyOwner()->GetPlayerID() != GetDestPlayerID() )
	//		svrErrClose(ResultCode::INVALID_PLAYERID);

	//	svrChkPtr( pPlayerInfoSystem = GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>() );

	//	svrChk( pPlayerInfoSystem->GainStamina( 1 ) );

	//	svrChk(GetMyOwner()->UpdateDBSync(GetTransID()));

	//Proc_End:

	//	if( !(hr) )
	//	{
	//		CloseTransaction( hr );
	//	}

	//	return hr;
	//}


	


};// namespace GameServer 
};// namespace SF 

