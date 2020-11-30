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
#include "Thread/SFThread.h"
#include "Container/SFPageQueue.h"
#include "Container/SFDualSortedMap.h"
#include "Net/SFNetRawUDP.h"
#include "Transaction/MessageRoute.h"
#include "PerformanceCounter/PerformanceCounterInstance.h"


namespace SF {
namespace Svr {



		//////////////////////////////////////////////////////////////////////////
		//
		//	performance counter class
		//


		class PerformanceCounterServer : public Thread
		{
		public:

			enum {
				TIMER_TIMOUT = 10 * 1000
			};

			typedef Result(PerformanceCounterServer::*MessageHandlerType)(const sockaddr_storage& from, MessageDataPtr &);

		private:

			Heap m_Heap;

			Net::RawUDP* m_RawUDP = nullptr;

			struct PacketInfo
			{
				MessageDataPtr pMessage;
				sockaddr_storage From;

				PacketInfo(void* ptr = nullptr)
				{
					assert(ptr == nullptr);
				}

				PacketInfo(const sockaddr_storage& from, MessageDataPtr pMsg)
					: pMessage(pMsg), From(from)
				{}

				PacketInfo(const PacketInfo& src)
					: pMessage(src.pMessage)
					, From(src.From)
				{}

				PacketInfo& operator = (const PacketInfo& src) { From = src.From; pMessage = src.pMessage; return *this; }
				PacketInfo& operator = (PacketInfo&& src) { From = src.From; pMessage = src.pMessage; src.pMessage = nullptr; return *this; }

				bool operator == (const PacketInfo& src) const { return pMessage == src.pMessage; }
				bool operator != (const PacketInfo& src) const { return pMessage != src.pMessage; }
			};

			MessageHandlerTable<MessageHandlerType>	m_HandlerTable;

			PageQueue<PacketInfo> m_NewDeleteQueue;

			PageQueue<PacketInfo> m_UpdateQueue;
			PageQueue<void*> m_TimedOutQueue;

			DualSortedMap<uint64_t, SharedPointerT<PerformanceCounterInstance>> m_InstanceMap;


			static PerformanceCounterServer *stm_pInstance;

		private:

			void UpdateNewFreeInstance();
			void UpdateValues();

			virtual void Run() override;

		public:
			PerformanceCounterServer();
			~PerformanceCounterServer();

			IHeap& GetHeap() { return *m_Heap.get(); }

			static Result Initialize(const char* serverAddress, uint port);
			static Result Initialize(const NetAddress& serverAddress);
			static Result Terminate();

			static uint GetInstanceCount();
			static uint GetInstanceList(uint startIndex, Array<SharedPointerT<PerformanceCounterInstance>>& instanceList);
			//static uint GetInstanceList(uint startIndex, uint bufferSize, EntityUID pInstanceBuffer);
			static Result GetInstance(uint64_t instanceUID, SharedPointerT<PerformanceCounterInstance>& pInstance);
			static Result GetInstance(EntityUID instanceUID, SharedPointerT<PerformanceCounterInstance>& pInstance);

		protected:

			Result HandleMessageEnqueue(const sockaddr_storage& remoteAddr, MessageDataPtr &pMsg);
			Result HandleMessageEnqueueUpdate(const sockaddr_storage& remoteAddr, MessageDataPtr &pMsg);

			Result HandleMessagePerformanceCounterNewC2SEvt(const sockaddr_storage& remoteAddr, MessageDataPtr &pMsg);
			Result HandleMessagePerformanceCounterFreeC2SEvt(const sockaddr_storage& remoteAddr, MessageDataPtr &pMsg);
			Result HandleMessagePerformanceCounterUpdateC2SEvt(const sockaddr_storage& remoteAddr, MessageDataPtr &pMsg);
		};


}; // namespace SF
}; // namespace Svr





