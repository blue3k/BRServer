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


#include "Net/Netdef.h"
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

	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Route message
	//

	// Route Message utility func
	template< class MsgClass >
	HRESULT RouteMessage( Message::MessageData* &pMsg )
	{
		HRESULT hr = S_OK;
		Svr::ServerEntity* pServerEntity = nullptr;
		MsgClass msg;

		svrChk( msg.ParseIMsg(pMsg) );

		svrChkErr( E_SVR_ROUTEMSG_FAILED, Svr::BrServer::GetInstance()->GetServerEntityManager()->GetServerEntity( msg.GetRouteContext().To, pServerEntity ) );

		svrChkPtr( pServerEntity->GetConnection() );
		svrChk( pServerEntity->GetConnection()->Send( pMsg ) );

	Proc_End:

		Util::SafeRelease( pMsg );

		return hr;
	}
	
	

	///////////////////////////////////////////////////////////////////////////////////////
	//
	// Message map helper
	//

	template< class MessageHandlerType >
	struct MessageHandler_TableItem
	{
	public:
		// Hash table mapping Item
		typedef OrderedLinkedList<UINT>::Node TableItemType;
		TableItemType TableNode;

		// MessageID for key
		UINT GetMessageCode() const { return MessageCode; }

		// Message Handler
		MessageHandlerType	Handler;

		// Constructor with constructor
		MessageHandler_TableItem( Message::MessageID MsgID, MessageHandlerType Handler )
			:MessageCode(MsgID.IDSeq.MsgID),
			Handler(Handler)
		{
			memset(&TableNode, 0, sizeof(TableNode) );
		}

	private:
		// MessageID for key
		UINT				MessageCode;
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
			HandlerTableType::iterator itItem;
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
		HRESULT Register( MessageHandlerType newHandler )
		{
			// prevent duplicated insert
			HandlerTableType::iterator itHandler;
			HRESULT hr = m_HandlerTable.find( typename MessageClassType::MID, itHandler );
			if( SUCCEEDED(hr) )
			{
				Assert(false);
				return hr;
			}

			void* pPtr = nullptr;
			if( FAILED(m_Allocator.Alloc( sizeof(TableItem), pPtr )) )
				return E_OUTOFMEMORY;

			TableItem *pNewItem = new(pPtr) TableItem( typename MessageClassType::MID, newHandler );
			if( pNewItem == nullptr )
				return E_OUTOFMEMORY;

			return m_HandlerTable.insert(typename MessageClassType::MID, pNewItem );
		}

		// Get message handler
		FORCEINLINE HRESULT GetHandler( Message::MessageID msgID, MessageHandlerType &handler )
		{
			HandlerTableType::iterator itHandler;
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

		HRESULT HandleMessage( TransactionResult* pRes )
		{
			HRESULT hr = S_OK;
			MessageHandlerType handler;

			hr = GetHandler(pRes->GetMsgID(),handler);
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


	#define	BR_ENTITY_MESSAGE(MessageType) RegisterMessageHandler<MessageType>( [&]( Net::IConnection* pConn, Message::MessageData* &pMsgData, Svr::Transaction* &pNewTrans)->HRESULT 

	#define BR_TRANS_MESSAGE(MessageType,MessageHandlerImpl) \
		RegisterMessageHandler<MessageType>( [&](Svr::TransactionResult* pRes)->HRESULT MessageHandlerImpl );


	typedef std::function<HRESULT(Net::IConnection *, Message::MessageData* &, Transaction* &)>	EntityMessageHandlerItem;




}; // namespace Svr
}; // namespace BR





