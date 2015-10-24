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
#include "Common/BrBaseTypes.h"
#include "Common/Thread.h"
#include "Common/PageQueue.h"
#include "Common/DualSortedMap.h"
#include "Net/NetRawUDP.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterInstance.h"


namespace BR {
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

				typedef HRESULT(PerformanceCounterServer::*MessageHandlerType)(const sockaddr_in6& from, Message::MessageData* &);
				MessageHandlerTable<MessageHandlerType>	m_HandlerTable;

			public:
				MessageHandler(PerformanceCounterServer *CounterServer);

				virtual HRESULT OnRecv(const sockaddr_in6& remoteAddr, Message::MessageData *pMsg) override;
			};

		private:


			Net::RawUDP* m_RawUDP;

			MessageHandler m_MessageHandler;

			struct PacketInfo
			{
				sockaddr_in6 From;
				Message::MessageData* pMessage;

				PacketInfo()
					: pMessage(nullptr)
				{}

				PacketInfo(const sockaddr_in6& from, Message::MessageData* pMsg)
					: pMessage(pMsg), From(from)
				{}

				static PacketInfo NullValue;

				PacketInfo& operator = (const PacketInfo& src) { From = src.From; pMessage = src.pMessage; return *this; }
				PacketInfo& operator = (PacketInfo&& src) { From = src.From; pMessage = src.pMessage; src.pMessage = nullptr; return *this; }
				bool operator == (const PacketInfo& src) const { return pMessage == src.pMessage; }

			};

			PageQueue<PacketInfo> m_NewDeleteQueue;

			PageQueue<PacketInfo> m_UpdateQueue;
			PageQueue<UINT64> m_TimedOutQueue;

			DualSortedMap<UINT64, SharedPointerT<PerformanceCounterInstance>> m_InstanceMap;


			static PerformanceCounterServer *stm_pInstance;

		private:
			PerformanceCounterServer();
			~PerformanceCounterServer();

			void UpdateNewFreeInstance();
			void UpdateValues();

			virtual bool Run() override;

		public:

			static HRESULT Initialize(const char* serverAddress, UINT port);
			static HRESULT Initialize(const NetAddress& serverAddress);
			static HRESULT Terminate();

			static UINT GetInstanceCount();
			static UINT GetInstanceList(UINT startIndex, Array<SharedPointerT<PerformanceCounterInstance>>& instanceList);
			//static UINT GetInstanceList(UINT startIndex, UINT bufferSize, EntityUID pInstanceBuffer);
			static HRESULT GetInstance(UINT64 instanceUID, SharedPointerT<PerformanceCounterInstance>& pInstance);
			static HRESULT GetInstance(EntityUID instanceUID, SharedPointerT<PerformanceCounterInstance>& pInstance);

		protected:

			HRESULT HandleMessageEnqueue(const sockaddr_in6& remoteAddr, Message::MessageData* &pMsg);
			HRESULT HandleMessageEnqueueUpdate(const sockaddr_in6& remoteAddr, Message::MessageData* &pMsg);

			HRESULT HandleMessagePerformanceCounterNewC2SEvt(const sockaddr_in6& remoteAddr, Message::MessageData* &pMsg);
			HRESULT HandleMessagePerformanceCounterFreeC2SEvt(const sockaddr_in6& remoteAddr, Message::MessageData* &pMsg);
			HRESULT HandleMessagePerformanceCounterUpdateC2SEvt(const sockaddr_in6& remoteAddr, Message::MessageData* &pMsg);
		};


}; // namespace BR
}; // namespace Svr





