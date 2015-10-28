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
#include "Net/NetDef.h"
#include "Net/Connection.h"
#include "ServerSystem/MasterEntity.h"
#include "Common/ClassUtil.h"
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
		typedef BR::NetClass ServerEntityClass;
		

	private:
		// Connection to remote
		TimeStampMS								m_LocalConnectionRetryTime;
		DurationMS								m_LocalConnectionRetryWait;
		SharedPointerT<Net::IConnection>	m_pConnRemote;
		SharedPointerT<Net::IConnection>	m_pConnLocal;
		CriticalSection						m_ConnectionLock;

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
		
	protected:

		virtual MemoryAllocator& GetAllocator()			{ return STDAllocator::GetInstance(); }

		HRESULT SetConnection(SharedPointerT<Net::IConnection> &destConn, BR::Net::IConnection * pConn);
		HRESULT UpdateConnection(BR::Net::IConnection* pConn);


	public:
		ServerEntity( UINT uiTransQueueSize = Const::SERVER_TRANS_QUEUE , UINT TransResQueueSize = Const::SERVER_TRANS_QUEUE );
		virtual ~ServerEntity();

		// return about initial connection or not
		inline bool IsInitialConnection();


		// set connection
		HRESULT SetRemoteConnection(BR::Net::IConnection * pConn);
		HRESULT SetLocalConnection(BR::Net::IConnection * pConn);

		// Get Connection
		FORCEINLINE BR::Net::IConnection* GetRemoteConnection()							{ return (Net::IConnection*)m_pConnRemote; }
		FORCEINLINE BR::Net::IConnection* GetLocalConnection()							{ return (Net::IConnection*)m_pConnLocal; }
		FORCEINLINE BR::Net::IConnection* GetConnection();
		void GetConnectionShared(SharedPointerT<Net::Connection>& outConn);

		template< class PolicyType >
		PolicyType* GetPolicy();

		// Get/Set Class Name
		inline ServerEntityClass GetRemoteClass() const;

		// Get public net address
		FORCEINLINE const NetAddress& GetPublicNetAddress() const;
		FORCEINLINE void SetPublicNetAddress( const NetAddress& netAddr );

		FORCEINLINE const NetAddress& GetPrivateNetAddress() const;
		FORCEINLINE void SetPrivateNetAddress(const NetAddress& netAddr);

		// Get ServerID
		FORCEINLINE ServerID GetServerID() const;
		FORCEINLINE void SetServerID( ServerID svrID );

		// Get Owner Server
		ServerEntity*				GetOwnerServer();

		// Initialize entity to proceed new connection
		virtual HRESULT InitializeEntity( EntityID newEntityID ) override;

		// Close entity and clear transaction
		virtual HRESULT TerminateEntity() override;

				// Process result
		//virtual HRESULT ProcessMessageResult( Message::MessageData* &pMsg ) override;

		// Process Message and release message after all processed
		virtual HRESULT ProcessMessage( Net::IConnection *pCon, Message::MessageData* &pMsg );

		// Process Connection event
		virtual HRESULT ProcessConnectionEvent( const BR::Net::IConnection::Event& conEvent );

		// Run entity
		virtual HRESULT TickUpdate(Svr::TimerAction *pAction = nullptr) override;

		//////////////////////////////////////////////////////////////////////////////////////////////
		// Overriding IConnectionEventHandler
		virtual void OnConnectionEvent(Net::IConnection* pConn, const Net::IConnection::Event& evt) override;
		virtual HRESULT OnRecvMessage(Net::IConnection* pConn, Message::MessageData* pMsg) override;
		virtual HRESULT OnNetSyncMessage(Net::IConnection* pConn, Net::NetCtrlIDs netCtrlID) override;

		/////////////////////////////////////////////////////////////////////////////////////
		// Event task handling
		virtual HRESULT OnEventTask(const Svr::EventTask& eventTask) override;
	};


#include "ServerEntity.inl"



}; // namespace Svr
}; // namespace BR




