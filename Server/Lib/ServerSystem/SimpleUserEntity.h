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
		virtual HRESULT SetConnection( SharedPointerT<Net::Connection>&& pConn );
		
		// Release connection if has
		virtual void ReleaseConnection();

		// Get Connection
		inline Net::Connection* GetConnection();
		void GetConnectionShared(SharedPointerT<Net::Connection>& outConn);


		// Initialize entity to proceed new connection
		virtual HRESULT InitializeEntity( EntityID newEntityID ) override;

		// Close entity and clear transaction
		virtual HRESULT TerminateEntity() override;

		// Process Connection event
		virtual HRESULT ProcessConnectionEvent( const Net::IConnection::Event& conEvent );

		// Process Message and release message after all processed
		HRESULT ProcessMessage(Message::MessageData* &pMsg);

		// Run entity
		virtual HRESULT TickUpdate(Svr::TimerAction *pAction = nullptr) override;


		////////////////////////////////////////////////////////////
		//
		//	User attributes
		//

		// Get Account ID
		inline AccountID GetAccountID() const;

		// Set Account ID, this will update Account ID table
		virtual HRESULT SetAccountID( AccountID accID );

		// Get Auth ticket
		inline AuthTicket GetAuthTicket() const;

		// Set Auth ticket
		inline void SetAuthTicket( AuthTicket authTicket );


		//////////////////////////////////////////////////////////////////////////////////////////////
		// Overriding IConnectionEventHandler
		virtual void OnConnectionEvent(Net::IConnection* pConn, const Net::IConnection::Event& evt) override;
		virtual HRESULT OnRecvMessage(Net::IConnection* pConn, Message::MessageData* pMsg) override;
		virtual HRESULT OnNetSyncMessage(Net::IConnection* pConn, Net::NetCtrlIDs netCtrlID) override;

		virtual HRESULT OnEventTask(const Svr::EventTask& eventTask) override;
	};


#include "SimpleUserEntity.inl"



}; // namespace Svr
}; // namespace BR




