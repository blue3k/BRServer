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
#include "Util/SFStrUtil.h"
#include "Util/SFTimeUtil.h"
#include "ServerLog/SvrLog.h"
#include "Multithread/SFThread.h"
#include "Util/SFUtility.h"
#include "SvrConst.h"
#include "Entity/SimpleUserEntity.h"
#include "Transaction/Transaction.h"
#include "Server/BrServer.h"
#include "SvrTrace.h"
#include "Net/SFConnectionUDP.h"

#include "Protocol/LoginMsgClass.h"
#include "Protocol/GameMsgClass.h"


namespace SF {
namespace Svr
{

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity base class
	//


	SimpleUserEntity::SimpleUserEntity()
		: SimpleEntity(Const::ENTITY_SIMPLE_TRANS_QUEUE, Const::ENTITY_SIMPLE_TRANSRES_QUEUE)
		, m_pConnection(nullptr)
		, m_ComponentManger(GetHeap())
	{
		SetTickInterval(DurationMS(Svr::Const::SIMPLEUSER_TICKTASK_INTERVAL));

		RegisterMessageHandler<Message::Login::HeartbeatC2SEvt>([this](const MessageDataPtr&, TransactionPtr& pNewTrans) -> Result
			{
				pNewTrans = nullptr;
				Heartbeat();
				return ResultCode::SUCCESS;
			});

		RegisterMessageHandler<Message::Game::HeartbeatC2SEvt>([this](const MessageDataPtr&, TransactionPtr& pNewTrans) -> Result
			{
				pNewTrans = nullptr;
				Heartbeat();
				return ResultCode::SUCCESS;
			});
	}

	SimpleUserEntity::~SimpleUserEntity()
	{
	}

	void SimpleUserEntity::SetEntityKillTimer(DurationMS timeOut)
	{
		m_TimeToKill.SetTimer(timeOut);
	}

	// set connection
	Result SimpleUserEntity::SetConnection(SharedPointerT<Net::Connection>&& pConn)
	{
		MutexScopeLock localLock(m_ConnectionLock);

		m_pConnection = Forward<SharedPointerT<Net::Connection>>(pConn);
		if (m_pConnection != nullptr)
		{
			m_pConnection->SetEventHandler(this); // TODO: Handle Net ready

			WeakPointerT<SimpleUserEntity> pThisWeak = AsSharedPtr<SimpleUserEntity>();
			m_pConnection->GetRecvMessageDelegates().AddDelegateUnique(uintptr_t(this), [pThisWeak, pTaskManager = GetTaskManager(), TaskGroupId = GetTaskGroupID()](Net::Connection* pConn, const SharedPointerT<Message::MessageData>& pMsg)
				{
					pTaskManager->RunOnTaskThread(TaskGroupId, [pThisWeak, pMsg = pMsg, pEndpoint = pConn->GetMessageEndpoint()]()
					{
						auto pThis = pThisWeak.AsSharedPtr<SimpleUserEntity>();
						if (pThis != nullptr)
						{
							pThis->ProcessMessage(pEndpoint, pMsg);
						}
					});
				});

			m_pConnection->GetNetSyncMessageDelegates().AddDelegateUnique(uintptr_t(this), [pTaskManager = GetTaskManager(), TaskGroupId = GetTaskGroupID()](Net::Connection* pConn)
			{
				pTaskManager->RunOnTaskThread(TaskGroupId, [pConn = pConn->AsSharedPtr<Net::Connection>()]()
				{
					if (pConn != nullptr)
					{
						pConn->UpdateSendQueue();
					}
				});
			});

			m_pConnection->GetConnectionEventDelegates().AddDelegateUnique(uintptr_t(this), [pThisWeak, pTaskManager = GetTaskManager(), TaskGroupId = GetTaskGroupID()](Net::Connection* pConn, const ConnectionEvent& EventData)
			{
				pTaskManager->RunOnTaskThread(TaskGroupId, [pThisWeak, EventData = EventData]()
				{
					auto pThis = pThisWeak.AsSharedPtr<SimpleUserEntity>();
					if (pThis != nullptr)
					{
						pThis->ProcessConnectionEvent(EventData);
					}
				});
			});

			//// purge received guaranteed messages
			//MessageDataPtr temp;
			//OnRecvMessage((Net::Connection*)m_pConnection, temp);

			// This connection will be updated with User entity
			m_pConnection->SetTickGroup(EngineTaskTick::None);
		}

		return ResultCode::SUCCESS;
	}

	// Release connection if has
	void SimpleUserEntity::ReleaseConnection(const char* reason)
	{
		MutexScopeLock localLock(m_ConnectionLock);

		if( m_pConnection == nullptr )
			return;

		m_pConnection->GetRecvMessageDelegates().RemoveDelegateAll();
		m_pConnection->SetEventHandler(nullptr);
		SharedPointerT<Net::Connection> temp(m_pConnection);
		Service::ConnectionManager->RemoveConnection(temp);
		m_pConnection->DisconnectNRelease(reason);
		m_pConnection.reset();
	}



	// Initialize entity to proceed new connection
	Result SimpleUserEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;

		SetAccountID( 0 );
		SetAuthTicket( 0 );

		svrCheck(SimpleEntity::InitializeEntity( newEntityID ) );

		return hr;
	}



	// Close entity and clear transaction
	Result SimpleUserEntity::TerminateEntity()
	{
		Result hr = ResultCode::SUCCESS;

		if( GetEntityState() == EntityState::FREE )
			return ResultCode::SUCCESS;

		SetAccountID( 0 );
		SetAuthTicket( 0 );

		if (m_pConnection != nullptr)
		{
			ReleaseConnection("Terminate user entity");
		}

		svrCheck(SimpleEntity::TerminateEntity() );

		return hr;
	}


	// Process Connection event
	Result SimpleUserEntity::ProcessConnectionEvent( const Net::ConnectionEvent& conEvent )
	{
		Result hr = ResultCode::SUCCESS;

		switch( conEvent.Components.EventType )
		{
		case Net::ConnectionEvent::EVT_CONNECTION_RESULT:
			break;
		case Net::ConnectionEvent::EVT_DISCONNECTED:
			PendingCloseTransaction("Connection disconnected");
			break;
		case Net::ConnectionEvent::EVT_STATE_CHANGE:
			break;
		default:
			break;
		};

		return hr;
	}

	// Process Message and release message after all processed
	Result SimpleUserEntity::ProcessMessage(const SharedPointerT<MessageEndpoint>& remoteEndpoint, const MessageDataPtr& pIMsg)
	{
		Result hr = ResultCode::SUCCESS;
		EntityID entityID; // entity ID to route
		Message::MessageHeader *pMsgHdr = nullptr;
		TransactionPtr pNewTrans;
		ThreadID currentThreadID = ThisThread::GetThreadID();

		svrChkPtr(pIMsg);
		pMsgHdr = pIMsg->GetMessageHeader();

		switch (pMsgHdr->msgID.IDs.Type)
		{
		case Message::MSGTYPE_RESULT:
			svrChk(ProcessMessageResult(pIMsg));
			goto Proc_End;

			break;
		case Message::MSGTYPE_COMMAND:
		case Message::MSGTYPE_EVENT:
		{
			//Assert(m_pHandlerTable);
			if (!(GetMessageHandlerTable().HandleMessage<TransactionPtr&>(pIMsg, pNewTrans)))
			{
				svrTrace(Error, "Failed to handle remote message Entity:{0}:{1}, MsgID:{2}", typeid(*this).name(), GetEntityID(), pMsgHdr->msgID);
				svrErr(ResultCode::SVR_NOTEXPECTED_MESSAGE);
			}
			break;
		}
		default:
			svrTrace(Error, "Not Processed Remote message Entity:{0}:{1}, MsgID:{2}", typeid(*this).name(), GetEntityID(), pMsgHdr->msgID);
			svrErr(ResultCode::SVR_NOTEXPECTED_MESSAGE);
			break;
		};


		if (pNewTrans != nullptr)
		{
			pNewTrans->SetRemoteEndpoint(remoteEndpoint);

			if (pNewTrans->GetOwnerEntity() == nullptr)
			{
				svrChk(pNewTrans->InitializeTransaction(this));
			}

			if (pNewTrans->IsDirectProcess())
			{
				Result hrRes = pNewTrans->StartTransaction();
				if (!pNewTrans->IsClosed())
				{
					pNewTrans->CloseTransaction(hrRes);
				}
			}
			else
			{
				Assert(pNewTrans->GetTransID().GetEntityID() == GetEntityID());
				svrChk(PendingTransaction(currentThreadID, pNewTrans));
			}
		}

	Proc_End:

		if (pNewTrans != nullptr && !pNewTrans->IsClosed())
		{
			pNewTrans->CloseTransaction(hr);
		}


		ReleaseTransaction(pNewTrans);

		return ResultCode::SUCCESS;

	}

	void SimpleUserEntity::Heartbeat()
	{
		m_TimeToKill.SetTimer(DurationMS(Const::LOGIN_TIME_WAIT_PLAYER_JOIN));
	}

	Result SimpleUserEntity::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;
		MessageDataPtr pIMsg;
		Net::ConnectionEvent conEvent;

		if( GetEntityState() == EntityState::FREE )
			return ResultCode::SUCCESS;

		if (m_TimeToKill.CheckTimer()
			&& GetEntityState() == EntityState::WORKING)
		{
			if (!m_PendingCloseHasCalled)
			{
				if (PendingCloseTransaction("Entity heartbeat timeout"))
					m_PendingCloseHasCalled = true;
			}
		}
		else
		{
			GetComponentManager().TickUpdate();
		}

		// Update connection
		// We need to make a copy here while m_pConnection can be changed on another thread
		auto& pConn = GetConnection();
		auto thisThreadID = ThisThread::GetThreadID();

		if (pConn != nullptr)
		{
			if (pConn->GetTickFlags() == 0 && pConn->GetRunningThreadID() != thisThreadID)
			{
				pConn->SetRunningThreadID(thisThreadID);
			}

			if(pConn->GetRunningThreadID() == thisThreadID)
				pConn->TickUpdate();
		}

		// We need to check m_pConnection again because the connection can be release during update
		if (pConn != nullptr)
		{
			// Process Connection event
			auto loopCount = pConn->GetConnectionEventCount();
			for (decltype(loopCount) iProc = 0; iProc < loopCount; iProc++)
			{
				if (!(pConn->DequeueConnectionEvent(conEvent)))
					break;

				ProcessConnectionEvent( conEvent );
			}


			if (pConn->GetConnectionState() != Net::ConnectionState::DISCONNECTED)
			{
				// Process message
				loopCount = pConn->GetRecvMessageCount();
				for( decltype(loopCount) iProc = 0; iProc < loopCount; iProc++ )
				{
					if (!(pConn->GetRecvMessage(pIMsg)))
						break;

					ProcessMessage(pConn->GetMessageEndpoint(), pIMsg);
				}
			}
			else
			{
				PendingCloseTransaction("Disconnected");
			}
		}
		else
		{
			PendingCloseTransaction("Null Connection maybe disconnected");
		}
 
		svrCheck(SimpleEntity::TickUpdate(pAction) );

		pIMsg = nullptr;

		return hr;
	}

	Result SimpleUserEntity::SetAccountID( AccountID accID )
	{
		m_AccountID = accID;
		return ResultCode::SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	// Overriding IConnectionEventHandler
	//void SimpleUserEntity::OnConnectionEvent(Net::Connection* pConn, const Net::ConnectionEvent& evt)
	//{
	//	Assert(pConn != nullptr);
	//	Assert(evt.Components.EventType != Net::ConnectionEvent::EventTypes::EVT_NONE);
	//	// this one is free to call on other thread
	//	ProcessConnectionEvent(evt);
	//}

	//Result SimpleUserEntity::OnRecvMessage(Net::Connection* pConn, MessageDataPtr& pMsg)
	//{
	//	WeakPointerT<SimpleUserEntity> pThisWeak = AsSharedPtr<SimpleUserEntity>();
	//	return GetTaskManager()->RunOnTaskThread(GetTaskGroupID(), [pThisWeak, pMsg = pMsg, pEndpoint = pConn->GetMessageEndpoint()]()
	//		{
	//			auto pThis = pThisWeak.AsSharedPtr<SimpleUserEntity>();
	//			if (pThis != nullptr)
	//			{
	//				pThis->ProcessMessage(pEndpoint, pMsg);
	//			}
	//		});
	//}

	//Result SimpleUserEntity::OnNetSyncMessage(Net::Connection* pConn)
	//{
	//	if (pConn == nullptr)
	//		return ResultCode::INVALID_POINTER;

	//	return GetTaskManager()->RunOnTaskThread(GetTaskGroupID(), [pConn = pConn->AsSharedPtr<Net::Connection>()]()
	//	{
	//		if (pConn != nullptr)
	//		{
	//			pConn->UpdateSendQueue();
	//		}
	//	});
	//}

	Result SimpleUserEntity::OnNetSendReadyMessage(Net::Connection* pConn)
	{
		if (pConn == nullptr)
			return ResultCode::INVALID_POINTER;

		return GetTaskManager()->RunOnTaskThread(GetTaskGroupID(), [pConn = pConn->AsSharedPtr<Net::Connection>()]()
		{
			if (pConn != nullptr)
			{
				pConn->UpdateSendBufferQueue();
			}
		});
	}


} // namespace Svr
} // namespace SF
