////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Remote Entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"
#include "String/SFStrUtil.h"
#include "Util/SFTimeUtil.h"
#include "ServerLog/SvrLog.h"
#include "Multithread/SFThread.h"
#include "Net/SFNetServerPeerTCP.h"
#include "SvrConst.h"
#include "ServerEntity/ServerEntity.h"
#include "Transaction/Transaction.h"
//#include "ServerSystem/PlugIn.h"
#include "Server/BrServer.h"
#include "SvrTrace.h"
#include "SvrConst.h"
#include "Task/ServerTaskEvent.h"
#include "Entity/EntityTable.h"
#include "Server/BrServerUtil.h"
#include "Entity/EntityManager.h"

#include "Protocol/Policy/ServerNetPolicy.h"

#include "Service/SFEngineServices.h"


namespace SF {
	namespace Svr {

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Entity base class
		//


		ServerEntity::ServerEntity(uint uiTransQueueSize, uint TransResQueueSize)
			: MasterEntity(uiTransQueueSize, TransResQueueSize)
			, m_RecvMessageQueue(GetHeap())
			, m_ServerID(0)
			, m_bIsInitialConnect(true)
			, m_ReceivedServerStatus(false)
			, m_ServerUpTime(UTCTimeStampSec::min())
		{
		}

		ServerEntity::~ServerEntity()
		{
		}


		// Initialize entity to proceed new connection
		Result ServerEntity::InitializeEntity(EntityID newEntityID)
		{
			Result hr = ResultCode::SUCCESS;

			svrCheck(MasterEntity::InitializeEntity(newEntityID));

			m_RecvMessageQueue.ClearQueue();

			return hr;
		}



		// Close entity and clear transaction
		Result ServerEntity::TerminateEntity()
		{
			Result hr = ResultCode::SUCCESS;

			if (GetEntityState() == EntityState::FREE)
				return ResultCode::SUCCESS;

			svrCheck(MasterEntity::TerminateEntity());

			return hr;
		}

		//// Called when this entity have a routed message
		//Result ServerEntity::OnRoutedMessage(MessageDataPtr &pMsg)
		//{
		//	// TODO: Call process message directly when it runs on the same thread
		//	Result hr = GetTaskManager()->AddEventTask(GetTaskGroupID(), ServerTaskEvent(this, WeakPointerT<Net::Connection>(), pMsg));
		//	if ((hr))
		//		pMsg = nullptr;

		//	return hr;
		//}

		// Process Message and release message after all processed
		Result ServerEntity::ProcessMessage(const SharedPointerT<MessageEndpoint>& remoteEndpoint, MessageDataPtr& pMsg)
		{
			RouteContext routeContext;
			TransactionID transID;
			SharedPointerT<Entity> pEntity;

			// First try to route message
			pMsg->GetRouteInfo(routeContext, transID);
			if (routeContext.GetTo() != GetEntityUID() && (Service::EntityTable->find(routeContext.GetTo(), pEntity)))
			{
				return pEntity->ProcessMessage(nullptr, pMsg);
			}

			if (routeContext.GetTo().UID != 0)
			{
				// TODO: we will not use this class, but when we do need to fix this
				//if (pMsg->GetMessageHeader()->msgID.IDs.Type == Message::MSGTYPE_COMMAND)
				//	NetSvrPolicyServer(SharedPointerT<Net::Connection>(pCon)).GenericFailureRes(routeContext.GetSwaped(), transID, ResultCode::SVR_INVALID_ENTITYUID);

				pMsg = nullptr;
				return ResultCode::SUCCESS_FALSE;
			}

			return super::ProcessMessage(remoteEndpoint, pMsg);
		}


		Result ServerEntity::TickUpdate(TimerAction* pAction)
		{
			Result hr = ResultCode::SUCCESS;
			Net::Connection* pConn = nullptr;
			Net::Connection* pConnRemote = nullptr;

			if (GetEntityState() == EntityState::FREE)
				return ResultCode::SUCCESS;

			svrCheck(MasterEntity::TickUpdate(pAction));
			return hr;
		}



		Result ServerEntity::OnEventTask(ServerTaskEvent& eventTask)
		{
			TransactionPtr pCurTran;
			MessageDataPtr pMsg;
			SharedPointerT<Net::Connection> pMyConn;

			switch (eventTask.EventType)
			{
			case ServerTaskEvent::EventTypes::CONNECTION_EVENT:
				//ProcessConnectionEvent(*eventTask.EventData.pConnectionEvent);
				break;
			case ServerTaskEvent::EventTypes::PACKET_MESSAGE_EVENT:
				pMsg = eventTask.EventData.MessageEvent.pMessage;
				if (pMsg != nullptr)
				{
					ProcessMessage(nullptr, pMsg);
				}
				else
				{
					svrTrace(Error, "null message pointer in event taqsk");
				}
				break;
			case ServerTaskEvent::EventTypes::PACKET_MESSAGE_SYNC_EVENT:
				//pMyConn = eventTask.EventData.MessageEvent.pConn.AsSharedPtr();
				//if (pMyConn != nullptr) pMyConn->UpdateSendQueue();
				break;
			case ServerTaskEvent::EventTypes::PACKET_MESSAGE_SEND_EVENT:
				//pMyConn = eventTask.EventData.MessageEvent.pConn.AsSharedPtr();
				//if (pMyConn != nullptr) pMyConn->UpdateSendBufferQueue();
				break;
			case ServerTaskEvent::EventTypes::TRANSRESULT_EVENT:
				if (eventTask.EventData.pTransResultEvent != nullptr)
				{
					if ((FindActiveTransaction(eventTask.EventData.pTransResultEvent->GetTransID(), pCurTran)))
					{
						SFUniquePtr<TransactionResult> pTransRes(eventTask.EventData.pTransResultEvent);
						eventTask.EventData.pTransResultEvent = nullptr;
						ProcessTransactionResult(pCurTran, pTransRes);
					}
					else
					{
						auto pNonConst = const_cast<TransactionResult*>(eventTask.EventData.pTransResultEvent);
						IHeap::Delete(pNonConst);
					}
				}
				else
				{
					svrTrace(SVR_TRANSACTION, "Failed to process transaction result. null Transaction result.");
				}
				break;
			default:
				return ResultCode::UNEXPECTED;
			}

			return ResultCode::SUCCESS;
		}



	} // namespace Svr
} // namespace SF

