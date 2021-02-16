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
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameNetPolicy.h"
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Message/GamePartyMsgClass.h"
#include "Protocol/Policy/GamePartyNetPolicy.h"
#include "Protocol/Message/PartyMatchingMsgClass.h"

#include "Transaction/MessageRoute.h"

#include "ServiceEntity/Game/GamePlayerEntity.h"
#include "Net/SFMessage.h"



namespace SF {
namespace Svr {


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Party
	//

	
	// Player voted event
	class PlayerTransGameMatchedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::PartyMatching::PlayerGameMatchedS2CEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::PartyMatching::PlayerGameMatchedS2CEvt> super;

	private:
		INT m_WaitingQueires;

	public:
		PlayerTransGameMatchedS2SEvt(IHeap& heap, MessageDataPtr &pIMsg );//  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransGameMatchedS2SEvt() {}

		Result OnJoinGameRes( Svr::TransactionResult* &pRes );
		Result OnLeavePartyRes(Svr::TransactionResult* &pRes);
		Result OnUpdateDBRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();
	};


	// Player leader changed
	class PlayerTransPartyQueuedGameMatchingS2CEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameParty::QueuedGameMatchingS2CEvt>
	{
	private:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameParty::QueuedGameMatchingS2CEvt> super;

	public:
		PlayerTransPartyQueuedGameMatchingS2CEvt(IHeap& heap, MessageDataPtr &pIMsg )  :UserTransactionS2SEvt( heap, pIMsg ) {}
		virtual ~PlayerTransPartyQueuedGameMatchingS2CEvt() {}

		// Start Transaction
		virtual Result StartTransaction();
	};


	// Dequeued
	class PlayerTransPartyMatchingItemDequeuedS2CEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameParty::MatchingItemDequeuedS2CEvt>
	{
	private:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameParty::MatchingItemDequeuedS2CEvt> super;

	public:
		PlayerTransPartyMatchingItemDequeuedS2CEvt(IHeap& heap, MessageDataPtr &pIMsg )  :UserTransactionS2SEvt( heap, pIMsg ) {}
		virtual ~PlayerTransPartyMatchingItemDequeuedS2CEvt() {}

		// Start Transaction
		virtual Result StartTransaction();
	};
	

	// Player leader changed
	class PlayerTransPartyCanceledGameMatchingS2CEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameParty::CanceledGameMatchingS2CEvt>
	{
	private:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameParty::CanceledGameMatchingS2CEvt> super;

	public:
		PlayerTransPartyCanceledGameMatchingS2CEvt(IHeap& heap, MessageDataPtr &pIMsg )  :UserTransactionS2SEvt(heap, pIMsg ) {}
		virtual ~PlayerTransPartyCanceledGameMatchingS2CEvt() {}

		// Start Transaction
		virtual Result StartTransaction();
	};
		


	
	class PlayerTransCreateParty : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::CreatePartyCmd>
	{
	private:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::CreatePartyCmd> super;

	private:
		PartyUID m_PartyUID;

	public:
		PlayerTransCreateParty(IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransCreateParty() {}

		Result OnCreatePartyRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, CreatePartyRes,m_PartyUID);
	};
	
	
	
	class PlayerTransJoinParty : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::JoinPartyCmd>
	{
	private:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::JoinPartyCmd> super;

	private:
		PlayerID m_LeaderID;
		StaticArray<uint8_t, 2048> m_ChatHistoryData;


	public:
		PlayerTransJoinParty(IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransJoinParty() {}

		Result OnJoinPartyRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, JoinPartyRes,GetPartyUID(),m_LeaderID, m_ChatHistoryData);
	};
	
	// Party player joined
	class PlayerTransPartyPlayerJoinedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameParty::PlayerJoinedS2CEvt>
	{
	private:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameParty::PlayerJoinedS2CEvt> super;

	public:
		PlayerTransPartyPlayerJoinedS2SEvt(IHeap& heap, MessageDataPtr &pIMsg )  :UserTransactionS2SEvt( heap, pIMsg ) {}
		virtual ~PlayerTransPartyPlayerJoinedS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction() override;
	};
	
	
	// Player leader changed
	class PlayerTransPartyLeaderChangedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameParty::PartyLeaderChangedS2CEvt>
	{
	private:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameParty::PartyLeaderChangedS2CEvt> super;

	public:
		PlayerTransPartyLeaderChangedS2SEvt(IHeap& heap, MessageDataPtr &pIMsg )  :UserTransactionS2SEvt( heap, pIMsg ) {}
		virtual ~PlayerTransPartyLeaderChangedS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction() override;
	};

	
	
	class PlayerTransLeaveParty : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::LeavePartyCmd>
	{
	private:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::LeavePartyCmd> super;

	public:
		PlayerTransLeaveParty(IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransLeaveParty() {}

		Result OnLeavePartyRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(NetSvrPolicyGame, LeavePartyRes);
	};
	
	
	
	// Player leader changed
	class PlayerTransPartyPlayerLeftS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameParty::PlayerLeftS2CEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameParty::PlayerLeftS2CEvt> super;

	public:
		PlayerTransPartyPlayerLeftS2SEvt(IHeap& heap, MessageDataPtr &pIMsg )  :UserTransactionS2SEvt(heap, pIMsg ) {}
		virtual ~PlayerTransPartyPlayerLeftS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction() override;
	};

	
	
	
	class PlayerTransPartyKickPlayer : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::PartyKickPlayerCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::PartyKickPlayerCmd> super;

	public:
		PlayerTransPartyKickPlayer(IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransPartyKickPlayer() {}

		Result OnPlayerKickRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(NetSvrPolicyGame, PartyKickPlayerRes);
	};
	
	
	
	// Player leader changed
	class PlayerTransPartyPlayerKickedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameParty::PlayerKickedS2CEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameParty::PlayerKickedS2CEvt> super;

	public:
		PlayerTransPartyPlayerKickedS2SEvt(IHeap& heap, MessageDataPtr &pIMsg )  :UserTransactionS2SEvt( heap, pIMsg ) {}
		virtual ~PlayerTransPartyPlayerKickedS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction() override;
	};


	
	
	class PlayerTransPartyInvite : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::PartyInviteCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::PartyInviteCmd> super;

	public:
		PlayerTransPartyInvite(IHeap& heap, MessageDataPtr &pIMsg );//:MessageTransaction(pIMsg) {}
		virtual ~PlayerTransPartyInvite() {}

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(NetSvrPolicyGame, PartyInviteRes);
	};
	
	
	class PlayerTransNotifyPartyInviteS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameServer::NotifyPartyInviteC2SEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameServer::NotifyPartyInviteC2SEvt> super;

	public:
		PlayerTransNotifyPartyInviteS2SEvt(IHeap& heap, MessageDataPtr &pIMsg ):UserTransactionS2SEvt(heap, pIMsg) {}
		virtual ~PlayerTransNotifyPartyInviteS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction() override;
	};
	

	
	class PlayerTransPartyChatMessage : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::PartyChatMessageCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::PartyChatMessageCmd> super;

	private:

	public:
		PlayerTransPartyChatMessage(IHeap& heap, MessageDataPtr &pIMsg )  :MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg)) {}
		virtual ~PlayerTransPartyChatMessage() {}

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(NetSvrPolicyGame, PartyChatMessageRes);
	};

	// Chat message from other entity	
	class PlayerTransPartyChatMessageS2CEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameParty::ChatMessageS2CEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameParty::ChatMessageS2CEvt> super;

	public:
		PlayerTransPartyChatMessageS2CEvt(IHeap& heap, MessageDataPtr &pIMsg )  :UserTransactionS2SEvt( heap, pIMsg ) {}
		virtual ~PlayerTransPartyChatMessageS2CEvt() {}

		// Start Transaction
		virtual Result StartTransaction() override;
	};
	
	
	class PlayerTransPartyQuickChatMessage : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::PartyQuickChatMessageCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::PartyQuickChatMessageCmd> super;

	private:

	public:
		PlayerTransPartyQuickChatMessage(IHeap& heap, MessageDataPtr &pIMsg )  : MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg)) {}
		virtual ~PlayerTransPartyQuickChatMessage() {}

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(NetSvrPolicyGame, PartyQuickChatMessageRes);
	};

	// Chat message from other entity	
	class PlayerTransPartyQuickChatMessageS2CEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameParty::QuickChatMessageS2CEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameParty::QuickChatMessageS2CEvt> super;

	public:
		PlayerTransPartyQuickChatMessageS2CEvt(IHeap& heap, MessageDataPtr &pIMsg )  :UserTransactionS2SEvt( heap, pIMsg ) {}
		virtual ~PlayerTransPartyQuickChatMessageS2CEvt() {}

		// Start Transaction
		virtual Result StartTransaction() override;
	};


	
} // namespace GameServer 
} // namespace SF 

