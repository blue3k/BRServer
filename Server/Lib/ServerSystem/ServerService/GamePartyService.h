////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameParty Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "ServerSystem/ServerServiceBase.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/GamePartyIPolicy.h"



namespace BR
{
 	namespace Svr
	{
 		class GamePartyService : public ServerServiceBase
		{
 			private:
				Policy::IPolicyGameParty* GetPolicyGameParty() { return GetPolicy<Policy::IPolicyGameParty>(); }
			public:
				enum { ID_SERVICEPOLICY = Policy::IPolicyGameParty::ID_POLICY };
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
}; // namespace BR


