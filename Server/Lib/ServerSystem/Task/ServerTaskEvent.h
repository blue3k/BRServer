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

#include "SFTypedefs.h"
#include "Net/Message.h"
#include "Object/SharedPointer.h"
#include "Net/NetDef.h"
#include "Task/ServerTask.h"

namespace SF {


	class TickTaskManager;

	namespace Svr {
		class TransactionResult;

	}


	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	EventItem -  interface for task operation
	//

	struct ServerTaskEvent
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

		BYTE					DataBuffer[sizeof(Net::ConnectionEvent)];
		struct
		{
			Net::ConnectionEvent *pConnectionEvent;
			struct {
				WeakPointerT<Net::Connection> pConn;
				MessageDataPtr pMessage;
			} MessageEvent;
			Svr::TransactionResult* pTransResultEvent;

			void Clear()
			{
				pConnectionEvent = nullptr;
				MessageEvent.pConn = WeakPointerT<Net::Connection>();
				MessageEvent.pMessage = nullptr;
				pTransResultEvent = nullptr;
			}
		} EventData;

		ServerTaskEvent();
		ServerTaskEvent(ServerTaskEvent&& src);
		ServerTaskEvent(TickTask* pTickTask); // poke ticking
		ServerTaskEvent(TickTask* pTickTask, const Net::ConnectionEvent& connectionEvent);
		ServerTaskEvent(TickTask* pTickTask, WeakPointerT<Net::Connection>&& pConn, SharedPointerT<Message::MessageData>& pMsg);
		ServerTaskEvent(EventTypes eventType, TickTask* pTickTask, WeakPointerT<Net::Connection>&& pConn); // send buffer message
		ServerTaskEvent(TickTask* pTickTask, Svr::TransactionResult* pTransRes);
		~ServerTaskEvent();

		ServerTaskEvent& operator = (const ServerTaskEvent& src);
		ServerTaskEvent& operator = (ServerTaskEvent&& src);

		ServerTaskEvent& operator = (void* src);

		bool operator == (const ServerTaskEvent& src) const;
		bool operator != (const ServerTaskEvent& src) const;
	};




#include "ServerTaskEvent.inl"


}; // namespace SF

