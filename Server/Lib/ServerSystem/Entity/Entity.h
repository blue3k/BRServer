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


#include "SFTypedefs.h"
#include "Thread/Thread.h"
#include "Net/NetDef.h"
#include "Task/ServerTask.h"
#include "Container/PageQueue.h"
#include "Types/BrSvrTypes.h"
#include "Types/BrSvrTypes.h"
#include "Object/SharedPointer.h"
#include "Transaction/Transaction.h"


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
		template<class MessageHandlerType>
		class MessageHandlerTable;
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
		typedef std::function<Result(Net::Connection *, MessageDataPtr &, TransactionPtr &)>	MessageHandlerType;

	private:

		MemoryManager m_MemoryManager;


		// Entity state
		EntityState		m_State;

		// Entity UID, which is assigned by Entity Server
		EntityUID		m_EntityUID;

		// Entity Create Time
		TimeStampMS			m_ulCreateTime;

		// Latestly processed transaction index
		mutable std::atomic<ULONG>	m_lTransIdx;


		// Entity transaction queue
		PageQueue<TransactionPtr>			m_transactionQueue;

		// Message handler table
		MessageHandlerTable<MessageHandlerType>	m_HandlerTable;



	protected:


		// Get transaction queue
		PageQueue<TransactionPtr>& GetTransactionQueue() { return m_transactionQueue; }

		TimeStampMS SetBestScehdulingTime(Transaction* pTrans);

		virtual void ReleaseTransaction(TransactionPtr& pTrans);

		Result _ClearEntity();

	public:
		Entity( uint uiTransQueueSize, uint TransResQueueSize );
		virtual ~Entity();

		IMemoryManager& GetMemoryManager() { return m_MemoryManager; }

		// Get Entity State
		EntityState GetEntityState() { return m_State; }

		// Set Entity State
		inline void SetEntityState( EntityState state );


		// Get Entity UID
		EntityUID GetEntityUID() const { return m_EntityUID; }

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
		Result RegisterMessageHandler(const char* fileName, uint lineNumber, MessageHandlerType newHandler )
		{
			//AssertRel(m_pHandlerTable);
			return m_HandlerTable.Register<MessageClassType>(fileName, lineNumber, newHandler );
		}

		Svr::MessageHandlerTable<MessageHandlerType>* GetMessageHandlerTable() { return &m_HandlerTable; }


		// Process Message and release message after all processed
		virtual Result ProcessMessage(ServerEntity* pServerEntity, Net::Connection *pCon, MessageDataPtr &pMsg);

		// Process result
		virtual Result ProcessMessageResult(MessageDataPtr &pMsg );


		//////////////////////////////////////////////////////////////////////////
		//
		//	Transaction handling
		//

		virtual Result FindActiveTransaction(const TransactionID& transID, Transaction* &pTransaction) = 0;

		virtual uint GetActiveTransactionCount() = 0;

		// Pending new transaction job
		virtual Result PendingTransaction(ThreadID thisThreadID, TransactionPtr &pTrans);
		virtual Result ProcessTransaction(TransactionPtr &pTrans);

		// Pending transaction result
		virtual Result PendingTransactionResult( TransactionResult* &pTransRes );

		virtual Result ProcessTransactionResult(Transaction *pCurTran, TransactionResult* &pTransRes) = 0;


	};





#include "Entity.inl"



}; // namespace Svr

extern template class SharedPointerT<Svr::Entity>;
extern template class WeakPointerT<Svr::Entity>;

template<> inline SharedPointerT<Svr::Entity> DefaultValue<SharedPointerT<Svr::Entity>>() { return SharedPointerT<Svr::Entity>(); }

}; // namespace SF




