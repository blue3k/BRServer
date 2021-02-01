////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : Simple User Entity definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Thread/SFThread.h"
#include "Thread/SFSystemSynchronization.h"
#include "Net/SFNetDef.h"
#include "Net/SFConnection.h"
#include "Entity/SimpleEntity.h"
#include "Component/BrComponent.h"


namespace SF {
	namespace Svr {

		class Transaction;
		class TransactionResult;


		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Simple User entity class
		//

		class SimpleUserEntity : public SimpleEntity, public Net::IConnectionEventHandler
		{
		public:

		private:
			// Connection to remote
			// To match with server connection pointer type, this should be atomic
			SharedPointerAtomicT<Net::Connection>	m_pConnection;
			CriticalSection m_ConnectionLock;

			// Account ID
			AccountID		m_AccountID;

			// Authenticate ticket
			AuthTicket		m_AuthTicket;

			// Component manager
			ComponentManager m_ComponentManger;

			bool m_PendingCloseHasCalled = false;

			// Time for kill this instance
			Util::TimeStampTimer m_TimeToKill;


		public:

			SimpleUserEntity();
			virtual ~SimpleUserEntity();

			void SetEntityKillTimer(DurationMS timeOut);

			// set connection
			virtual Result SetConnection(SharedPointerT<Net::Connection>&& pConn);

			// Release connection if has
			virtual void ReleaseConnection(const char* reason);

			// Get Connection
			inline const SharedPointerAtomicT<Net::Connection>& GetConnection() { return m_pConnection; }


			// Initialize entity to proceed new connection
			virtual Result InitializeEntity(EntityID newEntityID) override;

			// Close entity and clear transaction
			virtual Result TerminateEntity() override;

			// Process Connection event
			virtual Result ProcessConnectionEvent(const Net::ConnectionEvent& conEvent);

			// Process Message and release message after all processed
			Result ProcessMessageData(MessageDataPtr& pMsg);

			virtual void Heartbeat();

			// Run entity
			virtual Result TickUpdate(TimerAction* pAction = nullptr) override;


			//////////////////////////////////////////////////////////////////////////////////////////////////
			//
			//	Component manager
			//

			virtual Result PendingCloseTransaction(const char* reason) = 0;

			//////////////////////////////////////////////////////////////////////////////////////////////////
			//
			//	Component manager
			//

			ComponentManager& GetComponentManager() { return m_ComponentManger; }

			template< class ComponentType >
			ComponentType* GetComponent() { return m_ComponentManger.GetComponent<ComponentType>(); }

			// Get component with its type
			template< class ComponentType >
			const ComponentType* GetComponent() const { return m_ComponentManger.GetComponent<ComponentType>(); }

			////////////////////////////////////////////////////////////
			//
			//	User attributes
			//

			// Get Account ID
			AccountID GetAccountID() const { return m_AccountID; }

			// Set Account ID, this will update Account ID table
			virtual Result SetAccountID(AccountID accID);

			// Get Auth ticket
			AuthTicket GetAuthTicket() const { return m_AuthTicket; }

			// Set Auth ticket
			void SetAuthTicket(AuthTicket authTicket) { m_AuthTicket = authTicket; }

			Util::TimeStampTimer& GetTimeToKill() { return m_TimeToKill; }


			//////////////////////////////////////////////////////////////////////////////////////////////
			// Overriding IConnectionEventHandler
			virtual void OnConnectionEvent(Net::Connection* pConn, const Net::ConnectionEvent& evt) override;
			virtual Result OnRecvMessage(Net::Connection* pConn, MessageDataPtr& pMsg) override;
			virtual Result OnNetSyncMessage(Net::Connection* pConn) override;
			virtual Result OnNetSendReadyMessage(Net::Connection* pConn) override;

			virtual Result OnEventTask(ServerTaskEvent& eventTask) override;
		};


	} // namespace Svr
} // namespace SF

