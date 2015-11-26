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

#include "Protocol/Message/PartyMatchingQueueMsgClass.h"
#include "Protocol/Message/PartyMatchingMsgClass.h"


#include "ServerSystem/MessageRoute.h"

#include "ServerSystem/ServiceEntity/GamePartyEntity.h"
#include "Common/Message.h"
#include "ServerSystem/ServiceEntity/GamePartyTransaction.h"


namespace BR {
namespace Svr {

	class ServerEntity;



	// Close zone instance
	class PartyTransCloseInstance : public Svr::Transaction , public MemoryPoolObject<PartyTransCloseInstance>
	{
	private:

	public:
		PartyTransCloseInstance() : Svr::Transaction( TransactionID() ) { SetExclusive(true); }
		virtual ~PartyTransCloseInstance() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		void Release() { delete this; }
	};



	/////////////////////////////////////////////////////////////////////////////
	//
	//	GCM register/unregister transaction
	//

	//class PartyTransCreateParty : public Svr::MessageTransaction< ServerEntity, Policy::ISvrPolicyGameParty, Message::GameParty::CreatePartyCmd, PartyTransCreateParty, 1>
	//{
	//private:
	//	PartyUID m_PartyUID;

	//public:
	//	PartyTransCreateParty( Message::MessageData* &pIMsg ) : MessageTransaction( pIMsg ) {}
	//	virtual ~PartyTransCreateParty() {}

	//	// Start Transaction
	//	virtual HRESULT StartTransaction();

	//	BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(CreatePartyRes, RouteContext( GetRouteContext().To, m_PartyUID ) );
	//};



	class PartyTransJoinParty : public GamePartyMessageTransaction< Policy::ISvrPolicyGameParty, Message::GameParty::JoinPartyCmd, PartyTransJoinParty, 1>
	{
	private:
		PlayerID m_LeaderID;
		StaticOutputMemoryStream<GameConst::MAX_CHATLOG_BUFFER> m_MessageBuffer;

	public:
		PartyTransJoinParty( Message::MessageData* &pIMsg ) : GamePartyMessageTransaction( pIMsg ) {}
		virtual ~PartyTransJoinParty() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(JoinPartyRes,GetRouteContext().GetSwaped(), m_LeaderID,m_MessageBuffer.ToArray());
	};
	

	class PartyTransLeaveParty : public GamePartyMessageTransaction< Policy::ISvrPolicyGameParty, Message::GameParty::LeavePartyCmd, PartyTransLeaveParty, 1>
	{
	private:

	public:
		PartyTransLeaveParty( Message::MessageData* &pIMsg ) : GamePartyMessageTransaction( pIMsg ) {}
		virtual ~PartyTransLeaveParty() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(LeavePartyRes,GetRouteContext().GetSwaped());
	};

	

	class PartyTransKickPlayer : public GamePartyMessageTransaction< Policy::ISvrPolicyGameParty, Message::GameParty::KickPlayerCmd, PartyTransKickPlayer, 1>
	{
	private:

	public:
		PartyTransKickPlayer( Message::MessageData* &pIMsg ) : GamePartyMessageTransaction( pIMsg ) {}
		virtual ~PartyTransKickPlayer() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(KickPlayerRes,GetRouteContext().GetSwaped());
	};
	
	

	class PartyTransChatMessage : public GamePartyMessageTransaction< Policy::ISvrPolicyGameParty, Message::GameParty::ChatMessageC2SEvt, PartyTransChatMessage, 1>
	{
	private:

	public:
		PartyTransChatMessage( Message::MessageData* &pIMsg ) : GamePartyMessageTransaction( pIMsg ) {}
		virtual ~PartyTransChatMessage() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};

	
	class PartyTransQuickChatMessage : public GamePartyMessageTransaction< Policy::ISvrPolicyGameParty, Message::GameParty::QuickChatMessageC2SEvt, PartyTransQuickChatMessage, 1>
	{
	private:

	public:
		PartyTransQuickChatMessage( Message::MessageData* &pIMsg ) : GamePartyMessageTransaction( pIMsg ) {}
		virtual ~PartyTransQuickChatMessage() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};


	// Player voted event
	class PartyTransStartGameMatchCmd : public Svr::GamePartyMessageTransaction< Policy::ISvrPolicyGameParty, Message::GameParty::StartGameMatchCmd, PartyTransStartGameMatchCmd, sizeof(TransactionMessageHandlerType)*2>
	{
	public:
		PartyTransStartGameMatchCmd( Message::MessageData* &pIMsg );//  :GamePartyMessageTransaction( pIMsg ) {}
		virtual ~PartyTransStartGameMatchCmd() {}

		HRESULT OnPartyMatchingQueued( Svr::TransactionResult* &pRes );
		HRESULT OnCreateGame(TransactionResult* pRes);

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(StartGameMatchRes,GetRouteContext().GetSwaped());
	};


	// Player voted event
	class PartyTransCancelGameMatchCmd : public Svr::GamePartyMessageTransaction< Policy::ISvrPolicyGameParty, Message::GameParty::CancelGameMatchCmd, PartyTransCancelGameMatchCmd, sizeof(TransactionMessageHandlerType)*2>
	{
	public:
		PartyTransCancelGameMatchCmd( Message::MessageData* &pIMsg );//  :GamePartyMessageTransaction( pIMsg ) {}
		virtual ~PartyTransCancelGameMatchCmd() {}

		HRESULT OnPartyMatchingCanceled( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(CancelGameMatchRes,GetRouteContext().GetSwaped());
	};

	
	class PartyTransPartyMatchingCanceled : public GamePartyMessageTransaction< GamePartyEntity, Message::PartyMatchingQueue::PartyMatchingCanceledS2CEvt, PartyTransPartyMatchingCanceled>
	{
	private:

	public:
		PartyTransPartyMatchingCanceled( Message::MessageData* &pIMsg ) : GamePartyMessageTransaction( pIMsg ) {}
		virtual ~PartyTransPartyMatchingCanceled() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};


	class PartyTransMatchingItemDequeued : public GamePartyMessageTransaction< GamePartyEntity, Message::PartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt, PartyTransMatchingItemDequeued>
	{
	private:

	public:
		PartyTransMatchingItemDequeued( Message::MessageData* &pIMsg ) : GamePartyMessageTransaction( pIMsg ) {}
		virtual ~PartyTransMatchingItemDequeued() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};
	

	class PartyTransPartyGameMatchedS2CEvt : public GamePartyMessageTransaction< GamePartyEntity, Message::PartyMatching::PartyGameMatchedS2CEvt, PartyTransPartyGameMatchedS2CEvt>
	{
	private:

	public:
		PartyTransPartyGameMatchedS2CEvt( Message::MessageData* &pIMsg ) : GamePartyMessageTransaction( pIMsg ) {}
		virtual ~PartyTransPartyGameMatchedS2CEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};




} // namespace Svr 
} // namespace BR 

