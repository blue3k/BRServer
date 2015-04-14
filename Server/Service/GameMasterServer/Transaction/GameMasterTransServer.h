////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : KyungKun Ko
//
// Description : Game master server Transactions
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "ServerSystem/Transaction.h"
#include "Common/MemoryPool.h"

#include "Protocol/Message/GameMasterServerMsgClass.h"
#include "Protocol/Policy/GameMasterServerIPolicy.h"
#include "Protocol/Policy/ServerIPolicy.h"
#include "Protocol/Message/ServerMsgClass.h"


namespace BR {
namespace GameMasterServer {


	/////////////////////////////////////////////////////////////////////////////
	//
	//	 command transaction
	//

	// Server started
	class TransServerStarted : public Svr::MessageTransaction< Svr::ServerEntity, Policy::ISvrPolicyGameMasterServer, Message::Server::ServerConnectedC2SEvt, TransServerStarted>
	{
	private:

	public:
		TransServerStarted( Message::MessageData* &pIMsg ) :MessageTransaction( pIMsg ) {}
		virtual ~TransServerStarted() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

	};



};// namespace GameMasterServer 
};// namespace BR 

