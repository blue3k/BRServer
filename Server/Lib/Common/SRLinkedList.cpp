////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2010 MadK
// 
// Author : KyungKun Ko
// Date : 08/20/2010
//
// Description : Thread safe Stack implementation
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
//#include "Common/StrUtil.h"
#include "Common/Trace.h"
#include "Common/StackPool.h"



namespace SR
{
	StackPool::StackPool()
		:m_lSize(0)
	{
		//m_head.pHead = (volatile Item*)nullptr;
		//m_head.TSeqID = 0;
	}


	StackPool::~StackPool()
	{
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
#ifndef WIN64
		volatile Header newHeader;
		__int64 iOldValue = 0;
#endif
		Item* pEntry = NULL;
		CounterType myTicket = m_Ticket.AcquireTicket();
		CounterType TSeqID = (CounterType)&oldHeader;// Use local variables address as Thread unique id

		// Defence Too many thread entering race
		while( m_Ticket.GetMyWaitingOrder( myTicket ) > 4 )
		{
			Sleep(0);
		}


		do
		{
			// Defence Race condition
			if( (iTryCount%4) == 0 )
				Sleep(0);

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


#ifndef WIN64
			newHeader.TSeqID = TSeqID;
			newHeader.pHead = pEntry;
			iOldValue = *(__int64*)&oldHeader;
#endif

			_WriteBarrier();
		}
#ifdef WIN64
		while(_InterlockedCompareExchange128( (__int64*)&m_head, (__int64)TSeqID, (__int64)pEntry, (__int64*)&oldHeader) == 0);
#else
		while(_InterlockedCompareExchange64( (__int64*)&m_head, *(__int64*)&newHeader, iOldValue) != iOldValue);
#endif

		m_lSize.InterlockedDecrement();


		oldHeader.pHead->pNext = (Item*)nullptr;

		m_Ticket.ReleaseTicket();

		return (Item*)oldHeader.pHead;
	}


	void StackPool::Push(Item* pEntry)
	{
		UINT iTryCount = 1;
		volatile Header oldHeader;
#ifndef WIN64
		volatile Header newHeader;
		INT64 iOldValue = 0;
#endif
		CounterType myTicket = m_Ticket.AcquireTicket();
		CounterType TSeqID = (CounterType)&oldHeader;// Use local variables address as Thread unique id


		// Defence Too many thread entering race
		while( m_Ticket.GetMyWaitingOrder( myTicket ) > 4 )
		{
			Sleep(0);
		}


		do
		{
			// Defence Race condition
			if( (iTryCount%4) == 0 )
				Sleep(0);

			iTryCount++;

			m_head.TSeqID = TSeqID;

			oldHeader.pHead = m_head.pHead;
			oldHeader.TSeqID = TSeqID;

			pEntry->pNext = (Item*)oldHeader.pHead;

#ifndef WIN64
			newHeader.TSeqID = TSeqID;
			newHeader.pHead = pEntry;

			iOldValue = *(__int64*)&oldHeader;
#endif

			_WriteBarrier();
		}
#ifdef WIN64
		while(_InterlockedCompareExchange128( (__int64*)&m_head, (__int64)TSeqID, (__int64)pEntry, (__int64*)&oldHeader) == 0);
#else
		while(_InterlockedCompareExchange64( (__int64*)&m_head, *(__int64*)&newHeader, iOldValue) != iOldValue);
#endif

		m_lSize.InterlockedIncrement();

		m_Ticket.ReleaseTicket();
	}


};	// namespace Mad


