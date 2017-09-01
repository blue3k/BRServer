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
#include "Thread/Thread.h"
#include "Net/NetDef.h"
#include "Common/Task/Task.h"
#include "Container/PageQueue.h"
#include "ServerSystem/SvrTypes.h"
#include "ServerSystem/Entity.h"


namespace BR {
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
		SimpleEntity( UINT uiTransQueueSize = 16, UINT TransResQueueSize = 16 );
		virtual ~SimpleEntity();

		// clear transaction
		virtual Result ClearEntity() override;

		virtual Result FindActiveTransaction(const TransactionID& transID, Transaction* &pTransaction) override;
		

		// Run the task
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;


		virtual Result ProcessTransactionResult(Transaction *pCurTran, TransactionResult *pTransRes) override;

		virtual UINT GetActiveTransactionCount() override;
	};

#include "SimpleEntity.inl"


}; // namespace Svr
}; // namespace BR




