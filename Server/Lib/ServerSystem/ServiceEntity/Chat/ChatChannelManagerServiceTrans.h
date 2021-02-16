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
#include "Net/SFMessage.h"
#include "Protocol/Message/ChatChannelManagerMsgClass.h"
#include "Protocol/Policy/ChatChannelManagerNetPolicy.h"
#include "Transaction/MessageRoute.h"
#include "ServiceEntity/Chat/ChatChannelManagerServiceEntity.h"
#include "Transaction/ServerTransaction.h"


namespace SF {
	namespace Svr {


		class ChatChannelManagerTransCreateChatChannel : public ClusterEntityMessageTransaction< ChatChannelManagerServiceEntity, Message::ChatChannelManager::CreateChannelCmd>
		{
		public:
			typedef ClusterEntityMessageTransaction< ChatChannelManagerServiceEntity, Message::ChatChannelManager::CreateChannelCmd> super;

		private:
			EntityUID	m_UID;

		public:
			ChatChannelManagerTransCreateChatChannel(IHeap& heap, MessageDataPtr& pIMsg) : ClusterEntityMessageTransaction(heap, pIMsg) {}
			virtual ~ChatChannelManagerTransCreateChatChannel() {}

			// Start Transaction
			virtual Result StartTransaction() override;


			BR_SVR_MSGTRANS_CLOSE_ARGS(NetSvrPolicyChatChannelManager, CreateChannelRes, RouteContext(m_UID, GetRouteContext().GetFrom()), m_UID);
		};



		class ChatChannelManagerTransFindChatChannel : public ClusterEntityMessageTransaction< ChatChannelManagerServiceEntity, Message::ChatChannelManager::FindChannelCmd>
		{
		public:
			typedef ClusterEntityMessageTransaction< ChatChannelManagerServiceEntity, Message::ChatChannelManager::FindChannelCmd> super;

		private:
			EntityUID	m_UID;

		public:
			ChatChannelManagerTransFindChatChannel(IHeap& heap, MessageDataPtr& pIMsg) : ClusterEntityMessageTransaction(heap, pIMsg) {}
			virtual ~ChatChannelManagerTransFindChatChannel() {}

			// Start Transaction
			virtual Result StartTransaction() override;


			BR_SVR_MSGTRANS_CLOSE_ARGS(NetSvrPolicyChatChannelManager, FindChannelRes, RouteContext(m_UID, GetRouteContext().GetFrom()), m_UID);
		};




		class ChatChannelManagerTransChatChannelDeleted : public ClusterEntityMessageTransaction< ChatChannelManagerServiceEntity, Message::ChatChannelManager::ChatChannelDeletedC2SEvt>
		{
		public:
			typedef ClusterEntityMessageTransaction< ChatChannelManagerServiceEntity, Message::ChatChannelManager::ChatChannelDeletedC2SEvt> super;

		private:

		public:
			ChatChannelManagerTransChatChannelDeleted(IHeap& heap, MessageDataPtr& pIMsg) : ClusterEntityMessageTransaction(heap, pIMsg) {}
			virtual ~ChatChannelManagerTransChatChannelDeleted() {}

			// Start Transaction
			virtual Result StartTransaction() override;
		};


	} // namespace GameServer 
} // namespace SF 

