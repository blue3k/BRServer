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
#include "Transaction/Transaction.h"
#include "MemoryManager/SFMemoryPool.h"
#include "Container/SFArray.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"
#include "Protocol/GameMsgClass.h"
#include "Protocol/GameNetPolicy.h"
#include "Protocol/GameServerMsgClass.h"
#include "Protocol/GameServerNetPolicy.h"
#include "Protocol/GameInstanceMsgClass.h"
#include "Protocol/GameInstanceNetPolicy.h"

#include "Transaction/MessageRoute.h"

#include "ServiceEntity/Game/GamePlayerEntity.h"
#include "Net/SFMessage.h"

//#include "PlayerInfoManager.h"


namespace SF {
namespace Svr {


	

	/////////////////////////////////////////////////////////////////////////////
	//
	//	Friend transaction
	//

	class PlayerTransInviteFriend : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::InviteFriendCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::InviteFriendCmd> super;

	private:
		UTCTimeStampSec m_TimeStamp;

	public:
		PlayerTransInviteFriend(IHeap& heap, const MessageDataPtr &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransInviteFriend() {}

		Result OnGetPlayerShardID(Svr::TransactionResult* &pRes);
		Result OnNotifyAdded(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(NetSvrPolicyGame, InviteFriendRes);
	};



	class PlayerTransFriendAccept : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::AcceptFriendRequestCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::AcceptFriendRequestCmd> super;

	private:
		int m_WaitingResultCount;

		ServerFriendInformation m_NewFriend;

	public:
		PlayerTransFriendAccept(IHeap& heap, const MessageDataPtr &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransFriendAccept() {}

		Result OnGetPlayerShardID(Svr::TransactionResult* &pRes);
		Result OnFriendSlotStatus(Svr::TransactionResult* &pRes);
		Result OnFriendAdded( Svr::TransactionResult* &pRes );
		Result OnFriendQuickInfo(Svr::TransactionResult* &pRes);
		//Result OnGetNickName( Svr::TransactionResult* &pRes );

		Result SendNotifyToInviter();

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, AcceptFriendRequestRes,m_NewFriend);
	};


	class PlayerTransFriendAcceptedS2S : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameServer::FriendAcceptedC2SEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameServer::FriendAcceptedC2SEvt> super;

	public:
		PlayerTransFriendAcceptedS2S(IHeap& heap, const MessageDataPtr &pIMsg ):UserTransactionS2SEvt(heap, pIMsg) {}
		virtual ~PlayerTransFriendAcceptedS2S() {}

		// Start Transaction
		virtual Result StartTransaction() override;
	};
	


	class PlayerTransRemoveFriend : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::RemoveFriendCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::RemoveFriendCmd> super;

	private:
		int m_WaitingResultCount;

	public:
		PlayerTransRemoveFriend(IHeap& heap, const MessageDataPtr &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransRemoveFriend() {}

		Result OnRemoved( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, RemoveFriendRes, GetFriendID());
	};



	class PlayerTransFriendRemovedS2S : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameServer::FriendRemovedC2SEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameServer::FriendRemovedC2SEvt> super;

	public:
		PlayerTransFriendRemovedS2S(IHeap& heap, const MessageDataPtr &pIMsg ):UserTransactionS2SEvt(heap, pIMsg) {}
		virtual ~PlayerTransFriendRemovedS2S() {}

		// Start Transaction
		virtual Result StartTransaction() override;
	};


	class PlayerTransGetFriendList : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::GetFriendListCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::GetFriendListCmd> super;

	private:
		StaticArray<FriendInformation, 20> m_Friends;
		uint16_t m_MaxFriendSlot;
		uint16_t m_TotalNumberOfFriends;
		INT m_WaitingCount;

		//StaticArray<PlayerID, 6> m_NameQuery;


	public:
		PlayerTransGetFriendList(IHeap& heap, const MessageDataPtr &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransGetFriendList() {}

		Result OnGetList( Svr::TransactionResult* &pRes );
		//Result OnGetQuickInfo( Svr::TransactionResult* &pRes );
		Result OnGetQuickInfoWithNick(Svr::TransactionResult* &pRes);
		Result OnGetNickNames(Svr::TransactionResult* &pRes);

		Result FlushNameRequestList();

		// Start Transaction
		virtual Result StartTransaction() override;

		virtual Result CloseTransaction( Result hr ) override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, GetFriendListRes, m_MaxFriendSlot, m_TotalNumberOfFriends, GetStartIndex(), m_Friends);
	};

	

} // namespace GameServer 
} // namespace SF 

