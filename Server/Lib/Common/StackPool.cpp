////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 MadK
// 
// Author : KyungKun Ko
//
// Description : Thread safe Stack implementation
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Trace.h"
#include "Common/BrAssert.h"
#include "Common/StackPool.h"



namespace BR
{
	StackPool::StackPool()
		:m_lSize(0)
	{
		AssertRel( ((intptr_t)this & (BR_ALIGN_DOUBLE-1)) == 0 );
		//m_head.pHead = (volatile Item*)nullptr;
		//m_head.TSeqID = 0;
	}


	StackPool::~StackPool()
	{
		AssertRel( ((intptr_t)this & (BR_ALIGN_DOUBLE-1)) == 0 );
		Clear();
	}


	// Clear
	void StackPool::Clear()
	{
		// This stack didn't created item os nothing to release, just clear variables
		m_head.pHead = NULL;
		m_head.TSeqID = 0;
		m_lSize = (CounterType)0;
	}

	StackPool::Item* StackPool::Pop()
	{
		UINT iTryCount = 1;
		volatile Header oldHeader;
#ifndef _WIN64
		volatile Header newHeader;
		__int64 iOldValue = 0;
#endif
		Item* pEntry = NULL;
		CounterType myTicket = m_Ticket.AcquireTicket();
		CounterType TSeqID = (CounterType)&oldHeader;// Use local variables address as Thread unique id

		// Defence Too many thread entering race
		while( m_Ticket.GetMyWaitingOrder( myTicket ) > 4 )
		{
			ThisThread::SleepFor(DurationMS(0));
		}


		do
		{
			// Defence Race condition
			if( (iTryCount%4) == 0 )
				ThisThread::SleepFor(DurationMS(0));

			iTryCount++;

			m_head.TSeqID = TSeqID;

			oldHeader.pHead = m_head.pHead;
			oldHeader.TSeqID = TSeqID;

			if( oldHeader.pHead == NULL )
			{
				m_Ticket.ReleaseTicket();
				return NULL;
			}

			pEntry = oldHeader.pHead->pNext;


#ifndef _WIN64
			newHeader.TSeqID = TSeqID;
			newHeader.pHead = pEntry;
			iOldValue = *(__int64*)&oldHeader;
#endif

			//_WriteBarrier();
			std::atomic_thread_fence(std::memory_order_seq_cst);
		}
#ifdef _WIN64
		while(_InterlockedCompareExchange128( (__int64*)&m_head, (__int64)TSeqID, (__int64)pEntry, (__int64*)&oldHeader) == 0);
#else
		while(_InterlockedCompareExchange64( (__int64*)&m_head, *(__int64*)&newHeader, iOldValue) != iOldValue);
#endif

		m_lSize.fetch_sub(1,std::memory_order_relaxed);


		oldHeader.pHead->pNext = (Item*)nullptr;

		m_Ticket.ReleaseTicket();

		return (Item*)oldHeader.pHead;
	}


	void StackPool::Push(Item* pEntry)
	{
		UINT iTryCount = 1;
		volatile Header oldHeader;
#ifndef _WIN64
		volatile Header newHeader;
		INT64 iOldValue = 0;
#endif
		CounterType myTicket = m_Ticket.AcquireTicket();
		CounterType TSeqID = (CounterType)&oldHeader;// Use local variables address as Thread unique id


		// Defence Too many thread entering race
		while( m_Ticket.GetMyWaitingOrder( myTicket ) > 4 )
		{
			ThisThread::SleepFor(DurationMS(0));
		}


		do
		{
			// Defence Race condition
			if( (iTryCount%4) == 0 )
				ThisThread::SleepFor(DurationMS(0));

			iTryCount++;

			m_head.TSeqID = TSeqID;

			oldHeader.pHead = m_head.pHead;
			oldHeader.TSeqID = TSeqID;

			pEntry->pNext = (Item*)oldHeader.pHead;

#ifndef _WIN64
			newHeader.TSeqID = TSeqID;
			newHeader.pHead = pEntry;

			iOldValue = *(__int64*)&oldHeader;
#endif

			//_WriteBarrier();
			std::atomic_thread_fence(std::memory_order_seq_cst);
		}
#ifdef _WIN64
		while(_InterlockedCompareExchange128( (__int64*)&m_head, (__int64)TSeqID, (__int64)pEntry, (__int64*)&oldHeader) == 0);
#else
		while(_InterlockedCompareExchange64( (__int64*)&m_head, *(__int64*)&newHeader, iOldValue) != iOldValue);
#endif

		m_lSize.fetch_add(1, std::memory_order_relaxed);

		m_Ticket.ReleaseTicket();
	}


};	// namespace Mad


