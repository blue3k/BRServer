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
		const char*			m_FileName;
		uint				m_FileLine;

	public:
		// Hash table mapping Item
		typedef OrderedLinkedList<uint>::Node TableItemType;
		TableItemType TableNode;

		// MessageID for key
		uint GetMessageCode() const { return m_MessageCode; }

		const char* GetFileName() const { return m_FileName; }
		uint GetFileLine() const { return m_FileLine; }

		// Message Handler
		MessageHandlerType	Handler;

		// Constructor with constructor
		MessageHandler_TableItem(SF::Message::MessageID MsgID, MessageHandlerType Handler, const char* fileName, uint lineNumber)
			: m_MessageCode(MsgID.IDSeq.MsgID)
			, m_FileName(fileName)
			, m_FileLine(lineNumber)
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
		typedef MessageHandler_TableItem<MessageHandlerType> TableItem;
		typedef StaticHashTable<	uint, TableItem,
										//ConstMemFunc<TableItem,uint,&TableItem::GetMessageCode>,
										MapItemConverter<TableItem,typename TableItem::TableItemType,&TableItem::TableNode>,
										NonUniqueKeyTrait, ThreadSyncTraitNone
										> HandlerTableType;
	private:
		HandlerTableType m_HandlerTable;

	public:
		
		MessageHandlerTable( IHeap &allocator )
			: m_HandlerTable(allocator)
			, m_Allocator(allocator)
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


		// Register a new message handler
		template< class MessageClassType >
		Result Register(const char* fileName, uint lineNumber, MessageHandlerType newHandler )
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
			pNewItem = new(m_Allocator) TableItem(MessageClassType::MID, newHandler, fileName, lineNumber);
			
			if( pNewItem == nullptr )
				return ResultCode::OUT_OF_MEMORY;

			return m_HandlerTable.insert(key, pNewItem );
		}

		// Get message handler
		FORCEINLINE Result GetHandler( Message::MessageID msgID, MessageHandlerType &handler )
		{
			typename HandlerTableType::iterator itHandler;
			Result hr = m_HandlerTable.find(msgID.IDSeq.MsgID, itHandler);
			if( !(hr) )
				return hr;
			handler = itHandler->Handler;
			return ResultCode::SUCCESS;
		}


		// Call handler 
		Result HandleMessage( MessageDataPtr &pMsg )
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
		Result HandleMessage( Net::Connection * pCon, MessageDataPtr &pMsg, Param1 param1 )
		{
			Result hr = ResultCode::SUCCESS;
			MessageHandlerType handler;

			hr = GetHandler(pMsg->GetMessageHeader()->msgID,handler);
			if( !(hr) ) return ResultCode::SVR_NO_MESSAGE_HANDLER;

			return handler( pCon, pMsg, param1 );
		}
		
		template<class Param1, class Param2>
		Result HandleMessage(MessageDataPtr &pMsg, Param1 param1, Param2 param2 )
		{
			Result hr = ResultCode::SUCCESS;
			MessageHandlerType handler;

			hr = GetHandler(pMsg->GetMessageHeader()->msgID,handler);
			if( !(hr) ) return ResultCode::SVR_NO_MESSAGE_HANDLER;

			return handler( pMsg, param1, param2 );
		}

	private:
		IHeap			&m_Allocator;
	};


	#define	BR_ENTITY_MESSAGE(MessageType) RegisterMessageHandler<MessageType>( __FILE__, __LINE__, [&]( Net::Connection* pConn, MessageDataPtr &pMsgData, SF::TransactionPtr &pNewTrans)->Result 

	#define BR_TRANS_MESSAGE(MessageType,MessageHandlerImpl) \
		RegisterMessageHandler<MessageType>( __FILE__, __LINE__, [&](::SF::Svr::TransactionResult* pRes)->Result MessageHandlerImpl );


	typedef std::function<Result(Net::Connection *, MessageDataPtr &, Transaction* &)>	EntityMessageHandlerItem;




}; // namespace Svr
}; // namespace SF





