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

#include "SFTypedefs.h"
#include "Task/TimerScheduler.h"
#include "Entity/Entity.h"
#include "Transaction/Transaction.h"
#include "Memory/MemoryPool.h"

namespace SF {
namespace Svr {

	class Entity;
	class Transaction;

	class TimerActionTransaction : public TimerAction
	{
	private:

		WeakPointerT<Transaction> m_pTrans;

	public:
		TimerActionTransaction(SharedPointerT<Transaction>& pTrans);
		~TimerActionTransaction();

		void SetNextTickTime(SharedPointerT<Transaction>& pTrans);

		virtual bool OnTimerTick() override;

		virtual const char* GetDebugString() override;
	};


}; // namespace Svr
}; // namespace SF
