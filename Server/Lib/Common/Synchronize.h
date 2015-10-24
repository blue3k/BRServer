////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 MadK
// 
// Author : KyungKun Ko
//
// Description : Thread Synchronize
//
////////////////////////////////////////////////////////////////////////////////



#pragma once

#include "Common/Typedefs.h"
#include "Common/BrAssert.h"
#include "Common/Memory.h"
#include "Common/Utility.h"



namespace BR
{


	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Synchronization trait
	//

	// Synchronization trait, not thread safe
	struct ThreadSyncTraitNone
	{
	};

	// Synchronization trait, thread safe
	struct ThreadSyncTraitMT
	{
		bool ThreadSafe;
	};



	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Sync counter class
	//

	//typedef std::atomic<CounterType> SyncCounter;

	class SyncCounter : public std::atomic<CounterType>
	{
	public:
		SyncCounter()                    { store(0, std::memory_order_relaxed); }
		SyncCounter(CounterType value)   { store(value, std::memory_order_relaxed); }

		SyncCounter& operator = (CounterType value) { store(value, std::memory_order_seq_cst); return *this; }
		SyncCounter& operator = (const SyncCounter& value) { store(value.load(std::memory_order_relaxed), std::memory_order_seq_cst); return *this; }
	};



	// Scope counter
	class ScopeCounter
	{
	public:
		ScopeCounter(SyncCounter& counterInstance) : m_CounterInstance(counterInstance)
		{
			m_CounterInstance.fetch_add(1, std::memory_order_acquire);
		}

		~ScopeCounter()
		{
			m_CounterInstance.fetch_sub(1, std::memory_order_release);
		}

	private:
		SyncCounter &m_CounterInstance;
	};

	
	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Spinlock class
	//
	
	class SpinLock
	{
	public:
		// Lock state
		enum {
			STATE_FREE,
			STATE_LOCKED,
		} LOCK_STATE;
	private:

		// Lock state
		std::atomic<LONG> m_LockValue; 
   
	public:
		SpinLock() : m_LockValue(STATE_FREE) { } 
		~SpinLock() {}

	public:
		// Lock
		inline void Lock();
	
		// Unlock
		inline void UnLock();

		// Try lock bit a while
		inline bool TryLock(int iTryCount = 100);

		inline LONG	GetLockCount() { return m_LockValue; }
	};




	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Scopelock class
	//

	class ScopeLock
	{
	private:
		// Spin lock to controllered by scope
		SpinLock& m_SpinLock;


	public:
		// Constructor/destructor
		ScopeLock(SpinLock& nLock)
			:m_SpinLock(nLock)
		{
			m_SpinLock.Lock();
		}
	
		~ScopeLock()
		{
			m_SpinLock.UnLock();
		}
	};




	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Ticketing class
	//

	class Ticketing
	{
	public:
		typedef CounterType Ticket;

		// working thread count...until now
		SyncCounter m_Working;

		// woking complete thread count..
		SyncCounter m_Worked;

		Ticketing() {}
		~Ticketing() {}
		

		// Reset ticket
		inline void Reset();


		// getting a ticket
		inline Ticket AcquireTicket();

		// using done the ticket
		inline Ticket ReleaseTicket();

		// getting my waiting order
		inline Ticket GetMyWaitingOrder(Ticket) const;

		// getting total waiting thread count
		Ticket GetTotalWaitingCount() const;

		// getting working thread count...until now;
		inline Ticket GetNowWorkingCount() const;

		// getting worked thread count...until now;
		inline Ticket GetWorkingCompleteCount() const;
	};



	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Ticket Lock Class with mode support
	//

	class TicketLock
	{
	public:
		enum LockMode 
		{ 
			LOCK_FREE = 0, 
			LOCK_NONEXCLUSIVE,
			LOCK_EXCLUSIVE,
		};

	private:
		// read lock count
		std::atomic<ULONG>	m_OpMode;
		Ticketing			m_Ticketing;
		SyncCounter			m_NonExclusiveCount;

	public:
		inline TicketLock();
		inline ~TicketLock();

		// Exclusive lock/unlock
		inline void ExLock();
		inline void ExUnlock();

		// Non-Exclusive lock/unlock
		inline void NonExLock();
		inline void NonExUnlock();

		// Query status
		inline CounterType GetTicketCount() const;
		inline CounterType GetNonExclusiveCount() const;
		inline bool	IsLocked() const;
	};

	// Fake ticket lock
	class FakeTicketLock
	{
	public:
	private:

	public:
		FakeTicketLock(){}
		~FakeTicketLock(){}

		// Exclusive lock/unlock
		inline void ExLock() {}
		inline void ExUnlock() {}

		// Non-Exclusive lock/unlock
		inline void NonExLock() {}
		inline void NonExUnlock() {}

		// Query status
		inline CounterType GetTicketCount() const {return 0;}
		inline CounterType GetNonExclusiveCount() const {return 0;}
		inline bool	IsLocked() const {return false;}
	};





	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Scope TicketLock Class
	//

	template< class TicketLockType >
	class TicketScopeLockT
	{
	private:
		TicketLock::LockMode	m_LockMode;
		TicketLockType &m_TicketLock;

	public:

		TicketScopeLockT( TicketLock::LockMode lockMode, TicketLockType &ticketLock );	
		~TicketScopeLockT();

	};

	typedef TicketScopeLockT<TicketLock> TicketScopeLock;



	extern template class TicketScopeLockT<TicketLock>;
	extern template class TicketScopeLockT<FakeTicketLock>;



#include "Synchronize.inl"


}; // namespace BR

