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
#include "Common/PageQueue.h"
#include "Common/MemoryPool.h"

//#define REFERENCE_DEBUG_TRACKING

namespace BR
{
	class SharedReferenceManager;
	class SharedPointer;
	class SharedObject;

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Shared object pointer manager
	//

	class SharedReferenceManager
	{
	private:

		// Free item queue
		PageQueue<SharedObject*> m_FreeQueue;

		// Linked Object counter 
		SyncCounter m_ObjectCount;

#ifdef REFERENCE_DEBUG_TRACKING
		long DeletedObjects = 0;
#endif


	public:
#ifdef REFERENCE_DEBUG_TRACKING
		Concurrency::concurrent_unordered_map<UINT, SharedObject*> m_PendingFreeObjects;
#endif

	public:

		SharedReferenceManager()
			: m_ObjectCount(0)
		{

		}

		~SharedReferenceManager()
		{

		}

		// Register shared object
		void RegisterSharedObject(SharedObject* pSharedObject);

		// Free shared object
		void FreeSharedReference(SharedObject* pObj
#ifdef REFERENCE_DEBUG_TRACKING
			, const char* fileName, int lineNumber
#endif
			);

		// Garbagte Collect free Pointers
		void UpdateReferenceManager();
	};


}

