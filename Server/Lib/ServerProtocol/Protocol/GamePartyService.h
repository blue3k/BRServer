////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : Generated
// 
// Description : GameParty Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "ServerService/ServerServiceBase.h"
#include "Net/SFNetDef.h"
#include "Protocol/GamePartyNetPolicy.h"



namespace SF
{
 	class GamePartyService : public ServerServiceBase
	{
 		public:
		// Constructor
		GamePartyService ( ServerServiceInformation* pService );

		// Cmd: Join party
		Result JoinPartyCmd( const uint64_t &InTransactionID, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer );
		// Cmd: Event for player left.
		Result LeavePartyCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID );
		// Cmd: Kick player
		Result KickPlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick );
		// C2S: Send chat message to server.
		Result ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const char* InChatMessage );
		// C2S: Quick Chatting message
		Result QuickChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const uint32_t &InQuickChatID );
		// Cmd: Start party game matching
		Result StartGameMatchCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const uint32_t &InMaxGamePlayers );
		// Cmd: Cancel game matching
		Result CancelGameMatchCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID );

	}; // class GamePartyService : public ServerServiceBase


}; // namespace SF


