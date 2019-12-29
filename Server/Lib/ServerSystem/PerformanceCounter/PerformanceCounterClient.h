////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Performance counter
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "String/SFStrUtil.h"

#include "Types/SFEngineTypedefs.h"
#include "Container/SFDualSortedMap.h"
#include "Container/SFPageQueue.h"
#include "Net/SFNetRawUDP.h"
#include "PerformanceCounter/PerformanceCounterInstance.h"



namespace SF {

	namespace Net{
		class RawUDP;
	};

	namespace Svr {

		class PerformanceCounterInstance;

		//////////////////////////////////////////////////////////////////////////
		//
		//	performance counter class
		//


		class PerformanceCounterClient : public Thread
		{
		public:


		private:

			Heap m_Heap;

			NetAddress m_RemoteAddress;
			sockaddr_storage m_RemoteSockAddress;

			Net::RawUDP* m_RawUDP = nullptr;

			uint m_ServerID = 0;

			struct FreeInfo
			{
				PerformanceCounterInstance* pInstance;
				EntityUID UID;

				FreeInfo() : pInstance(nullptr), UID(0)
				{}

				FreeInfo(PerformanceCounterInstance* pInst) : pInstance(pInst), UID(0)
				{
					if(pInstance != nullptr)
						UID = pInst->GetInstanceEntityUID();
				}

				FreeInfo& operator = (const FreeInfo& src) { UID = src.UID; pInstance = src.pInstance; return *this; }
				FreeInfo& operator = (void* src) { assert(src == nullptr);  UID = 0; pInstance = nullptr; return *this; }

				bool operator == (const FreeInfo& src) const { return pInstance == src.pInstance; }
				bool operator != (const FreeInfo& src) const { return pInstance != src.pInstance; }
			};

			typedef DualSortedMap<uint64_t, WeakPointerT<PerformanceCounterInstance>> CounterInstanceMap;
			typedef PageQueue<FreeInfo> FreeInstanceQueue;
			typedef PageQueue<SharedPointerT<PerformanceCounterInstance>> InstanceQueue;

			FreeInstanceQueue m_FreeInstanceQueue;
			InstanceQueue m_NewInstanceQueue;

			CounterInstanceMap m_CounterInstanceMap;

			// default counter for generic purpose
			SharedPointerT<PerformanceCounterInstance> m_DefaultCounter;


			static PerformanceCounterClient *stm_pInstance;

		private:

			IHeap& GetHeap() { return m_Heap; }

			void SendNewCounterPacket(PerformanceCounterInstance *newInstance);

			void UpdateNewInstance(uint64_t newCount);
			void UpdateFreeInstance();
			void UpdateValues();


			Result HandleMessageUpdateCounterInfoS2CEvt(const sockaddr_storage& remoteAddr, MessageDataPtr &&pMsg);

			virtual void Run() override;

		public:
			PerformanceCounterClient();
			~PerformanceCounterClient();

			static Result Initialize(uint serverID, const NetAddress& serverAddress);
			static Result Terminate();

			static PerformanceCounterInstance* GetDefaultCounterInstance();

			static Result RegisterPerformanceCounterInstance(PerformanceCounterInstance *pInstance);

			// Remove counter instance, this will be called by counter instance
			void RemovePerformanceCounterInstance(PerformanceCounterInstance* pInstance);

		};


	}; // namespace SF
}; // namespace Svr





