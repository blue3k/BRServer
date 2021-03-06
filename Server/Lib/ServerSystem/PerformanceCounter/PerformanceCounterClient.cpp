////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Performance counter
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"

#include "Net/SFNetRawUDP.h"
#include "SvrTrace.h"
#include "Server/BrServerUtil.h"
#include "Entity/EntityTable.h"
#include "Entity/EntityManager.h"
#include "Protocol/MonitoringMsgClass.h"
#include "PerformanceCounter/PerformanceCounterClient.h"
#include "Service/ServerService.h"





namespace SF {
namespace Svr {


	PerformanceCounterClient *PerformanceCounterClient::stm_pInstance = nullptr;

	//Result PerformanceCounterClient::MessageHandler::OnRecv(const sockaddr_storage& remoteAddr, MessageDataPtr &pMsg)
	//{
	//	Result hr = ResultCode::SUCCESS;

	//	if (pMsg->GetMessageHeader()->msgID.GetMsgID() == Message::Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt::MID.GetMsgID())
	//	{
	//		svrChk(m_CounterClient.HandleMessageUpdateCounterInfoS2CEvt(remoteAddr, std::forward<MessageDataPtr>(pMsg)));
	//	}

	//Proc_End:

	//	return hr;
	//}


	PerformanceCounterClient::PerformanceCounterClient()
		: m_Heap("PerformanceCounterClient",GetSystemHeap())
		, m_FreeInstanceQueue(GetHeap())
		, m_NewInstanceQueue(GetHeap())
		, m_CounterInstanceMap(GetHeap())
	{
	}

	PerformanceCounterClient::~PerformanceCounterClient()
	{
		m_FreeInstanceQueue.ClearQueue();
		m_NewInstanceQueue.ClearQueue();

		m_CounterInstanceMap.ClearMap();

		if (m_RawUDP != nullptr)
		{
			m_RawUDP->TerminateNet();
			Util::SafeDelete(m_RawUDP);
		}
	}

	void PerformanceCounterClient::SendNewCounterPacket(PerformanceCounterInstance *newInstance)
	{
		if (m_RawUDP == nullptr)
		{
			Assert(false);
		}

		MessageDataPtr pMsg;

		StaticArray<PerformanceCounterInfo, 100> counterInfos(GetHeap());
		auto& counters = newInstance->GetCounters();
		for (uint iCounter = 0; iCounter < counters.size(); iCounter++)
		{
			auto counter = counters[iCounter];
			PerformanceCounterInfo info;
			memset(&info, 0, sizeof info);
			StrUtil::StringCopy(info.CounterName, counter->GetCounterName());
			info.DateType = (uint)counter->GetDataType();
			if (!(counterInfos.push_back(info)))
				break;
		}

		pMsg = Message::Monitoring::PerformanceCounterNewC2SEvt::Create(GetHeap(), newInstance->GetInstanceName(), newInstance->GetInstanceEntityUID(), counterInfos);
		if (pMsg == nullptr)
		{
			svrTrace(Error, "Failed to generate performance counter free packet");
		}
		else
		{
			m_RawUDP->SendMsg(m_RemoteSockAddress, pMsg);
		}
	}

	void PerformanceCounterClient::UpdateNewInstance(uint64_t newCount)
	{
		SharedPointerT<PerformanceCounterInstance> newInstance;
		for (uint iItem = 0; iItem < newCount && (m_NewInstanceQueue.Dequeue(newInstance)); iItem++)
		{
			if (newInstance == nullptr)
				continue;

			if (!(m_CounterInstanceMap.Insert(newInstance->GetInstanceEntityUID().UID, newInstance)))
			{
				svrTrace(Error, "Failed to insert new performance counter instance");
				continue;
			}

			SendNewCounterPacket((PerformanceCounterInstance*)newInstance);
		}


	}

	void PerformanceCounterClient::UpdateFreeInstance()
	{
		FreeInfo freeInfo;
		MessageDataPtr pMsg;

		auto newCount = m_NewInstanceQueue.GetEnqueCount();

		std::atomic_thread_fence(std::memory_order_seq_cst);

		auto freeCount = m_FreeInstanceQueue.GetEnqueCount();

		StaticArray<uint64_t, 10> freeList(GetHeap());
		for (uint iItem = 0; iItem < freeCount && (m_FreeInstanceQueue.Dequeue(freeInfo)); iItem++)
		{
			Assert(freeInfo.pInstance);
			if (freeInfo.pInstance == nullptr)
				continue;

			WeakPointerT<PerformanceCounterInstance> removed;
			if ((m_CounterInstanceMap.Remove(freeInfo.UID.UID, removed)))
			{
				freeList.push_back(freeInfo.UID);

				if (freeList.size() >= freeList.GetAllocatedSize() && m_RawUDP != nullptr)
				{
					pMsg = Message::Monitoring::PerformanceCounterFreeC2SEvt::Create(GetHeap(), freeList);
					if (pMsg == nullptr)
					{
						svrTrace(Error, "Failed to generate performance counter free packet");
					}
					else
					{
						m_RawUDP->SendMsg(m_RemoteSockAddress, pMsg);
					}
				}

			}
			else
			{
				Assert(false);
			}
		}

		if (freeList.size() > 0)
		{
			pMsg = Message::Monitoring::PerformanceCounterFreeC2SEvt::Create(GetHeap(), freeList);
			if (pMsg == nullptr)
			{
				svrTrace(Error, "Failed to generate performance counter free packet");
			}
			else
			{
				m_RawUDP->SendMsg(m_RemoteSockAddress, pMsg);
			}
		}

		UpdateNewInstance((uint64_t)newCount);
	}


	void PerformanceCounterClient::UpdateValues()
	{
		if (m_RawUDP == nullptr)
			return;

		m_CounterInstanceMap.CommitChanges();
		m_CounterInstanceMap.ForeachOrder(0, (uint)m_CounterInstanceMap.size(), [&](const intptr_t& key, const WeakPointerT<PerformanceCounterInstance>& value)
		{
			SharedPointerT<PerformanceCounterInstance> pInstance;

			pInstance = value.AsSharedPtr();
			if (pInstance == nullptr)
				return true;

			MessageDataPtr pMsg;

			StaticArray<uint64_t, 100> counterValues(GetHeap());
			auto& counters = pInstance->GetCounters();
			for (uint iCounter = 0; iCounter < counters.size(); iCounter++)
			{
				uint64_t tempValue = 0;
				if (!(counters[iCounter]->CopyTo((uint)sizeof(tempValue), (uint8_t*)&tempValue)))
				{
					Assert(false);
					break;
				}

				if (!(counterValues.push_back(tempValue)))
					break;
			}

			pMsg = Message::Monitoring::PerformanceCounterUpdateC2SEvt::Create(GetHeap(), pInstance->GetInstanceEntityUID(), counterValues);
			if(pMsg == nullptr)
			{
				svrTrace(Error, "Failed to generate performance counter free packet");
			}
			else
			{
				m_RawUDP->SendMsg(m_RemoteSockAddress, pMsg);
			}

			return true;
		});
	}

	void PerformanceCounterClient::Run()
	{
		SetPriority(Thread::PRIORITY::ABOVE_NORMAL);

		while (1)
		{
			// 50ms will be the precision of our timer
			auto loopInterval = UpdateInterval(DurationMS(1000));

			if (CheckKillEvent(loopInterval))
			{
				// Kill Event signaled
				break;
			}

			UpdateFreeInstance();
			UpdateValues();
		}
	}

	Result PerformanceCounterClient::Initialize(uint serverID, const NetAddress& serverAddress)
	{
		NetAddress localAddress;
		auto pRawUDP = new(GetSystemHeap()) Net::RawUDP();

		ScopeContext hr([&pRawUDP](Result result) {
			if (result)
				return;

			if (pRawUDP != nullptr)
			{
				pRawUDP->TerminateNet();
				IHeap::Delete(pRawUDP);
			}

			Terminate();
		});

		svrCheckPtr(pRawUDP);

		stm_pInstance = new(GetSystemHeap()) PerformanceCounterClient();

		Net::Addr2SockAddr(serverAddress, stm_pInstance->m_RemoteSockAddress);
		stm_pInstance->m_RemoteAddress = serverAddress;

		stm_pInstance->m_ServerID = serverID;

		// use same network family
		svrCheck(Net::GetLocalAddress(serverAddress.SocketFamily, localAddress));

		svrCheck(pRawUDP->InitializeNet(localAddress, 
			[](const sockaddr_storage& remoteAddr, SharedPointerT<Message::MessageData>& pMsg)
			{
				if (pMsg->GetMessageHeader()->msgID.GetMsgID() == Message::Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt::MID.GetMsgID())
				{
					stm_pInstance->HandleMessageUpdateCounterInfoS2CEvt(remoteAddr, std::forward<MessageDataPtr>(pMsg));
				}
				return ResultCode::SUCCESS;
			})
		);

		stm_pInstance->m_RawUDP = pRawUDP;
		pRawUDP = nullptr;

		stm_pInstance->Start();

		return hr;
	}

	Result PerformanceCounterClient::Terminate()
	{
		if (stm_pInstance == nullptr)
			return ResultCode::SUCCESS;

		stm_pInstance->Stop(true);

		IHeap::Delete(stm_pInstance);
		stm_pInstance = nullptr;

		return ResultCode::SUCCESS;
	}

	PerformanceCounterInstance* PerformanceCounterClient::GetDefaultCounterInstance()
	{
		if (stm_pInstance == nullptr)
			return nullptr;

		if (stm_pInstance->m_DefaultCounter == nullptr)
		{
			auto entityUID = EntityUID(stm_pInstance->m_ServerID, Service::EntityTable->GenEntityID(EntityFaculty::Service));
			stm_pInstance->m_DefaultCounter = SharedPointerT<PerformanceCounterInstance>(new(GetSystemHeap()) PerformanceCounterInstance(Util::GetServiceName(), entityUID));
			stm_pInstance->m_DefaultCounter->RegisterToClient();
		}

		return (PerformanceCounterInstance*)stm_pInstance->m_DefaultCounter;
	}

	// 
	Result PerformanceCounterClient::RegisterPerformanceCounterInstance(PerformanceCounterInstance *pInstance)
	{
		if (stm_pInstance == nullptr)
			return ResultCode::SUCCESS_FALSE;

		// This will gurantee that Dispose will be called in the same thread
		pInstance->AssignManager(stm_pInstance);

		return stm_pInstance->m_NewInstanceQueue.Enqueue(pInstance);

		//return std::forward<SharedPointerT<PerformanceCounterInstance>>(pInstance);
	}

	// Remove counter instance, this will be called by counter instance
	void PerformanceCounterClient::RemovePerformanceCounterInstance(PerformanceCounterInstance* pInstance)
	{
		if (stm_pInstance == nullptr)
			return;

		stm_pInstance->m_FreeInstanceQueue.Enqueue(FreeInfo(pInstance));
	}


	Result PerformanceCounterClient::HandleMessageUpdateCounterInfoS2CEvt(const sockaddr_storage& remoteAddr, MessageDataPtr &&pMsg)
	{
		Result hr = ResultCode::SUCCESS;
		Message::Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt messageClass(std::forward<MessageDataPtr>(pMsg));
		WeakPointerT<PerformanceCounterInstance> pFound;
		SharedPointerT<PerformanceCounterInstance> pInstance;

		svrChk(messageClass.ParseMsg());

		if (!(m_CounterInstanceMap.Find(messageClass.GetInstanceUID(), pFound)))
		{
			svrTrace(Error, "PerforamnceCounter:{0}, {1}, counter already deleted1", remoteAddr, messageClass.GetInstanceUID());
			return hr;
		}

		pInstance = pFound.AsSharedPtr();

		if (pInstance != nullptr)
		{
			SendNewCounterPacket((PerformanceCounterInstance*)pInstance);
		}
		else
		{
			svrTrace(Error, "PerforamnceCounter:{0}, {1}, counter already deleted", remoteAddr, messageClass.GetInstanceUID());
		}

	Proc_End:

		return hr;
	}


};	// Svr
};	// namespace SF


