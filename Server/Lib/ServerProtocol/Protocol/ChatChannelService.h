////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : Generated
// 
// Description : ChatChannel Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "ServerService/ServerServiceBase.h"
#include "Net/SFNetDef.h"
#include "Protocol/ChatChannelNetPolicy.h"



namespace SF
{
 	class ChatChannelService : public ServerServiceBase
	{
 		public:
		// Constructor
		ChatChannelService ( ServerServiceInformation* pService );

		// Cmd: Request to join chat channel
		Result JoinCmd( const uint64_t &InTransactionID, const char* InPasscode, const PlayerInformation &InJoiningPlayer );
		// Cmd: Leave chat channel
		Result LeaveCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID );
		// Cmd: Kick a player
		Result KickPlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick );
		// C2S: Sending a chatting message
		Result ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InSenderID, const char* InChatMessage );

	}; // class ChatChannelService : public ServerServiceBase


}; // namespace SF


