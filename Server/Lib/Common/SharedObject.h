////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 MadK
// 
// Author : KyungKun Ko
//
// Description : Shared pointer
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Synchronization.h"

//#define REFERENCE_DEBUG_TRACKING

namespace BR {

	class SharedReferenceManager;
	class SharedPointer;

	// Shared object base
	class SharedObject
	{
	public:
		typedef long long ReferenceCounterType;
		typedef std::atomic<ReferenceCounterType> Counter;

	private:
		// reference counter for shared references
		mutable Counter m_ReferenceCount;

		// reference counter for weak references
		mutable Counter m_WeakReferenceCount;

		// reference counter for manager queue reference
		mutable Counter m_ManagerReferenceCount;

		// reference manager object
		SharedReferenceManager *m_ReferenceManagerObject;

	public:

#ifdef REFERENCE_DEBUG_TRACKING

		volatile const char* LatestReleaseFile;
		volatile int LatestReleaseLine;
		volatile mutable SharedObjectState LatestReleaseState;

		volatile mutable const char* LatestQueueProcessResult;

#endif

		SharedObject();

		virtual ~SharedObject();

		inline ReferenceCounterType			GetReferenceCount() const				{ return m_ReferenceCount.load(std::memory_order_relaxed); }
		inline ReferenceCounterType			GetWeakReferenceCount() const			{ return m_WeakReferenceCount.load(std::memory_order_relaxed); }
		inline ReferenceCounterType			GetManagerReferenceCount() const		{ return m_ManagerReferenceCount.load(std::memory_order_relaxed); }

		inline SharedReferenceManager*		GetReferenceManager()					{ return m_ReferenceManagerObject; }

		//long TestID;

		virtual void Dispose() {}

	private:

		CounterType AddReference() const;
		CounterType ReleaseReference() const;

		CounterType AddWeakReference() const;
		CounterType ReleaseWeakReference() const;

		void GetSharedPointer(SharedPointer& shardPointer) const;

		friend class SharedReferenceManager;
		friend class SharedPointer;
		friend class WeakPointer;
	};

	//extern template class PageQueue<SharedObject*>;



} // namespace BR

