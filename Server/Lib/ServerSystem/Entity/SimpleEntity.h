////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
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
#include "Entity/Entity.h"


namespace SF {
namespace Svr {


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Simple Entity class
	//		: Entity with low transaction load, multiple transaction open not allowed
	//

	class SimpleEntity : public Entity
	{
	private:
		SharedPointerT<Transaction> m_pCurTran;

	public:
		SimpleEntity( uint uiTransQueueSize = 16, uint TransResQueueSize = 16 );
		virtual ~SimpleEntity();

		// clear transaction
		virtual Result ClearEntity() override;

		virtual Result FindActiveTransaction(const TransactionID& transID, TransactionPtr &pTransaction) override;
		

		// Run the task
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;


		virtual Result ProcessTransactionResult(TransactionPtr &pCurTran, TransactionResult* &pTransRes) override;

		virtual uint GetActiveTransactionCount() override;
	};



}; // namespace Svr
}; // namespace SF




