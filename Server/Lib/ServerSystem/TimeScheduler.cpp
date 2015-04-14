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
#include "ServerSystem/svrTrace.h"
#include "ServerSystem/TimeScheduler.h"



namespace BR {
namespace Svr {
	
	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	
	//

	template class SharedPointerT < TimerAction >;

	TimerAction::TimerAction()
		//: m_InQueueKey(0)
	{
		m_InQueueKey.TimerKey = -1;
		TimeData.TimerKey = -1;
	}

	TimerAction::~TimerAction()
	{
	}

	void TimerAction::SetNextTickTime(UINT32 nextTickTime)
	{
		//Assert((nextTickTime - TimeData.NextTickTime) < 5000); // 
		Assert((LONG)(nextTickTime - Util::Time.GetTimeMs()) < 3*60*1000);
		//Assert(nextTickTime != -1);
		Assert(nextTickTime != 0);
		
		TimeData.NextTickTime = nextTickTime;
	}



	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	
	//

	TimeScheduler::TimeScheduler()
		: m_FailSafeTimerTickInterval(500)
		, m_AssertOnInvalidTickTime(false)
		, m_WorkingThreadID(0)
	{
		m_TimerTickActionGetNextTick = [&](const UINT64& keyVal, const SharedPointerT<TimerAction>& pAction) -> bool
		{
			m_GetNextTickResult = pAction->GetScheduledTime();
			return false;
		};

		m_TimerTickActionUpdate = [&](const UINT64& keyVal, const SharedPointerT<TimerAction>& pAction) -> bool
		{
			ULONG currentTime = Util::Time.GetTimeMs();
			TimerAction::TimeKey key;
			key.TimerKey = keyVal;

			auto diff = (LONG)(key.NextTickTime - currentTime);
			if (diff > 0)
				return false;

			SharedPointerT<TimerAction> removed;
			HRESULT hr = m_TimerMap.Remove(key.TimerKey, removed);
			if (FAILED(hr))
			{
				hr = m_TimerMap.Remove(pAction->GetInQueueKey().TimerKey, removed);
				Assert(SUCCEEDED(hr));
			}
			Assert(removed == pAction);

			auto savedTime = key;
			pAction->m_InQueueKey.NextTickTime = -1;
			Assert((LONG)(pAction->TimeData.NextTickTime - Util::Time.GetTimeMs()) < 5000);

			bool bIsNeedToKeep = pAction->UpdateTick();

			// if it doesn't need to be scheduled anymore
			auto nextDiff = (LONG)(pAction->TimeData.NextTickTime - Util::Time.GetTimeMs());
			if (nextDiff < 0)
			{
				if (key.NextTickTime == pAction->TimeData.NextTickTime)
				{
					if (bIsNeedToKeep)
					{
						Assert(!m_AssertOnInvalidTickTime || bIsNeedToKeep);
						svrTrace(Trace::TRC_ERROR, "Same Timer value:%1% of %0% correct to fail safe timer value:%2%", pAction->GetDebugString(), pAction->TimeData.NextTickTime, m_FailSafeTimerTickInterval);
						pAction->TimeData.NextTickTime = Util::Time.GetTimeMs() + m_FailSafeTimerTickInterval;
					}
				}
				else if (pAction->TimeData.NextTickTime == -1 || !bIsNeedToKeep)
				{
					if (!bIsNeedToKeep)
					{
						//svrTrace(Trace::TRC_ERROR, "Remove tick timer value:-1, %0%", typeid(*(const TimerAction*)pAction).name());
						return true;
					}
				}
				else
				{
					// Too short period, but negative value is fine
					//if (bIsNeedToKeep)
					//{
					//	svrTrace(Trace::TRC_WARN, "Negative tick timer value:%0%, %1%", nextDiff, typeid(*(const TimerAction*)pAction).name());
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
				Assert(pAction->TimeData.NextTickTime != -1);
				pAction->m_InQueueKey.TimerKey = pAction->TimeData.TimerKey;
				if (FAILED(m_TimerMap.Insert(pAction->m_InQueueKey.TimerKey, pAction)))
				{
					Assert(false);
				}
			}

			ValidateTimerKeys();

			return true;

		};
	}

	void TimeScheduler::UpdateWorkingThreadID(ThreadID threadID)
	{
		Assert(threadID != 0);
		m_WorkingThreadID = threadID;
	}

	HRESULT TimeScheduler::AddTimerAction(ThreadID threadID, TimerAction* pAction)
	{
		HRESULT hr = S_OK;


		Assert(m_WorkingThreadID == threadID);
		if (m_WorkingThreadID != threadID)
		{
			svrErr(S_SVR_INVALID_THREAD);
		}

		//Assert(m_IsWriteLocked.load(std::memory_order_relaxed) == 0);

		svrChkPtr(pAction);
		svrAssert(pAction->m_InQueueKey.NextTickTime == -1);
		svrAssert(pAction->TimeData.NextTickTime != -1);
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

		if (pAction->m_InQueueKey.NextTickTime == -1) // if not schedule
			return hr;

		Assert(m_WorkingThreadID == threadID);
		if (m_WorkingThreadID != threadID)
		{
			svrErr(S_SVR_INVALID_THREAD);
		}

		svrChkPtr(pAction);

		if (FAILED(m_TimerMap.Remove(pAction->m_InQueueKey.TimerKey, removed)))
		{
			Assert(pAction->m_InQueueKey.NextTickTime == -1);
		}

		Assert(removed == pAction);
		pAction->m_InQueueKey.NextTickTime = -1;

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
			return S_SVR_INVALID_THREAD;
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
			svrErr(S_SVR_INVALID_THREAD);
		}

		if (pAction->TimeData.NextTickTime == pAction->m_InQueueKey.NextTickTime)
			return hr;

		if (pAction->m_InQueueKey.NextTickTime != -1)
		{
			SharedPointerT<TimerAction> removed;
			if (FAILED(m_TimerMap.Remove(pAction->m_InQueueKey.TimerKey, removed)))
			{
				Assert(false);
			}
			Assert(removed == pAction);
			pAction->m_InQueueKey.NextTickTime = -1;
		}

		Assert(!m_AssertOnInvalidTickTime || ((LONG)(pAction->TimeData.NextTickTime - Util::Time.GetTimeMs()) < (2*60*1000)));
		Assert(pAction->TimeData.NextTickTime != -1);
		if (pAction->TimeData.NextTickTime != -1)
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

	ULONG TimeScheduler::GetNextTimeTick()
	{
		m_GetNextTickResult = -1;
		m_TimerMap.ForeachReverseOrder(0, 1, m_TimerTickActionGetNextTick);

		return m_GetNextTickResult;
	}

	void TimeScheduler::ValidateTimerKeys()
	{
#ifdef DEBUG
		//MutexScopeLock localLock(m_WriteLock);
		//m_TimerMap.ForeachOrderWrite(0, (UINT)m_TimerMap.GetWriteItemCount(), [&](const UINT64& keyVal, SharedPointerT<TimerAction> pAction) -> bool
		//{
		//	Assert(pAction->m_InQueueKey.NextTickTime != -1);
		//	AssertRel(keyVal == pAction->m_InQueueKey.TimerKey);
		//	return true;
		//});
#endif
	}

	void TimeScheduler::UpdateTick(ThreadID threadID)
	{
		CommitChanges(threadID);

		//m_IsWriteLocked.fetch_add(1, std::memory_order_acquire);

		m_TimerMap.ForeachReverseOrder(0, (UINT)m_TimerMap.GetItemCount(), m_TimerTickActionUpdate);

		//m_IsWriteLocked.fetch_sub(1, std::memory_order_release);

		ValidateTimerKeys();
	}



}; // namespace Svr
}; // namespace BR

