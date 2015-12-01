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


#include "Net/NetDef.h"
#include "Common/BrAssert.h"
#include "Common/StaticHashTable.h"
#include "ServerSystem/SvrTypes.h"
#include "ServerSystem/SvrConfig.h"
#include "Common/SynchronizationTrait.h"


namespace BR {

	namespace Net {
		class IConnection;
	};

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
		UINT				m_MessageCode;
		const char*			m_FileName;
		UINT				m_FileLine;

	public:
		// Hash table mapping Item
		typedef OrderedLinkedList<UINT>::Node TableItemType;
		TableItemType TableNode;

		// MessageID for key
		UINT GetMessageCode() const { return m_MessageCode; }

		const char* GetFileName() const { return m_FileName; }
		UINT GetFileLine() const { return m_FileLine; }

		// Message Handler
		MessageHandlerType	Handler;

		// Constructor with constructor
		MessageHandler_TableItem(Message::MessageID MsgID, MessageHandlerType Handler, const char* fileName, UINT lineNumber)
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
		typedef Hash::StaticHashTable<	UINT, TableItem,
										//Indexing::ConstMemFunc<TableItem,UINT,&TableItem::GetMessageCode>,
										Indexing::MapItemConverter<TableItem,typename TableItem::TableItemType,&TableItem::TableNode>,
										Hash::NonUniqueKeyTrait, ThreadSyncTraitNone
										> HandlerTableType;
	private:
		HandlerTableType m_HandlerTable;

	public:
		
		MessageHandlerTable( MemoryAllocator &allocator )
			:m_Allocator(allocator)
		{
		}

		virtual ~MessageHandlerTable()
		{
			typename HandlerTableType::iterator itItem;
			m_HandlerTable.begin( itItem );
			while( itItem.IsValid() )
			{
				TableItem *pTableItem = *itItem;
				m_HandlerTable.erase( itItem );
				if( pTableItem ) m_Allocator.Free( pTableItem );
			}
		}


		// Register a new message handler
		template< class MessageClassType >
		HRESULT Register(const char* fileName, UINT lineNumber, MessageHandlerType newHandler )
		{
			auto key = MessageClassType::MID.IDSeq.MsgID;
			// prevent duplicated insert
			typename HandlerTableType::iterator itHandler;
			HRESULT hr = m_HandlerTable.find(key, itHandler );
			if( SUCCEEDED(hr) )
			{
				// Same category can be called multiple times. let's just ignore silently
				//auto pConflictedHandler = *itHandler;
				//svrTrace(Trace::TRC_ERROR, "Duplicated handler msgID:{0}, org:{1}:{2}", MessageClassType::MID, pConflictedHandler->GetFileName(), pConflictedHandler->GetFileLine())
				//Assert(false);
				return hr;
			}

			void* pPtr = nullptr;
			if( FAILED(m_Allocator.Alloc( sizeof(TableItem), pPtr )) )
				return E_OUTOFMEMORY;

			TableItem *pNewItem = new(pPtr) TableItem( MessageClassType::MID, newHandler, fileName, lineNumber);
			if( pNewItem == nullptr )
				return E_OUTOFMEMORY;

			return m_HandlerTable.insert(key, pNewItem );
		}

		// Get message handler
		FORCEINLINE HRESULT GetHandler( Message::MessageID msgID, MessageHandlerType &handler )
		{
			typename HandlerTableType::iterator itHandler;
			HRESULT hr = m_HandlerTable.find(msgID.IDSeq.MsgID, itHandler);
			if( FAILED(hr) )
				return hr;
			handler = itHandler->Handler;
			return S_OK;
		}


		// Call handler 
		HRESULT HandleMessage( Message::MessageData* &pMsg )
		{
			HRESULT hr = S_OK;
			MessageHandlerType handler;

			hr = GetHandler(pMsg->GetMessageHeader()->msgID,handler);
			if( FAILED(hr) ) return E_SVR_NO_MESSAGE_HANDLER;

			return handler( pMsg );
		}

		HRESULT HandleMessage( Message::MessageID msgID, TransactionResult* pRes )
		{
			HRESULT hr = S_OK;
			MessageHandlerType handler;

			hr = GetHandler(msgID,handler);
			if( FAILED(hr) ) return E_SVR_NO_RESULT_HANDLER;

			return handler( pRes );
		}

		template<class Param1>
		HRESULT HandleMessage( Net::IConnection * pCon, Message::MessageData* &pMsg, Param1 param1 )
		{
			HRESULT hr = S_OK;
			MessageHandlerType handler;

			hr = GetHandler(pMsg->GetMessageHeader()->msgID,handler);
			if( FAILED(hr) ) return E_SVR_NO_MESSAGE_HANDLER;

			return handler( pCon, pMsg, param1 );
		}
		
		template<class Param1, class Param2>
		HRESULT HandleMessage( Message::MessageData* &pMsg, Param1 param1, Param2 param2 )
		{
			HRESULT hr = S_OK;
			MessageHandlerType handler;

			hr = GetHandler(pMsg->GetMessageHeader()->msgID,handler);
			if( FAILED(hr) ) return E_SVR_NO_MESSAGE_HANDLER;

			return handler( pMsg, param1, param2 );
		}

	private:
		MemoryAllocator			&m_Allocator;
	};


	#define	BR_ENTITY_MESSAGE(MessageType) RegisterMessageHandler<MessageType>( __FILE__, __LINE__, [&]( Net::IConnection* pConn, Message::MessageData* &pMsgData, ::BR::Svr::Transaction* &pNewTrans)->HRESULT 

	#define BR_TRANS_MESSAGE(MessageType,MessageHandlerImpl) \
		RegisterMessageHandler<MessageType>( __FILE__, __LINE__, [&](::BR::Svr::TransactionResult* pRes)->HRESULT MessageHandlerImpl );


	typedef std::function<HRESULT(Net::IConnection *, Message::MessageData* &, Transaction* &)>	EntityMessageHandlerItem;




}; // namespace Svr
}; // namespace BR




