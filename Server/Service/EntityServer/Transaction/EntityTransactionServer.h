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

#include "SFTypedefs.h"
#include "Transaction/Transaction.h"
#include "Memory/MemoryPool.h"
#include "Transaction/ServerTransactionGeneric.h"

#include "Protocol/Message/EntityServerMsgClass.h"
#include "Protocol/Policy/EntityServerNetPolicy.h"
#include "Protocol/Message/ServerMsgClass.h"
#include "Protocol/Policy/ServerNetPolicy.h"


namespace SF {
namespace EntityServer {


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Entity command transaction
	//

	//// Server started
	//class TransServerStarted : public Svr::ServerStartedTrans< Svr::ServerEntity, Policy::NetSvrPolicyEntityServer, Message::Server::ServerConnectedC2SEvt, TransServerStarted>
	//{
	//private:
	//	typedef Svr::ServerStartedTrans< Svr::ServerEntity, Policy::NetSvrPolicyEntityServer, Message::Server::ServerConnectedC2SEvt, TransServerStarted> super;

	//public:
	//	TransServerStarted( MessageDataPtr &pIMsg ) : ServerStartedTrans(pIMsg) {}
	//	virtual ~TransServerStarted() {}

	//	virtual Result StartTransaction();
	//};



	
	/////////////////////////////////////////////////////////////////////////////
	//
	//	Entity command transaction
	//

	// Server started
	class TransGenericServerStarted : public Svr::ServerStartedTrans< Svr::ServerEntity, Policy::NetSvrPolicyEntityServer, Message::Server::ServerConnectedC2SEvt, TransGenericServerStarted>
	{
	private:

	public:
		TransGenericServerStarted( MessageDataPtr &pIMsg ) : ServerStartedTrans(pIMsg) {}
		virtual ~TransGenericServerStarted() {}
	};
	


};// namespace EntityServer 
};// namespace SF 

