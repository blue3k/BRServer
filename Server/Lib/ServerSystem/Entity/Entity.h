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
#include "Multithread/SFThread.h"
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
}

namespace SF {

	namespace Svr {
		template<class MessageHandlerType>
		class MessageHandlerTable;
	};

	class MessageEndpoint;


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
		typedef std::function<Result(const MessageDataPtr &, TransactionPtr &)>	MessageHandlerType;

	private:

		// Entity state
		EntityState		m_State{};

		// Entity UID, which is assigned by Entity Server
		EntityUID		m_EntityUID{};

		// Entity Create Time
		TimeStampMS			m_ulCreateTime{};

		// Lastly processed transaction index
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
		inline void SetEntityState(EntityState state) {
			m_State = state;
		}


		// Get Entity UID
		EntityUID GetEntityUID() const { return m_EntityUID; }

		// Set Entity UID
		void SetEntityUID(EntityUID entityUID, bool silenceAssert = false);

		// Clear entity UID before destroy
		void ClearEntityUID() {
			m_EntityUID = EntityUID(0);
		}


		// Get Entity ID
		inline EntityID GetEntityID() const {
			return EntityID((uint32_t)GetTaskID());
		}

		// Set/Get Entity Create TIme
		inline TimeStampMS GetEntityCreateTime() { return m_ulCreateTime; }
		inline void SetEntityCreateTime(TimeStampMS ulCreateTime) {
			m_ulCreateTime = ulCreateTime;
		}

		// Get Entity transaction queue count
		virtual CounterType GetPendingTransactionCount() const;

		// Generate new transaction index
		inline uint64_t GenTransIndex()
		{
			auto result = m_lTransIdx.fetch_add(1, std::memory_order_relaxed);
			// 0 is reserved to none
			if (result == 0) result = m_lTransIdx.fetch_add(1, std::memory_order_relaxed);

			return result;
		}



		// Initialize entity to proceed 
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
				[this](const MessageDataPtr& pMsg, TransactionPtr& pNewTrans)->Result
				{
					pNewTrans = new(GetHeap()) TransactionType(GetHeap(), pMsg);
					if (pNewTrans == nullptr)
						return ResultCode::OUT_OF_MEMORY;
					return ResultCode::SUCCESS;
				}
			);
		}

		Svr::MessageHandlerTable<MessageHandlerType>& GetMessageHandlerTable() { return m_HandlerTable; }


		// Process Message and release message after all processed
		virtual Result ProcessMessage(const SharedPointerT<MessageEndpoint>& remoteEndpoint, const MessageDataPtr &pMsg);

		// Process result
		virtual Result ProcessMessageResult(const MessageDataPtr &pMsg );


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
		virtual Result PendingTransactionResult(SFUniquePtr<TransactionResult>& pTransRes);

		virtual Result ProcessTransactionResult(TransactionPtr &pCurTran, SFUniquePtr<TransactionResult>& pTransRes) = 0;


	};


} // namespace Svr

//extern template class SharedPointerT<Svr::Entity>;
extern template class WeakPointerT<Svr::Entity>;


} // namespace SF




