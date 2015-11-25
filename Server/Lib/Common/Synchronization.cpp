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



namespace BR
{
	template class TicketScopeLockT<TicketLock>;
	template class TicketScopeLockT<FakeTicketLock>;




	Ticketing::Ticket Ticketing::GetTotalWaitingCount() const
	{
		return m_Working.load(std::memory_order_relaxed) - m_Worked.load(std::memory_order_relaxed);
	}




}; // namespace BR



