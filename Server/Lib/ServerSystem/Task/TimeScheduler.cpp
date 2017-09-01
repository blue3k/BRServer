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
#include "Thread/Thread.h"
#include "String/StrUtil.h"
#include "Common/Trace.h"
#include "Common/ToString.h"
#include "Task/TimeScheduler.h"
#include "ResultCode/SFResultCodeCommon.h"



namespace BR {

	template class SharedPointerT < TimerAction >;

	
	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	
	//


	TimerAction::TimerAction()
		: m_Queued(false)
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
		// circular time can be any value
		//Assert(nextTickTime != TimeStampMS::min());
		//if (nextTickTime < curTime || nextTickTime == TimeStampMS::min()) // correct the time
		if (nextTickTime < curTime) // correct the time
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
		Result hr = m_TimerMap.Remove(key.TimerKey, removed);
		if (!(hr))
		{
			hr = m_TimerMap.Remove(pAction->GetInQueueKey().TimerKey, removed);
			Assert((hr));
		}
		Assert(removed == pAction);
		// removed clear in-queue time
		pAction->m_InQueueKey.Components.NextTickTime = TimeStampMS::max();
		pAction->m_Queued = false;

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
					defTrace(Trace::TRC_DBG1, "Same Timer value:{1} of {0} correct to fail safe timer value:{2}", pAction->GetDebugString(), pAction->TimeData.Components.NextTickTime, m_FailSafeTimerTickInterval);
					pAction->TimeData.Components.NextTickTime = Util::Time.GetTimeMs() + m_FailSafeTimerTickInterval;
				}
			}
			// this condition isn't true for long running machine
			//else if (pAction->TimeData.Components.NextTickTime == TimeStampMS::max() || !bIsNeedToKeep)
			//{
			//	if (!bIsNeedToKeep)
			//	{
			//		//svrTrace(Trace::TRC_DBG1, "Remove tick timer value:-1, {0}", typeid(*(const TimerAction*)pAction).name());
			//		return true;
			//	}
			//}
			//else
			//{
			//	// Too short period, but negative value is fine
			//	//if (bIsNeedToKeep)
			//	//{
			//	//	svrTrace(Trace::TRC_WARN, "Negative tick timer value:{0}, {1}", nextDiff, typeid(*(const TimerAction*)pAction).name());
			//	//}
			//	//Assert(!m_AssertOnInvalidTickTime);
			//	//pAction->TimeData.NextTickTime = Util::Time.GetTimeMs() + m_FailSafeTimerTickInterval;
			//	//svrTrace(Trace::TRC_ERROR, "Invalid Timer value, correct to fail safe timer value");
			//}
		}
		else
		{
			//Assert(nextDiff < 10000);
		}

		if (bIsNeedToKeep)
		{
			//Assert(pAction->TimeData.TimerKey != 0);
			// Time tick can be 0xffffffff
			//Assert(pAction->TimeData.Components.NextTickTime != TimeStampMS::max());
			Assert(pAction->m_Queued == false);
			pAction->m_InQueueKey.TimerKey = pAction->TimeData.TimerKey;
			pAction->m_Queued = true;
			if (!(m_TimerMap.Insert(pAction->m_InQueueKey.TimerKey, pAction)))
			{
				pAction->m_Queued = false;
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

	Result TimeScheduler::AddTimerAction(ThreadID threadID, TimerAction* pAction)
	{
		Result hr = ResultCode::SUCCESS;


		Assert(m_WorkingThreadID == threadID);
		if (m_WorkingThreadID != threadID)
		{
			defErr(ResultCode::E_INVALID_THREAD);
		}

		//Assert(m_IsWriteLocked.load(std::memory_order_relaxed) == 0);

		defChkPtr(pAction);
		defAssert(pAction->m_InQueueKey.Components.NextTickTime == TimeStampMS::max() && pAction->m_Queued == false);
		// new time can be TimeStampMS::max() if muchine is running over 28 days
		//defAssert(pAction->TimeData.Components.NextTickTime != TimeStampMS::max());
		//defAssert(pAction->TimeData.TimerKey != 0);

		Assert(pAction->m_Queued == false);
		pAction->m_Queued = true;
		pAction->m_InQueueKey.TimerKey = pAction->TimeData.TimerKey;
		if (!(m_TimerMap.Insert(pAction->m_InQueueKey.TimerKey, pAction)))
		{
			pAction->m_Queued = false;
			Assert(false);
		}


	Proc_End:

		return hr;
	}

	Result TimeScheduler::RemoveTimerAction(ThreadID threadID, TimerAction* pAction)
	{
		Result hr = ResultCode::SUCCESS;
		SharedPointerT<TimerAction> removed;

		if (pAction == nullptr)
			return hr;

		if (pAction->m_Queued == false) // if not schedule
			return hr;

		Assert(m_WorkingThreadID == threadID);
		if (m_WorkingThreadID != threadID)
		{
			defErr(ResultCode::E_INVALID_THREAD);
		}

		defChkPtr(pAction);

		if (!(m_TimerMap.Remove(pAction->m_InQueueKey.TimerKey, removed)))
		{
			Assert(pAction->m_InQueueKey.Components.NextTickTime == TimeStampMS::max() && pAction->m_Queued == false);
		}

		Assert(removed == pAction);
		pAction->m_InQueueKey.Components.NextTickTime = TimeStampMS::max();
		pAction->m_Queued = false;

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

	Result TimeScheduler::CommitChanges(ThreadID threadID)
	{
		//Assert(m_IsWriteLocked.load(std::memory_order_relaxed) == 0);

		Assert(m_WorkingThreadID == threadID);
		if (m_WorkingThreadID != threadID)
		{
			return ResultCode::E_INVALID_THREAD;
		}
		return m_TimerMap.CommitChanges();
	}

	Result TimeScheduler::Reschedul(ThreadID threadID, TimerAction* pAction)
	{
		Result hr = ResultCode::SUCCESS;

		if (pAction == nullptr)
			return ResultCode::SUCCESS;

		//Assert(m_IsWriteLocked.load(std::memory_order_relaxed) == 0);

		Assert(m_WorkingThreadID == threadID);
		if (m_WorkingThreadID != threadID)
		{
			defErr(ResultCode::E_INVALID_THREAD);
		}

		if (pAction->TimeData.Components.NextTickTime == pAction->m_InQueueKey.Components.NextTickTime)
			return hr;

		//if (pAction->m_InQueueKey.Components.NextTickTime != TimeStampMS::max())
		if (pAction->m_Queued)
		{
			SharedPointerT<TimerAction> removed;
			if (!(m_TimerMap.Remove(pAction->m_InQueueKey.TimerKey, removed)))
			{
				Assert(false);
			}
			else
			{
				Assert(removed == pAction);
			}
			pAction->m_InQueueKey.Components.NextTickTime = TimeStampMS::max();
			pAction->m_Queued = false;
		}

		Assert(!m_AssertOnInvalidTickTime || ( (LONG)(pAction->TimeData.Components.NextTickTime - Util::Time.GetTimeMs()).count() < (2*60*1000)));
		Assert(pAction->TimeData.Components.NextTickTime != TimeStampMS::max());
		if (pAction->TimeData.Components.NextTickTime != TimeStampMS::max())
		{
			pAction->m_InQueueKey.TimerKey = pAction->TimeData.TimerKey;
			pAction->m_Queued = true;
			if (!(m_TimerMap.Insert(pAction->m_InQueueKey.TimerKey, pAction)))
			{
				pAction->m_Queued = false;
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




}; // namespace BR

