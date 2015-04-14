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

#include "Common/StrUtil.h"
#include "Common/ClassUtil.h"
#include "Common/ArrayUtil.h"
#include "Common/SharedPointer.h"
#include "Common/StaticDoubleLinkedList.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounter.h"




namespace BR {
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

			
			ULONG m_LatestUpdated;


			ULONG m_SyncSerial;
			StaticArray<PerformanceCounter*, 100> m_Counters;


			// assigned a value if it's managed by a manager
			class PerformanceCounterClient* m_pManager;


			void SetUpdatedTime(ULONG updatedTime);

			void AssignManager(PerformanceCounterClient* pManager);
			virtual void Dispose() override;

			friend class PerformanceCounterClient;
			friend class PerformanceCounterServer;

		public:

#ifndef SWIG
			PerformanceCounterInstance(const char *strInstanceName, EntityUID instanceEntityUID);
#endif

			ULONG GetUpdatedTime()										{ return m_LatestUpdated; }
			const char* GetInstanceName()								{ return m_pInstanceName; }
			EntityUID GetInstanceEntityUID()							{ return m_InstanceEntityUID; }
			//CounterListNode& GetListNode()							{ return m_ListNode; }

			//UINT GetNumCounter()										{ return (UINT)m_Counters.GetSize(); }
			Array<PerformanceCounter*>& GetCounters()					{ return m_Counters; }

			// Register this instance to client
			void RegisterToClient();

			// counterName should be global constant
			HRESULT AddCounter(PerformanceCounter* pCounter);
		};


#include "PerformanceCounterInstance.inl"

	}; // namespace BR
}; // namespace Svr





