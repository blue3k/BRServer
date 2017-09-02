////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 Blue3k
// 
// Author : KyungKun Ko
//
// Description : Zone master server implementation
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Memory/MemoryPool.h"

#include "ServerEntity/ServerEntity.h"
#include "Transaction/Transaction.h"
#include "ServerEntity/ServerEntityManager.h"

#include "Protocol/Message/ServerMsgClass.h"
#include "Protocol/Policy/ServerNetNetPolicy.h"



namespace SF {
namespace Svr {


	/////////////////////////////////////////////////////////////////////////////
	//
	//	 command transaction
	//


	// Server started
	template< class ServerEntityType, class PolicyType, class MessageType, class TransactionType >
	class ServerStartedTrans : public MessageTransaction< ServerEntityType, PolicyType, MessageType, TransactionType>
	{
	private:
		typedef MessageTransaction< ServerEntityType, PolicyType, MessageType, TransactionType, MessageHandlerBufferSize> super;

	public:
		ServerStartedTrans( IMemoryManager& memMgr, Message::MessageData* &pIMsg ) : super( memMgr, pIMsg )
		{
		}

		virtual ~ServerStartedTrans()
		{
		}

		// Start Transaction
		virtual Result StartTransaction()
		{
			Result hr = ResultCode::SUCCESS;
			Svr::ServerEntity *pServerEntity = nullptr;

			svrChk( super::StartTransaction() );

			//GetMyOwner()->SetClusterID( GetClusterID() );
			super::GetMyOwner()->SetReceivedServerStatus(true);
			super::GetMyOwner()->SetEntityUID( EntityUID(super::GetRouteContext().GetFrom().GetServerID(), super::GetMyOwner()->GetEntityID() ) );

			// check about already registered server
			if( (Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity(super::GetRouteContext().GetFrom().GetServerID(), pServerEntity )) )
			{
				if( pServerEntity != super::GetMyOwner() )
				{
					Net::Connection *pAlreadyConn = pServerEntity->GetConnection();
					Net::Connection *pMyConn = super::GetMyOwner()->GetConnection();
					NetAddress AlreadyAddr(0), MyAddr(0);
					if( pAlreadyConn )
						AlreadyAddr = pAlreadyConn->GetConnectionInfo().Remote;
					if( pMyConn )
						MyAddr = pMyConn->GetConnectionInfo().Remote;

					svrTrace( Trace::TRC_ASSERT, "Error, Duplicated ServerID:{0} : Already from : {1}:{2}, New {3}:{4}", super::GetRouteContext().GetFrom().GetServerID(), pAlreadyConn->GetCID(), AlreadyAddr, pMyConn->GetCID(), MyAddr );
				}
			}

			super::GetMyOwner()->SetPrivateNetAddress(super::GetPrivateAddress());

			super::GetMyOwner()->SetServerUpTime(TimeStampSec(DurationSec(super::GetStartUpTime())) );

		Proc_End:

			super::CloseTransaction(hr);

			return hr;
		}
	};


	class GenericServerStartedTrans : public ServerStartedTrans< Svr::ServerEntity, Policy::NetSvrPolicyServer, Message::Server::ServerConnectedC2SEvt, GenericServerStartedTrans>
	{
	private:

	public:
		GenericServerStartedTrans(IMemoryManager& memMgr, Message::MessageData* &pIMsg ) : ServerStartedTrans< Svr::ServerEntity, Policy::NetSvrPolicyServer, Message::Server::ServerConnectedC2SEvt, GenericServerStartedTrans>(memMgr, pIMsg) {}
		virtual ~GenericServerStartedTrans() {}
	};



	// Entity Server started
	class EntityServerStartedTrans : public ServerStartedTrans< Svr::ServerEntity, Policy::NetSvrPolicyServer, Message::Server::ServerConnectedC2SEvt, EntityServerStartedTrans>
	{
	public:
		typedef ServerStartedTrans< Svr::ServerEntity, Policy::NetSvrPolicyServer, Message::Server::ServerConnectedC2SEvt, EntityServerStartedTrans> super;

	private:

	public:
		EntityServerStartedTrans( Message::MessageData* &pIMsg );
		virtual ~EntityServerStartedTrans();

		Result OnGetClusterMemberList(Svr::TransactionResult* pRes);

		// Start Transaction
		virtual Result StartTransaction();

	};


}	// namespace Svr 
}	// namespace SF 

