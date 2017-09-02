////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server ServerTaskEvent manager
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Thread/Thread.h"
#include "String/StrUtil.h"
#include "Task/ServerTaskEvent.h"


namespace SF {

	
	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ServerTaskEvent class -  interface for task operation
	//


	ServerTaskEvent::ServerTaskEvent(ServerTaskEvent&& src)
		: TaskPtr(src.TaskPtr)
		//, EventData(src.EventData)
	{
		EventData.pConnectionEvent = src.EventData.pConnectionEvent;
		EventData.MessageEvent.pConn = std::forward<WeakPointerT<Net::Connection>>(src.EventData.MessageEvent.pConn);
		EventData.MessageEvent.pMessage = src.EventData.MessageEvent.pMessage;
		EventData.pTransResultEvent = src.EventData.pTransResultEvent;

		if (src.EventType == EventTypes::CONNECTION_EVENT)
		{
			memcpy(DataBuffer, src.DataBuffer, sizeof(DataBuffer));
			EventData.pConnectionEvent = (Net::ConnectionEvent*)DataBuffer;
		}

		EventType.store(src.EventType.load(std::memory_order_relaxed), std::memory_order_release);
	}

	ServerTaskEvent::ServerTaskEvent(TickTask* pTickTask)
		: EventType(EventTypes::POKE_TICK_EVENT)
	{
		TaskPtr.FromSharedObject(pTickTask);
		EventData.Clear();
	}

	ServerTaskEvent::ServerTaskEvent(TickTask* pTickTask, const Net::ConnectionEvent& connectionEvent)
		: EventType(EventTypes::CONNECTION_EVENT)
	{
		TaskPtr.FromSharedObject(pTickTask);
		memcpy(DataBuffer, &connectionEvent, sizeof(connectionEvent));
		EventData.pConnectionEvent = (Net::ConnectionEvent*)DataBuffer;
	}

	ServerTaskEvent::ServerTaskEvent(TickTask* pTickTask, WeakPointerT<Net::Connection>&& pConn, Message::MessageData* pMsg)
		: EventType(EventTypes::PACKET_MESSAGE_EVENT)
	{
		TaskPtr.FromSharedObject(pTickTask);
		EventData.MessageEvent.pConn = std::forward<WeakPointerT<Net::Connection>>(pConn);
		EventData.MessageEvent.pMessage = pMsg;
	}

	ServerTaskEvent::ServerTaskEvent(EventTypes eventType, TickTask* pTickTask, WeakPointerT<Net::Connection>&& pConn) // send buffer message
		: EventType(eventType)
	{
		TaskPtr.FromSharedObject(pTickTask);
		EventData.MessageEvent.pConn = std::forward<WeakPointerT<Net::Connection>>(pConn);
		EventData.MessageEvent.pMessage = nullptr;
	}

	ServerTaskEvent::ServerTaskEvent(TickTask* pTickTask, Svr::TransactionResult* pTransRes)
		: EventType(EventTypes::TRANSRESULT_EVENT)
	{
		Assert(pTransRes != nullptr);
		TaskPtr.FromSharedObject(pTickTask);
		EventData.pTransResultEvent = pTransRes;
	}

	ServerTaskEvent::~ServerTaskEvent()
	{
		EventData.MessageEvent.pConn = WeakPointerT<Net::Connection>();
	}

	ServerTaskEvent& ServerTaskEvent::operator = (const ServerTaskEvent& src)
	{
		EventData.pConnectionEvent = src.EventData.pConnectionEvent;
		EventData.MessageEvent.pConn = src.EventData.MessageEvent.pConn;
		EventData.MessageEvent.pMessage = src.EventData.MessageEvent.pMessage;
		EventData.pTransResultEvent = src.EventData.pTransResultEvent;

		auto srcEventType = src.EventType.load(std::memory_order_relaxed);
		if (srcEventType == EventTypes::CONNECTION_EVENT)
		{
			memcpy(DataBuffer, src.DataBuffer, sizeof(DataBuffer));
			EventData.pConnectionEvent = (Net::ConnectionEvent*)DataBuffer;
		}

		// This must be the last thing to do
		TaskPtr = src.TaskPtr;
		EventType.store(srcEventType, std::memory_order_release);

		return *this;
	}

	ServerTaskEvent& ServerTaskEvent::operator = (ServerTaskEvent&& src)
	{
		EventData.pConnectionEvent = src.EventData.pConnectionEvent;
		EventData.MessageEvent.pConn = std::forward<WeakPointerT<Net::Connection>>(src.EventData.MessageEvent.pConn);
		EventData.MessageEvent.pMessage = src.EventData.MessageEvent.pMessage;
		EventData.pTransResultEvent = src.EventData.pTransResultEvent;

		auto srcEventType = src.EventType.load(std::memory_order_relaxed);
		if (srcEventType == EventTypes::CONNECTION_EVENT)
		{
			memcpy(DataBuffer, src.DataBuffer, sizeof(DataBuffer));
			EventData.pConnectionEvent = (Net::ConnectionEvent*)DataBuffer;
		}

		// This must be the last thing to do
		TaskPtr = std::forward<WeakPointerT<TickTask>>(src.TaskPtr);
		EventType.store(srcEventType, std::memory_order_release);

		return *this;
	}



}; // namespace SF

