////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves
// 
// Author : KyungKun Ko
// Date : 01/19/2014
//
// Description : generic server Transactions
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/MemoryPool.h"

#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/Transaction.h"
#include "ServerSystem/ServerEntityManager.h"

#include "Protocol/Message/ServerMsgClass.h"
#include "Protocol/Policy/ServerNetPolicy.h"



namespace BR {
namespace Svr {


	/////////////////////////////////////////////////////////////////////////////
	//
	//	 command transaction
	//


	// Server started
	template< class ServerEntityType, class PolicyType, class MessageType, class TransactionType, size_t MessageHandlerBufferSize = sizeof(TransactionMessageHandlerType)*2 >
	class ServerStartedTrans : public MessageTransaction< ServerEntityType, PolicyType, MessageType, TransactionType, MessageHandlerBufferSize>
	{
	private:
		typedef MessageTransaction< ServerEntityType, PolicyType, MessageType, TransactionType, MessageHandlerBufferSize> super;

	public:
		ServerStartedTrans( Message::MessageData* &pIMsg ) : super( pIMsg )
		{
		}

		virtual ~ServerStartedTrans()
		{
		}

		// Start Transaction
		virtual HRESULT StartTransaction()
		{
			HRESULT hr = S_SYSTEM_OK;
			Svr::ServerEntity *pServerEntity = nullptr;

			svrChk( super::StartTransaction() );

			//GetMyOwner()->SetClusterID( GetClusterID() );
			super::GetMyOwner()->SetReceivedServerStatus(true);
			super::GetMyOwner()->SetEntityUID( EntityUID(super::GetRouteContext().GetFrom().GetServerID(), super::GetMyOwner()->GetEntityID() ) );

			// check about already registered server
			if( SUCCEEDED(Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity(super::GetRouteContext().GetFrom().GetServerID(), pServerEntity )) )
			{
				if( pServerEntity != super::GetMyOwner() )
				{
					Net::IConnection *pAlreadyConn = pServerEntity->GetConnection();
					Net::IConnection *pMyConn = super::GetMyOwner()->GetConnection();
					NetAddress AlreadyAddr(0), MyAddr(0);
					if( pAlreadyConn )
						AlreadyAddr = pAlreadyConn->GetConnectionInfo().Remote;
					if( pMyConn )
						MyAddr = pMyConn->GetConnectionInfo().Remote;

					svrTrace( Trace::TRC_ASSERT, "Error, Duplicated ServerID:{0} : Already from : {1}:{2}, New {3}:{4}", super::GetRouteContext().GetFrom().GetServerID(), pAlreadyConn->GetCID(), AlreadyAddr, pMyConn->GetCID(), MyAddr );
				}
				else
				{
					super::GetMyOwner()->SetPublicNetAddress(super::GetPublicAddress() );
				}
			}
			else
			{
				super::GetMyOwner()->SetPublicNetAddress(super::GetPublicAddress() );
			}

			super::GetMyOwner()->SetPrivateNetAddress(super::GetPrivateAddress());

			super::GetMyOwner()->SetServerUpTime(TimeStampSec(DurationSec(super::GetStartUpTime())) );

		Proc_End:

			super::CloseTransaction(hr);

			return hr;
		}
	};


	class GenericServerStartedTrans : public ServerStartedTrans< Svr::ServerEntity, Policy::ISvrPolicyServer, Message::Server::ServerConnectedC2SEvt, GenericServerStartedTrans>
	{
	private:

	public:
		GenericServerStartedTrans( Message::MessageData* &pIMsg ) : ServerStartedTrans< Svr::ServerEntity, Policy::ISvrPolicyServer, Message::Server::ServerConnectedC2SEvt, GenericServerStartedTrans>(pIMsg) {}
		virtual ~GenericServerStartedTrans() {}
	};



	// Entity Server started
	class EntityServerStartedTrans : public ServerStartedTrans< Svr::ServerEntity, Policy::ISvrPolicyServer, Message::Server::ServerConnectedC2SEvt, EntityServerStartedTrans>
	{
	public:
		typedef ServerStartedTrans< Svr::ServerEntity, Policy::ISvrPolicyServer, Message::Server::ServerConnectedC2SEvt, EntityServerStartedTrans> super;

	private:

	public:
		EntityServerStartedTrans( Message::MessageData* &pIMsg );
		virtual ~EntityServerStartedTrans();

		HRESULT OnGetClusterMemberList(Svr::TransactionResult* pRes);

		// Start Transaction
		virtual HRESULT StartTransaction();

	};


}	// namespace Svr 
}	// namespace BR 

