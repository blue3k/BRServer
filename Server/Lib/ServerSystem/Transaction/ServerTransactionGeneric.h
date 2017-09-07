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
//#include "Protocol/Policy/ServerNetNetPolicy.h"



namespace SF {
namespace Svr {


	/////////////////////////////////////////////////////////////////////////////
	//
	//	 command transaction
	//


	// Server started
	template< class ServerEntityType, class MessageType >
	class ServerStartedTrans : public MessageTransaction< ServerEntityType, MessageType>
	{
	private:
		typedef MessageTransaction< ServerEntityType, MessageType> super;

	public:
		ServerStartedTrans( IHeap& memMgr, MessageDataPtr &pIMsg ) : super( memMgr, pIMsg )
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
						AlreadyAddr = pAlreadyConn->GetRemoteInfo().PeerAddress;
					if( pMyConn )
						MyAddr = pMyConn->GetRemoteInfo().PeerAddress;

					svrTrace( Assert, "Error, Duplicated ServerID:{0} : Already from : {1}:{2}, New {3}:{4}", super::GetRouteContext().GetFrom().GetServerID(), pAlreadyConn->GetCID(), AlreadyAddr, pMyConn->GetCID(), MyAddr );
				}
			}

			super::GetMyOwner()->SetPrivateNetAddress(super::GetPrivateAddress());

			super::GetMyOwner()->SetServerUpTime(TimeStampSec(DurationSec(super::GetStartUpTime())) );

		Proc_End:

			super::CloseTransaction(hr);

			return hr;
		}
	};


	class GenericServerStartedTrans : public ServerStartedTrans< Svr::ServerEntity, Message::Server::ServerConnectedC2SEvt>
	{
	private:

	public:
		GenericServerStartedTrans(IHeap& memMgr, MessageDataPtr &pIMsg )
			: ServerStartedTrans< Svr::ServerEntity, Message::Server::ServerConnectedC2SEvt>(memMgr, pIMsg)
		{}
		virtual ~GenericServerStartedTrans() {}
	};



	// Entity Server started
	class EntityServerStartedTrans : public ServerStartedTrans< Svr::ServerEntity, Message::Server::ServerConnectedC2SEvt>
	{
	public:
		typedef ServerStartedTrans< Svr::ServerEntity, Message::Server::ServerConnectedC2SEvt> super;

	private:

	public:
		EntityServerStartedTrans(IHeap& memMgr, MessageDataPtr &pIMsg );
		virtual ~EntityServerStartedTrans();

		Result OnGetClusterMemberList(Svr::TransactionResult* pRes);

		// Start Transaction
		virtual Result StartTransaction();

	};


}	// namespace Svr 
}	// namespace SF 

