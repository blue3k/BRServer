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
	public:
		typedef MessageTransaction< ServerEntityType, PolicyType, MessageType, TransactionType, MessageHandlerBufferSize> super;

	private:

	public:
		ServerStartedTrans( Message::MessageData* &pIMsg ) :MessageTransaction( pIMsg )
		{
		}

		virtual ~ServerStartedTrans()
		{
		}

		// Start Transaction
		virtual HRESULT StartTransaction()
		{
			HRESULT hr = S_OK;
			Svr::ServerEntity *pServerEntity = nullptr;

			svrChk( super::StartTransaction() );

			//GetMyOwner()->SetClusterID( GetClusterID() );
			GetMyOwner()->SetReceivedServerStatus(true);
			GetMyOwner()->SetEntityUID( EntityUID( GetRouteContext().GetFrom().GetServerID(), GetMyOwner()->GetEntityID() ) );

			// check about already registered server
			if( SUCCEEDED(Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity( GetRouteContext().GetFrom().GetServerID(), pServerEntity )) )
			{
				if( pServerEntity != GetMyOwner() )
				{
					Net::IConnection *pAlreadyConn = pServerEntity->GetConnection();
					Net::IConnection *pMyConn = GetMyOwner()->GetConnection();
					NetAddress AlreadyAddr(0), MyAddr(0);
					if( pAlreadyConn )
						AlreadyAddr = pAlreadyConn->GetConnectionInfo().Remote;
					if( pMyConn )
						MyAddr = pMyConn->GetConnectionInfo().Remote;

					svrTrace( Trace::TRC_ASSERT, "Error, Duplicated ServerID:%0% : Already from : %1%:%2%, New %3%:%4%", GetRouteContext().GetFrom().GetServerID(), pAlreadyConn->GetCID(), AlreadyAddr, pMyConn->GetCID(), MyAddr );
				}
				else
				{
					GetMyOwner()->SetPublicNetAddress( GetPublicAddress() );
				}
			}
			else
			{
				GetMyOwner()->SetPublicNetAddress( GetPublicAddress() );
			}

			GetMyOwner()->SetPrivateNetAddress(GetPrivateAddress());

			GetMyOwner()->SetServerUpTime(TimeStampSec(DurationSec(GetStartUpTime())) );

		Proc_End:

			CloseTransaction(hr);

			return hr;
		}
	};


	class GenericServerStartedTrans : public Svr::ServerStartedTrans< Svr::ServerEntity, Policy::ISvrPolicyServer, Message::Server::ServerConnectedC2SEvt, GenericServerStartedTrans>
	{
	private:

	public:
		GenericServerStartedTrans( Message::MessageData* &pIMsg ) :ServerStartedTrans(pIMsg) {}
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

