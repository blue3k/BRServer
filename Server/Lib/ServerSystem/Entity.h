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
		typedef std::function<HRESULT(Net::IConnection *, Message::MessageData* &, Transaction* &)>	MessageHandlerType;

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


	protected:

		// Message handler table
		MessageHandlerTable<MessageHandlerType>*	m_pHandlerTable;

		// Get transaction queue
		inline PageQueue<Transaction*>& GetTransactionQueue();

		TimeStampMS SetBestScehdulingTime(Transaction* pTrans);

		virtual void ReleaseTransaction(Transaction* pTrans);

		HRESULT _ClearEntity();

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
		virtual HRESULT InitializeEntity( EntityID newEntityID );

		// clear transaction
		virtual HRESULT ClearEntity();

		// Close entity and clear transaction
		virtual HRESULT TerminateEntity();



		//virtual ULONG GetNextTickTimeout() override;


		//////////////////////////////////////////////////////////////////////////
		//
		//	Message handling
		//

		// Get allocator, This will be used for initializing message handler table
		virtual MemoryAllocator& GetAllocator() = 0;

		// register message handlers
		virtual HRESULT RegisterMessageHandlers();

		// Register message handler helper
		template< class MessageClassType >
		HRESULT RegisterMessageHandler( MessageHandlerType newHandler )
		{
			AssertRel(m_pHandlerTable);
			return m_pHandlerTable->Register<MessageClassType>( newHandler );
		}

		Svr::MessageHandlerTable<MessageHandlerType>* GetMessageHandlerTable() { return m_pHandlerTable; }

		// Process result
		virtual HRESULT ProcessMessageResult( Message::MessageData* &pMsg );


		//////////////////////////////////////////////////////////////////////////
		//
		//	Transaction handling
		//

		virtual HRESULT FindActiveTransaction(const TransactionID& transID, Transaction* &pTransaction) = 0;

		virtual UINT GetActiveTransactionCount() = 0;

		// Pending new transaction job
		virtual HRESULT PendingTransaction(ThreadID thisThreadID, Transaction* &pTrans);
		virtual HRESULT ProcessTransaction(Transaction* &pTrans);

		// Pending transaction result
		virtual HRESULT PendingTransactionResult( TransactionResult* &pTransRes );

		virtual HRESULT ProcessTransactionResult(Transaction *pCurTran, TransactionResult *pTransRes) = 0;


	};





#include "Entity.inl"



}; // namespace Svr

extern template class SharedPointerT<Svr::Entity>;
extern template class WeakPointerT<Svr::Entity>;

}; // namespace BR




