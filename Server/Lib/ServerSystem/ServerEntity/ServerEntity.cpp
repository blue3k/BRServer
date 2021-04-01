////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
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
#include "Net/SFNetServerPeerTCP.h"
#include "SvrConst.h"
#include "ServerEntity/ServerEntity.h"
#include "Transaction/Transaction.h"
//#include "ServerSystem/PlugIn.h"
#include "Server/BrServer.h"
#include "SvrTrace.h"
#include "SvrConst.h"
#include "Entity/EntityTable.h"
#include "Server/BrServerUtil.h"
#include "Entity/EntityManager.h"

#include "Protocol/ServerNetPolicy.h"

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
		Result ServerEntity::ProcessMessage(const SharedPointerT<MessageEndpoint>& remoteEndpoint, const MessageDataPtr& pMsg)
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


	} // namespace Svr
} // namespace SF

