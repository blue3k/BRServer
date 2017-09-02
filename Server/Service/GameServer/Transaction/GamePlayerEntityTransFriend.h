////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "ServerSystem/Transaction.h"
#include "Memory/MemoryPool.h"
#include "Container/SFArray.h"
#include "Types/BrBaseTypes.h"
#include "GameConst.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameNetPolicy.h"
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"

#include "ServerSystem/MessageRoute.h"

#include "GameServerClass.h"

#include "GameInstance/GamePlayerEntity.h"
#include "Net/Message.h"

//#include "PlayerInfoManager.h"


namespace SF {
namespace GameServer {


	

	/////////////////////////////////////////////////////////////////////////////
	//
	//	Friend transaction
	//

	class PlayerTransInviteFriend : public Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::InviteFriendCmd, PlayerTransInviteFriend>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::InviteFriendCmd, PlayerTransInviteFriend> super;

	private:
		TimeStampSec m_TimeStamp;

	public:
		PlayerTransInviteFriend( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransInviteFriend() {}

		Result OnGetPlayerShardID(Svr::TransactionResult* &pRes);
		Result OnNotifyAdded(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(InviteFriendRes);
	};



	class PlayerTransFriendAccept : public Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::AcceptFriendRequestCmd, PlayerTransFriendAccept, sizeof(Svr::TransactionMessageHandlerType)*6 >
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::AcceptFriendRequestCmd, PlayerTransFriendAccept, sizeof(Svr::TransactionMessageHandlerType) * 6 > super;

	private:
		int m_WaitingResultCount;

		ServerFriendInformation m_NewFriend;

	public:
		PlayerTransFriendAccept( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransFriendAccept() {}

		Result OnGetPlayerShardID(Svr::TransactionResult* &pRes);
		Result OnFriendSlotStatus(Svr::TransactionResult* &pRes);
		Result OnFriendAdded( Svr::TransactionResult* &pRes );
		Result OnFriendQuickInfo(Svr::TransactionResult* &pRes);
		//Result OnGetNickName( Svr::TransactionResult* &pRes );

		Result SendNotifyToInviter();

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(AcceptFriendRequestRes,m_NewFriend);
	};


	class PlayerTransFriendAcceptedS2S : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::GameServer::FriendAcceptedC2SEvt, PlayerTransFriendAcceptedS2S>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::GameServer::FriendAcceptedC2SEvt, PlayerTransFriendAcceptedS2S> super;

	public:
		PlayerTransFriendAcceptedS2S( Message::MessageData* &pIMsg ):UserTransactionS2SEvt(pIMsg) {}
		virtual ~PlayerTransFriendAcceptedS2S() {}

		// Start Transaction
		virtual Result StartTransaction();
	};
	


	class PlayerTransRemoveFriend : public Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::RemoveFriendCmd, PlayerTransRemoveFriend>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::RemoveFriendCmd, PlayerTransRemoveFriend> super;

	private:
		int m_WaitingResultCount;

	public:
		PlayerTransRemoveFriend( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransRemoveFriend() {}

		Result OnRemoved( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(RemoveFriendRes, GetFriendID());
	};



	class PlayerTransFriendRemovedS2S : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::GameServer::FriendRemovedC2SEvt, PlayerTransFriendRemovedS2S>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::GameServer::FriendRemovedC2SEvt, PlayerTransFriendRemovedS2S> super;

	public:
		PlayerTransFriendRemovedS2S( Message::MessageData* &pIMsg ):UserTransactionS2SEvt(pIMsg) {}
		virtual ~PlayerTransFriendRemovedS2S() {}

		// Start Transaction
		virtual Result StartTransaction();
	};


	class PlayerTransGetFriendList : public Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::GetFriendListCmd, PlayerTransGetFriendList>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::GetFriendListCmd, PlayerTransGetFriendList> super;

	private:
		StaticArray<FriendInformation, 20> m_Friends;
		uint16_t m_MaxFriendSlot;
		uint16_t m_TotalNumberOfFriends;
		INT m_WaitingCount;

		//StaticArray<PlayerID, 6> m_NameQuery;


	public:
		PlayerTransGetFriendList( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransGetFriendList() {}

		Result OnGetList( Svr::TransactionResult* &pRes );
		//Result OnGetQuickInfo( Svr::TransactionResult* &pRes );
		Result OnGetQuickInfoWithNick(Svr::TransactionResult* &pRes);
		Result OnGetNickNames(Svr::TransactionResult* &pRes);

		Result FlushNameRequestList();

		// Start Transaction
		virtual Result StartTransaction();

		virtual Result CloseTransaction( Result hr ) override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(GetFriendListRes, m_MaxFriendSlot, m_TotalNumberOfFriends, GetStartIndex(), m_Friends);
	};

	
	
	class PlayerTransGiveStamina : public Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::GiveStaminaCmd, PlayerTransGiveStamina>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::GiveStaminaCmd, PlayerTransGiveStamina> super;

	private:
		//Memento<UserGamePlayerInfoSystem::MEMENTO_SIZE> m_SavedData;
		TimeStampSec m_TimeStamp;
		INT m_WaitingQueries;

	public:
		PlayerTransGiveStamina( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransGiveStamina() {}

		Result OnSavedToDB( Svr::TransactionResult* &pRes );
		Result OnUpdateTime( Svr::TransactionResult* &pRes );
		Result OnNotifyAdded(  Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(GiveStaminaRes, GetTargetPlayer(), m_TimeStamp.time_since_epoch().count());
	};


} // namespace GameServer 
} // namespace SF 

