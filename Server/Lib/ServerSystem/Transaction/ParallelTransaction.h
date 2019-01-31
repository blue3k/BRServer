////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : KyungKun Ko
//
// Description : Parallel class definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"
#include "Thread/SFThread.h"
#include "Object/SFObjectPool.h"
#include "Memory/SFMemory.h"
#include "Net/SFNetDef.h"
#include "Entity/SimpleEntity.h"
#include "Transaction/Transaction.h"
#include "Types/BrSvrTypes.h"
#include "Component/ServerComponent.h"


#include "curl/curl.h"
#include "json/json.h"

#include "ServerSystem/Google/GoogleOAuth.h"

namespace SF {

	namespace Message {
		class MessageData;
	};


namespace Svr{


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	External task base class
	//

	class ParallelTransaction : public SubTransactionWitResult
	{
	public:
		ParallelTransaction(IHeap& memoryManager, TransactionID parentTransID, Message::MessageID MsgID)
			:SubTransactionWitResult(memoryManager, parentTransID, MsgID )
		{}
	};

	class ParallelTransactionNop : public ParallelTransaction
	{
	public:
		ParallelTransactionNop(IHeap& memoryManager)
			:ParallelTransaction(memoryManager, TransactionID(), 0)
		{}

	};


	

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	External task class
	//

	class ParallelTransactionManager : public SimpleEntity
	{
	public:

		enum {
			ComponentID = ServerComponentID_ParallelTransactionManager,
		};

		class TaskWorker : public Thread
		{
		private:
			PageQueue<ParallelTransaction*> &m_QuerieQueue;
			std::atomic<bool> m_bExitRequested;

		public:
			TaskWorker(PageQueue<ParallelTransaction*> &querieQueue);

			void SetExitRequested() { m_bExitRequested.store(true, std::memory_order_release); }

			virtual void Run() override;
		};

	private:
		// Working thread list
		typedef std::vector<TaskWorker*>	WorkerList;

		// Query Worker class
		WorkerList							m_QueryWorker;

		// Pending transaction Queue
		PageQueue<ParallelTransaction*>		m_PendingQueries;

	protected:

		Result EnqueueTransaction(ParallelTransaction* pTrans);

	public:
		ParallelTransactionManager();
		~ParallelTransactionManager();

		IHeap& GetHeap() { return GetSystemHeap(); }

		// Initialize server component
		virtual Result InitializeComponent();

		// Terminate server component
		virtual void TerminateComponent();

		//virtual Result OnRoutedMessage(Message::MessageData* &pMsg) override { assert(false); return ResultCode::NOT_IMPLEMENTED; }
	};




}; // namespace Svr
}; // namespace SF




