////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 MadK
// 
// Author : KyungKun Ko
//
// Description : Shared pointer
//
////////////////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include "Common/Synchronize.h"
#include "Common/SharedReferenceManager.h"
#include "Common/SharedObject.h"
#include "Common/SharedPointer.h"


namespace BR
{

	/////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Internal implementation
	//

	void SharedReferenceManager::RegisterSharedObject(SharedObject* pSharedObject)
	{
		assert(pSharedObject->m_ReferenceManagerObject == nullptr || pSharedObject->m_ReferenceManagerObject == this);

		if (pSharedObject->m_ReferenceManagerObject != this)
			m_ObjectCount.fetch_add(1, std::memory_order_relaxed);

		// This will just link manager to object
		pSharedObject->m_ReferenceManagerObject = this;

#ifdef REFERENCE_DEBUG_TRACKING
		Assert(pSharedObject->TestID >= 0 && pSharedObject->TestID < 10000);
		m_PendingFreeObjects.insert(std::make_pair((unsigned int)pSharedObject->TestID, pSharedObject));
#endif
	}

	// 
	void SharedReferenceManager::FreeSharedReference(SharedObject* pObj
#ifdef REFERENCE_DEBUG_TRACKING
		, const char* fileName, int lineNumber
#endif
		)
	{
		if (pObj == nullptr)
			return;

#ifdef REFERENCE_DEBUG_TRACKING
		auto state = pObj->m_SharedObjectState.load(std::memory_order_relaxed);

		pObj->LatestReleaseFile = fileName;
		pObj->LatestReleaseLine = lineNumber;
		pObj->LatestReleaseState = state;
#endif

		pObj->m_ManagerReferenceCount.fetch_add(1, std::memory_order_acquire);
		m_FreeQueue.Enqueue(pObj);
	}



	// Garbagte Collect free Pointers
	void SharedReferenceManager::UpdateReferenceManager()
	{
		auto numItems = m_FreeQueue.GetEnqueCount();
		for (auto item = 0; item < numItems; item++)
		{
			SharedObject* pObj = nullptr;
			if (!m_FreeQueue.Dequeue(pObj))
				break;

			auto managerRefCount = pObj->m_ManagerReferenceCount.fetch_sub(1, std::memory_order_relaxed) - 1;
			// it's queued more than once. leave this object for later operation
			if (managerRefCount > 0)
			{
#ifdef REFERENCE_DEBUG_TRACKING
				pObj->LatestQueueProcessResult = "In Queue again";
#endif
				continue;
			}


#ifdef REFERENCE_DEBUG_TRACKING

			AssertRel(pObj->GetReferenceCount() == 0);
			AssertRel(pObj->m_SharedObjectState == SharedObject::SharedObjectState::Disposed || pObj->m_SharedObjectState == SharedObject::SharedObjectState::Deleted);
			pObj->m_ReferenceManagerObject = nullptr;

			Interlocked::Increment(DeletedObjects);


			// no one is using this object pointer. We are good to release it
			AssertRel(pObj->GetReferenceCount() == 0 && pObj->GetWeakReferenceCount() == 0 && pObj->GetManagerReferenceCount() == 0);
			m_PendingFreeObjects.unsafe_erase(pObj->TestID);
			//_ReadWriteBarrier();
			std::atomic_thread_fence(std::memory_order_cst_seq);
#endif
			m_ObjectCount.fetch_sub(1, std::memory_order_relaxed);

			delete pObj;
		}


	}

}

