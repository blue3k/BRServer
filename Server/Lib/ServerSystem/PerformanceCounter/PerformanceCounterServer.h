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
#include "Types/BrBaseTypes.h"
#include "Thread/Thread.h"
#include "Container/PageQueue.h"
#include "Container/DualSortedMap.h"
#include "Net/NetRawUDP.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterInstance.h"


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

			class MessageHandler : public Net::RawUDP::MessageHandler
			{
			private:
				PerformanceCounterServer &m_CounterServer;

				typedef Result(PerformanceCounterServer::*MessageHandlerType)(const sockaddr_storage& from, Message::MessageData* &);
				MessageHandlerTable<MessageHandlerType>	m_HandlerTable;

			public:
				MessageHandler(PerformanceCounterServer *CounterServer);

				virtual Result OnRecv(const sockaddr_storage& remoteAddr, Message::MessageData *pMsg) override;
			};

		private:


			Net::RawUDP* m_RawUDP;

			MessageHandler m_MessageHandler;

			struct PacketInfo
			{
				Message::MessageData* pMessage;
				sockaddr_storage From;

				PacketInfo(void* ptr = nullptr)
					: pMessage(nullptr)
				{
					assert(ptr == nullptr);
				}

				PacketInfo(const sockaddr_storage& from, Message::MessageData* pMsg)
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

			PageQueue<PacketInfo> m_NewDeleteQueue;

			PageQueue<PacketInfo> m_UpdateQueue;
			PageQueue<void*> m_TimedOutQueue;

			DualSortedMap<uint64_t, SharedPointerT<PerformanceCounterInstance>> m_InstanceMap;


			static PerformanceCounterServer *stm_pInstance;

		private:
			PerformanceCounterServer();
			~PerformanceCounterServer();

			void UpdateNewFreeInstance();
			void UpdateValues();

			virtual void Run() override;

		public:

			static Result Initialize(const char* serverAddress, uint port);
			static Result Initialize(const NetAddress& serverAddress);
			static Result Terminate();

			static uint GetInstanceCount();
			static uint GetInstanceList(uint startIndex, Array<SharedPointerT<PerformanceCounterInstance>>& instanceList);
			//static uint GetInstanceList(uint startIndex, uint bufferSize, EntityUID pInstanceBuffer);
			static Result GetInstance(uint64_t instanceUID, SharedPointerT<PerformanceCounterInstance>& pInstance);
			static Result GetInstance(EntityUID instanceUID, SharedPointerT<PerformanceCounterInstance>& pInstance);

		protected:

			Result HandleMessageEnqueue(const sockaddr_storage& remoteAddr, Message::MessageData* &pMsg);
			Result HandleMessageEnqueueUpdate(const sockaddr_storage& remoteAddr, Message::MessageData* &pMsg);

			Result HandleMessagePerformanceCounterNewC2SEvt(const sockaddr_storage& remoteAddr, Message::MessageData* &pMsg);
			Result HandleMessagePerformanceCounterFreeC2SEvt(const sockaddr_storage& remoteAddr, Message::MessageData* &pMsg);
			Result HandleMessagePerformanceCounterUpdateC2SEvt(const sockaddr_storage& remoteAddr, Message::MessageData* &pMsg);
		};


}; // namespace SF
}; // namespace Svr





