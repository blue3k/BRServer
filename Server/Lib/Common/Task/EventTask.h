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
#include "Object/SharedPointer.h"
#include "Net/NetDef.h"
#include "Common/Task/Task.h"

namespace BR {


	class TaskManager;

	namespace Svr {
		class TransactionResult;

	}


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
			PACKET_MESSAGE_SYNC_EVENT,
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
			Svr::TransactionResult* pTransResultEvent;

			void Clear()
			{
				pConnectionEvent = nullptr;
				MessageEvent.pConn = WeakPointerT<Net::IConnection>();
				MessageEvent.pMessage = nullptr;
				pTransResultEvent = nullptr;
			}
		} EventData;

		EventTask();
		EventTask(EventTask&& src);
		EventTask(TickTask* pTickTask); // poke ticking
		EventTask(TickTask* pTickTask, const Net::IConnection::Event& connectionEvent);
		EventTask(TickTask* pTickTask, WeakPointerT<Net::IConnection>&& pConn, Message::MessageData* pMsg);
		EventTask(EventTypes eventType, TickTask* pTickTask, WeakPointerT<Net::IConnection>&& pConn); // send buffer message
		EventTask(TickTask* pTickTask, Svr::TransactionResult* pTransRes);
		~EventTask();

		EventTask& operator = (const EventTask& src);
		EventTask& operator = (EventTask&& src);

		EventTask& operator = (void* src);

		bool operator == (const EventTask& src) const;
		bool operator != (const EventTask& src) const;
	};




#include "EventTask.inl"


}; // namespace BR

