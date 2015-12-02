////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Entity server Transactions
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "ServerSystem/Transaction.h"
#include "Common/MemoryPool.h"
#include "ServerSystem/Transaction/ServerTransactionGeneric.h"

#include "Protocol/Message/EntityServerMsgClass.h"
#include "Protocol/Policy/EntityServerIPolicy.h"
#include "Protocol/Message/ServerMsgClass.h"
#include "Protocol/Policy/ServerIPolicy.h"


namespace BR {
namespace EntityServer {


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Entity command transaction
	//

	// Server started
	class TransServerStarted : public Svr::ServerStartedTrans< Svr::ServerEntity, Policy::ISvrPolicyEntityServer, Message::Server::ServerConnectedC2SEvt, TransServerStarted>
	{
	private:
		typedef Svr::ServerStartedTrans< Svr::ServerEntity, Policy::ISvrPolicyEntityServer, Message::Server::ServerConnectedC2SEvt, TransServerStarted> super;

	public:
		TransServerStarted( Message::MessageData* &pIMsg ) : ServerStartedTrans(pIMsg) {}
		virtual ~TransServerStarted() {}

		virtual HRESULT StartTransaction();
	};


	//// Server started
	//class TransEntityServerStarted : public Svr::ServerStartedTrans< Svr::ServerEntity, Policy::ISvrPolicyEntityServer, Message::Server::ServerConnectedC2SEvt, TransEntityServerStarted>
	//{
	//private:

	//public:
	//	TransEntityServerStarted( Message::MessageData* &pIMsg );
	//	virtual ~TransEntityServerStarted();

	//	// Start Transaction
	//	virtual HRESULT StartTransaction();

	//};


	
	/////////////////////////////////////////////////////////////////////////////
	//
	//	Entity command transaction
	//

	// Server started
	class TransGameServerStarted : public Svr::ServerStartedTrans< Svr::ServerEntity, Policy::ISvrPolicyEntityServer, Message::Server::ServerConnectedC2SEvt, TransGameServerStarted>
	{
	private:

	public:
		TransGameServerStarted( Message::MessageData* &pIMsg ) : ServerStartedTrans(pIMsg) {}
		virtual ~TransGameServerStarted() {}
	};
	


};// namespace EntityServer 
};// namespace BR 

