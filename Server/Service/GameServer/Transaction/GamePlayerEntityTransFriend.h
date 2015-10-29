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

#include "Common/Typedefs.h"
#include "ServerSystem/Transaction.h"
#include "Common/MemoryPool.h"
#include "Common/ArrayUtil.h"
#include "Common/BRBaseTypes.h"
#include "Common/GameConst.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameIPolicy.h"
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceIPolicy.h"

#include "ServerSystem/MessageRoute.h"

#include "GameServerClass.h"

#include "GameInstance/GamePlayerEntity.h"
#include "Common/Message.h"

//#include "PlayerInfoManager.h"


namespace BR {
namespace GameServer {


	

	/////////////////////////////////////////////////////////////////////////////
	//
	//	Friend transaction
	//

	class PlayerTransInviteFriend : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::InviteFriendCmd, PlayerTransInviteFriend>
	{
	private:
		TimeStampSec m_TimeStamp;

	public:
		PlayerTransInviteFriend( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransInviteFriend() {}

		HRESULT OnGetPlayerShardID(Svr::TransactionResult* &pRes);
		HRESULT OnNotifyAdded(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(InviteFriendRes);
	};



	class PlayerTransFriendAccept : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::AcceptFriendRequestCmd, PlayerTransFriendAccept, sizeof(Svr::TransactionMessageHandlerType)*6 >
	{
	private:
		int m_WaitingResultCount;

		ServerFriendInformation m_NewFriend;

	public:
		PlayerTransFriendAccept( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransFriendAccept() {}

		HRESULT OnGetPlayerShardID(Svr::TransactionResult* &pRes);
		HRESULT OnFriendSlotStatus(Svr::TransactionResult* &pRes);
		HRESULT OnFriendAdded( Svr::TransactionResult* &pRes );
		HRESULT OnFriendQuickInfo(Svr::TransactionResult* &pRes);
		//HRESULT OnGetNickName( Svr::TransactionResult* &pRes );

		HRESULT SendNotifyToInviter();

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(AcceptFriendRequestRes,m_NewFriend);
	};


	class PlayerTransFriendAcceptedS2S : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameServer::FriendAcceptedC2SEvt, PlayerTransFriendAcceptedS2S>
	{
	public:
		PlayerTransFriendAcceptedS2S( Message::MessageData* &pIMsg ):UserTransactionS2SEvt(pIMsg) {}
		virtual ~PlayerTransFriendAcceptedS2S() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};
	


	class PlayerTransRemoveFriend : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::RemoveFriendCmd, PlayerTransRemoveFriend>
	{
	private:
		int m_WaitingResultCount;

	public:
		PlayerTransRemoveFriend( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransRemoveFriend() {}

		HRESULT OnRemoved( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(RemoveFriendRes, GetFriendID());
	};



	class PlayerTransFriendRemovedS2S : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameServer::FriendRemovedC2SEvt, PlayerTransFriendRemovedS2S>
	{
	public:
		PlayerTransFriendRemovedS2S( Message::MessageData* &pIMsg ):UserTransactionS2SEvt(pIMsg) {}
		virtual ~PlayerTransFriendRemovedS2S() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};


	class PlayerTransGetFriendList : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GetFriendListCmd, PlayerTransGetFriendList>
	{
	private:
		StaticArray<FriendInformation, 20> m_Friends;
		UINT16 m_MaxFriendSlot;
		UINT16 m_TotalNumberOfFriends;
		INT m_WaitingCount;

		//StaticArray<PlayerID, 6> m_NameQuery;


	public:
		PlayerTransGetFriendList( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransGetFriendList() {}

		HRESULT OnGetList( Svr::TransactionResult* &pRes );
		//HRESULT OnGetQuickInfo( Svr::TransactionResult* &pRes );
		HRESULT OnGetQuickInfoWithNick(Svr::TransactionResult* &pRes);
		HRESULT OnGetNickNames(Svr::TransactionResult* &pRes);

		HRESULT FlushNameRequestList();

		// Start Transaction
		virtual HRESULT StartTransaction();

		virtual HRESULT CloseTransaction( HRESULT hr ) override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(GetFriendListRes, m_MaxFriendSlot, m_TotalNumberOfFriends, GetStartIndex(), m_Friends);
	};

	
	
	class PlayerTransGiveStamina : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GiveStaminaCmd, PlayerTransGiveStamina>
	{
	private:
		//Memento<UserGamePlayerInfoSystem::MEMENTO_SIZE> m_SavedData;
		TimeStampSec m_TimeStamp;
		INT m_WaitingQueries;

	public:
		PlayerTransGiveStamina( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransGiveStamina() {}

		HRESULT OnSavedToDB( Svr::TransactionResult* &pRes );
		HRESULT OnUpdateTime( Svr::TransactionResult* &pRes );
		HRESULT OnNotifyAdded(  Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(GiveStaminaRes, GetTargetPlayer(), m_TimeStamp.time_since_epoch().count());
	};
	//
	//class PlayerTransGiveStaminaS2S : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameServer::GiveStaminaC2SEvt, PlayerTransGiveStaminaS2S>
	//{
	//public:
	//	PlayerTransGiveStaminaS2S( Message::MessageData* &pIMsg );//:UserTransactionS2SEvt(pIMsg) {}
	//	virtual ~PlayerTransGiveStaminaS2S() {}

	//	HRESULT OnSavedToDB( Svr::TransactionResult* &pRes );

	//	// Start Transaction
	//	virtual HRESULT StartTransaction();
	//};
	//

} // namespace GameServer 
} // namespace BR 

