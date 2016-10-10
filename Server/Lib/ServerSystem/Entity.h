////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Entity class definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Common/Typedefs.h"
#include "Common/Thread.h"
#include "Net/NetDef.h"
#include "ServerSystem/Task.h"
#include "Common/PageQueue.h"
#include "Common/BrSvrTypes.h"
#include "ServerSystem/SvrTypes.h"
#include "Common/SharedPointer.h"
#include "ServerSystem/Transaction.h"


namespace BR {

	namespace Message {
		class MessageData;
	};

	class MemoryAllocator;

	namespace Svr {
		template<class MessageHandlerType>
		class MessageHandlerTable;
	};

	namespace Net {
		class IConnection;
		class Connection;
	};


namespace Svr{

	class Transaction;
	class TransactionResult;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity base class
	//

	class Entity : public TickTask
	{
	public:
		typedef std::function<Result(Net::IConnection *, Message::MessageData* &, Transaction* &)>	MessageHandlerType;

	private:
		// Entity state
		EntityState		m_State;

		// Entity UID, which is assigned by Entity Server
		EntityUID		m_EntityUID;

		// Entity Create Time
		TimeStampMS			m_ulCreateTime;

		// Latestly processed transaction index
		mutable std::atomic<ULONG>	m_lTransIdx;


		// Entity transaction queue
		PageQueue<Transaction*>			m_transactionQueue;

		StaticAllocator< sizeof(Svr::EntityMessageHandlerItem) * 30 > m_Allocator;

		// Message handler table
		MessageHandlerTable<MessageHandlerType>	m_HandlerTable;


	protected:


		// Get transaction queue
		inline PageQueue<Transaction*>& GetTransactionQueue();

		TimeStampMS SetBestScehdulingTime(Transaction* pTrans);

		virtual void ReleaseTransaction(Transaction* pTrans);

		Result _ClearEntity();

	public:
		Entity( UINT uiTransQueueSize, UINT TransResQueueSize );
		virtual ~Entity();

		// Get Entity State
		inline EntityState GetEntityState();

		// Set Entity State
		inline void SetEntityState( EntityState state );


		// Get Entity UID
		inline EntityUID GetEntityUID() const;

		// Set Entity UID
		inline void SetEntityUID( EntityUID entityUID );


		// Get Entity ID
		inline EntityID GetEntityID() const;

		// Set/Get Entity Create TIme
		inline TimeStampMS GetEntityCreateTime();
		inline void SetEntityCreateTime(TimeStampMS ulCreateTime);

		// Get Entity transaction queue count
		virtual CounterType GetPendingTransactionCount() const;

		// Generate new transaction index
		inline ULONG GenTransIndex();


		// Initialize entity to proceed new connection
		virtual Result InitializeEntity( EntityID newEntityID );

		// clear entity resources such as transactions
		virtual Result ClearEntity();

		// Clear resources and change entity state to closed
		virtual Result TerminateEntity();


		//////////////////////////////////////////////////////////////////////////
		//
		//	Message handling
		//


		// register message handlers
		virtual Result RegisterMessageHandlers();

		// Register message handler helper
		template< class MessageClassType >
		Result RegisterMessageHandler(const char* fileName, UINT lineNumber, MessageHandlerType newHandler )
		{
			//AssertRel(m_pHandlerTable);
			return m_HandlerTable.Register<MessageClassType>(fileName, lineNumber, newHandler );
		}

		Svr::MessageHandlerTable<MessageHandlerType>* GetMessageHandlerTable() { return &m_HandlerTable; }


		// Process Message and release message after all processed
		virtual Result ProcessMessage(ServerEntity* pServerEntity, Net::IConnection *pCon, Message::MessageData* &pMsg);

		// Process result
		virtual Result ProcessMessageResult( Message::MessageData* &pMsg );


		//////////////////////////////////////////////////////////////////////////
		//
		//	Transaction handling
		//

		virtual Result FindActiveTransaction(const TransactionID& transID, Transaction* &pTransaction) = 0;

		virtual UINT GetActiveTransactionCount() = 0;

		// Pending new transaction job
		virtual Result PendingTransaction(ThreadID thisThreadID, Transaction* &pTrans);
		virtual Result ProcessTransaction(Transaction* &pTrans);

		// Pending transaction result
		virtual Result PendingTransactionResult( TransactionResult* &pTransRes );

		virtual Result ProcessTransactionResult(Transaction *pCurTran, TransactionResult *pTransRes) = 0;


	};





#include "Entity.inl"



}; // namespace Svr

extern template class SharedPointerT<Svr::Entity>;
extern template class WeakPointerT<Svr::Entity>;

}; // namespace BR




