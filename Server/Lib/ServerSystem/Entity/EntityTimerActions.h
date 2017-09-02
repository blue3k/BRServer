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
#include "ServerSystem/Entity.h"
#include "ServerSystem/Transaction.h"
#include "Memory/MemoryPool.h"

namespace SF {
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

		virtual bool OnTimerTick() override;

		virtual const char* GetDebugString() override;
	};


}; // namespace Svr
}; // namespace SF

