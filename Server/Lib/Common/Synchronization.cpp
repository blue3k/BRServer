////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Thread library
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Trace.h"
#include "Common/StrUtil.h"
#include "Common/Thread.h"
#include "Common/Synchronization.h"
#include "Common/SystemSynchronization.h"



namespace BR
{
	template class TicketScopeLockT<TicketLock>;
	template class TicketScopeLockT<FakeTicketLock>;




	Ticketing::Ticket Ticketing::GetTotalWaitingCount() const
	{
		return m_Working.load(std::memory_order_relaxed) - m_Worked.load(std::memory_order_relaxed);
	}



#if defined(_WIN32) || defined(_WIN64)


#else

	Event::Event(bool isInitialySet, bool autoReset)
		: m_hEvent(isInitialySet)
		, m_AutoReset(autoReset)
	{
	}

	Event::~Event()
	{
	}

	void Event::Reset()
	{
		m_hEvent.store(false, std::memory_order_seq_cst);
	}

	void Event::Set()
	{
		m_hEvent.store(true, std::memory_order_seq_cst);
	}


	bool Event::WaitEvent(DurationMS waitTimeMs)
	{
		bool bSet = false;
		auto startTime = Util::Time.GetRawTimeMs();
		if (m_AutoReset)
		{
			do
			{
				bSet = m_hEvent.exchange(false, std::memory_order_seq_cst);
			} while (!bSet && Util::TimeSinceRaw(startTime) < waitTimeMs);
		}
		else
		{
			do
			{
				bSet = m_hEvent.load(std::memory_order_seq_cst);
			} while (!bSet && Util::TimeSinceRaw(startTime) < waitTimeMs);
		}

		return bSet;
	}

#endif



}; // namespace BR



