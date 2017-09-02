////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : GameParty Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "ServerService/ServerServiceBase.h"
#include "Net/NetDef.h"
#include "Protocol/Policy/GamePartyNetPolicy.h"



namespace SF
{
 	namespace Svr
	{
 		class GamePartyService : public ServerServiceBase
		{
 			public:
			// Constructor
			GamePartyService ( ServerServiceInformation* pService );

			// Cmd: Join Party
			Result JoinPartyCmd( const TransactionID &InTransactionID, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer );
			// Cmd: Kick player
			Result LeavePartyCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: Kick player
			Result KickPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick );
			// C2S: Chatting message
			Result ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const char* InChatMessage );
			// C2S: Quick Chatting message
			Result QuickChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const uint32_t &InQuickChatID );
			// Cmd: Match
			Result StartGameMatchCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint32_t &InMaxGamePlayers );
			// Cmd: Match
			Result CancelGameMatchCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID );

		}; // class GamePartyService : public ServerServiceBase


	}; // namespace Svr
}; // namespace SF


