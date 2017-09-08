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
#include "Thread/SystemSynchronization.h"
#include "Net/NetDef.h"
#include "Net/NetUtil.h"
#include "Net/Connection.h"
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

	class ServerEntity : public MasterEntity, public SF::Net::IConnectionEventHandler
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

		// public net address
		NetAddress				m_NetPublic;
		NetAddress				m_NetPrivate;

		// Server up time in UTC
		TimeStampSec m_ServerUpTime;
		
		// Connection to remote
		SharedPointerAtomicT<SF::Net::Connection>	m_pConnRemote;
		SharedPointerAtomicT<SF::Net::Connection>	m_pConnLocal;
		TimeStampMS							m_LocalConnectionRetryTime;
		DurationMS							m_LocalConnectionRetryWait;
		CriticalSection						m_ConnectionLock;

	protected:


		Result SetConnection(SharedPointerAtomicT<SF::Net::Connection> &destConn, SF::Net::Connection * pConn);
		Result UpdateConnection(SF::Net::Connection* pConn);


	public:
		ServerEntity( uint uiTransQueueSize = Const::SERVER_TRANS_QUEUE , uint TransResQueueSize = Const::SERVER_TRANS_QUEUE );
		virtual ~ServerEntity();

		bool GetReceivedServerStatus() { return m_ReceivedServerStatus; }
		void SetReceivedServerStatus(bool value) { m_ReceivedServerStatus = value; }

		TimeStampSec GetServerUpTime() { return m_ServerUpTime; }
		void SetServerUpTime(TimeStampSec value) { m_ServerUpTime = value; }

		// return about initial connection or not
		inline bool IsInitialConnection();


		// set connection
		Result SetRemoteConnection(SF::Net::Connection * pConn);
		Result SetLocalConnection(SF::Net::Connection * pConn);

		// Get Connection
		FORCEINLINE SF::Net::Connection* GetRemoteConnection()							{ return *m_pConnRemote; }
		FORCEINLINE SF::Net::Connection* GetLocalConnection()							{ return *m_pConnLocal; }
		SharedPointerT<SF::Net::Connection> GetConnection();
		void GetConnectionShared(SharedPointerT<SF::Net::Connection>& outConn);


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
		//virtual Result OnRoutedMessage(MessageDataPtr &pMsg) override;

		// Process Message and release message after all processed
		virtual Result ProcessMessage(ServerEntity *pServerEntity, Net::Connection* pCon, MessageDataPtr &pMsg ) override;

		// Process Connection event
		virtual Result ProcessConnectionEvent( const SF::Net::ConnectionEvent& conEvent );

		// Run entity
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;


		//////////////////////////////////////////////////////////////////////////////////////////////
		// Overriding IConnectionEventHandler
		virtual void OnConnectionEvent(SF::Net::Connection* pConn, const SF::Net::ConnectionEvent& evt) override;
		virtual Result OnRecvMessage(SF::Net::Connection* pConn, SharedPointerT<Message::MessageData>& pMsg) override;
		virtual Result OnNetSyncMessage(SF::Net::Connection* pConn) override;
		virtual Result OnNetSendReadyMessage(SF::Net::Connection* pConn) override;

		/////////////////////////////////////////////////////////////////////////////////////
		// Event task handling
		virtual Result OnEventTask(const ServerTaskEvent& eventTask) override;
	};



}; // namespace Svr
}; // namespace SF



#include "ServerEntity.inl"



