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
#include "Common/BrBaseTypes.h"
#include "Common/GameConst.h"
#include "Common/MemoryStream.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameIPolicy.h"
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Message/GamePartyMsgClass.h"
#include "Protocol/Policy/GamePartyIPolicy.h"
#include "Protocol/Message/PartyMatchingMsgClass.h"

#include "ServerSystem/MessageRoute.h"

#include "GameServerClass.h"

#include "GameInstance/GamePlayerEntity.h"
#include "Common/Message.h"



namespace BR {
namespace GameServer {


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Party
	//

	
	// Player voted event
	class PlayerTransGameMatchedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::PartyMatching::PlayerGameMatchedS2CEvt, PlayerTransGameMatchedS2SEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::PartyMatching::PlayerGameMatchedS2CEvt, PlayerTransGameMatchedS2SEvt> super;

	private:
		INT m_WaitingQueires;

	public:
		PlayerTransGameMatchedS2SEvt( Message::MessageData* &pIMsg );//  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransGameMatchedS2SEvt() {}

		HRESULT OnJoinGameRes( Svr::TransactionResult* &pRes );
		HRESULT OnLeavePartyRes(Svr::TransactionResult* &pRes);
		HRESULT OnUpdateDBRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();
	};


	// Player leader changed
	class PlayerTransPartyQueuedGameMatchingS2CEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameParty::QueuedGameMatchingS2CEvt, PlayerTransPartyQueuedGameMatchingS2CEvt>
	{
	private:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameParty::QueuedGameMatchingS2CEvt, PlayerTransPartyQueuedGameMatchingS2CEvt> super;

	public:
		PlayerTransPartyQueuedGameMatchingS2CEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransPartyQueuedGameMatchingS2CEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};


	// Dequeued
	class PlayerTransPartyMatchingItemDequeuedS2CEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameParty::MatchingItemDequeuedS2CEvt, PlayerTransPartyMatchingItemDequeuedS2CEvt>
	{
	private:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameParty::MatchingItemDequeuedS2CEvt, PlayerTransPartyMatchingItemDequeuedS2CEvt> super;

	public:
		PlayerTransPartyMatchingItemDequeuedS2CEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransPartyMatchingItemDequeuedS2CEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};
	

	// Player leader changed
	class PlayerTransPartyCanceledGameMatchingS2CEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameParty::CanceledGameMatchingS2CEvt, PlayerTransPartyCanceledGameMatchingS2CEvt>
	{
	private:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameParty::CanceledGameMatchingS2CEvt, PlayerTransPartyCanceledGameMatchingS2CEvt> super;

	public:
		PlayerTransPartyCanceledGameMatchingS2CEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransPartyCanceledGameMatchingS2CEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};
		


	
	class PlayerTransCreateParty : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::CreatePartyCmd, PlayerTransCreateParty>
	{
	private:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::CreatePartyCmd, PlayerTransCreateParty> super;

	private:
		PartyUID m_PartyUID;

	public:
		PlayerTransCreateParty( Message::MessageData* &pIMsg );
		virtual ~PlayerTransCreateParty() {}

		HRESULT OnCreatePartyRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(CreatePartyRes,m_PartyUID);
	};
	
	
	
	class PlayerTransJoinParty : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::JoinPartyCmd, PlayerTransJoinParty>
	{
	private:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::JoinPartyCmd, PlayerTransJoinParty> super;

	private:
		PlayerID m_LeaderID;
		StaticArray<BYTE, 2048> m_ChatHistoryData;


	public:
		PlayerTransJoinParty( Message::MessageData* &pIMsg );
		virtual ~PlayerTransJoinParty() {}

		HRESULT OnJoinPartyRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(JoinPartyRes,GetPartyUID(),m_LeaderID, m_ChatHistoryData);
	};
	
	// Party player joined
	class PlayerTransPartyPlayerJoinedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameParty::PlayerJoinedS2CEvt, PlayerTransPartyPlayerJoinedS2SEvt>
	{
	private:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameParty::PlayerJoinedS2CEvt, PlayerTransPartyPlayerJoinedS2SEvt> super;

	public:
		PlayerTransPartyPlayerJoinedS2SEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransPartyPlayerJoinedS2SEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};
	
	
	// Player leader changed
	class PlayerTransPartyLeaderChangedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameParty::PartyLeaderChangedS2CEvt, PlayerTransPartyLeaderChangedS2SEvt>
	{
	private:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameParty::PartyLeaderChangedS2CEvt, PlayerTransPartyLeaderChangedS2SEvt> super;

	public:
		PlayerTransPartyLeaderChangedS2SEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransPartyLeaderChangedS2SEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};

	
	
	class PlayerTransLeaveParty : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::LeavePartyCmd, PlayerTransLeaveParty>
	{
	private:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::LeavePartyCmd, PlayerTransLeaveParty> super;

	public:
		PlayerTransLeaveParty( Message::MessageData* &pIMsg );
		virtual ~PlayerTransLeaveParty() {}

		HRESULT OnLeavePartyRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(LeavePartyRes);
	};
	
	
	
	// Player leader changed
	class PlayerTransPartyPlayerLeftS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameParty::PlayerLeftS2CEvt, PlayerTransPartyPlayerLeftS2SEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameParty::PlayerLeftS2CEvt, PlayerTransPartyPlayerLeftS2SEvt> super;

	public:
		PlayerTransPartyPlayerLeftS2SEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransPartyPlayerLeftS2SEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};

	
	
	
	class PlayerTransPartyKickPlayer : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::PartyKickPlayerCmd, PlayerTransPartyKickPlayer>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::PartyKickPlayerCmd, PlayerTransPartyKickPlayer> super;

	public:
		PlayerTransPartyKickPlayer( Message::MessageData* &pIMsg );
		virtual ~PlayerTransPartyKickPlayer() {}

		HRESULT OnPlayerKickRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(PartyKickPlayerRes);
	};
	
	
	
	// Player leader changed
	class PlayerTransPartyPlayerKickedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameParty::PlayerKickedS2CEvt, PlayerTransPartyPlayerKickedS2SEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameParty::PlayerKickedS2CEvt, PlayerTransPartyPlayerKickedS2SEvt> super;

	public:
		PlayerTransPartyPlayerKickedS2SEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransPartyPlayerKickedS2SEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};


	
	
	class PlayerTransPartyInvite : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::PartyInviteCmd, PlayerTransPartyInvite>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::PartyInviteCmd, PlayerTransPartyInvite> super;

	public:
		PlayerTransPartyInvite( Message::MessageData* &pIMsg );//:MessageTransaction(pIMsg) {}
		virtual ~PlayerTransPartyInvite() {}

		//HRESULT OnNotifyAdded(  Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(PartyInviteRes);
	};
	
	
	class PlayerTransNotifyPartyInviteS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameServer::NotifyPartyInviteC2SEvt, PlayerTransNotifyPartyInviteS2SEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameServer::NotifyPartyInviteC2SEvt, PlayerTransNotifyPartyInviteS2SEvt> super;

	public:
		PlayerTransNotifyPartyInviteS2SEvt( Message::MessageData* &pIMsg ):UserTransactionS2SEvt(pIMsg) {}
		virtual ~PlayerTransNotifyPartyInviteS2SEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};
	

	
	class PlayerTransPartyChatMessage : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::PartyChatMessageCmd, PlayerTransPartyChatMessage>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::PartyChatMessageCmd, PlayerTransPartyChatMessage> super;

	private:

	public:
		PlayerTransPartyChatMessage( Message::MessageData* &pIMsg )  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransPartyChatMessage() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(PartyChatMessageRes);
	};

	// Chat message from other entitity	
	class PlayerTransPartyChatMessageS2CEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameParty::ChatMessageS2CEvt, PlayerTransPartyChatMessageS2CEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameParty::ChatMessageS2CEvt, PlayerTransPartyChatMessageS2CEvt> super;

	public:
		PlayerTransPartyChatMessageS2CEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransPartyChatMessageS2CEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};
	
	
	class PlayerTransPartyQuickChatMessage : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::PartyQuickChatMessageCmd, PlayerTransPartyQuickChatMessage>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::PartyQuickChatMessageCmd, PlayerTransPartyQuickChatMessage> super;

	private:

	public:
		PlayerTransPartyQuickChatMessage( Message::MessageData* &pIMsg )  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransPartyQuickChatMessage() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(PartyQuickChatMessageRes);
	};

	// Chat message from other entitity	
	class PlayerTransPartyQuickChatMessageS2CEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameParty::QuickChatMessageS2CEvt, PlayerTransPartyQuickChatMessageS2CEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameParty::QuickChatMessageS2CEvt, PlayerTransPartyQuickChatMessageS2CEvt> super;

	public:
		PlayerTransPartyQuickChatMessageS2CEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransPartyQuickChatMessageS2CEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};


	
} // namespace GameServer 
} // namespace BR 

