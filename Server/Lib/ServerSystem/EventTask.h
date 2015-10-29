////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server TickTask manager
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/Message.h"
#include "Common/SharedPointer.h"
#include "Net/Connection.h"
#include "ServerSystem/Task.h"

namespace BR {
namespace Svr
{
	class TaskManager;
	class TransactionResult;


	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	EventItem -  interface for task operation
	//

	struct EventTask
	{
		enum class EventTypes : UINT32
		{
			NONE,
			POKE_TICK_EVENT,
			PACKET_MESSAGE_EVENT,
			PACKET_MESSAGE_SEND_EVENT,
			CONNECTION_EVENT,
			TRANSRESULT_EVENT,
		};

		std::atomic<EventTypes> EventType;
		WeakPointerT<TickTask>	TaskPtr;

		BYTE					DataBuffer[sizeof(Net::IConnection::Event)];
		struct
		{
			Net::IConnection::Event *pConnectionEvent;
			struct {
				WeakPointerT<Net::IConnection> pConn;
				Message::MessageData* pMessage;
			} MessageEvent;
			TransactionResult* pTransResultEvent;
		} EventData;

		EventTask();
		EventTask(EventTask&& src);
		EventTask(TickTask* pTickTask); // poke ticking
		EventTask(TickTask* pTickTask, const Net::IConnection::Event& connectionEvent);
		EventTask(TickTask* pTickTask, WeakPointerT<Net::IConnection>&& pConn, Message::MessageData* pMsg);
		EventTask(TickTask* pTickTask, WeakPointerT<Net::IConnection>&& pConn); // send message
		EventTask(TickTask* pTickTask, TransactionResult* pTransRes);
		~EventTask();

		EventTask& operator = (const EventTask& src);
		EventTask& operator = (EventTask&& src);

		EventTask& operator = (void* src);

		bool operator == (const EventTask& src) const;
		bool operator != (const EventTask& src) const;
	};




#include "EventTask.inl"

}; // namespace Svr
}; // namespace BR

