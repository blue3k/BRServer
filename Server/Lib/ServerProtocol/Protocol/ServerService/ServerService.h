////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : Generated
// 
// Description : Server Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "ServerService/ServerServiceBase.h"
#include "Net/SFNetDef.h"
#include "Protocol/Policy/ServerNetPolicy.h"



namespace SF
{
 	class ServerService : public ServerServiceBase
	{
 		public:
		// Constructor
		ServerService ( ServerServiceInformation* pService );

		// Cmd: Generic failure message
		Result GenericFailureCmd( const uint64_t &InTransactionID );
		// C2S: Server Started or Connected
		Result ServerConnectedC2SEvt( const EntityID &InSenderEntityID, const uint32_t &InStartUpTime, const NetAddress &InPrivateAddress );

	}; // class ServerService : public ServerServiceBase


}; // namespace SF


