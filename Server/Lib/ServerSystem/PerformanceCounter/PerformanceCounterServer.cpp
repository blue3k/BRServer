////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Performance counter Server
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/TimeUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "Net/NetRawUDP.h"
#include "Protocol/Message/MonitoringMsgClass.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounter.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterServer.h"





namespace BR {
namespace Svr {

#define	BR_MESSAGE_HANDLER(MessageType,MessageHandlerName) m_HandlerTable.Register<Message::Monitoring::MessageType>(__FILE__,__LINE__,&PerformanceCounterServer::HandleMessage##MessageHandlerName);

	PerformanceCounterServer *PerformanceCounterServer::stm_pInstance = nullptr;


	PerformanceCounterServer::MessageHandler::MessageHandler(PerformanceCounterServer *CounterServer)
		: m_CounterServer(*CounterServer)
		, m_HandlerTable(STDAllocator::GetInstance())
	{
		BR_MESSAGE_HANDLER(PerformanceCounterNewC2SEvt, Enqueue);
		BR_MESSAGE_HANDLER(PerformanceCounterFreeC2SEvt, Enqueue);
		BR_MESSAGE_HANDLER(PerformanceCounterUpdateC2SEvt, PerformanceCounterUpdateC2SEvt);
	}

	Result PerformanceCounterServer::MessageHandler::OnRecv(const sockaddr_storage& remoteAddr, Message::MessageData *pMsg)
	{
		Result hr = ResultCode::SUCCESS;
		MessageHandlerType handler;
		svrChkPtr(pMsg);

		svrChk(m_HandlerTable.GetHandler(pMsg->GetMessageHeader()->msgID, handler));

		((&m_CounterServer)->*handler)(remoteAddr, pMsg);

	Proc_End:

		return hr;
	}

	PerformanceCounterServer::PerformanceCounterServer()
		: m_RawUDP(nullptr)
		, m_MessageHandler(this)
	{

	}

	PerformanceCounterServer::~PerformanceCounterServer()
	{
		if (m_RawUDP != nullptr)
		{
			m_RawUDP->TerminateNet();
			Util::SafeDelete(m_RawUDP);
		}
	}

	void PerformanceCounterServer::UpdateNewFreeInstance()
	{
		PacketInfo packetInfo;
		auto numItems = m_NewDeleteQueue.GetEnqueCount();
		for (UINT iItem = 0; iItem < numItems&& (m_NewDeleteQueue.Dequeue(packetInfo)); iItem++)
		{
			if (packetInfo.pMessage->GetMessageHeader()->msgID.IDSeq.MsgID == Message::Monitoring::PerformanceCounterNewC2SEvt::MID.IDSeq.MsgID)
			{
				HandleMessagePerformanceCounterNewC2SEvt(packetInfo.From, packetInfo.pMessage);
			}
			else if (packetInfo.pMessage->GetMessageHeader()->msgID.IDSeq.MsgID == Message::Monitoring::PerformanceCounterFreeC2SEvt::MID.IDSeq.MsgID)
			{
				HandleMessagePerformanceCounterFreeC2SEvt(packetInfo.From, packetInfo.pMessage);
			}
			else
			{
				// not handled message
				Assert(false);
			}
		}
	}

	void PerformanceCounterServer::UpdateValues()
	{
		PacketInfo packetInfo;
		auto numItems = m_UpdateQueue.GetEnqueCount();
		for (UINT iItem = 0; iItem < numItems&& (m_UpdateQueue.Dequeue(packetInfo)); iItem++)
		{
			if (packetInfo.pMessage->GetMessageHeader()->msgID.IDSeq.MsgID == Message::Monitoring::PerformanceCounterUpdateC2SEvt::MID.IDSeq.MsgID)
			{
				HandleMessagePerformanceCounterUpdateC2SEvt(packetInfo.From, packetInfo.pMessage);
			}
			else
			{
				// not handled message
				Assert(false);
			}
		}
	}

	void PerformanceCounterServer::Run()
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

			UpdateNewFreeInstance();

			void* item;
			while ((m_TimedOutQueue.Dequeue(item)))
			{
				SharedPointerT<PerformanceCounterInstance> removed;
				m_InstanceMap.Remove((UINT64)item, removed);
			}
			
			m_InstanceMap.CommitChanges();

			UpdateValues();
		}
	}

	Result PerformanceCounterServer::Initialize(const char* serverAddress, UINT port)
	{
		Result hr = ResultCode::SUCCESS;
		NetAddress localAddr;
		if (serverAddress == nullptr) serverAddress = "";
		if (port > USHRT_MAX) port = 0;

		// validate local IP
		svrChk(Net::SetLocalNetAddress(localAddr, serverAddress, port));

		svrChk(Initialize(localAddr));

	Proc_End:

		return hr;
	}

	Result PerformanceCounterServer::Initialize(const NetAddress& serverAddress)
	{
		Result hr = ResultCode::SUCCESS;
		NetAddress localAddress;
		auto pRawUDP = new Net::RawUDP();
		svrChkPtr(pRawUDP);

		stm_pInstance = new PerformanceCounterServer();

		svrChk(pRawUDP->InitializeNet(serverAddress, &stm_pInstance->m_MessageHandler));

		stm_pInstance->m_RawUDP = pRawUDP;
		pRawUDP = nullptr;

		stm_pInstance->Start();

	Proc_End:

		if (pRawUDP != nullptr)
		{
			pRawUDP->TerminateNet();
			Util::SafeDelete(pRawUDP);
		}

		if (!(hr))
			Terminate();

		return hr;
	}

	Result PerformanceCounterServer::Terminate()
	{
		if (stm_pInstance == nullptr)
			return ResultCode::SUCCESS;

		stm_pInstance->Stop(true);

		delete stm_pInstance;
		stm_pInstance = nullptr;

		return ResultCode::SUCCESS;
	}

	UINT PerformanceCounterServer::GetInstanceCount()
	{
		if (stm_pInstance == nullptr)
			return 0;

		return (UINT)stm_pInstance->m_InstanceMap.GetItemCount();
	}

	UINT PerformanceCounterServer::GetInstanceList(UINT startIndex, Array<SharedPointerT<PerformanceCounterInstance>>& instanceList)
	{
		//Result hr = ResultCode::SUCCESS;
		UINT numInstances = 0;
		if (stm_pInstance == nullptr)
			return numInstances;

		auto& instanceMap = stm_pInstance->m_InstanceMap;

		instanceMap.ForeachOrder(0, (UINT)instanceMap.GetItemCount(), [&instanceMap,&instanceList](const UINT64& key, const SharedPointerT<PerformanceCounterInstance>& value)
		{
			auto timeSince = Util::TimeSince(value->GetUpdatedTime());
			if (timeSince <= DurationMS(0) || timeSince > DurationMS(TIMER_TIMOUT))
			{
				stm_pInstance->m_TimedOutQueue.Enqueue((void*)key);
				return true;
			}

			if (!(instanceList.push_back(const_cast<SharedPointerT<PerformanceCounterInstance>&>(value))))
				return false;
			return true;
		});

	//Proc_End:

		return (UINT)instanceList.GetSize();
	}
/*
	UINT PerformanceCounterServer::GetInstanceList(UINT startIndex, UINT bufferSize, PerformanceCounterInstance** pInstanceBuffer)
	{
		Result hr = ResultCode::SUCCESS;
		UINT numInstances = 0;
		if (stm_pInstance == nullptr)
			return numInstances;

		if (pInstanceBuffer == nullptr)
			return 0;

		auto& instanceMap = stm_pInstance->m_InstanceMap;

		instanceMap.ForeachOrder(0, (UINT)instanceMap.GetItemCount(), [bufferSize, &pInstanceBuffer, &numInstances](const UINT64& key, const SharedPointerT<PerformanceCounterInstance>& value)
		{
			pInstanceBuffer[numInstances++] = (PerformanceCounterInstance*)const_cast<SharedPointerT<PerformanceCounterInstance>&>(value);

			if (numInstances >= bufferSize)
				return false;
			return true;
		});

	Proc_End:

		return numInstances;
	}
*/

	Result PerformanceCounterServer::GetInstance(UINT64 instanceUID, SharedPointerT<PerformanceCounterInstance>& pInstance)
	{
		return GetInstance(EntityUID(instanceUID), pInstance);
	}

	Result PerformanceCounterServer::GetInstance(EntityUID instanceUID, SharedPointerT<PerformanceCounterInstance>& pInstance)
	{
		Result hr = ResultCode::SUCCESS;
		auto& instanceMap = stm_pInstance->m_InstanceMap;

		if ((instanceMap.Find(instanceUID.UID, pInstance)))
		{
			auto timeSince = Util::TimeSince(pInstance->GetUpdatedTime());
			if (timeSince <= DurationMS(0) || timeSince > DurationMS(TIMER_TIMOUT))
			{
				stm_pInstance->m_TimedOutQueue.Enqueue((void*)instanceUID.UID);
				pInstance = SharedPointerT<PerformanceCounterInstance>();
				hr = ResultCode::FAIL;
				goto Proc_End;
			}
		}
		else
		{
			hr = ResultCode::FAIL;
		}

	Proc_End:

		return hr;
	}

	Result PerformanceCounterServer::HandleMessageEnqueue(const sockaddr_storage& remoteAddr, Message::MessageData* &pMsg)
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(m_NewDeleteQueue.Enqueue(PacketInfo(remoteAddr,pMsg)));
		pMsg = nullptr;

	Proc_End:

		Util::SafeRelease(pMsg);

		return hr;
	}

	Result PerformanceCounterServer::HandleMessageEnqueueUpdate(const sockaddr_storage& remoteAddr, Message::MessageData* &pMsg)
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(m_UpdateQueue.Enqueue(PacketInfo(remoteAddr, pMsg)));
		pMsg = nullptr;

	Proc_End:

		Util::SafeRelease(pMsg);

		return hr;
	}

	Result PerformanceCounterServer::HandleMessagePerformanceCounterNewC2SEvt(const sockaddr_storage& remoteAddr, Message::MessageData* &pMsg)
	{
		Result hr = ResultCode::SUCCESS;
		Message::Monitoring::PerformanceCounterNewC2SEvt messageClass(pMsg);
		SharedPointerT<PerformanceCounterInstance> pInstance;

		svrChk(messageClass.ParseMsg());

		// remove if exist
		m_InstanceMap.Remove(messageClass.GetInstanceUID().UID, pInstance);

		pInstance = SharedPointerT<PerformanceCounterInstance>(new PerformanceCounterInstance(messageClass.GetInstanceName(), messageClass.GetInstanceUID()));
		if (!(m_InstanceMap.Insert(pInstance->GetInstanceEntityUID().UID, pInstance)))
		{
			if (!(m_InstanceMap.FindInWriteTree(pInstance->GetInstanceEntityUID().UID, pInstance)))
			{
				svrTrace(Trace::TRC_ERROR, "PerforamnceCounter:{0}, Create/Find Both failed?", pInstance->GetInstanceEntityUID());
			}
			else
			{
				// we already have it
			}
			return hr;
		}

		pInstance->SetUpdatedTime(Util::Time.GetTimeMs());

		{
			auto& counters = messageClass.GetNewCounters();
			for (UINT iCounter = 0; iCounter < counters.GetSize(); iCounter++)
			{
				auto dataType = (PerformanceCounter::DataTypes)counters[iCounter].DateType;
				switch (dataType)
				{
				case PerformanceCounter::DataTypes::Int32:
					pInstance->AddCounter(new PerformanceCounterRaw<INT32>(counters[iCounter].CounterName));
					break;
				case PerformanceCounter::DataTypes::UInt32:
					pInstance->AddCounter(new PerformanceCounterRaw<UINT32>(counters[iCounter].CounterName));
					break;
				case PerformanceCounter::DataTypes::Int64:
					pInstance->AddCounter(new PerformanceCounterRaw<INT64>(counters[iCounter].CounterName));
					break;
				case PerformanceCounter::DataTypes::UInt64:
					pInstance->AddCounter(new PerformanceCounterRaw<UINT64>(counters[iCounter].CounterName));
					break;
				default:
					svrTrace(Trace::TRC_ERROR, "PerforamnceCounter:{0}, Invalid counter type:{1}", pInstance->GetInstanceEntityUID(), counters[iCounter].DateType);
					break;
				}
			}

		}

	Proc_End:

		return hr;
	}

	Result PerformanceCounterServer::HandleMessagePerformanceCounterFreeC2SEvt(const sockaddr_storage& remoteAddr, Message::MessageData* &pMsg)
	{
		Result hr = ResultCode::SUCCESS;
		Message::Monitoring::PerformanceCounterFreeC2SEvt messageClass(pMsg);

		svrChk(messageClass.ParseMsg());

		{
			auto& instances = messageClass.GetFreeInstances();
			for (UINT iInstance = 0; iInstance < instances.GetSize(); iInstance++)
			{
				SharedPointerT<PerformanceCounterInstance> pInstance;
				if (!(m_InstanceMap.Remove(instances[iInstance].UID, pInstance)))
				{
					svrTrace(Trace::TRC_ERROR, "PerforamnceCounter:{0}, Failed to remove", instances[iInstance]);
				}
			}
		}

	Proc_End:

		return hr;
	}

	Result PerformanceCounterServer::HandleMessagePerformanceCounterUpdateC2SEvt(const sockaddr_storage& remoteAddr, Message::MessageData* &pMsg)
	{
		Result hr = ResultCode::SUCCESS;
		Message::Monitoring::PerformanceCounterUpdateC2SEvt messageClass(pMsg);
		SharedPointerT<PerformanceCounterInstance> pInstance;

		svrChk(messageClass.ParseMsg());

		if (!(m_InstanceMap.Find(messageClass.GetInstanceUID().UID, pInstance))
			|| pInstance->GetCounters().GetSize() != messageClass.GetCounterValues().GetSize())
		{
			if (m_RawUDP != nullptr)
			{
				Message::MessageData* pMsgSend = nullptr;
				if (!(Message::Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt::BuildIMsg(pMsgSend, messageClass.GetInstanceUID())))
				{
					svrTrace(Trace::TRC_ERROR, "Failed to generate performance counter update request packet");
				}
				else
				{
					m_RawUDP->SendMsg(remoteAddr, pMsgSend);
				}
			}

			return hr;
		}

		pInstance->SetUpdatedTime(Util::Time.GetTimeMs());

		{
			auto& counters = pInstance->GetCounters();
			auto& conterValues = messageClass.GetCounterValues();
			for (UINT iCounter = 0; iCounter < conterValues.GetSize(); iCounter++)
			{
				// NOTE: assume that is LSB
				counters[iCounter]->CopyFrom(sizeof conterValues[iCounter], (BYTE*)&conterValues[iCounter]);
			}
		}

	Proc_End:

		return hr;
	}

};	// Svr
};	// namespace BR


