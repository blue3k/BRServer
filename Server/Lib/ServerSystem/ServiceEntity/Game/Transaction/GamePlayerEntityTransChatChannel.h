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
#include "Protocol/Message/ChatChannelMsgClass.h"
#include "Protocol/Policy/ChatChannelNetPolicy.h"


#include "Transaction/MessageRoute.h"

#include "ServiceEntity/Game/GamePlayerEntity.h"
#include "Net/SFMessage.h"



namespace SF {
namespace Svr {


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ChatChannel
	//


	
	class PlayerTransCreateOrJoinChatChannel : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::CreateOrJoinChatChannelCmd>
	{
	private:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::CreateOrJoinChatChannelCmd> super;

	private:
		EntityUID m_ChatChannelUID;

	public:
		PlayerTransCreateOrJoinChatChannel(IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransCreateOrJoinChatChannel() {}

		Result OnCreateRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, CreateOrJoinChatChannelRes, m_ChatChannelUID);
	};
	
	
	
	class PlayerTransJoinChatChannel : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::JoinChatChannelCmd>
	{
	private:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::JoinChatChannelCmd> super;

	private:
		PlayerID m_LeaderID;
		EntityUID m_ChatChannelID;


	public:
		PlayerTransJoinChatChannel(IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransJoinChatChannel() {}

		Result OnJoinChatChannelRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, JoinChatChannelRes, m_LeaderID, m_ChatChannelID);
	};
	
	// ChatChannel player joined
	class PlayerTransChatChannelPlayerJoinedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::ChatChannel::PlayerJoinedS2CEvt>
	{
	private:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::ChatChannel::PlayerJoinedS2CEvt> super;

	public:
		PlayerTransChatChannelPlayerJoinedS2SEvt(IHeap& heap, MessageDataPtr &pIMsg )  :UserTransactionS2SEvt( heap, pIMsg ) {}
		virtual ~PlayerTransChatChannelPlayerJoinedS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction() override;
	};
	
	
	// Player leader changed
	class PlayerTransChatChannelLeaderChangedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::ChatChannel::LeaderChangedS2CEvt>
	{
	private:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::ChatChannel::LeaderChangedS2CEvt> super;

	public:
		PlayerTransChatChannelLeaderChangedS2SEvt(IHeap& heap, MessageDataPtr &pIMsg )  :UserTransactionS2SEvt( heap, pIMsg ) {}
		virtual ~PlayerTransChatChannelLeaderChangedS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction() override;
	};

	
	
	class PlayerTransLeaveChatChannel : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::LeaveChatChannelCmd>
	{
	private:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::LeaveChatChannelCmd> super;

	public:
		PlayerTransLeaveChatChannel(IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransLeaveChatChannel() {}

		Result OnLeaveChatChannelRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(NetSvrPolicyGame, LeaveChatChannelRes);
	};
	
	
	
	// Player leader changed
	class PlayerTransChatChannelPlayerLeftS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::ChatChannel::PlayerLeftS2CEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::ChatChannel::PlayerLeftS2CEvt> super;

	public:
		PlayerTransChatChannelPlayerLeftS2SEvt(IHeap& heap, MessageDataPtr &pIMsg )  :UserTransactionS2SEvt(heap, pIMsg ) {}
		virtual ~PlayerTransChatChannelPlayerLeftS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction() override;
	};

	
	
	
	class PlayerTransChatChannelKickPlayer : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::ChatChannelKickPlayerCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::ChatChannelKickPlayerCmd> super;

	public:
		PlayerTransChatChannelKickPlayer(IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransChatChannelKickPlayer() {}

		Result OnPlayerKickRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(NetSvrPolicyGame, ChatChannelKickPlayerRes);
	};
	
	
	
	// Player leader changed
	class PlayerTransChatChannelPlayerKickedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::ChatChannel::PlayerKickedS2CEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::ChatChannel::PlayerKickedS2CEvt> super;

	public:
		PlayerTransChatChannelPlayerKickedS2SEvt(IHeap& heap, MessageDataPtr &pIMsg )  :UserTransactionS2SEvt( heap, pIMsg ) {}
		virtual ~PlayerTransChatChannelPlayerKickedS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction() override;
	};


	
	
	
	class PlayerTransChatChannelChatMessage : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::ChatChannelChatMessageCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::ChatChannelChatMessageCmd> super;

	private:

	public:
		PlayerTransChatChannelChatMessage(IHeap& heap, MessageDataPtr &pIMsg )  :MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg) ) {}
		virtual ~PlayerTransChatChannelChatMessage() {}

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(NetSvrPolicyGame, ChatChannelChatMessageRes);
	};

	// Chat message from other entity	
	class PlayerTransChatChannelChatMessageS2CEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::ChatChannel::ChatMessageS2CEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::ChatChannel::ChatMessageS2CEvt> super;

	public:
		PlayerTransChatChannelChatMessageS2CEvt(IHeap& heap, MessageDataPtr &pIMsg )  :UserTransactionS2SEvt( heap, pIMsg ) {}
		virtual ~PlayerTransChatChannelChatMessageS2CEvt() {}

		// Start Transaction
		virtual Result StartTransaction() override;
	};
	

	
} // namespace GameServer 
} // namespace SF 

