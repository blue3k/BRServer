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
#include "Memory/SFMemoryPool.h"
#include "Container/SFArray.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"

#include "Protocol/Message/ChatChannelMsgClass.h"
#include "Protocol/Policy/ChatChannelNetPolicy.h"



#include "Transaction/MessageRoute.h"

#include "Net/SFMessage.h"
#include "ServiceEntity/Chat/ChatChannelTransaction.h"
#include "ServiceEntity/Chat/ChatChannelEntity.h"


namespace SF {
namespace Svr {

	class ServerEntity;



	// Close zone instance
	class ChatChannelTransCloseInstance : public Transaction 
	{
	private:

	public:
		ChatChannelTransCloseInstance(IHeap& memMgr)
			: Svr::Transaction( memMgr, TransactionID() )
		{ SetExclusive(true); }
		virtual ~ChatChannelTransCloseInstance() {}

		// Start Transaction
		virtual Result StartTransaction();
	};



	/////////////////////////////////////////////////////////////////////////////
	//
	//	Party transaction
	//



	class ChatChannelTransJoin : public ChatChannelMessageTransaction< Message::ChatChannel::JoinCmd>
	{
	public:
		typedef ChatChannelMessageTransaction< Message::ChatChannel::JoinCmd> super;

	private:
		PlayerID m_LeaderID;

	public:
		ChatChannelTransJoin( IHeap& memMgr, MessageDataPtr &pIMsg )
			: ChatChannelMessageTransaction( memMgr, pIMsg )
		{}
		virtual ~ChatChannelTransJoin() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_SVR_MSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyChatChannel, JoinRes, GetRouteContext().GetSwaped(), m_LeaderID);
	};


	class ChatChannelTransLeave : public ChatChannelMessageTransaction< Message::ChatChannel::LeaveCmd>
	{
	public:
		typedef ChatChannelMessageTransaction< Message::ChatChannel::LeaveCmd> super;

	private:

	public:
		ChatChannelTransLeave(IHeap& memHeap, MessageDataPtr &pIMsg ) : ChatChannelMessageTransaction( memHeap, pIMsg ) {}
		virtual ~ChatChannelTransLeave() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_SVR_MSGTRANS_CLOSE(Policy::NetSvrPolicyChatChannel, LeaveRes,GetRouteContext().GetSwaped());
	};

	

	class ChatChannelTransKickPlayer : public ChatChannelMessageTransaction< Message::ChatChannel::KickPlayerCmd>
	{
	public:
		typedef ChatChannelMessageTransaction<Message::ChatChannel::KickPlayerCmd> super;

	private:

	public:
		ChatChannelTransKickPlayer(IHeap& memHeap, MessageDataPtr &pIMsg ) : ChatChannelMessageTransaction(memHeap, pIMsg ) {}
		virtual ~ChatChannelTransKickPlayer() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_SVR_MSGTRANS_CLOSE(Policy::NetSvrPolicyChatChannel, KickPlayerRes,GetRouteContext().GetSwaped());
	};
	
	

	class ChatChannelTransChatMessage : public ChatChannelMessageTransaction< Message::ChatChannel::ChatMessageC2SEvt>
	{
	public:
		typedef ChatChannelMessageTransaction< Message::ChatChannel::ChatMessageC2SEvt> super;

	private:

	public:
		ChatChannelTransChatMessage(IHeap& memHeap, MessageDataPtr &pIMsg ) : ChatChannelMessageTransaction(memHeap, pIMsg ) {}
		virtual ~ChatChannelTransChatMessage() {}

		// Start Transaction
		virtual Result StartTransaction();
	};

	



} // namespace Svr 
} // namespace SF 
