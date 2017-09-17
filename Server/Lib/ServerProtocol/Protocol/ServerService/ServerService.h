////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : Server Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "ServerService/ServerServiceBase.h"
#include "Net/NetDef.h"
#include "Protocol/Policy/ServerNetPolicy.h"



namespace SF
{
 	namespace Svr
	{
 		class ServerService : public ServerServiceBase
		{
 			public:
			// Constructor
			ServerService ( ServerServiceInformation* pService );

			// Cmd: 
			Result GenericFailureCmd( const TransactionID &InTransactionID );
			// C2S: Server Started or Connected
			Result ServerConnectedC2SEvt( const EntityID &InSenderEntityID, const uint32_t &InStartUpTime, const NetAddress &InPrivateAddress );

		}; // class ServerService : public ServerServiceBase


	}; // namespace Svr
}; // namespace SF


