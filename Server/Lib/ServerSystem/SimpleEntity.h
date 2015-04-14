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


#include "Common/Typedefs.h"
#include "Common/Thread.h"
#include "Net/NetDef.h"
#include "ServerSystem/Task.h"
#include "Common/PageQueue.h"
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
		virtual HRESULT ClearEntity();

		virtual HRESULT FindActiveTransaction(const TransactionID& transID, Transaction* &pTransaction) override;
		

		// Run the task
		virtual HRESULT TickUpdate(Svr::TimerAction *pAction = nullptr);


		virtual HRESULT ProcessTransactionResult(Transaction *pCurTran, TransactionResult *pTransRes) override;

		virtual UINT GetActiveTransactionCount();
	};

#include "SimpleEntity.inl"


}; // namespace Svr
}; // namespace BR




