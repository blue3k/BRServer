////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : ChatChannelManager Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "ServerService/ServerServiceBase.h"
#include "Net/SFNetDef.h"
#include "Protocol/Policy/ChatChannelManagerNetPolicy.h"



namespace SF
{
 	namespace Svr
	{
 		class ChatChannelManagerService : public ServerServiceBase
		{
 			public:
			// Constructor
			ChatChannelManagerService ( ServerServiceInformation* pService );

			// Cmd: Create a channel instance
			Result CreateChannelCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName, const char* InPasscode, const PlayerInformation &InCreator );
			// Cmd: Find a channel instance with name
			Result FindChannelCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName );
			// C2S: Notification that a chat channel instance has deleted
			Result ChatChannelDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount );

		}; // class ChatChannelManagerService : public ServerServiceBase


	}; // namespace Svr
}; // namespace SF


