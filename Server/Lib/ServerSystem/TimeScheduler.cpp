////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server TickTask manager
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Common/Thread.h"
#include "Common/StrUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/TimeScheduler.h"



namespace BR {

	template class SharedPointerT < Svr::TimerAction >;

namespace Svr {
	
	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	
	//


	TimerAction::TimerAction()
		//: m_InQueueKey(0)
	{
		m_InQueueKey.TimerKey = -1;
		TimeData.TimerKey = -1;
	}

	TimerAction::~TimerAction()
	{
	}

	void TimerAction::SetNextTickTime(TimeStampMS nextTickTime)
	{
		auto curTime = Util::Time.GetTimeMs();
		Assert(nextTickTime != TimeStampMS::min());
		if (nextTickTime < curTime || nextTickTime == TimeStampMS::min()) // correct the time
			nextTickTime = curTime;
		auto diffTime = nextTickTime - curTime;
		Assert((INT)diffTime.count() < 60*60*1000);
		
		TimeData.Components.NextTickTime = nextTickTime;
	}



	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	
	//

	TimeScheduler::TimeScheduler()
		: m_FailSafeTimerTickInterval(500)
		, m_AssertOnInvalidTickTime(false)
		//, m_WorkingThreadID(0)
		, m_IsWriteLocked(0)
	{
	}

	bool TimeScheduler::TimerTickActionGetNextTick(const UINT64& keyVal, const SharedPointerT<TimerAction>& pAction)
	{
		m_GetNextTickResult = pAction->GetScheduledTime();
		return false;
	}

	bool TimeScheduler::TimerTickActionUpdate(const UINT64& keyVal, const SharedPointerT<TimerAction>& pAction)
	{
		TimeStampMS currentTime = Util::Time.GetTimeMs();
		TimerAction::TimeKey key;
		key.TimerKey = keyVal;

		auto diff = (key.Components.NextTickTime - currentTime);
		if ((INT)diff.count() > 0)
			return false;

		SharedPointerT<TimerAction> removed;
		HRESULT hr = m_TimerMap.Remove(key.TimerKey, removed);
		if (FAILED(hr))
		{
			hr = m_TimerMap.Remove(pAction->GetInQueueKey().TimerKey, removed);
			Assert(SUCCEEDED(hr));
		}
		Assert(removed == pAction);
		// removed clear in-queue time
		pAction->m_InQueueKey.Components.NextTickTime = TimeStampMS::max();

		//auto savedTime = key;
		auto diffTime = pAction->TimeData.Components.NextTickTime - Util::Time.GetTimeMs();
		Assert((INT)diffTime.count() < 5000);

		bool bIsNeedToKeep = pAction->UpdateTick();

		// if it doesn't need to be scheduled anymore
		auto nextDiff = (INT)(pAction->TimeData.Components.NextTickTime - Util::Time.GetTimeMs()).count();
		if (nextDiff < 0)
		{
			if (key.Components.NextTickTime == pAction->TimeData.Components.NextTickTime)
			{
				if (bIsNeedToKeep)
				{
					Assert(!m_AssertOnInvalidTickTime || bIsNeedToKeep);
					svrTrace(Trace::TRC_ERROR, "Same Timer value:{1} of {0} correct to fail safe timer value:{2}", pAction->GetDebugString(), pAction->TimeData.Components.NextTickTime, m_FailSafeTimerTickInterval);
					pAction->TimeData.Components.NextTickTime = Util::Time.GetTimeMs() + m_FailSafeTimerTickInterval;
				}
			}
			else if (pAction->TimeData.Components.NextTickTime == TimeStampMS::max() || !bIsNeedToKeep)
			{
				if (!bIsNeedToKeep)
				{
					//svrTrace(Trace::TRC_DBG1, "Remove tick timer value:-1, {0}", typeid(*(const TimerAction*)pAction).name());
					return true;
				}
			}
			else
			{
				// Too short period, but negative value is fine
				//if (bIsNeedToKeep)
				//{
				//	svrTrace(Trace::TRC_WARN, "Negative tick timer value:{0}, {1}", nextDiff, typeid(*(const TimerAction*)pAction).name());
				//}
				//Assert(!m_AssertOnInvalidTickTime);
				//pAction->TimeData.NextTickTime = Util::Time.GetTimeMs() + m_FailSafeTimerTickInterval;
				//svrTrace(Trace::TRC_ERROR, "Invalid Timer value, correct to fail safe timer value");
			}
		}
		else
		{
			//Assert(nextDiff < 10000);
		}

		if (bIsNeedToKeep)
		{
			//MutexScopeLock localLock(m_WriteLock);
			Assert(pAction->TimeData.TimerKey != 0);
			Assert(pAction->TimeData.Components.NextTickTime != TimeStampMS::max());
			pAction->m_InQueueKey.TimerKey = pAction->TimeData.TimerKey;
			if (FAILED(m_TimerMap.Insert(pAction->m_InQueueKey.TimerKey, pAction)))
			{
				Assert(false);
			}
		}
		else
		{
			//svrTrace(Trace::TRC_DBG1, "Remove tick timer {0}", typeid(*(const TimerAction*)pAction).name());
		}

		ValidateTimerKeys();

		return true;

	}

	void TimeScheduler::UpdateWorkingThreadID(ThreadID threadID)
	{
		//Assert(threadID != 0);
		m_WorkingThreadID = threadID;
	}

	HRESULT TimeScheduler::AddTimerAction(ThreadID threadID, TimerAction* pAction)
	{
		HRESULT hr = S_OK;


		Assert(m_WorkingThreadID == threadID);
		if (m_WorkingThreadID != threadID)
		{
			svrErr(E_SVR_INVALID_THREAD);
		}

		//Assert(m_IsWriteLocked.load(std::memory_order_relaxed) == 0);

		svrChkPtr(pAction);
		svrAssert(pAction->m_InQueueKey.Components.NextTickTime == TimeStampMS::max());
		svrAssert(pAction->TimeData.Components.NextTickTime != TimeStampMS::max());
		svrAssert(pAction->TimeData.TimerKey != 0);


		pAction->m_InQueueKey.TimerKey = pAction->TimeData.TimerKey;
		if (FAILED(m_TimerMap.Insert(pAction->m_InQueueKey.TimerKey, pAction)))
		{
			Assert(false);
		}

//#ifdef DEBUG
//		UINT found = 0;
//		m_TimerMap.ForeachOrderWrite(0, (UINT)m_TimerMap.GetWriteItemCount(), [&](const UINT64& keyVal, SharedPointerT<TimerAction> pInAction) -> bool
//		{
//			if (pInAction == pAction) found++;
//			return true;
//		});
//		Assert(found == 1);
//#endif
		//ValidateTimerKeys();

	Proc_End:

		return hr;
	}

	HRESULT TimeScheduler::RemoveTimerAction(ThreadID threadID, TimerAction* pAction)
	{
		HRESULT hr = S_OK;
		SharedPointerT<TimerAction> removed;

		if (pAction == nullptr)
			return hr;

		if (pAction->m_InQueueKey.Components.NextTickTime == TimeStampMS::max()) // if not schedule
			return hr;

		Assert(m_WorkingThreadID == threadID);
		if (m_WorkingThreadID != threadID)
		{
			svrErr(E_SVR_INVALID_THREAD);
		}

		svrChkPtr(pAction);

		if (FAILED(m_TimerMap.Remove(pAction->m_InQueueKey.TimerKey, removed)))
		{
			Assert(pAction->m_InQueueKey.Components.NextTickTime == TimeStampMS::max());
		}

		Assert(removed == pAction);
		pAction->m_InQueueKey.Components.NextTickTime = TimeStampMS::max();

//#ifdef DEBUG
//		m_TimerMap.ForeachOrderWrite(0, (UINT)m_TimerMap.GetWriteItemCount(), [&](const UINT64& keyVal, SharedPointerT<TimerAction> pInAction) -> bool
//		{
//			Assert(pInAction != pAction);
//			return true;
//		});
//#endif
		//ValidateTimerKeys();

	Proc_End:

		return hr;
	}

	HRESULT TimeScheduler::CommitChanges(ThreadID threadID)
	{
		//Assert(m_IsWriteLocked.load(std::memory_order_relaxed) == 0);

		Assert(m_WorkingThreadID == threadID);
		if (m_WorkingThreadID != threadID)
		{
			return E_SVR_INVALID_THREAD;
		}
		return m_TimerMap.CommitChanges();
	}

	HRESULT TimeScheduler::Reschedul(ThreadID threadID, TimerAction* pAction)
	{
		HRESULT hr = S_OK;

		if (pAction == nullptr)
			return S_OK;

		//Assert(m_IsWriteLocked.load(std::memory_order_relaxed) == 0);

		Assert(m_WorkingThreadID == threadID);
		if (m_WorkingThreadID != threadID)
		{
			svrErr(E_SVR_INVALID_THREAD);
		}

		if (pAction->TimeData.Components.NextTickTime == pAction->m_InQueueKey.Components.NextTickTime)
			return hr;

		if (pAction->m_InQueueKey.Components.NextTickTime != TimeStampMS::max())
		{
			SharedPointerT<TimerAction> removed;
			if (FAILED(m_TimerMap.Remove(pAction->m_InQueueKey.TimerKey, removed)))
			{
				Assert(false);
			}
			Assert(removed == pAction);
			pAction->m_InQueueKey.Components.NextTickTime = TimeStampMS::max();
		}

		Assert(!m_AssertOnInvalidTickTime || ( (LONG)(pAction->TimeData.Components.NextTickTime - Util::Time.GetTimeMs()).count() < (2*60*1000)));
		Assert(pAction->TimeData.Components.NextTickTime != TimeStampMS::max());
		if (pAction->TimeData.Components.NextTickTime != TimeStampMS::max())
		{
			pAction->m_InQueueKey.TimerKey = pAction->TimeData.TimerKey;
			if (FAILED(m_TimerMap.Insert(pAction->m_InQueueKey.TimerKey, pAction)))
			{
				Assert(false);
			}
		}

		//ValidateTimerKeys();

	Proc_End:

		return hr;
	}

	TimeStampMS TimeScheduler::GetNextTimeTick()
	{
		m_GetNextTickResult = TimeStampMS::max();
		m_TimerMap.ForeachReverseOrder(0, 1, [&](const UINT64& keyVal, const SharedPointerT<TimerAction>& pAction) { return TimerTickActionGetNextTick(keyVal, pAction); });

		return m_GetNextTickResult;
	}

	void TimeScheduler::ValidateTimerKeys()
	{
#ifdef DEBUG
		//MutexScopeLock localLock(m_WriteLock);
		//m_TimerMap.ForeachOrderWrite(0, (UINT)m_TimerMap.GetWriteItemCount(), [&](const UINT64& keyVal, SharedPointerT<TimerAction> pAction) -> bool
		//{
		//	Assert(pAction->m_InQueueKey.NextTickTime != TimeStampMS::max());
		//	AssertRel(keyVal == pAction->m_InQueueKey.TimerKey);
		//	return true;
		//});
#endif
	}

	void TimeScheduler::UpdateTick(ThreadID threadID)
	{
		CommitChanges(threadID);

		//m_IsWriteLocked.fetch_add(1, std::memory_order_acquire);

		m_TimerMap.ForeachReverseOrder(0, (UINT)m_TimerMap.GetItemCount(), [&](const UINT64& keyVal, const SharedPointerT<TimerAction>& pAction) { return TimerTickActionUpdate(keyVal, pAction); });

		//m_IsWriteLocked.fetch_sub(1, std::memory_order_release);

		ValidateTimerKeys();
	}



}; // namespace Svr
}; // namespace BR

