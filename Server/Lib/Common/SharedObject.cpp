////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 MadK
// 
// Author : KyungKun Ko
//
// Description : Shared pointer
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/SharedObject.h"
#include "Common/Synchronization.h"
#include "Common/SharedReferenceManager.h"
#include "Common/SharedPointer.h"


namespace BR
{
	///////////////////////////////////////////////////////////////////////////
	//
	//
	//

	template class PageQueue<SharedObject*>;

	CounterType SharedObject::AddReference() const
	{
		auto org = m_ReferenceCount.fetch_add(1, std::memory_order_acquire);
		if (org == 0)
		{
			// First shared reference should have one designated weak reference
			Assert(GetWeakReferenceCount() == 1);
		}
		return org;
	}

	CounterType SharedObject::ReleaseReference() const
	{
		auto refManager = m_ReferenceManagerObject;
		auto org = m_ReferenceCount.fetch_sub(1, std::memory_order_release);
		auto decValue = org - 1;
		if (decValue <= 0)
		{
			const_cast<SharedObject*>(this)->Dispose();

			ReleaseWeakReference();
		}
		return org;
	}

	CounterType SharedObject::AddWeakReference() const
	{
		return m_WeakReferenceCount.fetch_add(1, std::memory_order_acquire);
	}

	CounterType SharedObject::ReleaseWeakReference() const
	{
		CounterType org = 0;
		if (m_ReferenceManagerObject != nullptr)
		{
			auto localRef = m_ReferenceManagerObject;
			Assert(localRef != nullptr);
			auto decValue = org = m_WeakReferenceCount.fetch_sub(1, std::memory_order_release);
			if (decValue <= 1)
			{
				localRef->FreeSharedReference(const_cast<SharedObject*>(this)
#ifdef REFERENCE_DEBUG_TRACKING
					, fileName, lineNumber
#endif
					);
			}
		}
		else
		{
			auto decValue = org = m_WeakReferenceCount.fetch_sub(1, std::memory_order_release);
			if (decValue <= 1)
			{
				delete this;
			}
		}

		return org;
	}


	void SharedObject::GetSharedPointer(SharedPointer& shardPointer) const
	{
		ReferenceCounterType curReference;
		do
		{
			curReference = m_ReferenceCount.load(std::memory_order_consume);
			if (curReference <= 0)
			{
				shardPointer = nullptr;
				return;
			}
		} while (!m_ReferenceCount.compare_exchange_weak(curReference, curReference + 1, std::memory_order_relaxed, std::memory_order_relaxed));

		shardPointer.SetPointer(const_cast<SharedObject*>(this));
	}

	///////////////////////////////////////////////////////////////////////////
	//
	//
	//


}

