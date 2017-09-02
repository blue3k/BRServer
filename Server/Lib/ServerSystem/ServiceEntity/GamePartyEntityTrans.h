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
#include "Common/MemoryStream.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameNetPolicy.h"
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Message/GamePartyMsgClass.h"
#include "Protocol/Policy/GamePartyNetPolicy.h"

#include "Protocol/Message/PartyMatchingQueueMsgClass.h"
#include "Protocol/Message/PartyMatchingMsgClass.h"


#include "ServerSystem/MessageRoute.h"

#include "ServerSystem/ServiceEntity/GamePartyEntity.h"
#include "Net/Message.h"
#include "ServerSystem/ServiceEntity/GamePartyTransaction.h"


namespace SF {
namespace Svr {

	class ServerEntity;



	// Close zone instance
	class PartyTransCloseInstance : public Transaction , public MemoryPoolObject<PartyTransCloseInstance>
	{
	private:

	public:
		PartyTransCloseInstance() : Svr::Transaction( TransactionID() ) { SetExclusive(true); }
		virtual ~PartyTransCloseInstance() {}

		// Start Transaction
		virtual Result StartTransaction();

		void Release() { delete this; }
	};



	/////////////////////////////////////////////////////////////////////////////
	//
	//	Party transaction
	//



	class PartyTransJoinParty : public GamePartyMessageTransaction< Policy::NetSvrPolicyGameParty, Message::GameParty::JoinPartyCmd, PartyTransJoinParty, 1>
	{
	public:
		typedef GamePartyMessageTransaction< Policy::NetSvrPolicyGameParty, Message::GameParty::JoinPartyCmd, PartyTransJoinParty, 1> super;

	private:
		PlayerID m_LeaderID;
		StaticOutputMemoryStream<GameConst::MAX_CHATLOG_BUFFER> m_MessageBuffer;

	public:
		PartyTransJoinParty( Message::MessageData* &pIMsg ) : GamePartyMessageTransaction( pIMsg ) {}
		virtual ~PartyTransJoinParty() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_SVR_MSGTRANS_CLOSE_ARGS(JoinPartyRes,GetRouteContext().GetSwaped(), m_LeaderID,m_MessageBuffer.ToArray());
	};
	

	class PartyTransLeaveParty : public GamePartyMessageTransaction< Policy::NetSvrPolicyGameParty, Message::GameParty::LeavePartyCmd, PartyTransLeaveParty, 1>
	{
	public:
		typedef GamePartyMessageTransaction< Policy::NetSvrPolicyGameParty, Message::GameParty::LeavePartyCmd, PartyTransLeaveParty, 1> super;

	private:

	public:
		PartyTransLeaveParty( Message::MessageData* &pIMsg ) : GamePartyMessageTransaction( pIMsg ) {}
		virtual ~PartyTransLeaveParty() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_SVR_MSGTRANS_CLOSE(LeavePartyRes,GetRouteContext().GetSwaped());
	};

	

	class PartyTransKickPlayer : public GamePartyMessageTransaction< Policy::NetSvrPolicyGameParty, Message::GameParty::KickPlayerCmd, PartyTransKickPlayer, 1>
	{
	public:
		typedef GamePartyMessageTransaction< Policy::NetSvrPolicyGameParty, Message::GameParty::KickPlayerCmd, PartyTransKickPlayer, 1> super;

	private:

	public:
		PartyTransKickPlayer( Message::MessageData* &pIMsg ) : GamePartyMessageTransaction( pIMsg ) {}
		virtual ~PartyTransKickPlayer() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_SVR_MSGTRANS_CLOSE(KickPlayerRes,GetRouteContext().GetSwaped());
	};
	
	

	class PartyTransChatMessage : public GamePartyMessageTransaction< Policy::NetSvrPolicyGameParty, Message::GameParty::ChatMessageC2SEvt, PartyTransChatMessage, 1>
	{
	public:
		typedef GamePartyMessageTransaction< Policy::NetSvrPolicyGameParty, Message::GameParty::ChatMessageC2SEvt, PartyTransChatMessage, 1> super;

	private:

	public:
		PartyTransChatMessage( Message::MessageData* &pIMsg ) : GamePartyMessageTransaction( pIMsg ) {}
		virtual ~PartyTransChatMessage() {}

		// Start Transaction
		virtual Result StartTransaction();
	};

	
	class PartyTransQuickChatMessage : public GamePartyMessageTransaction< Policy::NetSvrPolicyGameParty, Message::GameParty::QuickChatMessageC2SEvt, PartyTransQuickChatMessage, 1>
	{
	public:
		typedef GamePartyMessageTransaction< Policy::NetSvrPolicyGameParty, Message::GameParty::QuickChatMessageC2SEvt, PartyTransQuickChatMessage, 1> super;

	private:

	public:
		PartyTransQuickChatMessage( Message::MessageData* &pIMsg ) : GamePartyMessageTransaction( pIMsg ) {}
		virtual ~PartyTransQuickChatMessage() {}

		// Start Transaction
		virtual Result StartTransaction();
	};


	// Player voted event
	class PartyTransStartGameMatchCmd : public Svr::GamePartyMessageTransaction< Policy::NetSvrPolicyGameParty, Message::GameParty::StartGameMatchCmd, PartyTransStartGameMatchCmd, sizeof(TransactionMessageHandlerType)*2>
	{
	public:
		typedef Svr::GamePartyMessageTransaction< Policy::NetSvrPolicyGameParty, Message::GameParty::StartGameMatchCmd, PartyTransStartGameMatchCmd, sizeof(TransactionMessageHandlerType) * 2> super;

	public:
		PartyTransStartGameMatchCmd( Message::MessageData* &pIMsg );//  :GamePartyMessageTransaction( pIMsg ) {}
		virtual ~PartyTransStartGameMatchCmd() {}

		Result OnPartyMatchingQueued( Svr::TransactionResult* &pRes );
		Result OnCreateGame(TransactionResult* pRes);

		// Start Transaction
		virtual Result StartTransaction();

		BR_SVR_MSGTRANS_CLOSE(StartGameMatchRes,GetRouteContext().GetSwaped());
	};


	// Player voted event
	class PartyTransCancelGameMatchCmd : public Svr::GamePartyMessageTransaction< Policy::NetSvrPolicyGameParty, Message::GameParty::CancelGameMatchCmd, PartyTransCancelGameMatchCmd, sizeof(TransactionMessageHandlerType)*2>
	{
	public:
		typedef Svr::GamePartyMessageTransaction< Policy::NetSvrPolicyGameParty, Message::GameParty::CancelGameMatchCmd, PartyTransCancelGameMatchCmd, sizeof(TransactionMessageHandlerType) * 2> super;

	public:
		PartyTransCancelGameMatchCmd( Message::MessageData* &pIMsg );//  :GamePartyMessageTransaction( pIMsg ) {}
		virtual ~PartyTransCancelGameMatchCmd() {}

		Result OnPartyMatchingCanceled( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_SVR_MSGTRANS_CLOSE(CancelGameMatchRes,GetRouteContext().GetSwaped());
	};

	
	class PartyTransPartyMatchingCanceled : public GamePartyMessageTransaction< GamePartyEntity, Message::PartyMatchingQueue::PartyMatchingCanceledS2CEvt, PartyTransPartyMatchingCanceled>
	{
	public:
		typedef GamePartyMessageTransaction< GamePartyEntity, Message::PartyMatchingQueue::PartyMatchingCanceledS2CEvt, PartyTransPartyMatchingCanceled> super;

	private:

	public:
		PartyTransPartyMatchingCanceled( Message::MessageData* &pIMsg ) : GamePartyMessageTransaction( pIMsg ) {}
		virtual ~PartyTransPartyMatchingCanceled() {}

		// Start Transaction
		virtual Result StartTransaction();
	};


	class PartyTransMatchingItemDequeued : public GamePartyMessageTransaction< GamePartyEntity, Message::PartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt, PartyTransMatchingItemDequeued>
	{
	public:
		typedef GamePartyMessageTransaction< GamePartyEntity, Message::PartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt, PartyTransMatchingItemDequeued> super;

	private:

	public:
		PartyTransMatchingItemDequeued( Message::MessageData* &pIMsg ) : GamePartyMessageTransaction( pIMsg ) {}
		virtual ~PartyTransMatchingItemDequeued() {}

		// Start Transaction
		virtual Result StartTransaction();
	};
	

	class PartyTransPartyGameMatchedS2CEvt : public GamePartyMessageTransaction< GamePartyEntity, Message::PartyMatching::PartyGameMatchedS2CEvt, PartyTransPartyGameMatchedS2CEvt>
	{
	public:
		typedef GamePartyMessageTransaction< GamePartyEntity, Message::PartyMatching::PartyGameMatchedS2CEvt, PartyTransPartyGameMatchedS2CEvt> super;

	private:

	public:
		PartyTransPartyGameMatchedS2CEvt( Message::MessageData* &pIMsg ) : GamePartyMessageTransaction( pIMsg ) {}
		virtual ~PartyTransPartyGameMatchedS2CEvt() {}

		// Start Transaction
		virtual Result StartTransaction();
	};




} // namespace Svr 
} // namespace SF 

