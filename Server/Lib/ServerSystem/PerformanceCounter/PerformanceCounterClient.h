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
#include "Common/DualSortedMap.h"
#include "Common/Pagequeue.h"
#include "Net/NetRawUDP.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterInstance.h"



namespace BR {

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

			class MessageHandler : public Net::RawUDP::MessageHandler
			{
			private:
				PerformanceCounterClient &m_CounterClient;

			public:
				MessageHandler(PerformanceCounterClient *CounterServer)
					: m_CounterClient(*CounterServer)
				{}

				virtual HRESULT OnRecv(const sockaddr_in6& remoteAddr, Message::MessageData *pMsg) override;
			};

		private:

			NetAddress m_RemoteAddress;
			sockaddr_in6 m_RemoteSockAddress;

			Net::RawUDP* m_RawUDP;

			MessageHandler m_MessageHandler;



			struct FreeInfo
			{
				PerformanceCounterInstance* pInstance;
				EntityUID UID;

				FreeInfo() : pInstance(nullptr)
				{}

				FreeInfo(PerformanceCounterInstance* pInst) : pInstance(pInst)
				{
					UID = pInst->GetInstanceEntityUID();
				}

				FreeInfo& operator = (const FreeInfo& src) { UID = src.UID; pInstance = src.pInstance; return *this; }
				FreeInfo& operator = (void* src) { assert(src == nullptr);  UID = 0; pInstance = nullptr; return *this; }

				bool operator == (const FreeInfo& src) const { return pInstance == src.pInstance; }
				bool operator != (const FreeInfo& src) const { return pInstance != src.pInstance; }
			};

			typedef DualSortedMap<UINT64, WeakPointerT<PerformanceCounterInstance>> CounterInstanceMap;
			typedef PageQueue<FreeInfo> FreeInstanceQueue;
			typedef PageQueue<SharedPointerT<PerformanceCounterInstance>> InstanceQueue;

			FreeInstanceQueue m_FreeInstanceQueue;
			InstanceQueue m_NewInstanceQueue;

			CounterInstanceMap m_CounterInstanceMap;

			// default counter for generic purpose
			SharedPointerT<PerformanceCounterInstance> m_DefaultCounter;


			static PerformanceCounterClient *stm_pInstance;

		private:
			PerformanceCounterClient();
			~PerformanceCounterClient();

			void SendNewCounterPacket(PerformanceCounterInstance *newInstance);

			void UpdateNewInstance(ULONG newCount);
			void UpdateFreeInstance();
			void UpdateValues();


			HRESULT HandleMessageUpdateCounterInfoS2CEvt(const sockaddr_in6& remoteAddr, Message::MessageData* &pMsg);

			virtual void Run() override;

		public:

			static HRESULT Initialize(const NetAddress& serverAddress);
			static HRESULT Terminate();

			static PerformanceCounterInstance* GetDefaultCounterInstance();

			static HRESULT RegisterPerformanceCounterInstance(PerformanceCounterInstance *pInstance);

			// Remove counter instance, this will be called by counter instance
			void RemovePerformanceCounterInstance(PerformanceCounterInstance* pInstance);

		};


	}; // namespace BR
}; // namespace Svr





