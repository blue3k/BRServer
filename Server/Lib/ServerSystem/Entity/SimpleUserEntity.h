////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Simple User Entity definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"
#include "Thread/Thread.h"
#include "Thread/SystemSynchronization.h"
#include "Net/NetDef.h"
#include "Net/Connection.h"
#include "Entity/SimpleEntity.h"


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


	public:

		SimpleUserEntity();
		virtual ~SimpleUserEntity();


		// set connection
		virtual Result SetConnection( SharedPointerT<Net::Connection>&& pConn );
		
		// Release connection if has
		virtual void ReleaseConnection();

		// Get Connection
		inline const SharedPointerAtomicT<Net::Connection>& GetConnection() { return m_pConnection; }


		// Initialize entity to proceed new connection
		virtual Result InitializeEntity( EntityID newEntityID ) override;

		// Close entity and clear transaction
		virtual Result TerminateEntity() override;

		// Process Connection event
		virtual Result ProcessConnectionEvent( const Net::ConnectionEvent& conEvent );

		// Process Message and release message after all processed
		Result ProcessMessageData(MessageDataPtr &pMsg);

		// Run entity
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;


		////////////////////////////////////////////////////////////
		//
		//	User attributes
		//

		// Get Account ID
		inline AccountID GetAccountID() const;

		// Set Account ID, this will update Account ID table
		virtual Result SetAccountID( AccountID accID );

		// Get Auth ticket
		inline AuthTicket GetAuthTicket() const;

		// Set Auth ticket
		inline void SetAuthTicket( AuthTicket authTicket );


		// Called when this entity have a routed message
		//virtual Result OnRoutedMessage(MessageDataPtr &pMsg) override;


		//////////////////////////////////////////////////////////////////////////////////////////////
		// Overriding IConnectionEventHandler
		virtual void OnConnectionEvent(Net::Connection* pConn, const Net::ConnectionEvent& evt) override;
		virtual Result OnRecvMessage(Net::Connection* pConn, MessageDataPtr& pMsg) override;
		virtual Result OnNetSyncMessage(Net::Connection* pConn) override;
		virtual Result OnNetSendReadyMessage(Net::Connection* pConn) override;

		virtual Result OnEventTask(const ServerTaskEvent& eventTask) override;
	};


#include "SimpleUserEntity.inl"



}; // namespace Svr
}; // namespace SF




