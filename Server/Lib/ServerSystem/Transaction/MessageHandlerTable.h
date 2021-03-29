////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server Utility definition
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Net/SFNetDef.h"
#include "SFAssert.h"
#include "Container/SFStaticHashTable.h"
#include "Types/BrSvrTypes.h"
#include "Container/SFContainerTrait.h"
#include "Net/SFMessage.h"
#include "ResultCode/SFResultCodeSvr.h"



namespace SF {

	namespace Message {
		class MessageData;
	};
	namespace Net {
		class Connection;
	};
}

namespace SF {

	class MessageEndpoint;

namespace Svr {

	class SimpleUserEntity;
	class Transaction;
	class TransactionResult;


	///////////////////////////////////////////////////////////////////////////////////////
	//
	// Message map helper
	//

	template< class MessageHandlerType >
	struct MessageHandler_TableItem
	{
	private:
		// MessageID for key
		uint				m_MessageCode;

	public:
		// Hash table mapping Item
		typedef OrderedLinkedList<uint>::Node TableItemType;
		TableItemType TableNode;

		// MessageID for key
		uint GetMessageCode() const { return m_MessageCode; }

		// Message Handler
		MessageHandlerType	Handler;

		// Constructor with constructor
		MessageHandler_TableItem(SF::Message::MessageID MsgID, MessageHandlerType Handler)
			: m_MessageCode(MsgID.IDSeq.MsgID)
			, Handler(Handler)
		{
			memset(&TableNode, 0, sizeof(TableNode));
		}
	};


	///////////////////////////////////////////////////////
	//
	//	Message handler table class
	//

	template< class MessageHandlerType >
	class MessageHandlerTable
	{
	public:
		using TableItem = MessageHandler_TableItem<MessageHandlerType>;
		using HandlerTableType = StaticHashTable<uint, TableItem,
										//ConstMemFunc<TableItem,uint,&TableItem::GetMessageCode>,
										MapItemConverter<TableItem,typename TableItem::TableItemType,&TableItem::TableNode>,
										NonUniqueKeyTrait, ThreadSyncTraitNone
										>;
	private:
		IHeap& m_Heap;

		HandlerTableType m_HandlerTable;

	public:
		
		MessageHandlerTable( IHeap &allocator )
			: m_HandlerTable(allocator)
			, m_Heap(allocator)
		{
		}

		virtual ~MessageHandlerTable()
		{
			typename HandlerTableType::iterator itItem = m_HandlerTable.begin();
			while( itItem.IsValid() )
			{
				TableItem *pTableItem = *itItem;
				if (pTableItem == nullptr) continue;

				m_HandlerTable.erase( itItem );

				IHeap::Delete(pTableItem);

				itItem = m_HandlerTable.begin();
			}
		}

		IHeap& GetHeap() { return m_Heap; }

		// Register a new message handler
		template< class MessageClassType >
		Result Register(MessageHandlerType&& newHandler )
		{
			auto key = MessageClassType::MID.IDSeq.MsgID;
			// prevent duplicated insert
			typename HandlerTableType::iterator itHandler;
			Result hr = m_HandlerTable.find(key, itHandler );
			if( (hr) )
			{
				// Same category can be called multiple times. let's just ignore silently
				//auto pConflictedHandler = *itHandler;
				//svrTrace(Error, "Duplicated handler msgID:{0}, org:{1}:{2}", MessageClassType::MID, pConflictedHandler->GetFileName(), pConflictedHandler->GetFileLine())
				//Assert(false);
				return hr;
			}

			TableItem *pNewItem = nullptr;
			pNewItem = new(GetHeap()) TableItem(MessageClassType::MID, std::move(newHandler));
			
			if( pNewItem == nullptr )
				return ResultCode::OUT_OF_MEMORY;

			return m_HandlerTable.insert(key, pNewItem );
		}

		// Get message handler
		SF_FORCEINLINE Result GetHandler( Message::MessageID msgID, MessageHandlerType &outHandler )
		{
			typename HandlerTableType::iterator itHandler;
			Result hr = m_HandlerTable.find(msgID.IDSeq.MsgID, itHandler);
			if( !(hr) )
				return hr;
			outHandler = itHandler->Handler;
			return ResultCode::SUCCESS;
		}


		// Call handler 
		Result HandleMessage(const MessageDataPtr &pMsg)
		{
			Result hr = ResultCode::SUCCESS;
			MessageHandlerType handler;

			hr = GetHandler(pMsg->GetMessageHeader()->msgID,handler);
			if( !(hr) ) return ResultCode::SVR_NO_MESSAGE_HANDLER;

			return handler( pMsg );
		}

		Result HandleMessage( Message::MessageID msgID, TransactionResult* pRes )
		{
			Result hr = ResultCode::SUCCESS;
			MessageHandlerType handler;

			hr = GetHandler(msgID,handler);
			if( !(hr) ) return ResultCode::SVR_NO_RESULT_HANDLER;

			return handler( pRes );
		}

		template<class Param1>
		Result HandleMessage( Net::Connection * pCon, const MessageDataPtr &pMsg, Param1 param1 )
		{
			Result hr = ResultCode::SUCCESS;
			MessageHandlerType handler;

			hr = GetHandler(pMsg->GetMessageHeader()->msgID,handler);
			if( !(hr) ) return ResultCode::SVR_NO_MESSAGE_HANDLER;

			return handler( pCon, pMsg, param1 );
		}
		

		template<class Param1>
		Result HandleMessage(const SharedPointerT<MessageEndpoint>& remoteEndpoint, const MessageDataPtr &pMsg, Param1 param1 )
		{
			Result hr = ResultCode::SUCCESS;
			MessageHandlerType handler;

			hr = GetHandler(pMsg->GetMessageHeader()->msgID,handler);
			if( !(hr) ) return ResultCode::SVR_NO_MESSAGE_HANDLER;

			return handler(remoteEndpoint, pMsg, param1);
		}

		template<class Param1>
		Result HandleMessage(const MessageDataPtr& pMsg, Param1 param1)
		{
			Result hr = ResultCode::SUCCESS;
			MessageHandlerType handler;

			hr = GetHandler(pMsg->GetMessageHeader()->msgID, handler);
			if (!(hr)) return ResultCode::SVR_NO_MESSAGE_HANDLER;

			return handler(pMsg, param1);
		}

		template<class Param1, class Param2>
		Result HandleMessage(const MessageDataPtr &pMsg, Param1 param1, Param2 param2 )
		{
			Result hr = ResultCode::SUCCESS;
			MessageHandlerType handler;

			hr = GetHandler(pMsg->GetMessageHeader()->msgID,handler);
			if( !(hr) ) return ResultCode::SVR_NO_MESSAGE_HANDLER;

			return handler( pMsg, param1, param2 );
		}

	};


	#define	BR_ENTITY_MESSAGE(MessageType) RegisterMessageHandler<MessageType>([=](const MessageDataPtr &pMsgData, SF::TransactionPtr &pNewTrans)->Result 

	#define BR_TRANS_MESSAGE(MessageType,MessageHandlerImpl) \
		RegisterMessageHandler<MessageType>([=](::SF::Svr::TransactionResult* pRes)->Result MessageHandlerImpl );


	typedef std::function<Result(const SharedPointerT<MessageEndpoint>& , const MessageDataPtr &, Transaction* &)>	EntityMessageHandlerItem;




} // namespace Svr
} // namespace SF





