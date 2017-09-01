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
#include "Thread/Thread.h"
#include "Common/ClassUtil.h"
#include "Common/SystemSynchronization.h"
#include "Net/NetDef.h"
#include "Net/Connection.h"
#include "ServerSystem/MasterEntity.h"
#include "ServerSystem/SvrConst.h"



namespace BR {
namespace Svr {

	class Transaction;
	class TransactionResult;



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Remote server entity class
	//		- Will use with server type
	//

	class ServerEntity : public MasterEntity, public Net::IConnectionEventHandler
	{
	public:
		typedef NetClass ServerEntityClass;
		typedef MasterEntity super;
		

	private:

		Net::MsgQueue			m_RecvMessageQueue;

		// ServerID
		ServerID				m_ServerID;

		// Initial connect
		bool					m_bIsInitialConnect;

		// received server status
		BRCLASS_ATTRIBUTE(bool,ReceivedServerStatus);

		// public net address
		NetAddress				m_NetPublic;
		NetAddress				m_NetPrivate;

		// Server up time in UTC
		BRCLASS_ATTRIBUTE(TimeStampSec,ServerUpTime);
		
		// Connection to remote
		SharedPointerT<Net::IConnection>	m_pConnRemote;
		SharedPointerT<Net::IConnection>	m_pConnLocal;
		TimeStampMS							m_LocalConnectionRetryTime;
		DurationMS							m_LocalConnectionRetryWait;
		CriticalSection						m_ConnectionLock;

	protected:


		Result SetConnection(SharedPointerT<Net::IConnection> &destConn, Net::IConnection * pConn);
		Result UpdateConnection(Net::IConnection* pConn);


	public:
		ServerEntity( UINT uiTransQueueSize = Const::SERVER_TRANS_QUEUE , UINT TransResQueueSize = Const::SERVER_TRANS_QUEUE );
		virtual ~ServerEntity();

		// return about initial connection or not
		inline bool IsInitialConnection();


		// set connection
		Result SetRemoteConnection(Net::IConnection * pConn);
		Result SetLocalConnection(Net::IConnection * pConn);

		// Get Connection
		FORCEINLINE Net::IConnection* GetRemoteConnection()							{ return (Net::IConnection*)m_pConnRemote; }
		FORCEINLINE Net::IConnection* GetLocalConnection()							{ return (Net::IConnection*)m_pConnLocal; }
		Net::IConnection* GetConnection();
		void GetConnectionShared(SharedPointerT<Net::Connection>& outConn);

		template< class PolicyType >
		PolicyType* GetPolicy();

		// Get/Set Class Name
		inline ServerEntityClass GetRemoteClass() const;

		// Get public net address
		const NetAddress& GetPublicNetAddress() const;
		void SetPublicNetAddress( const NetAddress& netAddr );

		const NetAddress& GetPrivateNetAddress() const;
		void SetPrivateNetAddress(const NetAddress& netAddr);

		// Get ServerID
		ServerID GetServerID() const;
		void SetServerID( ServerID svrID );

		// Get Owner Server
		ServerEntity*				GetOwnerServer();

		// Initialize entity to proceed new connection
		virtual Result InitializeEntity( EntityID newEntityID ) override;

		// Close entity and clear transaction
		virtual Result TerminateEntity() override;

		// Called when this entity have a routed message
		//virtual Result OnRoutedMessage(Message::MessageData* &pMsg) override;

		// Process Message and release message after all processed
		virtual Result ProcessMessage(ServerEntity *pServerEntity, Net::IConnection *pCon, Message::MessageData* &pMsg ) override;

		// Process Connection event
		virtual Result ProcessConnectionEvent( const Net::IConnection::Event& conEvent );

		// Run entity
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;


		//////////////////////////////////////////////////////////////////////////////////////////////
		// Overriding IConnectionEventHandler
		virtual void OnConnectionEvent(Net::IConnection* pConn, const Net::IConnection::Event& evt) override;
		virtual Result OnRecvMessage(Net::IConnection* pConn, Message::MessageData* pMsg) override;
		virtual Result OnNetSyncMessage(Net::IConnection* pConn) override;
		virtual Result OnNetSendReadyMessage(Net::IConnection* pConn) override;

		/////////////////////////////////////////////////////////////////////////////////////
		// Event task handling
		virtual Result OnEventTask(const EventTask& eventTask) override;
	};


#include "ServerEntity.inl"



}; // namespace Svr
}; // namespace BR




