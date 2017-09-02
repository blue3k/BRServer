////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Performance counter
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "String/StrUtil.h"
#include "Common/ClassUtil.h"
#include "Container/SFArray.h"
#include "Object/SharedPointer.h"
#include "Common/StaticDoubleLinkedList.h"
#include "PerformanceCounter/PerformanceCounter.h"




namespace SF {
	namespace Svr {



		//////////////////////////////////////////////////////////////////////////
		//
		//	performance counter class
		//

		class PerformanceCounterInstance : public SharedObject
		{
		private:

			char m_pInstanceName[128];
			EntityUID m_InstanceEntityUID;

			
			TimeStampMS m_LatestUpdated;


			ULONG m_SyncSerial;
			StaticArray<PerformanceCounter*, 100> m_Counters;


			// assigned a value if it's managed by a manager
			class PerformanceCounterClient* m_pManager;


			void SetUpdatedTime(TimeStampMS updatedTime);

			void AssignManager(PerformanceCounterClient* pManager);
			virtual void Dispose() override;

			friend class PerformanceCounterClient;
			friend class PerformanceCounterServer;

		public:

#ifndef SWIG
			PerformanceCounterInstance(const char *strInstanceName, EntityUID instanceEntityUID);
#endif

			TimeStampMS GetUpdatedTime()								{ return m_LatestUpdated; }
			const char* GetInstanceName()								{ return m_pInstanceName; }
			EntityUID GetInstanceEntityUID()							{ return m_InstanceEntityUID; }
			//CounterListNode& GetListNode()							{ return m_ListNode; }

			//uint GetNumCounter()										{ return (uint)m_Counters.GetSize(); }
			Array<PerformanceCounter*>& GetCounters()					{ return m_Counters; }

			// Register this instance to client
			void RegisterToClient();

			// counterName should be global constant
			Result AddCounter(PerformanceCounter* pCounter);
		};


#include "PerformanceCounterInstance.inl"

	}; // namespace Svr

	extern template class SharedPointerT < Svr::PerformanceCounterInstance >;
	extern template class WeakPointerT < Svr::PerformanceCounterInstance >;

}; // namespace SF





