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


#include "Common/Typedefs.h"
#include "Common/Thread.h"
#include "Common/ObjectPool.h"
#include "Common/BrMemory.h"
#include "Net/NetDef.h"
#include "ServerSystem/SimpleEntity.h"
#include "ServerSystem/Transaction.h"
#include "ServerSystem/SvrTypes.h"
#include "ServerSystem/ServerComponent.h"
#include "Common/MemoryBufferUtil.h"

#include "curl/curl.h"
#include "json/json.h"

#include "ServerSystem/Google/GoogleOAuth.h"

namespace BR {

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
		ParallelTransaction(TransactionID parentTransID, Message::MessageID MsgID)
			:SubTransactionWitResult( parentTransID, MsgID )
		{}
	};

	class ParallelTransactionNop : public ParallelTransaction
	{
	public:
		ParallelTransactionNop()
			:ParallelTransaction(0, 0)
		{}

	};


	

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	External task class
	//

	class ParallelTransactionManager : protected SimpleEntity, public IServerComponent
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

		// Initialize server component
		virtual Result InitializeComponent();

		// Terminate server component
		virtual void TerminateComponent();

		//virtual Result OnRoutedMessage(Message::MessageData* &pMsg) override { assert(false); return ResultCode::NOT_IMPLEMENTED; }
	};




}; // namespace Svr
}; // namespace BR




