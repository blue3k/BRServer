////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Remote Entity definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"
#include "Multithread/SFThread.h"
#include "Multithread/SFSystemSynchronization.h"
#include "Net/SFNetDef.h"
#include "Net/SFNetUtil.h"
#include "Net/SFConnection.h"
#include "Entity/MasterEntity.h"
#include "SvrConst.h"



namespace SF {
	namespace Svr {

		class Transaction;
		class TransactionResult;



		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Remote server entity class
		//		- Will use with server type
		//

		class ServerEntity : public MasterEntity
		{
		public:

			typedef NetClass ServerEntityClass;
			typedef MasterEntity super;

		private:

			SF::Net::MsgQueue		m_RecvMessageQueue;

			// ServerID
			ServerID				m_ServerID;

			// Initial connect
			bool					m_bIsInitialConnect;

			// received server status
			bool m_ReceivedServerStatus = false;

			// Event router address
			String m_EventRouterAddress;

			// Server up time in UTC
			UTCTimeStampSec m_ServerUpTime;

		protected:


		public:
			ServerEntity(uint uiTransQueueSize = Const::SERVER_TRANS_QUEUE, uint TransResQueueSize = Const::SERVER_TRANS_QUEUE);
			virtual ~ServerEntity();

			bool GetReceivedServerStatus() { return m_ReceivedServerStatus; }
			void SetReceivedServerStatus(bool value) { m_ReceivedServerStatus = value; }

			UTCTimeStampSec GetServerUpTime() { return m_ServerUpTime; }
			void SetServerUpTime(UTCTimeStampSec value) { m_ServerUpTime = value; }

			const String& GetEventRouterAddress() const { return m_EventRouterAddress; }
			void SetEventRouterAddress(const String& routeAddress) { m_EventRouterAddress = routeAddress; }

			// Get ServerID
			ServerID GetServerID() const { return m_ServerID; }
			void SetServerID(ServerID svrID) { m_ServerID = svrID; }

			// Get Owner Server
			//ServerEntity* GetOwnerServer();

			// Initialize entity to proceed new connection
			virtual Result InitializeEntity(EntityID newEntityID) override;

			// Close entity and clear transaction
			virtual Result TerminateEntity() override;


			// Process Message and release message after all processed
			virtual Result ProcessMessage(const SharedPointerT<MessageEndpoint>& remoteEndpoint, const MessageDataPtr& pMsg) override;

			// Run entity
			virtual Result TickUpdate(TimerAction* pAction = nullptr) override;


			/////////////////////////////////////////////////////////////////////////////////////
			// Event task handling
			virtual Result OnEventTask(ServerTaskEvent& eventTask) override;
		};



	} // namespace Svr
} // namespace SF


