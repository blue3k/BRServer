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


#include "Common/Typedefs.h"
#include "Common/Thread.h"
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


		HRESULT SetConnection(SharedPointerT<Net::IConnection> &destConn, Net::IConnection * pConn);
		HRESULT UpdateConnection(Net::IConnection* pConn);


	public:
		ServerEntity( UINT uiTransQueueSize = Const::SERVER_TRANS_QUEUE , UINT TransResQueueSize = Const::SERVER_TRANS_QUEUE );
		virtual ~ServerEntity();

		// return about initial connection or not
		inline bool IsInitialConnection();


		// set connection
		HRESULT SetRemoteConnection(Net::IConnection * pConn);
		HRESULT SetLocalConnection(Net::IConnection * pConn);

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
		virtual HRESULT InitializeEntity( EntityID newEntityID ) override;

		// Close entity and clear transaction
		virtual HRESULT TerminateEntity() override;

		// Called when this entity have a routed message
		//virtual HRESULT OnRoutedMessage(Message::MessageData* &pMsg) override;

		// Process Message and release message after all processed
		virtual HRESULT ProcessMessage(ServerEntity *pServerEntity, Net::IConnection *pCon, Message::MessageData* &pMsg ) override;

		// Process Connection event
		virtual HRESULT ProcessConnectionEvent( const Net::IConnection::Event& conEvent );

		// Run entity
		virtual HRESULT TickUpdate(Svr::TimerAction *pAction = nullptr) override;


		//////////////////////////////////////////////////////////////////////////////////////////////
		// Overriding IConnectionEventHandler
		virtual void OnConnectionEvent(Net::IConnection* pConn, const Net::IConnection::Event& evt) override;
		virtual HRESULT OnRecvMessage(Net::IConnection* pConn, Message::MessageData* pMsg) override;
		virtual HRESULT OnNetSyncMessage(Net::IConnection* pConn) override;
		virtual HRESULT OnNetSendReadyMessage(Net::IConnection* pConn) override;

		/////////////////////////////////////////////////////////////////////////////////////
		// Event task handling
		virtual HRESULT OnEventTask(const Svr::EventTask& eventTask) override;
	};


#include "ServerEntity.inl"



}; // namespace Svr
}; // namespace BR




