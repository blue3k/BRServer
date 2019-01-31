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
#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"

#include "Protocol/Message/EntityServerMsgClass.h"
#include "Protocol/Policy/EntityServerIPolicy.h"
#include "Protocol/Message/ServerMsgClass.h"
#include "Protocol/Policy/ServerIPolicy.h"

#include "Protocol/Message/ClusterServerMsgClass.h"
#include "Protocol/Policy/ClusterServerIPolicy.h"



namespace BR {
namespace LoginServer {


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Entity command transaction
	//



	
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

