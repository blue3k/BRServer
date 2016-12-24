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
#include "Common/BrSvrTypes.h"
#include "ServerSystem/Entity.h"


namespace BR {
namespace Svr {


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Master Entity class with transaction manager
	//		: Entity with high transaction load, multiple transaction opening is allowed
	//

	class MasterEntity : public Entity
	{
	public:

		typedef DualSortedMap<UINT, SharedPointerT<Transaction>>			TransactionList;


	private:
		// maximum active transaction count
		UINT							m_uiMaxActiveTransaction;

		// Transaction manager
		TimeScheduler					m_activeTransactionScheduler;
		TransactionList					m_activeTrans;
		SharedPointerT<Transaction>		m_pExclusiveTransaction;


	public:
		MasterEntity( UINT uiTransQueueSize = 2048, UINT TransResQueueSize = 2048 );
		virtual ~MasterEntity();

		virtual void ReleaseTransaction(Transaction* pTrans) override;

		void ValidateTransactionCount();

		// change maximum active transaction
		void SetMaxActiveTransaction( UINT uiMaxActiveTransaction );

		// clear transaction
		virtual Result ClearEntity() override;

		virtual Result FindActiveTransaction(const TransactionID& transID, Transaction* &pTransaction) override;

		// Update entity process
		// Run the task
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;

		virtual Result ProcessTransactionResult(Transaction *pCurTran, TransactionResult *pTransRes) override;

		virtual UINT GetActiveTransactionCount() override;

		void UpdateWorkingThreadID(ThreadID threadID);


		//virtual Result OnRoutedMessage(Message::MessageData* &pMsg) override { assert(false); return ResultCode::NOT_IMPLEMENTED; }

		/////////////////////////////////////////////////////////////////////////////////////
		// Event task handling

		virtual void OnAddedToTaskManager(TaskWorker *pWorker) override;

		virtual Result OnEventTask(const EventTask& eventTask) override;

	};



#include "MasterEntity.inl"



}; // namespace Svr

	extern template class SharedPointerT<Svr::MasterEntity>;

}; // namespace BR




