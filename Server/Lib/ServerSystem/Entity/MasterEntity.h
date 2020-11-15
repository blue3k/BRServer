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
#include "Types/BrSvrTypes.h"
#include "Entity/Entity.h"


namespace SF {
	namespace Svr {


		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Master Entity class with transaction manager
		//		: Entity with high transaction load, multiple transaction opening is allowed
		//

		class MasterEntity : public Entity
		{
		public:

			typedef DualSortedMap<uint, SharedPointerT<Transaction>>			TransactionList;


		private:
			// maximum active transaction count
			uint							m_uiMaxActiveTransaction;

			// Transaction manager
			TimerScheduler					m_activeTransactionScheduler;
			TransactionList					m_activeTrans;
			SharedPointerT<Transaction>		m_pExclusiveTransaction;


		public:
			MasterEntity(uint uiTransQueueSize = 2048, uint TransResQueueSize = 2048);
			virtual ~MasterEntity();

			virtual void ReleaseTransaction(TransactionPtr& pTrans) override;

			void ValidateTransactionCount();

			// change maximum active transaction
			void SetMaxActiveTransaction(uint uiMaxActiveTransaction);

			// clear transaction
			virtual Result ClearEntity() override;

			virtual Result FindActiveTransaction(const TransactionID& transID, TransactionPtr& pTransaction) override;

			// Update entity process
			// Run the task
			virtual Result TickUpdate(TimerAction* pAction = nullptr) override;

			virtual Result ProcessTransactionResult(TransactionPtr& pCurTran, UniquePtr<TransactionResult>& pTransRes) override;

			virtual uint GetActiveTransactionCount() override;

			void UpdateWorkingThreadID(ThreadID threadID);


			//virtual Result OnRoutedMessage(MessageDataPtr &pMsg) override { assert(false); return ResultCode::NOT_IMPLEMENTED; }

			/////////////////////////////////////////////////////////////////////////////////////
			// Event task handling

			virtual void OnAddedToTaskManager(TaskWorker* pWorker) override;

			virtual Result OnEventTask(ServerTaskEvent& eventTask) override;

		};

	} // namespace Svr

	extern template class SharedPointerT<Svr::MasterEntity>;

}; // namespace SF




