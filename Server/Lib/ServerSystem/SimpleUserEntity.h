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


#include "Common/Typedefs.h"
#include "Common/Thread.h"
#include "Common/SystemSynchronization.h"
#include "Net/NetDef.h"
#include "Net/Connection.h"
#include "ServerSystem/SimpleEntity.h"


namespace BR {
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
		SharedPointerT<Net::Connection>	m_pConnection;
		BR::CriticalSection m_ConnectionLock;

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
		inline Net::Connection* GetConnection();
		void GetConnectionShared(SharedPointerT<Net::Connection>& outConn);


		// Initialize entity to proceed new connection
		virtual Result InitializeEntity( EntityID newEntityID ) override;

		// Close entity and clear transaction
		virtual Result TerminateEntity() override;

		// Process Connection event
		virtual Result ProcessConnectionEvent( const Net::IConnection::Event& conEvent );

		// Process Message and release message after all processed
		Result ProcessMessageData(Message::MessageData* &pMsg);

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
		//virtual Result OnRoutedMessage(Message::MessageData* &pMsg) override;


		//////////////////////////////////////////////////////////////////////////////////////////////
		// Overriding IConnectionEventHandler
		virtual void OnConnectionEvent(Net::IConnection* pConn, const Net::IConnection::Event& evt) override;
		virtual Result OnRecvMessage(Net::IConnection* pConn, Message::MessageData* pMsg) override;
		virtual Result OnNetSyncMessage(Net::IConnection* pConn) override;
		virtual Result OnNetSendReadyMessage(Net::IConnection* pConn) override;

		virtual Result OnEventTask(const EventTask& eventTask) override;
	};


#include "SimpleUserEntity.inl"



}; // namespace Svr
}; // namespace BR




