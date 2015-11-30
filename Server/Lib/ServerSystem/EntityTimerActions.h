////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server TickTask manager
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "ServerSystem/TimeScheduler.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/Transaction.h"
#include "Common/MemoryPool.h"

namespace BR {
namespace Svr {

	class Entity;
	class Transaction;

	class TimerActionTransaction : public TimerAction, public MemoryPoolObject<TimerActionTransaction>
	{
	private:

		WeakPointerT<Transaction> m_pTrans;

	public:
		TimerActionTransaction(SharedPointerT<Transaction>& pTrans);
		~TimerActionTransaction();

		void SetNextTickTime(SharedPointerT<Transaction>& pTrans);

		virtual bool UpdateTick() override;

		virtual const char* GetDebugString() override;
	};


}; // namespace Svr
}; // namespace BR

