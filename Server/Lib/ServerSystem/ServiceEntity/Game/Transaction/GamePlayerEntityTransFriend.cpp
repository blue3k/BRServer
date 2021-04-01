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

#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "ServerEntity/ServerEntityManager.h"

#include "Protocol/GameInstanceMsgClass.h"
#include "Protocol/GameInstanceNetPolicy.h"

#include "GamePlayerEntityTransFriend.h"
#include "ServiceEntity/Game/GamePlayerEntity.h"

#include "Server/BrServer.h"


#include "ServiceEntity/Game/Subsystem/UserFriendSystem.h"
//#include "GameSystem/UserGamePlayerInfoSystem.h"


#include "DB/Game/GameDB.h"
#include "DB/Game/GameQuery.h"
#include "DB/AccountDB.h"
#include "DB/AccountQuery.h"



	
namespace SF {
namespace Svr {



	/////////////////////////////////////////////////////////////////////////////
	//
	//	Friend transaction
	//

	
	PlayerTransInviteFriend::PlayerTransInviteFriend(IHeap& heap, const MessageDataPtr &pIMsg )
		:MessageTransaction(heap, pIMsg)
	{
		BR_TRANS_MESSAGE(DB::QueryGetPlayerShardIDCmd, { return OnGetPlayerShardID(pRes); });
		BR_TRANS_MESSAGE(DB::QueryNotification_AddCmd, { return OnNotifyAdded(pRes); });
	}

	Result PlayerTransInviteFriend::OnGetPlayerShardID(Svr::TransactionResult* &pRes)
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});
		EntityUID playerUID;
		DB::QueryGetPlayerShardIDCmd* pMsgRes;

		svrCheckClose( pRes->GetResult() );
		pMsgRes = (DB::QueryGetPlayerShardIDCmd*)pRes;

		if(pMsgRes->Result == 0)
		{
			svrCheck(Svr::GetServerComponent<DB::GameDB>()->Notification_Add(GetTransID(), pMsgRes->ShardID, GetFriendID(), true, NotificationType::FriendRequest, GetMyOwner()->GetPlayerID(), GetMyOwner()->GetFacebookUID(), GetMyOwner()->GetNickName(), m_TimeStamp));
		}
		else
		{
			svrErrorClose(ResultCode::INVALID_ACCOUNTID);
		}

		return ResultCode::SUCCESS;
		
	}

	Result PlayerTransInviteFriend::OnNotifyAdded(  Svr::TransactionResult* &pRes )
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});
		Svr::ServerEntity *pServerEntity = nullptr;
		EntityUID playerUID;
		DB::QueryNotification_AddCmd *pMsgRes;

		svrCheckClose( pRes->GetResult() );
		pMsgRes = (DB::QueryNotification_AddCmd*)pRes;

		if( ( Service::PlayerManager->FindPlayer( GetFriendID(), playerUID )) )
		{
			auto playerEndpoint = Service::MessageEndpointManager->GetEndpoint(playerUID);
			//svrCheck( Service::ServerEntityManager->GetServerEntity( playerUID.GetServerID(), pServerEntity ) );

			NetPolicyGameServer(playerEndpoint).NotifyC2SEvt( RouteContext(GetOwnerEntityUID(),playerUID),
				GetFriendID(), pMsgRes->NotificationID, NotificationType::FriendRequest, GetMyOwner()->GetPlayerID(), GetMyOwner()->GetFacebookUID(), GetMyOwner()->GetNickName(), m_TimeStamp.time_since_epoch().count() );
		}

		return ResultCode::SUCCESS;
		
	}
	// Start Transaction
	Result PlayerTransInviteFriend::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});

		m_TimeStamp = Util::Time.GetTimeUTCSec();

		svrCheck( super::StartTransaction() );

		if (!GetMyOwner()->GetComponent<UserFriendSystem>()->CanAddFriend())
			svrErrorClose(ResultCode::MAX_FRIEND);

		if (GetMyOwner()->GetComponent<UserFriendSystem>()->IsFriend(GetFriendID()))
			svrErrorClose(ResultCode::ALREADY_IN_FRIEND);

		svrCheck( Svr::GetServerComponent<DB::AccountDB>()->GetPlayerShardID( GetTransID(), GetFriendID() ) );

		return hr;
	}



	PlayerTransFriendAccept::PlayerTransFriendAccept( IHeap& heap, const MessageDataPtr &pIMsg )
		:MessageTransaction(heap, pIMsg)
	{
		BR_TRANS_MESSAGE(DB::QueryAddFriendCmd, { return OnFriendAdded(pRes); });
		BR_TRANS_MESSAGE(DB::QueryGetFriendSlotStatusCmd, { return OnFriendSlotStatus(pRes); });
		//BR_TRANS_MESSAGE(DB::QueryGetNickNameCmd, { return OnGetNickName(pRes); });
		BR_TRANS_MESSAGE(DB::QueryGetFriendQuickInfoWithNickCmd, { return OnFriendQuickInfo(pRes); });
		BR_TRANS_MESSAGE(DB::QueryGetPlayerShardIDCmd, { return OnGetPlayerShardID(pRes); });
	}

	Result PlayerTransFriendAccept::OnGetPlayerShardID(Svr::TransactionResult* &pRes)
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});
		DB::QueryGetPlayerShardIDCmd* pDBRes;

		svrCheck(pRes->GetResult());

		pDBRes = (DB::QueryGetPlayerShardIDCmd*)pRes;
		if (pDBRes->Result < 0)
			svrError(ResultCode::INVALID_PLAYERID);

		m_NewFriend.ShardID = pDBRes->ShardID;

		svrCheck(Svr::GetServerComponent<DB::GameDB>()->GetFriendSlotStatus(GetTransID(), m_NewFriend.ShardID, GetInviterID()));

		return hr;
	}

	Result PlayerTransFriendAccept::OnFriendSlotStatus(Svr::TransactionResult* &pRes)
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});
		auto *pDBRes = (DB::QueryGetFriendSlotStatusCmd*)pRes;
		INT maxFriend;

		svrCheck(pRes->GetResult());

		if (pDBRes->Result < 0)
			svrError(ResultCode::INVALID_PLAYERID);

		//svrCheckPtr(GetMyServer()->GetPresetGameConfig());

		//maxFriend = pDBRes->AddedFriendSlot + GetMyServer()->GetPresetGameConfig()->DefaultFriend;
		maxFriend = pDBRes->AddedFriendSlot + 20; // TODO: FIXME

		if (pDBRes->NumFriends >= maxFriend)
			svrErrorClose(ResultCode::TARGET_MAX_FRIEND);


		{
			auto myShardID = GetMyOwner()->GetShardID();

			// add db 
			m_WaitingResultCount = 0;
			svrCheck(Svr::GetServerComponent<DB::GameDB>()->AddFriend(GetTransID(), myShardID, GetMyOwner()->GetAccountID(), GetInviterID(), m_NewFriend.ShardID, GetInviterFacebookUID())); m_WaitingResultCount++;
			svrCheck(Svr::GetServerComponent<DB::GameDB>()->AddFriend(GetTransID(), m_NewFriend.ShardID, GetInviterID(), GetMyOwner()->GetAccountID(), myShardID, GetMyOwner()->GetFacebookUID())); m_WaitingResultCount++;
		}

		return hr;
	}

	Result PlayerTransFriendAccept::OnFriendAdded(Svr::TransactionResult* &pRes)
	{
		ScopeContext hr([this](Result hr)
			{
				m_WaitingResultCount--;
				if (m_WaitingResultCount <= 0)
				{
					SendNotifyToInviter();


					if (m_NewFriend.PlayerID != 0)
					{
						m_WaitingResultCount = 0;

						if ((Svr::GetServerComponent<DB::GameDB>()->GetFriendQuickInfoWithNickCmd(GetTransID(), m_NewFriend.ShardID, m_NewFriend.PlayerID)))
							m_WaitingResultCount++;
					}
					else
					{
						CloseTransaction(hr);
					}
				}
			});
		UserFriendSystem *pFriendSystem = nullptr;
		auto *pDBRes = (DB::QueryAddFriendCmd*)pRes;

		svrCheck(pRes->GetResult());


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
				if (!hr)
				{
					if (hr != ResultCode::MAX_FRIEND) // silence max friend error
						return hr;
				}

			}
		}

		return ResultCode::SUCCESS; 
	}

	Result PlayerTransFriendAccept::OnFriendQuickInfo(Svr::TransactionResult* &pRes)
	{
		ScopeContext hr([this](Result hr)
			{
				m_WaitingResultCount--;
				if (m_WaitingResultCount <= 0)
					CloseTransaction(hr);

			});
		ServerFriendInformation *pFriend = nullptr;
		auto *pDBRes = (DB::QueryGetFriendQuickInfoWithNickCmd*)pRes;

		svrCheck(pRes->GetResult());

		if (pDBRes->Result < 0 )
			svrError(ResultCode::INVALID_PLAYERID);

		svrCheckPtr(pFriend = GetMyOwner()->GetComponent<UserFriendSystem>()->GetFriend(pDBRes->PlayerID));
		StrUtil::StringCopy(pFriend->NickName, pDBRes->GameNick);
		pFriend->Level = pDBRes->Level;
		pFriend->WeeklyWin = pDBRes->WeeklyWin;
		pFriend->WeeklyLose = pDBRes->WeeklyLose;
		pFriend->IsPlayingGame = pDBRes->PlayerState != 0 ? 1 : 0;
		pFriend->LastActiveTime = pDBRes->LatestActiveTime;

		m_NewFriend = *pFriend;

		return ResultCode::SUCCESS;
	}
/*
	Result PlayerTransFriendAccept::OnGetNickName( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		ServerFriendInformation *pFriend = nullptr;

		svrCheck(pRes->GetResult());

		auto *pDBRes = (DB::QueryGetNickNamesCmd*)pRes;
		if( pDBRes->Result < 0 || pDBRes->RowsetResults.size() == 0 )
			svrErrorClose(ResultCode::INVALID_PLAYERID);

		{
			auto& result = pDBRes->RowsetResults.begin();
			svrCheck(StrUtil::StringCopy(m_NewFriend.NickName, result->NickName));

			svrCheckPtr(pFriend = GetMyOwner()->GetComponent<UserFriendSystem>()->GetFriend(result->UserID));
			svrCheck(StrUtil::StringCopy(pFriend->NickName, result->NickName));
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
		ScopeContext hr;
		Svr::ServerEntity *pServerEntity = nullptr;
		EntityUID playerUID;

		if (!(Service::PlayerManager->FindPlayer(GetInviterID(), playerUID)))
		{
			return hr;
		}

		auto playerEndpoint = Service::MessageEndpointManager->GetEndpoint(playerUID);
		//svrCheck( Service::ServerEntityManager->GetServerEntity( playerUID.GetServerID(), pServerEntity ) );

		svrCheck(NetPolicyGameServer(playerEndpoint).FriendAcceptedC2SEvt( RouteContext(GetOwnerEntityUID(),playerUID),
			GetInviterID(), GetMyOwner()->GetFriendInformation() ) );

		return hr;
	}

	// Start Transaction
	Result PlayerTransFriendAccept::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});

		m_WaitingResultCount = 0;
		memset( &m_NewFriend, 0, sizeof(m_NewFriend) );

		svrCheck( super::StartTransaction() );

		if (!GetMyOwner()->GetComponent<UserFriendSystem>()->CanAddFriend())
			svrErrorClose(ResultCode::MAX_FRIEND);

		// TODO: we need to check inviter's maximum friend slot
		svrCheck(Svr::GetServerComponent<DB::AccountDB>()->GetPlayerShardID(GetTransID(), GetInviterID()));

		return hr;
	}


	// Start Transaction
	Result PlayerTransFriendAcceptedS2S::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		svrCheck( super::StartTransaction() );

		if( GetMyOwner()->GetPlayerID() != GetDestPlayerID() )
		{
			svrError( ResultCode::INVALID_PLAYERID );
		}

		hr = GetMyOwner()->GetComponent<UserFriendSystem>()->AddFriend( GetAccepter() );
		if (hr == ResultCode::MAX_FRIEND)
		{
			return hr;
		}
		else if (!hr)
			return hr;

		svrCheck(NetSvrPolicyGame(GetRemoteEndpoint()).FriendRequestAcceptedS2CEvt(GetAccepter()));

		return hr;
	}



	PlayerTransRemoveFriend::PlayerTransRemoveFriend( IHeap& heap, const MessageDataPtr &pIMsg )
		:MessageTransaction(heap, pIMsg)
	{
		BR_TRANS_MESSAGE( DB::QueryRemoveFriendCmd, { return OnRemoved(pRes); });
	}

	Result PlayerTransRemoveFriend::OnRemoved( Svr::TransactionResult* &pRes )
	{
		ScopeContext hr([this](Result hr)
			{
				m_WaitingResultCount--;
				if (m_WaitingResultCount <= 0)
					CloseTransaction(hr);
			});
		DB::QueryRemoveFriendCmd *pDBRes = (DB::QueryRemoveFriendCmd*)pRes;

		svrCheck(pRes->GetResult());


		svrCheck( GetMyOwner()->GetComponent<UserFriendSystem>()->RemoveFriend( pDBRes->FriendUID ) );

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransRemoveFriend::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});
		Svr::ServerEntity *pServerEntity = nullptr;
		EntityUID playerUID;
		ServerFriendInformation* pFriendInfo = nullptr;

		m_WaitingResultCount = 0;

		svrCheck( super::StartTransaction() );

		pFriendInfo = GetMyOwner()->GetComponent<UserFriendSystem>()->GetFriend(GetFriendID());
		if (pFriendInfo == nullptr)
			svrErrorClose(ResultCode::INVALID_PLAYERID);

		// Find player and notify to remove
		if( (Service::PlayerManager->FindPlayer( GetFriendID(), playerUID )) )
		{
			auto playerEndpoint = Service::MessageEndpointManager->GetEndpoint(playerUID);
			//svrCheck( Service::ServerEntityManager->GetServerEntity( playerUID.GetServerID(), pServerEntity ) );

			svrCheck(NetPolicyGameServer(playerEndpoint).FriendRemovedC2SEvt( RouteContext(GetOwnerEntityUID(),playerUID),
				GetFriendID(), GetMyOwner()->GetPlayerID() ) );
		}

		// Reset 
		svrCheck(Svr::GetServerComponent<DB::GameDB>()->RemoveFriend(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetAccountID(), GetFriendID())); m_WaitingResultCount++;
		svrCheck(Svr::GetServerComponent<DB::GameDB>()->RemoveFriend(GetTransID(), pFriendInfo->ShardID, GetFriendID(), GetMyOwner()->GetAccountID())); m_WaitingResultCount++;

		return hr;
	}
	


	// Start Transaction
	Result PlayerTransFriendRemovedS2S::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		svrCheck( super::StartTransaction() );

		if( GetMyOwner()->GetPlayerID() != GetDestPlayerID() )
		{
			svrError( ResultCode::INVALID_PLAYERID );
		}

		svrCheck( GetMyOwner()->GetComponent<UserFriendSystem>()->RemoveFriend( GetRemoverID() ) );
		svrCheck(NetSvrPolicyGame(GetRemoteEndpoint()).FriendRemovedS2CEvt(GetRemoverID()));

		return hr;
	}



	PlayerTransGetFriendList::PlayerTransGetFriendList( IHeap& heap, const MessageDataPtr &pIMsg )
		: MessageTransaction(heap, pIMsg)
		, m_Friends(heap)
	{
		BR_TRANS_MESSAGE(DB::QueryGetFriendListCmd, { return OnGetList(pRes); });
		//BR_TRANS_MESSAGE(DB::QueryGetFriendQuickInfoCmd, { return OnGetQuickInfo(pRes); });
		BR_TRANS_MESSAGE(DB::QueryGetFriendQuickInfoWithNickCmd, { return OnGetQuickInfoWithNick(pRes); });
		//BR_TRANS_MESSAGE(DB::QueryGetNickNameCmd, { return OnGetNickNames(pRes); });
	}

	Result PlayerTransGetFriendList::OnGetList( Svr::TransactionResult* &pRes )
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr || m_WaitingCount <= 0)
					CloseTransaction(hr);
			});
		UserFriendSystem *pFriendSystem = nullptr;
		DB::QueryGetFriendListCmd *pDBRes = (DB::QueryGetFriendListCmd*)pRes;

		m_WaitingCount = 0;

		svrCheck(pRes->GetResult());

		svrCheckPtr( pFriendSystem = GetMyOwner()->GetComponent<UserFriendSystem>() );

		pFriendSystem->ClearFriendList();

		m_Friends.reserve( pDBRes->RowsetResults.size() );
		for (auto itRowset = pDBRes->RowsetResults.begin(); itRowset != pDBRes->RowsetResults.end(); ++itRowset)
		{
			int64_t FriendUID = itRowset->GetValue<int64_t>("FriendUID"_crc);
			int32_t FriendShardID = itRowset->GetValue<int32_t>("FriendShardID"_crc);
			int64_t FriendFacebookUID = itRowset->GetValue<int64_t>("FriendFacebookUID"_crc);
			int64_t FriendStaminaTime = itRowset->GetValue<int64_t>("FriendStaminaTime"_crc);

			ServerFriendInformation info(FriendUID, FriendShardID, FriendFacebookUID, "", 1, 0, 0, FALSE, 0, FriendStaminaTime);

			hr = pFriendSystem->AddFriend(info);
			if (hr == Result(ResultCode::MAX_FRIEND))
			{
				svrTrace(Warning, "Failed to add friend. Max friends, PlayerID:{0} to friend system", FriendUID);
				break;
			}
			else if (!hr)
			{
				svrTrace(Error, "Failed to add friend PlayerID:{0} to friend system", FriendUID);
				break;
			}

			if ((Svr::GetServerComponent<DB::GameDB>()->GetFriendQuickInfoWithNickCmd(GetTransID(), info.ShardID, info.PlayerID)))
				m_WaitingCount++;
		}

		m_TotalNumberOfFriends = (decltype(m_TotalNumberOfFriends))pFriendSystem->GetNumberOfFriends();

		return hr;
	}
/*
	Result PlayerTransGetFriendList::OnGetQuickInfo( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		FriendInformation *pFriend = nullptr;
		DB::QueryGetFriendQuickInfoCmd *pDBRes = nullptr;

		svrCheck(pRes->GetResult());

		pDBRes = (DB::QueryGetFriendQuickInfoCmd*)pRes;

		if( pDBRes->Result == 0 )
		{
			svrCheckPtr(pFriend = GetMyOwner()->GetComponent<UserFriendSystem>()->GetFriend(pDBRes->PlayerID));
			pFriend->Level = pDBRes->Level;
			pFriend->WeeklyWin = pDBRes->WeeklyWin;
			pFriend->WeeklyLose = pDBRes->WeeklyLose;
			pFriend->IsPlayingGame = pDBRes->PlayerState != 0 ? 1 : 0;
			pFriend->LastActiveTime = pDBRes->LatestActiveTime;
		}
		else
		{
			svrError(ResultCode::INVALID_PLAYERID);
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
		DB::QueryGetFriendQuickInfoWithNickCmd* pDBRes = nullptr;
		FriendInformation* pFriend = nullptr;
		ScopeContext hr([this, pDBRes](Result hr)
			{
				if (!(hr))
				{
					svrTrace(Error, "Failed to get friend PlayerID:{0}, hr={1:X8}", pDBRes ? pDBRes->PlayerID : 0, hr);
				}

				m_WaitingCount--;
				if (m_WaitingCount <= 0)
					CloseTransaction(hr);
			});

		svrCheck(pRes->GetResult());

		pDBRes = (DB::QueryGetFriendQuickInfoWithNickCmd*)pRes;

		if( pDBRes->Result == 0 )
		{
			svrCheckPtr( pFriend = GetMyOwner()->GetComponent<UserFriendSystem>()->GetFriend(pDBRes->PlayerID) );
			StrUtil::StringCopy(pFriend->NickName, pDBRes->GameNick);
			pFriend->Level = pDBRes->Level;
			pFriend->WeeklyWin = pDBRes->WeeklyWin;
			pFriend->WeeklyLose = pDBRes->WeeklyLose;
			pFriend->IsPlayingGame = pDBRes->PlayerState != 0 ? 1 : 0;
			pFriend->LastActiveTime = pDBRes->LatestActiveTime;
		}
		else
		{
			svrError(ResultCode::INVALID_PLAYERID);
		}

		return ResultCode::SUCCESS; 
	}


	// Start Transaction
	Result PlayerTransGetFriendList::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});
		UserFriendSystem* friendSystem = nullptr;
		m_MaxFriendSlot = 0;
		m_TotalNumberOfFriends = 0;

		svrCheck( super::StartTransaction() );

		m_WaitingCount = 0;

		friendSystem = GetMyOwner()->GetComponent<UserFriendSystem>();
		m_TotalNumberOfFriends = (decltype(m_TotalNumberOfFriends))friendSystem->GetNumberOfFriends();
		if (m_TotalNumberOfFriends == 0)
		{
			svrCheck(Svr::GetServerComponent<DB::GameDB>()->GetFriendList(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetAccountID()));
		}
		else
		{
			//if (Util::TimeSince(friendSystem->GetLatestStatusSync()) > UserFriendSystem::STATUS_SYNC_TIME)
			{
				friendSystem->ForeachFriends(0, m_TotalNumberOfFriends, [&](const ServerFriendInformation &friendInfo)
				{
					if ((Svr::GetServerComponent<DB::GameDB>()->GetFriendQuickInfoWithNickCmd(GetTransID(), friendInfo.ShardID, friendInfo.PlayerID)))
						m_WaitingCount++;
					return ResultCode::SUCCESS;
				});

				friendSystem->SetLatestStatusSync(Util::Time.GetTimeMs());
			}

			if (m_WaitingCount == 0)
				CloseTransaction(hr);
		}

		return hr;
	}

	Result PlayerTransGetFriendList::CloseTransaction( Result hr )
	{
		if( IsClosed() )
			return ResultCode::SUCCESS;

		auto friendSystem = GetMyOwner()->GetComponent<UserFriendSystem>();

		//m_MaxFriendSlot = (decltype(m_MaxFriendSlot))GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GetFriendSlot();
		m_MaxFriendSlot = 20;

		m_Friends.Clear();
		auto maxRequest = Util::Min(GetCount(), static_cast<uint16_t>(UserFriendSystem::MAX_FRIEND_REQUEST));
		friendSystem->ForeachFriends(GetStartIndex(), maxRequest, [&](const FriendInformation& friendInfo)
		{
			m_Friends.push_back(friendInfo);
			return ResultCode::SUCCESS;
		});

		return super::CloseTransaction(hr);
	}


}// namespace GameServer 
}// namespace SF 

