////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Performance counter
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "Net/NetRawUDP.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/EntityTable.h"
#include "ServerSystem/EntityManager.h"
#include "Protocol/Message/MonitoringMsgClass.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterClient.h"





namespace BR {
namespace Svr {


	PerformanceCounterClient *PerformanceCounterClient::stm_pInstance = nullptr;

	Result PerformanceCounterClient::MessageHandler::OnRecv(const sockaddr_storage& remoteAddr, Message::MessageData *pMsg)
	{
		Result hr = ResultCode::SUCCESS;

		if (pMsg->GetMessageHeader()->msgID.GetMsgID() == Message::Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt::MID.GetMsgID())
		{
			svrChk(m_CounterClient.HandleMessageUpdateCounterInfoS2CEvt(remoteAddr, pMsg));
		}
		else
		{
			// 
			Util::SafeRelease(pMsg);
		}

	Proc_End:

		return hr;
	}


	PerformanceCounterClient::PerformanceCounterClient()
		: m_RawUDP(nullptr)
		, m_MessageHandler(this)
		, m_ServerID(0)
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

		Message::MessageData * pMsg = nullptr;

		StaticArray<PerformanceCounterInfo, 100> counterInfos;
		auto& counters = newInstance->GetCounters();
		for (UINT iCounter = 0; iCounter < counters.GetSize(); iCounter++)
		{
			auto counter = counters[iCounter];
			PerformanceCounterInfo info;
			memset(&info, 0, sizeof info);
			StrUtil::StringCpy(info.CounterName, counter->GetCounterName());
			info.DateType = (UINT)counter->GetDataType();
			if (FAILED(counterInfos.push_back(info)))
				break;
		}

		if (FAILED(Message::Monitoring::PerformanceCounterNewC2SEvt::BuildIMsg(pMsg, 
			newInstance->GetInstanceName(), newInstance->GetInstanceEntityUID(), counterInfos)))
		{
			svrTrace(Trace::TRC_ERROR, "Failed to generate performance counter free packet");
		}
		else
		{
			m_RawUDP->SendMsg(m_RemoteSockAddress, pMsg);
		}
	}

	void PerformanceCounterClient::UpdateNewInstance(ULONG newCount)
	{
		SharedPointerT<PerformanceCounterInstance> newInstance;
		for (UINT iItem = 0; iItem < newCount && SUCCEEDED(m_NewInstanceQueue.Dequeue(newInstance)); iItem++)
		{
			if (newInstance == nullptr)
				continue;

			if (FAILED(m_CounterInstanceMap.Insert(newInstance->GetInstanceEntityUID().UID, newInstance)))
			{
				svrTrace(Trace::TRC_ERROR, "Failed to insert new performance counter instance");
				continue;
			}

			SendNewCounterPacket((PerformanceCounterInstance*)newInstance);
		}


	}

	void PerformanceCounterClient::UpdateFreeInstance()
	{
		FreeInfo freeInfo;
		Message::MessageData * pMsg = nullptr;

		auto newCount = m_NewInstanceQueue.GetEnqueCount();

		std::atomic_thread_fence(std::memory_order_seq_cst);

		auto freeCount = m_FreeInstanceQueue.GetEnqueCount();

		StaticArray<EntityUID, 10> freeList;
		for (UINT iItem = 0; iItem < freeCount && SUCCEEDED(m_FreeInstanceQueue.Dequeue(freeInfo)); iItem++)
		{
			Assert(freeInfo.pInstance);
			if (freeInfo.pInstance == nullptr)
				continue;

			WeakPointerT<PerformanceCounterInstance> removed;
			if (SUCCEEDED(m_CounterInstanceMap.Remove(freeInfo.UID.UID, removed)))
			{
				freeList.push_back(freeInfo.UID);

				if (freeList.GetSize() >= freeList.GetAllocatedSize() && m_RawUDP != nullptr)
				{
					if (FAILED(Message::Monitoring::PerformanceCounterFreeC2SEvt::BuildIMsg(pMsg, freeList)))
					{
						svrTrace(Trace::TRC_ERROR, "Failed to generate performance counter free packet");
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

		if (freeList.GetSize() > 0)
		{
			if (FAILED(Message::Monitoring::PerformanceCounterFreeC2SEvt::BuildIMsg(pMsg, freeList)))
			{
				svrTrace(Trace::TRC_ERROR, "Failed to generate performance counter free packet");
			}
			else
			{
				m_RawUDP->SendMsg(m_RemoteSockAddress, pMsg);
			}
		}

		UpdateNewInstance((ULONG)newCount);
	}


	void PerformanceCounterClient::UpdateValues()
	{
		if (m_RawUDP == nullptr)
			return;

		m_CounterInstanceMap.CommitChanges();
		m_CounterInstanceMap.ForeachOrder(0, (UINT)m_CounterInstanceMap.GetItemCount(), [&](const intptr_t& key, const WeakPointerT<PerformanceCounterInstance>& value)
		{
			//auto pNode = (PerformanceCounterInstance::CounterListNode*)*itCounter;
			//if (pNode == nullptr)
			//	continue;

			SharedPointerT<PerformanceCounterInstance> pInstance;
			//pNode->CounterPointer.GetSharedPointer(pInstance);
			value.GetSharedPointer(pInstance);
			if (pInstance == nullptr)
				return true;

			Message::MessageData * pMsg = nullptr;

			StaticArray<UINT64, 100> counterValues;
			auto& counters = pInstance->GetCounters();
			for (UINT iCounter = 0; iCounter < counters.GetSize(); iCounter++)
			{
				UINT64 value = 0;
				if (FAILED(counters[iCounter]->CopyTo((UINT)sizeof(value), (BYTE*)&value)))
				{
					Assert(false);
					break;
				}

				if (FAILED(counterValues.push_back(value)))
					break;
			}

			if (FAILED(Message::Monitoring::PerformanceCounterUpdateC2SEvt::BuildIMsg(pMsg, 
				pInstance->GetInstanceEntityUID(), counterValues)))
			{
				svrTrace(Trace::TRC_ERROR, "Failed to generate performance counter free packet");
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

	Result PerformanceCounterClient::Initialize(UINT serverID, const NetAddress& serverAddress)
	{
		Result hr = ResultCode::SUCCESS;
		NetAddress localAddress;
		auto pRawUDP = new Net::RawUDP();
		svrChkPtr(pRawUDP);

		stm_pInstance = new PerformanceCounterClient();

		Net::Addr2SockAddr(serverAddress, stm_pInstance->m_RemoteSockAddress);
		stm_pInstance->m_RemoteAddress = serverAddress;

		stm_pInstance->m_ServerID = serverID;

		// use same network family
		svrChk(Net::GetLocalAddress(serverAddress.SocketFamily, localAddress));

		svrChk(pRawUDP->InitializeNet(localAddress, &stm_pInstance->m_MessageHandler));

		stm_pInstance->m_RawUDP = pRawUDP;
		pRawUDP = nullptr;

		stm_pInstance->Start();

	Proc_End:

		if (pRawUDP != nullptr)
		{
			pRawUDP->TerminateNet();
			Util::SafeDelete(pRawUDP);
		}

		if (FAILED(hr))
			Terminate();

		return hr;
	}

	Result PerformanceCounterClient::Terminate()
	{
		if (stm_pInstance == nullptr)
			return ResultCode::SUCCESS;

		stm_pInstance->Stop(true);

		delete stm_pInstance;
		stm_pInstance = nullptr;

		return ResultCode::SUCCESS;
	}

	PerformanceCounterInstance* PerformanceCounterClient::GetDefaultCounterInstance()
	{
		auto& entityTable = GetEntityTable();

		if (stm_pInstance == nullptr)
		{
			Assert(false);
			return nullptr;
		}

		if (stm_pInstance->m_DefaultCounter == nullptr)
		{
			auto entityUID = EntityUID(stm_pInstance->m_ServerID, entityTable.GenEntityID(EntityFaculty::Service));
			stm_pInstance->m_DefaultCounter = SharedPointerT<PerformanceCounterInstance>(new PerformanceCounterInstance(Util::GetServiceNameA(), entityUID));
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


	Result PerformanceCounterClient::HandleMessageUpdateCounterInfoS2CEvt(const sockaddr_storage& remoteAddr, Message::MessageData* &pMsg)
	{
		Result hr = ResultCode::SUCCESS;
		Message::Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt messageClass(pMsg);
		WeakPointerT<PerformanceCounterInstance> pFound;
		SharedPointerT<PerformanceCounterInstance> pInstance;

		svrChk(messageClass.ParseMsg());

		if (FAILED(m_CounterInstanceMap.Find(messageClass.GetInstanceUID().UID, pFound)))
		{
			svrTrace(Trace::TRC_ERROR, "PerforamnceCounter:{0}, {1}, counter already deleted1", remoteAddr, messageClass.GetInstanceUID());
			return hr;
		}

		pFound.GetSharedPointer(pInstance);

		if (pInstance != nullptr)
		{
			SendNewCounterPacket((PerformanceCounterInstance*)pInstance);
		}
		else
		{
			svrTrace(Trace::TRC_ERROR, "PerforamnceCounter:{0}, {1}, counter already deleted", remoteAddr, messageClass.GetInstanceUID());
		}

	Proc_End:

		return hr;
	}


};	// Svr
};	// namespace BR


