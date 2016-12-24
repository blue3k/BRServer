////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server EventTask manager
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Common/Thread.h"
#include "Common/StrUtil.h"
#include "Common/Task/EventTask.h"


namespace BR {

	
	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	EventTask class -  interface for task operation
	//


	EventTask::EventTask(EventTask&& src)
		: TaskPtr(src.TaskPtr)
		//, EventData(src.EventData)
	{
		EventData.pConnectionEvent = src.EventData.pConnectionEvent;
		EventData.MessageEvent.pConn = std::forward<WeakPointerT<Net::IConnection>>(src.EventData.MessageEvent.pConn);
		EventData.MessageEvent.pMessage = src.EventData.MessageEvent.pMessage;
		EventData.pTransResultEvent = src.EventData.pTransResultEvent;

		if (src.EventType == EventTypes::CONNECTION_EVENT)
		{
			memcpy(DataBuffer, src.DataBuffer, sizeof(DataBuffer));
			EventData.pConnectionEvent = (Net::IConnection::Event*)DataBuffer;
		}

		EventType.store(src.EventType.load(std::memory_order_relaxed), std::memory_order_release);
	}

	EventTask::EventTask(TickTask* pTickTask)
		: EventType(EventTypes::POKE_TICK_EVENT)
	{
		TaskPtr.FromSharedObject(pTickTask);
		EventData.Clear();
	}

	EventTask::EventTask(TickTask* pTickTask, const Net::IConnection::Event& connectionEvent)
		: EventType(EventTypes::CONNECTION_EVENT)
	{
		TaskPtr.FromSharedObject(pTickTask);
		memcpy(DataBuffer, &connectionEvent, sizeof(connectionEvent));
		EventData.pConnectionEvent = (Net::IConnection::Event*)DataBuffer;
	}

	EventTask::EventTask(TickTask* pTickTask, WeakPointerT<Net::IConnection>&& pConn, Message::MessageData* pMsg)
		: EventType(EventTypes::PACKET_MESSAGE_EVENT)
	{
		TaskPtr.FromSharedObject(pTickTask);
		EventData.MessageEvent.pConn = std::forward<WeakPointerT<Net::IConnection>>(pConn);
		EventData.MessageEvent.pMessage = pMsg;
	}

	EventTask::EventTask(EventTypes eventType, TickTask* pTickTask, WeakPointerT<Net::IConnection>&& pConn) // send buffer message
		: EventType(eventType)
	{
		TaskPtr.FromSharedObject(pTickTask);
		EventData.MessageEvent.pConn = std::forward<WeakPointerT<Net::IConnection>>(pConn);
		EventData.MessageEvent.pMessage = nullptr;
	}

	EventTask::EventTask(TickTask* pTickTask, Svr::TransactionResult* pTransRes)
		: EventType(EventTypes::TRANSRESULT_EVENT)
	{
		Assert(pTransRes != nullptr);
		TaskPtr.FromSharedObject(pTickTask);
		EventData.pTransResultEvent = pTransRes;
	}

	EventTask::~EventTask()
	{
		EventData.MessageEvent.pConn = WeakPointerT<Net::IConnection>();
	}

	EventTask& EventTask::operator = (const EventTask& src)
	{
		EventData.pConnectionEvent = src.EventData.pConnectionEvent;
		EventData.MessageEvent.pConn = src.EventData.MessageEvent.pConn;
		EventData.MessageEvent.pMessage = src.EventData.MessageEvent.pMessage;
		EventData.pTransResultEvent = src.EventData.pTransResultEvent;

		auto srcEventType = src.EventType.load(std::memory_order_relaxed);
		if (srcEventType == EventTypes::CONNECTION_EVENT)
		{
			memcpy(DataBuffer, src.DataBuffer, sizeof(DataBuffer));
			EventData.pConnectionEvent = (Net::IConnection::Event*)DataBuffer;
		}

		// This must be the last thing to do
		TaskPtr = src.TaskPtr;
		EventType.store(srcEventType, std::memory_order_release);

		return *this;
	}

	EventTask& EventTask::operator = (EventTask&& src)
	{
		EventData.pConnectionEvent = src.EventData.pConnectionEvent;
		EventData.MessageEvent.pConn = std::forward<WeakPointerT<Net::IConnection>>(src.EventData.MessageEvent.pConn);
		EventData.MessageEvent.pMessage = src.EventData.MessageEvent.pMessage;
		EventData.pTransResultEvent = src.EventData.pTransResultEvent;

		auto srcEventType = src.EventType.load(std::memory_order_relaxed);
		if (srcEventType == EventTypes::CONNECTION_EVENT)
		{
			memcpy(DataBuffer, src.DataBuffer, sizeof(DataBuffer));
			EventData.pConnectionEvent = (Net::IConnection::Event*)DataBuffer;
		}

		// This must be the last thing to do
		TaskPtr = std::forward<WeakPointerT<TickTask>>(src.TaskPtr);
		EventType.store(srcEventType, std::memory_order_release);

		return *this;
	}



}; // namespace BR

