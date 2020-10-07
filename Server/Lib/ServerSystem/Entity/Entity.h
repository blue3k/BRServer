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
#include "Thread/SFThread.h"
#include "Net/SFNetDef.h"
#include "Task/ServerTask.h"
#include "Container/SFPageQueue.h"
#include "Types/BrSvrTypes.h"
#include "Types/BrSvrTypes.h"
#include "Object/SFSharedPointer.h"
#include "Transaction/MessageHandlerTable.h"


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

	class ServerEntity;
	class Transaction;
	class TransactionResult;

	using MessageDataPtr = SharedPointerT<Message::MessageData>;
	using TransactionPtr = SharedPointerT<Transaction>;



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity base class
	//

	class Entity : public TickTask
	{
	public:
		typedef std::function<Result(Net::Connection *, MessageDataPtr &, TransactionPtr &)>	MessageHandlerType;

	private:



		// Entity state
		EntityState		m_State;

		// Entity UID, which is assigned by Entity Server
		EntityUID		m_EntityUID;

		// Entity Create Time
		TimeStampMS			m_ulCreateTime;

		// Latestly processed transaction index
		mutable std::atomic<uint64_t>	m_lTransIdx;


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

		virtual void Dispose() override;

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
		inline uint64_t GenTransIndex();


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
		Result RegisterMessageHandler(MessageHandlerType&& newHandler )
		{
			//AssertRel(m_pHandlerTable);
			return m_HandlerTable.Register<MessageClassType>(std::forward<MessageHandlerType>(newHandler) );
		}


		template<class TransactionType>
		Result RegisterMessageHandler()
		{
			return m_HandlerTable.Register<typename TransactionType::MessageClassType>(
				[this](Net::Connection* pConnection, MessageDataPtr& pMsg, TransactionPtr& pNewTrans)->Result
				{
					svrMemReturn(pNewTrans = new(GetHeap()) TransactionType(GetHeap(), pMsg));
					return ResultCode::SUCCESS;
				}
			);
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

		virtual Result FindActiveTransaction(const TransactionID& transID, TransactionPtr &pTransaction) = 0;

		virtual uint GetActiveTransactionCount() = 0;

		// Pending new transaction job
		virtual Result PendingTransaction(ThreadID thisThreadID, TransactionPtr &pTrans);
		virtual Result ProcessTransaction(TransactionPtr &pTrans);

		// Pending transaction result
		virtual Result PendingTransactionResult(UniquePtr<TransactionResult>& pTransRes);

		virtual Result ProcessTransactionResult(TransactionPtr &pCurTran, UniquePtr<TransactionResult>& pTransRes) = 0;


	};





#include "Entity.inl"



}; // namespace Svr

//extern template class SharedPointerT<Svr::Entity>;
extern template class WeakPointerT<Svr::Entity>;


}; // namespace SF




